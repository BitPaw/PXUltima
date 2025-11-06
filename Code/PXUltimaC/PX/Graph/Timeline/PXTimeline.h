#pragma once

#ifndef PXTimelineIncluded
#define PXTimelineIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXTimelinePath_
{
    PXResourceInfo Info;
    PXColorRGBAI8 Color;
}
PXTimelinePath;

typedef struct PXTimelineEvent_
{
    PXResourceInfo Info;
    PXColorRGBAI8 Color;
    PXTexture* Texture;
}
PXTimelineEvent;

typedef struct PXTimeline_
{
    PXResourceInfo Info;
}
PXTimeline;

PXPublic PXResult PXAPI PXTimelineInitialize(PXTimeline PXREF pxTimeline);
PXPublic PXResult PXAPI PXTimelineEventAdd(PXTimeline PXREF pxTimeline, PXTimelineEvent PXREF pxTimelineEvent);
PXPublic PXResult PXAPI PXTimelineTimelineAdd(PXTimeline PXREF pxTimeline, PXTimelinePath PXREF pxTimelinePath);

#endif