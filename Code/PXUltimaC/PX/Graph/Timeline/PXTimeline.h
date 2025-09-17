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

PXPublic PXResult PXAPI PXTimelineInitialize(PXTimeline* const pxTimeline);
PXPublic PXResult PXAPI PXTimelineEventAdd(PXTimeline* const pxTimeline, PXTimelineEvent* const pxTimelineEvent);
PXPublic PXResult PXAPI PXTimelineTimelineAdd(PXTimeline* const pxTimeline, PXTimelinePath* const pxTimelinePath);

#endif