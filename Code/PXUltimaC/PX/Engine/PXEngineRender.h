#pragma once

#ifndef PXRenderingSystemIncluded
#define PXRenderingSystemIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMatrix.h>
#include <PX/Type/PXScene.h>
#include <PX/Type/PXSkyBox.h>
#include <PX/Type/PXColor.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/Graphic/DirectX/PXDirectX.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>
#include <PX/OS/Graphic/Vulcan/PXVulcan.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXTexture.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXDrawScript.h>
#include <PX/Type/PXLight.h>
#include <PX/Type/PXViewPort.h>
#include <PX/Type/PXGPU.h>
#include <PX/Engine/PXEngineModule.h>

//#include <PX/Type/Camera/PXCamera.h>
//#include <PX/Type/Shader/PXShader.h>
//#include <PX/Type/Material/PXMaterial.h>
//#include <PX/Type/Texture/PXTexture.h>




typedef struct PXGraphicConfig_
{
    PXRefreshRateMode WindowRefreshRateMode;
    PXI32S ScreenResolution[2];
    PXBool FullScreen;
}
PXGraphicConfig;



typedef enum PXEngineUIState
{
    UIStateInvalid,
    UIStateActive,
    UIStateHot,
    UIStateIDLE
}
PXEngineUIState;

typedef enum PXEngineUIScalingType_
{
    UIScalingInvalid,
    UIScalingRelativeToParent
}
PXEngineUIScalingType;

typedef struct UIContainerRoot_
{
    unsigned char _SUMMY__;
}
UIContainerRoot;



//-------------------------------------------------------------------------
// Define generic graphic API, make happen that you can use whatever
//-------------------------------------------------------------------------
typedef PXResult(PXAPI* PXGPUInitializeFunction)(void PXREF pxGPUAPI, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo);
typedef PXBool(PXAPI* PXGPUReleaseFunction)(void PXREF pxGPUAPI);

typedef PXResult(PXAPI* PXGPUSelectFunction)(void PXREF pxGPUAPI, PXWindow PXREF pxWindow);
typedef PXResult(PXAPI* PXGPUDeselectFunction)(void PXREF pxGPUAPI, PXWindow PXREF pxWindow);

typedef PXResult(PXAPI* PXGPULoadFunction)(void PXREF pxGPUAPI);
typedef PXResult(PXAPI* PXGPUUnloaddFunction)(void PXREF pxGPUAPI);

typedef PXResult(PXAPI* PXGPUViewPortSetFunction)(void PXREF pxGPUAPI, const PXViewPort PXREF pxViewPort);
typedef PXResult(PXAPI* PXGPUViewPortGetFunction)(void PXREF pxGPUAPI, PXViewPort PXREF pxViewPort);

typedef void (PXAPI* PXGPUSceneBeginFunction)(void PXREF pxGPUAPI);
typedef void (PXAPI* PXGPUSceneEndFunction)(void PXREF pxGPUAPI);

typedef PXResult(PXAPI* PXGPUScreenBufferReadFunction)(void PXREF pxGPUAPI, PXTexture2D PXREF pxTexture2D);

typedef PXResult(PXAPI* PXGPUDrawScriptCreate)(void PXREF pxGPUAPI, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType);
typedef PXResult(PXAPI* PXGPUDrawScriptBegin)(void PXREF pxGPUAPI, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXGPUDrawScriptEnd)(void PXREF pxGPUAPI, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXGPUDrawScriptDelete)(void PXREF pxGPUAPI, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXGPUDrawScriptExecute)(void PXREF pxGPUAPI, PXDrawScript PXREF pxDrawScript);


typedef PXResult(PXAPI* PXGPUModelRegisterFunction)(void PXREF pxGPUAPI, PXModel PXREF pxModel);
typedef PXResult(PXAPI* PXGPUModelDrawFunction)(void PXREF pxGPUAPI, PXScene PXREF pxScene, PXModel PXREF pxModel);
typedef PXResult(PXAPI* PXGPUModelSelectFunction)(void PXREF pxGPUAPI, PXModel PXREF pxModel);
typedef PXResult(PXAPI* PXGPUModelReleaseFunction)(void PXREF pxGPUAPI, PXModel PXREF pxModel);

typedef PXResult(PXAPI* PXGPULightSetFunction)(void PXREF pxGPUAPI, PXLight PXREF pxLight, const PXI32U index);
typedef PXResult(PXAPI* PXGPULightGetFunction)(void PXREF pxGPUAPI, PXLight PXREF pxLight, const PXI32U index);
typedef PXResult(PXAPI* PXGPULightEnableSetFunction)(void PXREF pxGPUAPI, PXLight PXREF pxLight, const PXI32U index, const PXBool enable);
typedef PXResult(PXAPI* PXGPULightEnableGetFunction)(void PXREF pxGPUAPI, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable);

typedef PXResult(PXAPI* PXGPUTexture1DRegisterFunction)(void PXREF pxGPUAPI, PXTexture1D PXREF pxTexture1D);
typedef PXResult(PXAPI* PXGPUTexture2DRegisterFunction)(void PXREF pxGPUAPI, PXTexture2D PXREF pxTexture2D);
typedef PXResult(PXAPI* PXGPUTexture3DRegisterFunction)(void PXREF pxGPUAPI, PXTexture3D PXREF pxTexture3D);
typedef PXResult(PXAPI* PXGPUTextureCubeRegisterFunction)(void PXREF pxGPUAPI, PXTextureCube PXREF pxTextureCube);



typedef PXResult(PXAPI* PXGPUClearFunction)(void PXREF pxGPUAPI, const PXColorRGBA_F32 PXREF backgroundColor);
typedef PXBool(PXAPI* PXGPUSceneDeployFunction)(void PXREF pxGPUAPI);

typedef PXResult(PXAPI* PXGPUDevicePhysicalListAmountFunction)(void PXREF graphicAPI, PXI32U PXREF amount);
typedef PXResult(PXAPI* PXGPUDevicePhysicalListFetchFunction)(void PXREF graphicAPI, const PXI32U amount, PXGPU PXREF pxGraphicsCardList);


typedef PXSize (PXAPI* PXGPUAmountFuntion)(const PXEngineRender PXREF pxEngineRender);
typedef PXResult(PXAPI* PXGPUListRefreshFuntion)(PXEngineRender PXREF pxEngineRender);
typedef PXResult(PXAPI* PXGPUContextCreateViaIndexFuntion)(PXEngineRender PXREF pxEngineRender, const PXSize index);
typedef PXResult(PXAPI* PXGPUContextCreateFuntion)(PXEngineRender PXREF pxEngineRender);


typedef PXResult(PXAPI* PXGPUSwapIntervalSetFunction)(void PXREF graphicAPI, PXWindow PXREF pxWindow, const PXI32U interval);
typedef PXResult(PXAPI* PXGPUSwapIntervalGetFunction)(void PXREF graphicAPI, PXWindow PXREF pxWindow, PXI32U PXREF interval);


typedef PXResult(PXAPI* PXGPURectangleDrawFunction)(void PXREF graphicAPI, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const char mode);
typedef PXResult(PXAPI* PXGPURectangleDrawTxFunction)(void PXREF graphicAPI, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXF32 txA, const PXF32 tyA, const PXF32 txB, const PXF32 tyB, const char mode);


typedef PXResult(PXAPI* PXGPUDrawColorRGBFFunction)(void PXREF graphicAPI, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha);

typedef PXResult(PXAPI* PXGPUDrawModeSetFunction)(void PXREF graphicAPI, const PXDrawFillMode pxGraphicDrawFillMode);

typedef PXResult(PXAPI* PXGPUShaderProgramCreateFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram);
typedef PXResult(PXAPI* PXGPUShaderProgramSelectFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram);
typedef PXResult(PXAPI* PXGPUShaderProgramDeleteFunction)(void PXREF graphicAPI, PXShaderProgram PXREF pxShaderProgram);

typedef PXResult(PXAPI* PXGPUShaderVariableSetFunction)(void PXREF graphicAPI, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariable);
//-------------------------------------------------------------------------




typedef void (PXAPI* PXGraphicUIElementTrigger)(void* sender, PXWindow PXREF pxWindow);

//-------------------------------------------------------------------------
PXPublic void PXAPI PXEngineUIElementColorSet4F(PXWindow PXREF pxWindow, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha);
PXPublic void PXAPI PXEngineUIElementSizeSet(PXWindow PXREF pxWindow, const PXF32 x, const PXF32 y, const PXF32 width, const PXF32 height, const PXI32U pxUIElementPositionMode);

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//PXPublic PXResult PXGraphicModelGenerate(PXGraphic PXREF pxGraphic, PXRenderable* PXREF renderable, const PXASCII filePath);
//PXPublic PXResult PXGraphicModelLoad(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXText PXREF filePath);
//PXPublic PXResult PXGraphicModelRegisterFromModel(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXModel PXREF model);
//-------------------------------------------------------------------------



typedef struct PXEngineRender_
{
    PXEngineModule ModuleInfo;

    // Grapgicpipeline
    //-------------------
    // References
    //-------------------
    //PXWindow* WindowReference;
    //-------------------

    //-------------------
    // Functions
    //-------------------


    //-------------------------------------------------
    // Core
    //-------------------------------------------------
    PXGPUInitializeFunction Initialize;
    PXGPUReleaseFunction Release;
    PXGPUSelectFunction Select;
    PXGPUDeselectFunction Deselect;
    PXGPUClearFunction Clear;


    // GPU-Select
    PXGPUDevicePhysicalListAmountFunction DevicePhysicalListAmount;
    PXGPUDevicePhysicalListFetchFunction DevicePhysicalListFetch;

    //-------------------------------------------------
    // Info & Settings
    //-------------------------------------------------
    PXGPUSwapIntervalSetFunction SwapIntervalSet;
    PXGPUSwapIntervalGetFunction SwapIntervalGet;


    //-------------------------------------------------
    // Scene
    //-------------------------------------------------
    PXGPUSceneBeginFunction SceneBegin;
    PXGPUSceneDeployFunction SceneDeploy;
    PXGPUSceneEndFunction SceneEnd;

    //-------------------------------------------------
    // Screen
    //-------------------------------------------------
    PXGPUScreenBufferReadFunction ScreenBufferRead;

    //-------------------------------------------------
    // View
    //-------------------------------------------------
    PXGPUViewPortSetFunction ViewPortSet;
    PXGPUViewPortGetFunction ViewPortGet;

    //-------------------------------------------------
    // Scripting
    //-------------------------------------------------
    PXGPUDrawScriptCreate DrawScriptCreate;
    PXGPUDrawScriptBegin DrawScriptBegin;
    PXGPUDrawScriptEnd DrawScriptEnd;
    PXGPUDrawScriptDelete DrawScriptDelete;
    PXGPUDrawScriptExecute DrawScriptExecute;

    //-------------------------------------------------
    // Textures
    //-------------------------------------------------
    PXGPUTexture1DRegisterFunction Texture1DRegister;
    PXGPUTexture2DRegisterFunction Texture2DRegister;
    PXGPUTexture3DRegisterFunction Texture3DRegister;
    PXGPUTextureCubeRegisterFunction TextureCubeRegister;


    //-------------------------------------------------
    // Model
    //-------------------------------------------------
    PXGPUModelRegisterFunction ModelRegister;
    PXGPUModelDrawFunction ModelDraw;
    PXGPUModelSelectFunction ModelSelect;
    PXGPUModelReleaseFunction ModelRelease;

    //-------------------------------------------------
    // Light
    //-------------------------------------------------
    PXGPULightSetFunction LightSet;
    PXGPULightGetFunction LightGet;
    PXGPULightEnableSetFunction LightEnableSet;
    PXGPULightEnableGetFunction LightEnableGet;

    //-------------------------------------------------
    // Shader
    //-------------------------------------------------
    PXGPUShaderVariableSetFunction ShaderVariableSet;

    PXGPUShaderProgramCreateFunction ShaderProgramCreate;
    PXGPUShaderProgramSelectFunction ShaderProgramSelect;
    PXGPUShaderProgramDeleteFunction ShaderProgramDelete;

    //-------------------------------------------------
    // Draw
    //-------------------------------------------------
    PXGPUDrawModeSetFunction DrawModeSet;
    PXGPUDrawColorRGBFFunction DrawColorRGBAF;
    PXGPURectangleDrawFunction RectangleDraw;
    PXGPURectangleDrawTxFunction RectangleDrawTx;
    //-------------------------------------------------

    union
    {
        char API[1];
        PXDirectX APIDirectX;
        PXOpenGL APIOpenGL;
        PXVulcan APIVulcan;
    };

    PXGraphicAPI GraphicSystem;

    // RenderSettings
    PXBool DrawWireFrame;    


    PXDictionaryECS GPULookup;
}
PXEngineRender;

typedef struct PXEngineRenderCreateInfo_
{
    PXECSCreateInfo Info;

    PXEngineRender* RenderSystem; // Self assigned reference

    PXGraphicInitializeMode Mode;
    PXWindow* WindowReference;

    PXSize TargetFrameRate;

    PXSize Width;
    PXSize Height;

    PXDirectXVersion DirectXVersion;
    PXDirectXDriverType DirectXDriverType;

    PXGraphicAPI GraphicSystem;
}
PXEngineRenderCreateInfo;

PXPublic void PXAPI PXRenderingSystemRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXEngineRenderCreate(PXEngineRender** pxEngineRenderREF, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo);
PXPublic PXResult PXAPI PXEngineRenderHookAPI(PXEngineRender PXREF pxEngineRender, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo);
PXPublic PXResult PXAPI PXEngineRenderRelease(PXEngineRender PXREF pxEngineRender);


PXPublic PXResult PXAPI PXEngineRenderOnActivate(PXEngineRender PXREF pxEngineRender, PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineRenderOnDeactivate(PXEngineRender PXREF pxEngineRender, PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineRenderUpdate(PXEngineRender PXREF pxEngineRender, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);


PXPublic PXResult PXAPI PXEngineRenderDrawScene(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXEngineRenderDrawModel(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene, PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXEngineRenderDrawSkyBox(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox);


PXPublic PXResult PXAPI PXEngineRenderHotSwap(PXEngineRender PXREF pxEngineRender, const PXGraphicAPI pxGraphicAPI);


PXPublic PXResult PXAPI PXEngineRenderGPUMerge(PXEngineRender PXREF pxEngineRender, PXGPU PXREF pxGPU);


// We ask the system seperately what GPUs we have.
// APIs like OpenGL, DIrectX and vulcan cant list devices that dont expose themself.
PXPublic PXResult PXAPI PXEngineRenderGPUListRefresh(PXEngineRender PXREF pxEngineRender);
PXPublic PXSize PXAPI PXEngineRenderGPUListAmount(const PXEngineRender PXREF pxEngineRender);
PXPublic PXGPU* PXAPI PXEngineRenderGPUGetViaIndex(const PXEngineRender PXREF pxEngineRender, const PXSize index);



PXPublic PXResult PXAPI PXEngineRenderDisplayListRefresh(PXEngineRender PXREF pxEngineRender);



PXPublic PXResult PXAPI PXEngineRenderGPUAffinityCreate(PXEngineRender PXREF pxEngineRender, const PXSize index);

#endif