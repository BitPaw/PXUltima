#pragma once

#include <Async/PXLock.h>

namespace PX
{
	class Lock : public PXLock
	{
		public:
		void Clear();

		ActionResult Create();
		ActionResult Delete();
		ActionResult Engage();
		ActionResult Release();
	};
}