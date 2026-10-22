#pragma once

#ifndef PXDriverIncluded
#define PXDriverIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXDriver_
{
    char Name[64];
    void* ImageBaseAddress;
}
PXDriver;

PXPublic PXResult PXAPI PXDriverListFetch(PXDriver* PXREF pxDriverList, PXSize PXREF listSize);

#endif