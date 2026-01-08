#pragma once

#ifndef PXTimelineIncluded
#define PXTimelineIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXTimelinePath_
{
    PXECSInfo Info;
    PXColorRGBAI8 Color;
}
PXTimelinePath;

typedef struct PXTimelineEvent_
{
    PXECSInfo Info;
    PXColorRGBAI8 Color;
    PXTexture* Texture;
}
PXTimelineEvent;

typedef struct PXTimeline_
{
    PXECSInfo Info;
}
PXTimeline;

PXPublic PXResult PXAPI PXTimelineInitialize(PXTimeline PXREF pxTimeline);
PXPublic PXResult PXAPI PXTimelineEventAdd(PXTimeline PXREF pxTimeline, PXTimelineEvent PXREF pxTimelineEvent);
PXPublic PXResult PXAPI PXTimelineTimelineAdd(PXTimeline PXREF pxTimeline, PXTimelinePath PXREF pxTimelinePath);

#endif