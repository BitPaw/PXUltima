#ifndef LockInclude
#define LockInclude

#include <OS/OSVersion.h>
#include <Error/ActionResult.h>

#if OSUnix
#include <semaphore.h>
#define LockID sem_t // is union, cannot be defined as "sem_t" only -> compile error
#elif OSWindows
#include <windows.h>
//#include <process.h>
#define LockID HANDLE // same as void*
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    // Clear value of lock, sets CPrivate data to 0.
    PXPublic void LockClear(LockID* const asyncLockID);

	PXPublic ActionResult LockCreate(LockID* const asyncLockID);
	PXPublic ActionResult LockDelete(LockID* const asyncLockID);
	PXPublic ActionResult LockLock(LockID* const asyncLockID);
	PXPublic ActionResult LockRelease(LockID* const asyncLockID);
#ifdef __cplusplus
}
#endif

#endif
