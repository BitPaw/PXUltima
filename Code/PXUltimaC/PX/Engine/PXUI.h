#pragma once

#ifndef PXUIIncluded
#define PXUIIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXWindow.h>
#include <PX/Type/PXRectangle.h>

#if OSUnix
#include <X11/Xlib.h>
#elif OSWindows
#endif // OSUnix



typedef struct PXUI_ PXUI;


typedef struct PXUITheme_
{
    PXBrush* BrushMainPrimary;
    PXBrush* BrushMainSecoundary;

    PXBrush* BrushDarkBackground;
    PXBrush* BrushTextWhite;

    PXFont* FontTitle;
    PXFont* FontContent;
}
PXUITheme;

typedef struct PXUIInfo_
{
    PXECSCreateInfo Info;

    PXText Path;

    PXSize MonitorListAmount;
    PXMonitor* MonitorList;

    PXSize DisplayListAmount;
    PXDisplay* DisplayList;
}
PXUIInfo;


PXPublic PXResult PXAPI PXUILoadFromFile(PXUI PXREF pxUI, PXText PXREF pxFilePath);

PXPublic PXResult PXAPI PXUICreate(PXUI** pxUIREF, PXUIInfo PXREF pxUIInfo);
PXPublic PXResult PXAPI PXUIRelease(PXUI PXREF pxUI);

PXPublic PXResult PXAPI PXUIWindowResolve(PXUI PXREF pxUI, PXECSInfo** pxECSInfo, PXText PXREF pxTextName);

PXPrivate PXResult PXAPI PXUICreateContext(PXUI PXREF pxUI);

PXPublic void PXAPI PXUIScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH);


PXPublic PXResult PXAPI PXUICursorPosition(PXVector2I32S PXREF position, const PXBool isWrite);



//PXPrivate void PXMathCircle(PXColorCircle PXREF pxColorCircle);

// UNSORTERD
PXPublic PXResult PXAPI PXWindowTabListSwapPage(PXWindow PXREF pxWindow);


#if OSUnix
PXInternal PXResult PXAPI PXUIErrorFromX(const int xErrorCode);
PXInternal int PXAPI PXUICallBackHandler(Display PXREF displayHandle, XErrorEvent PXREF xErrorEvent);
PXPublic PXUI* PXAPI PXUIDisplayInstance(void);

PXPublic Display* PXAPI PXUIDisplayGet(PXUI PXREF pxUI);

PXPublic int PXAPI PXUIScreenIDDefaultGet(PXUI PXREF pxUI);
#elif OSWindows
// ??
#endif


#endif