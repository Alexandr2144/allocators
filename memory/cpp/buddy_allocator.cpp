#include "memory/buddy_allocator.h"

#include "core/math/basic.h"
#include "core/tools/memory.h"
#include "core/tools/asserts.h"


namespace Memory {
	using HugeBlockInfo = BuddyAllocator::HugeBlockInfo;
	using BlockInfo     = BuddyAllocator::BlockInfo;
	using FreeSpace     = BuddyAllocator::FreeSpace;


	// ------------------------------------------------------------------------
	static size_t getSizeByLevel(size_t level, size_t blocksize)
	{
		M_ASSERT(level <= BuddyAllocator::LEVEL_MAX);
		return (blocksize >> (BuddyAllocator::LEVEL_MAX - level));
	}

	// ------------------------------------------------------------------------
	static size_t getLevelBySize(size_t size, size_t powerbase)
	{
		int power = Data::log2((int)(2 * size - 1));
		return (size_t)Data::max(0, power - (int)powerbase);
	}

	// ------------------------------------------------------------------------
	static int getUsedMaskWindowId(size_t size, int powerbase)
	{
	}

	// ------------------------------------------------------------------------
	static void setUsageStateU(BlockInfo* blockInfo, Byte* addr, size_t level, int powerbase, bool isUsed)
	{
		Byte* base = (Byte*)Native::page_begin_ptr(addr);

		size_t power = level + powerbase;
		size_t upIndex = (addr - base) >> power;
		if (level == 5) {
			blockInfo->usage[upIndex] = isUsed ? uint64_t(-1) : 0;
		}
		else if (level == 6) {
			M_ASSERT(upIndex == 1);
			blockInfo->usage[2] = isUsed ? uint64_t(-1) : 0;
			blockInfo->usage[3] = isUsed ? uint64_t(-1) : 0;
		}
	}

	// ------------------------------------------------------------------------
	static void setUsageStateD(BlockInfo* blockInfo, Byte* addr, size_t level, int powerbase, bool isUsed)
	{
		Byte* base = (Byte*)Native::page_begin_ptr(addr);

		size_t idx0 = (addr - base) >> powerbase;
		size_t window = idx0 >> 6;

		size_t offset = idx0 - (window << 6);
		size_t bitmask = (1 << (1 << level)) - 1;

		if (!isUsed)
			 blockInfo->usage[window] &= ~(bitmask << offset);
		else blockInfo->usage[window] |= bitmask << offset;
	}

	// ------------------------------------------------------------------------
	static void setUsageState(BlockInfo* blockInfo, Byte* addr, size_t level, int powerbase, bool isUsed)
	{
		M_ASSERT(level != 7);

		if (level > 4)
			 setUsageStateU(blockInfo, addr, level, powerbase, isUsed);
		else setUsageStateD(blockInfo, addr, level, powerbase, isUsed);
	}

	// ------------------------------------------------------------------------
	static Bytes allocateFromBlock(BlockInfo* blockInfo, size_t level, size_t size)
	{
		auto& free = blockInfo->free;

		size_t splitSize = size;
		size_t splitLevel = level;
		while (Data::isEmpty(free[splitLevel])) {
			if (splitLevel++ == 7) {
				return Data::noBytes;
			}
			splitSize <<= 1;
		}

		Byte* memForSplitPtr = (Byte*)Data::head(free[splitLevel]);
		Bytes memForSplit = Data::toBytes(memForSplitPtr, splitSize);
		free[splitLevel] = Data::removeHead(free[splitLevel]);

		if (splitLevel != level) {
			Bytes memAlloc;
			while (splitLevel != level) {
				splitLevel -= 1;
				splitSize >>= 1;

				Bytes memSplit = Data::split(memForSplit, &memAlloc, splitSize);
				FreeSpace* split = (FreeSpace*)memSplit.begin;

				free[splitLevel] = Data::addToHead(free[splitLevel], &split->node);
			}
			return memAlloc;
		}
		else {
			return memForSplit;
		}
	}

	// ------------------------------------------------------------------------
	static BlockInfo* initBlockInfo(BuddyAllocator& alloc, Bytes memBlock)
	{
		BlockInfo* pInfo = (BlockInfo*)memBlock.begin;
		Data::byteset(Data::toBytes(pInfo), 0);

		pInfo->free[7] = SingleListHead{ (SingleListNode*)pInfo };
		pInfo->owner = &alloc;
		return pInfo;
	}

	// ------------------------------------------------------------------------
	static void createNewBlock(BuddyAllocator& alloc)
	{
		size_t binfoLevel = getLevelBySize(sizeof(BlockInfo), alloc.powerbase);
		size_t binfoSize = getSizeByLevel(binfoLevel, alloc.blocksize);

		Bytes memBlock = Tools::virtual_alloc(alloc.blocksize, Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);
		BlockInfo* pInfo = initBlockInfo(alloc, memBlock);

		Bytes memInfo = allocateFromBlock(pInfo, binfoLevel, binfoSize);
		alloc.head = Data::addToHead(alloc.head, &pInfo->node);

		M_ASSERT((Byte*)pInfo == memInfo.begin);
		setUsageState(pInfo, (Byte*)pInfo, binfoLevel, alloc.powerbase, true);
	}

	// ------------------------------------------------------------------------
	Bytes allocateFromNode(BuddyAllocator& alloc, DoubleListNode* blockInfoNode, size_t level)
	{
		BlockInfo* blockInfo = (BlockInfo*)blockInfoNode;

		size_t memSize = getSizeByLevel(level, alloc.blocksize);
		Bytes memory = allocateFromBlock(blockInfo, level, memSize);
		if (!Data::isNull(memory)) {
			setUsageState(blockInfo, memory.begin, level, alloc.powerbase, true);
		}
		return memory;
	}

	// ------------------------------------------------------------------------
	static Bytes hugeAlloc(BuddyAllocator& alloc, size_t size)
	{
		Bytes memBlock = Tools::virtual_alloc(size + sizeof(HugeBlockInfo), Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);
		HugeBlockInfo* pInfo = (HugeBlockInfo*)memBlock.begin;
		Data::byteset(Data::toBytes(pInfo), 0);

		pInfo->owner = &alloc;
		Data::addToHead(alloc.head, &pInfo->node);
		return memBlock;
	}

	// ------------------------------------------------------------------------
	Bytes allocate(BuddyAllocator& alloc, size_t size)
	{
		if (size == 0) return Data::noBytes;

		if (size > alloc.blocksize / 2) {
			return hugeAlloc(alloc, size);
		}

		DoubleListHead list = alloc.head;
		size_t level = getLevelBySize(size, alloc.powerbase);

		while (!Data::isEmpty(list)) {
			Bytes memory = allocateFromNode(alloc, Data::head(list), level);
			if (!Data::isNull(memory)) return memory;

			list = Data::next(list);
		}

		createNewBlock(alloc);
		return allocateFromNode(alloc, Data::head(alloc.head), level);
	}

	// ------------------------------------------------------------------------
	static void hugeRelease(BuddyAllocator& alloc, Bytes bytes)
	{
		DoubleListNode* node = (DoubleListNode*)bytes.begin;
		Data::remove(alloc.head, node);
		Tools::virtual_free(bytes);
	}

	// ------------------------------------------------------------------------
	static bool isFreeU(BlockInfo* blockInfo, size_t index, size_t level)
	{
		M_ASSERT(level > 4 && level != 7);

		if (level == 5) {
			return (blockInfo->usage[index] == 0);
		}
		else if (level == 6 && index == 1) {
			return (blockInfo->usage[2] == 0) && (blockInfo->usage[3] == 0);
		}
		else {
			return false;
		}
	}

	// ------------------------------------------------------------------------
	static bool isFreeD(BlockInfo* blockInfo, size_t index, size_t level)
	{
		size_t idx0 = index << level;

		size_t window = idx0 >> 6;
		size_t offset = idx0 - (window << 6);
		size_t bitmask = (1 << (1 << level)) - 1;
		size_t mask = bitmask << offset;

		return !(blockInfo->usage[window] & mask);
	}

	// ------------------------------------------------------------------------
	static bool isFree(BlockInfo* blockInfo, size_t index, size_t level)
	{
		if (level > 4)
			 return isFreeU(blockInfo, index, level);
		else return isFreeD(blockInfo, index, level);
	}

	// ------------------------------------------------------------------------
	void release(BuddyAllocator& alloc, Bytes bytes)
	{
		if (Data::isNull(bytes)) return;

		size_t size = bytes.end - bytes.begin;
		if (size > alloc.blocksize / 2) {
			hugeRelease(alloc, bytes);
			return;
		}

		auto blockInfo = (BlockInfo*)Native::page_begin_ptr(bytes.begin);
		Byte* base = (Byte*)Native::page_begin_ptr(blockInfo);

		size_t level = getLevelBySize(size, alloc.powerbase);
		size_t power = level + alloc.powerbase;

		size_t index = (bytes.begin - base) >> power;
		while (isFree(blockInfo, index ^ 1, level)) {
			M_ASSERT(level != 7);
			index >>= 1;
			level += 1;
		}

		Byte* addr = base + (index << (level + alloc.powerbase));
		FreeSpace* freeSpace = (FreeSpace*)addr;

		blockInfo->free[level] = Data::addToHead(blockInfo->free[level], &freeSpace->node);
		setUsageState(blockInfo, addr, level, alloc.powerbase, false);
	}

	// ------------------------------------------------------------------------
	void drop(BuddyAllocator& alloc)
	{
		DoubleListHead list = alloc.head;
		while (!Data::isEmpty(list)) {
			DoubleListNode* toRemove = Data::head(list);
			list = Data::next(list);

			void* block = Native::page_begin_ptr(toRemove);
			Tools::virtual_free(Data::toBytes(block, 1));
		}
	}

	// ------------------------------------------------------------------------
	void move(BuddyAllocator& from, BuddyAllocator& to)
	{
		to = from;
		from = BuddyAllocator();
	}

	// ------------------------------------------------------------------------
	int calcMinSizePower(size_t blocksize, size_t orderMax)
	{
		return Data::log2((int)blocksize) - (int)orderMax;
	}

	// ------------------------------------------------------------------------
	BuddyAllocator::BuddyAllocator(size_t blockSize)
		: blocksize(blockSize)
		, powerbase(calcMinSizePower(blockSize, LEVEL_MAX))
		, head{ nullptr }
	{
	}

	// ------------------------------------------------------------------------
	BuddyAllocator::BuddyAllocator()
		: blocksize(Native::page_size())
		, powerbase(calcMinSizePower(Native::page_size(), LEVEL_MAX))
		, head{ nullptr }
	{
	}

	// ------------------------------------------------------------------------
	BuddyAllocator::~BuddyAllocator()
	{
		if (!Data::isEmpty(head)) drop(*this);
	}

} // namespace Memory