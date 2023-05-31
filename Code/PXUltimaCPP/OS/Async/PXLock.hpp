#ifndef PXCPPLockInclude
#define PXCPPLockInclude

#include <OS/Async/PXLock.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	enum class LockType
	{
		Invalid,
		Global,
		ProcessOnly
	};

	class Lock : PXLock
	{
		public:
		Lock();
		~Lock();

		void Clear();

		PX::ActionResult Create(const LockType type);
		PX::ActionResult Delete();
		PX::ActionResult Engage();
		PX::ActionResult Release();
	};
}
#endif