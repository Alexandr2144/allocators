#include "native/memory.h"

#include <Windows.h>


namespace Native {
	// ------------------------------------------------------------------------
	//M_EXPORT size_t page_size = 0x1000;

	M_EXPORT size_t      page_size()
	{
		return 0x1000;
	}

	// ------------------------------------------------------------------------
	M_EXPORT const void* page_begin_ptr(const void* ptr)
	{
		return (const void*)((uintptr_t)(~0x0FFF) & ((uintptr_t)ptr));
	}

	// ------------------------------------------------------------------------
	M_EXPORT void* page_begin_ptr(void* ptr)
	{
		return const_cast<void*>(page_begin_ptr((const void*)ptr));
	}

} // namespace Native