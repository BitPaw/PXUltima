#pragma once

#ifndef PXRenderingSystemIncluded
#define PXRenderingSystemIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMatrix.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXScene.h>

//#include <PX/Type/Camera/PXCamera.h>
//#include <PX/Type/Shader/PXShader.h>
//#include <PX/Type/Material/PXMaterial.h>
//#include <PX/Type/Texture/PXTexture.h>

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



// The rectangle of the view you can render in.
typedef struct PXViewPort_
{
    PXI32S X;
    PXI32S Y;
    PXI32S Width;
    PXI32S Height;
    PXF32 ClippingMinimum;
    PXF32 ClippingMaximum;
}
PXViewPort;


PXPublic PXResult PXAPI PXViewPortSetWH
(
    PXViewPort PXREF pxViewPort,
    const PXI32S width,
    const PXI32S height
);
PXPublic PXResult PXAPI PXViewPortSetXYWH
(
    PXViewPort PXREF pxViewPort,
    const PXI32S x,
    const PXI32S y,
    const PXI32S width,
    const PXI32S height
);


typedef struct PXRenderEntity_
{
    PXMatrix4x4F MatrixModel; // Position of target to render
    struct PXCamera_* CameraReference; // Camera required for rendering
    struct PXShaderProgram_* ShaderProgramReference; // Shader for the whole model
    struct PXMaterial_* MaterialOverride;
    struct PXTexture_* Texture2DOverride;
    void* ObjectReference; // Containing the object, type described in 'Type'
    PXI32U Type;

    // Dirty flags
}
PXRenderEntity;


typedef struct PXRenderer_
{
    PXECSInfo Info;

    PXCamera* CameraDefault;
    PXCamera* CameraCurrent;

    PXBool DrawWireFrame;
}
PXRenderer;

PXPublic void PXAPI PXRenderingSystemRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXRendererCreate(PXRenderer** pxRendererREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXRendererRelease(PXRenderer PXREF pxRenderer);

PXPublic PXResult PXAPI PXRendererSceneDraw(PXRenderer PXREF pxRenderer, PXScene PXREF pxScene);

PXPublic PXCamera* PXAPI PXRendererCameraCurrent(const PXRenderer PXREF pxRenderer);

#endif