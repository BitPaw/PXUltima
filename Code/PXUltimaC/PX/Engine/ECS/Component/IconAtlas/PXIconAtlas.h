#pragma once

#ifndef PXIconAtlasIncluded
#define PXIconAtlasIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Icon/PXIcon.h>

typedef struct PXIconAtlas_
{
    PXECSInfo Info;

    PXTexture* IconTexture2D;

    PXSize IconListAmount;
    PXIcon* IconList;
}
PXIconAtlas;

typedef struct PXIconAtlasCreateInfo_
{
    PXECSCreateInfo Info;

    PXI16U CellSize;
    PXI16U CellAmountX; // Will be generated
    PXI16U CellAmountY; // Will be generated
    PXI16U CellAmountTotal; // Will be generated
}
PXIconAtlasCreateInfo;

PXPublic PXResult PXAPI PXIconAtlasCreate(PXIconAtlas PXREF pxIconAtlas, PXIconAtlasCreateInfo PXREF pxIconAtlasCreateInfo);

#endif