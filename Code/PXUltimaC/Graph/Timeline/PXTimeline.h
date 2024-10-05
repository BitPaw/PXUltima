#ifndef PXTimelineINCLUDE
#define PXTimelineINCLUDE

#include <Media/PXResource.h>

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
    PXTexture2D* Texture;
}
PXTimelineEvent;

typedef struct PXTimeline_
{
    PXResourceInfo Info;
}
PXTimeline;

PXPublic PXActionResult PXAPI PXTimelineInitialize(PXTimeline* const pxTimeline);
PXPublic PXActionResult PXAPI PXTimelineEventAdd(PXTimeline* const pxTimeline, PXTimelineEvent* const pxTimelineEvent);
PXPublic PXActionResult PXAPI PXTimelineTimelineAdd(PXTimeline* const pxTimeline, PXTimelinePath* const pxTimelinePath);

#endif