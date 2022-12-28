#ifndef GraphicINCLUDE
#define GraphicINCLUDE

#include <Format/Image.h>
#include <Format/Model.h>
#include <Format/Type.h>
#include <Graphic/OpenGL/OpenGL.h>
#include <Math/PXMatrix.h>
#include <Container/LinkedList/PXLinkedList.h>
#include <Async/PXLock.h>

#define PXShaderNotRegisterd (unsigned int)-1

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum GraphicRenderFilter_
	{
		GraphicRenderFilterInvalid,
		GraphicRenderFilterNoFilter, // No filter Option, use this for Pixelated Textures.
		GraphicRenderFilterBilinear, 		// Level I Filter
		GraphicRenderFilterTrilinear		// Level II Filter
	}
	GraphicRenderFilter;

	typedef enum ShaderType_
	{
		ShaderTypeInvalid,
		ShaderTypeVertex,     // .vert - a vertex shader
		ShaderTypeFragment,   // .frag - a fragment shader
		ShaderTypeTessellationControl,    // .tesc - a tessellation control shader
		ShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
		ShaderTypeGeometry,      // .geom - a geometry shader		
		ShaderTypeCompute,   // .comp - a compute shader
	}
	ShaderType;

	typedef enum GraphicImageLayout_
	{
		GraphicImageLayoutInvalid,

		// Returns the value of the texture element that is nearest to the specified texture coordinates.
		GraphicImageLayoutNearest,

		// Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
		// These can include items wrapped or repeated from other parts of a texture, depending on the values of GL_TEXTURE_WRAP_Sand GL_TEXTURE_WRAP_T, and on the exact mapping.
		GraphicImageLayoutLinear,

		//Chooses the mipmap that most closely matches the size of the pixel being texturedand
		//uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates) to produce a texture value.
		GraphicImageLayoutMipMapNearestNearest,

		//Chooses the mipmap that most closely matches the size of the pixel being texturedand
		//uses the GL_LINEAR criterion(a weighted average of the four texture elements that are closest to the specified texture coordinates) to produce a texture value.
		GraphicImageLayoutMipMapLinearNearest,

		//Chooses the two mipmaps that most closely match the size of the pixel being texturedand
		//uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates)
		//to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
		GraphicImageLayoutMipMapNNearestLinear,

		//Chooses the two mipmaps that most closely match the size of the pixel being texturedand
		//uses the GL_LINEAR criterion(a weighted average of the texture elements that are closest to the specified texture coordinates)
		//to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
		GraphicImageLayoutMipMapLinearLinear
	}
	GraphicImageLayout;

	typedef enum GraphicImageWrap_
	{
		GraphicImageWrapInvalid,

		// Images will be used 'as is' and will not be streched whatsoever.
		GraphicImageWrapNoModification,

		// Strech
		GraphicImageWrapStrechEdges,

		GraphicImageWrapStrechEdgesAndMirror,

		// Tiles the image in a gridformat
		GraphicImageWrapRepeat,

		// Tiles the image in a gridformat but also flip them every time.
		GraphicImageWrapRepeatAndMirror,
	}
	GraphicImageWrap;

	typedef enum GraphicImageType_
	{
		GraphicImageTypeInvalid,

		GraphicImageTypeTexture2D,
		GraphicImageTypeTexture3D,

		GraphicImageTypeTextureCubeContainer,
		GraphicImageTypeTextureCubeRight,
		GraphicImageTypeTextureCubeLeft,
		GraphicImageTypeTextureCubeTop,
		GraphicImageTypeTextureCubeDown,
		GraphicImageTypeTextureCubeBack,
		GraphicImageTypeTextureCubeFront
	}
	GraphicImageType;

	typedef enum GraphicRenderMode_
	{
		GraphicRenderModeInvalid,
		GraphicRenderModePoint,
		GraphicRenderModeLine,
		GraphicRenderModeLineLoop,
		GraphicRenderModeLineStrip,
		GraphicRenderModeLineStripAdjacency,
		GraphicRenderModeLineAdjacency,
		GraphicRenderModeTriangle,
		GraphicRenderModeTriangleAdjacency,
		GraphicRenderModeTriangleFAN,
		GraphicRenderModeTriangleStrip,
		GraphicRenderModeTriangleStripAdjacency,
		GraphicRenderModeWireFrame,
		GraphicRenderModeSquare,
		GraphicRenderModePatches
	}
	GraphicRenderMode;

	typedef enum CameraPerspective_
	{
		CameraPerspective2D, // Orthographic
		CameraPerspective3D // Perspective
	}
	CameraPerspective;

	typedef struct Shader_
	{
		unsigned int ID;
		ShaderType Type;
		PXSize ContentSize;
		char* Content;
	}
	Shader;


	PXPublic void PXShaderConstruct(Shader* const shader);

	PXPublic void ShaderDataSet(Shader* const shader, const ShaderType type, const char* data, const PXSize size);


	typedef struct ShaderProgram_
	{
		unsigned int ID;
	}
	ShaderProgram;


	typedef struct PXTexture_
	{
		unsigned int ID;

		GraphicImageType Type;
		GraphicRenderFilter Filter;
		GraphicImageLayout LayoutNear;
		GraphicImageLayout LayoutFar;
		GraphicImageWrap WrapHeight;
		GraphicImageWrap WrapWidth;

		Image Image;
	}
	PXTexture;

	PXPublic void PXTextureConstruct(PXTexture* const texture);
	PXPublic void PXTextureDestruct(PXTexture* const texture);

	typedef struct PXTextureCube_
	{
		unsigned int ID;
		Image ImageList[6];
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
		RefreshRateUnlimited,
		RefreshRateVSync,
		RefreshRateCustomSync
	}
	RefreshRateMode;

	typedef struct GraphicConfig_
	{
		RefreshRateMode WindowRefreshRateMode;
		unsigned int ScreenResolution[2];
		unsigned char FullScreen;
	}
	GraphicConfig;


	//typedef struct Renderable_ Renderable; ?

	typedef struct PXRenderableMeshSegment_
	{
		unsigned int NumberOfVertices;
		unsigned int TextureID;
		unsigned int ShaderID;

		GraphicRenderMode RenderMode;

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

	typedef struct PXUIPanel_
	{
		PXRenderable Renderable;
	}
	PXUIPanel;

	typedef struct PXUIButton_
	{
		PXRenderable Renderable;
	}
	PXUIButton;

	typedef struct PXUIImage_
	{
		PXRenderable Renderable;
	}
	PXUIImage;

	typedef struct PXUIText_
	{
		PXRenderable Renderable;
		PXFont* TextFont;
	}
	PXUIText;


	typedef struct GraphicContext_
	{
		OpenGLContext OpenGLInstance;

		void* AttachedWindow;

		PXLock _resourceLock;

		PXSkyBox* _currentSkyBox;

		//---<Registered Objects>---
		PXLinkedListFixed _renderList; // PXRenderable
		PXLinkedListFixed _textureList; // PXTexture
		//LinkedList<AudioClip*> _audioClipList;

		PXLinkedListFixed _pxModelList;

		//LinkedList<Sound*> _soundList;
		PXLinkedListFixed _fontList; // PXFont
		PXLinkedListFixed _shaderProgramList; // ShaderProgram;
		//LinkedList<Dialog*> _dialogList;
		//LinkedList<Level*> _levelList;
		//LinkedList<Collider*> _physicList;
		//--------------------------
	}
	GraphicContext;


	// Create
	PXPublic PXActionResult GraphicUIPanelRegister(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel);
	PXPublic PXActionResult GraphicUIPanelUpdate(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel);
	PXPublic PXActionResult GraphicUIPanelUnregister(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel);

	// 
	// Update
	// Destroy

	PXPublic PXActionResult GraphicUITextRegister(GraphicContext* const graphicContext, PXUIText* const pxUIText, const PXSize x, const PXSize y, const PXSize width, const PXSize height, const PXTextUTF8 text);



	 
	 
	 
	//-------------------------------------------------------------------------




	PXPublic void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);






	//---<OpenGL Translate>----------------
	PXPrivate OpenGLDataType GraphicDataTypeToOpenGL(const ImageDataFormat imageDataFormat);
	PXPrivate OpenGLImageFormat GraphicImageFormatToOpenGL(const ImageDataFormat imageDataFormat);
	PXPrivate OpenGLShaderType GraphicShaderFromOpenGL(const ShaderType shaderType);
	PXPrivate OpenGLTextureType ImageTypeGraphicToOpenGL(const GraphicImageType graphicImageType);
	PXPublic OpenGLRenderMode GraphicRenderModeToOpenGL(const GraphicRenderMode graphicRenderMode);
	//-------------------------------------

	//-------------------------------------
	PXPublic void GraphicInstantiate(GraphicContext* const graphicContext);
	//-------------------------------------

	//---<Shader>-----------------------------------------------------------------
	PXPublic PXActionResult GraphicShaderProgramCreate(GraphicContext* const graphicContext);
	PXPublic PXActionResult GraphicShaderCompile(GraphicContext* const graphicContext);
	PXPublic PXActionResult GraphicShaderUse(GraphicContext* const graphicContext, const unsigned int shaderID);

	PXPublic PXActionResult GraphicShaderProgramCreateVFPathA(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	PXPublic PXActionResult GraphicShaderProgramCreateVFPathW(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);
	PXPublic PXActionResult GraphicShaderProgramCreateVFData(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, Shader* vertexShader, Shader* fragmentShader);

	PXPublic void GraphicShaderUpdateMatrix4x4F(GraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);
	PXPublic unsigned int GraphicShaderVariableIDFetch(GraphicContext* const graphicContext, const unsigned int shaderID, const char* const name);
	PXPublic void GraphicShaderProgramUse(GraphicContext* const graphicContext, const unsigned int shaderID);
	//-------------------------------------------------------------------------

	//---<Rendering>-----------------------------------------------------------
	PXPublic unsigned char GraphicImageBufferSwap(GraphicContext* const graphicContext);
	PXPublic PXActionResult GraphicRenderElement(GraphicContext* const graphicContext, GraphicRenderMode renderMode, PXSize start, PXSize amount);
	PXPublic PXActionResult GraphicRenderList(GraphicContext* const graphicContext, GraphicRenderMode renderMode, PXSize start, PXSize amount);
	//-------------------------------------------------------------------------
	

	//---<Texture>----------------------------------------------------------------
	PXPublic PXActionResult GraphicTextureScreenShot(GraphicContext* const graphicContext, Image* const image);

	PXPublic PXActionResult GraphicTextureRegisterA(GraphicContext* const graphicContext, PXTexture* const texture, const char* const filePath);
	PXPublic PXActionResult GraphicTextureRegisterW(GraphicContext* const graphicContext, PXTexture* const texture, const wchar_t* const filePath);
	PXPublic PXActionResult GraphicTextureRegister(GraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic PXActionResult GraphicTextureRelease(GraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic PXActionResult GraphicTextureUse(GraphicContext* const graphicContext, PXTexture* const texture);

	PXPublic PXActionResult GraphicTextureCubeRegister(GraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult GraphicTextureCubeRegisterUse(GraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic PXActionResult GraphicTextureCubeRelease(GraphicContext* const graphicContext, PXTextureCube* const textureCube);
	//-------------------------------------------------------------------------

	//---<Model>---------------------------------------------------------------
	PXPublic PXActionResult GraphicSkyboxRegister(GraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult GraphicSkyboxRegisterA
	(
		GraphicContext* const graphicContext,
		PXSkyBox** skyBox,
		const char* shaderVertex,
		const char* shaderFragment,
		const char* textureRight,
		const char* textureLeft,
		const char* textureTop,
		const char* textureBottom,
		const char* textureBack,
		const char* textureFront
	);
	PXPublic PXActionResult GraphicSkyboxUse(GraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic PXActionResult GraphicSkyboxRelease(GraphicContext* const graphicContext, PXSkyBox* const skyBox);

	PXPublic PXSize GraphicModelListSize(const GraphicContext* const graphicContext);
	PXPublic PXBool GraphicModelListGetFromIndex(const GraphicContext* const graphicContext, PXModel** pxModel, const PXSize index);

	PXPublic PXSize GraphicRenderableListSize(const GraphicContext* const graphicContext);
	PXPublic PXBool GraphicRenderableListGetFromIndex(const GraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index);

	PXPublic PXActionResult GraphicModelCreate(GraphicContext* const graphicContext, PXModel** const pxModel);
	PXPublic PXBool GraphicModelRegister(GraphicContext* const graphicContext, PXModel* const pxModel);
	PXPublic PXActionResult GraphicRenderableCreate(GraphicContext* const graphicContext, PXRenderable** const pxRenderable);
	PXPublic PXBool GraphicRenderableRegister(GraphicContext* const graphicContext, PXRenderable* const pxRenderable);


	PXPublic void GraphicModelShaderSet(GraphicContext* const graphicContext, PXRenderable* const renderable, const ShaderProgram* const shaderProgram);
	PXPublic PXActionResult GraphicModelRegisterA(GraphicContext* const graphicContext, PXRenderable** const renderable, const char* const filePath);
	PXPublic PXActionResult GraphicModelRegisterFromModel(GraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model);
	PXPublic PXActionResult GraphicModelRegisterFromData(GraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize);
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