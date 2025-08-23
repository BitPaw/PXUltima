#ifndef PXStreamInclude
#define PXStreamInclude

#include <PX/Media/PXResource.h>


typedef struct PXStream_ PXStream;
typedef struct PXStreamOpenInfo_ PXStreamOpenInfo;


typedef struct PXStreamOnFrameInfo_
{
	void* DataAdress;
	PXSize DataSize;
	PXSize Width;
	PXSize Height;
	PXInt8U BitPerPixel;
}
PXStreamOnFrameInfo;


typedef PXActionResult(PXAPI* PXStreamCreateFunction)(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
typedef PXActionResult(PXAPI* PXStreamUpdateFunction)(PXStream* const pxStream);
typedef PXActionResult(PXAPI* PXStreamOnFrameFunction)(PXStream* const pxStream, const PXStreamOnFrameInfo* const dataAdress);



#define PXStreamModeWindow		1
#define PXStreamModeMonitor		2
#define PXStreamModeCamera		3
#define PXStreamModeTV			4


typedef struct PXStreamWindow_
{
#if OSUnix
#elif OSWindows
	RECT Rectangle;
	HWND WindowHandle;
	HDC WindowDC;
	HDC MemoryDC;
	HBITMAP BitmapHandle;
#endif
}
PXStreamWindow;

typedef struct PXStreamCamera_
{
#if OSUnix
#elif OSWindows
	struct IMFSample* Sample;
	struct IMFSourceReader* SourceReader;
	struct IMFMediaBuffer* MediaBuffer;
	struct IMFMediaSource* MediaSource;
#endif

	PXSize Width;
	PXSize Height;
}
PXStreamCamera;


typedef struct PXStream_
{
	PXStreamCreateFunction Create;
	PXStreamUpdateFunction Update;
	PXStreamUpdateFunction Release;
	PXStreamOnFrameFunction OnFrame;

	PXInt16U Mode;

	union
	{
		PXStreamWindow Window;
		PXStreamCamera Camera;
	};
}
PXStream;

typedef struct PXStreamOpenInfo_
{
	union
	{
		PXWindowHandle WindowHandle;
		PXMonitorHandle MonitorHandle;
	};

	PXStreamOnFrameFunction OnFrameCallBack;

	PXInt16U DeviceIndex;
	PXInt16U Mode;
}
PXStreamOpenInfo;




PXPrivate PXActionResult PXAPI PXStreamCreateWindow(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXActionResult PXAPI PXStreamCreateMonitor(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXActionResult PXAPI PXStreamCreateCamera(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXActionResult PXAPI PXStreamCreateTV(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);

PXPrivate PXActionResult PXAPI PXStreamUpdateWindow(PXStream* const pxStream);
PXPrivate PXActionResult PXAPI PXStreamUpdateCamera(PXStream* const pxStream);
PXPrivate PXActionResult PXAPI PXStreamUpdateTV(PXStream* const pxStream);

PXPrivate PXActionResult PXAPI PXStreamReleaseWindow(PXStream* const pxStream);
PXPrivate PXActionResult PXAPI PXStreamReleaseMonitor(PXStream* const pxStream);
PXPrivate PXActionResult PXAPI PXStreamReleaseCamera(PXStream* const pxStream);

PXPublic PXActionResult PXAPI PXStreamOpen(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPublic PXActionResult PXAPI PXStreamClose(PXStream* const pxStream);

#endif