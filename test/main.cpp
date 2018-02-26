#include "memory/simple_linear_allocator.h"
#include "memory/buddy_allocator.h"

#include "tools/asserts.h"

#include <stdlib.h>

#include "data/math.h"


void test_1()
{
	Memory::SimpleLinearAllocator alloc_4;
	{
		Memory::SimpleLinearAllocator alloc_2;
		Memory::SimpleLinearAllocator alloc_3;

		Memory::drop(alloc_2);

		Memory::allocate(alloc_2, 4096);
		Memory::allocate(alloc_2, 2);
		Memory::allocate(alloc_2, 4);
		Memory::allocate(alloc_2, 4096);

		Memory::move(alloc_2, alloc_4);
		Memory::drop(alloc_2);
	}
}


int main()
{
	Tools::ConsoleCrashHandler handler;
	Native::setCrashHandler(&handler);

	Memory::BuddyAllocator alloc_1;
	Data::Bytes a = Memory::allocate(alloc_1, 1);
	Data::Bytes b = Memory::allocate(alloc_1, 120);

	//test_1();
}