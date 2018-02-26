#pragma once
#ifndef NATIVE_CRASH_H
#define NATIVE_CRASH_H

#include <stdint.h>
#include "export.h"

#define M_CL Native::CL{ __FUNCTION__, __FILE__, __LINE__ }


namespace Native {
	struct CL {
		const char* function;
		const char* file;
		int line;
	};

	struct ICrashHandler {
		virtual void onTrace(void* fPtr, char const* fName, char const* file, uint32_t line) = 0;
		virtual void onCrash(uint32_t code, Native::CL cl, char const* msg) = 0;
	};
	M_EXPORT ICrashHandler* setCrashHandler(ICrashHandler* handler);

	M_EXPORT void crash(uint32_t code, Native::CL cl, char const* msg);
	M_EXPORT void stacktrace(int from, int to);

} // namespace Native


#endif // NATIVE_CRASH_H