#include "native/crash.h"

#include <exception>
#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "Dbghelp.lib")


namespace Native {
	static ICrashHandler* crash_handler = nullptr;

	// ------------------------------------------------------------------------
	M_EXPORT ICrashHandler* setCrashHandler(ICrashHandler* handler)
	{
		ICrashHandler* backup = crash_handler;
		crash_handler = handler;
		return backup;
	}

	// ------------------------------------------------------------------------
	M_EXPORT void crash(uint32_t code, Native::CL cl, char const* msg)
	{
		if (crash_handler) {
			crash_handler->onCrash(code, cl, msg);
		}

		std::terminate();
	}

	// ------------------------------------------------------------------------
	static SYMBOL_INFO* prepareProcessSymbols(HANDLE hProc, char* buffer, size_t symLen)
	{
		
		SymInitialize(hProc, NULL, TRUE);

		SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = (ULONG)symLen;
		return symbol;
	}

	// ------------------------------------------------------------------------
	static int clamp(int from, int to, int maxDepth)
	{
		int depth = to - from;
		if (depth < 0)        return from;
		if (depth > maxDepth) return from + maxDepth;
		return to;
	}

	// ------------------------------------------------------------------------
	M_EXPORT void stacktrace(int from, int to)
	{
		static const size_t maxTraceDepth = 512;
		static const size_t maxSymbolLen = 512;
		
		void* trace[maxTraceDepth];
		char memSymbol[sizeof(SYMBOL_INFO) + maxSymbolLen + 1];
		
		HANDLE hProc = GetCurrentProcess();
		SYMBOL_INFO* symbol = prepareProcessSymbols(hProc, memSymbol, maxSymbolLen);

		WORD depth = RtlCaptureStackBackTrace(from, clamp(from, to, maxTraceDepth), trace, NULL);

		DWORD displacement;
		IMAGEHLP_LINE64 line;
		for (int i = 0; i < depth; i++) {
			DWORD64 address = (DWORD64)(trace[i]);
			BOOL ok = SymFromAddr(hProc, address, NULL, symbol);

			if (SymGetLineFromAddr64(hProc, address, &displacement, &line))
				 crash_handler->onTrace(trace[i], symbol->Name, line.FileName, line.LineNumber);
			else crash_handler->onTrace(trace[i], symbol->Name, nullptr, 0);
		}
	}

} // namespace Native