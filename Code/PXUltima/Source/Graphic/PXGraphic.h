#ifndef PXGraphicINCLUDE
#define PXGraphicINCLUDE

#include <Media/PXType.h>
#include <Media/PXImage.h>
#include <Media/PXFont.h>
#include <Media/PXModel.h>
#include <Media/PXSound.h>
#include <Media/PXColor.h>
#include <OS/Graphic/OpenGL/OpenGL.h>
#include <Math/PXMatrix.h>
#include <Container/LinkedList/PXLinkedList.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Thread/PXLock.h>
#include <Media/PXColor.h>

#define PXShaderNotRegisterd (unsigned int)-1

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXGraphicRenderFilter_
	{
		PXGraphicRenderFilterInvalid,
		PXGraphicRenderFilterNoFilter, // No filter Option, use this for Pixelated Textures.
		PXGraphicRenderFilterBilinear, // Level I Filter
		PXGraphicRenderFilterTrilinear // Level II Filter
	}
	PXGraphicRenderFilter;

	typedef enum PXShaderType_
	{
		PXShaderTypeInvalid,
		PXShaderTypeVertex,     // .vert - a vertex shader
		PXShaderTypeFragment,   // .frag - a fragment shader
		PXShaderTypeTessellationControl,    // .tesc - a tessellation control shader
		PXShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
		PXShaderTypeGeometry,      // .geom - a geometry shader
		PXShaderTypeCompute,   // .comp - a compute shader
	}
	PXShaderType;

	typedef enum PXGraphicImageLayout_
	{
		PXGraphicImageLayoutInvalid,

		// Returns the value of the texture element that is nearest to the specified texture coordinates.
		PXGraphicImageLayoutNearest,

		// Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
		// These can include items wrapped or repeated from other parts of a texture, depending on the values of GL_TEXTURE_WRAP_Sand GL_TEXTURE_WRAP_T, and on the exact mapping.
		PXGraphicImageLayoutLinear,

		//Chooses the mipmap that most closely matches the size of the pixel being texturedand
		//uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates) to produce a texture value.
		PXGraphicImageLayoutMipMapNearestNearest,

		//Chooses the mipmap that most closely matches the size of the pixel being texturedand
		//uses the GL_LINEAR criterion(a weighted average of the four texture elements that are closest to the specified texture coordinates) to produce a texture value.
		PXGraphicImageLayoutMipMapLinearNearest,

		//Chooses the two mipmaps that most closely match the size of the pixel being texturedand
		//uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates)
		//to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
		PXGraphicImageLayoutMipMapNNearestLinear,

		//Chooses the two mipmaps that most closely match the size of the pixel being texturedand
		//uses the GL_LINEAR criterion(a weighted average of the texture elements that are closest to the specified texture coordinates)
		//to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
		PXGraphicImageLayoutMipMapLinearLinear
	}
	PXGraphicImageLayout;

	typedef enum PXGraphicImageWrap_
	{
		PXGraphicImageWrapInvalid,

		// Images will be used 'as is' and will not be streched whatsoever.
		PXGraphicImageWrapNoModification,

		// Strech
		PXGraphicImageWrapStrechEdges,

		PXGraphicImageWrapStrechEdgesAndMirror,

		// Tiles the image in a gridformat
		PXGraphicImageWrapRepeat,

		// Tiles the image in a gridformat but also flip them every time.
		PXGraphicImageWrapRepeatAndMirror,
	}
	PXGraphicImageWrap;

	typedef enum PXGraphicImageType_
	{
		PXGraphicImageTypeInvalid,

		PXGraphicImageTypeTexture2D,
		PXGraphicImageTypeTexture3D,

		PXGraphicImageTypeTextureCubeContainer,
		PXGraphicImageTypeTextureCubeRight,
		PXGraphicImageTypeTextureCubeLeft,
		PXGraphicImageTypeTextureCubeTop,
		PXGraphicImageTypeTextureCubeDown,
		PXGraphicImageTypeTextureCubeBack,
		PXGraphicImageTypeTextureCubeFront
	}
	PXGraphicImageType;

	typedef enum PXGraphicRenderMode_
	{
		PXGraphicRenderModeInvalid,
		PXGraphicRenderModePoint,
		PXGraphicRenderModeLine,
		PXGraphicRenderModeLineLoop,
		PXGraphicRenderModeLineStrip,
		PXGraphicRenderModeLineStripAdjacency,
		PXGraphicRenderModeLineAdjacency,
		PXGraphicRenderModeTriangle,
		PXGraphicRenderModeTriangleAdjacency,
		PXGraphicRenderModeTriangleFAN,
		PXGraphicRenderModeTriangleStrip,
		PXGraphicRenderModeTriangleStripAdjacency,
		PXGraphicRenderModeWireFrame,
		PXGraphicRenderModeSquare,
		PXGraphicRenderModePatches
	}
	PXGraphicRenderMode;

	typedef struct PXShader_
	{
		unsigned int ID;
		PXShaderType Type;
		PXSize ContentSize;
		char* Content;
	}
	PXShader;


	PXPublic void PXShaderConstruct(PXShader* const shader);

	PXPublic void PXShaderDataSet(PXShader* const shader, const PXShaderType type, const char* data, const PXSize size);


	typedef struct PXShaderProgram_
	{
		unsigned int ID;
	}
	PXShaderProgram;


	typedef struct PXTexture_
	{
		unsigned int ID;

		PXGraphicImageType Type;
		PXGraphicRenderFilter Filter;
		PXGraphicImageLayout LayoutNear;
		PXGraphicImageLayout LayoutFar;
		PXGraphicImageWrap WrapHeight;
		PXGraphicImageWrap WrapWidth;

		PXImage Image;
	}
	PXTexture;

	PXPublic void PXTextureConstruct(PXTexture* const texture);
	PXPublic void PXTextureDestruct(PXTexture* const texture);

	typedef struct PXTextureCube_
	{
		unsigned int ID;
		PXImage ImageList[6];
	}
	PXTextureCube;

	typedef struct CSprite_
	{
		unsigned int ID;
		PXTexture* Texture;
	}
	CSprite;



	typedef enum RefreshRateMode_
	{
		PXRefreshRateUnlimited,
		PXRefreshRateVSync,
		PXRefreshRateCustomSync
	}
	RefreshRateMode;

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

		PXUIElementTypeCustom
	}
	PXUIElementType;


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
		PXInt32U TextureID;
		PXInt32U ShaderID;
		//---------------------------------------

		//---<State-Info>------------------------
		PXBool IsEnabled;
		//PXBool Active;
		PXUIHoverState Hover;
		//---------------------------------------

		//---<Property>--------------------------
		PXInt16U ID;
		PXUIElementType Type;
		char Name[32];
		float X;
		float Y;
		float Width;
		float Height;
		//---------------------------------------

		//---<Events>----------------------------
		PXUIOnClick OnClickCallback;
		PXUIOnMouseEnter OnMouseEnterCallback;
		PXUIOnMouseLeave OnMouseLeaveCallback;
		//---------------------------------------
	}
	PXUIElement;

	typedef struct PXGraphicContext_
	{
		OpenGLContext OpenGLInstance;

		void* AttachedWindow;

		PXLock _resourceLock;

		PXSkyBox* _currentSkyBox;

		//---<Registered Objects>---
		PXInt32U UniqeIDGeneratorCounter;

		PXLinkedListFixed _renderList; // PXRenderable
	
		PXDictionary UIElementLookUp;
		PXDictionary TextureLookUp;
		PXDictionary ModelLookUp;
		PXDictionary FontLookUp;
		PXDictionary SoundLookup;
		PXDictionary ShaderProgramLookup;
		//--------------------------
	}
	PXGraphicContext;


	//-------------------------------------------------------------------------
	PXPublic void PXUIElementConstruct(PXUIElement* const pxUIElement);
	PXPublic void PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha);
	PXPublic void PXUIElementPositionSetXYWH(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height);

	PXPrivate PXInt32U PXGraphicGenerateUniqeID(PXGraphicContext* const graphicContext);

	PXPublic PXActionResult PXGraphicUIElementRegister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXGraphicUIElementUpdate(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	PXPublic PXActionResult PXGraphicUIElementUnregister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement);
	//-------------------------------------------------------------------------




	PXPublic void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);






	//---<OpenGL Translate>----------------
	PXPrivate OpenGLDataType PXGraphicDataTypeToOpenGL(const PXColorFormat imageDataFormat);
	PXPrivate OpenGLImageFormat PXGraphicImageFormatToOpenGL(const PXColorFormat imageDataFormat);
	PXPrivate OpenGLShaderType PXGraphicShaderFromOpenGL(const PXShaderType shaderType);
	PXPrivate OpenGLTextureType ImageTypeGraphicToOpenGL(const PXGraphicImageType graphicImageType);
	PXPublic OpenGLRenderMode PXGraphicRenderModeToOpenGL(const PXGraphicRenderMode graphicRenderMode);
	//-------------------------------------

	//-------------------------------------
	PXPublic void PXGraphicInstantiate(PXGraphicContext* const graphicContext);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------
	PXPublic PXActionResult PXGraphicShaderProgramCreate(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicShaderCompile(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicShaderUse(PXGraphicContext* const graphicContext, const unsigned int shaderID);

	PXPublic PXActionResult PXGraphicShaderProgramLoadGLSLA(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, const PXTextASCII vertexShaderFilePath, const PXTextASCII fragmentShaderFilePath);
	PXPublic PXActionResult PXGraphicShaderProgramLoadGLSLW(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, const PXTextUNICODE vertexShaderFilePath, const PXTextUNICODE fragmentShaderFilePath);
	PXPublic PXActionResult PXGraphicShaderProgramLoadGLSL(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, PXShader* const vertexShader, PXShader* const fragmentShader);

	PXPublic void PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);
	PXPublic unsigned int PXGraphicShaderVariableIDFetch(PXGraphicContext* const graphicContext, const unsigned int shaderID, const char* const name);
	PXPublic void PXGraphicShaderProgramUse(PXGraphicContext* const graphicContext, const unsigned int shaderID);
	//-------------------------------------------------------------------------

	//---<Rendering>-----------------------------------------------------------
	PXPublic PXBool PXGraphicImageBufferSwap(PXGraphicContext* const graphicContext);
	PXPublic PXActionResult PXGraphicRenderElement(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount);
	PXPublic PXActionResult PXGraphicRenderList(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount);
	//-------------------------------------------------------------------------


	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const image);

	PXPublic PXActionResult PXGraphicTextureLoadA(PXGraphicContext* const graphicContext, PXTexture* const texture, const PXTextASCII filePath);
	PXPublic PXActionResult PXGraphicTextureLoadW(PXGraphicContext* const graphicContext, PXTexture* const texture, const PXTextUNICODE filePath);
	PXPublic PXActionResult PXGraphicTextureRegister(PXGraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic PXActionResult PXGraphicTextureRelease(PXGraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic PXActionResult PXGraphicTextureUse(PXGraphicContext* const graphicContext, PXTexture* const texture);

	PXPublic PXActionResult PXGraphicTextureCubeRegister(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult PXGraphicTextureCubeRegisterUse(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult PXGraphicTextureCubeRelease(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube);
	//-------------------------------------------------------------------------



	//---<Font>----------------------------------------------------------------------
	PXPublic PXActionResult PXGraphicFontLoadA(PXGraphicContext* const graphicContext, PXFont* const pxFont, const PXTextASCII filePath);
	PXPublic PXActionResult PXGraphicFontRegister(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXGraphicFontRelease(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	PXPublic PXActionResult PXGraphicFontUse(PXGraphicContext* const graphicContext, PXFont* const pxFont);
	//-------------------------------------------------------------------------


	//---<Model>---------------------------------------------------------------
	PXPublic PXActionResult PXGraphicSkyboxRegister(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult PXGraphicSkyboxRegisterA
	(
		PXGraphicContext* const graphicContext,
		PXSkyBox* const skyBox,
		const char* shaderVertex,
		const char* shaderFragment,
		const char* textureRight,
		const char* textureLeft,
		const char* textureTop,
		const char* textureBottom,
		const char* textureBack,
		const char* textureFront
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


	PXPublic void PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderProgram);
	//PXPublic PXActionResult PXGraphicModelGenerate(PXGraphicContext* const graphicContext, PXRenderable** const renderable, const PXTextASCII filePath);
	PXPublic PXActionResult PXGraphicModelLoadA(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXTextASCII filePath);
	PXPublic PXActionResult PXGraphicModelRegisterFromModel(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model);
	PXPublic PXActionResult PXGraphicModelRegisterFromData(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);
	//-------------------------------------------------------------------------

	//PXPublic PXActionResult Load(PXModel& model, const wchar_t* filePath, const bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, PXModel* model, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(PXRenderable& renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);


	//PXPublic PXActionResult Load(Level& level, const wchar_t* filePath, const bool loadAsynchronously = true);
	// 	PXPublic PXActionResult Load(Collider* collider);

	//PXPublic PXActionResult Load(PXFont& font, const wchar_t* filePath, bool loadAsynchronously = true);
	//PXPublic PXActionResult Load(ShaderProgram& shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);

	//PXPublic PXActionResult Load(Image& image, const wchar_t* filePath, bool loadAsynchronously = true);

#ifdef __cplusplus
}
#endif

#endif
