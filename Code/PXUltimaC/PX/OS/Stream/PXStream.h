#ifndef PXStreamIncluded
#define PXStreamIncluded

#include <PX/Engine/PXResource.h>


typedef struct PXStream_ PXStream;
typedef struct PXStreamOpenInfo_ PXStreamOpenInfo;


typedef struct PXStreamOnFrameInfo_
{
	void* DataAdress;
	PXSize DataSize;
	PXSize Width;
	PXSize Height;
	PXI8U BitPerPixel;
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

	PXI16U Mode;

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

	PXI16U DeviceIndex;
	PXI16U Mode;
}
PXStreamOpenInfo;




PXPrivate PXResult PXAPI  PXStreamCreateWindow(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXResult PXAPI  PXStreamCreateMonitor(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXResult PXAPI  PXStreamCreateCamera(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPrivate PXResult PXAPI  PXStreamCreateTV(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);

PXPrivate PXResult PXAPI  PXStreamUpdateWindow(PXStream* const pxStream);
PXPrivate PXResult PXAPI  PXStreamUpdateCamera(PXStream* const pxStream);
PXPrivate PXResult PXAPI  PXStreamUpdateTV(PXStream* const pxStream);

PXPrivate PXResult PXAPI  PXStreamReleaseWindow(PXStream* const pxStream);
PXPrivate PXResult PXAPI  PXStreamReleaseMonitor(PXStream* const pxStream);
PXPrivate PXResult PXAPI  PXStreamReleaseCamera(PXStream* const pxStream);

PXPublic PXResult PXAPI PXStreamOpen(PXStream* const pxStream, PXStreamOpenInfo* const pxStreamOpenInfo);
PXPublic PXResult PXAPI PXStreamClose(PXStream* const pxStream);

#endif