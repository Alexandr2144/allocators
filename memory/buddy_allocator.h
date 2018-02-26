#pragma once
#ifndef MEMORY_BUDDY_ALLOCATOR_H
#define MEMORY_BUDDY_ALLOCATOR_H

#include "data/linked_list.h"
#include "data/bytes.h"


namespace Memory {
	using Data::Bytes;
	using Data::Byte;

	using Data::SingleListNode;
	using Data::SingleListHead;

	struct BuddyAllocator {
		struct FreeSpace {
			SingleListNode node;
		};
		struct BlockInfo {
			SingleListNode node;
			SingleListHead free[8];
			Byte usedMask[8];
		};

		SingleListHead head;

		 BuddyAllocator();
		~BuddyAllocator();
	};

	Bytes allocate(BuddyAllocator& alloc, size_t size);
	void  release(BuddyAllocator& alloc, Bytes bytes);

	void drop(BuddyAllocator& alloc);

} // namespace Memory


#endif // MEMORY_BUDDY_ALLOCATOR_H