#ifndef PXStreamInclude
#define PXStreamInclude

#include <PX/Media/PXResource.h>

typedef struct PXStream_
{
	int x;
}
PXStream;

typedef struct PXStreamOpenInfo_
{
	union
	{
		PXWindowHandle WindowHandle;
		PXMonitorHandle MonitorHandle;
	};

	int DeviceIndex;
}
PXStreamOpenInfo;


PXPublic PXActionResult PXAPI PXStreamOpen(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPublic PXActionResult PXAPI PXStreamClose(PXStream* const pxStream);

#endif