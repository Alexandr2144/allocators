#pragma once
#ifndef SIMPLE_LINEAR_ALLOCATOR_H
#define SIMPLE_LINEAR_ALLOCATOR_H

#include <stddef.h>

#include "core/data/linked_list.h"
#include "core/data/bytes.h"


namespace Memory {
	using Data::Bytes;
	using Data::Byte;

	using Data::SingleListNode;
	using Data::SingleListHead;
	using Data::SingleListTail;

	struct SimpleLinearAllocator {
		struct Block {
			SingleListNode node;
		};
		SingleListHead head;
		SingleListTail tail;

		Byte* begin;
		Byte* end;

		 SimpleLinearAllocator();
		~SimpleLinearAllocator();
	};

   size_t memsize(SimpleLinearAllocator& alloc, Byte* ptr);

	Bytes allocate(SimpleLinearAllocator& alloc, size_t size);
	void drop(SimpleLinearAllocator& alloc);

	void move(SimpleLinearAllocator& from, SimpleLinearAllocator& to);

} // namespace Memory


#endif // SIMPLE_LINEAR_ALLOCATOR_H