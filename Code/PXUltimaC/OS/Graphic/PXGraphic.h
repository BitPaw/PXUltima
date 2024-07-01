#ifndef PXGraphicINCLUDE
#define PXGraphicINCLUDE

#include <Media/PXType.h>
#include <Media/PXImage.h>
#include <Media/PXColor.h>
#include <Media/PXColor.h>
#include <Media/PXResource.h>
#include <Math/PXMatrix.h>
#include <Container/LinkedList/PXLinkedList.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Async/PXLock.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>

#define PXShaderNotRegisterd (unsigned int)-1

typedef enum PXGraphicSystem_
{
	PXGraphicSystemInvalid,
	PXGraphicSystemOpenGL,
	PXGraphicSystemDirectX,
	PXGraphicSystemVulcan
}
PXGraphicSystem;

typedef PXInt32U PXGraphicResourceID;

typedef struct PXGraphicResourceInfo_
{
	PXGraphicResourceID ID;
	PXGraphicResourceLocation Location;
	PXGraphicSystem System;

	union
	{
		PXMSHandle DirectXID;
		PXInt32U OpengGLID;
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
	PXGUIElement* WindowReference;

#if OSUnix

#elif OSWindows
	HDC HandleDeviceContext;
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
	PXInt32S ScreenResolution[2];
	PXBool FullScreen;
}
PXGraphicConfig;


//typedef struct Renderable_ Renderable; ?

typedef struct PXRenderableMeshSegment_
{
	unsigned int NumberOfVertices;
	unsigned int TextureID;
	unsigned int ShaderID;

	PXGraphicDrawMode RenderMode;

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
typedef PXActionResult(PXAPI* PXGraphicInitializeFunction)(void* const pxGraphicAPI, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
typedef PXBool(PXAPI* PXGraphicReleaseFunction)(void* const pxGraphicAPI);

typedef void (PXAPI* PXGraphicSelectFunction)(void* const pxGraphicAPI);
typedef void (PXAPI* PXGraphicDeselectFunction)(void* const pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicLoadFunction)(void* const pxGraphicAPI);
typedef PXActionResult(PXAPI* PXGraphicUnloaddFunction)(void* const pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicViewPortSetFunction)(void* const pxGraphicAPI, const PXViewPort* const pxViewPort);
typedef PXActionResult(PXAPI* PXGraphicViewPortGetFunction)(void* const pxGraphicAPI, PXViewPort* const pxViewPort);

typedef void (PXAPI* PXGraphicSceneBeginFunction)(void* const pxGraphicAPI);
typedef void (PXAPI* PXGraphicSceneEndFunction)(void* const pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicScreenBufferReadFunction)(void* const pxGraphicAPI, PXImage* const pxImage);

typedef PXActionResult(PXAPI* PXGraphicDrawScriptCreate)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptBegin)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptEnd)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptDelete)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXGraphicDrawScriptExecute)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);


typedef PXActionResult(PXAPI* PXGraphicModelRegisterFunction)(void* const pxGraphicAPI, PXModel* const pxModel);
typedef PXActionResult(PXAPI* PXGraphicModelDrawFunction)(void* const pxGraphicAPI, PXRenderEntity* const pxRenderEntity);
typedef PXActionResult(PXAPI* PXGraphicModelSelectFunction)(void* const pxGraphicAPI, PXModel* const pxModel);
typedef PXActionResult(PXAPI* PXGraphicModelReleaseFunction)(void* const pxGraphicAPI, PXModel* const pxModel);

typedef PXActionResult(PXAPI* PXGraphicLightSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
typedef PXActionResult(PXAPI* PXGraphicLightGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
typedef PXActionResult(PXAPI* PXGraphicLightEnableSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
typedef PXActionResult(PXAPI* PXGraphicLightEnableGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


typedef PXActionResult(PXAPI* PXGraphicTextureActionFunction)(void* const pxGraphicAPI, PXGraphicTexturInfo* const pxGraphicTexturInfo);


typedef void (PXAPI* PXGraphicClearFunction)(void* const pxGraphicAPI, const PXColorRGBAF* const backgroundColor);
typedef PXBool(PXAPI* PXGraphicSceneDeployFunction)(void* const pxGraphicAPI);

typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListAmountFunction)(void* const graphicAPI, PXInt32U* const amount);
typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListFetchFunction)(void* const graphicAPI, const PXInt32U amount, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);

typedef PXActionResult(PXAPI* PXGraphicSwapIntervalSetFunction)(void* const graphicAPI, const PXInt32U interval);
typedef PXActionResult(PXAPI* PXGraphicSwapIntervalGetFunction)(void* const graphicAPI, PXInt32U* const interval);


typedef PXActionResult(PXAPI* PXGraphicRectangleDrawFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB, const char mode);
typedef PXActionResult(PXAPI* PXGraphicRectangleDrawTxFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB, const float txA, const float tyA, const float txB, const float tyB, const char mode);


typedef PXActionResult(PXAPI* PXGraphicDrawColorRGBFFunction)(void* const graphicAPI, const float red, const float green, const float blue, const float alpha);

typedef PXActionResult(PXAPI* PXGraphicDrawModeSetFunction)(void* const graphicAPI, const PXGraphicDrawFillMode pxGraphicDrawFillMode);

typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);
typedef PXActionResult(PXAPI* PXGraphicShaderProgramSelectFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram);
typedef PXActionResult(PXAPI* PXGraphicShaderProgramDeleteFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram);

typedef PXActionResult(PXAPI* PXShaderVariableSetFunction)(void* const graphicAPI, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariable);
//-------------------------------------------------------------------------


typedef struct PXGraphic_
{
	//-------------------
	// References
	//-------------------
	PXGUIElement* WindowReference;
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
	PXGraphicDevicePhysical* DevicePhysicalList;
}
PXGraphic;


//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXGraphicInstantiate(PXGraphic* const pxGraphic, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXActionResult PXAPI PXGraphicRelease(PXGraphic* const pxGraphic);
PXPublic PXActionResult PXAPI PXGraphicHotSwap(PXGraphic* const pxGraphic, const PXGraphicSystem pxGraphicSystem);
//-----------------------------------------------------





typedef void (PXAPI* PXGraphicUIElementTrigger)(void* sender, PXGUIElement* const pxGUIElement);

//-------------------------------------------------------------------------
PXPublic void PXAPI PXUIElementColorSet4F(PXGUIElement* const pxGUIElement, const float red, const float green, const float blue, const float alpha);
PXPublic void PXAPI PXUIElementSizeSet(PXGUIElement* const pxGUIElement, const float x, const float y, const float width, const float height, const PXInt32U pxUIElementPositionMode);

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
PXPublic void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable);

PXPublic void PXAPI PXTextureConstruct(PXTexture2D* const texture);


//-------------------------------------------------------------------------

//-----------------------------------------------------
// Sprite
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphic* const pxGraphic, PXSprite* const pxSprite);



PXPublic void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);


PXPublic void PXAPI PXGraphicModelShaderSet(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);
//PXPublic PXActionResult PXGraphicModelGenerate(PXGraphic* const pxGraphic, PXRenderable** const renderable, const PXTextASCII filePath);
//PXPublic PXActionResult PXGraphicModelLoad(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXText* const filePath);
//PXPublic PXActionResult PXGraphicModelRegisterFromModel(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXModel* const model);
//-------------------------------------------------------------------------



PXPublic void PXAPI PXCameraConstruct(PXCamera* const camera);


//-----------
PXPublic float PXAPI PXCameraAspectRatio(const PXCamera* const camera);
PXPublic void PXAPI PXCameraAspectRatioChange(PXCamera* const camera, const PXSize width, const PXSize height);

PXPublic void PXAPI PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective);
PXPublic void PXAPI PXCameraViewChangeToOrthographic(PXCamera* const camera, const PXSize width, const PXSize height, const float nearPlane, const float farPlane);
PXPublic void PXAPI PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane);
//-----------

//---<Transform>-----------------------------------------------------------
PXPublic void PXAPI PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F);
PXPublic void PXAPI PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z);

PXPublic void PXAPI PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F);
PXPublic void PXAPI PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z);

PXPublic void PXAPI PXCameraFollow(PXCamera* const camera, const float deltaTime);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic void PXAPI PXCameraUpdate(PXCamera* const camera, const float deltaTime);
//-------------------------------------------------------------------------

#endif