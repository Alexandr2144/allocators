#include "memory/simple_pool_allocator.h"

#include "native/memory.h"


namespace Memory {
	// ------------------------------------------------------------------------
	Bytes allocate(SimplePoolAllocator& alloc, size_t size)
	{
		return Data::noBytes;
	}

	// ------------------------------------------------------------------------
	void release(SimplePoolAllocator& alloc, Bytes mem)
	{

	}

	// ------------------------------------------------------------------------
	void reset(SimplePoolAllocator& alloc)
	{

	}

	// ------------------------------------------------------------------------
	void drop(SimplePoolAllocator& alloc)
	{

	}

	// ------------------------------------------------------------------------
	SimplePoolAllocator::SimplePoolAllocator()
	{
		reset(*this);
	}

	// ------------------------------------------------------------------------
	SimplePoolAllocator::~SimplePoolAllocator()
	{
		drop(*this);
	}
} // namespace Memory