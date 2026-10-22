#pragma once

#ifndef PXTexture2DArrayIncluded
#define PXTexture2DArrayIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXTexture2DArray_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;
    };

    PXSize Width;
    PXSize Height;
    PXSize AmountOfTextures;
}
PXTexture2DArray;

#endif