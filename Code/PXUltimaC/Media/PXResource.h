#ifndef PXResourceINCLUDE
#define PXResourceINCLUDE

#include <OS/File/PXFile.h>
#include <Media/PXColor.h>
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
		PXGraphicSystemOpenGL,
		PXGraphicSystemDirectX,
		PXGraphicSystemVulcan
	}
	PXGraphicSystem;

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

#define PXRectangleOffsetSet(adress, l, t, r, b) \
	(adress)->Left = l; \
	(adress)->Top = t; \
	(adress)->Right = r; \
	(adress)->Bottom = b;





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

		PXSize VertexDataRowSize;

		PXVertexBufferFormat Format;
	}
	PXVertexBuffer;

	PXPublic void* PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferDataType pxVertexBufferDataType, const PXSize index);




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



	typedef struct PXIndexRange_
	{
		void* DataPoint;
		PXSize DataRange;

	}
	PXIndexRange;

	// Index buffer, used to store the vertex render order.
	// Additionally contains info about how to actually render, like modes.
	typedef struct PXIndexBuffer_
	{
		PXResourceID ResourceID; // IDirect3DIndexBuffer9

		PXInt8U IndexTypeSize;

		void* IndexData;
		PXInt32U IndexDataSize;
		PXInt32U IndexDataAmount;

		PXDataType DataType;
		PXInt32U DrawModeID;

		PXTexture2D* Texture2D;
	}
	PXIndexBuffer;


	typedef struct PXVertexStructure_ PXVertexStructure;

	typedef struct PXVertexStructure_
	{
		PXResourceID ResourceID;

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

		PXVertexStructure* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
		PXVertexStructure* StructureParent; // Structural parent of structure
		PXVertexStructure* StructureSibling; // Stuctual sibling, works like a linked list.
		PXVertexStructure* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
		//-----------------------------


		//-----------------------------
		// Settings
		//-----------------------------
		PXBool IgnoreViewPosition; // Removes positiondata from the view matrix
		PXBool IgnoreViewRotation; // remove rotationdata from the view matrix
		PXRectangleOffset Margin;
		//-----------------------------
	}
	PXVertexStructure;


	PXPublic void PXVertexStructureConstruct(PXVertexStructure* const pxVertexStructure);
	PXPublic void PXVertexStructureDestruct(PXVertexStructure* const pxVertexStructure);




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
		unsigned int PXID;

		PXVector2F TextureScaleOffset;

		PXVertexStructure VertexStructure;
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
		PXInt32S Height;
		PXInt32S Width;
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
		AudioDeviceTypeInvalid,
		AudioDeviceTypeInput,
		AudioDeviceTypeOutput
	}
	PXAudioDeviceType;

	typedef struct PXAudioDevice_
	{
		struct
		{
			int HandleID;
		}
		Handle;

		PXInt32U DeviceID;

		char DeviceName[256];
		char DisplayName[256];
		PXAudioDeviceRole Role;


		float Pitch; // [0.5 - 2.0]
		float Volume;
		float Position[3];
		float Velocity[3];
		PXBool Looping;



		PXAudioDeviceType Type;

		PXInt16U ManufacturerID;
		PXInt16U ProductID;
		PXInt16U DriverVersionMajor;
		PXInt16U DriverVersionMinor;

		PXInt32U SupportFlags;
		PXInt32U FormatSupportFlags;

		PXInt16U        FormatTag;         /* format type */
		PXInt16U        Channels;          /* number of channels (i.e. mono, stereo...) */
		PXInt32U       SamplesPerSecound;     /* sample rate */
		PXInt32U       AverageBytesPerSecound;    /* for buffer estimation */
		PXInt16U        BlockAlignSize;        /* block size of data */
		PXInt16U        BitsPerSample;     /* number of bits per sample of mono data */

		union
		{
			PXInt16U wValidBitsPerSample; // Valid bits in each sample container
			PXInt16U wSamplesPerBlock;    // Samples per block of audio data; valid
			// if wBitsPerSample=0 (but rarely used).
			PXInt16U wReserved;           // Zero if neither case above applies.
		} Samples;
		PXInt32U dwChannelMask;          // Positions of the audio channels


		#if OSWindows
		GUID SubFormat;               // Format identifier GUID
		#endif
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





	typedef struct PXVideo_
	{
		int __Dummy__;
	}
	PXVideo;




    typedef struct PXMaterialContainer_
    {
        int __dummy__;
    }
    PXMaterialContainer;





	PXPublic PXActionResult PXFileTypeInfoProbe(PXFileTypeInfo* const pxFileTypeInfo, const PXText* const pxText);


	PXPublic PXActionResult PXResourceLoad(void* resource, const PXText* const filePath);
	PXPublic PXActionResult PXResourceLoadA(void* resource, const char* const filePath);


#ifdef __cplusplus
}
#endif

#endif
