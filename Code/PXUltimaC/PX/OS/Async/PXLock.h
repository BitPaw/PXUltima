#pragma once

#ifndef PXLockIncluded
#define PXLockIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXLockType_
{
    PXLockTypeInvalid,
    PXLockTypeGlobal,
    PXLockTypeProcessOnly
}
PXLockType;

typedef struct PXLock_ PXLock;

typedef struct PXLockCreateInfo_
{
    PXECSCreateInfo Info;
    PXLockType Type;
}
PXLockCreateInfo;

typedef PXResult(PXAPI* PXLockFunction)(PXLock PXREF pxLock);
typedef PXResult(PXAPI* PXLockEnterFunction)(PXLock PXREF pxLock, const PXBool forceEntering);

PXPublic PXResult PXAPI PXLockRegisterToECS();

PXPublic PXResult PXAPI PXLockCreate(PXLock** lockREF, PXLockCreateInfo PXREF pxLockCreateInfo);
PXPublic PXResult PXAPI PXLockDelete(PXLock PXREF lock);
PXPublic PXResult PXAPI PXLockEngage(PXLock PXREF lock, const PXBool forceEnter);
PXPublic PXResult PXAPI PXLockRelease(PXLock PXREF lock);

PXPublic PXResult PXAPI PXSemaphorCreate(PXLock PXREF pxLock);
PXPublic PXResult PXAPI PXSemaphorDelete(PXLock PXREF pxLock);
PXPublic PXResult PXAPI PXSemaphorEnter(PXLock PXREF pxLock, const PXBool forceEntering);
PXPublic PXResult PXAPI PXSemaphorLeave(PXLock PXREF pxLock);

PXPublic PXResult PXAPI PXCriticalSectionCreate(PXLock PXREF pxLock);
PXPublic PXResult PXAPI PXCriticalSectionDelete(PXLock PXREF pxLock);
PXPublic PXResult PXAPI PXCriticalSectionEnter(PXLock PXREF pxLock, const PXBool forceEntering);
PXPublic PXResult PXAPI PXCriticalSectionLeave(PXLock PXREF pxLock);

#endif
