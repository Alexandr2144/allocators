#pragma once
#ifndef MEMORY_EXT_BUDDY_ALLOCATOR_H
#define MEMORY_EXT_BUDDY_ALLOCATOR_H

#include "memory/buddy_allocator.h"


namespace Memory {
	struct ExtBuddyAllocator {
		BuddyAllocator small;
		BuddyAllocator medium;
		BuddyAllocator large;
		BuddyAllocator huge;

		size_t small_quota;
		size_t medium_quota;
		size_t large_quota;

		ExtBuddyAllocator();
	};

	Bytes allocate(ExtBuddyAllocator& alloc, size_t size);
	void release(ExtBuddyAllocator& alloc, Bytes bytes);
	void drop(ExtBuddyAllocator& alloc);

	void move(ExtBuddyAllocator& from, ExtBuddyAllocator& to);

} // namespace Memory


#endif // MEMORY_EXT_BUDDY_ALLOCATOR_H