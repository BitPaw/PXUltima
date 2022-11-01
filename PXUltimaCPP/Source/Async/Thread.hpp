#pragma once

#include <Async/PXThread.h>

namespace PX
{
	class Thread : PXThread
	{
		public:
		ActionResult Run(const ThreadFunction threadFunction, const void* parameter);

		static size_t CurrentID();
		void CurrentGet();

		void WaitForFinish();
	};
}