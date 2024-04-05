#ifndef PXLockInclude
#define PXLockInclude

#include <OS/System/PXOSVersion.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix
#include <semaphore.h>
typedef sem_t PXLockIDType; // is union, cannot be defined as "sem_t" only -> compile error
#elif OSWindows
#include <windows.h>
//#include <process.h>
typedef HANDLE PXLockIDType; // same as void*
#endif

typedef enum PXLockType_
{
	PXLockTypeInvalid,
	PXLockTypeGlobal,
	PXLockTypeProcessOnly
}
PXLockType;

typedef struct PXLock_
{
	PXLockIDType ID;
	PXLockType Type;
	PXInt8U LockCounter;

#if OSWindows
	CRITICAL_SECTION LockCriticalSection;
#endif
}
PXLock;

// Clear value of lock, sets CPrivate data to 0.
PXPublic void PXAPI PXLockClear(PXLock* const lock);

PXPublic PXActionResult PXAPI PXLockCreate(PXLock* const lock, const PXLockType type);
PXPublic PXActionResult PXAPI PXLockDelete(PXLock* const lock);
PXPublic PXActionResult PXAPI PXLockEngage(PXLock* const lock);
PXPublic PXActionResult PXAPI PXLockRelease(PXLock* const lock);

#endif