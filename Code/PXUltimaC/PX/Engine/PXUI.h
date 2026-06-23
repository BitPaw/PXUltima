#pragma once

#ifndef PXUIIncluded
#define PXUIIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXText.h>
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXUI_ PXUI;

typedef struct PXUIInfo_ 
{
    PXECSCreateInfo Info;

    PXText Path;
}
PXUIInfo;

PXPublic PXResult PXAPI PXUILoadFromFile(PXUI PXREF pxUI, PXText PXREF pxFilePath);

PXPublic PXResult PXAPI PXUICreate(PXUI** pxUIREF, PXUIInfo PXREF pxUIInfo);
PXPublic PXResult PXAPI PXUIRelease(PXUI PXREF pxUI);

#endif