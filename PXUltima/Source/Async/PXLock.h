#ifndef PXLockInclude
#define PXLockInclude

#include <OS/OSVersion.h>
#include <Error/ActionResult.h>

#if OSUnix
#include <semaphore.h>
typedef sem_t PXLockIDType; // is union, cannot be defined as "sem_t" only -> compile error
#elif OSWindows
#include <windows.h>
//#include <process.h>
typedef HANDLE PXLockIDType; // same as void*
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXLock_
	{
		PXLockIDType PXLockID;
	}
	PXLock;

    // Clear value of lock, sets CPrivate data to 0.
    PXPublic void PXLockClear(PXLock* const lock);

	PXPublic ActionResult PXLockCreate(PXLock* const lock);
	PXPublic ActionResult PXLockDelete(PXLock* const lock);
	PXPublic ActionResult PXLockEngage(PXLock* const lock);
	PXPublic ActionResult PXLockRelease(PXLock* const lock);
#ifdef __cplusplus
}
#endif

#endif
