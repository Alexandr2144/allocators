#pragma once
#ifndef DATA_BYTES_H
#define DATA_BYTES_H

#include "data/array.h"

#include <stdint.h>


namespace Data {
	using Byte = uint8_t;
	using Bytes = Array<Byte>;
	using Bytes_CRef = Bytes const&;

	extern Bytes noBytes;

	bool isNull(Bytes_CRef bytes);
	Bytes align(Bytes_CRef bytes, uintptr_t alignment);
	Bytes split(Bytes_CRef base, Bytes* part, size_t partSize);
	Bytes toBytes(void* ptr, size_t size);

	template <class T>
	Bytes toBytes(T* ptr) { return toBytes(ptr, sizeof(T)); }

	bool copy(Bytes from, Bytes to);

} // namespace Data


#endif // DATA_BYTES_H