#pragma once
#ifndef MEMORY_CELL_ALLOCATOR_H
#define MEMORY_CELL_ALLOCATOR_H

#include <stddef.h>

#include "data/bytes.h"


namespace Memory {
	using Data::Bytes;
	using Data::Byte;

	struct SimplePoolAllocator {
		struct Block {

		};

		 SimplePoolAllocator();
		~SimplePoolAllocator();
	};

	Bytes allocate(SimplePoolAllocator& alloc, size_t size);
	void  release(SimplePoolAllocator& alloc, Bytes mem);
	void  reset(SimplePoolAllocator& alloc);
	void  drop(SimplePoolAllocator& alloc);

} // namespace Memory


#endif // MEMORY_CELL_ALLOCATOR_H