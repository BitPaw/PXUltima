#pragma once

#include <OS/Async/PXThread.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class Thread : public PXThread
	{
		public:
		PX::ActionResult Run(const ThreadFunction threadFunction, const void* parameter);

		void Suspend();
		void Resume();
		void Sleep(const PXSize sleepTime);

		static PXSize CurrentID();
		void CurrentGet();

		void WaitForFinish();
	};
}