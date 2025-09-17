#ifndef PXEntityIncludedd
#define PXEntityIncludedd

//#include <PX/Engine/PXResource.h>
#include <PX/Container/List/PXList.h>
#include <PX/Media/PXText.h>
#include <PX/Container/Dictionary/PXDictionary.h>

typedef PXI32U PXEntityID;
typedef PXI32U PXComponentID;

// Something that can exist in the scene. A sprite, skybox, ...
typedef struct PXEntity_
{
    PXEntityID ID;
}
PXEntity;

// Components can be a mesh, position and rotation, 
typedef struct PXComponent_
{
    PXComponentID ID;
}
PXComponent;

#define PXComponentInvalidArrayIndex (PXSize)-1

// To prevent stale references to resources
typedef struct PXComponentRef_
{
    // ID that we expect it to have. 
    // We compare this to the reference we are holding.
    // On match, reference is valid.
    // On missmatch, recalibrate the reference.
    PXSize ArrayIndex;

    // ID genereated by the component, primary key of system
    PXComponentID ExpectedID;

    // Be aware this can be stale.
    // Update before use to prevent this!
    PXComponent* Component;
}
PXComponentRef;


// List containing one type of component.
// Use "PXComponentRef" class for references to elements
typedef struct PXComponentList_
{
    PXDictionary Lookup;
    PXI32U TypeID;

    PXI32U UniqeCounter;
}
PXComponentList;


typedef struct PXComponentManager_
{
    PXDictionaryT(PXI32U, PXComponentList) ComponentLookup;
    PXI32U UniqeCounter;
}
PXComponentManager;


// Info to create
typedef struct PXComponentInfo_
{
    PXText Name;
    PXComponentID ID;
    PXComponent* Component;
}
PXComponentInfo;

PXPublic void PXAPI PXComponentRefCheck(PXComponentRef* const pxComponentRef, PXComponentList* const pxComponentList);

PXPublic void PXAPI PXComponentManagerAdd(PXComponentManager* const pxComponentManager, PXComponentInfo* const pxComponentInfo);











PXPublic void PXAPI PXEntityComponentGet(PXEntity* const pxEntity, void* component);


#endif