#pragma once

#ifndef PXGraphicIncluded
#define PXGraphicIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Media/PXColor.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/Graphic/DirectX/PXDirectX.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>
#include <PX/OS/Graphic/Vulcan/PXVulcan.h>

#define PXShaderNotRegisterd (unsigned int)-1




typedef PXI32U PXGraphicResourceID;

typedef struct PXGraphicResourceInfo_
{
    PXGraphicResourceID ID;
    PXGraphicResourceLocation Location;
    PXGraphicSystem System;

    union
    {
        PXMSHandle DirectXID;
        PXI32U OpengGLID;
    };
}
PXGraphicResourceInfo;

typedef enum PXGraphicInitializeMode_
{
    PXGraphicInitializeModeInvalid,
    PXGraphicInitializeModeWindowfull,
    PXGraphicInitializeModeWindowless,
    PXGraphicInitializeModeOSGUI,
    PXGraphicInitializeModeOSGUIElement
}
PXGraphicInitializeMode;

typedef struct PXGraphicInitializeInfo_
{
    PXGraphicInitializeMode Mode;
    PXWindow* WindowReference;

#if OSUnix
    Display* DisplayConnection;
#elif OSWindows
    //HDC HandleDeviceContext;
#endif

    PXSize TargetFrameRate;

    PXSize Width;
    PXSize Height;

    PXDirectXVersion DirectXVersion;
    PXDirectXDriverType DirectXDriverType;

    PXGraphicSystem GraphicSystem;

    struct PXGraphic_* Graphic;
}
PXGraphicInitializeInfo;


typedef struct PXGraphicConfig_
{
    PXRefreshRateMode WindowRefreshRateMode;
    PXI32S ScreenResolution[2];
    PXBool FullScreen;
}
PXGraphicConfig;


//typedef struct Renderable_ Renderable; ?

typedef struct PXRenderableMeshSegment_
{
    unsigned int NumberOfVertices;
    unsigned int TextureID;
    unsigned int ShaderID;

    PXDrawMode RenderMode;

    PXBool DoRendering;
}
PXRenderableMeshSegment;

typedef struct PXRenderable_
{
    PXMatrix4x4F MatrixModel;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int IBO;

    PXSize MeshSegmentListSize;
    PXRenderableMeshSegment* MeshSegmentList;

    PXBool DoRendering;
}
PXRenderable;


//---<UI Elements>---------------------------------------------------------











PXPublic PXResult PXAPI PXFrameBufferCreate(PXFrameBuffer PXREF pxFrameBuffer, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo);






typedef enum PXUIState
{
    UIStateInvalid,
    UIStateActive,
    UIStateHot,
    UIStateIDLE
}
PXUIState;

typedef enum PXUIScalingType_
{
    UIScalingInvalid,
    UIScalingRelativeToParent
}
PXUIScalingType;

typedef struct UIContainerRoot_
{
    unsigned char _SUMMY__;
}
UIContainerRoot;



//-------------------------------------------------------------------------
// Define generic graphic API, make happen that you can use whatever
//-------------------------------------------------------------------------
typedef PXResult(PXAPI* PXGraphicInitializeFunction)(void PXREF pxGraphicAPI, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
typedef PXBool(PXAPI* PXGraphicReleaseFunction)(void PXREF pxGraphicAPI);

typedef void (PXAPI* PXGraphicSelectFunction)(void PXREF pxGraphicAPI);
typedef void (PXAPI* PXGraphicDeselectFunction)(void PXREF pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicLoadFunction)(void PXREF pxGraphicAPI);
typedef PXActionResult(PXAPI* PXGraphicUnloaddFunction)(void PXREF pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicViewPortSetFunction)(void PXREF pxGraphicAPI, const PXViewPort PXREF pxViewPort);
typedef PXActionResult(PXAPI* PXGraphicViewPortGetFunction)(void PXREF pxGraphicAPI, PXViewPort PXREF pxViewPort);

typedef void (PXAPI* PXGraphicSceneBeginFunction)(void PXREF pxGraphicAPI);
typedef void (PXAPI* PXGraphicSceneEndFunction)(void PXREF pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicScreenBufferReadFunction)(void PXREF pxGraphicAPI, PXTexture PXREF PXTexture);

typedef PXActionResult(PXAPI* PXGraphicDrawScriptCreate)(void PXREF pxGraphicAPI, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptBegin)(void PXREF pxGraphicAPI, PXDrawScript PXREF pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptEnd)(void PXREF pxGraphicAPI, PXDrawScript PXREF pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptDelete)(void PXREF pxGraphicAPI, PXDrawScript PXREF pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptExecute)(void PXREF pxGraphicAPI, PXDrawScript PXREF pxDrawScript);


typedef PXActionResult(PXAPI* PXGraphicModelRegisterFunction)(void PXREF pxGraphicAPI, PXModel PXREF pxModel);
typedef PXActionResult(PXAPI* PXGraphicModelDrawFunction)(void PXREF pxGraphicAPI, PXRenderEntity PXREF pxRenderEntity);
typedef PXActionResult(PXAPI* PXGraphicModelSelectFunction)(void PXREF pxGraphicAPI, PXModel PXREF pxModel);
typedef PXActionResult(PXAPI* PXGraphicModelReleaseFunction)(void PXREF pxGraphicAPI, PXModel PXREF pxModel);

typedef PXActionResult(PXAPI* PXGraphicLightSetFunction)(void PXREF pxGraphicAPI, PXLight PXREF pxLight, const PXI32U index);
typedef PXActionResult(PXAPI* PXGraphicLightGetFunction)(void PXREF pxGraphicAPI, PXLight PXREF pxLight, const PXI32U index);
typedef PXActionResult(PXAPI* PXGraphicLightEnableSetFunction)(void PXREF pxGraphicAPI, PXLight PXREF pxLight, const PXI32U index, const PXBool enable);
typedef PXActionResult(PXAPI* PXGraphicLightEnableGetFunction)(void PXREF pxGraphicAPI, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable);


typedef PXActionResult(PXAPI* PXGraphicTextureActionFunction)(void PXREF pxGraphicAPI, PXTexturInfo PXREF pxGraphicTexturInfo);


typedef void (PXAPI* PXGraphicClearFunction)(void PXREF pxGraphicAPI, const PXColorRGBAF PXREF backgroundColor);
typedef PXBool(PXAPI* PXGraphicSceneDeployFunction)(void PXREF pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListAmountFunction)(void PXREF graphicAPI, PXI32U PXREF amount);
typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListFetchFunction)(void PXREF graphicAPI, const PXI32U amount, PXGPUPhysical PXREF pxGraphicDevicePhysicalList);

typedef PXActionResult(PXAPI* PXGraphicSwapIntervalSetFunction)(void PXREF graphicAPI, const PXI32U interval);
typedef PXActionResult(PXAPI* PXGraphicSwapIntervalGetFunction)(void PXREF graphicAPI, PXI32U PXREF interval);


typedef PXActionResult(PXAPI* PXGraphicRectangleDrawFunction)(void PXREF graphicAPI, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const char mode);
typedef PXActionResult(PXAPI* PXGraphicRectangleDrawTxFunction)(void PXREF graphicAPI, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXF32 txA, const PXF32 tyA, const PXF32 txB, const PXF32 tyB, const char mode);


typedef PXActionResult(PXAPI* PXGraphicDrawColorRGBFFunction)(void PXREF graphicAPI, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha);

typedef PXActionResult(PXAPI* PXGraphicDrawModeSetFunction)(void PXREF graphicAPI, const PXDrawFillMode pxGraphicDrawFillMode);

typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram, PXShader PXREF shaderList, const PXSize amount);
typedef PXActionResult(PXAPI* PXGraphicShaderProgramSelectFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram);
typedef PXActionResult(PXAPI* PXGraphicShaderProgramDeleteFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram);

typedef PXActionResult(PXAPI* PXShaderVariableSetFunction)(void PXREF graphicAPI, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariable);
//-------------------------------------------------------------------------


typedef struct PXGraphic_
{
    //-------------------
    // References
    //-------------------
    PXWindow* WindowReference;
    void* EventOwner;
    //-------------------

    //-------------------
    // Functions
    //-------------------


    //-------------------------------------------------
    // Core
    //-------------------------------------------------
    PXGraphicInitializeFunction Initialize;
    PXGraphicReleaseFunction Release;
    PXGraphicSelectFunction Select;
    PXGraphicDeselectFunction Deselect;
    PXGraphicClearFunction Clear;

    //-------------------------------------------------
    // Info & Settings
    //-------------------------------------------------
    PXGraphicSwapIntervalSetFunction SwapIntervalSet;
    PXGraphicSwapIntervalGetFunction SwapIntervalGet;
    PXGraphicDevicePhysicalListAmountFunction DevicePhysicalListAmount;
    PXGraphicDevicePhysicalListFetchFunction DevicePhysicalListFetch;

    //-------------------------------------------------
    // Scene
    //-------------------------------------------------
    PXGraphicSceneBeginFunction SceneBegin;
    PXGraphicSceneDeployFunction SceneDeploy;
    PXGraphicSceneEndFunction SceneEnd;

    //-------------------------------------------------
    // Screen
    //-------------------------------------------------
    PXGraphicScreenBufferReadFunction ScreenBufferRead;

    //-------------------------------------------------
    // View
    //-------------------------------------------------
    PXGraphicViewPortSetFunction ViewPortSet;
    PXGraphicViewPortGetFunction ViewPortGet;

    //-------------------------------------------------
    // Scripting
    //-------------------------------------------------
    PXGraphicDrawScriptCreate DrawScriptCreate;
    PXGraphicDrawScriptBegin DrawScriptBegin;
    PXGraphicDrawScriptEnd DrawScriptEnd;
    PXGraphicDrawScriptDelete DrawScriptDelete;
    PXGraphicDrawScriptExecute DrawScriptExecute;

    //-------------------------------------------------
    // Textures
    //-------------------------------------------------
    PXGraphicTextureActionFunction TextureAction;

    //-------------------------------------------------
    // Model
    //-------------------------------------------------
    PXGraphicModelRegisterFunction ModelRegister;
    PXGraphicModelDrawFunction ModelDraw;
    PXGraphicModelSelectFunction ModelSelect;
    PXGraphicModelReleaseFunction ModelRelease;

    //-------------------------------------------------
    // Light
    //-------------------------------------------------
    PXGraphicLightSetFunction LightSet;
    PXGraphicLightGetFunction LightGet;
    PXGraphicLightEnableSetFunction LightEnableSet;
    PXGraphicLightEnableGetFunction LightEnableGet;

    //-------------------------------------------------
    // Shader
    //-------------------------------------------------
    PXShaderVariableSetFunction ShaderVariableSet;

    PXGraphicShaderProgramCreateFunction ShaderProgramCreate;
    PXGraphicShaderProgramSelectFunction ShaderProgramSelect;
    PXGraphicShaderProgramDeleteFunction ShaderProgramDelete;

    //-------------------------------------------------
    // Draw
    //-------------------------------------------------
    PXGraphicDrawModeSetFunction DrawModeSet;
    PXGraphicDrawColorRGBFFunction DrawColorRGBAF;
    PXGraphicRectangleDrawFunction RectangleDraw;
    PXGraphicRectangleDrawTxFunction RectangleDrawTx;
    //-------------------------------------------------

    union
    {
        PXDirectX DirectXInstance;
        PXOpenGL OpenGLInstance;
        PXVulcan VulcanInstance;
    };

    PXGraphicSystem GraphicSystem;

    PXSize DevicePhysicalListSize;
    PXGPUPhysical* DevicePhysicalList;

#if OSUnix
    Display* DisplayConnection;
#endif
}
PXGraphic;



PXPublic PXGraphic* PXAPI PXGraphicInstantiateGET(void);


//-----------------------------------------------------
PXPublic PXResult PXAPI PXGraphicInstantiate(PXGraphic PXREF pxGraphic, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXGraphicRelease(PXGraphic PXREF pxGraphic);
PXPublic PXResult PXAPI PXGraphicHotSwap(PXGraphic PXREF pxGraphic, const PXGraphicSystem pxGraphicSystem);
//-----------------------------------------------------





typedef void (PXAPI* PXGraphicUIElementTrigger)(void* sender, PXWindow PXREF pxWindow);

//-------------------------------------------------------------------------
PXPublic void PXAPI PXUIElementColorSet4F(PXWindow PXREF pxWindow, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha);
PXPublic void PXAPI PXUIElementSizeSet(PXWindow PXREF pxWindow, const PXF32 x, const PXF32 y, const PXF32 width, const PXF32 height, const PXI32U pxUIElementPositionMode);

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
PXPublic void PXAPI PXRenderableConstruct(PXRenderable PXREF pxRenderable);

PXPublic void PXAPI PXTextureConstruct(PXTexture PXREF texture);


//-------------------------------------------------------------------------

//-----------------------------------------------------
// Sprite
//-----------------------------------------------------
PXPublic PXResult PXAPI PXGraphicSpriteConstruct(PXGraphic PXREF pxGraphic, PXSprite PXREF pxSprite);



PXPublic void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment PXREF pxRenderableMeshSegment);


PXPublic void PXAPI PXGraphicModelShaderSet(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXShaderProgram PXREF shaderPXProgram);


//PXPublic PXResult PXGraphicModelGenerate(PXGraphic PXREF pxGraphic, PXRenderable* PXREF renderable, const PXASCII filePath);
//PXPublic PXResult PXGraphicModelLoad(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXText PXREF filePath);
//PXPublic PXResult PXGraphicModelRegisterFromModel(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXModel PXREF model);
//-------------------------------------------------------------------------



PXPublic void PXAPI PXCameraConstruct(PXCamera PXREF camera);


//-----------
PXPublic PXF32 PXAPI PXCameraAspectRatio(const PXCamera PXREF camera);
PXPublic void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height);

PXPublic void PXAPI PXCameraViewChange(PXCamera PXREF camera, const PXCameraPerspective cameraPerspective);
PXPublic void PXAPI PXCameraViewChangeToOrthographic(PXCamera PXREF camera, const PXSize width, const PXSize height, const PXF32 nearPlane, const PXF32 farPlane);
PXPublic void PXAPI PXCameraViewChangeToPerspective(PXCamera PXREF camera, const PXF32 fieldOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane);
//-----------

//---<Transform>-----------------------------------------------------------
PXPublic void PXAPI PXCameraRotate(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F);
PXPublic void PXAPI PXCameraRotateXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXCameraMove(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F);
PXPublic void PXAPI PXCameraMoveXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXCameraFollow(PXCamera PXREF camera, const PXF32 deltaTime);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic void PXAPI PXCameraUpdate(PXCamera PXREF camera, const PXF32 deltaTime);
//-------------------------------------------------------------------------

#endif
