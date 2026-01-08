#pragma once

#ifndef PXResourceManagerIncludedd
#define PXResourceManagerIncludedd

#include <PX/OS/Error/PXActionResult.h>
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
#include "ECS/Entity/HitBox/PXHitBox.h"
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

#define PXECSInfoIdentityMask    0b0000000000001111
#define PXECSInfoExist           0b0000000000000001 // Indicate if resource is valid
#define PXECSInfoActive          0b0000000000000010 // Is it interactable or does it tick?
#define PXECSInfoRender          0b0000000000000100 // Shall it be rendered?
#define PXECSInfoSelected        0b0000000000001000

#define PXECSInfoStorageMask     0b0000000011110000
#define PXECSInfoStorageDrive    0b0000000000010000 // Resource is in permanent storage
#define PXECSInfoStorageCached   0b0000000000100000 // Resource is in semi-permanent cache (temp file)
#define PXECSInfoStorageMemory   0b0000000001000000 // Resource exists in RAM
#define PXECSInfoStorageDevice   0b0000000010000000 // Resource exists in spesific device

#define PXECSInfoNameMask        0b0000001100000000
#define PXECSInfoHasName         0b0000000100000000 // Is the name stored
#define PXECSInfoHasSource       0b0000001000000000 // Is resource loaded from a path, if yes, path is cached.

#define PXECSInfoUseByMask       0b0001100000000000
#define PXECSInfoUseByOS         0b0000000000000000
#define PXECSInfoUseByUser       0b0000100000000000
#define PXECSInfoUseByEngine     0b0001000000000000
#define PXECSInfoUseByUndefined  0b0001100000000000

#define PXECSInfoPermissionMask    0b1110000000000000
#define PXECSInfoPermissionREAD    0b1000000000000000
#define PXECSInfoPermissionWRITE   0b0100000000000000
#define PXECSInfoPermissionEXECUTE 0b0010000000000000


#define PXECSInfoConstData PXECSInfoPermissionREAD | PXECSInfoStorageMemory | PXECSInfoExist | PXECSInfoActive | PXECSInfoRender

#define PXECSInfoOK              PXECSInfoExist | PXECSInfoActive | PXECSInfoRender
#define PXECSInfoNoRender              PXECSInfoExist | PXECSInfoActive















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

typedef PXActionResult(PXAPI* PXModuleStateFunction)(void** moduleAdress, const PXI32U flags); // Initialize or Releases a module.

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











typedef struct PXECSComponentScale2D
{
   // PXCompoment dx;
    PXVector2F32 Scale;
};

typedef struct PXECSComponentPosition2D
{
    PXVector2F32 Position2D;
};

typedef struct PXECSComponentRotation2D
{
    PXVector2F32 Rotation2D;
};


typedef struct PXMatrixModelViewProjection_
{
    PXMatrix4x4F MatrixModel;
    PXMatrix4x4F MatrixView;
    PXMatrix4x4F MatrixProjection;
}
PXMatrixModelViewProjection;



#define PXDeviceDisplaySize 32
#define PXDeviceNameSize 32
#define PXDeviceIDSize 64
#define PXDeviceKeySize 128

#define PXDeviceOpenGLDriverSize 32
#define PXDeviceOpenGLVendorSize 32
#define PXDeviceOpenGLRendererSize 32
#define PXDeviceOpenGLShaderSize 32



typedef struct PXGPUPhysical_
{
    char DeviceDisplay[PXDeviceDisplaySize]; // \\.\DISPLAY6
    char DeviceName[PXDeviceNameSize]; // NVIDIA GeForce GTX 1080
    char DeviceID[PXDeviceIDSize]; // Windows DeviceID, "PCI\VEN_10DE&DEV_1B80&SUBSYS_336..."
    char DeviceKey[PXDeviceKeySize]; // Windows Regestry "\Registry\Machine\System\Current..."

    char Driver[PXDeviceOpenGLDriverSize]; // xxxxx.DLL

    char Vendor[PXDeviceOpenGLVendorSize];
    char Renderer[PXDeviceOpenGLRendererSize];
    char Shader[PXDeviceOpenGLShaderSize];

    PXI64U VideoMemoryDedicated; // dedicated video memory, total size (in kb) of the GPU memory
    PXI64U VideoMemoryCurrent; // total available memory, total size (in Kb) of the memory available for allocations
    PXI64U VideoMemoryTotal; // current available dedicated video memory (in kb), currently unused GPU memory

    PXI64U VideoMemoryEvictionCount; // How many times memory got displaced to Main-RAM
    PXI64U VideoMemoryEvictionSize; // size of total video memory evicted (in kb)

    PXBool IsConnectedToMonitor;

    PXMonitor* AttachedMonitor;
}
PXGPUPhysical;



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
    PXECSInfo Info;

    union
    {
        void* GenericHandle;
        void* WaveOutHandle; // HWAVEOUT
        void* WaveInHandle; // HWAVEIN
    };


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

    PXI32U FXSettingFlagList;


    PXF32 Pitch; // [0.5 - 2.0]
    PXF32 Volume;
    PXF32 Position[3];
    PXF32 Velocity[3];
    PXBool Looping;




    //-------------------------------------------------
    // General Shared Info
    //-------------------------------------------------
    char DeviceName[PXAudioDeviceNameSize];
    PXI16U ManufacturerID;
    PXI16U ProductID;
    PXI8U DriverVersionMajor;
    PXI8U DriverVersionMinor;
    PXI32U SupportFlags;
    //-------------------------------------------------


    PXI32U FormatSupportFlags;

    PXI16U FormatTag;         // format type
    PXI16U Channels;          // number of channels (i.e. mono, stereo...)
    PXI32U SamplesPerSecound;     // sample rate
    PXI32U AverageBytesPerSecound;    // for buffer estimation
    PXI16U BlockAlignSize;        // block size of data
    PXI16U BitsPerSample;     // number of bits per sample of mono data

    //-------------------------------------------------
    // Used By MIDI
    //-------------------------------------------------
    PXI16U Technology;           // type of device
    PXI16U Voices;               // # of voices (internal synth only)
    PXI16U Notes;                // max # of notes (internal synth only)
    PXI16U ChannelMask;          // channels used (internal synth only)
    //-------------------------------------------------

    union
    {
        PXI16U wValidBitsPerSample; // Valid bits in each sample container
        PXI16U wSamplesPerBlock;    // Samples per block of audio data; valid
        // if wBitsPerSample=0 (but rarely used).
        PXI16U wReserved;           // Zero if neither case above applies.
    } Samples;
    PXI32U dwChannelMask;          // Positions of the audio channels


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


    //PXI16U        wFormatTag;         /* format type * /
    PXI16U Channels;          /* number of channels (i.e. mono, stereo...) * /
    PXI32U SamplesPerSecound;     /* sample rate * /
    PXI32U AverageBytesPerSecound;    /* for buffer estimation * /
    PXI16U BlockAlignSize;        /* block size of data * /
    PXI16U BitsPerSample;     /* number of bits per sample of mono data * /


}
PXAudioSource;*/



/*
PXPrivate PXResult PXAPI PXResourceCreateShaderProgram(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXShaderProgram PXREF pxShaderProgram);
PXPrivate PXResult PXAPI PXResourceCreateIcon(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXIcon PXREF pxIcon);
PXPrivate PXResult PXAPI PXResourceCreateFont(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXFont PXREF pxFont);
PXPrivate PXResult PXAPI PXResourceCreateMaterial(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXMaterial PXREF pxMaterial);
PXPrivate PXResult PXAPI PXResourceCreateIconAtlas(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXIconAtlas PXREF pxIconAtlas);
PXPrivate PXResult PXAPI PXResourceCreateTexture(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXTexture PXREF pxTexture);
PXPrivate PXResult PXAPI PXResourceCreateModel(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXModel PXREF pxModel);
PXPrivate PXResult PXAPI PXResourceCreateSprite(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXSprite PXREF pxSprite);
PXPrivate PXResult PXAPI PXResourceCreateSpriteAnimator(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXSpriteAnimator PXREF pxSpriteAnimator);
PXPrivate PXResult PXAPI PXResourceCreateHitBox(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXHitBox PXREF pxHitBox);
PXPrivate PXResult PXAPI PXResourceCreateSound(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXSound PXREF pxSound);
PXPrivate PXResult PXAPI PXResourceCreateTimer(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXTimer PXREF pxEngineTimer);
PXPrivate PXResult PXAPI PXResourceCreateWindow(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXWindow PXREF pxWindow);
PXPrivate PXResult PXAPI PXResourceCreateSpriteMap(PXResourceCreateInfo PXREF pxResourceCreateInfo, PXSpriteMap PXREF pxSpriteMap);
*/



PXPublic PXResult PXAPI PXResourceLoad(PXResourceMoveInfo PXREF pxResourceLoadInfo, const PXText PXREF filePath);
PXPublic PXResult PXAPI PXResourceLoadA(PXResourceMoveInfo PXREF pxResourceLoadInfo, const char PXREF filePath);

PXPublic PXResult PXAPI PXResourceSave(PXResourceMoveInfo PXREF pxResourceSaveInfo, const PXText PXREF filePath);
PXPublic PXResult PXAPI PXResourceSaveA(PXResourceMoveInfo PXREF pxResourceSaveInfo, const char PXREF filePath);










// Returns the global resouremanager. 
// If not yet init, do so.
//PXPublic PXResourceManager* PXAPI PXResourceManagerGet(void);

#endif