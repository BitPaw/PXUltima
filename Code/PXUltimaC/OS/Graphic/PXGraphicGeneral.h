#ifndef PXGraphicGeneralINCLUDE
#define PXGraphicGeneralINCLUDE

#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXGraphicSystem_
	{
		PXGraphicSystemInvalid,
		PXGraphicSystemPXOpenGL,
		PXGraphicSystemDirectX,
		PXGraphicSystemVulcan
	}
	PXGraphicSystem;

	typedef enum RefreshRateMode_
	{
		PXRefreshRateUnlimited,
		PXRefreshRateVSync,
		PXRefreshRateCustomSync
	}
	RefreshRateMode;

	typedef enum PXGraphicResourceLocation_
	{
		PXGraphicResourceLocationInvalid,
		PXGraphicResourceLocationFullyUnloaded, // Resource is known but not loaded
		PXGraphicResourceLocationLoadedNotRegisterd, // Rsource is loaded but not known by external rendering systems
		PXGraphicResourceLocationLoadAndRegistered, // Resource si loaded and registerd
		PXGraphicResourceLocationRegisteredOnly, // Resouce is uploaded and not loaded anymore.
	}
	PXGraphicResourceLocation;

	typedef enum PXGraphicResourceType_
	{
		PXGraphicResourceTypeInvalid,
		PXGraphicResourceTypeModel,
		PXGraphicResourceTypeTexure,
		PXGraphicResourceTypeSkyBox,
		PXGraphicResourceTypeFont
	}
	PXGraphicResourceType;


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

	typedef enum PXVertexBufferFormat_
	{
		PXVertexBufferFormatInvalid,



		// OpenGL

		PXVertexBufferFormatXY, // GL_V2F
		PXVertexBufferFormatC4UB_XY,
		PXVertexBufferFormatC4UB_XYZ,
		PXVertexBufferFormatRGBXYZ,
		PXVertexBufferFormatN3F_XYZ,
		PXVertexBufferFormatC4F_N3F_XYZ,
		PXVertexBufferFormatT2F_XYZ,
		PXVertexBufferFormatT4F_XYZW,
		PXVertexBufferFormatT2F_C4UB_XYZ,
		PXVertexBufferFormatT2F_C3F_XYZ,
		PXVertexBufferFormatT2F_N3F_XYZ,
		PXVertexBufferFormatT2F_C4F_N3F_XYZ,
		PXVertexBufferFormatT4F_C4F_N3F_XYZW,

		// Direct X

		PXVertexBufferFormatXYZ, // Normal spcace (x, y, z)
		PXVertexBufferFormatXYZC, // Normal spcace (x, y, z, color-RGB as 32-int)

		PXVertexBufferFormatXYZRHW, // DirectX only, use for pixel size instead of normal space. 

		PXVertexBufferFormatXYZHWC, // X, Y, Z, Tx, Ty, color-RGB as 32-int

		PXVertexBufferFormatXYZB1,
		PXVertexBufferFormatXYZB2,
		PXVertexBufferFormatXYZB3,
		PXVertexBufferFormatXYZB4,
		PXVertexBufferFormatXYZB5,
		PXVertexBufferFormatXYZW,

	}
	PXVertexBufferFormat;



	typedef struct PXResourceID_
	{
		union
		{
			void* DirectXInterface;
			PXInt32U OpenGLID;
		};

		PXInt32U PXID;
	}
	PXResourceID;



	typedef struct PXTexture2D_
	{
		PXResourceID ResourceID; // IDirect3DTexture9

		PXGraphicImageType Type;
		PXGraphicRenderFilter Filter;
		PXGraphicImageLayout LayoutNear;
		PXGraphicImageLayout LayoutFar;
		PXGraphicImageWrap WrapHeight;
		PXGraphicImageWrap WrapWidth;

		PXImage Image;
	}
	PXTexture2D;


	typedef struct PXTexture3D_
	{
		PXResourceID ResourceID; // IDirect3DVolumeTexture9

		PXInt32U Width;
		PXInt32U Height;
		PXInt32U Depth;

		PXColorFormat Format;
	}
	PXTexture3D;

	// A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
	typedef struct PXTextureCube_
	{
		PXResourceID ResourceID; // IDirect3DCubeTexture9

		PXColorFormat Format;

		PXImage ImageList[6];
	}
	PXTextureCube;



	typedef enum PXDrawScriptType_
	{
		PXDrawScriptTypeInvalid,
		PXDrawScriptTypeAll,
		PXDrawScriptTypePixelState,
		PXDrawScriptTypeVertexState
	}
	PXDrawScriptType;

	// A sequence of actions that tells the fixed pipline how to act.
	// This is a precursor of a shader, so it's use is discurraged for
	// modern applications and hardware that support shaders, not only
	// for performance reasons but for versitility too.
	typedef struct PXDrawScript_
	{
		PXResourceID ResourceID;

		PXDrawScriptType Type;
	}
	PXDrawScript;


	typedef struct PXVertexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DVertexBuffer9, ID3D11Buffer

		void* VertexData;
		PXSize VertexDataSize;

		PXSize VertexDataRowSize;

		PXVertexBufferFormat Format;
	}
	PXVertexBuffer;


	typedef struct PXIndexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DIndexBuffer9

		void* IndexData;
		PXInt32U IndexDataSize;

		PXDataType DataType;
	}
	PXIndexBuffer;




	typedef struct PXVertexStructure_
	{
		PXVertexBuffer VertexBuffer;
		PXIndexBuffer IndexBuffer;
	}
	PXVertexStructure;





	typedef struct PXDepthStencilSurface_
	{
		int x;
	}
	PXDepthStencilSurface;


	typedef struct PXMaterial_
	{
		float Diffuse[4];
		float Ambient[4];
		float Specular[4];	// shininess
		float Emissive[4];
		float Power;        // Sharpness if specular highlight
	}
	PXMaterial;


	typedef struct PXViewPort_
	{
		PXInt32U X;
		PXInt32U Y;
		PXInt32U Width;
		PXInt32U Height;
		float ClippingMinimum;
		float ClippingMaximum;
	}
	PXViewPort;


	typedef struct PXRenderTarget_
	{
		PXResourceID ResourceID; // IDirect3DVertexBuffer9
	}
	PXRenderTarget;


	// Offset the whole child-Container
	// (Make empty space)
	typedef struct PXMargin_
	{
		float Left;
		float Bottom;
		float Right;
		float Top;
	}
	PXMargin;


	// Offset the space as the child container 
	// (Make object take more space)
	typedef struct PXPadding_
	{
		float LeftTop;
		float Top;
		float Right;
		float Bottom;
	}
	PXPadding;



	typedef struct PXShader_
	{
		PXResourceID ResourceID; // IDirect3DVertexShader9, IDirect3DPixelShader9
	
		PXShaderType Type;

		PXSize ContentSize;
		const char* Content;
	}
	PXShader;


	typedef struct PXShaderProgram_
	{
		PXResourceID ResourceID;

		PXShader VertexShader;
		PXShader PixelShader;
	}
	PXShaderProgram;






#ifdef __cplusplus
}
#endif


#endif