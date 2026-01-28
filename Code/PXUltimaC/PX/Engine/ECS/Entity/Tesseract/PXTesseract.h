#pragma once

#ifndef PXTesseractIncluded
#define PXTesseractIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXTesseract_
{
    PXECSInfo Info;
}
PXTesseract;

typedef struct PXTesseractCreateInfo_
{
    PXECSCreateInfo Info;
}
PXTesseractCreateInfo;

PXPublic PXResult PXAPI PXTesseractRegisterToECS();
PXPublic PXResult PXAPI PXTesseractCreate(PXTesseract** pxTesseractREF, PXTesseractCreateInfo PXREF pxTesseractCreateInfo);
PXPublic PXResult PXAPI PXTesseractRelease(PXTesseract PXREF pxTesseract);
PXPublic PXResult PXAPI PXTesseractDraw(PXTesseract PXREF pxTesseract, PXDrawInfo PXREF pxDrawInfo);

#endif