#ifndef PXResourceINCLUDE
#define PXResourceINCLUDE

#include "PXImage.h"

#include <PX/Container/ListDynamic/PXListDynamic.h>
#include <PX/Container/ListHierarchical/PXListHierarchical.h>
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Hardware/PXKeyBoard.h>
#include <PX/Math/PXMatrix.h>
#include <PX/OS/Time/PXTime.h>
#include <PX/Container/Buffer/PXBuffer.h>

#include <stdarg.h>

#if OSUnix
#include <stdio.h>
#include <X11/X.h> // X11 XID, ulong 8 Byte
#include <X11/Xlib.h> // XDisplay
#endif

//#include <PX/OS/File/PXFile.h>

#define PXBoilerPlate

// Predefine
typedef enum PXActionResult_ PXActionResult;

typedef struct PXBuffer_ PXBuffer;
typedef struct PXModel_ PXModel;
typedef struct PXImage_ PXImage;
typedef struct PXCodeDocumentElement_ PXCodeDocumentElement;
typedef struct PXFile_ PXFile;
typedef struct PXTime_ PXTime;
typedef struct PXText_ PXText;
typedef struct PXCodeDocument_ PXCodeDocument;
typedef struct PXGUISystem_ PXGUISystem;
typedef struct PXWindow_ PXWindow;
typedef struct PXFileTypeInfo_ PXFileTypeInfo;
typedef struct PXCompiler_ PXCompiler;
typedef struct PXProcessor_ PXProcessor;
typedef struct PXHierarchicalNode_ PXHierarchicalNode;
typedef struct PXFileEntry_ PXFileEntry;
typedef struct PXDebug_ PXDebug;
typedef struct PXDisplay_ PXDisplay;
typedef struct PXNativDraw_ PXNativDraw;
typedef struct PXTask_ PXTask;
typedef struct PXThread_ PXThread;
typedef struct PXThreadPool_ PXThreadPool;

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





// Cool assert for debug



#define PXAssert(condition, message) \
if(!(condition)) \
{ \
    const char* buffer = _PX_FILENAME_; \
    PXLogPrint(PXLoggingError, buffer, "Assert", message); \
    DebugBreak(); \
}













//---------------------------------------------------------
// Resource types and structs
//---------------------------------------------------------

// Describes the exact type of a resource
#define PXResourceTypeInvalid 0 // Resource invalid, don't use.

#define    PXResourceTypeCustom   1 // Undetected but valid format. Needs to be handled by the caller

    //-----------------------------------------------------
    // Resource Level A - Indepepended
    //-----------------------------------------------------
#define PXResourceTypeImage       2       // Image for pixeldata
#define PXResourceTypeSound       3      
#define PXResourceTypeVideo       4      
#define PXResourceTypeModel       5       // 3D model, collection of vertex data
#define PXResourceTypeFont        6       // Collection of spites or points to render text
#define PXResourceTypeMaterial    7      
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Resource Level B - Context spesific/Internal OS Resources
    //-----------------------------------------------------
#define PXResourceTypeBrush         8
#define PXResourceTypeTexture2D      9    // Texture to render on a surface
#define PXResourceTypeTextureCube    10   
#define PXResourceTypeShaderProgram  11  
#define PXResourceTypeSkybox         12   
#define PXResourceTypeSprite         13 
#define PXResourceTypeIcon            14
#define PXResourceTypeIconAtlas       15 
#define PXResourceTypeSpriteAnimator  16 
#define PXResourceTypeText            17
#define PXResourceTypeTimer           18
#define PXResourceTypeEngineSound     19
#define PXResourceTypeGUIElement      20
#define PXResourceTypeHitBox           21
#define PXResourceTypeMaterialList    22
#define PXResourceTypeCodeDocument    23
#define PXResourceTypeDocument         24
#define PXResourceTypeBinary          25
#define PXResourceTypeStructuredText   26
#define PXResourceTypeInstaller        27  // compressed executable
#define PXResourceTypeArchiv           28  // Compressed object
//-----------------------------------------------------

// Extended basic components

#define PXResourceTypeDialogBox     29
#define PXResourceTypeSpriteMap     30
//-----------------------------------------------------



#define ResourceIDNameLength 64

#define PXResourceInfoIdentityMask    0b0000000000001111
#define PXResourceInfoExist           0b0000000000000001 // Indicate if resource is valid
#define PXResourceInfoActive          0b0000000000000010 // Is it interactable or does it tick?
#define PXResourceInfoRender          0b0000000000000100 // Shall it be rendered?
#define PXResourceInfoSelected        0b0000000000001000

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

#define PXResourceInfoPermissionMask    0b1110000000000000
#define PXResourceInfoPermissionREAD    0b1000000000000000
#define PXResourceInfoPermissionWRITE   0b0100000000000000
#define PXResourceInfoPermissionEXECUTE 0b0010000000000000


#define PXResourceInfoConstData PXResourceInfoPermissionREAD | PXResourceInfoStorageMemory | PXResourceInfoExist | PXResourceInfoActive | PXResourceInfoRender

#define PXResourceInfoOK              PXResourceInfoExist | PXResourceInfoActive | PXResourceInfoRender
#define PXResourceInfoNoRender              PXResourceInfoExist | PXResourceInfoActive



// IDs used by rendering APIs to keep track of the object reference.
// OpenGL uses 32-Bit Integer as an ID.
// DirectX uses direct pointers to object references.
typedef union PXOSHandle_
{
    PXInt32U OpenGLID; // Simple ID for an object.
    void* DirectXInterface; // DirectX uses interfaces to communicate to a element.
#if OSUnix
    Window WindowID; // Linux X11 System
    XFontStruct* FontHandle;
    pthread_t ThreadHandle;
#elif OSWindows
    HWND WindowID; // Windows only, used for GUI elements
    HBRUSH BrushHandle;
    HFONT FontHandle;
    HMENU MenuHandle;
    HICON IconHandle;
    HANDLE ThreadHandle;
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
    //PXInt32U Flags; // general information
    PXInt32U Behaviour; // Depends on the type of the resource

    PXInt32U Setting;
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




//---------------------------------------------------------
// Module - Library or seperate Element that manages
//---------------------------------------------------------

#define PXModuleStateDoInitialize   1<<0
#define PXModuleStateDoRelease      1<<1

typedef PXActionResult(PXAPI* PXModuleStateFunction)(void** moduleAdress, const PXInt32U flags); // Initialize or Releases a module.

typedef struct PXModule_
{
    void* ModuleAdress;
    PXModuleStateFunction StateChange;
}
PXModule;
//---------------------------------------------------------











#include <PX/OS/Async/PXLock.h>
#include <PX/Container/Buffer/PXBuffer.h>









typedef struct PXResource_
{
    PXInt32U Type;
    void* ResourceAdress;
}
PXResource;



#define PXResourceManagerInit (1<<0)

// Container to manage resources by loading or saving
typedef struct PXResourceManager_
{
    PXLock CreateLock;

    PXListDynamic NameCache;
    PXListDynamic SourcePathCache;

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
    PXDictionary IconLookUp;
    PXDictionary IconAtlasLookUp;
    PXDictionary SpriteMapAtlasLookUp;

    struct PXShaderProgram_* ShaderFailback;
    struct PXModel_* ModelFailback;
    struct PXTexture2D_* Texture2DFailBack;

    PXInt32U Flags;
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
    PXResourceInfo Info;

    struct PXTexture2D_* IconTexture2D;

    PXSize IconListAmount;
    PXIcon* IconList;
}
PXIconAtlas;

typedef struct PXIconAtlasCreateInfo_
{
    PXInt16U CellSize;
    PXInt16U CellAmountX; // Will be generated
    PXInt16U CellAmountY; // Will be generated
    PXInt16U CellAmountTotal; // Will be generated
}
PXIconAtlasCreateInfo;


PXPublic void PXAPI PXIconLoad(PXIcon* const pxIcon);

//---------------------------------------------------------








//---------------------------------------------------------
// GUI-Element 
//---------------------------------------------------------
typedef struct PXRectangleXYWHI32_
{
    int X;
    int Y;
    int Width;
    int Height;
}
PXRectangleXYWHI32;

typedef struct PXRectangleLTRBI32_
{
    int Left;
    int Top;
    int Right;
    int Bottom;
}
PXRectangleLTRBI32;

// PXPadding;: Offset the space as the child container (Make object take more space)
// PXMargin;  Offset the whole child-Container (Make empty space)
typedef struct PXRectangleLTRBF32_
{
    PXF32 Left;
    PXF32 Top;
    PXF32 Right;
    PXF32 Bottom;
}
PXRectangleLTRBF32;

PXPublic void PXAPI PXRectangleLTRBI32ToXYWHI32(const PXRectangleLTRBI32* const pxRectangleLTRBI32, PXRectangleXYWHI32* const pxRectangleXYWHI32);
PXPublic void PXAPI PXRectangleXYWHI32ToLTRBI32(const PXRectangleXYWHI32* const pxRectangleXYWHI32, PXRectangleLTRBI32* const pxRectangleLTRBI32);
//---------------------------------------------------------





typedef struct PXTexture1D_
{
    PXResourceInfo Info;

    PXGraphicRenderFilter Filter;
    PXGraphicImageLayout LayoutNear;
    PXGraphicImageLayout LayoutFar;
    PXGraphicImageWrap WrapHeight;
    PXGraphicImageWrap WrapWidth;

    PXImage* Image;
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

    PXImage* Image;
}
PXTexture2D;

typedef struct PXTexture3D_
{
    PXResourceInfo Info;

    PXImage* Image;
}
PXTexture3D;

// A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
typedef struct PXTextureCube_
{
    PXResourceInfo Info;

    PXColorFormat Format;

    PXImage* ImageA;
    PXImage* ImageB;
    PXImage* ImageC;
    PXImage* ImageD;
    PXImage* ImageE;
    PXImage* ImageF;
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


PXPublic PXActionResult PXAPI PXTextureLoadData(PXTexture* const pxTexture);


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

    PXF32 Diffuse[4];
    PXF32 Ambient[4];
    PXF32 Specular[4];    // shininess
    PXF32 Emission[4];
    PXF32 Power;        // Sharpness if specular highlight

    PXF32 Weight;         // Ranges between 0 and 1000
    PXF32 Dissolved;
    PXF32 Density; // range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

    PXTexture2D* DiffuseTexture;

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





//---------------------------------------------------------
// Shader
//---------------------------------------------------------
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

    PXShaderVariableTypePXF32Single,
    PXShaderVariableTypePXF32Vector2,
    PXShaderVariableTypePXF32Vector3,
    PXShaderVariableTypePXF32Vector4,
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

#define PXShaderVariableBehaviourFailedFetch (1<<0) // A resolusion to the shader failed

typedef struct PXShaderVariable_
{
    PXResourceInfo Info;

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
//---------------------------------------------------------







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








/*
// OLD SOLUTION!


#define PXVertexBufferFormatFormat          0
#define PXVertexBufferFormatPosition        1
#define PXVertexBufferFormatTexturePosition 2
#define PXVertexBufferFormatNormal          3
#define PXVertexBufferFormatColor           4

#define PXVertexBufferFormatI08        0
#define PXVertexBufferFormatI16        1
#define PXVertexBufferFormatI32        2
#define PXVertexBufferFormatF16        3
#define PXVertexBufferFormatF32        4

#define PXVertexBufferFormatMake(format, position, texturePosition, normal, color) \
format << (4 * PXVertexBufferFormatFormat) | \
position << (4 * PXVertexBufferFormatPosition) | \
texturePosition << (4 * PXVertexBufferFormatTexturePosition) | \
normal << (4 * PXVertexBufferFormatNormal) | \
color << (4 * PXVertexBufferFormatColor)

#define PXVertexBufferFormatExtract(target, value) (value >> (4*target)) & 0x0F


#define PXVertexBufferFormatP3F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 3, 0, 0, 0)
#define PXVertexBufferFormatP3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 0, 0, 0)

#define PXVertexBufferFormatN3F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 0, 0, 3, 0)
#define PXVertexBufferFormatN3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 0, 0, 3, 0)

#define PXVertexBufferFormatT2F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 0, 2, 0, 0)
#define PXVertexBufferFormatT2F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 0, 2, 0, 0)


#define PXVertexBufferFormatT2P3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 0, 0)
#define PXVertexBufferFormatT2N3P3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 3, 0)

typedef PXInt32U PXVertexBufferFormat;
*/



//---------------------------------------------------------
// Vertex
//---------------------------------------------------------




#if 0

typedef enum PXVertexBufferFormat_
{
    PXVertexBufferFormatInvalid,

    // Non inteleaved
    PXVertexBufferFormatP2I8,   // XY - Int8 - Position 
    PXVertexBufferFormatP3I8,   // XYX - Int8 - Position 

    PXVertexBufferFormatP3F16,    // XYZ - PXF32 - Position
    PXVertexBufferFormatN3F16,    // XYZ - PXF32 - Normals 
    PXVertexBufferFormatT2F16,    // UV - PXF32 - Texture coordiantes

    PXVertexBufferFormatP3F32,    // XYZ - PXF32 - Position
    PXVertexBufferFormatN3F32,    // XYZ - PXF32 - Normals 
    PXVertexBufferFormatT2F32,    // UV - PXF32 - Texture coordiantes


    // OpenGL

    PXVertexBufferFormatP2F32, // GL_V2F
    PXVertexBufferFormatC4UBP2F32,
    PXVertexBufferFormatC4UBP3F32,
    PXVertexBufferFormatC3P3F32,
    PXVertexBufferFormatN3P3F32, // NNN PPP
    PXVertexBufferFormatC4N3P3F32,
    PXVertexBufferFormatT2P3F32, // TT PPP
    PXVertexBufferFormatT4P4,
    PXVertexBufferFormatT2C4UBP3,
    PXVertexBufferFormatT2C3P3,
    PXVertexBufferFormatT2N3P3, // TT NNN PPP
    PXVertexBufferFormatT2C4N3P3,
    PXVertexBufferFormatT4C4N3P4,

    // Direct X


    PXVertexBufferFormatXYZC, // Normal spcace (x, y, z, color-RGB as 32-int)

    PXVertexBufferFormatXYZRHW, // DirectX only, use for pixel size instead of normal space.

    PXVertexBufferFormatXYZHWC, // X, Y, Z, Tx, Ty, color-RGB as 32-int

    PXVertexBufferFormatP3B1,
    PXVertexBufferFormatP3B2,
    PXVertexBufferFormatP3B3,
    PXVertexBufferFormatP3B4,
    PXVertexBufferFormatP3B5,
    PXVertexBufferFormatP4,
}
PXVertexBufferFormat;
#endif


typedef struct PXVertexBufferFormatInfo_
{
    PXInt8U VertexAttributesAmount;

    PXInt8U Format;
    PXInt8U Position;
    PXInt8U TexturePosition;
    PXInt8U Normal;
    PXInt8U Color;

    PXInt8U Stride; // Byte per vertex

    // Bytes per data.
    // Can only be 1, 2, 3, 4. Nothing else!
    // char = 1
    // short = 2
    // float = 4
    PXInt8U VertexElementSize;

    PXInt8U VertexTotalSize; // XYZ = 3

    char AsText[32];
}
PXVertexBufferFormatInfo;


//PXPublic void PXAPI PXVertexBufferFormatInfoExtract(PXVertexBufferFormatInfo* const pxVertexBufferFormatInfo, const PXVertexBufferFormat pxVertexBufferFormat);




#define PXVertexBufferLayoutTypeInvalid     0
#define PXVertexBufferLayoutTypePosition    1
#define PXVertexBufferLayoutTypeNormal      2
#define PXVertexBufferLayoutTypeTexturePos  3
#define PXVertexBufferLayoutTypeColor       4
#define PXVertexBufferLayoutTypeCustom      5

// Describes how the data is stuctured
typedef struct PXBufferLayout_
{
    PXType Format;
    PXInt8U AmountOfElements; // 2=2D, 3=3D, 4=RGBA, ...
    PXInt8U Type;
    PXBool UpdatePerPrimitive; // 1=Instancing
}
PXBufferLayout;





// Buffer to contain veretx data like position, normals, texturepositions, ...
// A mesh can contain multible vertexBuffers or just one thats interleaved
typedef struct PXVertexBuffer_
{
    PXResourceInfo Info; // VertexArrayObject (VAO)

    struct PXBuffer_ VertexData;

    PXSize LayoutAmount;

    union
    {
        PXBufferLayout LayoutPrime[4]; // Store up zo 4, saving allocations
        PXBufferLayout* LayoutList;
    };
}
PXVertexBuffer;
//---------------------------------------------------------


PXPublic PXBufferLayout* PXAPI PXVertexBufferLayoutGET(PXVertexBuffer* const pxVertexBuffer);



/*
PXPublic void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferFormat XVertexBufferFormat, const PXSize index);
*/



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

    PXBuffer Data;

    PXType DataType; // Data type for buffer

    PXInt32U DrawModeID; // How to draw, modes like triangle or lines

    PXSize LayoutListAmount;
    PXSize SegmentListAmount; // 0=Not allowed. 1=Mesh is single, n=Segmented

    union
    {
        PXIndexSegment SegmentPrime; // Only used if Segment is only one
        PXIndexSegment* SegmentList;
    };

    union
    {
        PXBufferLayout LayoutPrime;
        PXBufferLayout* LayoutList;
    };
}
PXIndexBuffer;


// Allocate an indexbuffer in a way to minimize size for given amount of vertex points
PXPublic void PXAPI PXIndexBufferPrepare(PXIndexBuffer* const pxIndexBuffer, const PXSize amountVertex, const PXSize amountMaterials);
PXPublic PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer* const pxIndexBuffer);

PXPublic PXIndexSegment* PXAPI PXIndexBufferSegmentListGET(const PXIndexBuffer* const pxIndexBuffer);
PXPublic PXBufferLayout* PXAPI PXIndexLayoutListGET(const PXIndexBuffer* const pxIndexBuffer);

PXPublic PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer* const pxIndexBuffer, const PXInt8U type);


//---------------------------------------------------------
// Mesh
//---------------------------------------------------------

// A mesh is a structure that contains vertex and index data to render itself
typedef struct PXMesh_
{
    PXResourceInfo Info; // Contains VAO

    // Primary allocatedspace
    //PXSize VertexDataSize;
    //void* VertexDataAdress;

    // Can either store interleaved data or seperate ones
    PXSize VertexBufferListAmount; // Realistically, is not more than 1,2,3,4,..

    union
    {
        PXVertexBuffer VertexBufferPrime[4]; // Can store position, normal, texturepos and one additional parameter 
        PXVertexBuffer* VertexBufferList; // Used as the same above, but can store many more.
    }; 

    PXIndexBuffer IndexBuffer; // Contains IBO


    PXSize MaterialContaierListAmount;
    PXMaterialContainer* MaterialContaierList;
}
PXMesh;

PXPublic PXActionResult PXAPI PXMeshVertexLayoutPrint(PXMesh* const pxMesh);

// Define and allocate vertex data to be stored and how
PXPublic PXActionResult PXAPI PXMeshVertexLayout(PXMesh* const pxMesh, const PXSize index, PXBufferLayout* const pxVertexBufferLayoutList, const PXSize amount);
PXPublic PXActionResult PXAPI PXMeshIndexLayout(PXMesh* const pxMesh, const PXSize primitveAmount, const PXSize segmentAmount);

PXPublic PXBufferLayout* PXAPI PXMeshVertexBufferGET(PXMesh* const pxMesh, const PXInt8U type);

PXPublic void* PXAPI PXMeshVertexInsert(PXMesh* const pxMesh, const PXInt8U type);
PXPublic void* PXAPI PXMeshIndexInsert(PXMesh* const pxMesh, const PXInt8U type);

PXPublic PXSize PXAPI PXMeshVertexStrideGET(PXMesh* const pxMesh);

PXPublic PXVertexBuffer* PXAPI PXMeshVertexBufferListGET(PXMesh* const pxMesh);
PXPublic PXVertexBuffer* PXAPI PXMeshVertexBufferListSET(PXMesh* const pxMesh, const PXSize amount);

PXPublic PXSize PXAPI PXMeshIndexBufferLengthGET(PXMesh* const pxMesh);

PXPublic PXActionResult PXAPI PXMeshVertexLayoutTransmute(PXMesh* const pxMesh);
PXPublic PXActionResult PXAPI PXMeshNormalDataGenerate(PXMesh* const pxMesh);
PXPublic PXActionResult PXAPI PXMeshVertexArrayAdd
(    
    PXMesh* const pxMesh,
    void* data, 
    const PXSize dataLength,
    PXBufferLayout* const pxVertexBufferLayoutList,
    const PXSize pxVertexBufferLayoutListAmount
);
//---------------------------------------------------------


PXPublic PXSize PXAPI PXMeshTriangleAmount(PXMesh* const pxMesh);
PXPublic PXF32* PXAPI PXMeshTriangleIndex(PXMesh* const pxMesh, const PXSize index);
//PXPublic void* PXAPI PXMeshVertexDataInsertionPoint(PXMesh* const pxMesh, const PXVertexBufferDataType pxVertexBufferDataType);

// A Model is a renderable entity.
// Has a mesh as a base structure
// Matrix for where it is
// Render information
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

   // PXModel* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
    //PXModel* StructureParent; // Structural parent of structure
    //XModel* StructureSibling; // Stuctual sibling, works like a linked list.
    //PXModel* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
    //-----------------------------

    //-----------------------------
    // Settings
    //-----------------------------
    PXBool IgnoreViewPosition; // Removes positiondata from the view matrix
    PXBool IgnoreViewRotation; // remove rotationdata from the view matrix
    PXBool RenderBothSides;
    //  Margin ??
    //-----------------------------
    struct PXHitBox_* HitBox;

    PXF32 Range;
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
    PXInt32U Type;

    // Dirty flags
}
PXRenderEntity;





PXPublic void PXAPI PXModelConstruct(PXModel* const pxModel);



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
    PXF32 ClippingMinimum;
    PXF32 ClippingMaximum;
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

    PXF32 Diffuse[4];           // Diffuse color of light
    PXF32 Specular[4];          // Specular color of light
    PXF32 Ambient[4];           // Ambient color of light
    PXF32 Position[3];          // Position in world space
    PXF32 Direction[3];         // Direction in world space
    PXF32 CutoffRange;          // Cutoff range
    PXF32 Falloff;              // Falloff
    PXF32 AttenuationConstant;  // Constant attenuation
    PXF32 AttenuationLinear;    // Linear attenuation
    PXF32 AttenuationQuadratic; // Quadratic attenuation
    PXF32 Theta;                // Inner angle of spotlight cone
    PXF32 Phi;                  // Outer angle of spotlight cone

    PXLightType Type;           // Type of light source

    PXBool Enabled;
}
PXLight;




typedef struct PXFontPageCharacter_
{
    PXInt32U ID;
    PXF32 Position[2]; // Position of the character image in the texture.
    PXF32 Size[2];    // Size of the character image in the texture.
    PXF32 Offset[2];// Offset from the position-center.
    int XAdvance; // How much the current position should be advanced after drawing the character.
}
PXFontPageCharacter;

typedef struct PXFontPage_
{
    PXSize CharacteListEntrys;
    PXFontPageCharacter* CharacteList;

    struct PXTexture2D_* Texture;
}
PXFontPage;

PXPublic PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID);





#define PXFontAntialiased   (1<<0)
#define PXFontItalic        (1<<1)
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

    PXInt32S Height;
    PXInt32S Width;
    PXInt32S Escapement;
    PXInt32S Orientation;
    PXInt32S Weight;

    PXInt8U CharSet;
    PXInt8U OutPrecision;
    PXInt8U ClipPrecision;
    PXInt8U Quality;
    PXInt8U PitchAndFamily;

    char Name[32]; // ToDo: Remove this
}
PXFont;

PXPublic PXFontPage* PXAPI PXFontPageGet(PXFont* const pxFont, const PXSize index);











//-----------------------------------------------------
// Text
//-----------------------------------------------------
typedef struct PXEngineText_
{
    PXResourceInfo Info;

    PXVector2F32 Position;
    PXVector2F32 Scaling;

    PXBool DoubleRenderForShadow;

    PXText* Text;
    PXFont* Font;
    PXF32 FontScaling;

    PXInt32U TextRenderAmount;
}
PXEngineText;
//-----------------------------------------------------





//-----------------------------------------------------
// Audio
//-----------------------------------------------------






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

#define PXHitBoxBehaviourWallStatic PXHitBoxBehaviourKeepOut | PXHitBoxBehaviourKeepIn
//---------------------------------------------------------

typedef void(PXAPI* PXHitBoxCollisionDetect)(void* owner, struct PXHitBox_* const pxHitBox);

// Collidable entity that can be defined for different behaviours
typedef struct PXHitBox_ PXHitBox;

typedef struct PXHitBox_
{
    PXResourceInfo Info;

    PXModel* Model;

    PXHitBoxForm Form;

    PXHitBox* ColliderChild;
    PXHitBox* ColliderParent;

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
    PXF32 X;
    PXF32 Y;
    PXF32 Width;
    PXF32 Height;
}
PXRectangleF;



typedef struct PXSprite_
{
    PXResourceInfo Info; // PX-Engine register info

    PXVector2F32 TextureScalePositionOffset;
    PXVector2F32 TextureScalePointOffset;

    PXModel* Model;
    PXTexture2D* Texture;
    PXMaterial* Material; // Use this instand of a texture, right?
    PXHitBox* HitBox;
    PXShaderProgram* ShaderProgarm;
}
PXSprite;




typedef struct PXSpriteMapEntity_
{
    PXVector2F32 Position; // XY in the world, will be alligned to world space
    PXVector2F32 SizeInCells;
    PXVector2F32 Offset; // Slight offset, used for animation or general control.
}
PXSpriteMapEntity;


typedef struct PXSpriteMap_
{
    PXResourceInfo Info;

    PXShaderProgram* Shader;
    PXModel* Model;
    PXTexture2D* Texture2D;

    PXSpriteMapEntity* SpriteMapEntityList;
}
PXSpriteMap;









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

    PXVector3F32 LookAtPosition;
    PXVector3F32 CurrentRotation;

    //---<Follow>---
    PXVector3F32 Offset;
    PXVector3F32 DeadZone;
    //PXInt8U TargetFollowFlag;
    PXMatrix4x4F* Target;
    PXF32 FollowSpeed; // Ranges from 0 to 1 .. FollowSpeed; = 0.98f

    PXCameraPerspective Perspective;

    PXF32 FieldOfView;
    PXInt32S Height;
    PXInt32S Width;
    PXF32 Near;
    PXF32 Far;

    PXF32 WalkSpeed;
    PXF32 ViewSpeed;

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



#if 1

//-----------------------------------------------------
// UI-Element
//-----------------------------------------------------

// Ancering will stick the given edge to a side.
// Offset will be interpretet 0=NoSpace, 1=???
// Goal: Scale the object with screensize
#define PXWindowAncerParent           0b00000000000000000000000000001111
#define PXWindowAncerParentLeft       0b00000000000000000000000000000001
#define PXWindowAncerParentTop        0b00000000000000000000000000000010
#define PXWindowAncerParentRight      0b00000000000000000000000000000100
#define PXWindowAncerParentBottom     0b00000000000000000000000000001000

// Let siblings calulate their offset themself.
// Goal: Group multible objects together that belong together
#define PXWindowAncerSibling          0b00000000000000000000000011110000
#define PXWindowAncerSiblingLeft      0b00000000000000000000000000010000
#define PXWindowAncerSiblingTop       0b00000000000000000000000000100000
#define PXWindowAncerSiblingRight     0b00000000000000000000000001000000
#define PXWindowAncerSiblingBottom    0b00000000000000000000000010000000

#define PXWindowKeepFlags             0b00000000000000000000111100000000
#define PXWindowKeepPositionX         0b00000000000000000000000100000000
#define PXWindowKeepPositionY         0b00000000000000000000001000000000
#define PXWindowKeepWidth             0b00000000000000000000010000000000
#define PXWindowKeepHeight            0b00000000000000000000100000000000

// Allign content inside a element
#define PXWindowAllignFlags                 0b00000000000000011111000000000000
#define PXWindowAllignLeft                  0b00000000000000000001000000000000
#define PXWindowAllignTop                   0b00000000000000000010000000000000
#define PXWindowAllignRight                 0b00000000000000000100000000000000
#define PXWindowAllignBottom                0b00000000000000001000000000000000
#define PXWindowAllignCenter                0b00000000000000010000000000000000

#define PXWindowAllignIgnoreParentMargin    0b00000000100000000000000000000000

// Interaction
#define PXWindowBehaviourInteractMask       0b11111111000000000000000000000000
#define PXWindowBehaviourHoverable          0b00000001000000000000000000000000
#define PXWindowBehaviourIsBeingHovered     0b00000010000000000000000000000000
#define PXWindowBehaviourSelectable         0b00000100000000000000000000000000
#define PXWindowBehaviourIsBeeingSelected   0b00001000000000000000000000000000
#define PXWindowBehaviourBorder             0b00010000000000000000000000000000
#define PXWindowBehaviourScrollBarHor       0b00100000000000000000000000000000
#define PXWindowBehaviourScrollBarVer       0b01000000000000000000000000000000

#define PXWindowBehaviourDefaultKeepAspect   PXWindowKeepWidth | PXWindowKeepHeight
#define PXWindowBehaviourDefaultInputNormal  PXWindowBehaviourSelectable | PXWindowBehaviourHoverable
#define PXWindowBehaviourDefaultText         PXWindowKeepHeight | PXWindowAllignCenter
#define PXWindowBehaviourDefaultBuffer       PXWindowBehaviourDefaultKeepAspect

#endif


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
    PXUIElementTypeTabPage,
    PXUIElementTypeToggle,
    PXUIElementTypeCheckBox,
    PXUIElementTypeComboBox,
    PXUIElementTypeColorPicker,
    PXUIElementTypeSlider,
    PXUIElementTypeImageList,
    PXUIElementTypeRenderFrame,

    PXUIElementTypeMenuStrip, // Topline of a window that contains a selectable list of things

    PXUIElementTypeScene,
    PXUIElementTypeCode,
    PXUIElementTypeHexEditor,
    PXUIElementTypeGraphBehaviour,
    PXUIElementTypeGraphTime,
    PXUIElementTypeSoundPlayerMixer,
    PXUIElementTypeVideoCutter,
    PXUIElementTypeDataBaseManager,
    PXUIElementTypeNetworkTester,
    PXUIElementTypeInputView,
    PXUIElementTypeHardwareInfo,

    PXUIElementTypeFileManager,    // TreeView of filesystem with utility
    PXUIElementTypeResourceManger, // TreeView of a list of resource entrys
    PXUIElementTypeResourceEntry,  // Panel with name and icons of type and propertys 
    PXUIElementTypeResourceInfo    // Resource info
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
    PXF32 Scale;
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
    PXF32 Percentage;
    PXColorRGBI8 BarColor;
}
PXUIElementProgressBarInfo;

typedef struct PXUIElementComboBoxInfo_
{
    char** DataList;
    PXSize DataListAmount;
}
PXUIElementComboBoxInfo;



typedef struct PXUIElementPosition_
{
    PXRectangleLTRBF32 Margin;
    PXRectangleLTRBF32 Padding;
    PXRectangleXYWHI32 Form;

   // PXF32 Left;
   // PXF32 Top;
   // PXF32 Right;
   // PXF32 Bottom;

    // PXInt16U FlagListFormat; // Unused
//   PXInt8U FlagListAncer;
    // PXInt8U FlagListKeep;
}
PXUIElementPosition;


typedef void (PXAPI* PXWindowEventFunction)(void* const owner, struct PXWindowEvent_* const pxWindowEvent);





typedef struct PXWindowDrawInfo_
{
    PXRectangleXYWHI32 RectangleXYWH;

#if OSUnix
    int ScreenIDHandle;
    Display* DisplayHandle;
    Drawable WindowIDHandle;
    GC GraphicContntainerHandle;
#elif OSWindows
    HWND hwnd;
    HDC hDC;
    BOOL bErase;
#endif

    char* Content;
    PXSize ContentSize;
}
PXWindowDrawInfo;

typedef PXActionResult (PXAPI* PXWindowDrawFunction)(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);


//---------------------------------------------------------
// Brush
//---------------------------------------------------------

// if true, color is a struct, otherwise a pointer to that struct
#define PXWindowBrushBehaviourColorEmbeded  (1<<0)

// Color of GUI Element and tools to apply
typedef struct PXWindowBrush_
{
    PXResourceInfo Info;

    union
    {
        PXColorRGBI8 ColorDate;
        PXColorRGBI8* ColorReference;
    };
}
PXWindowBrush;

PXPublic void PXAPI PXWindowBrushColorSet(PXWindowBrush* const pxGUIElementBrush, const PXByte red, const PXByte green, const PXByte blue);

//---------------------------------------------------------

/*
typedef struct PXWindowData_
{
    union
    {
        PXDirectorySearchCache DirectorySearchCache; // Used for a treeview
    };
}
PXWindowData;
*/


















//---------------------------------------------------------
// UI Elements - Window 
//---------------------------------------------------------






#define UseRawMouseData 1
#define PXWindowTitleSizeMax 256
#define PXWindowSizeDefault -1

// The mode in which the mouse pointer will be.
typedef enum PXWindowCursorMode_
{
    PXWindowCursorIgnore,     // Show the cursor but dont register any Input.
    PXWindowCursorShow,     // Show the cursor and register Input as normal.
    PXWindowCursorInvisible,     // Hide Cursor. You can still use it as normal. Not locked.
    PXWindowCursorLock,
    PXWindowCursorLockAndHide
}
PXWindowCursorMode;

typedef enum PXMouseButton_
{
    PXMouseButtonInvalid,
    PXMouseButtonLeft,
    PXMouseButtonMiddle,
    PXMouseButtonRight,
    PXMouseButtonScrollUp,
    PXMouseButtonRightDown,
    PXMouseButtonSpecialA,
    PXMouseButtonSpecialB,
    PXMouseButtonSpecialC,
    PXMouseButtonSpecialD,
    PXMouseButtonSpecialE,
}
PXMouseButton;

typedef enum PXMouseScrollDirection_
{
    PXMouseScrollDirectionInvalid,
    PXMouseScrollDirectionUp,
    PXMouseScrollDirectionDown,
    PXMouseScrollDirectionLeft,
    PXMouseScrollDirectionRight
}
PXMouseScrollDirection;

typedef enum PXCursorIcon_
{
    PXCursorIconInvalid,
    PXCursorIconNormal,
    PXCursorIconIBeam,
    PXCursorIconWait,
    PXCursorIconCross,
    PXCursorIconUp,
    PXCursorIconHand,
    PXCursorIconNotAllowed,
    PXCursorIconAppStarting,
    PXCursorIconResizeHorizontal,
    PXCursorIconResizeVertical,
    PXCursorIconResizeClockwise,
    PXCursorIconResizeClockwiseCounter,
    PXCursorIconResizeAll,
}
PXCursorIcon;

typedef enum PXWindowEventType_
{
    PXWindowEventTypeInvalid,

    PXWindowEventTypeElementMove,
    PXWindowEventTypeElementResize,
    PXWindowEventTypeElementDestroy,
    PXWindowEventTypeElementClose,
    PXWindowEventTypeElementCreate,
    PXWindowEventTypeElementSelect,
    PXWindowEventTypeElementClick,
    PXWindowEventTypeElementFocusEnter,
    PXWindowEventTypeElementFocusLeave,

    PXWindowEventTypeInputMouseButton,
    PXWindowEventTypeInputMouseMove,
    PXWindowEventTypeInputKeyboard,


    WindowEventActivate,
    WindowEventRedrawSet,
    WindowEventFocusSet,
    WindowEventFocusKill,
    WindowEventEnable,
    WindowEventTextSet,
    WindowEventTextGet,
    WindowEventTextGetLength,
    WindowEventPaint,
    WindowEventClose,
    WindowEventSessionQuerryEnd,
    WindowEventSessionEnd,
    WindowEventQuerryOpen,
    WindowEventQuit,
    WindowEventBackgroundErase,
    WindowEventSystemColorChange,
    WindowEventShowWindow,
    WindowEventIconChange,
    WindowEventSettingChange,
    WindowEventDeviceModeChange,
    WindowEventActivateApp,
    WindowEventFontChange,
    WindowEventTimeChange,
    WindowEventCancelMode,
    WindowEventCursorSet,
    WindowEventMouseActivate,
    WindowEventChildActivate,
    WindowEventQueueSync,
    WindowEventSizeChange,
    WindowEventIconPaint,
    WindowEventIconBackgroundErase,
    WindowEventDialogControlNext,
    WindowEventSPOOLERSTATUS,
    WindowEventItemDraw,
    WindowEventItemMeasure,
    WindowEventItemDelete,
    WindowEventVKEYTOITEM,
    WindowEventCHARTOITEM,
    WindowEventFontSet,
    WindowEventFontGet,
    WindowEventSETHOTKEY,
    WindowEventGETHOTKEY,
    WindowEventQUERYDRAGICON,
    WindowEventCOMPAREITEM,
    WindowEventGETOBJECT,
    WindowEventCOMPACTING,
    WindowEventCOMMNOTIFY,
    WindowEventWINDOWPOSCHANGING,
    WindowEventWINDOWPOSCHANGED,
    WindowEventPOWER,
    WindowEventCOPYDATA,
    WindowEventCANCELJOURNAL,
    WindowEventNOTIFY,
    WindowEventINPUTLANGCHANGEREQUEST,
    WindowEventINPUTLANGCHANGE,
    WindowEventTCARD,
    WindowEventHELP,
    WindowEventUSERCHANGED,
    WindowEventNOTIFYFORMAT,
    WindowEventCONTEXTMENU,
    WindowEventSTYLECHANGING,
    WindowEventSTYLECHANGED,
    WindowEventDISPLAYCHANGE,
    WindowEventGETICON,
    WindowEventSETICON,
    WindowEventNCCREATE,
    WindowEventNCDESTROY,
    WindowEventNCCALCSIZE,
    WindowEventNCHITTEST,
    WindowEventNCPAINT,
    WindowEventNCACTIVATE,
    WindowEventGETDLGCODE,
    WindowEventSYNCPAINT,
    WindowEventNCMOUSEMOVE,
    WindowEventNCLBUTTONDOWN,
    WindowEventNCLBUTTONUP,
    WindowEventNCLBUTTONDBLCLK,
    WindowEventNCRBUTTONDOWN,
    WindowEventNCRBUTTONUP,
    WindowEventNCRBUTTONDBLCLK,
    WindowEventNCMBUTTONDOWN,
    WindowEventNCMBUTTONUP,
    WindowEventNCMBUTTONDBLCLK,
    WindowEventNCXBUTTONDOWN,
    WindowEventNCXBUTTONUP,
    WindowEventNCXBUTTONDBLCLK,
    WindowEventINPUT_DEVICE_CHANGE,
    WindowEventINPUT,
    WindowEventKEYFIRST,
    WindowEventKEYDOWN,
    WindowEventKEYUP,
    WindowEventCHAR,
    WindowEventDEADCHAR,
    WindowEventSYSKEYDOWN,
    WindowEventSYSKEYUP,
    WindowEventSYSCHAR,
    WindowEventSYSDEADCHAR,
    WindowEventUNICHAR,
    WindowEventKEYLAST,
    //UNICODE_NOCHAR,
    WindowEventIME_STARTCOMPOSITION,
    WindowEventIME_ENDCOMPOSITION,
    WindowEventIME_COMPOSITION,
    WindowEventIME_KEYLAST,
    WindowEventINITDIALOG,
    WindowEventCOMMAND,
    WindowEventSYSCOMMAND,
    WindowEventTIMER,
    WindowEventHSCROLL,
    WindowEventVSCROLL,
    WindowEventINITMENU,
    WindowEventINITMENUPOPUP,
    WindowEventGESTURE,
    WindowEventGESTURENOTIFY,
    WindowEventMENUSELECT,
    WindowEventMENUCHAR,
    WindowEventENTERIDLE,
    WindowEventMENURBUTTONUP,
    WindowEventMENUDRAG,
    WindowEventMENUGETOBJECT,
    WindowEventUNINITMENUPOPUP,
    WindowEventMENUCOMMAND,
    WindowEventCHANGEUISTATE,
    WindowEventUPDATEUISTATE,
    WindowEventQUERYUISTATE,
    WindowEventCTLCOLORMSGBOX,
    WindowEventCTLCOLOREDIT,
    WindowEventCTLCOLORLISTBOX,
    WindowEventCTLCOLORBTN,
    WindowEventCTLCOLORDLG,
    WindowEventCTLCOLORSCROLLBAR,
    WindowEventCTLCOLORSTATIC,
    WindowEventGETHMENU,
    WindowEventMOUSEFIRST,
    WindowEventMOUSEMOVE,
    WindowEventLBUTTONDOWN,
    WindowEventLBUTTONUP,
    WindowEventLBUTTONDBLCLK,
    WindowEventRBUTTONDOWN,
    WindowEventRBUTTONUP,
    WindowEventRBUTTONDBLCLK,
    WindowEventMBUTTONDOWN,
    WindowEventMBUTTONUP,
    WindowEventMBUTTONDBLCLK,
    WindowEventMOUSEWHEEL,
    WindowEventXBUTTONDOWN,
    WindowEventXBUTTONUP,
    WindowEventXBUTTONDBLCLK,
    WindowEventMOUSEHWHEEL,
    WindowEventMOUSELAST,
    WindowEventPARENTNOTIFY,
    WindowEventENTERMENULOOP,
    WindowEventEXITMENULOOP,
    WindowEventNEXTMENU,
    WindowEventSIZING,
    WindowEventCAPTURECHANGED,
    WindowEventMOVING,
    WindowEventPOWERBROADCAST,
    WindowEventDEVICECHANGE,
    WindowEventMDICREATE,
    WindowEventMDIDESTROY,
    WindowEventMDIACTIVATE,
    WindowEventMDIRESTORE,
    WindowEventMDINEXT,
    WindowEventMDIMAXIMIZE,
    WindowEventMDITILE,
    WindowEventMDICASCADE,
    WindowEventMDIICONARRANGE,
    WindowEventMDIGETACTIVE,
    WindowEventMDISETMENU,
    WindowEventENTERSIZEMOVE,
    WindowEventEXITSIZEMOVE,
    WindowEventDROPFILES,
    WindowEventMDIREFRESHMENU,
    WindowEventPOINTERDEVICECHANGE,
    WindowEventPOINTERDEVICEINRANGE,
    WindowEventPOINTERDEVICEOUTOFRANGE,
    WindowEventTOUCH,
    WindowEventNCPOINTERUPDATE,
    WindowEventNCPOINTERDOWN,
    WindowEventNCPOINTERUP,
    WindowEventPOINTERUPDATE,
    WindowEventPOINTERDOWN,
    WindowEventPOINTERUP,
    WindowEventPOINTERENTER,
    WindowEventPOINTERLEAVE,
    WindowEventPOINTERACTIVATE,
    WindowEventPOINTERCAPTURECHANGED,
    WindowEventTOUCHHITTESTING,
    WindowEventPOINTERWHEEL,
    WindowEventPOINTERHWHEEL,
    WindowEventPOINTERROUTEDTO,
    WindowEventPOINTERROUTEDAWAY,
    WindowEventPOINTERROUTEDRELEASED,
    WindowEventIME_SETCONTEXT,
    WindowEventIME_NOTIFY,
    WindowEventIME_CONTROL,
    WindowEventIME_COMPOSITIONFULL,
    WindowEventIME_SELECT,
    WindowEventIME_CHAR,
    WindowEventIME_REQUEST,
    WindowEventIME_KEYDOWN,
    WindowEventIME_KEYUP,
    WindowEventMOUSEHOVER,
    WindowEventMOUSELEAVE,
    WindowEventNCMOUSEHOVER,
    WindowEventNCMOUSELEAVE,
    WindowEventWTSSESSION_CHANGE,
    WindowEventTABLET_FIRST,
    WindowEventTABLET_LAST,
    WindowEventDPICHANGED,
    WindowEventDPICHANGED_BEFOREPARENT,
    WindowEventDPICHANGED_AFTERPARENT,
    WindowEventGETDPISCALEDSIZE,
    WindowEventCUT,
    WindowEventCOPY,
    WindowEventPASTE,
    WindowEventCLEAR,
    WindowEventUNDO,
    WindowEventRENDERFORMAT,
    WindowEventRENDERALLFORMATS,
    WindowEventDESTROYCLIPBOARD,
    WindowEventDRAWCLIPBOARD,
    WindowEventPAINTCLIPBOARD,
    WindowEventVSCROLLCLIPBOARD,
    WindowEventSIZECLIPBOARD,
    WindowEventASKCBFORMATNAME,
    WindowEventCHANGECBCHAIN,
    WindowEventHSCROLLCLIPBOARD,
    WindowEventQUERYNEWPALETTE,
    WindowEventPALETTEISCHANGING,
    WindowEventPALETTECHANGED,
    WindowEventHOTKEY,
    WindowEventPRINT,
    WindowEventPRINTCLIENT,
    WindowEventAPPCOMMAND,
    WindowEventTHEMECHANGED,
    WindowEventCLIPBOARDUPDATE,
    WindowEventDWMCOMPOSITIONCHANGED,
    WindowEventDWMNCRENDERINGCHANGED,
    WindowEventDWMCOLORIZATIONCOLORCHANGED,
    WindowEventDWMWINDOWMAXIMIZEDCHANGE,
    WindowEventDWMSENDICONICTHUMBNAIL,
    WindowEventDWMSENDICONICLIVEPREVIEWBITMAP,
    WindowEventGETTITLEBARINFOEX,
    WindowEventHANDHELDFIRST,
    WindowEventHANDHELDLAST,
    WindowEventAFXFIRST,
    WindowEventAFXLAST,
    WindowEventPENWINFIRST,
    WindowEventPENWINLAST,
    WindowEventAPP
}
PXWindowEventType;







#define PXDisplayScreenMonitorLength 32
#define PXDisplayScreenNameLength 32
#define PXDisplayScreenDeviceLength 128

// Container where windows can be created in
typedef struct PXDisplayScreen_
{
    char GraphicDeviceName[PXDisplayScreenDeviceLength];
    char NameMonitor[PXDisplayScreenMonitorLength];
    char NameID[PXDisplayScreenNameLength];

    int Width;
    int Height;
    int Cells;
    int Planes;
    int WidthMM;
    int HeightMM;


}
PXDisplayScreen;









// Version holder
typedef struct PXVersion_
{
    PXInt8U Major;
    PXInt8U Minor;
    PXInt8U Build;
    PXInt8U Patch;
}
PXVersion;

PXPublic PXActionResult PXAPI PXVersionFromString(PXVersion* const pxVersion, char* versioNString);
PXPublic PXActionResult PXAPI PXVersionToString(PXVersion* const pxVersion, char* versioNString);











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
#if OSUnix
#elif OSWindows
    HMONITOR ID;
#endif

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
// Scollbar
//---------------------------------------------------------

#define PXScollbarBehaviourVertical     (1<<0)
#define PXScollbarBehaviourHorrizontal  (1<<1)
#define PXScollbarBehaviourBoth         PXScollbarBehaviourHorrizontal | PXScollbarBehaviourVertical

typedef struct PXScollbar_
{
    PXResourceInfo Info;
}
PXScollbar;
//---------------------------------------------------------














typedef
#if OSUnix
int
#elif OSWindows
HWND
#else
void*
#endif
PXNativDrawWindowHandle;


typedef struct PXDisplay_
{
#if OSUnix
    Display* DisplayHandle;
    Window WindowRootHandle;
    GC GraphicContent;
#elif OSWindows
    int WindowRootHandle;
    void* DisplayHandle;
    int GraphicContent;
#endif

    char* Data;
    char* Name;

    int ProtocolVersion;
    int ProtocolRevision;

    char* ServerVendor;
    int VendorRelease;

    int ScreenDefaultID;
    int ScreenListAmount;

    PXSize DisplayScreenListAmount;
    PXDisplayScreen* DisplayScreenList;

    PXBool IsConnected;
    PXBool IsPrimary;
}
PXDisplay;




typedef struct PXNativDraw_
{
    PXResourceManager* ResourceManager;

    PXSize MonitorListAmount;
    PXMonitor* MonitorList;

    PXSize DisplayListAmount;
    PXDisplay* DisplayList;

    PXGUISystem* GUISystem;
}
PXNativDraw;




// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_
{
    PXResourceInfo Info;

#if OSUnix
#elif OSWindows
    HDC DeviceContextHandle; // Required for manual rendering
#endif

    void* InteractOwner; // Object that is the owner of given callbacks
    PXWindowEventFunction InteractCallBack; // Callback function for all events
    PXWindowDrawFunction DrawFunction; // If this is set, override OS-drawing and use this function instead


    PXUIElementPosition Position;

    //---<State-Info>------------------------
   // PXWindowBrush* BrushFront;
   // PXWindowBrush* BrushBackground;
    PXFont* FontForText;
    PXIcon* Icon; // Icon to be rendered

    //PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
    PXUIHoverState Hover;
    PXInt32U FlagsList;
    //---------------------------------------


    //---[Extended data]---------------------
    PXUIElementType Type;
    void* ExtendedData; // Extra data that will be allocated seperatly for additional use
    //---------------------------------------

    // Change this to something better
    char* NameContent;
    PXSize NameContentSize;
}
PXWindow;


//-----------------------------------------------------
// PXWindowExtendedBehaviour
//-----------------------------------------------------
typedef struct PXWindowMenuItem_ PXWindowMenuItem;


typedef struct PXWindowExtendedMenuItem_
{
    PXSize MenuItemAmount;
    PXWindowMenuItem* MenuItemList;
}
PXWindowExtendedMenuItem;

typedef struct PXWindowExtendedBehaviourTab_
{
    PXSize TABPageAmount;
    PXSize TABPageIndexCurrent;
    PXWindow* TABPageList; 
}
PXWindowExtendedBehaviourTab;

typedef struct PXResourceViewEntry_
{
    PXWindow* EntryList;
    PXResource* Resource;
}
PXResourceViewEntry;

typedef struct PXWindowExtendedBehaviourResourceView_
{
    PXSize EntryAmount;
    PXSize EntryIndexCurrent;
    PXResourceViewEntry* EntryList;

    PXResourceManager* ResourceManager;
}
PXWindowExtendedBehaviourResourceView;
//-----------------------------------------------------








typedef struct PXKeyBoardKeyInfo_
{
    PXVirtualKey Key;
    PXKeyPressState Mode;

    unsigned short Repeat; // Die Wiederholungsanzahl f�r die aktuelle Meldung.Der Wert gibt an, wie oft die Tastatureingabe automatisch angezeigt wird, wenn der Benutzer den Schl�ssel h�lt.Die Wiederholungsanzahl ist immer 1 f�r eine WM _ KEYUP - Nachricht.
    unsigned short ScanCode;// Der Scancode.Der Wert h�ngt vom OEM ab.
    unsigned short SpecialKey;// Gibt an, ob es sich bei der Taste um eine erweiterte Taste handelt, z.B.die rechte ALT - und STRG - Taste, die auf einer erweiterten Tastatur mit 101 oder 102 Tasten angezeigt werden.Der Wert ist 1, wenn es sich um einen erweiterten Schl�ssel handelt.andernfalls ist es 0.
    unsigned short KontextCode; // Der Kontextcode.Der Wert ist f�r eine WM _ KEYUP - Nachricht immer 0.
    unsigned short PreState; // Der vorherige Schl�sselzustand.Der Wert ist immer 1 f�r eine WM _ KEYUP - Nachricht.
    unsigned short GapState;

    unsigned char KeyID;
}
PXKeyBoardKeyInfo;


typedef struct PXWindowEventClose_
{
    PXBool CommitToClose;
}
PXWindowEventClose;

typedef struct PXWindowEventResize_
{
    PXInt16S Width;
    PXInt16S Height;
}
PXWindowEventResize;

typedef struct PXWindowEventInputMouseButton_
{
    PXKeyPressState PressState;
    PXMouseButton Button;
}
PXWindowEventInputMouseButton;

typedef struct PXWindowEventSelect_
{
    PXWindow* UIElementSelected;
}
PXWindowEventSelect;

typedef struct PXWindowEventInputMouseMove_
{
    PXInt32S AxisX;
    PXInt32S AxisY;
    PXInt32S DeltaX;
    PXInt32S DeltaY;
    PXInt32S PositionX;
    PXInt32S PositionY;
}
PXWindowEventInputMouseMove;

typedef struct PXWindowEventInputKeyboard_
{
    PXKeyPressState PressState;
    PXVirtualKey VirtualKey;

    PXInt16U CharacterID;
}
PXWindowEventInputKeyboard;














typedef struct PXWindowEvent_
{
    union
    {
        PXWindowEventClose Close;
        PXWindowEventResize Resize;
        PXWindowEventInputMouseButton InputMouseButton;
        PXWindowEventInputMouseMove InputMouseMove;
        PXWindowEventInputKeyboard InputKeyboard;
        PXWindowEventSelect Select;
    };

    //-----------------------------
    // Original data
    //-----------------------------
#if OSUnix
    XEvent* EventData;
#elif OSWindows
    HWND WindowHandle;
    UINT EventID;
    WPARAM ParamW;
    LPARAM ParamL;
#endif

    //-----------------------------
    // Translated Data
    //-----------------------------
    PXWindowEventType Type;

    PXWindow* UIElementReference;
    PXWindow* UIElementSender;
}
PXWindowEvent;

typedef struct PXWindowPixelSystemInfo_
{
#if OSUnix

#elif OSWindows
    HDC HandleDeviceContext;
    HWND HandleWindow;
#endif


    PXInt8U BitPerPixel; // 32=8Bit Default

    PXBool OpenGL;
    PXBool DirectX;
    PXBool GDI;
}
PXWindowPixelSystemInfo;





















//---------------------------------------------------------
// Menu Item - Behaviour
//---------------------------------------------------------
#define PXWindowMenuItemText
#define PXWindowMenuItemIcon
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


typedef struct PXWindowMenuItem_
{
    PXResourceInfo Info;

    PXIcon* Icon;

    char* TextData;
    PXSize TextSize;
}
PXWindowMenuItem;


typedef struct PXWindowMenuItemInfo_
{
    PXInt32U Flags;
    PXInt32U State;
    PXWindow* Parent;
    struct PXWindowMenuItemList_* ChildList;

    char* TextData;
    PXSize TextSize;
}
PXWindowMenuItemInfo;

typedef struct PXWindowMenuItemList_
{
    PXWindowMenuItemInfo* MenuItemInfoListData;
    PXSize MenuItemInfoListAmount;
}
PXWindowMenuItemList;
//---------------------------------------------------------






typedef struct PXUIElementPositionCalulcateInfo_
{
    // Input
    PXF32 WindowWidth;
    PXF32 WindowHeight;

    // Result
    PXInt32U DepthCounter;

    // Margin total
    PXF32 MarginLeft;
    PXF32 MarginTop;
    PXF32 MarginRight;
    PXF32 MarginBottom;


    PXF32 AA;
    PXF32 BA;
    PXF32 BB;
    PXF32 AB;

    PXF32 X;
    PXF32 Y;
    PXF32 Width;
    PXF32 Height;
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

    struct PXWindow_* ItemParent;
    struct PXWindow_* TreeView;

    //struct PXUIElement_* ElementSource;
    void* OwningObject;
    PXInt32U OwningObjectType;

    PXUIElementTreeViewItemInsertMode InsertMode;

    // Result
    struct _TREEITEM* ItemHandle;
}
PXUIElementTreeViewItemInfo;


typedef struct PXUIElementTabPageSingleInfo_
{
    PXWindow** UIElement;
    char* PageName;
    PXIcon* TABIcon;
    PXUIElementType UIElementType;
}
PXUIElementTabPageSingleInfo;


typedef struct PXUIElementTabPageInfo_
{
    PXUIElementTabPageSingleInfo* TabPageSingleInfoList;
    PXSize TabPageSingleInfoAmount;
}
PXUIElementTabPageInfo;



typedef struct PXWindowCreateWindowInfo_
{
    //void* EventFunction;
    void* EventOwner;

    PXWindow* UIElementReference;

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
PXWindowCreateWindowInfo;

typedef union PXWindowCreateInfoData_
{
    PXWindowMenuItemList MenuItem;
    PXWindowCreateWindowInfo Window;
    //  PXUIElementTextInfo Text;
    PXUIElementButtonInfo Button;
    PXUIElementTreeViewItemInfo TreeViewItem;
    PXUIElementSceneRenderInfo SceneRender;
    PXUIElementTabPageInfo TabPage;
    PXUIElementComboBoxInfo ComboBox;

    // Fetched
    PXRectangleXYWHI32 Size;
}
PXWindowCreateInfoData;

typedef enum PXWindowPropertyUpdateType_
{
    PXWindowPropertyUpdateTypeInvalid,
    PXWindowPropertyUpdateTypeRead,
    PXWindowPropertyUpdateTypeWrite,
    PXWindowPropertyUpdateTypeCompare
}
PXWindowPropertyUpdateType;

// Info about a window property like position or text content
typedef struct PXWindowPropertyInfo_
{
    PXWindow* WindowCurrent;
    PXWindow* WindowParentREF;
    PXUIElementProperty Property;
    PXWindowCreateInfoData Data;

    PXBool Show;    
    PXWindowPropertyUpdateType UpdateType;
}
PXWindowPropertyInfo;

typedef struct PXWindowCreateInfo_
{
    // Handles
    PXWindow* WindowCurrent;
    PXNativDrawWindowHandle CurrnetID;

    PXWindow* WindowParent;
    PXNativDrawWindowHandle ParentID;

    PXDisplay DisplayCurrent;

    // Positions
    PXInt32S X;
    PXInt32S Y;
    PXInt32U Width;
    PXInt32U Height;

    // Style
    PXInt32U BorderWidth;
    PXInt32U Border;

    // Setings
    PXBool Simple;
    PXBool AvoidCreation;
    PXBool Invisible;




    // CallBack on event
    void* InteractOwner;
    PXWindowEventFunction InteractCallBack;
  
 

    //PXInt32U FlagList;

    PXUIElementType Type;
    PXInt32U Behaviour;
    PXInt32U Setting; 
    PXColorRGBAF* ColorTintReference;

    PXColorRGBAF Color;


    PXBool UseCustomDraw;
    void* CustomDrawFunction;

    PXUIElementPosition Position;

#if OSUnix
#elif OSWindows
    HDC DeviceContextHandle;
    HINSTANCE InstanceHandle;

    PXInt32U WindowsWindowsStyleFlagsExtended;
    PXInt32U WindowsStyleFlags;
    char* WindowsTextContent;
    PXSize WindowsTextSize;
    const char* WindowsClassName;
#endif

    PXWindowDrawFunction DrawFunctionEngine; // default rendering of the engine
    PXWindowDrawFunction DrawFunctionOverride; // user defined rendering to overruide default

    // Additions
  

    char* Name;

  

    PXWindowCreateInfoData Data;
}
PXWindowCreateInfo;




PXPublic void PXAPI PXUIElementPositionCalculcate(PXWindow* const pxGUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo);















































#define PXSoundBehaviourLoop (1<<0)


typedef struct PXSound_
{
    PXResourceInfo Info;

    void* BaseObject;
    void* Data;

    PXInt32U SampleRate;
    PXInt32U ByteRate;
    PXInt16U NumerOfChannels;
    PXInt16U BlockAllign;
    PXInt16U BitsPerSample;

    PXSize DataSize;
    PXInt32U ChunkSize;
    PXInt16U AudioFormat;
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


    PXF32 Pitch; // [0.5 - 2.0]
    PXF32 Volume;
    PXF32 Position[3];
    PXF32 Velocity[3];
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
    PXF32 Pitch; // [0.5 - 2.0]
    PXF32 Volume;
    PXF32 Position[3];
    PXF32 Velocity[3];
    unsigned char Looping;


    //PXInt16U        wFormatTag;         /* format type * /
    PXInt16U Channels;          /* number of channels (i.e. mono, stereo...) * /
    PXInt32U SamplesPerSecound;     /* sample rate * /
    PXInt32U AverageBytesPerSecound;    /* for buffer estimation * /
    PXInt16U BlockAlignSize;        /* block size of data * /
    PXInt16U BitsPerSample;     /* number of bits per sample of mono data * /


}
PXAudioSource;*/














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

PXPublic const char* PXFileLocationModeToString(const PXFileLocationMode pxFileLocationMode);





//---------------------------------------------------------
// File Management
//---------------------------------------------------------
#define PXFileFormatMask                        0x000000FF

#define PXFileFormatInvalid                     0
#define PXFileFormatUnkown                      1
#define PXFileFormatA3DS                        2
#define PXFileFormatAAC                         3 // .acc
#define PXFileFormatAVI                         4 // .avi
#define PXFileFormatBitMap                      5 // .bmp
#define PXFileFormatBinkVideo                   6 // .bnk
#define PXFileFormatC                           7
#define PXFileFormatCSharp                      8
#define PXFileFormatCSS                         9
#define PXFileFormatCPP                         10
#define PXFileFormatCanonRaw3                   11
#define PXFileFormatDirectDrawSurfaceTexture    12 // .dds
#define PXFileFormatBinaryWindows               13
#define PXFileFormatBinaryLinux                 14
#define PXFileFormatEML                         15
#define PXFileFormatFastFile                    16
#define PXFileFormatFilmBox                     17
#define PXFileFormatFLAC                        18
#define PXFileFormatSpriteFont                  19
#define PXFileFormatGIF                         20
#define PXFileFormatOpenGLShader                21
#define PXFileFormatDirectXShader               22
#define PXFileFormatHighEfficiencyImageFile     23
#define PXFileFormatHTML                        24
#define PXFileFormatINI                         25
#define PXFileFormatEugeneRoshalArchive         26 // .rar
#define PXFileFormatJava                        27 // .jar
#define PXFileFormatJavaScript                  28 // .js
#define PXFileFormatJPEG                        29
#define PXFileFormatJSON                        30
#define PXFileFormatM4A                         31
#define PXFileFormatMIDI                        32
#define PXFileFormatMP3                         33
#define PXFileFormatMP4                         34
#define PXFileFormatMSI                         35
#define PXFileFormatMTL                         36
#define PXFileFormatN64                         37
#define PXFileFormatWavefront                   38
#define PXFileFormatMatroska                    39 // .mkv
#define PXFileFormatOGG                         40
#define PXFileFormatPDF                         41
#define PXFileFormatPHP                         42
#define PXFileFormatPLY                         43
#define PXFileFormatPNG                         44
#define PXFileFormatQOI                         45
#define PXFileFormatSTEP                        46
#define PXFileFormatSTL                         47
#define PXFileFormatSVG                         48
#define PXFileFormatTAR                         49
#define PXFileFormatTGA                         50
#define PXFileFormatTagImage                    51
#define PXFileFormatTrueTypeFont                52
#define PXFileFormatUniversalSceneDescription   53
#define PXFileFormatVideoObject                 54
#define PXFileFormatVRML                        55
#define PXFileFormatWAD                         56
#define PXFileFormatWave                        57
#define PXFileFormatWEBM                        58
#define PXFileFormatWEBP                        59
#define PXFileFormatWMA                         60
#define PXFileFormatXML                         61
#define PXFileFormatYAML                        62
#define PXFileFormatZIP                         63

#define PXFileFormatLua                         64 // .luaobj

#define PXFileFormatRedshiftWwisePackage        65 // r3d2\0x01\0x00\0x00\0x00, .wpk
#define PXFileFormatRedshiftMesh                66 // r3d2Mesh, .scb, sco
#define PXFileFormatRedshiftSkeleton            67 // r3d2sklt, .skl
#define PXFileFormatRedshiftAnimation           68 // r3d2anmd, r3d2canm, .anm
#define PXFileFormatWAudioBank                  69 // BKHD, .bnk 
#define PXFileFormatRGeometryWorld              70 // WGEO, .wgeo
#define PXFileFormatRGeometryMap                71 // OEGM, .mapgeo
#define PXFileFormatRProperty                   72 // 
#define PXFileFormatRTexture                    73 // TEX, .tex
#define PXFileFormatRPreLoad                    74 // PreLoad
#define PXFileFormatRLightGrid                  75 // 3
#define PXFileFormatRStringTable                76 // RST
#define PXFileFormatRPropertyOverride           78 // PTCH
#define PXFileFormatRSkinSimple                 79 // 0x33221100
#define PXFileFormatRSkinedMesh                 80 // 0x22FD4FC3
#define PXFileFormat6D                          81
#define PXFileFormatOEGM                        82


//---------------------------------------------------------
// Flags
#define PXFileFormatVarriantMask            0b00000000000000110000000000000000
#define PXFileFormatVarriantUndefined       0b00000000000000000000000000000000
#define PXFileFormatVarriantBinary          0b00000000000000010000000000000000
#define PXFileFormatVarriantText            0b00000000000000100000000000000000

#define PXFileFormatSignatureOffsetMask     0b00000000000001000000000000000000
#define PXFileFormatSignatureOffsetAtBegin  0b00000000000000000000000000000000
#define PXFileFormatSignatureOffsetAtEnd    0b00000000000001000000000000000000





//---------------------------------------------------------
// Resource types and structs
//---------------------------------------------------------
#define PXFileFormatTypeMask                        0x0000FF00
//#define PXFileFormatTypeSet(flag)   (flag << 8)

#define PXFileFormatTypeInvalid     0<<8 // Resource invalid, don't use.
#define PXFileFormatTypeCustom      1<<8 // Undetected but valid format. Needs to be handled by the caller
#define PXFileFormatTypeImage       2<<8 // Image for pixeldata
#define PXFileFormatTypeTexture     3<<8 // texture is like an image but with additional context
#define PXFileFormatTypeSound       4<<8      
#define PXFileFormatTypeVideo       5<<8      
#define PXFileFormatTypeModel       6<<8 // 3D model, collection of vertex data
#define PXFileFormatTypeFont        7<<8 // Collection of spites or points to render text
#define PXFileFormatTypeMaterial    8<<8      
#define PXFileFormatTypeCode        9<<8
#define PXFileFormatTypeShader      10<<8  
#define PXFileFormatTypeIcon        11<<8
#define PXFileFormatTypeDocument    12<<8
#define PXFileFormatTypeBinary      13<<8
#define PXFileFormatTypeArchiv      14<<8  // Compressed object
//-----------------------------------------------------










typedef struct PXResourceTransphereInfo_ PXResourceTransphereInfo;

typedef PXActionResult (PXAPI* PXResourceFileSizePredict)(void* const resource, PXSize* const fileSize);
typedef PXActionResult (PXAPI* PXResourceTransphereFunction)(PXResourceTransphereInfo* const pxResourceTransphereInfo);




//---------------------------------------------------------
// Contains all info about a file
typedef struct PXFileFormatInfo_
{
    char* ExtensionText;
    PXSize ExtensionLength;

    char* SigantureText;
    PXSize SigantureLength;

    PXInt8U SigantureOffset;

    PXInt32U Flags; // Behaviour

    //PXFileFormat FormatID;

    //PXInt32U ResourceType;

    PXResourceTransphereFunction ResourcePeek;
    PXResourceTransphereFunction ResourceLoad;
    PXResourceTransphereFunction ResourceSave;
}
PXFileFormatInfo;
//---------------------------------------------------------


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


    //PXResourceFileSizePredict FileSizePredict;
    //PXResourceTransphereFunction ResourcePeek;
    //PXResourceTransphereFunction ResourceLoad;
    //PXResourceTransphereFunction ResourceSave;

    PXResourceTransphereFunction OnDeviceDataRegister;  // Preallocate resources on the device
    PXResourceTransphereFunction OnDeviceDataUpload;    // Upload data fully

    void* ResourceSource;
    PXInt32U ResourceType;        // Type of the resource that 'Target' points to. Example: Image, Sound, Video...

    void* ResourceLoadContainer; // Used to store load/Store spesific helper object

    PXFileFormatInfo FormatInfo;
   // PXFileFormatInfo FormatInfoExpected;        // The format detected by the resource loader

    PXInt8U Flags;

    //void* Owner;

    PXF32 TimePeek;
    PXF32 TimeTransphere;
    PXF32 TimeDeviceDataRegister;
    PXF32 TimeDeviceDataUpload;
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
    PXByte* Data; // [Do not use directly] Data from where to read/write depending on the used method of linking.
    PXSize DataCursor; // [Do not use directly] Current position of the data.
    PXSize DataCursorBitOffset; // [Do not use directly] Current offset in bits of current byte
    PXSize DataUsed; // [Do not use directly] The total size of the data block.
    PXSize DataAllocated; // [Do not use directly] The size of the data pace in which you can move without triggering an invalid access.
    //--------------------

    PXAccessMode AccessMode;
    PXMemoryCachingMode CachingMode;
    PXFileLocationMode LocationMode;


    //-----------------------------------------------------
    // OS-Register
    //-----------------------------------------------------
#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
    int MappingHandle;
#elif OSWindows
    HANDLE MappingHandle;
#endif

#if OSWindows
    HANDLE FileHandle;
#endif

    FILE* FileID;
    int FileDescriptorID;
    //-----------------------------------------------------



    PXBitFormat BitFormatOfData;
    PXEndian EndiannessOfData;

    // The file path can't always be fetched from the OS.
    // for this we store the name here at creation time.
    char* FilePathData;
    PXSize FilePathSize; 

    PXTime TimeCreation;  // FILETIME
    PXTime TimeAccessLast;
    PXTime TimeWriteLast;

    // Statistic
    PXSize CounterOperationsRead;
    PXSize CounterOperationsWrite;
}
PXFile;
//---------------------------------------------------------

// Check is there is data to be read. 
// Use when we check if a file is loaded and not at the end
PXPublic PXBool PXAPI PXFileDataAvailable(const PXFile* const pxFile);




















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

typedef struct PXEngineSpriteMapInfo_
{
    PXShaderProgram* ShaderProgramCurrent;

    PXVector2I32S CellSize;
    PXVector2I32U MapSize;

    char* MapFilePath; // The texture that contains all textures to build the map.
}
PXEngineSpriteMapInfo;


typedef struct PXEngineFontCreateInfo_
{
    PXShaderProgram* ShaderProgramCurrent;

    PXSize RegisteredNameLength;
    char* RegisteredName; // Name of the font, registered by the OS
}
PXEngineFontCreateInfo;

typedef struct PXIconCreateInfo_
{
    PXImage* IconImage; // if set, generate a system icon from this
    PXSize OffsetX;
    PXSize OffsetY;
    PXSize Width;
    PXSize Height;
    PXSize RowSize;
    PXSize BitPerPixel;
}
PXIconCreateInfo;

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



typedef struct PXSpriteFrame_
{
    int x;
}
PXSpriteFrame;

typedef struct PXSpriteCreateInfo_
{
    PXTexture2D* TextureCurrent;
    PXShaderProgram* ShaderProgramCurrent;

    PXVector2F32 TextureScalingPoints[4];

    PXVector3F32 Position;
    PXVector2F32 Scaling;

    PXBool ViewRotationIgnore;
    PXBool ViewPositionIgnore;
    PXBool ContainsMultible; // Sprite is not a single texture but has multible
    PXInt16U CellSize;



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
    PXModelFormRectangleTX,
    PXModelFormCircle,
    PXModelFormCube
}
PXModelForm;



typedef struct PXModelCreateInfo_
{
    PXShaderProgram* ShaderProgramReference;
    PXF32 Scale;

    PXInt32U HitBoxBehaviour;

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
    PXF32 DeltaTime; // The time until we swap to the next screen
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
    PXF32 UpdateRate;
}
PXSpriteAnimatorInfo;




#define PXResourceCreateBehaviourSpawnInScene   (1<<0)
#define PXResourceCreateBehaviourLoadASYNC      (1<<1)


#define PXResourceCreateBehaviourIsASYNCCall      (1<<8)


typedef struct PXResourceCreateInfo_
{
    void** ObjectReference; // Reference to an adress to be filled with an object
    PXSize ObjectAmount; // If set to more than one, "ObjectReference" will contain a list of values

    void* Parent;

    char* FilePath;
    PXSize FilePathSize;

    char* Name;

    PXInt32U Type;
    PXInt32U Flags;

    // Do we need this?
   // PXDictionary* Lookup;
    //PXSize ObjectSize;

    union
    {
        char Data[1]; // Dummy value to access data without cast
        PXEngineSpriteMapInfo SpriteMap;
        PXEngineFontCreateInfo Font;
        PXSkyBoxCreateEventInfo SkyBox;
        PXSpriteCreateInfo Sprite;
        PXSpriteAnimatorInfo SpriteAnimator;
        PXEngineSoundCreateInfo Sound;
        PXShaderProgramCreateInfo ShaderProgram;
        PXTextureCubeCreateInfo TextureCube;
        PXTexture2DCreateInfo Texture2D;
        PXImageCreateInfo Image;
        PXWindowCreateInfo UIElement;
        PXModelCreateInfo Model;
        PXHitboxCreateInfo HitBox;
        PXBrushCreateInfo Brush;
        PXTimerCreateInfo Timer;
        PXIconAtlasCreateInfo IconAtlas;
        PXIconCreateInfo Icon;
    };
}
PXResourceCreateInfo;















// Returns the global resouremanager. 
// If not yet init, do so.
PXPublic PXResourceManager* PXAPI PXResourceManagerInstanceFetch(void);
PXPublic void PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager);



PXPublic PXInt32U PXAPI PXResourceManagerGenerateUniqeID();






PXPrivate PXActionResult PXAPI PXResourceCreateSkybox(PXResourceCreateInfo* const pxResourceCreateInfo, PXSkyBox* const pxSkyBox);
PXPrivate PXActionResult PXAPI PXResourceCreateBrush(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindowBrush* const pxWindowBrush);
PXPrivate PXActionResult PXAPI PXResourceCreateImage(PXResourceCreateInfo* const pxResourceCreateInfo, PXImage* const pxImage);
PXPrivate PXActionResult PXAPI PXResourceCreateShaderProgram(PXResourceCreateInfo* const pxResourceCreateInfo, PXShaderProgram* const pxShaderProgram);
PXPrivate PXActionResult PXAPI PXResourceCreateIcon(PXResourceCreateInfo* const pxResourceCreateInfo, PXIcon* const pxIcon);
PXPrivate PXActionResult PXAPI PXResourceCreateFont(PXResourceCreateInfo* const pxResourceCreateInfo, PXFont* const pxFont);
PXPrivate PXActionResult PXAPI PXResourceCreateMaterial(PXResourceCreateInfo* const pxResourceCreateInfo, PXMaterial* const pxMaterial);
PXPrivate PXActionResult PXAPI PXResourceCreateIconAtlas(PXResourceCreateInfo* const pxResourceCreateInfo, PXIconAtlas* const pxIconAtlas);
PXPrivate PXActionResult PXAPI PXResourceCreateTextureCube(PXResourceCreateInfo* const pxResourceCreateInfo, PXTextureCube* const pxTextureCube);
PXPrivate PXActionResult PXAPI PXResourceCreateTexture2D(PXResourceCreateInfo* const pxResourceCreateInfo, PXTexture2D* const pxTexture2D);
PXPrivate PXActionResult PXAPI PXResourceCreateModel(PXResourceCreateInfo* const pxResourceCreateInfo, PXModel* const pxModel);
PXPrivate PXActionResult PXAPI PXResourceCreateSprite(PXResourceCreateInfo* const pxResourceCreateInfo, PXSprite* const pxSprite);
PXPrivate PXActionResult PXAPI PXResourceCreateSpriteAnimator(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteAnimator* const pxSpriteAnimator);
PXPrivate PXActionResult PXAPI PXResourceCreateHitBox(PXResourceCreateInfo* const pxResourceCreateInfo, PXHitBox* const pxHitBox);
PXPrivate PXActionResult PXAPI PXResourceCreateSound(PXResourceCreateInfo* const pxResourceCreateInfo, PXSound* const pxSound);
PXPrivate PXActionResult PXAPI PXResourceCreateTimer(PXResourceCreateInfo* const pxResourceCreateInfo, PXEngineTimer* const pxEngineTimer);
PXPrivate PXActionResult PXAPI PXResourceCreateWindow(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindow* const pxWindow);
PXPrivate PXActionResult PXAPI PXResourceCreateSpriteMap(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteMap* const pxSpriteMap);



// Generate and store new resource. Load if possible
PXPublic PXActionResult PXAPI PXResourceManagerAdd(PXResourceCreateInfo* const pxResourceCreateInfo);
PXPublic PXActionResult PXAPI PXResourceManagerAddV(PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount);



PXPublic PXActionResult PXAPI PXResourceStoreName(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize);
PXPublic PXActionResult PXAPI PXResourceStorePath(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize);
PXPublic PXActionResult PXAPI PXResourceFetchName(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize);
PXPublic PXActionResult PXAPI PXResourceFetchPath(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize);




PXPublic PXActionResult PXAPI PXFileTypeInfoProbe(PXFileFormatInfo* const pxFileFormatInfo, const PXText* const pxText);

PXPublic PXActionResult PXAPI PXResourceManagerReferenceValidate(PXResourceReference* const pxResourceReference);

PXPublic PXActionResult PXAPI PXResourceLoad(PXResourceTransphereInfo* const pxResourceLoadInfo, const PXText* const filePath);
PXPublic PXActionResult PXAPI PXResourceLoadA(PXResourceTransphereInfo* const pxResourceLoadInfo, const char* const filePath);

PXPublic PXActionResult PXAPI PXResourceSave(PXResourceTransphereInfo* const pxResourceSaveInfo, const PXText* const filePath);
PXPublic PXActionResult PXAPI PXResourceSaveA(PXResourceTransphereInfo* const pxResourceSaveInfo, const char* const filePath);



PXPublic PXActionResult PXAPI PXResourceSerialize(PXResource* const pxResource, PXFile* const pxFile);
PXPublic PXActionResult PXAPI PXResourceParse(PXResource* const pxResource, PXFile* const pxFile);



PXPublic PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName);


#endif