#ifndef PXLevelINCLUDE
#define PXLevelINCLUDE

#include "../PXResource.h"

typedef struct PXLevel_
{
    PXInt16U Version;
    PXInt32U EntryAmount;
}
PXLevel;

#define PXLevelEntryFlagIsEmbedded (1 << 0)
//#define PXLevelEntryFlagIsEmbedded (1 << 0)

typedef struct PXLevelEntry_
{
    void* DataAdress;
    PXSize DataSize; // Can also be 0 is size if not known

    PXInt32U Flags;
    PXInt32U ResourceType; // Expected type
}
PXLevelEntry;

PXPublic PXActionResult PXAPI PXLevelLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXLevelSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif