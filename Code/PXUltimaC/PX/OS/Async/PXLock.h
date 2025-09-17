#ifndef PXLockIncluded
#define PXLockIncluded

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
    PXI32U LockCounter;

    union
    {
        PXLockIDType SemaphoreHandle;

#if OSWindows
        CRITICAL_SECTION SectionHandle;
#endif
    };
}
PXLock;

PXPublic PXResult PXAPI PXLockCreate(PXLock* const lock, const PXLockType type);
PXPublic PXResult PXAPI PXLockDelete(PXLock* const lock);
PXPublic PXResult PXAPI PXLockEngage(PXLock* const lock);
PXPublic PXResult PXAPI PXLockRelease(PXLock* const lock);

#endif
