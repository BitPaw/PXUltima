#pragma once

#ifndef PXIconIncluded
#define PXIconIncluded

#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>

typedef enum PXIconID_
{
    PXIconIDInvalid,
    PXIconIDCustom,
    PXIconIDError,
    PXIconIDWarning,
    PXIconIDQuestionmark
}
PXIconID;

// Icon/Symbol to be rendered for GUI Elements
typedef struct PXIcon_
{
    PXECSInfo Info;

    union
    {
        HICON IconHandle;
    };
}
PXIcon;

typedef struct PXIconCreateInfo_
{
    PXECSCreateInfo Info;

    PXTexture* IconImage; // if set, generate a system icon from this
    PXSize OffsetX;
    PXSize OffsetY;
    PXSize Width;
    PXSize Height;
    PXSize RowSize;
    PXSize BitPerPixel;
}
PXIconCreateInfo;

PXPublic PXResult PXAPI PXIconRegisterToECS();

PXPublic PXResult PXAPI PXIconLoad(PXIcon PXREF pxIcon);
PXPublic PXResult PXAPI PXIconGetSystem(PXIcon PXREF pxIcon, const int iconID);
PXPublic PXResult PXAPI PXIconGetViaFilePath(PXIcon PXREF pxIcon, const PXText PXREF filePath);
PXPublic PXResult PXAPI PXIconFromTexture(PXIcon PXREF pxIcon, PXTexture PXREF pxTexture);

PXPublic PXResult PXAPI PXIconCreate(PXIcon** pxIcon, PXIconCreateInfo PXREF pxIconCreateInfo);

#endif