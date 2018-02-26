#pragma once
#ifndef TOOLS_ASSERTS_H
#define TOOLS_ASSERTS_H

#include "native/crash.h"

#define M_COND(X) Tools::AssertHandle{ #X, (X), M_CL }


namespace Tools {
	struct AssertHandle {
		const char* msg;
		bool passed;
		Native::CL cl;
	};

	struct ConsoleCrashHandler
		: public Native::ICrashHandler
	{
		virtual void onTrace(void* fPtr, const char* fName, const char* file, uint32_t line) override;
		virtual void onCrash(uint32_t code, Native::CL cl, const char* msg) override;
	};


	void assert(AssertHandle handle);
	void assert_ext(AssertHandle handle, const char* fmt, ...);
	void assert_msg(Native::CL cl, const char* fmt, ...);

} // namespace Tools


#endif // TOOLS_ASSERTS_H