#ifndef PXResourceINCLUDE
#define PXResourceINCLUDE

#include <Math/PXMatrix.h>

#include "PXImage.h"

#ifdef __cplusplus
extern "C"
{
#endif


	// Predefine
	typedef enum PXActionResult_ PXActionResult;
	typedef enum PXFileFormat_ PXFileFormat;

	typedef struct PXFile_ PXFile;
	typedef struct PXText_ PXText;
	typedef struct PXDocument_ PXDocument;
	typedef struct PXUIElement_ PXUIElement;
	typedef struct PXFileTypeInfo_ PXFileTypeInfo;


	typedef enum PXRefreshRateMode_
	{
		PXRefreshRateUnlimited,
		PXRefreshRateVSync,
		PXRefreshRateCustomSync
	}
	PXRefreshRateMode;

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
		PXGraphicResourceTypeImage,
		PXGraphicResourceTypeTexure,
		PXGraphicResourceTypeSkyBox,
		PXGraphicResourceTypeFont,
		PXGraphicResourceTypeSound
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
		PXShaderTypePixel,   // .frag - a fragment shader
		PXShaderTypeTessellationControl,    // .tesc - a tessellation control shader
		PXShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
		PXShaderTypeGeometry,      // .geom - a geometry shader
		PXShaderTypeCompute,   // .comp - a compute shader
	}
	PXGraphicShaderType;

	PXPublic const char* PXAPI PXGraphicShaderTypeToString(const PXGraphicShaderType pxGraphicShaderType);

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

	typedef enum PXResourceAction_
	{
		PXResourceActionInvalid,
		PXResourceActionCreate,
		PXResourceActionDelete,
		PXResourceActionUpdate,
		PXResourceActionSelect
	}
	PXResourceAction;


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
		PXVertexBufferFormatT2F_XYZ, // TT PPP
		PXVertexBufferFormatT4F_XYZW,
		PXVertexBufferFormatT2F_C4UB_XYZ,
		PXVertexBufferFormatT2F_C3F_XYZ,
		PXVertexBufferFormatT2F_N3F_XYZ, // TT NNN PPP
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

	PXPublic PXInt8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat);



	// To keep track of the object reference.
	// OpenGL uses 32-Bit Integer as an ID.
	// DirectX uses direct pointers to object references.
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
#define PXResourceIDIsUsed(pxPesourceID) (pxPesourceID)->DirectXInterface != (void*)-1
#define PXResourceIDMarkAsUnused(pxPesourceID) (pxPesourceID)->DirectXInterface = (void*)-1


	// PXPadding;: Offset the space as the child container (Make object take more space)
	// PXMargin;  Offset the whole child-Container (Make empty space)
	typedef struct PXRectangleOffset_
	{
		float Left;
		float Top;
		float Right;
		float Bottom;
	}
	PXRectangleOffset;

	PXPublic inline void PXAPI PXRectangleOffsetSet
	(
		PXRectangleOffset* const pxRectangleOffset,
		float left,
		float top,
		float right,
		float bottom
	);





	typedef struct PXTexture1D_
	{
		PXResourceID ResourceID;

		PXGraphicRenderFilter Filter;
		PXGraphicImageLayout LayoutNear;
		PXGraphicImageLayout LayoutFar;
		PXGraphicImageWrap WrapHeight;
		PXGraphicImageWrap WrapWidth;

		struct PXImage_* Image;
	}
	PXTexture1D;

	typedef struct PXTexture2D_
	{
		PXResourceID ResourceID;

		PXGraphicRenderFilter Filter;
		PXGraphicImageLayout LayoutNear;
		PXGraphicImageLayout LayoutFar;
		PXGraphicImageWrap WrapHeight;
		PXGraphicImageWrap WrapWidth;

		struct PXImage_* Image;
	}
	PXTexture2D;

#define PXTexture2DSet(texture, format, width, height) \
	(texture)->ResourceID.DirectXInterface = (void*)-1; \
	(texture)->Filter = PXGraphicRenderFilterNoFilter; \
	(texture)->LayoutNear = PXGraphicImageLayoutNearest; \
	(texture)->LayoutFar = PXGraphicImageLayoutNearest; \
	(texture)->WrapHeight= PXGraphicImageWrapStrechEdges; \
	(texture)->WrapWidth = PXGraphicImageWrapStrechEdges; \
	(texture)->Image->Width = width; \
	(texture)->Image->Height = height;  \
	(texture)->Image->PixelDataSize = 0;  \
	(texture)->Image->PixelData = 0; \
	(texture)->Image->Format = format; \


	typedef struct PXTexture3D_
	{
		PXResourceID ResourceID;

		struct PXImage_* Image;
	}
	PXTexture3D;

	// A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
	typedef struct PXTextureCube_
	{
		PXResourceID ResourceID;

		PXColorFormat Format;

		struct PXImage_* ImageA;
		struct PXImage_* ImageB;
		struct PXImage_* ImageC;
		struct PXImage_* ImageD;
		struct PXImage_* ImageE;
		struct PXImage_* ImageF;
	}
	PXTextureCube;


	typedef struct PXTexture_
	{
		PXGraphicTextureType Type;

		union
		{
			PXTexture1D Texture1D;
			PXTexture2D Texture2D;
			PXTexture3D Texture3D;
			PXTextureCube TextureCube;
		};
	}
	PXTexture;

	typedef enum PXMaterialIlluminationMode_
	{
		IlluminationNone,
		IlluminationColorAndAmbientDisable,		// [0] Color on and Ambient off
		IlluminationColorAndAmbientEnable,		// [1] Color on and Ambient on
		IlluminationHighlightEnable,	// [2] Highlight on
		IlluminationReflectionOnRayTraceEnable,	// [3] Reflection on and Ray trace on
		IlluminationReflectionOnRayTraceTransparency, 	// [4] Transparency: Glass on, Reflection : Ray trace on
		IlluminationReflectionOnRayTraceFresnel, 	// [5] Reflection : Fresnel on and Ray trace on
		IlluminationReflectionOnRayTraceTransparencyFresnel, 	// [6] Transparency : Refraction on, Reflection : Fresnel offand Ray trace on
		IlluminationReflectionOnRayTraceFullEnable,	// [7] Transparency : Refraction on, Reflection : Fresnel onand Ray trace on
		IlluminationReflectionEnable, 	// [8] Reflection on and Ray trace off
		IlluminationTransparencyEnable, 	// [9] Transparency : Glass on, Reflection : Ray trace off
		IlluminationShadowsEnable  	// [10] Casts shadows onto invisible surfaces
	}
	PXMaterialIlluminationMode;

	// Material for a surface to render on
	typedef struct PXMaterial_
	{
		char Name[32];

		float Diffuse[4];
		float Ambient[4];
		float Specular[4];	// shininess
		float Emission[4];
		float Power;        // Sharpness if specular highlight

		float Weight; 		// Ranges between 0 and 1000
		float Dissolved;
		float Density; // range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.


		char DiffuseTextureFilePath[260];

		struct PXTexture2D_* DiffuseTexture;

		PXMaterialIlluminationMode IlluminationMode;
	}
	PXMaterial;

	// Container to save a list of materials.
	typedef struct PXMaterialContainer_
	{
		PXSize MaterialListAmount;
		PXMaterial* MaterialList;
	}
	PXMaterialContainer;


	PXPublic PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName);



	typedef enum PXShaderVariableType_
	{
		PXShaderVariableTypeInvalid,

		PXShaderVariableTypeBoolSignle,
		PXShaderVariableTypeBoolVector2,
		PXShaderVariableTypeBoolVector3,
		PXShaderVariableTypeBoolVector4,

		PXShaderVariableTypeInt32SSingle,
		PXShaderVariableTypeInt32SVector2,
		PXShaderVariableTypeInt32SVector3,
		PXShaderVariableTypeInt32SVector4,

		PXShaderVariableTypeInt32USingle,
		PXShaderVariableTypeInt32UVector2,
		PXShaderVariableTypeInt32UVector3,
		PXShaderVariableTypeInt32UVector4,

		PXShaderVariableTypeFloatSingle,
		PXShaderVariableTypeFloatVector2,
		PXShaderVariableTypeFloatVector3,
		PXShaderVariableTypeFloatVector4,
		PXShaderVariableTypeDouble,

		// Matrix
		PXShaderVariableTypeMatrix2x2,
		PXShaderVariableTypeMatrix2x3,
		PXShaderVariableTypeMatrix2x4,
		PXShaderVariableTypeMatrix3x2,
		PXShaderVariableTypeMatrix3x3,
		PXShaderVariableTypeMatrix3x4,
		PXShaderVariableTypeMatrix4x2,
		PXShaderVariableTypeMatrix4x3,
		PXShaderVariableTypeMatrix4x4,

		PXShaderVariableTypeSampler2DF,
		PXShaderVariableTypeSampler2DI32S,
		PXShaderVariableTypeSampler2DI32U,
		PXShaderVariableTypeSamplerF2DShadow,

		PXShaderVariableTypeSampler2DArrayF,
		PXShaderVariableTypeSampler2DArrayI32S,
		PXShaderVariableTypeSampler2DArrayI32U,

		PXShaderVariableTypeSamplerF2DArrayShadow,

		PXShaderVariableTypeSampler3DF,
		PXShaderVariableTypeSampler3DI32S,
		PXShaderVariableTypeSampler3DI32U,

		PXShaderVariableTypeSamplerCubeF,
		PXShaderVariableTypeSamplerCubeI32S,
		PXShaderVariableTypeSamplerCubeI32U,

		PXShaderVariableTypeSamplerFCubeShadow,
	}
	PXShaderVariableType;

	typedef enum PXShaderVariableLocallity_
	{
		PXShaderVariableLocallityInvalid,
		PXShaderVariableLocallityGlobal, // Uniform
		PXShaderVariableLocallityVertexOnly, // Atrribute
	}
	PXShaderVariableLocallity;

#define PXShaderVariableNameSize 32

	typedef struct PXShaderVariable_
	{
		char Name[PXShaderVariableNameSize]; // Shader variable name, used only for and ID fetch as it is very slow!
		PXSize NameSize;
		PXShaderVariableType DataType;
		PXInt32U RegisterIndex; // ID to make, 
		PXSize DataTypeSize;

		// Only for DirectX
		PXInt32U RegisterCount;
		PXInt32U Rows;
		PXInt32U Columns;
		PXInt32U Elements;
		PXInt32U StructMembers;


		// Used for moving data only
		void* Data; // Raw data data that is defined by the type
		PXSize Amount;
		PXGraphicShaderType ShaderType;
		PXShaderVariableLocallity Locallity;
	}
	PXShaderVariable;

	typedef struct PXShader_
	{
		PXResourceID ResourceID;

		PXGraphicShaderType Type;

		PXInt8U VersionMajor;
		PXInt8U VersionMinor;

		char* ShaderFilePath;

		PXSize ContentSize;
		const char* Content;

		PXSize VariableListAmount;
		PXShaderVariable* VariableListData;
	}
	PXShader;


	typedef struct PXShaderProgram_
	{
		PXResourceID ResourceID;

		PXSize VariableListAmount;
		PXShaderVariable* VariableListData;
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


	//-----------------------------------------------------
	// Vertex rendering info
	//-----------------------------------------------------

	typedef enum PXVertexBufferDataType_
	{
		PXVertexBufferDataTypeInvalid,

		PXVertexBufferDataTypeVertex,
		PXVertexBufferDataTypeTexture,
		PXVertexBufferDataTypeNormal
	}
	PXVertexBufferDataType;

	typedef struct PXVertexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DVertexBuffer9, ID3D11Buffer

		void* VertexData;
		PXSize VertexDataSize;
		PXSize VertexDataAmount;

		PXSize VertexDataRowSize;

		PXVertexBufferFormat Format;
	}
	PXVertexBuffer;

	PXPublic void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferDataType pxVertexBufferDataType, const PXSize index);




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



	typedef struct PXIndexSegment_
	{
		PXMaterial* Material;
		PXSize DataRange;
	}
	PXIndexSegment;

	// Index buffer, used to store the vertex render order.
	// Additionally contains info about how to actually render, like modes.
	typedef struct PXIndexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DIndexBuffer9

		PXInt8U IndexTypeSize;

		void* IndexData;
		PXSize IndexDataSize;

		PXInt32U IndexDataAmount;

		PXInt32U DataType;
		PXInt32U DrawModeID;

		PXSize SegmentListSize;
		PXSize SegmentListAmount;

		union
		{
			PXIndexSegment* SegmentList;
			PXIndexSegment SegmentPrime; // Only used if Segment is only one
		};	
	}
	PXIndexBuffer;

	typedef struct PXModel_
	{
		PXResourceID ResourceID;
		PXBool Enabled;

		//-----------------------------
		// Render info
		//-----------------------------
		PXMatrix4x4F ModelMatrix;
		PXShaderProgram* ShaderProgramReference;
		//-----------------------------


		//-----------------------------
		// Vertex data
		//-----------------------------
		PXVertexBuffer VertexBuffer;
		PXIndexBuffer IndexBuffer;

		struct PXModel_* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
		struct PXModel_* StructureParent; // Structural parent of structure
		struct PXModel_* StructureSibling; // Stuctual sibling, works like a linked list.
		struct PXModel_* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
		//-----------------------------

		//PXSize MaterialContaierListSize;
		PXSize MaterialContaierListAmount;
		PXMaterialContainer* MaterialContaierList;

		//-----------------------------
		// Settings
		//-----------------------------
		PXBool IgnoreViewPosition; // Removes positiondata from the view matrix
		PXBool IgnoreViewRotation; // remove rotationdata from the view matrix
		PXBool RenderBothSides;
		PXRectangleOffset Margin;
		//-----------------------------
	}
	PXModel;


	PXPublic void PXAPI PXModelConstruct(PXModel* const pxModel);
	PXPublic void PXAPI PXModelDestruct(PXModel* const pxModel);




	typedef struct PXVertexElement_
	{
		PXInt32U Type;
		PXSize Length;
		PXSize Stride;
		PXSize StartAdress;
	}
	PXVertexElement;



	typedef struct PXDepthStencilSurface_
	{
		int x;
	}
	PXDepthStencilSurface;





	// The rectangle of the view you can render in.
	typedef struct PXViewPort_
	{
		PXInt32S X;
		PXInt32S Y;
		PXInt32S Width;
		PXInt32S Height;
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

		float Diffuse[4];           // Diffuse color of light
		float Specular[4];          // Specular color of light
		float Ambient[4];           // Ambient color of light
		float Position[3];          // Position in world space
		float Direction[3];         // Direction in world space
		float CutoffRange;          // Cutoff range
		float Falloff;              // Falloff
		float AttenuationConstant;  // Constant attenuation
		float AttenuationLinear;    // Linear attenuation
		float AttenuationQuadratic; // Quadratic attenuation
		float Theta;                // Inner angle of spotlight cone
		float Phi;                  // Outer angle of spotlight cone

		PXLightType Type;           // Type of light source

		PXBool Enabled;
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
		PXSize CharacteListEntrys;
		PXFontPageCharacter* CharacteList;
		
		struct PXTexture2D_* Texture;
		char TextureFilePath[260];
	}
	PXFontPage;

	PXPublic PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID);

#define PXFontPageGet(adress, index) (!currentPageIndex) ?  &(adress)->MainPage : &(adress)->AdditionalPageList[currentPageIndex-1];



	typedef struct PXFont_
	{
		PXFontPage MainPage;

		PXFontPage* AdditionalPageList;
		PXSize AdditionalPageListSize;

		PXInt32U PXID;
	}
	PXFont;



	typedef enum PXBlendingMode_
	{
		PXBlendingModeInvalid,

		PXBlendingModeNone,

		PXBlendingModeOneToOne, // Direct 1:1 mixing
		PXBlendingModeSoureAlphaOnly
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
		PXInt32U PXID;
		PXBool Enabled;

		char Name[20];

		PXVector2F TextureScalePositionOffset;
		PXVector2F TextureScalePointOffset;

		PXModel Model;

		struct PXTexture2D_* Texture;
	}
	PXSprite;




	// Camera
	typedef enum PXCameraPerspective_
	{
		PXCameraPerspectiveInvalid,
		PXCameraPerspective2D,
		PXCameraPerspective3D // Perspective
	}
	PXCameraPerspective;

	typedef struct PXCamera_
	{
		PXMatrix4x4F MatrixModel;
		PXMatrix4x4F MatrixView;
		PXMatrix4x4F MatrixProjection;	

		PXVector3F LookAtPosition;
		PXVector3F CurrentRotation;

		//---<Follow>---
		PXVector3F Offset;
		PXMatrix4x4F* Target;
		float FollowSpeed; // Ranges from 0 to 1 .. FollowSpeed; = 0.98f

		PXCameraPerspective Perspective;

		float FieldOfView;
		PXInt32S Height;
		PXInt32S Width;
		float Near;
		float Far;

		float WalkSpeed;
		float ViewSpeed;
	}
	PXCamera;


	typedef	struct PXSkyBox_
	{
		struct PXModel_* Model;
		struct PXTextureCube_* TextureCube;
		struct PXShaderProgram_* ShaderProgramReference;
	}
	PXSkyBox;





	//-----------------------------------------------------
	// UI-Element
	//-----------------------------------------------------
#define PXUIElementDoRendering (1 << 0) 
#define PXUIElementIsActive (1 << 1)
#define PXUIElementIsHoverable (1 << 2)
#define PXUIElementDrawBorder (1 << 3)

#define PXUIElementNormal PXUIElementDoRendering | PXUIElementIsActive | PXUIElementIsHoverable
#define PXUIElementDecorative PXUIElementDoRendering | PXUIElementIsActive | PXUIElementDrawBorder
#define PXUIElementText PXUIElementDoRendering | PXUIElementIsActive 


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
		PXUIElementTypeCustom,
		PXUIElementTypeWindow,
		PXUIElementTypePanel,
		PXUIElementTypeText,
		PXUIElementTypeButton,
		PXUIElementTypeButtonText,
		PXUIElementTypeImage,
		PXUIElementTypeDropDown,
		PXUIElementTypeListBox,
		PXUIElementTypeTextEdit,
		PXUIElementTypeRichEdit,
		PXUIElementTypeScrollBar,
		PXUIElementTypeTrackBar,
		PXUIElementTypeStatusBar,
		PXUIElementTypeUpDown,
		PXUIElementTypeProgressBar,
		PXUIElementTypeHotKey,
		PXUIElementTypeCalender,
		PXUIElementTypeToolTip,
		PXUIElementTypeAnimate,
		PXUIElementTypeDatePicker,
		PXUIElementTypeGroupBox,
		PXUIElementTypeRadioButton,
		PXUIElementTypeGroupRadioButton,
		PXUIElementTypeTreeView,
		PXUIElementTypeTreeViewItem,
		PXUIElementTypeIPInput,
		PXUIElementTypeLink,
		PXUIElementTypeHeader,
		PXUIElementTypeFontSelector,
		PXUIElementTypePageScroll,
		PXUIElementTypeTabControll,
		PXUIElementTypeToggle,
		PXUIElementTypeCheckBox,
		PXUIElementTypeComboBox,
		PXUIElementTypeColorPicker,
		PXUIElementTypeSlider,
		PXUIElementTypeImageList,
		PXUIElementTypeRenderFrame
	}
	PXUIElementType;

	PXPublic const char* PXAPI  PXUIElementTypeToString(const PXUIElementType pxUIElementType);


	typedef enum PXUIElementInteractType_
	{
		PXUIElementInteractTypeInvalid,
		PXUIElementInteractTypeSelect,
		PXUIElementInteractTypeClick,
		PXUIElementInteractTypeMouseEnter,
		PXUIElementInteractTypeMouseLeave
	}
	PXUIElementInteractType;


	typedef struct PXUIElementFrameBufferInfo_
	{
		struct PXTexture2D_* TextureReference;

		PXInt32U Width;
		PXInt32U Height;
		PXInt32U BufferID;
		PXInt32U RenderID;
	}
	PXUIElementFrameBufferInfo;

	typedef struct PXUIElementImageInfo_
	{
		struct PXTexture2D_* TextureReference;
	}
	PXUIElementImageInfo;

	typedef enum PXUIElementTextAllign_
	{
		PXUIElementTextAllignInvalid,
		PXUIElementTextAllignLeft,
		PXUIElementTextAllignRight,
		PXUIElementTextAllignCenter
	}
	PXUIElementTextAllign;

	typedef struct PXUIElementTextInfo_
	{
		char* Content;
		struct PXFont_* FontID;
		float Scale;
		PXUIElementTextAllign Allign;
	}
	PXUIElementTextInfo;

	typedef struct PXUIElementItemInfo_
	{
		char* TextData;
		PXSize TextSize;
	}
	PXUIElementItemInfo;

	typedef struct PXUIElementSceneRenderInfo_
	{
		struct PXEngine_* Engine;
		struct PXEngineStartInfo_* StartInfo;
	}
	PXUIElementSceneRenderInfo;

	typedef struct PXUIElementButtonInfo_
	{
		PXUIElementTextInfo TextInfo;
	}
	PXUIElementButtonInfo;

	typedef struct PXUIElementProgressBarInfo_
	{
		float Percentage;
		PXColorRGBI8 BarColor;
	}
	PXUIElementProgressBarInfo;








// Ancering will stick the given edge to a side.
// Offset will be interpretet 0=NoSpace, 1=???
// Goal: Scale the object with screensize
#define PXUIElementAncerParent			0b11110000
#define PXUIElementAncerParentLeft		0b10000000
#define PXUIElementAncerParentTop		0b01000000
#define PXUIElementAncerParentRight		0b00100000
#define PXUIElementAncerParentBottom	0b00010000

// Let siblings calulate their offset themself.
// Goal: Group multible objects together that belong together
#define PXUIElementAncerSibling			0b00001111
#define PXUIElementAncerSiblingLeft		0b00001000
#define PXUIElementAncerSiblingTop		0b00000100
#define PXUIElementAncerSiblingRight	0b00000010
#define PXUIElementAncerSiblingBottom	0b00000001

#define PXUIElementKeepFlags		0b00001111
#define PXUIElementKeepPositionX	0b00000001
#define PXUIElementKeepPositionY	0b00000010
#define PXUIElementKeepWidth		0b00000100
#define PXUIElementKeepHeight		0b00001000

#define PXUIElementAllignFlags		0b11110000
#define PXUIElementAllignLeft		0b00010000
#define PXUIElementAllignTop		0b00100000
#define PXUIElementAllignRight		0b01000000
#define PXUIElementAllignBottom		0b10000000
#define PXUIElementAllignCenter		0b11110000

	typedef struct PXUIElementPosition_
	{
		float MarginLeft;
		float MarginTop;
		float MarginRight;
		float MarginBottom;

		float PaddingLeft;
		float PaddingTop;
		float PaddingRight;
		float PaddingBottom;

		float X;
		float Y;
		float Width;
		float Height;

		PXInt16U FlagListFormat; // Unused
		PXInt8U FlagListAncer;
		PXInt8U FlagListKeep;
	}
	PXUIElementPosition;


	typedef void (PXAPI* PXWindowEventFunction)(void* const owner, struct PXWindowEvent_* const pxWindowEvent);


	// Atomic UI-Element
	// Only Text can be text
	// Only image can be image
	typedef struct PXUIElement_
	{
		char NameData[128];
		PXSize NameSize;

		//------------------------------
		// References 
		//------------------------------
		struct PXUIElement_* Parent;
		struct PXUIElement_* Sibling;
		struct PXUIElement_* Child;

		//------------------------------
		// Events
		//------------------------------
		void* InteractOwner;
		PXWindowEventFunction InteractCallBack;


		PXUIElementPosition Position;

		//---<State-Info>------------------------
		PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
		PXUIHoverState Hover;
		PXInt32U FlagsList;
		//---------------------------------------

		//---<Property>--------------------------
		//PXUIElementPositionMode PositionMode;

#if 0
		union
		{
			PXUIElementFrameBufferInfo FrameBufferInfo;
			PXUIElementImageInfo ImageInfo;
			PXUIElementTextInfo TextInfo;
			PXUIElementProgressBarInfo ProgressBar;
			PXUIElementButtonInfo Button;
			PXUIElementItemInfo Item;
			PXUIElementSceneRenderInfo SceneRender;
		};
#endif

		PXUIElementType Type;
		PXSize ID;
	}
	PXUIElement;
	//-----------------------------------------------------



	typedef struct PXUIElementPositionCalulcateInfo_
	{
		// Input
		float WindowWidth;
		float WindowHeight;

		// Result
		PXInt32U DepthCounter;
		
		// Margin total
		float MarginLeft;
		float MarginTop;
		float MarginRight;
		float MarginBottom;


		float AA;
		float BA;
		float BB;
		float AB;

		float X;
		float Y;
		float Width;
		float Height;
	}
	PXUIElementPositionCalulcateInfo;


	PXPublic void PXAPI PXUIElementPositionCalculcate(PXUIElement* const pxUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo);








	typedef struct PXSound_
	{
		void* BaseObject;
		void* Data;

		PXSize DataSize;

		PXInt32U ChunkSize;
		PXInt32U SampleRate;
		PXInt32U ByteRate;

		PXInt16U AudioFormat;
		PXInt16U NumerOfChannels;
		PXInt16U BlockAllign;
		PXInt16U BitsPerSample;
	}
	PXSound;





	typedef enum PXAudioDeviceRole_
	{
		PXAudioDeviceRoleInvalid,
		PXAudioDeviceRoleNotDefault,
		PXAudioDeviceRoleDefaultConsole,
		PXAudioDeviceRoleDefaultMultimedia,
		PXAudioDeviceRoleDefaultCommunications,
		PXAudioDeviceRoleDefaultGame,
		PXAudioDeviceRoleGlobalDefault
	}
	PXAudioDeviceRole;


	typedef enum PXAudioDeviceType_
	{
		PXAudioDeviceTypeInvalid,
		PXAudioDeviceTypeInput,
		PXAudioDeviceTypeOutput
	}
	PXAudioDeviceType;

#define PXAudioDeviceNameSize 64
#define PXAudioFX3DPositionEnable	(1 <<  0) 
#define PXAudioFXPlaySpeedEnable	(1 <<  1)
#define PXAudioFXVolumeMONOEnable	(1 <<  2)
#define PXAudioFXVolumeSTEREOEnable	(1 <<  3)
#define PXAudioFXEnable				(1 <<  4)
#define PXAudioFXGargle				(1 <<  5) // modulates the amplitude of the signal
#define PXAudioFXChorus				(1 <<  6) // Mixing of similar pitches
#define PXAudioFXFlanger			(1 <<  7) // Delay with smal distortion over time, simpar effect as "Chorus"
#define PXAudioFXEcho				(1 <<  8) // Delay and Repeat
#define PXAudioFXDistortion			(1 <<  9)
#define PXAudioFXCompressor			(1 << 10)
#define PXAudioFXParamEq			(1 << 11)
#define PXAudioFXWavesReverb		(1 << 12)
#define PXAudioFXI3DL2Reverb		(1 << 13)




	typedef struct PXAudioDeviceInput_
	{
		int x;
	}
	PXAudioDeviceInput;

	typedef struct PXAudioDeviceOutput_
	{
		int x;
	}
	PXAudioDeviceOutput;

	typedef struct PXAudioDevice_
	{
		PXResourceID ResourceID;

#if OSWindows
		union
		{
			UINT DeviceID; // Windows MultiMedia
			GUID DeviceGUID; // DirectSound Device Identifier GUID
		};		
#endif

	
		PXAudioDeviceRole Role;
		PXAudioDeviceType Type;
		PXInt32U FXSettingFlagList;


		float Pitch; // [0.5 - 2.0]
		float Volume;
		float Position[3];
		float Velocity[3];
		PXBool Looping;


	
	
		//-------------------------------------------------
		// General Shared Info
		//-------------------------------------------------
		char DeviceName[PXAudioDeviceNameSize];
		PXInt16U ManufacturerID;
		PXInt16U ProductID;
		PXInt8U DriverVersionMajor;
		PXInt8U DriverVersionMinor;
		PXInt32U SupportFlags;
		//-------------------------------------------------

	
		PXInt32U FormatSupportFlags;

		PXInt16U FormatTag;         // format type 
		PXInt16U Channels;          // number of channels (i.e. mono, stereo...)
		PXInt32U SamplesPerSecound;     // sample rate
		PXInt32U AverageBytesPerSecound;    // for buffer estimation 
		PXInt16U BlockAlignSize;        // block size of data 
		PXInt16U BitsPerSample;     // number of bits per sample of mono data

		//-------------------------------------------------
		// Used By MIDI
		//-------------------------------------------------
		PXInt16U Technology;           // type of device 
		PXInt16U Voices;               // # of voices (internal synth only) 
		PXInt16U Notes;                // max # of notes (internal synth only)
		PXInt16U ChannelMask;          // channels used (internal synth only)
		//-------------------------------------------------

		union
		{
			PXInt16U wValidBitsPerSample; // Valid bits in each sample container
			PXInt16U wSamplesPerBlock;    // Samples per block of audio data; valid
			// if wBitsPerSample=0 (but rarely used).
			PXInt16U wReserved;           // Zero if neither case above applies.
		} Samples;
		PXInt32U dwChannelMask;          // Positions of the audio channels


		void* SoundBuffer;
		void* Buffer3DInterface;
		void* Listen3DInterface;
		void* FXGargle;
		void* FXChorus;
		void* FXFlanger;
		void* FXEcho;
		void* FXDistortion;
		void* FXCompressor8;
		void* FXParamEq;
		void* FXWavesReverb;
		void* FXI3DL2Reverb;
	}
	PXAudioDevice;



	/*
	typedef struct PXAudioSource_
	{
		void* Xinterface; // IXAudio2SourceVoice

		unsigned int ID;
		float Pitch; // [0.5 - 2.0]
		float Volume;
		float Position[3];
		float Velocity[3];
		unsigned char Looping;


		//PXInt16U        wFormatTag;         /* format type * /
		PXInt16U Channels;          /* number of channels (i.e. mono, stereo...) * /
		PXInt32U SamplesPerSecound;     /* sample rate * /
		PXInt32U AverageBytesPerSecound;    /* for buffer estimation * /
		PXInt16U BlockAlignSize;        /* block size of data * /
		PXInt16U BitsPerSample;     /* number of bits per sample of mono data * /


	}
	PXAudioSource;*/




#define PXDeviceDisplaySize 32
#define PXDeviceNameSize 32
#define PXDeviceIDSize 64
#define PXDeviceKeySize 128

#define PXDeviceOpenGLDriverSize 32
#define PXDeviceOpenGLVendorSize 32
#define PXDeviceOpenGLRendererSize 32
#define PXDeviceOpenGLShaderSize 32

#define MonitorNameLength 32

	typedef struct PXMonitor_
	{
		char Name[MonitorNameLength];
		char Driver[MonitorNameLength];
		PXInt16U X;
		PXInt16U Y;
		PXInt16U Width;
		PXInt16U Height;
	}
	PXMonitor;

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

		PXMonitor AttachedMonitor;
	}
	PXGraphicDevicePhysical;



	typedef struct PXVideo_
	{
		int __Dummy__;
	}
	PXVideo;



	typedef struct PXResourceLoadInfo_
	{
		void* Target;
		PXFile* FileReference;
		PXGraphicResourceType Type;
	}
	PXResourceLoadInfo;

	typedef struct PXResourceSaveInfo_
	{
		void* Target;	
		PXFile* FileReference;
		PXGraphicResourceType Type;
		PXFileFormat Format;
	}
	PXResourceSaveInfo;




	PXPublic PXActionResult PXAPI PXFileTypeInfoProbe(PXFileTypeInfo* const pxFileTypeInfo, const PXText* const pxText);


	PXPublic PXActionResult PXAPI PXResourceLoad(PXResourceLoadInfo* const pxResourceLoadInfo, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXResourceLoadA(PXResourceLoadInfo* const pxResourceLoadInfo, const char* const filePath);

	PXPublic PXActionResult PXAPI PXResourceSave(PXResourceSaveInfo* const pxResourceSaveInfo, const PXText* const filePath);
	PXPublic PXActionResult PXAPI PXResourceSaveA(PXResourceSaveInfo* const pxResourceSaveInfo, const char* const filePath);

#ifdef __cplusplus
}
#endif

#endif
