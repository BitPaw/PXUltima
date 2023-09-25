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





#define PXDeviceDisplaySize 32
#define PXDeviceNameSize 32
#define PXDeviceIDSize 64
#define PXDeviceKeySize 128
	
#define PXDeviceOpenGLDriverSize 32
#define PXDeviceOpenGLVendorSize 32
#define PXDeviceOpenGLRendererSize 32
#define PXDeviceOpenGLShaderSize 32

	typedef struct PXGraphicDevicePhysical_
	{
		char DeviceDisplay[PXDeviceDisplaySize]; // \\.\DISPLAY6
		char DeviceName[PXDeviceNameSize]; // NVIDIA GeForce GTX 1080
		char DeviceID[PXDeviceIDSize]; // Windows DeviceID, "PCI\VEN_10DE&DEV_1B80&SUBSYS_336..."
		char DeviceKey[PXDeviceKeySize]; // Windows Regestry "\Registry\Machine\System\Current..."

		char Driver[PXDeviceOpenGLDriverSize]; // xxxxx.DLL

		char Vendor[PXDeviceOpenGLVendorSize];
		char Renderer[PXDeviceOpenGLRendererSize];
		char Shader[PXDeviceOpenGLShaderSize];

		PXInt64U VideoMemoryDedicated; // dedicated video memory, total size (in kb) of the GPU memory
		PXInt64U VideoMemoryCurrent; // total available memory, total size (in Kb) of the memory available for allocations
		PXInt64U VideoMemoryTotal; // current available dedicated video memory (in kb), currently unused GPU memory

		PXInt64U VideoMemoryEvictionCount; // How many times memory got displaced to Main-RAM
		PXInt64U VideoMemoryEvictionSize; // size of total video memory evicted (in kb)

		PXBool IsConnectedToMonitor;
	}
	PXGraphicDevicePhysical;



	typedef struct PXGraphicContext_ PXGraphicContext;

	typedef struct PXGraphicInitializeInfo_
	{
		PXWindow* WindowReference;

		PXSize Width;
		PXSize Height;	

		PXDirectXVersion DirectXVersion;
		PXDirectXDriverType DirectXDriverType;

		PXGraphicSystem GraphicSystem;

		PXGraphicContext* Graphic;
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


	typedef enum PXUIElementPositionMode_
	{
		PXUIElementPositionInvalid,
		PXUIElementPositionGlobally,
		PXUIElementPositionRelative
	}
	PXUIElementPositionMode;


	typedef struct PXUIElement_ PXUIElement;

	typedef void (*PXUIOnClick)(PXUIElement* const pxUIElement);
	typedef void (*PXUIOnMouseEnter)(PXUIElement* const pxUIElement);
	typedef void (*PXUIOnMouseLeave)(PXUIElement* const pxUIElement);

	typedef struct PXSpriteRectangle_
	{
		PXRectangleF Position;
		PXRectangleF Texture;
		PXColorRGBAF Color;
	}
	PXSpriteRectangle;

	typedef struct PXUITextInfo_
	{
		PXTextUTF8 Text;
		PXInt32U FontID;
		PXInt32U TextShaderID;
	}
	PXUITextInfo;

	typedef struct PXUIElement_
	{
		//---<Render Settings>-------------------
		PXRenderable Renderable;
		PXColorRGBAF BackGroundColor;
		PXFont* FontID;
		PXTexture2D* TextureReference;
		PXShaderProgram* ShaderReference;
		//---------------------------------------

		//---<State-Info>------------------------
		PXBool IsEnabled;
		//PXBool Active;
		PXBool IsHoverable;
		PXUIHoverState Hover;
		//---------------------------------------

		//---<Property>--------------------------
		PXInt16U ID;
		PXUIElementType Type;
		PXUIElementPositionMode PositionMode;
		char Name[32];

		float NameTextScale;

		PXRectangleOffset Margin;
		PXRectangleOffset Padding;
		//---------------------------------------

		//---<Hiracy>----------------------------
		PXUIElement* Parent;
		PXUIElement* Sibling;
		PXUIElement* Child;
		//---------------------------------------

		PXInt32U FrameRenderWidth;
		PXInt32U FrameRenderHeight;
		PXInt32U FrameBufferID;
		PXInt32U FrameRenderID;
		PXTexture2D* FrameRenderTextureReference;


		//---<Events>----------------------------
		PXUIOnClick OnClickCallback;
		PXUIOnMouseEnter OnMouseEnterCallback;
		PXUIOnMouseLeave OnMouseLeaveCallback;
		//---------------------------------------
	}
	PXUIElement;




	//-------------------------------------------------------------------------
	// Define generic graphic API, make happen that you can use whatever
	//-------------------------------------------------------------------------
	typedef PXBool (PXAPI*PXGraphicInitializeFunction)(void* const pxGraphicAPI, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
	typedef PXBool(PXAPI*PXGraphicReleaseFunction)(void* const pxGraphicAPI);

	typedef void (PXAPI*PXGraphicSelectFunction)(void* const pxGraphicAPI);
	typedef void (PXAPI*PXGraphicDeselectFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI*PXGraphicLoadFunction)(void* const pxGraphicAPI);
	typedef PXActionResult(PXAPI*PXGraphicUnloaddFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI*PXGraphicViewPortSetFunction)(void* const pxGraphicAPI, const PXViewPort* const pxViewPort);
	typedef PXActionResult(PXAPI*PXGraphicViewPortGetFunction)(void* const pxGraphicAPI, PXViewPort* const pxViewPort);

	typedef void (PXAPI*PXGraphicSceneBeginFunction)(void* const pxGraphicAPI);
	typedef void (PXAPI*PXGraphicSceneEndFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(PXAPI*PXGraphicDrawScriptCreate)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptBegin)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptEnd)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptDelete)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(PXAPI*PXGraphicDrawScriptExecute)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);

	typedef PXActionResult(PXAPI*PXGraphicTextureCubeRegisterFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(PXAPI*PXGraphicTextureCubeRegisterUseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(PXAPI*PXGraphicTextureCubeReleaseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);


	typedef PXActionResult(PXAPI*PXGraphicVertexStructureRegisterFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);
	typedef PXActionResult(PXAPI*PXGraphicVertexStructureDrawFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure, const PXCamera* const pxCamera);
	typedef PXActionResult(PXAPI*PXGraphicVertexStructureSelectFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);
	typedef PXActionResult(PXAPI*PXGraphicVertexStructureReleaseFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);

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


	typedef PXActionResult(PXAPI* PXGraphicRectangleDrawFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB);
	typedef PXActionResult(PXAPI* PXGraphicRectangleDrawTxFunction)(void* const graphicAPI, const float xA, const float yA, const float xB, const float yB, const float txA, const float tyA, const float txB, const float tyB);


	typedef PXActionResult(PXAPI* PXGraphicDrawColorRGBFFunction)(void* const graphicAPI, const float red, const float green, const float blue, const float alpha);

	typedef PXActionResult (PXAPI* PXGraphicDrawModeSetFunction)(void* const graphicAPI, const PXGraphicDrawFillMode pxGraphicDrawFillMode);


	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromFileVPFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromFileVPAFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
	typedef PXActionResult(PXAPI* PXGraphicShaderProgramCreateFromStringVPFunction)(void* const graphicAPI, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
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


	typedef struct PXGraphicContext_
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
		PXGraphicVertexStructureRegisterFunction VertexStructureRegister;
		PXGraphicVertexStructureDrawFunction VertexStructureDraw;
		PXGraphicVertexStructureSelectFunction VertexStructureSelect;
		PXGraphicVertexStructureReleaseFunction VertexStructureRelease;

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
		PXVertexStructure SpriteScaled;
		PXVertexStructure SpriteUnScaled;


		PXSize DevicePhysicalListSize;
		PXGraphicDevicePhysical* DevicePhysicalList;

	}
	PXGraphicContext;

#if 0
	const PXSize size = sizeof(PXGraphicContext);
#endif 





	//-------------------------------------------------------------------------
	PXPublic void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable);

	PXPublic void PXAPI PXShaderConstruct(PXShader* const shader);

	PXPublic void PXAPI PXShaderDataSet(PXShader* const shader, const PXGraphicShaderType type, const char* data, const PXSize size);
	PXPublic void PXAPI PXTextureConstruct(PXTexture2D* const texture);
	PXPublic void PXAPI PXTextureDestruct(PXTexture2D* const texture);

	PXPublic void PXAPI PXUIElementConstruct(PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType);
	PXPublic void PXAPI PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha);
	PXPublic void PXAPI PXUIElementSizeSet(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXUIElementPositionMode pxUIElementPositionMode);

	PXPublic void PXAPI PXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText);
	PXPublic void PXAPI PXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text);
	PXPublic void PXAPI PXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...);
	PXPublic void PXAPI PXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont);
	PXPublic void PXAPI PXUIElementHoverable(PXUIElement* const pxUIElement, const PXBool isHoverable);
	PXPublic void PXAPI PXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);
	PXPublic void PXAPI PXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);

	PXPrivate PXInt32U PXAPI PXGraphicGenerateUniqeID(PXGraphicContext* const graphicContext);

	PXPublic PXActionResult PXAPI PXGraphicUIElementRegister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXAPI PXGraphicUIElementUpdate(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXAPI PXGraphicUIElementUnregister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	//-------------------------------------------------------------------------

	//-----------------------------------------------------
	// Sprite
	//-----------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphicContext* const graphicContext, PXSprite* const pxSprite);
	PXPublic PXActionResult PXAPI PXGraphicSpriteDraw(PXGraphicContext* const graphicContext, const PXSprite* const pxSprite, const PXCamera* const pxCamera);
	PXPublic PXActionResult PXAPI PXGraphicSpriteTextureLoadA(PXGraphicContext* const graphicContext, PXSprite* const pxSprite, const char* textureFilePath);
	PXPublic PXActionResult PXAPI PXGraphicSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y);
	PXPublic PXActionResult PXAPI PXGraphicSpriteRegister(PXGraphicContext* const graphicContext, PXSprite* const pxSprite);


	PXPublic void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);


	//-------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicInstantiate(PXGraphicContext* const graphicContext, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);

	PXPublic void PXAPI PXGraphicHotSwap(PXGraphicContext* const graphicContext, const PXGraphicSystem pxGraphicSystem);

	PXPublic void PXAPI PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------

	PXPublic void PXAPI PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);

	//-------------------------------------------------------------------------

	// Load image resource and register it to prevent multible loads of the same file
	PXPublic PXActionResult PXAPI PXGraphicLoadImage(PXGraphicContext* const graphicContext, PXImage* const pxImage, const PXText* const pxImageFilePath);


	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const pxImage);

	PXPublic PXActionResult PXAPI PXGraphicTexture2DLoad(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXGraphicTexture2DLoadA(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const char* const filePath);
	//-------------------------------------------------------------------------


	//---<Font>----------------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicFontLoad(PXGraphicContext* const graphicContext, PXFont* const pxFont, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXGraphicFontRegister(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXAPI PXGraphicFontRelease(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXAPI PXGraphicFontUse(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	//-------------------------------------------------------------------------


	//---<Model>---------------------------------------------------------------
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRegister(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRegisterD
	(
		PXGraphicContext* const graphicContext,
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
		PXGraphicContext* const graphicContext,
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
	PXPublic PXActionResult PXAPI PXGraphicSkyboxUse(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXAPI PXGraphicSkyboxRelease(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);

	//PXPublic PXSize PXGraphicModelListSize(const PXGraphicContext* const graphicContext);
	//PXPublic PXBool PXGraphicModelListGetFromIndex(const PXGraphicContext* const graphicContext, PXModel** pxModel, const PXSize index);

	PXPublic PXSize PXAPI PXGraphicRenderableListSize(const PXGraphicContext* const graphicContext);
	PXPublic PXBool PXAPI PXGraphicRenderableListGetFromIndex(const PXGraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index);

	//PXPublic PXActionResult PXGraphicModelCreate(PXGraphicContext* const graphicContext, PXModel** const pxModel);
	//PXPublic PXBool PXGraphicModelRegister(PXGraphicContext* const graphicContext, PXModel* const pxModel);
	PXPublic PXActionResult PXAPI PXGraphicRenderableCreate(PXGraphicContext* const graphicContext, PXRenderable** const pxRenderable);
	PXPublic PXBool PXAPI PXGraphicRenderableRegister(PXGraphicContext* const graphicContext, PXRenderable* const pxRenderable);


	PXPublic void PXAPI PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);
	//PXPublic PXActionResult PXGraphicModelGenerate(PXGraphicContext* const graphicContext, PXRenderable** const renderable, const PXTextASCII filePath);
	//PXPublic PXActionResult PXGraphicModelLoad(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXText* const filePath);
	//PXPublic PXActionResult PXGraphicModelRegisterFromModel(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model);
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