#include "PXLock.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

const char PXLockText[] = "Lock";

PXResult PXAPI PXLockCreate(PXLock PXREFREF pxLock, const PXLockType pxLockType)
{
    if(!pxLock)
    {
        return PXActionRefusedArgumentNull;
    }

    PXActionResult pxActionResult;

    *pxLock = PXMemoryHeapCallocT(PXLock, 1);

    (*pxLock)->Type = pxLockType;

    switch(pxLockType)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorCreate(*pxLock);
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionCreate(*pxLock);
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    return pxActionResult;
}

PXResult PXAPI PXLockDelete(PXLock PXREF pxLock)
{
    if(!pxLock)
    {
        return PXActionRefusedArgumentNull;
    }

    PXActionResult pxActionResult;

    switch(pxLock->Type)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorDelete(pxLock);
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionDelete(pxLock);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return pxActionResult;
}

PXResult PXAPI PXLockEngage(PXLock PXREF lock, const PXBool forceEnter)
{
    if(!lock)
    {
        return PXActionRefusedArgumentNull;
    }

    PXActionResult pxActionResult;

    ++lock->LockCounter;

    switch(lock->Type)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorEnter(lock);
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionEnter(lock, forceEnter);
            break;
        }
        default:
        {
            // PXAssert(0, "Impossible State");
#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXLockText,
                "Engage",
                "Counter:%i, Force:%i",
                lock->LockCounter,
                forceEnter
            );
#endif
            return PXActionInvalidStateImpossible;
        }  
    }

    return pxActionResult;
}

PXResult PXAPI PXLockRelease(PXLock PXREF pxLock)
{
    if(!pxLock)
    {
        return PXActionRefusedArgumentNull;
    }

    PXActionResult pxActionResult;

    switch(pxLock->Type)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorLeave(pxLock);
            break; 
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionLeave(pxLock);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    --(pxLock->LockCounter);

    return PXActionSuccessful;
}