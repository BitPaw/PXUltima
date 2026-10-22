#pragma once

#ifndef PXGraphicIncluded
#define PXGraphicIncluded
#include <PX/Type/PXType.h>

typedef enum PXRefreshRateMode_
{
    PXRefreshRateUnlimited,
    PXRefreshRateVSync,
    PXRefreshRateCustomSync
}
PXRefreshRateMode;

typedef enum PXDrawMode_
{
    PXDrawModeInvalid,
    PXDrawModePoint,
    PXDrawModeLine,
    PXDrawModeLineLoop,
    PXDrawModeLineStrip,
    PXDrawModeLineStripAdjacency,
    PXDrawModeLineAdjacency,
    PXDrawModeTriangle,
    PXDrawModeTriangleAdjacency,
    PXDrawModeTriangleFAN,
    PXDrawModeTriangleStrip,
    PXDrawModeTriangleStripAdjacency,
    PXDrawModeWireFrame,
    PXDrawModeSquare,
    PXDrawModeSquareStrip,
    PXDrawModePatches
}
PXDrawMode;

typedef enum PXDrawFillMode_
{
    PXDrawModeFillInvalid,
    PXDrawModeFillPoints,
    PXDrawModeFillLines,
    PXDrawModeFillFill
}
PXDrawFillMode;

typedef enum PXGraphicAPI_
{
    PXGraphicAPIInvalid,
    PXGraphicAPINative,
    PXGraphicAPIOpenGL,
    PXGraphicAPIDirectX,
    PXGraphicAPIVulcan
}
PXGraphicAPI;

typedef enum PXGraphicInitializeMode_
{
    PXGraphicInitializeModeInvalid,
    PXGraphicInitializeModeWindowfull,
    PXGraphicInitializeModeWindowless,
    PXGraphicInitializeModeOSGUI,
    PXGraphicInitializeModeOSGUIElement
}
PXGraphicInitializeMode;

#endif