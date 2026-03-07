#pragma once

#ifndef PXD8WIncluded
#define PXD8WIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexturePool.h>


typedef struct PXD8TextureTableEntry_
{
    PXI32U RawDataSize;
    char PixelFormat[4];
    PXI32U TextureWidth;
    PXI32U TextureHeight;
    PXI32U MinmapCount;
    PXI32U UnknownB;
    PXI32U UnknownC;
    PXI32U UnknownD;
    PXI32U UnknownE;
    PXI32U UnknownF;
    PXI32U UnknownG;
    PXI32U UnknownH;

    PXI32U Offset;
    PXFile* TextureFile;
}
PXD8TextureTableEntry;


typedef struct PXD8TextureSet_
{
    PXI32U TextureSetDataSize;
    PXI32U TextureTotalSize;
    PXI32U TextureEntryAmount;
}
PXD8TextureSet;

typedef struct PXD8TextureTable_
{
    PXI32U TextureAmount; // Total amount of textures in this file
    PXI32U TextureSetAmount; // Textures are grouped in sets
    PXI32U TotalSizeRequested; // All data?

    PXD8TextureSet* SetList;

    // char Unkown[12]; // ????

    PXD8TextureTableEntry* EntryList;
}
PXD8TextureTable;

PXPublic PXResult PXAPI PXD8WLoadFromFile(PXTexturePool PXREF pxTexturePool, PXECSCreateInfo PXREF pxResourceLoadInfo, PXFile* pxTexturePoolFile);

#endif
