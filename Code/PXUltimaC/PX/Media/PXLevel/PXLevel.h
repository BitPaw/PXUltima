#pragma once

#ifndef PXLevelIncluded
#define PXLevelIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXLevel_
{
    PXI16U Version;
    PXI32U EntryAmount;
}
PXLevel;

#define PXLevelEntryFlagIsEmbedded (1 << 0)
//#define PXLevelEntryFlagIsEmbedded (1 << 0)

typedef struct PXLevelEntry_
{
    void* DataAdress;
    PXSize DataSize; // Can also be 0 is size if not known

    PXI32U Flags;
    PXI32U ResourceType; // Expected type
}
PXLevelEntry;

PXPublic PXResult PXAPI PXLevelLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLevelSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif