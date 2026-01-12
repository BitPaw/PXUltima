#pragma once

#ifndef PXEntityIncludedd
#define PXEntityIncludedd

//#include <PX/Engine/PXResource.h>
#include <PX/Container/List/PXList.h>
#include <PX/Media/PXText.h>
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Engine/ID/PXID.h>
#include <PX/OS/File/PXFileFormat.h>


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

// Base object for inheritence
typedef struct PXECSCreateInfo_ PXECSCreateInfo;
typedef struct PXECSInfo_ PXECSInfo;

typedef PXResult(PXAPI* PXECSRegisterFunction)();
typedef PXResult(PXAPI* PXECSCreateFunction)(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDestroyFunction)(PXECSInfo PXREF pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);
typedef PXResult(PXAPI* PXECSDrawFunction)(PXECSInfo PXREF pxWindow, struct PXWindowDrawInfo_ PXREF pxWindowDrawInfo);

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
typedef struct PXECSRegisterInfoDynamic_
{
    // Filed by system
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



// Purpose: Register types for the ECS so that we can spawn 
// them as or add them as components to entities
PXPublic PXResult PXAPI PXECSRegister
(
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
);








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









PXPublic PXResult PXAPI PXECSElementToString
(
    PXText PXREF pxText, 
    const PXECSInfo PXREF pxECSInfo,
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    const PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
);





// To prevent stale references to resources
// Use this if you reference any component in an entity or nested component
typedef struct PXECSElementRef_
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

    union
    {
        PXECSInfo* Element;
        //PXComponent* Component;
        //PXComponent* Component;
        //PXComponent* Component;
    };
}
PXECSElementRef;



/*


// Something that can exist in the scene. A sprite, skybox, ...
typedef struct PXEntity_
{
    PXRegistrationInfo RegistrationInfo;

    // Data?
}
PXEntity;

// Components can be a mesh, position and rotation, 
typedef struct PXComponent_
{
    PXRegistrationInfo RegistrationInfo;

    // Payload??
}
PXComponent;

// Holds combination of assets
// When we load a file that has chained files attached, this is the object.
typedef struct PXResource_
{
    PXRegistrationInfo RegistrationInfo;
}
PXResource;



// Holds data stored on disk or similaroo
typedef struct PXAsset_
{
    PXRegistrationInfo RegistrationInfo;

    // Payload??
}
PXAsset;



*/



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
typedef struct PXResourceMoveInfo_
{
    void* Owner;                // Who is the caller?
    void* ResourceTarget;       // Generic object, tager
    // PXResourceManager* Manager; // The callback manager. This is set by the resource loader itself. Used for chain dependencys
    PXFile* FileReference;      // The attached file that hold the data

    //PXResourceFileSizePredict FileSizePredict;
    //PXResourceTransphereFunction ResourcePeek;
    //PXResourceTransphereFunction ResourceLoad;
    //PXResourceTransphereFunction ResourceSave;

    PXResourceTransphereFunction OnDeviceDataRegister;  // Preallocate resources on the device
    PXResourceTransphereFunction OnDeviceDataUpload;    // Upload data fully

    void* ResourceSource;
    PXI32U ResourceType;  // Type of the resource that 'Target' points to. Example: Image, Sound, Video...

    void* ResourceLoadContainer; // Used to store load/Store spesific helper object

    PXFileFormatInfo FormatInfo;
    PXFileFormatInfo FormatInfoExpected;        // The format detected by the resource loader

    PXI8U Flags;

    //void* Owner;

    PXF32 TimePeek;
    PXF32 TimeTransphere;
    PXF32 TimeDeviceDataRegister;
    PXF32 TimeDeviceDataUpload;
}
PXResourceMoveInfo;


























//---------------------------------------------------------



//---------------------------------------------------------
// Component
//---------------------------------------------------------




//---------------------------------------------------------




//---------------------------------------------------------
// Component registion system
//---------------------------------------------------------

// List containing one type of component.
// Use "PXComponentRef" class for references to elements
typedef struct PXComponentType_
{
    PXID ID;
    PXDictionary* Lookup; // Ref to a lookuptable
}
PXComponentType;


// This is a registration info of a component.
// 3rd partys can add their own type with this
typedef struct PXComponentInfo_
{
    PXID ID;
    PXText Name; // Whats the name of the component?
    //PXComponent* Component; // 
}
PXComponentInfo;


PXPublic PXResult PXAPI PXECSInit(void);






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

// Set property values for an object.
// Names can always be set, some only if object supports it.
PXPublic PXResult PXAPI PXECSPropertyIO(PXECSProperty PXREF pxECSProperty);



//---------------------------------------------------------
// Component
//---------------------------------------------------------
PXPublic PXResult PXAPI PXECSComponentRegister(PXComponentType PXREF pxEntity);

// Check if a reference has gotten stale
PXPublic PXResult PXAPI PXECSElementRefCheck(PXDictionary PXREF pxDictionary, PXECSElementRef PXREF pxECSElementRef);
//---------------------------------------------------------


//---------------------------------------------------------
// Entity-Component
//---------------------------------------------------------
//PXPublic PXResult PXAPI PXECSEntityComponentGet(PXEntity PXREF pxEntity, PXComponent** component);
//PXPublic PXResult PXAPI PXECSEntityComponentAdd(PXEntity PXREF pxEntity, PXComponent** component);
//---------------------------------------------------------


//---------------------------------------------------------
// Resource
//---------------------------------------------------------
//typedef struct PXECSInfo_
//{
 //   int x;
//}
//;// PXECSInfo;

//PXPublic PXResult PXAPI PXECSResourceAdd(PXEntity PXREF pxEntity, PXComponent** component);
//---------------------------------------------------------



//---------------------------------------------------------
// Asset
//---------------------------------------------------------
typedef struct PXAssetInfo_
{
    int x;
}
PXAssetInfo;




#define PXResourceCreateBehaviourSpawnInScene   (1<<0)
#define PXResourceCreateBehaviourLoadASYNC      (1<<1)
#define PXResourceCreateBehaviourIsASYNCCall      (1<<8)




// Base object for inheritence
typedef struct PXECSCreateInfo_
{
    PXECSElementRef** ElementRef;
    PXECSInfo** ObjectReference; // Reference to an adress to be filled with an object
    PXSize ObjectAmount; // If set to more than one, "ObjectReference" will contain a list of values

    void* Parent;

    PXText FilePath;
    PXText Name;

    PXI32U Type;
    PXI32U Flags;

    const PXECSRegisterInfoStatic* Static;
    PXECSRegisterInfoDynamic* Dynamic;
}
PXECSCreateInfo;

// To create the base object for an ECS thing.
// 1. Spawn object reference, if type does not exist yield error
PXPublic PXResult PXAPI PXECSCreate(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo);

//PXPublic PXResult PXAPI PXECSAssetAdd(PXECSRegistrationInfo PXREF pxECSRegistrationInfo);
//---------------------------------------------------------


#endif