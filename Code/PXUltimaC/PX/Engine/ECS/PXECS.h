#pragma once

#ifndef PXEntityIncludedd
#define PXEntityIncludedd

#include <PX/Engine/PXID.h>
#include <PX/OS/File/PXFileFormat.h>

// Base object for inheritence
typedef struct PXECSCreateInfo_ PXECSCreateInfo;
typedef struct PXECSInfo_ PXECSInfo;
typedef struct PXECSRegisterInfo_ PXECSRegisterInfo;

typedef void(PXAPI* PXECSRegisterFunction)(PXECSRegisterInfo PXREF pxECSRegisterInfo);
typedef PXResult(PXAPI* PXECSCreateFunction)(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDestroyFunction)(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDrawFunction)(PXECSInfo PXREF pxECSInfo, PXDrawInfo PXREF pxDrawInfo);


typedef enum PXECSType_
{
    PXECSTypeInvalid,
    PXECSTypeEntity, // Spawnable
    PXECSTypeComponent, // Attachable data to entity
    PXECSTypeResource, // Data independed from an entity
    PXECSTypeSystem,
    PXECSTypeContainer
}
PXECSType;

PXPublic const char* PXECSTypeToString(const PXECSType pxECSType);




//------------------------------------------------------------------------
// How is it? 4-bits
#define PXECSFlagStateMask              0b00000000000000000000000000001111
#define PXECSFlagStateINVALID           0b00000000000000000000000000000000 // Resource does not exist. Not created or deleted
#define PXECSFlagStateINITIALIZING      0b00000000000000000000000000000001 // Resource is in its init phase
#define PXECSFlagStateREADY             0b00000000000000000000000000000010 // Resource is OK and ready to be used
#define PXECSFlagStateRUNNING           0b00000000000000000000000000000011 // Resource is currently running
#define PXECSFlagStateWAITING           0b00000000000000000000000000000100 // Waits for another resource
#define PXECSFlagStateSUSPENDED         0b00000000000000000000000000000101 // Start in a stopped state or was stopped 
#define PXECSFlagStateCOMPLETED         0b00000000000000000000000000000110 // Processing done, result can be fetched
#define PXECSFlagStateFAILED            0b00000000000000000000000000000111 // Was killed by crash or intentionally.
#define PXECSFlagStateMARKED_FOR_DELETE 0b00000000000000000000000000001000 // Marked for deleting
#define PXECSFlagStateDELETED           0b00000000000000000000000000001011 // Resource deleted
#define PXECSFlagStateUNLOADED          0b00000000000000000000000000001010
#define PXECSFlagStateLOADING           0b00000000000000000000000000001001
#define PXECSFlagStateSTALE             0b00000000000000000000000000001110
#define PXECSFlagStateRECOVERING        0b00000000000000000000000000001111
#define PXECSFlagStateDISABLED          0b00000000000000000000000000001100
#define PXECSFlagStateRESERVED          0b00000000000000000000000000001101
#define PXECSFlagStateRESERVED0         0b00000000000000000000000000001110
#define PXECSFlagStateRESERVED1         0b00000000000000000000000000001111
//------------------------------------------------------------------------



//------------------------------------------------------------------------
// Permission - 4-bit
#define PXECSInfoPermissionMask         0b00000000000000000000000001110000
#define PXECSInfoPermissionREAD         0b00000000000000000000000000010000
#define PXECSInfoPermissionWRITE        0b00000000000000000000000000100000
#define PXECSInfoPermissionEXECUTE      0b00000000000000000000000001000000

#define PXECSFlagStateDirty             0b00000000000000000000000010000000 // Needs refresh on updated data
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Where stored
#define PXECSFlagStorageMask            0b00000000000000000000111100000000
#define PXECSFlagStorageDrive           0b00000000000000000000000100000000 // Resource is in permanent storage
#define PXECSFlagStorageCached          0b00000000000000000000001000000000 // Resource is in semi-permanent cache (temp file)
#define PXECSFlagStorageMemory          0b00000000000000000000010000000000 // Resource exists in RAM
#define PXECSFlagStorageDevice          0b00000000000000000000100000000000 // Resource exists in spesific device
//------------------------------------------------------------------------
// Who created?
#define PXECSInfoUseByMask              0b00000000000000001111000000000000
#define PXECSInfoUseByOS                0b00000000000000000001000000000000
#define PXECSInfoUseByUser              0b00000000000000000010000000000000
#define PXECSInfoUseByEngine            0b00000000000000000100000000000000
#define PXECSInfoUseByUndefined         0b00000000000000001000000000000000
//------------------------------------------------------------------------
#define PXECSFlagBehaviourisOK          0b00000000000000010000000000000000 // ..?
#define PXECSFlagBehaviourEnabled       0b00000000000000010000000000000000 // Not update but state of self, like ON/OFF
#define PXECSFlagBehaviourActive        0b00000000000000100000000000000000 // Participates in updates
#define PXECSFlagBehaviourCanRender     0b00000000000001000000000000000000
#define PXECSFlagBehaviourHidden        0b00000000000010000000000000000000 // Can or will be rendered
//------------------------------------------------------------------------
#define PXECSFlagBehaviourSelectable    0b00000000000100000000000000000000 // 
#define PXECSFlagBehaviourSelected      0b00000000001000000000000000000000
#define PXECSFlagBehaviourTARGETABLE    0b00000000010000000000000000000000 // 
#define PXECSFlagBehaviourTargeted      0b00000000100000000000000000000000
//------------------------------------------------------------------------
#define PXECSFlagBehaviourPERSISTENT    0b00000001000000000000000000000000 // Entity survives scene changes
#define PXECSFlagBehaviourINTERACTABLE  0b00000010000000000000000000000000
#define PXECSFlagBindingIsKnown         0b00000100000000000000000000000000
#define PXECSFlagBindingIsOK            0b00001000000000000000000000000000
//------------------------------------------------------------------------
// Has name, source path?
#define PXECSFlagNameMask               0b00010000000000000000000000000000
#define PXECSFlagHasName                0b00100000000000000000000000000000 // Is the name stored
#define PXECSFlagHasSource              0b01000000000000000000000000000000 // Is resource loaded from a path, if yes, path is cached.
#define PXECSFlagUndefinedA             0b10000000000000000000000000000000
//------------------------------------------------------------------------



//#define PXECSInfoConstData PXECSInfoPermissionREAD | PXECSInfoStorageMemory | PXECSInfoExist | PXECSInfoActive | PXECSInfoRender

//#define PXECSInfoOK              PXECSInfoExist | PXECSInfoActive | PXECSInfoRender
//#define PXECSInfoNoRender              PXECSInfoExist | PXECSInfoActive


typedef PXI32U PXECSFlagList;



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
    const PXECSRegisterInfoStatic* InfoStatic;
    PXECSRegisterInfoDynamic* InfoDynamic;
}
PXECSRegisterInfo;

#if 0
typedef struct PXECSRegisterInfo_
{
    PXECSRegisterInfoStatic Static;
    PXECSRegisterInfoDynamic Dynamic;
}
PXECSRegisterInfo;

typedef struct PXECSTypeInfo_
{
    PXECSRegisterInfoStatic* Static;
    PXECSRegisterInfoDynamic* Dynamic;
}
PXECSTypeInfo;
#endif




// Internal engine identification
// Additional use is to define current storage and interactions.
typedef struct PXECSInfo_
{
    PXID ID; // Identification of this object managed by the engine itself.

    PXECSFlagList FlagListState;
    PXECSFlagList FlagListSettings;
}
PXECSInfo;

PXPublic void PXAPI PXECSInfoIDGenerate(PXECSInfo PXREF pxECSInfo);

PXPublic void PXAPI PXECSInfoFlagStateAdd(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic void PXAPI PXECSInfoFlagStateRemove(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic PXBool PXAPI PXECSInfoFlagStateCheck(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);

PXPublic void PXAPI PXECSInfoFlagSettingsAdd(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic void PXAPI PXECSInfoFlagSettingsRemove(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic PXBool PXAPI PXECSInfoFlagSettingsCheck(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);




typedef PXResult(PXAPI* PXECSEventCallBack)(PXECSInfo PXREF pxECSInfo);

// Intereted, we need a callback so we can reverse this.
typedef struct PXECSEventInfo_
{
    PXECSInfo Info;

    PXECSEventCallBack EventCallBack;
}
PXECSEventInfo;


PXPublic PXI8U PXAPI PXECSInfoStateEqual(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic PXI8U PXAPI PXECSInfoStateGet(const PXECSInfo PXREF pxECSInfo);
PXPublic void PXAPI PXECSInfoStateSet(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList);
PXPublic const char* PXAPI PXExecuteStateToString(const PXI32U behaviour);


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
PXPublic PXResult PXAPI PXECSTypeGet(PXECSRegisterInfo PXREF pxECSTypeInfo, const PXSize index);
PXPublic PXSize PXAPI PXECSEntryAmount(void);
PXPublic PXResult PXAPI PXECSEntryGet(PXECSReferece PXREF pxECSReferece, const PXSize index);
//---------------------------------------------------------

PXPublic PXResult PXAPI PXECSLoad(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXECSSave(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo);


// Purpose: Register types for the ECS so that we can spawn 
// them as or add them as components to entities
PXPublic PXResult PXAPI PXECSRegister(PXECSRegisterInfo PXREF pxECSRegisterInfo);


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
PXPublic PXResult PXAPI PXECSRefCheck(PXDictionary PXREF pxDictionary, PXECSReferece PXREF pxECSReferece);
//---------------------------------------------------------

#endif