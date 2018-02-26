#include "data/bytes.h"

#include <memory.h>


namespace Data {
	// ------------------------------------------------------------------------
	Bytes noBytes{ nullptr, nullptr };

	// ------------------------------------------------------------------------
	bool isNull(Bytes_CRef bytes)
	{
		return (bytes.begin == nullptr);
	}

	// ------------------------------------------------------------------------
	Bytes align(Bytes_CRef bytes, uintptr_t alignment)
	{
		uintptr_t mask = alignment - 1;
		uintptr_t end = uintptr_t(bytes.end + mask) & ~mask;
		return Bytes{ bytes.begin, (Byte*)end };
	}

	// ------------------------------------------------------------------------
	Bytes split(Bytes_CRef base, Bytes* part, size_t partSize)
	{
		*part = toBytes(base.begin, partSize);
		return Bytes{ part->end, base.end };
	}

	// ------------------------------------------------------------------------
	bool copy(Bytes from, Bytes to)
	{
		size_t srcSize = from.end - from.begin;
		size_t destSize = to.end - to.begin;
		if (srcSize > destSize) {
			return false;
		}

		memcpy(to.begin, from.begin, srcSize);
		return true;
	}

	// ------------------------------------------------------------------------
	Bytes toBytes(void* ptr, size_t size)
	{
		return Bytes{ (Byte*)ptr, (Byte*)ptr + size };
	}

} // namespace Data