#include "native/memory.h"

#include <Windows.h>


namespace Native {
	// ------------------------------------------------------------------------
	static DWORD mprotect_flags(int flags)
	{
		const bool executable = (flags & PAGE_PROT_EXECUTABLE);
		const bool write = (flags & PAGE_PROT_WRITE);
		const bool read = (flags & PAGE_PROT_READ);

		if (read && !write && !executable) {
			return PAGE_READONLY;
		}
		else if (read && !write && executable) {
			return PAGE_EXECUTE_READ;
		}
		else if (write && !executable) {
			return PAGE_READWRITE;
		}
		else if (write && executable) {
			return PAGE_EXECUTE_READWRITE;
		}
		else {
			return 0;
		}
	}

	// ------------------------------------------------------------------------
	M_EXPORT void* virtual_alloc(size_t size, int prot)
	{
		return VirtualAlloc(nullptr, size, MEM_COMMIT, mprotect_flags(prot));
	}

	// ------------------------------------------------------------------------
	M_EXPORT bool virtual_free(void* ptr, size_t size)
	{
		return VirtualFree(ptr, 0, MEM_RELEASE);
	}

} // namespace Native