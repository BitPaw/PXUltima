#pragma once

#include <OS/Async/PXProgram.h>

namespace PX
{
	class Program : public PXProgram
	{
		public:
		// Execute function Asyncroinusly in another Thread.
		PXActionResult ExecuteAS(const char* programPath, const char* parameterString, PXProgramExecutedEvent* callback);
		PXActionResult ExecuteAL(const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent* callback);

		PXActionResult ExecuteWS(const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback);


		ProcessHandle CurrentProcess();
		ProcessID CurrentProcessID();

		void Attach();
		void Detach();
		void MemoryRead();
	};
}