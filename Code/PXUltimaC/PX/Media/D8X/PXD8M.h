#pragma once

#ifndef PXD8MIncluded
#define PXD8MIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXD8M_
{
    int x;   
}
PXD8M;

PXPublic PXResult PXAPI PXD8MLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

#endif
