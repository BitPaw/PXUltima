#pragma once

#ifndef PXAwaitIncluded
#define PXAwaitIncluded

#include <PX/Type/PXType.h>

typedef struct PXAwaitInfo_
{
    volatile void* DataTarget;
    const void* DataExpect;
    PXSize DataSize;
}
PXAwaitInfo;

PXPublic PXBool PXAPI PXAwaitChange(PXAwaitInfo PXREF pxAwaitInfo);

#endif
