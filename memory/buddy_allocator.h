#pragma once
#ifndef MEMORY_BUDDY_ALLOCATOR_H
#define MEMORY_BUDDY_ALLOCATOR_H

#include "core/data/linked_list.h"
#include "core/data/bytes.h"


namespace Memory {
	using Data::Bytes;
	using Data::Byte;

	using Data::SingleListNode;
	using Data::DoubleListNode;
	using Data::SingleListHead;
	using Data::DoubleListHead;

	struct BuddyAllocator {
		static const size_t LEVEL_MAX = 7;

		struct FreeSpace {
			SingleListNode node;
		};
		struct BlockInfo {
			DoubleListNode node;
			SingleListHead free[8]; // LEVEL_MAX + 1
			uint64_t usage[4];      // depend of LEVEL_MAX

			BuddyAllocator* owner;
		};
		struct HugeBlockInfo {
			DoubleListNode node;
			BuddyAllocator* owner;
		};

		size_t blocksize;
		int powerbase;

		DoubleListHead head;

		 BuddyAllocator(size_t blockSize);
		 BuddyAllocator();
		~BuddyAllocator();
	};

	Bytes allocate(BuddyAllocator& alloc, size_t size);
	void release(BuddyAllocator& alloc, Bytes bytes);
	void drop(BuddyAllocator& alloc);

	void move(BuddyAllocator& from, BuddyAllocator& to);

} // namespace Memory


#endif // MEMORY_BUDDY_ALLOCATOR_H