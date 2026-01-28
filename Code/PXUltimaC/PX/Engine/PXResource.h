#pragma once

#ifndef PXResourceManagerIncludedd
#define PXResourceManagerIncludedd

#include <PX/OS/Error/PXResult.h>
#include <PX/Media/PXColor.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMatrix.h>
#include <PX/OS/File/PXFileFormat.h>

#include <stdarg.h>

#if OSUnix
#include <stdio.h>
#include <X11/X.h> // X11 XID, ulong 8 Byte
#include <X11/Xlib.h> // XDisplay
#endif
#include "ECS/Resource/Icon/PXIcon.h"
#include "ECS/Resource/Font/PXFont.h"
#include "ECS/Resource/Window/PXWindow.h"
#include "ECS/Resource/Texture/PXTexture.h"
#include "ECS/Entity/Sprite/PXSprite.h"
#include "ECS/Resource/Shader/PXShader.h"
#include "ECS/Resource/Monitor/PXMonitor.h"
#include "ECS/Resource/Window/PXWindow.h"
#include "ECS/Entity/Camera/PXCamera.h"
#include "ECS/Component/Rectangle/PXRectangle.h"

typedef enum PXGraphicResourceLocation_
{
    PXGraphicResourceLocationInvalid,
    PXGraphicResourceLocationFullyUnloaded, // Resource is known but not loaded
    PXGraphicResourceLocationLoadedNotRegisterd, // Rsource is loaded but not known by external rendering systems
    PXGraphicResourceLocationLoadAndRegistered, // Resource si loaded and registerd
    PXGraphicResourceLocationRegisteredOnly, // Resouce is uploaded and not loaded anymore.
}
PXGraphicResourceLocation;


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


//---------------------------------------------------------
// Resource types and structs
//---------------------------------------------------------

// Describes the exact type of a resource
#define PXResourceTypeInvalid 0 // Resource invalid, don't use.

#define PXResourceTypeCustom   1 // Undetected but valid format. Needs to be handled by the caller

    //-----------------------------------------------------
    // Resource Level A - Indepepended
    //-----------------------------------------------------
#define PXResourceTypeTexture     2       // Image for pixeldata
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

















typedef enum PXHandleType_
{
    PXHandleTypeInvalid,
    PXHandleTypeDirectory,
    PXHandleTypeThread,
    PXHandleTypeProcess,
    PXHandleTypeIOCompletionReserve,
    PXHandleTypeEvent,
    PXHandleTypeMutant,
    PXHandleTypeSemaphore,
    PXHandleTypeTimer,
    PXHandleTypeIETimer,
    PXHandleTypeWindowStation,
    PXHandleTypeDesktop,
    PXHandleTypeFile,
    PXHandleTypeWaitCompleation,
    PXHandleTypeIoCompleation,
    PXHandleTypeTpWorkerFactor,
    PXHandleTypeSection,
    PXHandleTypeKey,
    PXHandleTypeALPCPort,
    PXHandleTypeEtwRegistration
}
PXHandleType;



typedef struct PXHandleInfo_
{
    char Description[256];
    char TypeName[32]; 
    PXHandleType Type;
}
PXHandleInfo;


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









// Cool assert for debug

#define PXAssert(condition, message) \
if(!(condition)) \
{ \
    const char* buffer = _PX_FILENAME_; \
    PXLogPrint(PXLoggingError, buffer, "Assert", message); \
    DebugBreak(); \
}






//---------------------------------------------------------
// Module - Library or seperate Element that manages
//---------------------------------------------------------

#define PXModuleStateDoInitialize   1<<0
#define PXModuleStateDoRelease      1<<1

typedef PXResult(PXAPI* PXModuleStateFunction)(void** moduleAdress, const PXI32U flags); // Initialize or Releases a module.

typedef struct PXModule_
{
    void* ModuleAdress;
    PXModuleStateFunction StateChange;
}
PXModule;
//---------------------------------------------------------




typedef struct PXResource_
{
    PXI32U Type;
    void* ResourceAdress;
}
PXResource;


/*
#define PXResourceManagerInit (1<<0)

// Container to manage resources by loading or saving
typedef struct PXResourceManager_
{
    PXLock CreateLock;

    PXListDynamic NameCache;
    PXListDynamic SourcePathCache;

    // Register List
    PXI32U UniqeIDGeneratorCounter;

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
    struct PXTexture_* Texture2DFailBack;

    PXI32U Flags;
}
PXResourceManager;
//=========================================================


*/















/*

typedef struct PXTexture_
{
    PXECSInfo Info;

    PXGraphicRenderFilter Filter;
    PXGraphicImageLayout LayoutNear;
    PXGraphicImageLayout LayoutFar;
    PXGraphicImageWrap WrapWidth;

    PXTexture* Image;
}
PXTexture;

typedef struct PXTexture_
{
    PXECSInfo Info;

    PXGraphicRenderFilter Filter;
    PXGraphicImageLayout LayoutNear;
    PXGraphicImageLayout LayoutFar;
    PXGraphicImageWrap WrapHeight;
    PXGraphicImageWrap WrapWidth;

    PXTexture* Image;
}
PXTexture;

typedef struct PXTexture_
{
    PXECSInfo Info;

    PXTexture* Image;
}
PXTexture;

// A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
typedef struct PXTexture_
{
    PXECSInfo Info;

    PXColorFormat Format;

    PXTexture* ImageA;
    PXTexture* ImageB;
    PXTexture* ImageC;
    PXTexture* ImageD;
    PXTexture* ImageE;
    PXTexture* ImageF;
}
PXTexture;


typedef struct PXTextureEE_
{
    PXTextureType Type;

    union
    {
        PXTexture Texture1D;
        PXTexture Texture2D;
        PXTexture Texture3D;
        PXTexture TextureCube;
    };
};
//PXTexture;

*/

typedef struct PXDepthStencilSurface_
{
    int x;
}
PXDepthStencilSurface;

typedef struct PXRenderTarget_
{
    PXECSInfo Info; // IDirect3DVertexBuffer9
}
PXRenderTarget;












//-----------------------------------------------------
// Text
//-----------------------------------------------------
#if 1
typedef struct PXDialogText_
{
    PXECSInfo Info;

    PXVector2F32 Position;
    PXVector2F32 Scaling;

    PXBool DoubleRenderForShadow;

    PXText* Text;
    PXFont* Font;
    PXF32 FontScaling;

    PXI32U TextRenderAmount;
}
PXDialogText;
#endif
//-----------------------------------------------------







// Shapes

typedef struct PXRectangleF_
{
    PXF32 X;
    PXF32 Y;
    PXF32 Width;
    PXF32 Height;
}
PXRectangleF;



typedef struct PXMatrixModelViewProjection_
{
    PXMatrix4x4F MatrixModel;
    PXMatrix4x4F MatrixView;
    PXMatrix4x4F MatrixProjection;
}
PXMatrixModelViewProjection;








//---------------------------------------------------------
// Graphic system
//---------------------------------------------------------
#define PXGraphicSystemInvalid  0
#define PXGraphicSystemNative   1
#define PXGraphicSystemOpenGL   2
#define PXGraphicSystemDirectX  3
#define PXGraphicSystemVulcan   4

typedef PXI8U PXGraphicSystem;
//---------------------------------------------------------


typedef struct PXNativDraw_
{
    PXSize MonitorListAmount;
    PXMonitor* MonitorList;

    PXSize DisplayListAmount;
    PXDisplay* DisplayList;

    PXGUIManager* GUISystem;
}
PXNativDraw;





typedef enum PXWindowPropertyUpdateType_
{
    PXWindowPropertyUpdateTypeInvalid,
    PXWindowPropertyUpdateTypeRead,
    PXWindowPropertyUpdateTypeWrite,
    PXWindowPropertyUpdateTypeCompare
}
PXWindowPropertyUpdateType;









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


    //PXI16U        wFormatTag;         /* format type * /
    PXI16U Channels;          /* number of channels (i.e. mono, stereo...) * /
    PXI32U SamplesPerSecound;     /* sample rate * /
    PXI32U AverageBytesPerSecound;    /* for buffer estimation * /
    PXI16U BlockAlignSize;        /* block size of data * /
    PXI16U BitsPerSample;     /* number of bits per sample of mono data * /


}
PXAudioSource;*/

#endif