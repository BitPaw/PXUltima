#pragma once

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


typedef PXActionResult(PXAPI* PXStreamCreateFunction)(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);
typedef PXActionResult(PXAPI* PXStreamUpdateFunction)(PXStream PXREF pxStream);
typedef PXActionResult(PXAPI* PXStreamOnFrameFunction)(PXStream PXREF pxStream, const PXStreamOnFrameInfo PXREF dataAdress);



#define PXStreamModeWindow		1
#define PXStreamModeMonitor		2
#define PXStreamModeCamera		3
#define PXStreamModeTV			4


typedef struct PXStreamWindow_
{
#if OSUnix
#elif OSWindows
	RECT Rectangle;

	PXWindow* WindowRef;

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




PXPrivate PXResult PXAPI PXStreamCreateWindow(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);
PXPrivate PXResult PXAPI PXStreamCreateMonitor(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);
PXPrivate PXResult PXAPI PXStreamCreateCamera(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);
PXPrivate PXResult PXAPI PXStreamCreateTV(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);

PXPrivate PXResult PXAPI PXStreamUpdateWindow(PXStream PXREF pxStream);
PXPrivate PXResult PXAPI PXStreamUpdateCamera(PXStream PXREF pxStream);
PXPrivate PXResult PXAPI PXStreamUpdateTV(PXStream PXREF pxStream);

PXPrivate PXResult PXAPI PXStreamReleaseWindow(PXStream PXREF pxStream);
PXPrivate PXResult PXAPI PXStreamReleaseMonitor(PXStream PXREF pxStream);
PXPrivate PXResult PXAPI PXStreamReleaseCamera(PXStream PXREF pxStream);

PXPublic PXResult PXAPI PXStreamOpen(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo);
PXPublic PXResult PXAPI PXStreamClose(PXStream PXREF pxStream);

#endif