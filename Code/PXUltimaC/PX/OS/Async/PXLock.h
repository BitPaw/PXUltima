#ifndef PXLockInclude
#define PXLockInclude

#include <PX/OS/System/Version/PXOSVersion.h>
#include <PX/OS/Error/PXActionResult.h>

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
    PXLockType Type;
    PXInt32U LockCounter;

    union
    {
        PXLockIDType SemaphoreHandle;

#if OSWindows
        CRITICAL_SECTION SectionHandle;
#endif
    };
}
PXLock;

PXPublic PXActionResult PXAPI PXLockCreate(PXLock* const lock, const PXLockType type);
PXPublic PXActionResult PXAPI PXLockDelete(PXLock* const lock);
PXPublic PXActionResult PXAPI PXLockEngage(PXLock* const lock);
PXPublic PXActionResult PXAPI PXLockRelease(PXLock* const lock);

#endif
