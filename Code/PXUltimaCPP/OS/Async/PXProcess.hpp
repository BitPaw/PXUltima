#pragma once

#include <OS/Async/PXProcess.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	class Process : public PXProcess
	{
		public:
		Process();

		void Current();
		void Parent();

		PX::ActionResult Create(PX::Text& programmPath, const PXProcessCreationMode mode);

		PX::ActionResult Open();
		PX::ActionResult Close();

		PXSize MemoryWrite(const void* const targetAdress, const void* const buffer, const PXSize bufferSize);
		PXSize MemoryRead(void* const targetAdress, void* const buffer, const PXSize bufferSize);

		static PX::ActionResult MemoryInfoFetch(PXProcessMemoryInfo& pxProcessMemoryInfo);
	};
}