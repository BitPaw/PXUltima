#ifndef PXGraphicGeneralINCLUDE
#define PXGraphicGeneralINCLUDE

#include <Math/PXVector.h>
#include <Math/PXMatrix.h>
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

	typedef enum PXGraphicShaderType_
	{
		PXShaderTypeInvalid,
		PXShaderTypeVertex,     // .vert - a vertex shader
		PXShaderTypeFragment,   // .frag - a fragment shader
		PXShaderTypeTessellationControl,    // .tesc - a tessellation control shader
		PXShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
		PXShaderTypeGeometry,      // .geom - a geometry shader
		PXShaderTypeCompute,   // .comp - a compute shader
	}
	PXGraphicShaderType;

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

	typedef enum PXGraphicTextureType_
	{
		PXGraphicTextureTypeInvalid,

		PXGraphicTextureType1D,
		PXGraphicTextureType1DArray,
		PXGraphicTextureType1DBuffer,
		PXGraphicTextureType2D,
		PXGraphicTextureType2DArray,
		PXGraphicTextureType2DProxy,
		PXGraphicTextureType3D,

		PXGraphicTextureTypeCubeContainer,
		PXGraphicTextureTypeCubeProxy,
		PXGraphicTextureTypeCubeArray,
		PXGraphicTextureTypeCubeRight,
		PXGraphicTextureTypeCubeLeft,
		PXGraphicTextureTypeCubeTop,
		PXGraphicTextureTypeCubeDown,
		PXGraphicTextureTypeCubeBack,
		PXGraphicTextureTypeCubeFront,

		PXGraphicTextureTypeBuffer,
		PXGraphicTextureTypeRectangle,
		PXGraphicTextureTypeRectangleProxy,

		PXGraphicTextureType2DMultiSample,
		PXGraphicTextureType2DMultiSampleArray
	}
	PXGraphicTextureType;

	typedef enum PXGraphicDrawMode_
	{
		PXGraphicDrawModeInvalid,
		PXGraphicDrawModePoint,
		PXGraphicDrawModeLine,
		PXGraphicDrawModeLineLoop,
		PXGraphicDrawModeLineStrip,
		PXGraphicDrawModeLineStripAdjacency,
		PXGraphicDrawModeLineAdjacency,
		PXGraphicDrawModeTriangle,
		PXGraphicDrawModeTriangleAdjacency,
		PXGraphicDrawModeTriangleFAN,
		PXGraphicDrawModeTriangleStrip,
		PXGraphicDrawModeTriangleStripAdjacency,
		PXGraphicDrawModeWireFrame,
		PXGraphicDrawModeSquare,
		PXGraphicDrawModeSquareStrip,
		PXGraphicDrawModePatches
	}
	PXGraphicDrawMode;

	typedef enum PXGraphicDrawFillMode_
	{
		PXGraphicDrawFillModeInvalid,
		PXGraphicDrawFillModePoints,
		PXGraphicDrawFillModeLines,
		PXGraphicDrawFillModeFill
	}
	PXGraphicDrawFillMode;


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

	PXPublic PXInt8U PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat);



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

#define PXResourceIDIsUnused(pxPesourceID) (pxPesourceID)->DirectXInterface == (void*)-1
#define PXResourceIDMarkAsUnused(pxPesourceID) (pxPesourceID)->DirectXInterface = (void*)-1



	typedef struct PXTexture2D_
	{
		PXResourceID ResourceID; // IDirect3DTexture9

		PXGraphicRenderFilter Filter;
		PXGraphicImageLayout LayoutNear;
		PXGraphicImageLayout LayoutFar;
		PXGraphicImageWrap WrapHeight;
		PXGraphicImageWrap WrapWidth;

		PXImage Image;
	}
	PXTexture2D;

#define PXTexture2DSet(texture, format, width, height) \
	(texture)->ResourceID.DirectXInterface = (void*)-1; \
	(texture)->Filter = PXGraphicRenderFilterNoFilter; \
	(texture)->LayoutNear = PXGraphicImageLayoutNearest; \
	(texture)->LayoutFar = PXGraphicImageLayoutNearest; \
	(texture)->WrapHeight= PXGraphicImageWrapStrechEdges; \
	(texture)->WrapWidth = PXGraphicImageWrapStrechEdges; \
	(texture)->Image.Width = width; \
	(texture)->Image.Height = height;  \
	(texture)->Image.PixelDataSize = 0;  \
	(texture)->Image.PixelData = 0; \
	(texture)->Image.Format = format; \


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






	typedef struct PXShader_
	{
		PXResourceID ResourceID; // IDirect3DVertexShader9, IDirect3DPixelShader9

		PXGraphicShaderType Type;

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


#define PXDrawModeIDQuad					1 << 0
#define PXDrawModeIDInvalid					1 << 1
#define PXDrawModeIDPoint					1 << 2
#define PXDrawModeIDLine					1 << 3
#define PXDrawModeIDLineLoop				1 << 4
#define PXDrawModeIDLineStrip				1 << 5
#define PXDrawModeIDLineStripAdjacency		1 << 6
#define PXDrawModeIDLineAdjacency			1 << 7
#define PXDrawModeIDTriangle				1 << 8
#define PXDrawModeIDTriangleAdjacency		1 << 9
#define PXDrawModeIDTriangleFAN				1 << 10
#define PXDrawModeIDTriangleStrip			1 << 11
#define PXDrawModeIDTriangleStripAdjacency	1 << 12
#define PXDrawModeIDWireFrame				1 << 13
#define PXDrawModeIDSquare					1 << 14
#define PXDrawModeIDPatches					1 << 15



	typedef struct PXIndexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DIndexBuffer9

		PXInt8U IndexTypeSize;

		void* IndexData;
		PXInt32U IndexDataSize;
		PXInt32U IndexDataAmount;

		PXDataType DataType;
		PXInt32U DrawModeID;
	}
	PXIndexBuffer;




	typedef struct PXVertexStructure_
	{
		PXResourceID ResourceID;

		PXMatrix4x4F ModelMatrix;

		PXShaderProgram* ShaderProgramReference;

		PXVertexBuffer VertexBuffer;
		PXIndexBuffer IndexBuffer;
	}
	PXVertexStructure;

	typedef struct PXVertexElement_
	{
		PXDataType Type;
		PXSize Length;
		PXSize Stride;
		void* StartAdress;
	}
	PXVertexElement;



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


#define PXViewPortSetXYWH(adress, x, y, width, height) \
	(adress)->X = x; \
	(adress)->Y = y; \
	(adress)->Width = width; \
	(adress)->Height = height; \
	(adress)->ClippingMinimum = 0; \
	(adress)->ClippingMaximum = 1;

#define PXViewPortSetWH(adress, width, height) PXViewPortSetXYWH(adress, 0, 0, width, height)

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








	typedef enum PXLightType_
	{
		PXLightTypeInvalid,
		PXLightTypePoint,
		PXLightTypeSpot,
		PXLightTypeDirectional
	} 
	PXLightType;

	typedef struct PXLight_
	{
		PXResourceID ResourceID; // D3DLIGHT9

		PXBool Enabled;

		PXLightType    Type;            /* Type of light source */
		float   Diffuse[4];         /* Diffuse color of light */
		float   Specular[4];        /* Specular color of light */
		float   Ambient[4];         /* Ambient color of light */
		float       Position[3];         /* Position in world space */
		float       Direction[3];        /* Direction in world space */
		float           CutoffRange;            /* Cutoff range */
		float           Falloff;          /* Falloff */
		float           AttenuationConstant;     /* Constant attenuation */
		float           AttenuationLinear;     /* Linear attenuation */
		float           AttenuationQuadratic;     /* Quadratic attenuation */
		float           Theta;            /* Inner angle of spotlight cone */
		float           Phi;              /* Outer angle of spotlight cone */
	}
	PXLight;




	typedef struct PXFontPageCharacter_
	{
		PXInt32U ID;
		float Position[2]; // Position of the character image in the texture.
		float Size[2];	// Size of the character image in the texture.
		float Offset[2];// Offset from the position-center.
		int XAdvance; // How much the current position should be advanced after drawing the character.
	}
	PXFontPageCharacter;

	typedef struct PXFontPage_
	{
		PXSize CharacteListSize;
		PXFontPageCharacter* CharacteList;
		PXTexture2D Texture;
	}
	PXFontPage;

	PXPublic PXFontPageCharacter* PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID);

#define PXFontPageGet(adress, index) (!currentPageIndex) ?  &(adress)->MainPage : &(adress)->AdditionalPageList[currentPageIndex-1];
	


	typedef struct PXFont_
	{
		PXFontPage MainPage;

		PXFontPage* AdditionalPageList;
		PXSize AdditionalPageListSize;
	}
	PXFont;



	typedef enum PXBlendingMode_
	{
		PXBlendingModeInvalid,

		PXBlendingModeNone,

		PXBlendingModeOneToOne // Direct 1:1 mixing
	}
	PXBlendingMode;



	// Shapes

	typedef struct PXRectangleF_
	{
		float X;
		float Y;
		float Width;
		float Height;
	}
	PXRectangleF;



	typedef struct PXSprite
	{
		PXMatrix4x4F Position;

		PXTexture2D TextureReference; // This could be a pointer

		PXVector2F TextureScaleOffset;

		PXMargin Margin;
	}
	PXSprite;




	// Camera
	typedef enum PXCameraPerspective_
	{
		PXCameraPerspective2D,
		PXCameraPerspective3D // Perspective
	}
	PXCameraPerspective;

	typedef struct PXCamera_
	{
		PXMatrix4x4F MatrixModel;
		PXMatrix4x4F MatrixView;
		PXMatrix4x4F MatrixProjection;

		float WalkSpeed;
		float ViewSpeed;

		PXVector3F LookAtPosition;
		PXVector3F CurrentRotation;

		//---<Follow>---
		PXVector3F Offset;
		PXMatrix4x4F* Target;
		float FollowSpeed; // Ranges from 0 to 1 .. FollowSpeed; = 0.98f

		PXCameraPerspective Perspective;

		float FieldOfView;
		PXSize Height;
		PXSize Width;
		float Near;
		float Far;
	}
	PXCamera;


	typedef	struct PXSkyBox_
	{
		PXVertexStructure VertexStructure;

		PXTextureCube TextureCube;

		PXShaderProgram* ShaderProgramReference;
	}
	PXSkyBox;



	typedef struct PXSound_
	{
		PXInt32U SampleRate;
		PXInt16U NumerOfChannels;
		PXInt16U BitsPerSample;

		PXSize DataSize;
		void* Data;
	}
	PXSound;




#ifdef __cplusplus
}
#endif


#endif