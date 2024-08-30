#pragma once

#include <OS/Debug/PXDebug.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	class Debug : public PXDebug
	{
		public:
		Debug();
		~Debug();

		PX::ActionResult ProcessBeeingDebugged(bool& isPresent);
		static bool ProcessBeeingDebuggedCurrent();

		void DebuggerSendMessage(PX::Text& const message);


		static bool DebuggerInitialize();


		PX::ActionResult StartProcess(const PX::Text& const applicationName);

		void Continue();
		void Pause();
		bool PauseOther(const PXProcessHandle pxProcessID);
		void Attach();

		void Detach();

		void StackTrace();

		PXSize MemoryRead(const void* const adress, void* const outputData, const PXSize outputDataSize);
		PXSize MemoryWrite(const void* const adress, const void* const inputData, const PXSize inputDataSize);

		PX::ActionResult WaitForEvent();
	};
}