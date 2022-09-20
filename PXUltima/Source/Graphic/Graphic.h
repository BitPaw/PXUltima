#ifndef GraphicINCLUDE
#define GraphicINCLUDE

#include <Format/Image.h>
#include <Format/Model.h>
#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum RenderFilter_
	{
		RenderFilterInvalid,		
		RenderFilterNoFilter, // No filter Option, use this for Pixelated Textures.
		RenderFilterBilinear, 		// Level I Filter
		RenderFilterTrilinear		// Level II Filter
	}
	ImageFilter;

	typedef enum ShaderType_
	{
		ShaderTypeUnkown,
		ShaderTypeVertex,     // .vert - a vertex shader
		ShaderTypeTessellationControl,    // .tesc - a tessellation control shader
		ShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
		ShaderTypeGeometry,      // .geom - a geometry shader
		ShaderTypeFragment,   // .frag - a fragment shader
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


	typedef struct CTexture_
	{
		unsigned int ID;		

		GraphicImageType Type;
		ImageFilter Filter;
		GraphicImageLayout LayoutNear;
		GraphicImageLayout LayoutFar;
		GraphicImageWrap WrapHeight;
		GraphicImageWrap WrapWidth;

		Image Image;
	}
	CTexture;

	typedef struct CTextureCube_
	{
		unsigned int ID;
		Image Image[6];
	}
	CTextureCube;

	typedef struct CSprite_
	{
		unsigned int ID;
		CTexture* Texture;
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


	// Shader

	CPublic ActionResult GraphicShaderProgramCreate();
	CPublic ActionResult GraphicShaderCompile();
	CPublic ActionResult GraphicShaderUse(const unsigned int shaderID);

	CPublic ActionResult GraphicShaderProgramCreateVF(ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath);

	CPublic void GraphicShaderUpdateMatrix4x4F(const unsigned int locationID, float* matrix4x4);
	CPublic unsigned int GraphicShaderVariableIDFetch(const unsigned int shaderID, const char* const name);
	CPublic void GraphicShaderProgramUse(const unsigned int shaderID);


	// Rendering
	CPublic ActionResult GraphicRenderElement(GraphicRenderMode renderMode, size_t start, size_t amount);
	CPublic ActionResult GraphicRenderList(GraphicRenderMode renderMode, size_t start, size_t amount);


	// Texture

	CPublic ActionResult GraphicTextureUse(CTexture* const texture);
	CPublic ActionResult GraphicTextureRegister(CTexture* const texture);
	CPublic ActionResult GraphicTextureRelease(CTexture* const texture);
	CPublic ActionResult GraphicTextureCubeRegister(CTextureCube* const textureCube);
	CPublic ActionResult GraphicTextureCubeRelease(CTextureCube* const textureCube);

	// Model
	CPublic ActionResult GraphicSkyboxRegister(CSkyBox* const skyBox);
	CPublic ActionResult GraphicSkyboxUse(CSkyBox* const skyBox);

	CPublic ActionResult GraphicModelRegisterFromModel(Renderable* const renderable, const Model* const model);
	CPublic ActionResult GraphicModelRegisterFromData(Renderable* const renderable, const float* vertexData, const size_t vertexDataSize, const unsigned int* indexList, const size_t indexListSize);


#ifdef __cplusplus
}
#endif

#endif