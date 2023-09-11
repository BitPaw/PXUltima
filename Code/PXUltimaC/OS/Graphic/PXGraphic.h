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













	PXPublic void PXShaderConstruct(PXShader* const shader);

	PXPublic void PXShaderDataSet(PXShader* const shader, const PXGraphicShaderType type, const char* data, const PXSize size);





	

	PXPublic void PXTextureConstruct(PXTexture2D* const texture);
	PXPublic void PXTextureDestruct(PXTexture2D* const texture);


	


	

	 




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

	PXPublic void PXRenderableConstruct(PXRenderable* const pxRenderable);




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
		float X;
		float Y;
		float Width;
		float Height;
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
	typedef void (*PXGraphicSelectFunction)(void* const pxGraphicAPI);
	typedef void (*PXGraphicDeselectFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(*PXGraphicLoadFunction)(void* const pxGraphicAPI);
	typedef PXActionResult(*PXGraphicUnloaddFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(*PXGraphicViewPortSetFunction)(void* const pxGraphicAPI, const PXViewPort* const pxViewPort);
	typedef PXActionResult(*PXGraphicViewPortGetFunction)(void* const pxGraphicAPI, PXViewPort* const pxViewPort);

	typedef PXActionResult(*PXGraphicShaderProgramCreateFunction)(void* const pxGraphicAPI, PXShaderProgram* const pxShaderProgram);
	typedef PXActionResult(*PXGraphicShaderProgramSelectFunction)(void* const pxGraphicAPI, PXShaderProgram* const pxShaderProgram);
	typedef PXActionResult(*PXGraphicShaderProgramDeleteFunction)(void* const pxGraphicAPI, PXShaderProgram* const pxShaderProgram);

	typedef void (*PXGraphicSceneBeginFunction)(void* const pxGraphicAPI);
	typedef void (*PXGraphicSceneEndFunction)(void* const pxGraphicAPI);

	typedef PXActionResult(*PXGraphicDrawScriptCreate)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
	typedef PXActionResult(*PXGraphicDrawScriptBegin)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(*PXGraphicDrawScriptEnd)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(*PXGraphicDrawScriptDelete)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);
	typedef PXActionResult(*PXGraphicDrawScriptExecute)(void* const pxGraphicAPI, PXDrawScript* const pxDrawScript);

	typedef PXActionResult(*PXGraphicTextureCubeRegisterFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(*PXGraphicTextureCubeRegisterUseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);
	typedef PXActionResult(*PXGraphicTextureCubeReleaseFunction)(void* const pxGraphicAPI, PXTextureCube* const textureCube);


	typedef PXActionResult(*PXGraphicVertexStructureRegisterFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);
	typedef PXActionResult(*PXGraphicVertexStructureDrawFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure, const PXCamera* const pxCamera);
	typedef PXActionResult(*PXGraphicVertexStructureSelectFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);
	typedef PXActionResult(*PXGraphicVertexStructureReleaseFunction)(void* const pxGraphicAPI, PXVertexStructure* const pxVertexStructure);

	typedef PXActionResult(*PXGraphicLightSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
	typedef PXActionResult(*PXGraphicLightGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index);
	typedef PXActionResult(*PXGraphicLightEnableSetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
	typedef PXActionResult(*PXGraphicLightEnableGetFunction)(void* const pxGraphicAPI, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


	typedef PXActionResult(*PXGraphicTexture2DRegisterFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);
	typedef PXActionResult(*PXGraphicTexture2DReleaseFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);
	typedef PXActionResult(*PXGraphicTexture2DSelectFunction)(void* const pxGraphicAPI, PXTexture2D* const pxTexture2D);

	typedef void (*PXGraphicClearFunction)(void* const pxGraphicAPI, const PXColorRGBAF* const backgroundColor);
	typedef PXBool(*PXGraphicSceneDeployFunction)(void* const pxGraphicAPI);
	//-------------------------------------------------------------------------


	typedef struct PXGraphicContext_
	{
		PXDirectX DirectXInstance;
		PXOpenGL OpenGLInstance;

		void* AttachedWindow;

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

		PXGraphicSystem GraphicSystem;

		// Cached most-common objects
		PXVertexStructure SpriteScaled;
		PXVertexStructure SpriteUnScaled;

	

		//-------------------------------------------------
		// API
		//-------------------------------------------------
		void* EventOwner;

		PXGraphicSelectFunction Select;
		PXGraphicDeselectFunction Deselect;
		PXGraphicClearFunction Clear;
		PXGraphicSceneDeployFunction SceneDeploy;
		PXGraphicViewPortSetFunction ViewPortSet;
		PXGraphicViewPortGetFunction ViewPortGet;
		PXGraphicShaderProgramCreateFunction ShaderProgramCreate;
		PXGraphicShaderProgramSelectFunction ShaderProgramSelect;
		PXGraphicShaderProgramDeleteFunction ShaderProgramDelete;
		PXGraphicSceneBeginFunction SceneBegin;
		PXGraphicSceneEndFunction SceneEnd;
		PXGraphicDrawScriptCreate DrawScriptCreate;
		PXGraphicDrawScriptBegin DrawScriptBegin;
		PXGraphicDrawScriptEnd DrawScriptEnd;
		PXGraphicDrawScriptDelete DrawScriptDelete;
		PXGraphicDrawScriptExecute DrawScriptExecute;
		PXGraphicTextureCubeRegisterFunction TextureCubeRegister;
		PXGraphicTextureCubeRegisterUseFunction TextureCubeRegisterUse;
		PXGraphicTextureCubeReleaseFunction TextureCubeRelease;
		PXGraphicVertexStructureRegisterFunction VertexStructureRegister;
		PXGraphicVertexStructureDrawFunction VertexStructureDraw;
		PXGraphicVertexStructureSelectFunction VertexStructureSelect;
		PXGraphicVertexStructureReleaseFunction VertexStructureRelease;
		PXGraphicLightSetFunction LightSet;
		PXGraphicLightGetFunction LightGet;
		PXGraphicLightEnableSetFunction LightEnableSet;
		PXGraphicLightEnableGetFunction LightEnableGet;
		PXGraphicTexture2DRegisterFunction Texture2DRegister;
		PXGraphicTexture2DReleaseFunction Texture2DRelease;
		PXGraphicTexture2DSelectFunction  Texture2DSelect;
		//-------------------------------------------------
	}
	PXGraphicContext;






	//-------------------------------------------------------------------------
	PXPublic void PXUIElementConstruct(PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType);
	PXPublic void PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha);
	PXPublic void PXUIElementPositionSetXYWH(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXUIElementPositionMode pxUIElementPositionMode);

	PXPublic void PXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText);
	PXPublic void PXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text);
	PXPublic void PXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...);
	PXPublic void PXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont);
	PXPublic void PXUIElementHoverable(PXUIElement* const pxUIElement, const PXBool isHoverable);
	PXPublic void PXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);
	PXPublic void PXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent);

	PXPrivate PXInt32U PXGraphicGenerateUniqeID(PXGraphicContext* const graphicContext);

	PXPublic PXActionResult PXGraphicUIElementRegister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXGraphicUIElementUpdate(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXGraphicUIElementUnregister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	//-------------------------------------------------------------------------

	PXPublic PXActionResult PXGraphicSwapIntervalSet(PXGraphicContext* const graphicContext, const PXInt32U interval);
	PXPublic PXActionResult PXGraphicSwapIntervalGet(PXGraphicContext* const graphicContext, PXInt32U* const interval);


	//-----------------------------------------------------
	// Sprite
	//-----------------------------------------------------
	PXPublic PXActionResult PXGraphicSpriteConstruct(PXGraphicContext* const graphicContext, PXSprite* const pxSprite);
	PXPublic PXActionResult PXGraphicSpriteDraw(PXGraphicContext* const graphicContext, const PXSprite* const pxSprite, const PXCamera* const pxCamera);
	PXPublic PXActionResult PXGraphicSpriteTextureLoadA(PXGraphicContext* const graphicContext, PXSprite* const pxSprite, const char* textureFilePath);
	PXPublic PXActionResult PXGraphicSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y);
	PXPublic PXActionResult PXGraphicSpriteRegister(PXGraphicContext* const graphicContext, PXSprite* const pxSprite);

		//-----------------------------------------------------
	// Shader
	//-----------------------------------------------------
	PXPublic PXActionResult PXGraphicShaderProgramCreateVP(PXGraphicContext* const pxGraphicContext, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
	PXPublic PXActionResult PXGraphicShaderProgramCreateVPA(PXGraphicContext* const pxGraphicContext, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);


	PXPublic PXActionResult PXGraphicShaderVariableIDFetch(PXGraphicContext* const graphicContext, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name);

	PXPublic PXActionResult PXGraphicDrawModeSet(PXGraphicContext* const graphicContext, const PXGraphicDrawFillMode pxGraphicDrawFillMode);
	PXPublic void PXGraphicDrawColorRGBF(PXGraphicContext* const graphicContext, const float red, const float green, const float blue);
	PXPublic void PXGraphicDrawColorRGBAF(PXGraphicContext* const graphicContext, const float red, const float green, const float blue, const float alpha);
	PXPublic PXActionResult PXGraphicRectangleDraw(PXGraphicContext* const graphicContext, const float xA, const float yA, const float xB, const float yB);
	PXPublic PXActionResult PXGraphicRectangleDrawTx
	(
		PXGraphicContext* const graphicContext, 
		const float xA, 
		const float yA,
		const float xB, 
		const float yB,
		const float txA,
		const float tyA,
		const float txB,
		const float tyB
	);
	PXPublic void PXGraphicBlendingMode(PXGraphicContext* const graphicContext, const PXBlendingMode pxBlendingMode);



	PXPublic void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);


	//-------------------------------------
	PXPublic void PXGraphicInstantiate(PXGraphicContext* const graphicContext);

	PXPublic void PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------

	PXPublic void PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);

	//-------------------------------------------------------------------------

	//---<Rendering>-----------------------------------------------------------

	PXPublic PXActionResult PXGraphicRenderElement(PXGraphicContext* const graphicContext, PXGraphicDrawMode renderMode, PXSize start, PXSize amount);
	PXPublic PXActionResult PXGraphicRenderList(PXGraphicContext* const graphicContext, PXGraphicDrawMode renderMode, PXSize start, PXSize amount);
	//-------------------------------------------------------------------------

	// Load image resource and register it to prevent multible loads of the same file
	PXPublic PXActionResult PXGraphicLoadImage(PXGraphicContext* const graphicContext, PXImage* const pxImage, const PXText* const pxImageFilePath);


	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const pxImage);

	PXPublic PXActionResult PXGraphicTexture2DLoad(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const PXText* const filePath);
	PXPublic PXActionResult PXGraphicTexture2DLoadA(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const char* const filePath);
	//-------------------------------------------------------------------------


	//---<Font>----------------------------------------------------------------------
	PXPublic PXActionResult PXGraphicFontLoad(PXGraphicContext* const graphicContext, PXFont* const pxFont, const PXText* const filePath);
	PXPublic PXActionResult PXGraphicFontRegister(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXGraphicFontRelease(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXGraphicFontUse(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	//-------------------------------------------------------------------------


	//---<Model>---------------------------------------------------------------
	PXPublic PXActionResult PXGraphicSkyboxRegister(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXGraphicSkyboxRegisterD
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
	PXPublic PXActionResult PXGraphicSkyboxRegisterA
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
	PXPublic PXActionResult PXGraphicSkyboxUse(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXGraphicSkyboxRelease(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);

	//PXPublic PXSize PXGraphicModelListSize(const PXGraphicContext* const graphicContext);
	//PXPublic PXBool PXGraphicModelListGetFromIndex(const PXGraphicContext* const graphicContext, PXModel** pxModel, const PXSize index);

	PXPublic PXSize PXGraphicRenderableListSize(const PXGraphicContext* const graphicContext);
	PXPublic PXBool PXGraphicRenderableListGetFromIndex(const PXGraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index);

	//PXPublic PXActionResult PXGraphicModelCreate(PXGraphicContext* const graphicContext, PXModel** const pxModel);
	//PXPublic PXBool PXGraphicModelRegister(PXGraphicContext* const graphicContext, PXModel* const pxModel);
	PXPublic PXActionResult PXGraphicRenderableCreate(PXGraphicContext* const graphicContext, PXRenderable** const pxRenderable);
	PXPublic PXBool PXGraphicRenderableRegister(PXGraphicContext* const graphicContext, PXRenderable* const pxRenderable);


	PXPublic void PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);
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



	PXPublic void PXCameraConstruct(PXCamera* const camera);
	PXPublic void PXCameraDestruct(PXCamera* const camera);


	//-----------
	PXPublic float PXCameraAspectRatio(const PXCamera* const camera);
	PXPublic void PXCameraAspectRatioChange(PXCamera* const camera, const PXSize width, const PXSize height);

	PXPublic void PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective);
	PXPublic void PXCameraViewChangeToOrthographic(PXCamera* const camera, const PXSize width, const PXSize height, const float nearPlane, const float farPlane);
	PXPublic void PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane);
	//-----------

	//---<Transform>-----------------------------------------------------------
	PXPublic void PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F);
	PXPublic void PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z);

	PXPublic void PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F);
	PXPublic void PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z);

	PXPublic void PXCameraFollow(PXCamera* const camera, const float deltaTime);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic void PXCameraUpdate(PXCamera* const camera, const float deltaTime);
	//-------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif

#endif
#endif