#ifndef PXGraphicINCLUDE
#define PXGraphicINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXGraphicUSE OSDeviceToUse == OSDeviceDestop
#if PXGraphicUSE

#include <Media/PXType.h>
#include <Media/PXImage.h>
#include <Media/PXFont.h>
#include <Media/PXModel.h>
#include <Media/PXSound.h>
#include <Media/PXColor.h>
#include <Math/PXMatrix.h>
#include <Container/LinkedList/PXLinkedList.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Async/PXLock.h>
#include <Media/PXColor.h>

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










	typedef struct PXShader_
	{
		unsigned int ID;
		PXShaderType Type;
		PXSize ContentSize;
		const char* Content;
	}
	PXShader;


	PXPublic void PXShaderConstruct(PXShader* const shader);

	PXPublic void PXShaderDataSet(PXShader* const shader, const PXShaderType type, const char* data, const PXSize size);


	typedef struct PXShaderProgram_
	{
		unsigned int ID;
	}
	PXShaderProgram;


	

	PXPublic void PXTextureConstruct(PXTexture2D* const texture);
	PXPublic void PXTextureDestruct(PXTexture2D* const texture);


	


	typedef struct PXSprite
	{
		PXGraphicResourceID ID;
		PXTexture2D Texture;
		PXMatrix4x4F Position;

		PXVector2F TextureScaleOffset;

		PXMargin Margin;
	}
	PXSprite;

	 




	typedef struct PXGraphicConfig_
	{
		RefreshRateMode WindowRefreshRateMode;
		unsigned int ScreenResolution[2];
		unsigned char FullScreen;
	}
	PXGraphicConfig;


	//typedef struct Renderable_ Renderable; ?

	typedef struct PXRenderableMeshSegment_
	{
		unsigned int NumberOfVertices;
		unsigned int TextureID;
		unsigned int ShaderID;

		PXGraphicRenderMode RenderMode;

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


	typedef	struct PXSkyBox_
	{
		PXRenderable Renderable;
		PXTextureCube TextureCube;
	}
	PXSkyBox;

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
		PXInt32U TextureID;
		PXInt32U ShaderID;
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
		PXInt32U FrameRenderTextureID;


		//---<Events>----------------------------
		PXUIOnClick OnClickCallback;
		PXUIOnMouseEnter OnMouseEnterCallback;
		PXUIOnMouseLeave OnMouseLeaveCallback;
		//---------------------------------------
	}
	PXUIElement;

	typedef struct PXGraphicContext_
	{
		PXDirectX DirectXInstance;
		PXOpenGL OpenGLInstance;

#if PXOpenGLUSE

	
	
#endif

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




	PXPublic void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);



	PXPublic void PXMarginSet(PXMargin* const pxMargin, const float left, const float bottom, const float right, const float top);


	//---<PXOpenGL Translate>----------------
#if PXOpenGLUSE || 1
	PXPrivate PXOpenGLDataType PXGraphicDataTypeToPXOpenGL(const PXColorFormat imageDataFormat);
	PXPrivate PXOpenGLImageFormat PXGraphicImageFormatToPXOpenGL(const PXColorFormat imageDataFormat);
	PXPrivate PXOpenGLShaderType PXGraphicShaderFromPXOpenGL(const PXShaderType shaderType);
	PXPrivate PXOpenGLTextureType ImageTypeGraphicToPXOpenGL(const PXGraphicImageType graphicImageType);
	PXPublic PXOpenGLRenderMode PXGraphicRenderModeToPXOpenGL(const PXGraphicRenderMode graphicRenderMode);
#endif
	//-------------------------------------

	//-------------------------------------
	PXPublic void PXGraphicInstantiate(PXGraphicContext* const graphicContext);

	PXPublic void PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------
	PXPublic PXActionResult PXGraphicShaderProgramCreate(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicShaderCompile(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicShaderUse(PXGraphicContext* const graphicContext, const unsigned int shaderID);

	PXPublic PXActionResult PXGraphicShaderProgramLoadGLSL(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderPXProgram, const PXText* const vertexShaderFilePath, const PXText* const fragmentShaderFilePath);
	PXPublic PXActionResult PXGraphicShaderProgramLoadGLSLA(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderPXProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);

	PXPublic void PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);
	PXPublic unsigned int PXGraphicShaderVariableIDFetch(PXGraphicContext* const graphicContext, const unsigned int shaderID, const char* const name);
	PXPublic void PXGraphicShaderPXProgramUse(PXGraphicContext* const graphicContext, const unsigned int shaderID);
	//-------------------------------------------------------------------------

	//---<Rendering>-----------------------------------------------------------
	PXPublic PXBool PXGraphicFrameBufferSwap(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicRenderElement(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount);
	PXPublic PXActionResult PXGraphicRenderList(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount);
	//-------------------------------------------------------------------------

	// Load image resource and register it to prevent multible loads of the same file
	PXPublic PXActionResult PXGraphicLoadImage(PXGraphicContext* const graphicContext, PXImage* const pxImage, const PXText* const pxImageFilePath);


	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const pxImage);

	PXPublic PXActionResult PXGraphicTextureLoad(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const PXText* const filePath);
	PXPublic PXActionResult PXGraphicTextureLoadA(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const char* const filePath);
	PXPublic PXActionResult PXGraphicTextureRegister(PXGraphicContext* const graphicContext, PXTexture2D* const texture);
	PXPublic PXActionResult PXGraphicTextureRelease(PXGraphicContext* const graphicContext, PXTexture2D* const texture);
	PXPublic PXActionResult PXGraphicTextureUse(PXGraphicContext* const graphicContext, PXTexture2D* const texture);

	PXPublic PXActionResult PXGraphicTextureCubeRegister(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult PXGraphicTextureCubeRegisterUse(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult PXGraphicTextureCubeRelease(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	//-------------------------------------------------------------------------

	//---<Sprte>---------------------------------------------------------------
	PXPublic PXActionResult PXSpriteConstruct(PXSprite* const pxSprite);

	PXPublic PXActionResult PXSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y);

	PXPublic PXActionResult PXGraphicSpriteRegister(PXGraphicContext* const graphicContext, PXSprite* const pxSprite);
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

	PXPublic PXSize PXGraphicModelListSize(const PXGraphicContext* const graphicContext);
	PXPublic PXBool PXGraphicModelListGetFromIndex(const PXGraphicContext* const graphicContext, PXModel** pxModel, const PXSize index);

	PXPublic PXSize PXGraphicRenderableListSize(const PXGraphicContext* const graphicContext);
	PXPublic PXBool PXGraphicRenderableListGetFromIndex(const PXGraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index);

	PXPublic PXActionResult PXGraphicModelCreate(PXGraphicContext* const graphicContext, PXModel** const pxModel);
	PXPublic PXBool PXGraphicModelRegister(PXGraphicContext* const graphicContext, PXModel* const pxModel);
	PXPublic PXActionResult PXGraphicRenderableCreate(PXGraphicContext* const graphicContext, PXRenderable** const pxRenderable);
	PXPublic PXBool PXGraphicRenderableRegister(PXGraphicContext* const graphicContext, PXRenderable* const pxRenderable);


	PXPublic void PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);
	//PXPublic PXActionResult PXGraphicModelGenerate(PXGraphicContext* const graphicContext, PXRenderable** const renderable, const PXTextASCII filePath);
	PXPublic PXActionResult PXGraphicModelLoad(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXText* const filePath);
	PXPublic PXActionResult PXGraphicModelRegisterFromModel(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model);
	PXPublic PXActionResult PXGraphicModelRegisterFromData(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);
	//-------------------------------------------------------------------------

	//PXPublic PXActionResult Load(PXModel& model, const wchar_t* filePath, const bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, PXModel* model, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);


	//PXPublic PXActionResult Load(Level& level, const wchar_t* filePath, const bool loadAsynchronously = true);
	// 	PXPublic PXActionResult Load(Collider* collider);

	//PXPublic PXActionResult Load(PXFont& font, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(ShaderPXProgram& shaderPXProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);

	//PXPublic PXActionResult Load(Image& image, const wchar_t* filePath, bool loadAsynchronously = true);

#ifdef __cplusplus
}
#endif

#endif
#endif