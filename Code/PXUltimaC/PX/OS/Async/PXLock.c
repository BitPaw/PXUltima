#include "PXLock.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

#if OSUnix
#include <semaphore.h>
#elif OSWindows
#include <windows.h>
//#include <process.h>
#endif

typedef struct PXLock_
{
    PXECSInfo Info;

    PXLockType Type;
    PXI32U LockCounter;

    PXLockFunction Create;
    PXLockFunction Release;
    PXLockEnterFunction Enter;
    PXLockFunction Leave;

    union
    {
        //----------------------
        // semaphore
#if OSUnix
        sem_t SemaphoreHandle; // is union, cannot be defined as "sem_t" only -> compile error
#elif OSWindows
        HANDLE SemaphoreHandle;
#endif
        //----------------------


        //----------------------
        // Windows Critical secion
#if OSWindows
        CRITICAL_SECTION SectionHandle;
#endif
        //----------------------
    };
}
PXLock;



const char PXOSSemaphore[] = "Semaphore";
const char PXOSCriticalSection[] = "Crit-Section";
const char PXLockText[] = "Lock";
const PXI8U PXLockTextLength = sizeof(PXLockText);
const PXECSRegisterInfoStatic PXLockRegisterInfoStatic =
{
    {sizeof(PXLockText), sizeof(PXLockText), PXLockText, TextFormatASCII},
    sizeof(PXLock),
    __alignof(PXLock),
    PXECSTypeResource,
    PXLockCreate
};
PXECSRegisterInfoDynamic PXLockRegisterInfoDynamic;

PXResult PXAPI PXLockRegisterToECS()
{
    PXECSRegister(&PXLockRegisterInfoStatic, &PXLockRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXLockCreate(PXLock** lockREF, PXLockCreateInfo PXREF pxLockCreateInfo)
{
    PXLock* pxLock = PXNull;

    if(!lockREF)
    {
        return PXResultRefusedParameterNull;
    }

    pxLockCreateInfo->Info.Static = &PXLockRegisterInfoStatic;
    pxLockCreateInfo->Info.Dynamic = &PXLockRegisterInfoDynamic;
    PXECSCreate(lockREF, pxLockCreateInfo);

    pxLock = *lockREF;

    pxLock->Type = pxLockCreateInfo->Type;

    switch(pxLockCreateInfo->Type)
    {
        case PXLockTypeGlobal:
        {
            pxLock->Create = PXSemaphorCreate;
            pxLock->Release = PXSemaphorDelete;
            pxLock->Enter = PXSemaphorEnter;
            pxLock->Leave = PXSemaphorLeave;
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxLock->Create = PXCriticalSectionCreate;
            pxLock->Release = PXCriticalSectionDelete;
            pxLock->Enter = PXCriticalSectionEnter;
            pxLock->Leave = PXCriticalSectionLeave;
            break;
        }
        default:
            return PXResultRefusedParameterInvalid;
    }

    PXResult pxResult = pxLock->Create(pxLock);

    return pxResult;
}

PXResult PXAPI PXLockDelete(PXLock PXREF pxLock)
{
    if(!pxLock)
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = pxLock->Release(pxLock);

    return pxResult;
}

PXResult PXAPI PXLockEngage(PXLock PXREF pxLock, const PXBool forceEnter)
{
    if(!pxLock)
    {
        return PXResultRefusedParameterNull;
    }

    ++pxLock->LockCounter;

    PXResult pxResult = pxLock->Enter(pxLock, forceEnter);

    return pxResult;
}

PXResult PXAPI PXLockRelease(PXLock PXREF pxLock)
{
    if(!pxLock)
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = pxLock->Leave(pxLock);

    --(pxLock->LockCounter);

    return pxResult;
}

const PXI32U _lockWaitSpan = 100;
const PXI32U _lockWaitTrys = 3;


PXResult PXAPI PXSemaphorCreate(PXLock PXREF pxLock)
{
    PXActionResult pxActionResult;


#if OSUnix

    int sharedPointer = 0;
    unsigned int value = 1;

    const int resultID = sem_init(&pxLock->ID, sharedPointer, value);
    pxActionResult = PXErrorCurrent(0 == resultID); // 0=sucessful, -1=Error

#elif OSWindows
    LONG lInitialCount = 1;
    LONG lMaximumCount = 1;

    pxLock->SemaphoreHandle = CreateSemaphoreA(PXNull, lInitialCount, lMaximumCount, PXNull);
    pxActionResult = PXErrorCurrent(pxLock->SemaphoreHandle != PXNull);

#else
    pxActionResult = PXActionRefusedNotImplemented;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLockText,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

    return pxActionResult; // Success
}

PXResult PXAPI PXSemaphorDelete(PXLock PXREF pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLockText,
        PXOSSemaphore,
        "Created: HANDLE:<%p>",
        pxLock->SemaphoreHandle
    );
#endif

#if OSUnix
    const int closingResult = sem_destroy(&lock->ID);
#elif OSWindows
    const BOOL closingResult = CloseHandle(pxLock->SemaphoreHandle);
#endif
}

PXResult PXAPI PXSemaphorEnter(PXLock PXREF pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Enter"
    );
#endif

    PXActionResult waitResult = PXResultInvalid;

#if OSUnix
    const int waitResultID = sem_wait(&lock->SemaphoreHandle);
    waitResult = PXErrorCurrent(0 == waitResultID);
#elif OSWindows

    PXSize tryCounter = 0;

    for(;;)
    {
        const DWORD result = WaitForSingleObject(pxLock->SemaphoreHandle, _lockWaitSpan); // INFINITE


        if(WAIT_OBJECT_0 == result)
        {
            waitResult = PXResultOK;
            // We got runtime
            break;
        }

        if(WAIT_TIMEOUT == result)
        {
            ++tryCounter;

            if(_lockWaitTrys >= tryCounter)
            {
                continue;
            }

#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingWarning,
                PXOSName,
                PXOSSemaphore,
                "Failed.. timeout!"
            );
#endif


            waitResult = PXActionRefusedNotReady;
            break;
        }


        switch(result)
        {
            case WAIT_ABANDONED:
                waitResult = PXActionFailedWaitAbandoned;
                break;

            case WAIT_FAILED:
                waitResult = PXActionFailedWaitTimeout;
                break;
        }
    }

#endif

    return waitResult;
}

PXResult PXAPI PXSemaphorLeave(PXLock PXREF pxLock)
{
    if(!pxLock)
    {
        return PXResultInvalid;
    }

    if(0 == pxLock->SemaphoreHandle)
    {
        return PXResultInvalid;
    }

#if PXLogEnable  && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSSemaphore,
        "Leave"
    );
#endif

#if OSUnix
    const int releaseResultID = sem_post(&pxLock->ID);
    const PXResult releaseResult = PXErrorCurrent(0 == releaseResultID);
#elif OSWindows
    const BOOL releaseResultID = ReleaseSemaphore(pxLock->SemaphoreHandle, 1, PXNull);
    const PXResult releaseResult = PXErrorCurrent(releaseResultID);
#endif   

    return releaseResult;
}

PXResult PXAPI PXCriticalSectionCreate(PXLock PXREF pxLock)
{
#if OSUnix

#elif OSWindows     
    PXClear(CRITICAL_SECTION, &pxLock->SectionHandle);
    InitializeCriticalSection(&pxLock->SectionHandle);
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLockText,
        PXOSCriticalSection,
        "Create"
    );
#endif
}

PXResult PXAPI PXCriticalSectionDelete(PXLock PXREF pxLock)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLockText,
        PXOSCriticalSection,
        "Delete"
    );
#endif

#if OSUnix

#elif OSWindows
    DeleteCriticalSection(&pxLock->SectionHandle);
#endif
}

PXResult PXAPI PXCriticalSectionEnter(PXLock PXREF pxLock, const PXBool forceEntering)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLockText,
        PXOSCriticalSection,
        "Enter"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION PXREF criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    if(forceEntering)
    {
        EnterCriticalSection(criticalSectionCast);
    }
    else
    {
        PXSize failEnterCounter = 0;

        for(;;)
        {
            const BOOL success = TryEnterCriticalSection(criticalSectionCast); // Windows XP
            const PXResult pxResult = PXErrorCurrent(success);

            if(success)
            {
                break;
            }

            ++failEnterCounter;

            if(failEnterCounter < 10)
            {
                Sleep(0);
                continue;
            }

#if PXLogEnable 
            PXLogPrint
            (
                PXLoggingInfo,
                PXLockText,
                PXOSCriticalSection,
                "Enter failed! Waiting..."
            );
#endif

            return PXActionFailedLockEnter;
        }
    }

#endif

    return PXResultOK;
}

PXResult PXAPI PXCriticalSectionLeave(PXLock PXREF pxLock)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOSName,
        PXOSCriticalSection,
        "Leave"
    );
#endif

#if OSUnix

#elif OSWindows
    CRITICAL_SECTION PXREF criticalSectionCast = (CRITICAL_SECTION*)&pxLock->SectionHandle;

    LeaveCriticalSection(criticalSectionCast);
#endif

    return PXResultOK;
}
