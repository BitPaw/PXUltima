#include "PXLock.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI  PXLockCreate(PXLock* const lock, const PXLockType type)
{
    PXActionResult pxActionResult;

    PXClear(PXLock, lock);
    lock->Type = type;

    switch(type)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorCreate(lock);
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionCreate(lock);
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    return pxActionResult;
}

PXResult PXAPI  PXLockDelete(PXLock* const lock)
{
    PXActionResult pxActionResult;

    switch(lock->Type)
    {
        case PXLockTypeGlobal:
        {
            pxActionResult = PXSemaphorDelete(lock);
            break;
        }
        case PXLockTypeProcessOnly:
        {
            pxActionResult = PXCriticalSectionDelete(lock);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return pxActionResult;
}

PXResult PXAPI  PXLockEngage(PXLock* const lock)
{
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
            pxActionResult = PXCriticalSectionEnter(lock);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return pxActionResult;
}

PXResult PXAPI  PXLockRelease(PXLock* const pxLock)
{
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