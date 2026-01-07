#pragma once

#ifndef PXSoftwareRenderIncluded
#define PXSoftwareRenderIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>

typedef struct PXSoftwareRender_
{
    unsigned int __dummy__;
}
PXSoftwareRender;

PXPublic void PXAPI PXSoftwareRenderDrawRectangle
(
    PXTexture PXREF image,
    const PXSize x,
    const PXSize y,
    const PXSize width,
    const PXSize height,
    const unsigned char red,
    const unsigned char green,
    const unsigned char blue,
    const unsigned char alpha
);
PXPublic void PXAPI PXTextureDrawA
(
    PXTexture PXREF image,
    const PXSize x,
    const PXSize y,
    const PXSize width,
    const PXSize height,
    const PXFont PXREF font,
    const char* text
);
PXPublic void PXAPI PXTextureDrawW
(
    PXTexture PXREF image,
    const PXSize x,
    const PXSize y,
    const PXSize width,
    const PXSize height,
    const PXFont PXREF font,
    const wchar_t* text
);

#pragma optimize( "", off )
PXPublic void PXAPI PXTextureMerge
(
    PXTexture PXREF image,
    const PXSize x,
    const PXSize y,
    const PXSize insertX,
    const PXSize insertY,
    const PXSize insertWidth,
    const PXSize insertHeight,
    const PXTexture PXREF imageInsert
);

#endif
