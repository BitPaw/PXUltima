#pragma once

#ifndef PXTransformViewIncluded
#define PXTransformViewIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXTransformView_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
    PXVector3F32* Position;
}
PXTransformView;

typedef struct PXTransformViewCrerateInfo_
{
    PXECSCreateInfo Info;

    PXVector3F32* Position;

    PXWindowCreateInfo Window;
}
PXTransformViewCrerateInfo;

PXPublic PXResult PXAPI PXTransformViewRegisterToECS();

PXPublic PXResult PXAPI PXTransformViewCreate(PXTransformView** pxTransformViewREF, PXTransformViewCrerateInfo PXREF pxTransformViewCrerateInfo);
PXPublic PXResult PXAPI PXTransformViewRelease(PXTransformView PXREF pxTransformView);
PXPublic PXResult PXAPI PXTransformViewDraw(PXTransformView PXREF pxTransformView, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif