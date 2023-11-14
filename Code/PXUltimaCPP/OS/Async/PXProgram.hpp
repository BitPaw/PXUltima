#pragma once

#include <OS/Async/PXProgram.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class Program : public PXProgram
	{
		public:
		// Execute function Asyncroinusly in another Thread.
		PX::ActionResult ExecuteAS(const char* programPath, const char* parameterString, PXProgramExecutedEvent* callback);
		PX::ActionResult ExecuteAL(const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent* callback);

		PX::ActionResult ExecuteWS(const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback);


		ProcessHandle CurrentProcess();
		ProcessID CurrentProcessID();

		void Attach();
		void Detach();
		void MemoryRead();
	};
}