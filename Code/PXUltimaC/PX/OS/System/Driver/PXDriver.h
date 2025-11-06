#pragma once

#ifndef PXDriverIncluded
#define PXDriverIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXDriver_
{
    char Name[64];
    void* ImageBaseAdress;
}
PXDriver;

PXPublic PXResult PXAPI PXDriverListFetch(PXDriver* PXREF pxDriverList, PXSize PXREF listSize);

#endif