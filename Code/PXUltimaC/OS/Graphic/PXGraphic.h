#ifndef PXGraphicINCLUDE
#define PXGraphicINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXGraphicUSE OSDeviceToUse == OSDeviceDestop
#if PXGraphicUSE

#include <Media/PXType.h>
#include <Media/PXImage.h>
#include <Media/PXColor.h>
#include <Math/PXMatrix.h>
#include <Container/LinkedList/PXLinkedList.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Async/PXLock.h>
#include <Media/PXColor.h>

#include <Media/PXResource.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>


#define PXShaderNotRegisterd (unsigned int)-1

#ifdef __cplusplus
extern "C"
{
#endif

	

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



	typedef struct PXGraphicInitializeInfo_
	{
		PXWindow* WindowReference;

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

	typedef enum PXUIHoverState_
	{
		PXUIHoverStateInvalid,
		PXUIHoverStateNotBeeingHovered, // Not beeing hovered
		PXUIHoverStateHovered, // IS beeing hovered
		PXUIHoverStateHoveredButOverlapped // User hovers over this object but its been blocked by other object
	}
	PXUIHoverState;

	typedef enum PXUIElementType_
	{
		PXUIElementTypeInvalid,
		PXUIElementTypePanel,
		PXUIElementTypeText,
		PXUIElementTypeButton,
		PXUIElementTypeImage,

		PXUIElementTypeDropDown,
		PXUIElementTypeToggle,
		PXUIElementTypeCheckBox,
		PXUIElementTypeColorPicker,
		PXUIElementTypeSlider,
		PXUIElementTypeRadioButton,
		PXUIElementTypeToolTip,

		PXUIElementTypeRenderFrame,

		PXUIElementTypeCustom
	}
	PXUIElementType;




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


	typedef struct PXUIElement_ PXUIElement;

	typedef void (PXAPI*PXUIOnClick)(PXUIElement* const pxUIElement);
	typedef void (PXAPI*PXUIOnMouseEnter)(PXUIElement* const pxUIElement);
	typedef void (PXAPI*PXUIOnMouseLeave)(PXUIElement* const pxUIElement);


	typedef struct PXUIElementFrameBufferInfo_
	{
		PXTexture2D* TextureReference;

		PXInt32U Width;
		PXInt32U Height;
		PXInt32U BufferID;
		PXInt32U RenderID;
	}
	PXUIElementFrameBufferInfo;

	typedef struct PXUIElementImageInfo_
	{
		PXTexture2D* TextureReference;
	}
	PXUIElementImageInfo;

	typedef struct PXUITextInfo_
	{
		char Content[32];
		PXFont* FontID;
		float Scale;
	}
	PXUITextInfo;

#define PXUIElementDoRendering (1 << 0) 
#define PXUIElementIsActive (1 << 1)
#define PXUIElementIsHoverable (1 << 2)
#define PXUIElementDrawBorder (1 << 3)

#define PXUIElementNormal PXUIElementDoRendering | PXUIElementIsActive | PXUIElementIsHoverable
#define PXUIElementDecorative PXUIElementDoRendering | PXUIElementIsActive | PXUIElementDrawBorder
#define PXUIElementText PXUIElementDoRendering | PXUIElementIsActive 

#define PXUIElementAncerParent  0b11110000

#define PXUIElementAncerParentLeft 0b10000000
#define PXUIElementAncerParentTop 0b01000000
#define PXUIElementAncerParentRight 0b00100000
#define PXUIElementAncerParentBottom 0b00010000

#define PXUIElementAncerSibling 0b00001111

#define PXUIElementAncerSiblingLeft 0b00001000
#define PXUIElementAncerSiblingTop 0b00000100
#define PXUIElementAncerSiblingRight 0b00000010
#define PXUIElementAncerSiblingBottom 0b00000001

#define PXUIElementPositionGlobal 0
#define PXUIElementPositionRelative PXUIElementAncerParent


	// Atomic UI-Element
	// Only Text can be text
	// Only image can be image
	typedef struct PXUIElement_
	{
		//------------------------------
		// References 
		//------------------------------
		PXUIElement* Parent;
		PXUIElement* Sibling;
		PXUIElement* Child;

		//------------------------------
		// Events
		//------------------------------
		PXUIOnClick OnClickCallback;
		PXUIOnMouseEnter OnMouseEnterCallback;
		PXUIOnMouseLeave OnMouseLeaveCallback;


		//------------------------------
		// Position
		//------------------------------
		PXRectangleOffset Margin;
		PXRectangleOffset Padding;
		PXInt8U AncerFlagList;


		//---<State-Info>------------------------
		PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
		PXUIHoverState Hover;
		PXInt32U FlagsList; 
		//---------------------------------------

		//---<Property>--------------------------
		PXInt16U ID;
		PXUIElementType Type;
		//PXUIElementPositionMode PositionMode;

		union
		{
			PXUIElementFrameBufferInfo FrameBufferInfo;
			PXUIElementImageInfo ImageInfo;
			PXUITextInfo TextInfo;
		};	
	}
	PXUIElement;

	//const PXSize element = sizeof(PXUIElement);
	//const PXSize possibleElements = 4096 / sizeof(PXUIElement);







	//-------------------------------------------------------------------------
	// Define generic graphic API, make happen that you can use whatever
	//-------------------------------------------------------------------------
	typedef PXActionResult(PXAPI*PXGraphicInitializeFunction)(void* const pxGraphicAPI, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
	typedef PXBool(PXAPI*PXGraphicReleaseFunction)(void* const pxGraphicAPI);

	typedef void (PXAPI*PXGraphicSelectFunction)(void* const pxGraphicAPI);
	typedef void (PXAPI*PXGraphicDeselectFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI*PXGraphicLoadFunction)(void* const pxGraphicAPI);
	typedef PXActionResult(PXAPI*PXGraphicUnloaddFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI*PXGraphicViewPortSetFunction)(void* const pxGraphicAPI, const PXViewPort* const pxViewPort);
	typedef PXActionResult(PXAPI*PXGraphicViewPortGetFunction)(void* const pxGraphicAPI, PXViewPort* const pxViewPort);

	typedef void (PXAPI*PXGraphicSceneBeginFunction)(void* const pxGraphicAPI);
	typedef void (PXAPI*PXGraphicSceneEndFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI* PXGraphicScreenBufferReadFunction)(void* const pxGraphicAPI, PXImage* const pxImage);

	typedef PXActionResult(PXAPI*PXGraphicDrawScriptCreate)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptBegin)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptEnd)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptDelete)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptExecute)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);

	typedef PXActionResult(PXAPI*PXGraphicTextureCubeRegisterFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(PXAPI*PXGraphicTextureCubeRegisterUseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(PXAPI*PXGraphicTextureCubeReleaseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);


	typedef PXActionResult(PXAPI*PXGraphicModelRegisterFunction)(void* const pxGraphicAPI, PXModel* const pxModel);
	typedef PXActionResult(PXAPI*PXGraphicModelDrawFunction)(void* const pxGraphicAPI, PXModel* const pxModel, const PXCamera* const pxCamera);
	typedef PXActionResult(PXAPI*PXGraphicModelSelectFunction)(void* const pxGraphicAPI, PXModel* const pxModel);
	typedef PXActionResult(PXAPI*PXGraphicModelReleaseFunction)(void* const pxGraphicAPI, PXModel* const pxModel);

	typedef PXActionResult(PXAPI*PXGraphicLightSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
	typedef PXActionResult(PXAPI*PXGraphicLightGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
	typedef PXActionResult(PXAPI*PXGraphicLightEnableSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
	typedef PXActionResult(PXAPI*PXGraphicLightEnableGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


	typedef PXActionResult(PXAPI*PXGraphicTexture2DRegisterFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);
	typedef PXActionResult(PXAPI*PXGraphicTexture2DReleaseFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);
	typedef PXActionResult(PXAPI*PXGraphicTexture2DSelectFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);

	typedef void (PXAPI*PXGraphicClearFunction)(void* const pxGraphicAPI, const PXColorRGBAF* const backgroundColor);
	typedef PXBool(PXAPI*PXGraphicSceneDeployFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListAmountFunction)(void* const graphicAPI, PXInt32U* const amount);
	typedef PXActionResult(PXAPI* PXGraphicDevicePhysicalListFetchFunction)(void* const graphicAPI, const PXInt32U amount, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);

	typedef PXActionResult (PXAPI* PXGraphicSwapIntervalSetFunction)(void* const graphicAPI, const PXInt32U interval);
	typedef PXActionResult (PXAPI* PXGraphicSwapIntervalGetFunction)(void* const graphicAPI, PXInt32U* const interval);


	typedef PXActionResult(PXAPI* PXGraphicRectangleDrawFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB, const char mode);
	typedef PXActionResult(PXAPI* PXGraphicRectangleDrawTxFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB, const float txA, const float tyA, const float txB, const float tyB, const char mode);


	typedef PXActionResult(PXAPI* PXGraphicDrawColorRGBFFunction)(void* const graphicAPI, const float red, const float green, const float blue, const float alpha);

	typedef PXActionResult (PXAPI* PXGraphicDrawModeSetFunction)(void* const graphicAPI, const PXGraphicDrawFillMode pxGraphicDrawFillMode);


	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromFileVPFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, const PXText* const vertexShaderFilePath, const PXText* const fragmentShaderFilePath);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromFileVPAFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromStringVPFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, const PXText* const vertexShaderFilePath, const PXText* const fragmentShaderFilePath);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromStringVPAFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);


	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramSelectFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramDeleteFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram);


	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIDFetchFunction)(void* const graphicAPI, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name);

	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx1Function)(void* const graphicAPI, const PXInt32U location, const float v0);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx1xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx1Function)(void* const graphicAPI, const PXInt32U location, const PXInt32U v0);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx1xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXInt32U* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx2Function)(void* const graphicAPI, const PXInt32U location, const float v0, const float v1);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx2xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx2Function)(void* const graphicAPI, const PXInt32U location, const PXInt32U v0, const PXInt32U v1);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx2xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXInt32U* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx3Function)(void* const graphicAPI, const PXInt32U location, const float v0, const float v1, const float v2);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx3xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx3Function)(void* const graphicAPI, const PXInt32U location, const PXInt32U v0, const PXInt32U v1, const PXInt32U v2);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx3xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXInt32U* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx4Function)(void* const graphicAPI, const PXInt32U location, const float v0, const float v1, const float v2, const float v3);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableFx4xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx4Function)(void* const graphicAPI, const PXInt32U location, const PXInt32U v0, const PXInt32U v1, const PXInt32U v2, const PXInt32U v3);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableIx4xNFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXInt32U* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableMatrix2fvFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXBool transpose, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableMatrix3fvFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXBool transpose, const float* const value);
	typedef PXActionResult(PXAPI* PXGraphicShaderVariableMatrix4fvFunction)(void* const graphicAPI, const PXInt32U location, const PXSize count, const PXBool transpose, const float* const value);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------


	typedef struct PXGraphic_
	{
		//-------------------
		// References
		//-------------------
		PXWindow* AttachedWindow;
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
		PXGraphicTexture2DRegisterFunction Texture2DRegister;
		PXGraphicTexture2DReleaseFunction Texture2DRelease;
		PXGraphicTexture2DSelectFunction  Texture2DSelect;

		PXGraphicTextureCubeRegisterFunction TextureCubeRegister;
		PXGraphicTextureCubeRegisterUseFunction TextureCubeRegisterUse;
		PXGraphicTextureCubeReleaseFunction TextureCubeRelease;


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
		PXGraphicShaderProgramCreateFromFileVPFunction ShaderProgramCreateFromFileVP;
		PXGraphicShaderProgramCreateFromFileVPAFunction ShaderProgramCreateFromFileVPA;
		PXGraphicShaderProgramCreateFromStringVPFunction ShaderProgramCreateFromStringVP;
		PXGraphicShaderProgramCreateFromStringVPAFunction ShaderProgramCreateFromStringVPA;

		PXGraphicShaderProgramCreateFunction ShaderProgramCreate;
		PXGraphicShaderProgramSelectFunction ShaderProgramSelect;
		PXGraphicShaderProgramDeleteFunction ShaderProgramDelete;
		PXGraphicShaderVariableIDFetchFunction  ShaderVariableIDFetch;
		PXGraphicShaderVariableFx1Function  ShaderVariableFx1;
		PXGraphicShaderVariableFx1xNFunction  ShaderVariableFx1xN;
		PXGraphicShaderVariableIx1Function  ShaderVariableIx1;
		PXGraphicShaderVariableIx1xNFunction  ShaderVariableIx1xN;
		PXGraphicShaderVariableFx2Function  ShaderVariableFx2;
		PXGraphicShaderVariableFx2xNFunction  ShaderVariableFx2xN;
		PXGraphicShaderVariableIx2Function  ShaderVariableIx2;
		PXGraphicShaderVariableIx2xNFunction ShaderVariableIx2xN;
		PXGraphicShaderVariableFx3Function  ShaderVariableFx3;
		PXGraphicShaderVariableFx3xNFunction  ShaderVariableFx3xN;
		PXGraphicShaderVariableIx3Function  ShaderVariableIx3;
		PXGraphicShaderVariableIx3xNFunction  ShaderVariableIx3xN;
		PXGraphicShaderVariableFx4Function  ShaderVariableFx4;
		PXGraphicShaderVariableFx4xNFunction  ShaderVariableFx4xN;
		PXGraphicShaderVariableIx4Function  ShaderVariableIx4;
		PXGraphicShaderVariableIx4xNFunction  ShaderVariableIx4xN;
		PXGraphicShaderVariableMatrix2fvFunction  ShaderVariableMatrix2fv;
		PXGraphicShaderVariableMatrix3fvFunction  ShaderVariableMatrix3fv;
		PXGraphicShaderVariableMatrix4fvFunction  ShaderVariableMatrix4fv;



		//-------------------------------------------------
		// Draw
		//-------------------------------------------------
		PXGraphicDrawModeSetFunction DrawModeSet;
		PXGraphicDrawColorRGBFFunction DrawColorRGBAF;
		PXGraphicRectangleDrawFunction RectangleDraw;
		PXGraphicRectangleDrawTxFunction RectangleDrawTx;	
		//-------------------------------------------------



		PXGraphicSystem GraphicSystem;

		union
		{
			PXDirectX DirectXInstance;
			PXOpenGL OpenGLInstance;
			PXVulcan VulcanInstance;
		};


		PXLock _resourceLock;

		PXSkyBox* _currentSkyBox;

		//---<Registered Objects>---
		PXInt32U UniqeIDGeneratorCounter;

		PXLinkedListFixed _renderList; // PXRenderable
	
		PXDictionary ResourceImageLookUp;

		PXDictionary UIElementLookUp;
		PXDictionary TextureLookUp;
		PXDictionary SpritelLookUp;
		PXDictionary ModelLookUp;
		PXDictionary FontLookUp;
		PXDictionary SoundLookup;
		PXDictionary ShaderPXProgramLookup;
		//--------------------------



		// Cached most-common objects
		PXModel SpriteScaled;
		PXModel SpriteUnScaled;


		PXSize DevicePhysicalListSize;
		PXGraphicDevicePhysical* DevicePhysicalList;



		PXInt32U UIElementIDCounter;
		PXUIElement UIElementBase;

	}
	PXGraphic;

#if 0
	const PXSize size = sizeof(PXGraphic);
#endif 

	typedef void (PXAPI* PXGraphicUIElementTrigger)(void* sender, PXUIElement* const pxUIElement);


	//-------------------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicUIElementCreate(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement, const PXSize amount, PXUIElement* const pxUIElementParrent);
	PXPublic PXActionResult PXAPI PXGraphicUIElementDelete(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement);

	PXPublic PXActionResult PXAPI PXGraphicUIElementTypeSet(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType);
	PXPublic void PXAPI PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha);
	PXPublic void PXAPI PXUIElementSizeSet(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXInt32U pxUIElementPositionMode);

	PXPublic void PXAPI PXGraphicPXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText);
	PXPublic void PXAPI PXGraphicPXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text);
	PXPublic void PXAPI PXGraphicPXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...);
	PXPublic void PXAPI PXGraphicPXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont);
	PXPublic void PXAPI PXGraphicUIElementFlagSet(PXUIElement* const pxUIElement, const PXInt32U flagList);
	PXPublic void PXAPI PXGraphicPXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);
	PXPublic void PXAPI PXGraphicPXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);

	PXPublic PXActionResult PXAPI PXGraphicUIElementIterator(PXGraphic* const pxGraphic, void* sender, PXGraphicUIElementTrigger preFound, PXGraphicUIElementTrigger postFound);
	PXPublic PXActionResult PXAPI PXGraphicUIElementPrint(PXGraphic* const pxGraphic);
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	PXPublic void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable);

	PXPublic void PXAPI PXShaderConstruct(PXShader* const shader);

	PXPublic void PXAPI PXShaderDataSet(PXShader* const shader, const PXGraphicShaderType type, const char* data, const PXSize size);
	PXPublic void PXAPI PXTextureConstruct(PXTexture2D* const texture);
	PXPublic void PXAPI PXTextureDestruct(PXTexture2D* const texture);

	PXPrivate PXInt32U PXAPI PXGraphicGenerateUniqeID(PXGraphic* const pxGraphic);

	PXPublic PXActionResult PXAPI PXGraphicUIElementRegister(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXAPI PXGraphicUIElementUpdate(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXAPI PXGraphicUIElementUnregister(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement);
	//-------------------------------------------------------------------------

	//-----------------------------------------------------
	// Sprite
	//-----------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphic* const pxGraphic, PXSprite* const pxSprite);
	PXPublic PXActionResult PXAPI PXGraphicSpriteDraw(PXGraphic* const pxGraphic, const PXSprite* const pxSprite, const PXCamera* const pxCamera);
	PXPublic PXActionResult PXAPI PXGraphicSpriteTextureLoadA(PXGraphic* const pxGraphic, PXSprite* const pxSprite, const char* textureFilePath);
	PXPublic PXActionResult PXAPI PXGraphicSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y);
	PXPublic PXActionResult PXAPI PXGraphicSpriteRegister(PXGraphic* const pxGraphic, PXSprite* const pxSprite);


	PXPublic void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);


	//-------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicInstantiate(PXGraphic* const pxGraphic, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
	PXPublic PXActionResult PXAPI PXGraphicRelease(PXGraphic* const pxGraphic);

	PXPublic void PXAPI PXGraphicHotSwap(PXGraphic* const pxGraphic, const PXGraphicSystem pxGraphicSystem);

	PXPublic void PXAPI PXGraphicResourceRegister(PXGraphic* const pxGraphic, PXGraphicResourceInfo* const pxGraphicResourceInfo);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------

	PXPublic void PXAPI PXGraphicShaderUpdateMatrix4x4F(PXGraphic* const pxGraphic, const unsigned int locationID, const float* const matrix4x4);

	//-------------------------------------------------------------------------

	// Load image resource and register it to prevent multible loads of the same file
	PXPublic PXActionResult PXAPI PXGraphicLoadImage(PXGraphic* const pxGraphic, PXImage* const pxImage, const PXText* const pxImageFilePath);


	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicTexture2DLoad(PXGraphic* const pxGraphic, PXTexture2D* const texture, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXGraphicTexture2DLoadA(PXGraphic* const pxGraphic, PXTexture2D* const texture, const char* const filePath);
	//-------------------------------------------------------------------------


	//---<Font>----------------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicFontLoad(PXGraphic* const pxGraphic, PXFont* const pxFont, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXGraphicFontRegister(PXGraphic* const pxGraphic, PXFont* const pxFont);
	PXPublic PXActionResult PXAPI PXGraphicFontRelease(PXGraphic* const pxGraphic, PXFont* const pxFont);
	PXPublic PXActionResult PXAPI PXGraphicFontUse(PXGraphic* const pxGraphic, PXFont* const pxFont);
	//-------------------------------------------------------------------------


	//---<Model>---------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRegister(PXGraphic* const pxGraphic, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRegisterD
	(
		PXGraphic* const pxGraphic,
		PXSkyBox* const skyBox,
		const PXText* const shaderVertex,
		const PXText* const shaderFragment,
		const PXText* const textureRight,
		const PXText* const textureLeft,
		const PXText* const textureTop,
		const PXText* const textureBottom,
		const PXText* const textureBack,
		const PXText* const textureFront
	);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRegisterA
	(
		PXGraphic* const pxGraphic,
		PXSkyBox* const skyBox,
		const char* const shaderVertex,
		const char* const shaderFragment,
		const char* const textureRight,
		const char* const textureLeft,
		const char* const textureTop,
		const char* const textureBottom,
		const char* const textureBack,
		const char* const textureFront
	);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxUse(PXGraphic* const pxGraphic, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRelease(PXGraphic* const pxGraphic, PXSkyBox* const skyBox);

	//PXPublic PXSize PXGraphicModelListSize(const PXGraphic* const pxGraphic);
	//PXPublic PXBool PXGraphicModelListGetFromIndex(const PXGraphic* const pxGraphic, PXModel** pxModel, const PXSize index);

	PXPublic PXSize PXAPI PXGraphicRenderableListSize(const PXGraphic* const pxGraphic);
	PXPublic PXBool PXAPI PXGraphicRenderableListGetFromIndex(const PXGraphic* const pxGraphic, PXRenderable** pxRenderable, const PXSize index);

	//PXPublic PXActionResult PXGraphicModelCreate(PXGraphic* const pxGraphic, PXModel** const pxModel);
	//PXPublic PXBool PXGraphicModelRegister(PXGraphic* const pxGraphic, PXModel* const pxModel);
	PXPublic PXActionResult PXAPI PXGraphicRenderableCreate(PXGraphic* const pxGraphic, PXRenderable** const pxRenderable);
	PXPublic PXBool PXAPI PXGraphicRenderableRegister(PXGraphic* const pxGraphic, PXRenderable* const pxRenderable);


	PXPublic void PXAPI PXGraphicModelShaderSet(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);
	//PXPublic PXActionResult PXGraphicModelGenerate(PXGraphic* const pxGraphic, PXRenderable** const renderable, const PXTextASCII filePath);
	//PXPublic PXActionResult PXGraphicModelLoad(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXText* const filePath);
	//PXPublic PXActionResult PXGraphicModelRegisterFromModel(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXModel* const model);
	//-------------------------------------------------------------------------





	//PXPublic PXActionResult Load(PXModel& model, const wchar_t* filePath, const bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, PXModel* model, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);


	//PXPublic PXActionResult Load(Level& level, const wchar_t* filePath, const bool loadAsynchronously = true);
	// 	PXPublic PXActionResult Load(Collider* collider);

	//PXPublic PXActionResult Load(PXFont& font, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(ShaderPXProgram& shaderPXProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);

	//PXPublic PXActionResult Load(Image& image, const wchar_t* filePath, bool loadAsynchronously = true);



	PXPublic void PXAPI PXCameraConstruct(PXCamera* const camera);
	PXPublic void PXAPI PXCameraDestruct(PXCamera* const camera);


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



#ifdef __cplusplus
}
#endif

#endif
#endif