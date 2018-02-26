#include "buddy_allocator.h"

#include "native/memory.h"
#include "data/math.h"


namespace Memory {
	static const int order_delta = Data::log2((int)Native::page_size()) - 7;

	using FreeSpace = BuddyAllocator::FreeSpace;
	using BlockInfo = BuddyAllocator::BlockInfo;

	// ------------------------------------------------------------------------
	static void markSpaceAsUsed(BlockInfo* blockInfo, Byte* addr, int order)
	{
		size_t sizeScale = order + order_delta;
		size_t usedId = (addr - (Byte*)Native::page_begin_ptr(addr)) >> sizeScale;
		blockInfo->usedMask[order] |= 1 << usedId;
	}

	// ------------------------------------------------------------------------
	static size_t getSizeByOrder(int order)
	{
		return (Native::page_size() >> (7 - order));
	}

	// ------------------------------------------------------------------------
	static int getOrderBySize(size_t size)
	{
		int order = Data::log2((int)(2*size - 1)) - order_delta;
		return Data::max(0, order);
	}

	// ------------------------------------------------------------------------
	static Bytes hugeAlloc(BuddyAllocator& alloc, size_t size)
	{
		return Data::noBytes;
	}

	// ------------------------------------------------------------------------
	static Bytes allocate(BlockInfo* blockInfo, int order, size_t size)
	{
		SingleListHead list = blockInfo->free[order];
		SingleListNode* node = Data::head(list);
		if (node != nullptr) {			
			blockInfo->free[order] = Data::removeHead(list);
			return Data::toBytes((Byte*)node, size);
		}
		if (order == 7) {
			return Data::noBytes;
		}

		Bytes memSpace = allocate(blockInfo, order + 1, 2*size);

		Bytes allocMemory;
		Bytes freeMemory = Data::split(memSpace, &allocMemory, size);

		FreeSpace* pSpace = (FreeSpace*)freeMemory.begin;
		blockInfo->free[order] = Data::addToHead(list, &pSpace->node);
		return allocMemory;
	}

	// ------------------------------------------------------------------------
	static void createNewBlock(BuddyAllocator& alloc)
	{
		Bytes memBlock = Native::virtual_alloc(Native::page_size(), Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);

		FreeSpace* pSpace = (FreeSpace*)memBlock.begin;
		pSpace->node = SingleListNode{ nullptr };

		BlockInfo info;
		for (int i = 0; i < 8; ++i) {
			info.free[i] = SingleListHead{ nullptr };
			info.usedMask[i] = 0;
		}
		info.free[7] = SingleListHead{ &pSpace->node };

		int order = getOrderBySize(sizeof(BlockInfo));
		Bytes memBlockInfo = allocate(&info, order, getSizeByOrder(order));
		Data::copy(Data::toBytes(&info), memBlockInfo);

		BlockInfo* pInfo = (BlockInfo*)memBlockInfo.begin;
		alloc.head = Data::addToHead(alloc.head, &pInfo->node);

		markSpaceAsUsed(pInfo, (Byte*)pInfo, order);
	}

	// ------------------------------------------------------------------------
	Bytes allocate(BuddyAllocator& alloc, size_t size)
	{
		if (size > Native::page_size()) {
			return hugeAlloc(alloc, size);
		}

		SingleListHead list = alloc.head;
		int order = getOrderBySize(size);

		while (!Data::isEmpty(list)) {
			SingleListNode* blockInfoNode = Data::head(list);
			BlockInfo* blockInfo = (BlockInfo*)blockInfoNode;

			Bytes memory = allocate(blockInfo, order, getSizeByOrder(order));
			if (!Data::isNull(memory)) {
				markSpaceAsUsed(blockInfo, memory.begin, order);
				return memory;
			}
		}

		createNewBlock(alloc);
		SingleListNode* blockInfoNode = Data::head(alloc.head);
		Bytes memory = allocate((BlockInfo*)blockInfoNode, order, getSizeByOrder(order));
		markSpaceAsUsed((BlockInfo*)blockInfoNode, memory.begin, order);
		return memory;
	}

	// ------------------------------------------------------------------------
	void release(BuddyAllocator& alloc, Bytes bytes)
	{

	}

	// ------------------------------------------------------------------------
	void drop(BuddyAllocator& alloc)
	{

	}

	// ------------------------------------------------------------------------
	BuddyAllocator::BuddyAllocator()
		: head{ nullptr }
	{
	}

	// ------------------------------------------------------------------------
	BuddyAllocator::~BuddyAllocator()
	{
		if (!Data::isEmpty(head)) drop(*this);
	}

} // namespace Memory