#pragma once

#ifndef PXISBNIncluded
#define PXISBNIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXISBN_
{
    int x;
}
PXISBN;

PXPublic PXBool PXAPI PXISBNIsValid(const void* data, const PXSize dataSize);

#endif
