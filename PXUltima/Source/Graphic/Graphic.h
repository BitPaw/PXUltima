#ifndef GraphicINCLUDE
#define GraphicINCLUDE

#include <Format/Image.h>
#include <Format/Model.h>
#include <Format/Type.h>
#include <Graphic/OpenGL/OpenGL.h>

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
		size_t ContentSize;
		char* Content;
	}
	Shader;

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

	CPublic void PXTextureConstruct(PXTexture* const texture);
	CPublic void PXTextureDestruct(PXTexture* const texture);

	typedef struct CTextureCube_
	{
		unsigned int ID;
		Image ImageList[6];
	}
	CTextureCube;

	typedef struct CSprite_
	{
		unsigned int ID;
		PXTexture* Texture;
	}
	CSprite;

	typedef	struct CSkyBox_
	{
		CTextureCube TextureCube;
	}
	CSkyBox;

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

	typedef struct Renderable_
	{
		unsigned int ID; // VAO

		GraphicRenderMode RenderMode;
	}
	Renderable;

	typedef struct GraphicContext_
	{
		OpenGLContext OpenGLInstance;

		void* AttachedWindow;
	}
	GraphicContext;




	//---<OpenGL Translate>----------------
	CPrivate OpenGLShaderType GraphicShaderFromOpenGL(const ShaderType shaderType);
	CPrivate OpenGLTextureType ImageTypeGraphicToOpenGL(const GraphicImageType graphicImageType);
	//-------------------------------------


	CPublic void GraphicInstantiate(GraphicContext* const graphicContext);

	CPublic unsigned char GraphicImageBufferSwap(GraphicContext* const graphicContext);



	// Shader

	CPublic ActionResult GraphicShaderProgramCreate(GraphicContext* const graphicContext);
	CPublic ActionResult GraphicShaderCompile(GraphicContext* const graphicContext);
	CPublic ActionResult GraphicShaderUse(GraphicContext* const graphicContext, const unsigned int shaderID);

	CPublic ActionResult GraphicShaderProgramCreateVF(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);

	CPublic void GraphicShaderUpdateMatrix4x4F(GraphicContext* const graphicContext, const unsigned int locationID, float* matrix4x4);
	CPublic unsigned int GraphicShaderVariableIDFetch(GraphicContext* const graphicContext, const unsigned int shaderID, const char* const name);
	CPublic void GraphicShaderProgramUse(GraphicContext* const graphicContext, const unsigned int shaderID);


	// Rendering
	CPublic ActionResult GraphicRenderElement(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount);
	CPublic ActionResult GraphicRenderList(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount);


	// Texture

	CPublic ActionResult GraphicTextureUse(GraphicContext* const graphicContext, PXTexture* const texture);
	CPublic ActionResult GraphicTextureRegister(GraphicContext* const graphicContext, PXTexture* const texture);
	CPublic ActionResult GraphicTextureRelease(GraphicContext* const graphicContext, PXTexture* const texture);

	CPublic ActionResult GraphicTextureCubeRegister(GraphicContext* const graphicContext, CTextureCube* const textureCube);
	CPublic ActionResult GraphicTextureCubeRegisterUse(GraphicContext* const graphicContext, CTextureCube* const textureCube);
	CPublic ActionResult GraphicTextureCubeRelease(GraphicContext* const graphicContext, CTextureCube* const textureCube);

	// Model
	CPublic ActionResult GraphicSkyboxRegister(GraphicContext* const graphicContext, CSkyBox* const skyBox);
	CPublic ActionResult GraphicSkyboxUse(GraphicContext* const graphicContext, CSkyBox* const skyBox);
	CPublic ActionResult GraphicSkyboxRelease(GraphicContext* const graphicContext, CSkyBox* const skyBox);

	CPublic ActionResult GraphicModelRegisterFromModel(GraphicContext* const graphicContext, Renderable* const renderable, const Model* const model);
	CPublic ActionResult GraphicModelRegisterFromData(GraphicContext* const graphicContext, Renderable* const renderable, const float* vertexData, const size_t vertexDataSize, const unsigned int* indexList, const size_t indexListSize);



#ifdef __cplusplus
}
#endif

#endif