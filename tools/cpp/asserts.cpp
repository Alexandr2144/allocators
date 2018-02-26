#include "tools/asserts.h"

#include <stdio.h>


namespace Tools {
	// ------------------------------------------------------------------------
	void ConsoleCrashHandler::onTrace(void* fPtr, const char* fName, const char* file, uint32_t line)
	{
		printf("[0x%p] %s ", fPtr, fName);
		if (file != nullptr) {
			//printf("(%s:%d)", file, line);
		}
		putchar('\n');
	}

	// ------------------------------------------------------------------------
	void ConsoleCrashHandler::onCrash(uint32_t code, Native::CL cl, const char* msg)
	{
		puts("-------- Crash report --------");
		printf("Location: line %d, %s\n", cl.line, cl.function);
		printf("File: %s\n", cl.file);

		if (code != 0) printf("Code: 0x%08X\n", code);
		printf("Message: %s\n", msg);

		puts("-------- Stack trace ---------");
		Native::stacktrace(0, 32);
	}

	// ------------------------------------------------------------------------
	void assert(AssertHandle handle)
	{
		if (handle.passed) return;
		Native::crash(0, handle.cl, handle.msg);
	}

	// ------------------------------------------------------------------------
	void assert_ext(AssertHandle handle, const char* fmt, ...);

	// ------------------------------------------------------------------------
	void assert_msg(Native::CL cl, const char* fmt, ...);

} // namespace Tools