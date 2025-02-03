#include "PXLock.h"

#include <OS/Memory/PXMemory.h>

PXActionResult PXAPI PXLockCreate(PXLock* const lock, const PXLockType type)
{
    PXClear(PXLock, lock);
    lock->Type = type;

#if OSUnix
    int sharedPointer = 0;
    unsigned int value = 1;

    const int resultID = sem_init(&lock->ID, sharedPointer, value);
    const unsigned char sucessful = resultID == 0; // 0=sucessful, -1=Error

    lock->Type = PXLockTypeGlobal;

#elif OSWindows

    switch(type)
    {
        case PXLockTypeGlobal:
        {
            LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = 0;
            LONG lInitialCount = 1;
            LONG lMaximumCount = 1;
            LPCSTR lpName = "BFE_ASYNC_LOCK";

            lock->SemaphoreHandle = CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, PXNull);
            const PXActionResult pxActionResult = PXErrorCurrent(lock->SemaphoreHandle != PXNull);

            if(PXActionSuccessful != pxActionResult)
            {
        
                return pxActionResult;
            }

            lock->Type = PXLockTypeGlobal;

            break;
        }
        case PXLockTypeProcessOnly:
        {
#if OSUnix

#elif OSWindows     
            InitializeCriticalSection(&lock->SectionHandle);
            break;     
#endif       
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLockDelete(PXLock* const lock)
{
    switch(lock->Type)
    {
        case PXLockTypeGlobal:
        {
            int closingResult = -1;

#if OSUnix
            closingResult = sem_destroy(&lock->ID);
#elif OSWindows
            closingResult = CloseHandle(lock->SemaphoreHandle);
#endif

            break;
        }
        case PXLockTypeProcessOnly:
        {
#if OSUnix

#elif OSWindows
            DeleteCriticalSection(&lock->SectionHandle);
#endif
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLockEngage(PXLock* const lock)
{
    ++lock->LockCounter;

    switch(lock->Type)
    {
        case PXLockTypeGlobal:
        {
#if OSUnix
            const int waitResultID = sem_wait(&lock->SemaphoreHandle);
            const PXActionResult waitResult = PXErrorCurrent(0 == waitResultID);
#elif OSWindows
            const DWORD result = WaitForSingleObject(lock->SemaphoreHandle, INFINITE);
            PXActionResult waitResult;

            switch(result)
            {
                case WAIT_ABANDONED:
                    waitResult = PXActionRefusedStateInvalid;
                    break;

                case WAIT_OBJECT_0:
                    waitResult = PXActionSuccessful;
                    break;

                case WAIT_TIMEOUT:
                    waitResult = PXActionRefusedNotReady;
                    break;

                case WAIT_FAILED:
                    waitResult = PXErrorCurrent(0);
                    break;

                default:
                    waitResult = PXActionInvalid;
                    break;
            }  
#endif

            return waitResult;      
        }
        case PXLockTypeProcessOnly:
        {
#if OSUnix

#elif OSWindows
            EnterCriticalSection(&lock->SectionHandle);
#endif
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLockRelease(PXLock* const lock)
{
    switch(lock->Type)
    {
        case PXLockTypeGlobal:
        {
#if OSUnix
            const int releaseResultID = sem_post(&lock->ID);
            const PXActionResult releaseResult = PXErrorCurrent(0 == releaseResultID);
#elif OSWindows
            const BOOL releaseResultID = ReleaseSemaphore(lock->SemaphoreHandle, 1, PXNull);
            const PXActionResult releaseResult = PXErrorCurrent(releaseResultID);   
#endif
            return releaseResult; 
        }
        case PXLockTypeProcessOnly:
        {
#if OSUnix

#elif OSWindows
            LeaveCriticalSection(&lock->SectionHandle);
#endif
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    --(lock->LockCounter);

    return PXActionSuccessful;
}