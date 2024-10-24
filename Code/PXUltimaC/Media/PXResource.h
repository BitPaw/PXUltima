#ifndef PXResourceINCLUDE
#define PXResourceINCLUDE

#include <Math/PXMatrix.h>

#include "PXImage.h"
#include <Container/FlexDataCache/PXFlexDataCache.h>
#include <Container/HierarchicalList/PXHierarchicalList.h>
#include <Container/Dictionary/PXDictionary.h>
//#include <OS/File/PXDirectory.h>
#include <OS/Memory/PXMemory.h>

#include <stdarg.h>

#if OSUnix
#include <stdio.h>
#include <X11/X.h> // X11 XID, ulong 8 Byte
#include <X11/Xlib.h> // XDisplay
#endif

//#include <OS/File/PXFile.h>

#define PXBoilerPlate

// Predefine
typedef enum PXActionResult_ PXActionResult;
typedef enum PXFileFormat_ PXFileFormat;

typedef struct PXCodeDocumentElement_ PXCodeDocumentElement;
typedef struct PXFile_ PXFile;
typedef struct PXText_ PXText;
typedef struct PXCodeDocument_ PXCodeDocument;
typedef struct PXGUISystem_ PXGUISystem;
typedef struct PXGUIElement_ PXGUIElement;
typedef struct PXFileTypeInfo_ PXFileTypeInfo;
typedef struct PXCompiler_ PXCompiler;
typedef struct PXProcessor_ PXProcessor;
typedef struct PXHierarchicalNode_ PXHierarchicalNode;
typedef struct PXFileEntry_ PXFileEntry;

extern void _chkstk(size_t s);

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
    PXShaderTypeVertex,                 // .vert - vertex shader
    PXShaderTypePixel,                  // .frag - fragment shader
    PXShaderTypeTessellationControl,    // .tesc - tessellation control shader
    PXShaderTypeTessellationEvaluation, // .tese - tessellation evaluation shader
    PXShaderTypeGeometry,               // .geom - geometry shader
    PXShaderTypeCompute,                // .comp - compute shader
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

    // PXUltima-Custom
    PXVertexBufferFormatXYI8,
    PXVertexBufferFormatXYZI8, 




    // OpenGL

    PXVertexBufferFormatXYFloat, // GL_V2F
    PXVertexBufferFormatC4UB_XY,
    PXVertexBufferFormatC4UB_XYZ,
    PXVertexBufferFormatRGBXYZ,
    PXVertexBufferFormatN3F_XYZ, // NNN PPP
    PXVertexBufferFormatC4F_N3F_XYZ,
    PXVertexBufferFormatT2F_XYZ, // TT PPP
    PXVertexBufferFormatT4F_XYZW,
    PXVertexBufferFormatT2F_C4UB_XYZ,
    PXVertexBufferFormatT2F_C3F_XYZ,
    PXVertexBufferFormatT2F_N3F_XYZ, // TT NNN PPP
    PXVertexBufferFormatT2F_C4F_N3F_XYZ,
    PXVertexBufferFormatT4F_C4F_N3F_XYZW,

    // Direct X

    PXVertexBufferFormatXYZFloat, // Normal spcace (x, y, z)
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

PXPublic const char* PXAPI PXVertexBufferFormatToString(const PXVertexBufferFormat pxVertexBufferFormat);
PXPublic PXInt8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat);





















//---------------------------------------------------------
// Resource types and structs
//---------------------------------------------------------

// Describes the exact type of a resource
typedef enum PXResourceType_
{
    PXResourceTypeInvalid, // Invalid, don't use- only for internal

    PXResourceTypeCustom            = PXInt32Make('C', 'U', 'S', 'T'), // Undetected but valid format. Needs to be handled by the caller

    //-----------------------------------------------------
    // Resource Level 0 - Internal OS Resources
    //-----------------------------------------------------
    PXResourceTypeBrush             = PXInt32Make('B', 'R', 'S', 'H'),
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Resource Level 1 - Indepepended
    //-----------------------------------------------------
    PXResourceTypeImage             = PXInt32Make('I', 'M', 'A', 'G'), // Image for pixeldata
    PXResourceTypeSound             = PXInt32Make('S', 'O', 'U', 'D'),
    PXResourceTypeVideo             = PXInt32Make('V', 'I', 'D', 'E'),
    PXResourceTypeModel             = PXInt32Make('M', 'O', 'D', 'L'), // 3D model, collection of vertex data
    PXResourceTypeFont              = PXInt32Make('F', 'O', 'N', 'T'), // Collection of spites or points to render text
    PXResourceTypeMaterial          = PXInt32Make('M', 'T', 'R', 'L'),
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Resource Level 2 - Context spesific
    //-----------------------------------------------------
    PXResourceTypeTexture2D         = PXInt32Make('T', 'X', '2', 'D'), // Texture to render on a surface
    PXResourceTypeTextureCube       = PXInt32Make('T', 'X', 'C', 'U'),
    PXResourceTypeShaderProgram     = PXInt32Make('S', 'H', 'A', 'D'),
    PXResourceTypeSkybox            = PXInt32Make('S', 'K', 'Y', 'B'),
    PXResourceTypeSprite            = PXInt32Make('S', 'P', 'R', 'I'),
    PXResourceTypeSpriteAnimator    = PXInt32Make('S', 'A', 'N', 'I'),
    PXResourceTypeText              = PXInt32Make('T', 'E', 'X', 'T'),
    PXResourceTypeTimer             = PXInt32Make('T', 'I', 'M', 'E'),
    PXResourceTypeEngineSound       = PXInt32Make('E', 'S', 'N', 'D'),
    PXResourceTypeGUIElement        = PXInt32Make('G', 'U', 'I', 'E'),
    PXResourceTypeHitBox            = PXInt32Make('H', 'B', 'O', 'X'),
    PXResourceTypeMaterialList      = PXInt32Make('M', 'L', 'I', 'S'),
    PXResourceTypeCodeDocument      = PXInt32Make('C', 'O', 'D', 'E'),
    PXResourceTypeDocument          = PXInt32Make('D', 'O', 'C', 'U'),
    PXResourceTypeBinary            = PXInt32Make('B', 'I', 'N', 'A'),
    PXResourceTypeStructuredText    = PXInt32Make('D', 'O', 'C', 'U'),
    PXResourceTypeInstaller         = PXInt32Make('I', 'N', 'S', 'T'), // compressed executable
    PXResourceTypeArchiv            = PXInt32Make('A', 'R', 'C', 'H'), // Compressed object
    //-----------------------------------------------------

    // Extended basic components
    PXResourceTypeDialogBox
}
PXResourceType;

#define ResourceIDNameLength 64

#define PXResourceInfoIdentityMask    0b0000000000001111
#define PXResourceInfoExist           0b0000000000000001 // Indicate if resource is valid
#define PXResourceInfoActive          0b0000000000000010 // Is it interactable or does it tick?
#define PXResourceInfoRender          0b0000000000000100 // Shall it be rendered?

#define PXResourceInfoStorageMask     0b0000000011110000
#define PXResourceInfoStorageDrive    0b0000000000010000 // Resource is in permanent storage
#define PXResourceInfoStorageCached   0b0000000000100000 // Resource is in semi-permanent cache (temp file)
#define PXResourceInfoStorageMemory   0b0000000001000000 // Resource exists in RAM
#define PXResourceInfoStorageDevice   0b0000000010000000 // Resource exists in spesific device

#define PXResourceInfoNameMask        0b0000001100000000
#define PXResourceInfoHasName         0b0000000100000000 // Is the name stored
#define PXResourceInfoHasSource       0b0000001000000000 // Is resource loaded from a path, if yes, path is cached.

#define PXResourceInfoUseByMask       0b0001100000000000
#define PXResourceInfoUseByOS         0b0000000000000000
#define PXResourceInfoUseByUser       0b0000100000000000
#define PXResourceInfoUseByEngine     0b0001000000000000
#define PXResourceInfoUseByUndefined  0b0001100000000000


#define PXResourceInfoOK              PXResourceInfoExist | PXResourceInfoActive | PXResourceInfoRender




// IDs used by rendering APIs to keep track of the object reference.
// OpenGL uses 32-Bit Integer as an ID.
// DirectX uses direct pointers to object references.
typedef union PXOSHandle_
{
    PXInt32U OpenGLID; // Simple ID for an object.
    void* DirectXInterface; // DirectX uses interfaces to communicate to a element.
#if OSUnix
    Window WindowID; // Linux X11 System
#elif OSWindows
    HWND WindowID; // Windows only, used for GUI elements
    HBRUSH BrushHandle;
    HFONT FontHandle;
    HMENU MenuHandle;
    HICON IconHandle;
#endif
}
PXOSHandle;

// Internal engine identification
// Additional use is to define current storage and interactions.
typedef struct PXResourceInfo_
{
    PXHierarchicalNode Hierarchy;

    PXOSHandle Handle;

    PXInt32U ID; // Identification of this object managed by the engine itself.
    PXInt32U Flags; // general information
    PXInt32U Behaviour; // Depends on the type of the resource
}
PXResourceInfo;

// Object to use instead of a plain adress. 
// This can assure we have the correct and expected object.
// This should prevent stale references
typedef struct PXResourceReference_
{
    PXInt32U IDExpected; // Key to precheck expected ID behind this reference
    void* ResourceAdress; // Reference to actual object. Check if this adress is in range.
}
PXResourceReference;


























typedef struct PXResource_
{
    PXResourceType Type;
    void* ResourceAdress;
}
PXResource;



// Container to manage resources by loading or saving
typedef struct PXResourceManager_
{
    PXFlexDataCache NameCache;
    PXFlexDataCache SourcePathCache;

    // Register List
    PXInt32U UniqeIDGeneratorCounter;

    PXDictionary MaterialLookUp;
    PXDictionary SpritelLookUp;
    PXDictionary FontLookUp;
    PXDictionary TextLookUp;
    PXDictionary TimerLookUp;
    PXDictionary SoundLookUp;
    PXDictionary HitBoxLookUp;
    PXDictionary ImageLookUp;
    PXDictionary BrushLookUp;
    PXDictionary TextureLookUp;
    PXDictionary ModelLookUp;
    PXDictionary SkyBoxLookUp;
    PXDictionary ShaderProgramLookup;
    PXDictionary GUIElementLookup;
    PXDictionary SpriteAnimator;

    struct PXShaderProgram_* ShaderFailback;
    struct PXModel_* ModelFailback;
    struct PXTexture2D_* Texture2DFailBack;
}
PXResourceManager;
//=========================================================







//---------------------------------------------------------
// Icon/Symbols
//---------------------------------------------------------

#define PXIconxx

typedef enum PXIconID_
{
    PXIconIDInvalid,
    PXIconIDCustom,
    PXIconIDError,
    PXIconIDWarning,
    PXIconIDQuestionmark
}
PXIconID;

// Icon/Symbol to be rendered for GUI Elements
typedef struct PXIcon_
{
    PXResourceInfo Info;
}
PXIcon;

typedef struct PXIconAtlas_
{
    int dummy;
}
PXIconAtlas;

PXPublic void PXAPI PXIconLoad(PXIcon* const pxIcon);

//---------------------------------------------------------











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
    PXResourceInfo Info;

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
    PXResourceInfo Info;

    PXGraphicRenderFilter Filter;
    PXGraphicImageLayout LayoutNear;
    PXGraphicImageLayout LayoutFar;
    PXGraphicImageWrap WrapHeight;
    PXGraphicImageWrap WrapWidth;

    struct PXImage_* Image;
}
PXTexture2D;

typedef struct PXTexture3D_
{
    PXResourceInfo Info;

    struct PXImage_* Image;
}
PXTexture3D;

// A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
typedef struct PXTextureCube_
{
    PXResourceInfo Info;

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
    IlluminationColorAndAmbientDisable,        // [0] Color on and Ambient off
    IlluminationColorAndAmbientEnable,        // [1] Color on and Ambient on
    IlluminationHighlightEnable,    // [2] Highlight on
    IlluminationReflectionOnRayTraceEnable,    // [3] Reflection on and Ray trace on
    IlluminationReflectionOnRayTraceTransparency,     // [4] Transparency: Glass on, Reflection : Ray trace on
    IlluminationReflectionOnRayTraceFresnel,     // [5] Reflection : Fresnel on and Ray trace on
    IlluminationReflectionOnRayTraceTransparencyFresnel,     // [6] Transparency : Refraction on, Reflection : Fresnel offand Ray trace on
    IlluminationReflectionOnRayTraceFullEnable,    // [7] Transparency : Refraction on, Reflection : Fresnel onand Ray trace on
    IlluminationReflectionEnable,     // [8] Reflection on and Ray trace off
    IlluminationTransparencyEnable,     // [9] Transparency : Glass on, Reflection : Ray trace off
    IlluminationShadowsEnable      // [10] Casts shadows onto invisible surfaces
}
PXMaterialIlluminationMode;

// Material for a surface to render on
typedef struct PXMaterial_
{
    PXResourceInfo Info;

    // Name would be too wasteful here, we shall store it in another container
    // TexturePath can not be put here, but we might store it differently.

    float Diffuse[4];
    float Ambient[4];
    float Specular[4];    // shininess
    float Emission[4];
    float Power;        // Sharpness if specular highlight

    float Weight;         // Ranges between 0 and 1000
    float Dissolved;
    float Density; // range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

    struct PXTexture2D_* DiffuseTexture;

    PXMaterialIlluminationMode IlluminationMode;
}
PXMaterial;


// Container to save a list of materials.
// This is only ment to be a faster lookup for a model.
typedef struct PXMaterialContainer_
{
    PXSize MaterialListAmount;
    PXMaterial* MaterialList;
}
PXMaterialContainer;




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
    PXResourceInfo Info;

    PXGraphicShaderType Type;

    PXInt8U VersionMajor;
    PXInt8U VersionMinor;

    struct PXFile_* ShaderFile;

    PXSize VariableListAmount;
    PXShaderVariable* VariableListData;
}
PXShader;


typedef struct PXShaderProgram_
{
    PXResourceInfo Info;

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
    PXResourceInfo Info;

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
    PXResourceInfo Info;

    void* VertexData;
    PXSize VertexDataSize;

    PXVertexBufferFormat Format;
}
PXVertexBuffer;

PXPublic void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferDataType pxVertexBufferDataType, const PXSize index);




#define PXDrawModeIDQuad                    (1 << 0)
#define PXDrawModeIDInvalid                 (1 << 1)
#define PXDrawModeIDPoint                   (1 << 2)
#define PXDrawModeIDLine                    (1 << 3)
#define PXDrawModeIDLineLoop                (1 << 4)
#define PXDrawModeIDLineStrip               (1 << 5)
#define PXDrawModeIDLineStripAdjacency      (1 << 6)
#define PXDrawModeIDLineAdjacency           (1 << 7)
#define PXDrawModeIDTriangle                (1 << 8)
#define PXDrawModeIDTriangleAdjacency       (1 << 9)
#define PXDrawModeIDTriangleFAN             (1 << 10)
#define PXDrawModeIDTriangleStrip           (1 << 11)
#define PXDrawModeIDTriangleStripAdjacency  (1 << 12)
#define PXDrawModeIDWireFrame               (1 << 13)
#define PXDrawModeIDSquare                  (1 << 14)
#define PXDrawModeIDPatches                 (1 << 15)



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
    PXResourceInfo Info;

    void* IndexData;
    PXSize IndexDataSize;
    PXInt32U IndexDataType;

    PXInt32U DrawModeID; // How to draw, modes like triangle or lines

    PXSize SegmentListSize;
    PXSize SegmentListAmount;

    union
    {
        PXIndexSegment* SegmentList;
        PXIndexSegment SegmentPrime; // Only used if Segment is only one
    };
}
PXIndexBuffer;

// A mesh is a structure that contains vertex and index data to render itself
typedef struct PXMesh_
{
    PXVertexBuffer VertexBuffer;
    PXIndexBuffer IndexBuffer;
}
PXMesh;

typedef struct PXModel_
{
    PXResourceInfo Info;

    PXMesh Mesh;

    //-----------------------------
// Render info
//-----------------------------
    PXMatrix4x4F ModelMatrix;
    PXShaderProgram* ShaderProgramReference;
    //-----------------------------

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



typedef struct PXRenderEntity_
{
    PXMatrix4x4F MatrixModel; // Position of target to render
    struct PXCamera_* CameraReference; // Camera required for rendering
    struct PXShaderProgram_* ShaderProgramReference; // Shader for the whole model
    struct PXMaterial_* MaterialOverride;
    struct PXTexture2D_* Texture2DOverride;
    void* ObjectReference; // Containing the object, type described in 'Type'
    PXResourceType Type;

    // Dirty flags
}
PXRenderEntity;





PXPublic void PXAPI PXModelConstruct(PXModel* const pxModel);
PXPublic void PXAPI PXModelDestruct(PXModel* const pxModel);


typedef struct PXModelFormatTransmuteInfo_
{
    // What formats are supported
    PXSize VertexFormatAmount;
    PXVertexBufferFormat* VertexFormatList;


    PXVertexBufferFormat VertexFormatPreference;
}
PXModelFormatTransmuteInfo;


PXPublic void PXAPI PXModelFormatTransmute(PXModel* const pxModel, PXModelFormatTransmuteInfo* const pxModelFormatTransmuteInfo);



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
    PXResourceInfo Info; // IDirect3DVertexBuffer9
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
    PXResourceInfo Info;

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
    float Size[2];    // Size of the character image in the texture.
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
}
PXFontPage;

PXPublic PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID);

#define PXFontPageGet(adress, index) (!currentPageIndex) ?  &(adress)->PagePrime : &(adress)->PageList[currentPageIndex-1];



#define PXFontAntialiased   (1<<0)
#define PXFontItalics       (1<<1)
#define PXFontBold          (1<<2)
#define PXFontStrikeOut     (1<<3)
#define PXFontUnderline     (1<<4)

typedef struct PXFont_
{
    PXResourceInfo Info;

    PXSize PageListAmount;
    PXInt16U Size;

    union
    {
        PXFontPage PagePrime;
        PXFontPage* PageList;
    };
}
PXFont;











//-----------------------------------------------------
// Text
//-----------------------------------------------------
typedef struct PXEngineText_
{
    PXResourceInfo Info;

    PXVector2F Position;
    PXVector2F Scaling;

    PXBool DoubleRenderForShadow;

    PXText* Text;
    PXFont* Font;
    float FontScaling;

    PXInt32U TextRenderAmount;
}
PXEngineText;
//-----------------------------------------------------





//-----------------------------------------------------
// Audio
//-----------------------------------------------------
typedef struct PXEngineSound_
{
    struct PXSound_* Sound;

    PXInt32U PXID;

    PXBool SoundLoop;
}
PXEngineSound;





//-----------------------------------------------------
// HitBox
//-----------------------------------------------------
typedef enum PXHitBoxForm_
{
    PXHitBoxTypeInvalid,
    PXHitBoxTypeBox,
    PXHitBoxTypeCube,
    PXHitBoxTypeCircle
}
PXHitBoxForm;

//---------------------------------------------------------
// BehaviourFlag
//---------------------------------------------------------
#define PXHitBoxBehaviourKeepOut 0b00000001 // Prevent from entering
#define PXHitBoxBehaviourKeepIn  0b00000010 // Prevent from leaving
#define PXHitBoxBehaviourDetect  0b00000100 // Trigger if in inside
#define PXHitBoxBehaviourPhysics 0b00001000 // Apply physics
#define PXHitBoxBehaviourGravity 0b00010000 // Apply Gravity
//---------------------------------------------------------

typedef void(PXAPI* PXHitBoxCollisionDetect)(void* owner, struct PXHitBox_* const pxHitBox);

// Collidable entity that can be defined for different behaviours
typedef struct PXHitBox_
{
    PXResourceInfo Info;

    PXModel* Model;

    PXHitBoxForm Form;

    struct PXHitBox_* ColliderChild;
    struct PXHitBox_* ColliderParent;

    void* CallBackOwner;
    PXHitBoxCollisionDetect CollisionDetectCallBack;
}
PXHitBox;
//-----------------------------------------------------
















//-----------------------------------------------------
// Timer
//-----------------------------------------------------
typedef struct PXEngineTimerEventInfo_
{
    struct PXEngineTimer_* TimerReference;

    PXInt32U Acuracy;

    PXBool WasHandled;

    //PXInt32S DelayShift;
}
PXEngineTimerEventInfo;

typedef PXActionResult(PXAPI* PXTimerCallBack)(void* const owner, PXEngineTimerEventInfo* const pxEngineTimerEventInfo);

typedef struct PXEngineTimer_
{
    PXResourceInfo Info;

    void* Owner;
    PXTimerCallBack CallBack;

    PXInt32U TimeStampStart;

    PXInt32U TimeDeltaTarget;
    PXInt32S TimeDelayShift;
}
PXEngineTimer;
//-----------------------------------------------------

















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
    PXResourceInfo Info;

    PXVector2F TextureScalePositionOffset;
    PXVector2F TextureScalePointOffset;

    PXModel* Model;
    PXTexture2D* Texture;
    PXMaterial* Material; // Use this instand of a texture, right?
    PXHitBox* HitBox;
    PXShaderProgram* ShaderProgarm;
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

#define PXCameraFollowPosition 0
#define PXCameraFollowRotation 0
#define PXCameraFollowRotationY 0


typedef struct PXCamera_
{
    PXMatrix4x4F MatrixModel;
    PXMatrix4x4F MatrixView;
    PXMatrix4x4F MatrixProjection;

    PXVector3F LookAtPosition;
    PXVector3F CurrentRotation;

    //---<Follow>---
    PXVector3F Offset;
    PXVector3F DeadZone;
    //PXInt8U TargetFollowFlag;
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

    PXBool LockMovement;
    PXBool LockView;
}
PXCamera;


typedef struct PXSkyBox_
{
    PXResourceInfo Info;

    struct PXModel_* Model;
    struct PXTextureCube_* TextureCube;
    struct PXShaderProgram_* ShaderProgramReference;
}
PXSkyBox;





//-----------------------------------------------------
// UI-Element
//-----------------------------------------------------

// Ancering will stick the given edge to a side.
// Offset will be interpretet 0=NoSpace, 1=???
// Goal: Scale the object with screensize
#define PXGUIElementAncerParent           0b0000000000000000000000000000000000001111
#define PXGUIElementAncerParentLeft       0b0000000000000000000000000000000000000001
#define PXGUIElementAncerParentTop        0b0000000000000000000000000000000000000010
#define PXGUIElementAncerParentRight      0b0000000000000000000000000000000000000100
#define PXGUIElementAncerParentBottom     0b0000000000000000000000000000000000001000

// Let siblings calulate their offset themself.
// Goal: Group multible objects together that belong together
#define PXGUIElementAncerSibling          0b0000000000000000000000000000000011110000
#define PXGUIElementAncerSiblingLeft      0b0000000000000000000000000000000000010000
#define PXGUIElementAncerSiblingTop       0b0000000000000000000000000000000000100000
#define PXGUIElementAncerSiblingRight     0b0000000000000000000000000000000001000000
#define PXGUIElementAncerSiblingBottom    0b0000000000000000000000000000000010000000

#define PXGUIElementKeepFlags             0b0000000000000000000000000000111100000000
#define PXGUIElementKeepPositionX         0b0000000000000000000000000000000100000000
#define PXGUIElementKeepPositionY         0b0000000000000000000000000000001000000000
#define PXGUIElementKeepWidth             0b0000000000000000000000000000010000000000
#define PXGUIElementKeepHeight            0b0000000000000000000000000000100000000000

// Allign content inside a element
#define PXGUIElementAllignFlags           0b0000000000000000000000011111000000000000
#define PXGUIElementAllignLeft            0b0000000000000000000000000001000000000000
#define PXGUIElementAllignTop             0b0000000000000000000000000010000000000000
#define PXGUIElementAllignRight           0b0000000000000000000000000100000000000000
#define PXGUIElementAllignBottom          0b0000000000000000000000001000000000000000
#define PXGUIElementAllignCenter          0b0000000000000000000000010000000000000000


#define PXGUIElementBehaviourHoverable    0b0000000000000000000000100000000000000000
#define PXGUIElementBehaviourSelectable   0b0000000000000000000001000000000000000000
#define PXGUIElementBehaviourBorder       0b0000000000000000000010000000000000000000
#define PXGUIElementBehaviourScrollBarHor 0b0000000000000000000100000000000000000000
#define PXGUIElementBehaviourScrollBarVer 0b0000000000000000001000000000000000000000


#define PXGUIElementBehaviourDefaultKeepAspect   PXGUIElementKeepWidth | PXGUIElementKeepHeight
#define PXGUIElementBehaviourDefaultDecorative   PXResourceInfoOK | PXGUIElementBehaviourBorder
#define PXGUIElementBehaviourDefaultInputNormal  PXResourceInfoOK | PXGUIElementBehaviourSelectable | PXGUIElementBehaviourHoverable
#define PXGUIElementBehaviourDefaultText         PXResourceInfoOK | PXGUIElementAllignTop | PXGUIElementKeepHeight | PXGUIElementAllignCenter
#define PXGUIElementBehaviourDefaultBuffer       PXGUIElementBehaviourDefaultKeepAspect

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
    PXUIElementTypeWindow, // General window to draw into
    PXUIElementTypePanel, // static generic element
    PXUIElementTypeText, // text, letters
    PXUIElementTypeButton, // Button to click
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
    PXUIElementTypeFileDirectyView,
    PXUIElementTypeFileDirectyViewEntry,
    PXUIElementTypeIPInput,
    PXUIElementTypeLink,
    PXUIElementTypeHeader, // Object insode another for header info
    PXUIElementTypeFontSelector,
    PXUIElementTypePageScroll,
    PXUIElementTypeTabControll,
    PXUIElementTypeToggle,
    PXUIElementTypeCheckBox,
    PXUIElementTypeComboBox,
    PXUIElementTypeColorPicker,
    PXUIElementTypeSlider,
    PXUIElementTypeImageList,
    PXUIElementTypeRenderFrame,

    PXUIElementTypeMenuStrip, // Topline of a window that contains a selectable list of things

    PXUIElementTypeFileManager // TreeView of filesystem with utility
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

typedef struct PXUIElementComboBoxInfo_
{
    char** DataList;
    PXSize DataListAmount;
}
PXUIElementComboBoxInfo;




#if OSUnix
typedef PXInt32U PXWindowID; // XID
#elif OSWindows
typedef HWND PXWindowID; // HWND
#endif




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

    float Left;
    float Top;
    float Right;
    float Bottom;

   // PXInt16U FlagListFormat; // Unused
 //   PXInt8U FlagListAncer;
   // PXInt8U FlagListKeep;
}
PXUIElementPosition;


typedef void (PXAPI* PXWindowEventFunction)(void* const owner, struct PXWindowEvent_* const pxWindowEvent);


typedef struct PXRectangle_
{
    int Left;
    int Top;
    int Right;
    int Bottom;
}
PXRectangle;


typedef struct PXGUIElementDrawInfo_
{
    PXRectangle Rectangle;

#if OSUnix
    int dummy;
#elif OSWindows
    HWND hwnd;
    HDC hDC;
    BOOL bErase;
#endif
}
PXGUIElementDrawInfo;

typedef PXActionResult (PXAPI* PXGUIElementDrawFunction)(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXGUIElementDrawInfo* const pxGUIElementDrawInfo);


//---------------------------------------------------------
// Brush
//---------------------------------------------------------

// if true, color is a struct, otherwise a pointer to that struct
#define PXGUIElementBrushBehaviourColorEmbeded  (1<<0)

// Color of GUI Element and tools to apply
typedef struct PXGUIElementBrush_
{
    PXResourceInfo Info;

    union
    {
        PXColorRGBI8 ColorDate;
        PXColorRGBI8* ColorReference;
    };
}
PXGUIElementBrush;

PXPublic void PXAPI PXGUIElementBrushColorSet(PXGUIElementBrush* const pxGUIElementBrush, const PXByte red, const PXByte green, const PXByte blue);

//---------------------------------------------------------

/*
typedef struct PXGUIElementData_
{
    union
    {
        PXDirectorySearchCache DirectorySearchCache; // Used for a treeview
    };     
}
PXGUIElementData;
*/

// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXGUIElement_
{
    PXResourceInfo Info;

#if OSUnix
#elif OSWindows
    HDC DeviceContextHandle; // Required for manual rendering
#endif

    void* InteractOwner; // Object that is the owner of given callbacks
    PXWindowEventFunction InteractCallBack; // Callback function for all events
    PXGUIElementDrawFunction DrawFunction; // If this is set, override OS-drawing and use this function instead


    PXUIElementPosition Position;

    //---<State-Info>------------------------
    PXGUIElementBrush* BrushFront;
    PXGUIElementBrush* BrushBackground;
    PXFont* FontForText;

    //PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
    PXUIHoverState Hover;
    PXInt32U FlagsList;
    //---------------------------------------


    PXUIElementType Type;

    void* ExtendedData; // Extra data that will be allocated seperatly for additional use

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

    struct PXGUIElement_** ListEEData;
    PXSize ListEESize;



  


    // Change this to something better
    char* NameContent;
    PXSize NameContentSize;
}
PXGUIElement;
//-----------------------------------------------------




#define PXGUIMenuItemTypeImage      (1<<0)
#define PXGUIMenuItemTypeCheckmark  (1<<1)
#define PXGUIMenuItemTypeDATA       (1<<2)
#define PXGUIMenuItemTypeFTYPE      (1<<3)
#define PXGUIMenuItemTypeID         (1<<4)
#define PXGUIMenuItemTypeSTATE      (1<<5)
#define PXGUIMenuItemTypeSTRING     (1<<6)
#define PXGUIMenuItemTypeSUBMENU    (1<<7)
#define PXGUIMenuItemTypeTYPE       (1<<8)
#define PXGUIMenuItemTypeMFT_MENUBARBREAK  (1<<10)
#define PXGUIMenuItemTypeMFT_MENUBREAK          (1<<11)
#define PXGUIMenuItemTypeMFT_OWNERDRAW         (1<<12)
#define PXGUIMenuItemTypeMFT_RADIOCHECK         (1<<13)
#define PXGUIMenuItemTypeMFT_RIGHTJUSTIFY         (1<<14)
#define PXGUIMenuItemTypeMFT_RIGHTORDER         (1<<15)
#define PXGUIMenuItemTypeMFT_SEPARATOR         (1<<16)
#define PXGUIMenuItemTypeMFT_STRING         (1<<17)

#define PXGUIMenuItemStateMFS_CHECKED  (1<<0)
#define PXGUIMenuItemStateMFS_DEFAULT  (1<<1)
#define PXGUIMenuItemStateMFS_DISABLED  (1<<2)
#define PXGUIMenuItemStateMFS_ENABLED  (1<<3)
#define PXGUIMenuItemStateMFS_GRAYED  (1<<4)
#define PXGUIMenuItemStateMFS_HILITE  (1<<5)
#define PXGUIMenuItemStateMFS_UNCHECKED  (1<<6)
#define PXGUIMenuItemStateMFS_UNHILITE  (1<<7)


typedef struct PXGUIElementMenuItemInfo_
{
    PXInt32U Flags;
    PXInt32U State;
    PXGUIElement* Parent;
    struct PXGUIElementMenuItemList_* ChildList;

    char* TextData;
    PXSize TextSize;



}
PXGUIElementMenuItemInfo;

typedef struct PXGUIElementMenuItemList_
{
    PXGUIElementMenuItemInfo* MenuItemInfoListData;
    PXSize MenuItemInfoListAmount;
}
PXGUIElementMenuItemList;






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





typedef enum PXUIElementProperty_
{
    PXUIElementPropertyInvalid,
    PXUIElementPropertyTextContent,
    PXUIElementPropertyTextAllign,
    PXUIElementPropertyTextColor,
    PXUIElementPropertySize,
    PXUIElementPropertySizeParent,

    PXUIElementPropertySliderPercentage,

    PXUIElementPropertyProgressbarPercentage,
    PXUIElementPropertyProgressbarBarColor,

    PXUIElementPropertyBackGroundColor,

    PXUIElementPropertyVisibility,

    PXUIElementPropertyComboBoxAdd,


    PXUIElementPropertyItemAdd,
    PXUIElementPropertyItemDelete,
    PXUIElementPropertyItemUpdate,

    // Tree view

    PXUIElementPropertyTreeView_CreateDragImage,
    PXUIElementPropertyTreeView_DeleteAllItems,
    PXUIElementPropertyTreeView_DeleteItem,
    PXUIElementPropertyTreeView_EditLabel,
    PXUIElementPropertyTreeView_EndEditLabelNow,
    PXUIElementPropertyTreeView_EnsureVisible,
    PXUIElementPropertyTreeView_Expand,
    PXUIElementPropertyTreeView_GetBkColor,
    PXUIElementPropertyTreeView_GetCheckState,
    PXUIElementPropertyTreeView_GetChild,
    PXUIElementPropertyTreeView_GetCount,
    PXUIElementPropertyTreeView_GetDropHilight,
    PXUIElementPropertyTreeView_GetEditControl,
    PXUIElementPropertyTreeView_GetExtendedStyle,
    PXUIElementPropertyTreeView_GetFirstVisible,
    PXUIElementPropertyTreeView_GetImageList,
    PXUIElementPropertyTreeView_GetIndent,
    PXUIElementPropertyTreeView_GetInsertMarkColor,
    PXUIElementPropertyTreeView_GetISearchString,
    PXUIElementPropertyTreeView_GetItem,
    PXUIElementPropertyTreeView_GetItemHeight,
    PXUIElementPropertyTreeView_GetItemPartRect,
    PXUIElementPropertyTreeView_GetItemRect,
    PXUIElementPropertyTreeView_GetItemState,
    PXUIElementPropertyTreeView_GetLastVisible,
    PXUIElementPropertyTreeView_GetLineColor,
    PXUIElementPropertyTreeView_GetNextItem,
    PXUIElementPropertyTreeView_GetNextSelected,
    PXUIElementPropertyTreeView_GetNextSibling,
    PXUIElementPropertyTreeView_GetNextVisible,
    PXUIElementPropertyTreeView_GetParent,
    PXUIElementPropertyTreeView_GetPrevSibling,
    PXUIElementPropertyTreeView_GetPrevVisible,
    PXUIElementPropertyTreeView_GetRoot,
    PXUIElementPropertyTreeView_GetScrollTime,
    PXUIElementPropertyTreeView_GetSelectedCount,
    PXUIElementPropertyTreeView_GetSelection,
    PXUIElementPropertyTreeView_GetTextColor,
    PXUIElementPropertyTreeView_GetToolTips,
    PXUIElementPropertyTreeView_GetUnicodeFormat,
    PXUIElementPropertyTreeView_GetVisibleCount,
    PXUIElementPropertyTreeView_HitTest,
    PXUIElementPropertyTreeView_InsertItem,
    PXUIElementPropertyTreeView_MapAccIDToHTREEITEM,
    PXUIElementPropertyTreeView_MapHTREEITEMtoAccID,
    PXUIElementPropertyTreeView_Select,
    PXUIElementPropertyTreeView_SelectDropTarget,
    PXUIElementPropertyTreeView_SelectItem,
    PXUIElementPropertyTreeView_SelectSetFirstVisible,
    PXUIElementPropertyTreeView_SetAutoScrollInfo,
    PXUIElementPropertyTreeView_SetBkColor,
    PXUIElementPropertyTreeView_SetBorder,
    PXUIElementPropertyTreeView_SetCheckState,
    PXUIElementPropertyTreeView_SetExtendedStyle,
    PXUIElementPropertyTreeView_SetHot,
    PXUIElementPropertyTreeView_SetImageList,
    PXUIElementPropertyTreeView_SetIndent,
    PXUIElementPropertyTreeView_SetInsertMark,
    PXUIElementPropertyTreeView_SetInsertMarkColor,
    PXUIElementPropertyTreeView_SetItem,
    PXUIElementPropertyTreeView_SetItemHeight,
    PXUIElementPropertyTreeView_SetItemState,
    PXUIElementPropertyTreeView_SetLineColor,
    PXUIElementPropertyTreeView_SetScrollTime,
    PXUIElementPropertyTreeView_SetTextColor,
    PXUIElementPropertyTreeView_SetToolTips,
    PXUIElementPropertyTreeView_SetUnicodeFormat,
    PXUIElementPropertyTreeView_ShowInfoTip,
    PXUIElementPropertyTreeView_SortChildren,
    PXUIElementPropertyTreeView_SortChildrenCB,

}
PXUIElementProperty;





typedef enum PXUIElementTreeViewItemInsertMode_
{
    PXUIElementTreeViewItemInsertModeROOT,
    PXUIElementTreeViewItemInsertModeFIRST,
    PXUIElementTreeViewItemInsertModeLAST,
    PXUIElementTreeViewItemInsertModeSORT
}
PXUIElementTreeViewItemInsertMode;

typedef struct PXUIElementTreeViewItemInfo_
{
    char* TextDataOverride;
    PXSize TextSizeOverride;

    struct PXGUIElement_* ItemParent;
    struct PXGUIElement_* TreeView;

    //struct PXUIElement_* ElementSource;
    void* OwningObject;
    PXResourceType OwningObjectType;

    PXUIElementTreeViewItemInsertMode InsertMode;

    // Result
    struct _TREEITEM* ItemHandle;
}
PXUIElementTreeViewItemInfo;


typedef struct PXUIElementTabPageSingleInfo_
{
    PXGUIElement** UIElement;
    char* PageName;
    PXInt32U ImageID;

}
PXUIElementTabPageSingleInfo;

typedef struct PXUIElementTabPageInfo_
{
    PXUIElementTabPageSingleInfo* TabPageSingleInfoList;
    PXSize TabPageSingleInfoAmount;
}
PXUIElementTabPageInfo;



typedef struct PXGUIElementCreateWindowInfo_
{
    //void* EventFunction;
    void* EventOwner;

    PXGUIElement* UIElementReference;

    PXColorRGBAI8 BackGroundColor;

    PXInt32S X;
    PXInt32S Y;
    PXInt32S Width;
    PXInt32S Height;
    char* Title;

    PXBool IsVisible;
    PXBool CreateMessageThread; // Run events in another thread
    PXBool MaximizeOnStart;
}
PXGUIElementCreateWindowInfo;

typedef struct PXWindowSizeInfo_
{
    PXInt32S X;
    PXInt32S Y;
    PXInt32S Width;
    PXInt32S Height;
}
PXWindowSizeInfo;

typedef union PXGUIElementCreateInfoData_
{
    PXGUIElementMenuItemList MenuItem;
    PXGUIElementCreateWindowInfo Window;
    PXUIElementTextInfo Text;
    PXUIElementButtonInfo Button;
    PXUIElementTreeViewItemInfo TreeViewItem;
    PXUIElementSceneRenderInfo SceneRender;
    PXUIElementTabPageInfo TabPage;
    PXUIElementComboBoxInfo ComboBox;

    // Fetched
    PXWindowSizeInfo Size;
}
PXGUIElementCreateInfoData;

typedef struct PXGUIElementUpdateInfo_
{
    PXGUIElement* UIElement;
    PXGUIElement* WindowReference;
    PXUIElementProperty Property;
    PXGUIElementCreateInfoData Data;

    PXBool Show;
}
PXGUIElementUpdateInfo;

typedef struct PXGUIElementCreateInfo_
{
    PXGUIElement* UIElementParent;
    PXGUIElement* UIElementWindow;

    //PXInt32U FlagList;

    PXUIElementType Type;
    PXInt32U BehaviourFlags;
    PXColorRGBAF* ColorTintReference;

    PXColorRGBAF Color;


    PXBool UseCustomDraw;
    void* CustomDrawFunction;

    PXUIElementPosition Position;

#if OSUnix
#elif OSWindows
    PXInt32U WindowsWindowsStyleFlagsExtended;
    PXInt32U WindowsStyleFlags;
    char* WindowsTextContent;
    PXSize WindowsTextSize;
    const char* WindowsClassName;
#endif

    PXGUIElementDrawFunction DrawFunctionEngine; // default rendering of the engine
    PXGUIElementDrawFunction DrawFunctionOverride; // user defined rendering to overruide default

    // Additions
    PXBool AvoidCreation;

    char* Name;

    void* InteractOwner;
    PXWindowEventFunction InteractCallBack;

    PXGUIElementCreateInfoData Data;
}
PXGUIElementCreateInfo;




PXPublic void PXAPI PXUIElementPositionCalculcate(PXGUIElement* const pxGUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo);


















































typedef struct PXSound_
{
    PXResourceInfo Info;

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
#define PXAudioFX3DPositionEnable    (1 <<  0)
#define PXAudioFXPlaySpeedEnable    (1 <<  1)
#define PXAudioFXVolumeMONOEnable    (1 <<  2)
#define PXAudioFXVolumeSTEREOEnable    (1 <<  3)
#define PXAudioFXEnable                (1 <<  4)
#define PXAudioFXGargle                (1 <<  5) // modulates the amplitude of the signal
#define PXAudioFXChorus                (1 <<  6) // Mixing of similar pitches
#define PXAudioFXFlanger            (1 <<  7) // Delay with smal distortion over time, simpar effect as "Chorus"
#define PXAudioFXEcho                (1 <<  8) // Delay and Repeat
#define PXAudioFXDistortion            (1 <<  9)
#define PXAudioFXCompressor            (1 << 10)
#define PXAudioFXParamEq            (1 << 11)
#define PXAudioFXWavesReverb        (1 << 12)
#define PXAudioFXI3DL2Reverb        (1 << 13)




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
    PXResourceInfo Info;

#if OSWindows
    union
    {
        UINT DeviceID; // Windows MultiMedia
        GUID DeviceGUID; // DirectSound Device Identifier GUID
    };
#endif


    PXAudioDeviceRole Role;
    PXAudioDeviceType Type;
    PXBool IsRunning;

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
    void* FXCompressor;
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

















//---------------------------------------------------------
// File Management
//---------------------------------------------------------
typedef enum PXFileFormat_
{
    PXFileFormatInvalid,
    PXFileFormatUnkown,
    PXFileFormatA3DS,
    PXFileFormatAAC,
    PXFileFormatAVI,
    PXFileFormatBitMap,
    PXFileFormatBinkVideo,
    PXFileFormatC,
    PXFileFormatCSharp,
    PXFileFormatCSS,
    PXFileFormatCPP,
    PXFileFormatCanonRaw3,
    PXFileFormatDirectDrawSurfaceTexture,
    PXFileFormatBinaryWindows,
    PXFileFormatBinaryLinux,
    PXFileFormatEML,
    PXFileFormatFastFile,
    PXFileFormatFilmBox,
    PXFileFormatFLAC,
    PXFileFormatSpriteFont,
    PXFileFormatGIF,
    PXFileFormatHighEfficiencyImageFile,
    PXFileFormatHTML,
    PXFileFormatINI,
    PXFileFormatEugeneRoshalArchive, // .rar
    PXFileFormatJava,
    PXFileFormatJPEG,
    PXFileFormatJSON,
    PXFileFormatM4A,
    PXFileFormatMIDI,
    PXFileFormatMP3,
    PXFileFormatMP4,
    PXFileFormatMSI,
    PXFileFormatMTL,
    PXFileFormatN64,
    PXFileFormatWavefront,
    PXFileFormatMatroska, // .mkv
    PXFileFormatOGG,
    PXFileFormatPDF,
    PXFileFormatPHP,
    PXFileFormatPLY,
    PXFileFormatPNG,
    PXFileFormatQOI,
    PXFileFormatSTEP,
    PXFileFormatSTL,
    PXFileFormatSVG,
    PXFileFormatTGA,
    PXFileFormatTagImage,
    PXFileFormatTrueTypeFont,
    PXFileFormatUniversalSceneDescription,
    PXFileFormatVideoObject,
    PXFileFormatVRML,
    PXFileFormatWave,
    PXFileFormatWEBM,
    PXFileFormatWEBP,
    PXFileFormatWMA,
    PXFileFormatXML,
    PXFileFormatYAML,
    PXFileFormatZIP
}
PXFileFormat;

typedef enum PXFileLocationMode_
{
    PXFileLocationModeInvalid,
    PXFileLocationModeInternal, // Memory is handled internally.
    PXFileLocationModeExternal, // Memory is stored outside this object
    PXFileLocationModeMappedVirtual, // Used 'VirtalAlloc()' / 'mmap()'
    PXFileLocationModeMappedFromDisk, // Used 'FileView()' / 'fmap()'
    PXFileLocationModeDirectCached, // Read & Write operations are cached into a buffer first.
    PXFileLocationModeDirectUncached // Read & Write operations are directly put into
}
PXFileLocationMode;





typedef struct PXResourceTransphereInfo_ PXResourceTransphereInfo;

typedef PXActionResult (PXAPI* PXResourceFileSizePredict)(void* const resource, PXSize* const fileSize);
typedef PXActionResult (PXAPI* PXResourceTransphereFunction)(PXResourceTransphereInfo* const pxResourceTransphereInfo);



// Used for progress, to know how far we came in peek, load, register, ...
#define PXResourceTransphereDidPeek             (1 << 0)
#define PXResourceTransphereDidCompile          (1 << 1)
#define PXResourceTransphereDidLoad             (1 << 2)
#define PXResourceTransphereDidSave             (1 << 3)
#define PXResourceTransphereDidRegister         (1 << 4)
#define PXResourceTransphereDidUpload           (1 << 5)
#define PXResourceTransphereDidDestroyInRAM     (1 << 6)
#define PXResourceTransphereDidDestroyOnDevice  (1 << 7)

// Used to tell what system can be used
// Will also be used to tell what system was used
#define PXResourceTransphereOwnerOS         (1 <<  8) // Handled by OS
#define PXResourceTransphereOwnerPX         (1 <<  9) // Handled by PXUltima itself
#define PXResourceTransphereOwnerMOD        (1 << 10) // Handled by a mod that was loaded
#define PXResourceTransphereOwnerCustom     (1 << 11) // Handles by a custom injected function

// This object shall be used to define an interaction with a 
// resource to peek, load or save
typedef struct PXResourceTransphereInfo_
{
    void* Owner;                // Who is the caller?
    void* ResourceTarget;       // Generic object, tager
    PXResourceManager* Manager; // The callback manager. This is set by the resource loader itself. Used for chain dependencys
    PXFile* FileReference;      // The attached file that hold the data
    
    
    PXResourceFileSizePredict FileSizePredict;
    PXResourceTransphereFunction ResourcePeek;
    PXResourceTransphereFunction ResourceLoad;
    PXResourceTransphereFunction ResourceSave;
    
    PXResourceTransphereFunction OnDeviceDataRegister;  // Preallocate resources on the device
    PXResourceTransphereFunction OnDeviceDataUpload;    // Upload data fully
    
    void* ResourceSource;
    PXResourceType ResourceType;        // Type of the resource that 'Target' points to. Example: Image, Sound, Video...
    
    
    PXFileFormat FormatReal;
    PXFileFormat FormatExpected;        // The format detected by the resource loader

    PXInt8U Flags;

    //void* Owner;

    float TimePeek;
    float TimeTransphere;
    float TimeDeviceDataRegister;
    float TimeDeviceDataUpload;
}
PXResourceTransphereInfo;





typedef enum PXFileElementInfoType_
{
    PXFileElementInfoTypeInvalid,
    PXFileElementInfoTypeUnkown, // DT_UNKNOWN

    PXFileElementInfoTypeFile, // DT_REG
    PXFileElementInfoTypeDictionary, // DT_DIR

    PXFileElementInfoTypeNamedPipeFIFO, // DT_FIFO
    PXFileElementInfoTypeLink, // DT_LNK
    PXFileElementInfoTypeSocket, // DT_SOCK

    PXFileElementInfoTypeDeviceCharacter, // DT_CHR
    PXFileElementInfoTypeDeviceBlock, // DT_BLK

    PXFileElementInfoTypeDictionaryRoot, // '.'
    PXFileElementInfoTypeDictionaryParent // '..'
}
PXFileElementInfoType;


// Permentant data that is and will be stored on disk.
typedef struct PXFileEntry_
{
    PXInt32U ID;

    char* FilePathData;
    PXSize FilePathSize;

    PXSize Size;
    PXInt8U Depth;

    PXFileElementInfoType Type;
}
PXFileEntry;



typedef struct PXFile_
{
    //---<PosisionData>---
    void* Data; // [Do not use directly] Data from where to read/write depending on the used method of linking.
    PXSize DataCursor; // [Do not use directly] Current position of the data.
    PXSize DataCursorBitOffset; // [Do not use directly] Current offset in bits of current byte
    PXSize DataUsed; // [Do not use directly] The total size of the data block.
    PXSize DataAllocated; // [Do not use directly] The size of the data pace in which you can move without triggering an invalid access.
    //--------------------

    PXAccessMode AccessMode;
    PXMemoryCachingMode CachingMode;
    PXFileLocationMode LocationMode;

#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
    FILE* ID;
    int MappingID;
#elif OSWindows
    HANDLE ID;
    HANDLE MappingID;
    //FILE* IDPOSIX;
#endif

    PXBitFormat BitFormatOfData;
    PXEndian EndiannessOfData;

    // The file path can't always be fetched from the OS.
    // for this we store the name here at creation time.
    char* FilePathData;
    PXSize FilePathSize;

    //FILETIME creationTime;
    //FILETIME lastAccessTime;
    //FILETIME lastWriteTime;

    // Statistic
    PXSize CounterOperationsRead;
    PXSize CounterOperationsWrite;
}
PXFile;
//---------------------------------------------------------























typedef struct PXEngineSoundCreateInfo_
{
    PXBool SoundLoop;
}
PXEngineSoundCreateInfo;
//-----------------------------------------------------



//-----------------------------------------------------
// ShaderProgram
//-----------------------------------------------------
typedef struct PXShaderProgramCreateInfo_
{
    char* ShaderVertexText;
    PXSize ShaderVertexTextSize;
    char* ShaderPixelText;
    PXSize ShaderPixelTextSize;

    char* ShaderVertexFilePath;
    char* ShaderPixelFilePath;

    PXFile ShaderVertexFile;
    PXFile ShaderPixelFile;
}
PXShaderProgramCreateInfo;
//-----------------------------------------------------


typedef struct PXEngineFontCreateInfo_
{
    PXShaderProgram* ShaderProgramCurrent;
}
PXEngineFontCreateInfo;

typedef struct PXSkyBoxCreateEventInfo_
{
    PXShaderProgramCreateInfo ShaderProgramCreateInfo;

    char* SkyBoxTextureA;
    char* SkyBoxTextureB;
    char* SkyBoxTextureC;
    char* SkyBoxTextureD;
    char* SkyBoxTextureE;
    char* SkyBoxTextureF;
}
PXSkyBoxCreateEventInfo;

typedef struct PXHitboxCreateInfo_
{
    PXHitBox* HitBox;

    // Mode
    PXInt32U Behaviour;

    PXModel* Model;
}
PXHitboxCreateInfo;


typedef struct PXBrushCreateInfo_
{
    PXColorRGBI8 Color;
}
PXBrushCreateInfo;


typedef struct PXSpriteCreateInfo_
{
    PXTexture2D* TextureCurrent;
    PXShaderProgram* ShaderProgramCurrent;

    PXVector2F TextureScalingPoints[4];

    PXVector3F Position;
    PXVector2F Scaling;

    PXBool ViewRotationIgnore;
    PXBool ViewPositionIgnore;


    // If any vale is set, we will generate a hitbox
    PXInt32U HitboxBehaviour;

}
PXSpriteCreateInfo;



typedef struct PXTimerCreateInfo_
{
    void* Owner;
    void* CallBack;
    PXSize TimeDeltaTarget;
}
PXTimerCreateInfo;




typedef struct PXImageCreateInfo_
{
    // if no filepath exists, use this data
    PXImage Image;
}
PXImageCreateInfo;

typedef struct PXTexture2DCreateInfo_
{
    // if no filepath exists, use this data
    PXImageCreateInfo Image;

}
PXTexture2DCreateInfo;





typedef struct PXTextureCubeCreateInfo_
{
    char* FilePathA;
    char* FilePathB;
    char* FilePathC;
    char* FilePathD;
    char* FilePathE;
    char* FilePathF;
}
PXTextureCubeCreateInfo;



typedef enum PXModelForm_
{
    PXModelFormInvalid,
    PXModelFormCustom,
    PXModelFormTriangle,
    PXModelFormRectangle,
    PXModelFormCircle,
    PXModelFormCube
}
PXModelForm;



typedef struct PXModelCreateInfo_
{
    PXShaderProgram* ShaderProgramReference;
    float Scale;


    // If not loaded by a file, this data shal be used
    PXVertexBuffer VertexBuffer;
    PXIndexBuffer IndexBuffer;

    PXModelForm Form;
}
PXModelCreateInfo;




typedef struct PXTextureActionInfo_
{
    void** TextureReference;
    PXSize Amount;
    PXGraphicTextureType Type;
    PXResourceAction Action;
}
PXGraphicTexturInfo;

// KeyFrame, info about each animated frame
typedef struct PXSpriteAnimatorTimeStamp_
{
    PXTexture2D* Texture;
    float DeltaTime; // The time until we swap to the next screen
}
PXSpriteAnimatorTimeStamp;


#define PXSpriteAnimatorBehaviourAnimationEnable    (1 << 0)
#define PXSpriteAnimatorBehaviourPlayOnce            (1 << 1)
#define PXSpriteAnimatorBehaviourUseUpdateRate        (1 << 2)

// Object to handle a sprite animation by switching the attached texture
typedef struct PXSpriteAnimator_
{
    PXResourceInfo Info;

    PXSprite* SpriteTarget;

    PXInt32U LastUpdate;
    PXInt32U RateUpdate;

    PXSpriteAnimatorTimeStamp* TimeStampList;
    PXSize TimeStampAmount;
    PXSize TimeStampCurrent;
}
PXSpriteAnimator;


// Info to create this 
typedef struct PXSpriteAnimatorInfo_
{
    PXSpriteAnimatorTimeStamp* TimeStampList;
    PXSize TimeStampAmount;
    PXInt32U Behaviour;
    PXSprite* SpriteTarget;
    float UpdateRate;
}
PXSpriteAnimatorInfo;






typedef struct PXResourceCreateInfo_
{
    void** ObjectReference; // Reference to an adress to be filled with an object
    PXSize ObjectAmount; // If set to more than one, "ObjectReference" will contain a list of values

    char* FilePath;
    PXSize FilePathSize;

    char* Name;

    PXResourceType Type;

    PXBool SpawnEnabled;

    union
    {
        PXEngineFontCreateInfo Font;
        PXSkyBoxCreateEventInfo SkyBox;
        PXSpriteCreateInfo Sprite;
        PXSpriteAnimatorInfo SpriteAnimator;
        PXEngineSoundCreateInfo Sound;
        PXShaderProgramCreateInfo ShaderProgram;
        PXTextureCubeCreateInfo TextureCube;
        PXTexture2DCreateInfo Texture2D;
        PXImageCreateInfo Image;
        PXGUIElementCreateInfo UIElement;
        PXModelCreateInfo Model;
        PXHitboxCreateInfo HitBox;
        PXBrushCreateInfo Brush;
        PXTimerCreateInfo Timer;
    };
}
PXResourceCreateInfo;















PXPublic const char* PXAPI PXResourceTypeToString(const PXResourceType pxResourceType);


PXPublic void PXAPI PXResourceManagerInit(PXResourceManager* const pxResourceManager);
PXPublic void PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager);


PXPrivate PXInt32U PXAPI PXResourceManagerGenerateUniqeID(PXResourceManager* const pxResourceManager);











// Generate and store new resource. Load if possible
PXPublic PXActionResult PXAPI PXResourceManagerAdd(PXResourceManager* const pxResourceManager, PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount);



PXPublic PXActionResult PXAPI PXResourceStoreName(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize);
PXPublic PXActionResult PXAPI PXResourceStorePath(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize);
PXPublic PXActionResult PXAPI PXResourceFetchName(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize);
PXPublic PXActionResult PXAPI PXResourceFetchPath(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize);




PXPublic PXActionResult PXAPI PXFileTypeInfoProbe(PXResourceTransphereInfo* const pxFileTypeInfo, const PXText* const pxText);

PXPublic PXActionResult PXAPI PXResourceManagerReferenceValidate(PXResourceManager* const pxResourceManager, PXResourceReference* const pxResourceReference);

PXPublic PXActionResult PXAPI PXResourceLoad(PXResourceTransphereInfo* const pxResourceLoadInfo, const PXText* const filePath);
PXPublic PXActionResult PXAPI PXResourceLoadA(PXResourceTransphereInfo* const pxResourceLoadInfo, const char* const filePath);

PXPublic PXActionResult PXAPI PXResourceSave(PXResourceTransphereInfo* const pxResourceSaveInfo, const PXText* const filePath);
PXPublic PXActionResult PXAPI PXResourceSaveA(PXResourceTransphereInfo* const pxResourceSaveInfo, const char* const filePath);



PXPublic PXActionResult PXAPI PXResourceSerialize(PXResource* const pxResource, PXFile* const pxFile);
PXPublic PXActionResult PXAPI PXResourceParse(PXResource* const pxResource, PXFile* const pxFile);



PXPublic PXMaterial* PXAPI PXMaterialContainerFind(PXResourceManager* const pxResourceManager, const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName);




#endif
