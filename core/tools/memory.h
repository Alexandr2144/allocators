#pragma once
#ifndef TOOLS_MEMORY_H
#define TOOLS_MEMORY_H

#include "core/data/bytes.h"
#include "native/memory.h"

namespace Tools {
	using Native::PageProtectFlags;

	using Data::Bytes;
	using Data::Byte;

	Bytes virtual_alloc(size_t size, int prot);
	bool virtual_free(Bytes bytes);

} // namespace Tools


#endif // TOOLS_MEMORY_H