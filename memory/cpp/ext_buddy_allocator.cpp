#include "memory/ext_buddy_allocator.h"

#include "native/memory.h"


namespace Memory {
	// ------------------------------------------------------------------------
	Bytes allocate(ExtBuddyAllocator& alloc, size_t size)
	{
		if (size < (alloc.small.blocksize >> alloc.small_quota)) {
			return allocate(alloc.small, size);
		}
		else if (size < (alloc.medium.blocksize >> alloc.medium_quota)) {
			return allocate(alloc.medium, size);
		}
		else if (size < (alloc.large.blocksize >> alloc.large_quota)) {
			return allocate(alloc.large, size);
		}
		else {
			return allocate(alloc.huge, size);
		}
	}

	// ------------------------------------------------------------------------
	void release(ExtBuddyAllocator& alloc, Bytes bytes)
	{
		using HugeBlockInfo = BuddyAllocator::HugeBlockInfo;
		using BlockInfo = BuddyAllocator::BlockInfo;

		size_t size = bytes.end - bytes.begin;
		if (size < alloc.huge.blocksize / 2) {
			auto block = (BlockInfo*)bytes.begin;
			release(*block->owner, bytes);
		}
		else {
			release(alloc.huge, bytes);
		}
	}

	// ------------------------------------------------------------------------
	void drop(ExtBuddyAllocator& alloc)
	{
		drop(alloc.small);
		drop(alloc.medium);
		drop(alloc.large);
		drop(alloc.huge);
	}

	// ------------------------------------------------------------------------
	void move(ExtBuddyAllocator& from, ExtBuddyAllocator& to)
	{
		to = from;
		from = ExtBuddyAllocator();
	}

	// ------------------------------------------------------------------------
	ExtBuddyAllocator::ExtBuddyAllocator()
		: small  ( 1*Native::page_size())
		, medium ( 4*Native::page_size())
		, large  (16*Native::page_size())
		, huge   (64*Native::page_size())
		, small_quota  (3)
		, medium_quota (3)
		, large_quota  (3)
	{
	}

} // namespace Memory