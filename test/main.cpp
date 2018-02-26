#include "memory/ext_buddy_allocator.h"
#include "memory/buddy_allocator.h"
#include "core/tools/asserts.h"
#include "core/math/basic.h"

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;


void malloc_test()
{
	int size = rand() % 1024;
	void* ptr = malloc(size);
	M_ASSERT(ptr != nullptr);
	free(ptr);
}


static Memory::BuddyAllocator balloc;
void balloc_test()
{
	int size = rand() % 1024;
	auto bytes = Memory::allocate(balloc, size);
	M_ASSERT(Data::isNull(bytes) == (size == 0));
	Memory::release(balloc, bytes);
}

static Memory::ExtBuddyAllocator bext_alloc;
void bext_alloc_test()
{
	int size = rand() % 1024;
	auto bytes = Memory::allocate(bext_alloc, size);
	M_ASSERT(Data::isNull(bytes) == (size == 0));
	Memory::release(bext_alloc, bytes);
}



milliseconds test(void(*testcase)(void), size_t cycles)
{
	auto t0 = std::chrono::high_resolution_clock().now();

	for (int i = 0; i < cycles; ++i) {
		testcase();
	}

	auto t1 = std::chrono::high_resolution_clock().now();
	auto ns = t1.time_since_epoch() - t0.time_since_epoch();
	return duration_cast<milliseconds>(ns);
}


int main()
{
	size_t cycles = 100000;
	
	auto t1 = test(balloc_test, cycles);
	auto t2 = test(bext_alloc_test, cycles);
	auto t3 = test(malloc_test, cycles);
	
	printf("buddy:      %lld ms, per op %lld ns\n", t1.count(), duration_cast<nanoseconds>(t1).count() / cycles);
	printf("ext_buddy:  %lld ms, per op %lld ns\n", t2.count(), duration_cast<nanoseconds>(t2).count() / cycles);
	printf("malloc:     %lld ms, per op %lld ns\n", t3.count(), duration_cast<nanoseconds>(t3).count() / cycles);

	//printf("coeff: %.2f\n", float(t1.count()) / float(t2.count()) - 1.f);
	system("pause");
}