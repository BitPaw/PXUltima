#pragma once

#ifndef PXECSEventDemuxerIncluded
#define PXECSEventDemuxerIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/ECS/PXECS.h>

PXPublic PXResult PXAPI PXECSEventDemuxerCreate();
PXPublic PXResult PXAPI PXECSEventDemuxerRelease();

PXPublic PXResult PXAPI PXECSEventDemuxerHandleAdd(HANDLE winHandle, PXECSInfo PXREF pxECSPayload);
PXPublic PXResult PXAPI PXECSEventDemuxerHandleRemove(HANDLE winHandle);

PXPublic PXResult PXAPI PXECSEventDemuxerWaitCancel();

// Blocks until an event occurs, returns number of events.
PXPublic PXResult PXAPI PXECSEventDemuxerWait();

#endif