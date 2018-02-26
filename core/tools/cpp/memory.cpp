#include "core/tools/memory.h"


namespace Tools {
	using Data::toBytes;

	// ------------------------------------------------------------------------
	Bytes virtual_alloc(size_t size, int prot)
	{
		Byte* ptr = (Byte*)Native::virtual_alloc(size, prot);
		Bytes bytes = Data::align(toBytes(ptr, size), Native::page_size());
		return bytes;
	}

	// ------------------------------------------------------------------------
	bool virtual_free(Bytes bytes)
	{
		return Native::virtual_free(bytes.begin, bytes.end - bytes.begin);
	}

} // namespace Tools