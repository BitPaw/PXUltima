#pragma once

#ifndef PXEntityIncludedd
#define PXEntityIncludedd

#include <PX/Engine/ID/PXID.h>
#include <PX/OS/File/PXFileFormat.h>





// Base object for inheritence
typedef struct PXECSCreateInfo_ PXECSCreateInfo;
typedef struct PXECSInfo_ PXECSInfo;

typedef PXResult(PXAPI* PXECSRegisterFunction)();
typedef PXResult(PXAPI* PXECSCreateFunction)(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDestroyFunction)(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDrawFunction)(PXECSInfo PXREF pxECSInfo, PXDrawInfo PXREF pxDrawInfo);


typedef enum PXECSType_
{
    PXECSTypeInvalid,
    PXECSTypeEntity, // Spawnable
    PXECSTypeComponent, // Attachable data to entity
    PXECSTypeResource, // Data independed from an entity
    PXECSTypeSystem
}
PXECSType;

PXPublic const char* PXECSTypeToString(const PXECSType pxECSType);






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














// Object data used for compile time info
// Storeing reflecting like data that does not change
typedef struct PXECSRegisterInfoStatic_
{
    PXText NameOfType;
    PXI32U TypeSize;
    PXI32U AllignedSize;
    PXECSType Type;

    PXECSCreateFunction CreateCallback;
    PXECSDestroyFunction DestroyCallback;
    PXECSDrawFunction DrawCallBack;
}
PXECSRegisterInfoStatic;

// Dynamic data that will depend on runtime
// Will be filed by system
typedef struct PXECSRegisterInfoDynamic_
{
    PXID ID;
    PXDictionary* LookupTable;
}
PXECSRegisterInfoDynamic;

typedef struct PXECSRegisterInfo_
{
    PXECSRegisterInfoStatic Static;
    PXECSRegisterInfoDynamic Dynamic;
}
PXECSRegisterInfo;


// Internal engine identification
// Additional use is to define current storage and interactions.
typedef struct PXECSInfo_
{
    //PXHierarchicalNode Hierarchy;

   // PXHandle Handle;

    PXID ID; // Identification of this object managed by the engine itself.

    PXI32U Setting; // Rendering behaviour
    PXI32U Behaviour; // Depends on the type of the resource
}
PXECSInfo;


// To prevent stale references to resources
// Use this if you reference any component in an entity or nested component
typedef struct PXECSReferece_
{
    // ID that we expect it to have. 
    // We compare this to the reference we are holding.
    // On match, reference is valid.
    // On missmatch, recalibrate the reference.
    // Note: We could use an index for beeing "safer" buuut.. 
    //       we can also directly use the adress.
    // PXSize ArrayIndex;

    // ID to be expected behind the pointer.
    // If the object has another ID, rescan.
    // If the scan yields no result, the object got deleted.
    // In that case, invalidate this struct!
    PXID ExpectedID;

    // Direct access to the component.
    // Be aware this can be stale.
    // Update before use to prevent this!
    // If adress is not in range from the container, scan 
    // for matching ID. If no ID is found, object got deleted.
    PXECSInfo* Element;
}
PXECSReferece;


typedef struct PXECSTypeInfo_
{
    PXECSRegisterInfoStatic* Static;
    PXECSRegisterInfoDynamic* Dynamic;
}
PXECSTypeInfo;





#define PXResourcePropertyPath 0
#define PXResourcePropertyName 1

#define PXResourcePropertyStore 1
#define PXResourcePropertyFetch 0

typedef struct PXECSProperty_
{ 
    // Payload
    union
    {
        PXText Text;
        PXI32S I32S;
        PXSize Size;
    };

    // Setting
    PXI16U PropertyID; // Personal ID different for every type
    PXI16U Type; // Type for payload
    PXBool DoWrite;
}
PXECSProperty;



//---------------------------------------------------------



#define PXResourceCreateBehaviourSpawnInScene   (1<<0)
#define PXResourceCreateBehaviourLoadASYNC      (1<<1)
#define PXResourceCreateBehaviourIsASYNCCall      (1<<8)


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

// Base object for inheritence.
// Info about handling on creation, peek, load and save.
typedef struct PXECSCreateInfo_
{
    //PXECSElementRef** ElementRef; // Unused for now
    PXECSInfo** ObjectReference; // Reference to an adress to be filled with an object
    PXSize ObjectAmount; // If set to more than one, "ObjectReference" will contain a list of values

    void* Parent;

    PXText FilePath;
    PXText Name;

    PXI32U Type;
    PXI32U Flags;

    // Type reflection info
    const PXECSRegisterInfoStatic* Static;
    PXECSRegisterInfoDynamic* Dynamic;

    // Callbacks
    // PXResourceFileSizePredict FileSizePredict;
    // PXResourceTransphereFunction ResourcePeek;
    // PXResourceTransphereFunction ResourceLoad;
    // PXResourceTransphereFunction ResourceSave;
    PXResourceTransphereFunction OnDeviceDataRegister;  // Preallocate resources on the device
    PXResourceTransphereFunction OnDeviceDataUpload;    // Upload data fully

    // File format when interacting with disk
    PXFileFormatInfo FormatInfo;
    PXFileFormatInfo FormatInfoExpected;    // The format detected by the resource loader
    PXFile* FileCurrent;

    // Stats for benchmarking
    PXF32 TimePeek;
    PXF32 TimeTransphere;
    PXF32 TimeDeviceDataRegister;
    PXF32 TimeDeviceDataUpload;
}
PXECSCreateInfo;

// To create the base object for an ECS thing.
// 1. Spawn object reference, if type does not exist yield error

PXPublic PXResult PXAPI PXECSInit(void);
PXPublic PXResult PXAPI PXECSCreate(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);

PXPublic PXSize PXAPI PXECSTypeAmount(void);
PXPublic PXResult PXAPI PXECSTypeGet(PXECSTypeInfo PXREF pxECSTypeInfo, const PXSize index);
PXPublic PXSize PXAPI PXECSEntryAmount(void);
PXPublic PXResult PXAPI PXECSEntryGet(PXECSReferece PXREF pxECSReferece, const PXSize index);
//---------------------------------------------------------

PXPublic PXResult PXAPI PXECSLoad(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXECSSave(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);


// Purpose: Register types for the ECS so that we can spawn 
// them as or add them as components to entities
PXPublic PXResult PXAPI PXECSRegister
(
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
);






PXPublic PXResult PXAPI PXECSElementToString
(
    PXText PXREF pxText,
    const PXECSInfo PXREF pxECSInfo,
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    const PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
);

// Set property values for an object.
// Names can always be set, some only if object supports it.
PXPublic PXResult PXAPI PXECSPropertyIO(PXECSProperty PXREF pxECSProperty);



//---------------------------------------------------------
// Component
//---------------------------------------------------------

// Check if a reference has gotten stale
PXPublic PXResult PXAPI PXECSElementRefCheck(PXDictionary PXREF pxDictionary, PXECSReferece PXREF pxECSReferece);
//---------------------------------------------------------





#endif