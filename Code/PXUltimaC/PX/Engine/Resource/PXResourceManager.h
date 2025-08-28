#ifndef PXResourceManagerIncluded
#define PXResourceManagerIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Container/ListDynamic/PXListDynamic.h>
#include <PX/Media/PXColor.h>





typedef enum PXIlluminationMode_
{
    PXIlluminationNone,
    PXIlluminationColorAndAmbientDisable,        // [0] Color on and Ambient off
    PXIlluminationColorAndAmbientEnable,        // [1] Color on and Ambient on
    PXIlluminationHighlightEnable,    // [2] Highlight on
    PXIlluminationReflectionOnRayTraceEnable,    // [3] Reflection on and Ray trace on
    PXIlluminationReflectionOnRayTraceTransparency,     // [4] Transparency: Glass on, Reflection : Ray trace on
    PXIlluminationReflectionOnRayTraceFresnel,     // [5] Reflection : Fresnel on and Ray trace on
    PXIlluminationReflectionOnRayTraceTransparencyFresnel,     // [6] Transparency : Refraction on, Reflection : Fresnel offand Ray trace on
    PXIlluminationReflectionOnRayTraceFullEnable,    // [7] Transparency : Refraction on, Reflection : Fresnel onand Ray trace on
    PXIlluminationReflectionEnable,     // [8] Reflection on and Ray trace off
    PXIlluminationTransparencyEnable,     // [9] Transparency : Glass on, Reflection : Ray trace off
    PXIlluminationShadowsEnable      // [10] Casts shadows onto invisible surfaces
}
PXIlluminationMode;

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

typedef enum PXRenderFilter_
{
    PXRenderFilterInvalid,
    PXRenderFilterNoFilter, // No filter Option, use this for Pixelated Textures.
    PXRenderFilterBilinear, // Level I Filter
    PXRenderFilterTrilinear // Level II Filter
}
PXRenderFilter;

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

typedef enum PXImageLayout_
{
    PXImageLayoutInvalid,

    // Returns the value of the texture element that is nearest to the specified texture coordinates.
    PXImageLayoutNearest,

    // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
    // These can include items wrapped or repeated from other parts of a texture, depending on the values of GL_TEXTURE_WRAP_Sand GL_TEXTURE_WRAP_T, and on the exact mapping.
    PXImageLayoutLinear,

    //Chooses the mipmap that most closely matches the size of the pixel being texturedand
    //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates) to produce a texture value.
    PXImageLayoutMipMapNearestNearest,

    //Chooses the mipmap that most closely matches the size of the pixel being texturedand
    //uses the GL_LINEAR criterion(a weighted average of the four texture elements that are closest to the specified texture coordinates) to produce a texture value.
    PXImageLayoutMipMapLinearNearest,

    //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
    //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates)
    //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
    PXImageLayoutMipMapNNearestLinear,

    //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
    //uses the GL_LINEAR criterion(a weighted average of the texture elements that are closest to the specified texture coordinates)
    //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
    PXImageLayoutMipMapLinearLinear
}
PXImageLayout;

typedef enum PXImageWrap_
{
    PXImageWrapInvalid,

    // Images will be used 'as is' and will not be streched whatsoever.
    PXImageWrapNoModification,

    // Strech
    PXImageWrapStrechEdges,

    PXImageWrapStrechEdgesAndMirror,

    // Tiles the image in a gridformat
    PXImageWrapRepeat,

    // Tiles the image in a gridformat but also flip them every time.
    PXImageWrapRepeatAndMirror,
}
PXImageWrap;

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

typedef enum PXDrawMode_
{
    PXDrawModeInvalid,
    PXDrawModePoint,
    PXDrawModeLine,
    PXDrawModeLineLoop,
    PXDrawModeLineStrip,
    PXDrawModeLineStripAdjacency,
    PXDrawModeLineAdjacency,
    PXDrawModeTriangle,
    PXDrawModeTriangleAdjacency,
    PXDrawModeTriangleFAN,
    PXDrawModeTriangleStrip,
    PXDrawModeTriangleStripAdjacency,
    PXDrawModeWireFrame,
    PXDrawModeSquare,
    PXDrawModeSquareStrip,
    PXDrawModePatches
}
PXDrawMode;

typedef enum PXDrawFillMode_
{
    PXDrawModeFillInvalid,
    PXDrawModeFillPoints,
    PXDrawModeFillLines,
    PXDrawModeFillFill
}
PXDrawFillMode;

typedef enum PXResourceAction_
{
    PXResourceActionInvalid,
    PXResourceActionCreate,
    PXResourceActionDelete,
    PXResourceActionUpdate,
    PXResourceActionSelect
}
PXResourceAction;













typedef
#if OSUnix
int
#elif OSWindows
HWND
#else
void*
#endif
PXWindowHandle;

typedef
#if OSUnix
int
#elif OSWindows
HMONITOR
#else
void*
#endif
PXMonitorHandle;











typedef struct PXTexture_
{
    void* PixelData;
    PXSize PixelDataSize;

    PXSize Width;
    PXSize Height;
    PXSize Depth;

    PXColorFormat Format;
}
PXTexture;


PXPublic PXResult PXAPI PXTextureRelease(PXTexture* const pxTexture);
PXPublic PXResult PXAPI PXTextureCopyAsIs(PXTexture* const pxTexture, const PXTexture* const pxTextureSource);
PXPublic PXResult PXAPI PXTextureCopyAsNew(PXTexture* const pxTexture, const PXTexture* const pxTextureSource);
PXPublic PXResult PXAPI PXTextureResize(PXTexture* const pxTexture, const PXColorFormat format, const PXSize width, const PXSize height);
PXPublic PXResult PXAPI PXTextureFlipHorizontal(PXTexture* const pxTexture);
PXPublic PXResult PXAPI PXTextureFlipVertical(PXTexture* const pxTexture);
PXPublic PXResult PXAPI PXTextureRemoveColor(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue);
PXPublic PXResult PXAPI PXTextureFillColorRGBA8(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);
PXPublic void* PXAPI PXTextureDataPoint(const PXTexture* const pxTexture, const PXSize x, const PXSize y);
PXPublic PXSize PXAPI PXTexturePixelPosition(const PXTexture* const pxTexture, const PXSize x, const PXSize y);
PXPublic void PXAPI PXTexturePixelSetRGB8
(
    PXTexture* const pxTexture,
    const PXSize x,
    const PXSize y,
    const PXByte red,
    const PXByte green,
    const PXByte blue
);














//---------------------------------------------------------
// Icon/Symbols
//---------------------------------------------------------
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
    PXI16U CellSize;
    PXI16U CellAmountX; // Will be generated
    PXI16U CellAmountY; // Will be generated
    PXI16U CellAmountTotal; // Will be generated
}
PXIconAtlasCreateInfo;


PXPublic PXResult PXAPI PXGUIIconLoad(PXIcon* const pxIcon);
//---------------------------------------------------------

















//---------------------------------------------------------
typedef struct PXRectangleXYWHI32_
{
    PXI32S X;
    PXI32S Y;
    PXI32S Width;
    PXI32S Height;
}
PXRectangleXYWHI32;

typedef struct PXRectangleLTRBI32_
{
    PXI32S Left;
    PXI32S Top;
    PXI32S Right;
    PXI32S Bottom;
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



















typedef PXI32U PXResourceID;

typedef struct PXResourceManager_
{
    PXLock AsyncLock;

    PXResourceID UniqeIDCounter;

    PXListDynamic NameCache;
    PXListDynamic SourcePathCache;

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

}
PXResourceManager;











#define PXResourceCreateBehaviourSpawnInScene   (1<<0)
#define PXResourceCreateBehaviourLoadASYNC      (1<<1)
#define PXResourceCreateBehaviourIsASYNCCall      (1<<8)

typedef struct PXResourceCreateInfo_
{
    void** ObjectReference; // Reference to an adress to be filled with an object
    PXSize ObjectAmount; // If set to more than one, "ObjectReference" will contain a list of values

    void* Parent;


    char* FilePathAdress;
    PXSize FilePathSize;

    char* NameÁdress;
    PXSize NameÁdressSize;

    PXI32U Type;
    PXI32U Flags;

    // Do we need this?
   // PXDictionary* Lookup;
    //PXSize ObjectSize;

#if 0
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
#endif
}
PXResourceCreateInfo;















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
typedef union PXHandle_
{
    PXI32U OpenGLID; // Simple ID for an object.
    void* DirectXInterface; // DirectX uses interfaces to communicate to a element.
#if OSUnix
    Window WindowHandle; // Linux X11 System
    XFontStruct* FontHandle;
    pthread_t ThreadHandle;
#elif OSWindows
    HWND WindowHandle;
    HBRUSH BrushHandle;
    HFONT FontHandle;
    HMENU MenuHandle;
    HICON IconHandle;
    HANDLE ThreadHandle;
#endif
}
PXHandle;



















typedef PXI32U PXResourceID;

// Internal engine identification
// Additional use is to define current storage and interactions.
typedef struct PXResourceInfo_
{
    PXHierarchicalNode Hierarchy;

    PXHandle Handle;

    PXResourceID ID; // Identification of this object managed by the engine itself.
   
    PXI32U Setting; // Rendering behaviour
    PXI32U Behaviour; // Depends on the type of the resource
}
PXResourceInfo;












// Returns the global resouremanager. 
// If not yet init, do so.
PXPublic PXResourceManager* PXAPI PXResourceManagerInstanceFetch(void);
PXPublic PXActionResult PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager);


// Create uniqe identification, 7
PXPublic PXResourceID PXAPI PXResourceManagerGenerateUniqeID();


#define PXResourceTransphereLoad 1
#define PXResourceTransphereStore 2
PXPublic PXActionResult PXAPI PXResourceTransphere();


// Additional property storage

#define PXResourcePropertyName 1
#define PXResourcePropertyPath 2

typedef struct PXResourceProperty_
{
    char* NameAdress;
    PXSize NameSize
}
PXResourceProperty;

PXPublic PXActionResult PXAPI PXResourcePropertyE(PXResourceProperty* const pxResourceProperty, const PXBool doWrite);




PXPublic PXResult PXAPI PXResourceAdd(PXResourceInfo* const pxResourceInfo, void* payload);
PXPublic PXResult PXAPI PXResourceRemove(PXResourceInfo* const pxResourceInfo);








PXPrivate PXResult PXAPI PXResourceCreateSkybox(PXResourceCreateInfo* const pxResourceCreateInfo, PXSkyBox* const pxSkyBox);
PXPrivate PXResult PXAPI PXResourceCreateBrush(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindowBrush* const pxWindowBrush);
PXPrivate PXResult PXAPI PXResourceCreateImage(PXResourceCreateInfo* const pxResourceCreateInfo, PXImage* const pxImage);
PXPrivate PXResult PXAPI PXResourceCreateShaderProgram(PXResourceCreateInfo* const pxResourceCreateInfo, PXShaderProgram* const pxShaderProgram);
PXPrivate PXResult PXAPI PXResourceCreateIcon(PXResourceCreateInfo* const pxResourceCreateInfo, PXIcon* const pxIcon);
PXPrivate PXResult PXAPI PXResourceCreateFont(PXResourceCreateInfo* const pxResourceCreateInfo, PXFont* const pxFont);
PXPrivate PXResult PXAPI PXResourceCreateMaterial(PXResourceCreateInfo* const pxResourceCreateInfo, PXMaterial* const pxMaterial);
PXPrivate PXResult PXAPI PXResourceCreateIconAtlas(PXResourceCreateInfo* const pxResourceCreateInfo, PXIconAtlas* const pxIconAtlas);
PXPrivate PXResult PXAPI PXResourceCreateTextureCube(PXResourceCreateInfo* const pxResourceCreateInfo, PXTextureCube* const pxTextureCube);
PXPrivate PXResult PXAPI PXResourceCreateTexture2D(PXResourceCreateInfo* const pxResourceCreateInfo, PXTexture2D* const pxTexture2D);
PXPrivate PXResult PXAPI PXResourceCreateModel(PXResourceCreateInfo* const pxResourceCreateInfo, PXModel* const pxModel);
PXPrivate PXResult PXAPI PXResourceCreateSprite(PXResourceCreateInfo* const pxResourceCreateInfo, PXSprite* const pxSprite);
PXPrivate PXResult PXAPI PXResourceCreateSpriteAnimator(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteAnimator* const pxSpriteAnimator);
PXPrivate PXResult PXAPI PXResourceCreateHitBox(PXResourceCreateInfo* const pxResourceCreateInfo, PXHitBox* const pxHitBox);
PXPrivate PXResult PXAPI PXResourceCreateSound(PXResourceCreateInfo* const pxResourceCreateInfo, PXSound* const pxSound);
PXPrivate PXResult PXAPI PXResourceCreateTimer(PXResourceCreateInfo* const pxResourceCreateInfo, PXEngineTimer* const pxEngineTimer);
PXPrivate PXResult PXAPI PXResourceCreateWindow(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindow* const pxWindow);
PXPrivate PXResult PXAPI PXResourceCreateSpriteMap(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteMap* const pxSpriteMap);


#endif