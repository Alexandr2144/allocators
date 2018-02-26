#pragma once
#ifndef NATIVE_MEMORY_H
#define NATIVE_MEMORY_H

#include <stddef.h>

#include "data/bytes.h"
#include "native/export.h"


namespace Native {
	using Data::Bytes;
	using Data::Byte;


	//M_EXPORT extern size_t page_size;

	M_EXPORT size_t      page_size();

	M_EXPORT void*       page_begin_ptr(void* ptr);
	M_EXPORT const void* page_begin_ptr(const void* ptr);


	enum PageProtectFlags {
		PAGE_PROT_READ = 0x01,
		PAGE_PROT_WRITE = 0x02,
		PAGE_PROT_EXECUTABLE = 0x04,
	};

	M_EXPORT Bytes virtual_alloc(size_t size, int prot);
	M_EXPORT bool virtual_free(Bytes bytes);

} // namespace Native


#endif // NATIVE_MEMORY_H