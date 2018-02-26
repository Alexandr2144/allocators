#include "memory/simple_linear_allocator.h"

#include "native/memory.h"


namespace Memory {
	using Block = SimpleLinearAllocator::Block;

	// ------------------------------------------------------------------------
	Bytes hugeAlloc(SimpleLinearAllocator& alloc, size_t size)
	{
		Bytes memBlock = Native::virtual_alloc(size, Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);
		Block* block = (Block*)memBlock.begin;

		if (Data::isEmpty(alloc.tail)) {
			alloc.tail = SingleListTail{ &block->node };
		}

		alloc.head = Data::addToHead(alloc.head, &block->node);
		return memBlock;
	}

	// ------------------------------------------------------------------------
	void addFirstBlock(SimpleLinearAllocator& alloc)
	{
		Bytes memBlock = Native::virtual_alloc(Native::page_size(), Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);
		Block* block = (Block*)memBlock.begin;

		alloc.end = memBlock.end;
		alloc.begin = memBlock.begin + sizeof(Block);
		alloc.head = SingleListHead{ &block->node };
		alloc.tail = SingleListTail{ &block->node };
	}

	// ------------------------------------------------------------------------
	void addNewBlock(SimpleLinearAllocator& alloc)
	{
		Bytes memBlock = Native::virtual_alloc(Native::page_size(), Native::PAGE_PROT_READ | Native::PAGE_PROT_WRITE);
		Block* block = (Block*)memBlock.begin;

		alloc.end = memBlock.end;
		alloc.begin = memBlock.begin + sizeof(Block);
		alloc.tail = Data::addToNonEmptyTail(alloc.tail, &block->node);
	}

	// ------------------------------------------------------------------------
	Bytes allocate(SimpleLinearAllocator& alloc, size_t size)
	{
		if (size > Native::page_size() - sizeof(Block)) {
			return hugeAlloc(alloc, size);
		}

		if (isEmpty(alloc.head)) {
			addFirstBlock(alloc);
		}
		else if (alloc.begin + size > alloc.end) {
			addNewBlock(alloc);
		}

		Bytes memory = Data::toBytes(alloc.begin, size);
		memory = Data::align(memory, 4);

		alloc.begin += Data::count(memory);
		return memory;
	}

	// ------------------------------------------------------------------------
	void move(SimpleLinearAllocator& from, SimpleLinearAllocator& to)
	{
		to = from;
		from = SimpleLinearAllocator();
	}

	// ------------------------------------------------------------------------
	void drop(SimpleLinearAllocator& alloc)
	{
		SingleListHead list = alloc.head;
		while (!Data::isEmpty(list)) {
			SingleListNode* toRemove = Data::head(list);
			list = Data::next(list);

			void* block = Native::page_begin_ptr(toRemove);
			Bytes memory = Data::toBytes(block, 0);
			Native::virtual_free(memory);
		}
		
		alloc = SimpleLinearAllocator();
	}

	// ------------------------------------------------------------------------
	SimpleLinearAllocator::SimpleLinearAllocator()
		: head(SingleListHead{ nullptr })
		, tail(SingleListTail{ nullptr })
		, begin(nullptr)
		, end(nullptr)
	{
	}

	// ------------------------------------------------------------------------
	SimpleLinearAllocator::~SimpleLinearAllocator()
	{
		if (!Data::isEmpty(head)) drop(*this);
	}

} // namespace Memory