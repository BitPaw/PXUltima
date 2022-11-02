#ifndef GraphicINCLUDE
#define GraphicINCLUDE

#include <Format/Image.h>
#include <Format/Model.h>
#include <Format/Type.h>
#include <Graphic/OpenGL/OpenGL.h>
#include <Math/PXMatrix.h>

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

	PXPublic void ShaderDataSet(Shader* const shader, const ShaderType type, const size_t size, const char* data);


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

	typedef	struct PXSkyBox_
	{
		PXTextureCube TextureCube;
	}
	PXSkyBox;

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

	typedef struct PXRenderable_
	{
		unsigned char DoRendering;

		unsigned int ID; // VAO

		unsigned int VBO;

		PXMatrix4x4F MatrixModel;

		GraphicRenderMode RenderMode;

		unsigned int RenderSize;
	}
	PXRenderable;

	typedef struct GraphicContext_
	{
		OpenGLContext OpenGLInstance;

		void* AttachedWindow;
	}
	GraphicContext;




	//---<OpenGL Translate>----------------
	PXPrivate OpenGLShaderType GraphicShaderFromOpenGL(const ShaderType shaderType);
	PXPrivate OpenGLTextureType ImageTypeGraphicToOpenGL(const GraphicImageType graphicImageType);
	//-------------------------------------


	PXPublic void GraphicInstantiate(GraphicContext* const graphicContext);

	PXPublic unsigned char GraphicImageBufferSwap(GraphicContext* const graphicContext);



	// Shader

	PXPublic ActionResult GraphicShaderProgramCreate(GraphicContext* const graphicContext);
	PXPublic ActionResult GraphicShaderCompile(GraphicContext* const graphicContext);
	PXPublic ActionResult GraphicShaderUse(GraphicContext* const graphicContext, const unsigned int shaderID);

	PXPublic ActionResult GraphicShaderProgramCreateVFPath(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);
	PXPublic ActionResult GraphicShaderProgramCreateVFData(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, Shader* vertexShader, Shader* fragmentShader);

	PXPublic void GraphicShaderUpdateMatrix4x4F(GraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4);
	PXPublic unsigned int GraphicShaderVariableIDFetch(GraphicContext* const graphicContext, const unsigned int shaderID, const char* const name);
	PXPublic void GraphicShaderProgramUse(GraphicContext* const graphicContext, const unsigned int shaderID);


	// Rendering
	PXPublic ActionResult GraphicRenderElement(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount);
	PXPublic ActionResult GraphicRenderList(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount);


	// Texture
	PXPublic ActionResult GraphicTextureScreenShot(GraphicContext* const graphicContext, Image* const image);

	PXPublic ActionResult GraphicTextureUse(GraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic ActionResult GraphicTextureRegister(GraphicContext* const graphicContext, PXTexture* const texture);
	PXPublic ActionResult GraphicTextureRelease(GraphicContext* const graphicContext, PXTexture* const texture);

	PXPublic ActionResult GraphicTextureCubeRegister(GraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic ActionResult GraphicTextureCubeRegisterUse(GraphicContext* const graphicContext, PXTextureCube* const textureCube);
	PXPublic ActionResult GraphicTextureCubeRelease(GraphicContext* const graphicContext, PXTextureCube* const textureCube);

	// Model
	PXPublic ActionResult GraphicSkyboxRegister(GraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic ActionResult GraphicSkyboxUse(GraphicContext* const graphicContext, PXSkyBox* const skyBox);
	PXPublic ActionResult GraphicSkyboxRelease(GraphicContext* const graphicContext, PXSkyBox* const skyBox);

	PXPublic ActionResult GraphicModelRegisterFromModel(GraphicContext* const graphicContext, PXRenderable* const renderable, const Model* const model);
	PXPublic ActionResult GraphicModelRegisterFromData(GraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const size_t vertexDataSize, const unsigned int* indexList, const size_t indexListSize);



#ifdef __cplusplus
}
#endif

#endif