#include "PXECS.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/Engine/ECS/PXNamePool.h>

#include <PX/OS/PXOS.h>


// Resources
#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>
#include <PX/Engine/ECS/Entity/SkyBox/PXSkyBox.h>
#include <PX/Engine/ECS/Entity/SpaceGrid/PXSpaceGrid.h>
#include <PX/Engine/ECS/Entity/UI/ECSEntityInfo/PXECSEntityInfo.h>
#include <PX/Engine/ECS/Entity/UI/TabList/PXTabList.h>
#include <PX/Engine/ECS/Entity/UI/TransformView/PXTransformView.h>
#include <PX/Engine/ECS/Entity/UI/FileDirectory/PXFileDirectory.h>
#include <PX/Engine/ECS/Entity/UI/ColorPicker/PXColorPicker.h>
#include <PX/Engine/ECS/Resource/Brush/PXBrush.h>
#include <PX/Engine/ECS/Resource/Display/PXDisplay.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>
#include <PX/Engine/ECS/Resource/FrameBuffer/PXFrameBuffer.h>
#include <PX/Engine/ECS/Resource/Icon/PXIcon.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <PX/Engine/ECS/Resource/Monitor/PXMonitor.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Engine/ECS/Resource/Sound/PXSound.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Engine/ECS/Resource/Timer/PXTimer.h>
#include <PX/Engine/ECS/Resource/Video/PXVideo.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Engine/ECS/Entity/SkyBox/PXSkyBox.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Container/ListDynamic/PXListDynamic.h>


// Components

// Assets?

// Private class
// Stores all components
// Additional lookup and managing of entity to component relation
typedef struct PXECS_
{
    // To know what types we know
    PXDictionaryT(PXID, PXComponentType*)* ComponentTypeLookup;

    // Entity to Component
    PXDictionaryT(PXID, PXComponent*)* ComponentLookup;

    // For multithreading we need protection
    PXLock* ResourceLock;

    PXLock* AsyncLock;

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

    PXI32U Flags;
}
PXECS;

const char PXECSText[] = "ECS-PX";

PXECS _pxECS;

const PXECSRegisterFunction pxECSRegisterList[] =
{
    PXCameraRegisterToECS,
    PXModelRegisterToECS,
    PXColorPickerRegisterToECS,
    PXTabListRegisterToECS,
    PXSkyBoxRegisterToECS,
    PXECSEntityInfoRegisterToECS,
    PXSpaceGridRegisterToECS,
    PXTransformViewRegisterToECS,
    PXFileDirectoryRegisterToECS,
    PXLockRegisterToECS,
    PXThreadRegisterToECS,
    PXBrushRegisterToECS,
    PXFontRegisterToECS,
    PXFrameBufferRegisterToECS,
    PXIconRegisterToECS,
    PXMeshRegisterToECS,
    PXSkyBoxRegisterToECS,
    PXWindowRegisterToECS
};
const PXI16U _pxECSRegisterListAmount = sizeof(pxECSRegisterList) / sizeof(PXECSRegisterFunction);

const char* PXECSTypeToString(const PXECSType pxECSType)
{
    switch(pxECSType)
    {
        case PXECSTypeInvalid:  return "Invalid";
        case PXECSTypeEntity:   return "Enity";
        case PXECSTypeComponent:    return "Component";
        case PXECSTypeResource: return "Resource";
        case PXECSTypeSystem: return "System";

        default:
            return 0;
    }
}

PXResult PXAPI PXECSRegister
(
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
)
{
    // We need a uniqe ID to register
    pxECSRegisterInfoDynamic->ID = PXIDGenerate();

    // We cant trust the pointer from beeing safe to store
    // It must be stored seperately
    PXNamePoolStore
    (
        pxECSRegisterInfoDynamic->ID, 
        &pxECSRegisterInfoStatic->NameOfType,
        &pxECSRegisterInfoStatic->NameOfType
    );

    PXAssert(pxECSRegisterInfoStatic->TypeSize > sizeof(PXECSInfo), "Used ECS object is missing info");


#if PXLogEnable
    char buffer[128];
    PXText pxTextModule;
    PXTextFromAdressA(&pxTextModule, buffer, 0, sizeof(buffer));

    PXLibraryNameFromAdress(PXNull, &pxTextModule, pxECSRegisterInfoStatic->CreateCallback);

    const char* typeName = PXECSTypeToString(pxECSRegisterInfoStatic->Type);

    PXLogPrint
    (
        PXLoggingAllocation,
        PXECSText,
        "Register",
        "0d%4.4i_%s:%s_%i, %s",
        pxECSRegisterInfoDynamic->ID,
        pxTextModule.A,
        pxECSRegisterInfoStatic->NameOfType.A,
        pxECSRegisterInfoStatic->TypeSize,
        typeName
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXECSElementToString
(
    PXText PXREF pxText,
    const PXECSInfo PXREF pxECSInfo,
    const PXECSRegisterInfoStatic PXREF pxECSRegisterInfoStatic,
    const PXECSRegisterInfoDynamic PXREF pxECSRegisterInfoDynamic
)
{
    char buffer[260];
    PXText pxTextModule;
    PXTextFromAdressA(&pxTextModule, buffer, 0, sizeof(buffer));

    PXLibraryNameFromAdress(PXNull, &pxTextModule, pxECSRegisterInfoStatic->CreateCallback);

    // Combine string
    PXTextPrint
    (
        pxText,
        "0d%4.4i_%s:%s_%i",
        pxECSRegisterInfoDynamic->ID,
        pxTextModule.A,
        pxECSRegisterInfoStatic->NameOfType.A,
        pxECSRegisterInfoStatic->TypeSize
    );

    //0x0123ABCD_PXUltima:PXFont_28_8
    // PXID_Module:TypeName_Size_Elements?


    return PXResultOK;
}

PXResult PXAPI PXECSInit(void)
{
    PXClear(PXECS, &_pxECS);

    PXListDynamicInit(&_pxECS.NameCache, sizeof(PXID), 2);
    PXListDynamicInit(&_pxECS.SourcePathCache, sizeof(PXID), 2);

    PXNamePoolInit();

    // Register all entitys
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXECSText,
        "Register",
        "Predefined types - Amount:<%i>",
        _pxECSRegisterListAmount
    );
#endif

    PXSize failCounter = 0;

    for(PXSize i = 0; i < _pxECSRegisterListAmount; ++i)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            PXECSText,
            "Register",
            "[%i/%i]",
            i+1,
            _pxECSRegisterListAmount
        );
#endif

        PXResult pxResult = pxECSRegisterList[i]();

        if(PXResultOK != pxResult)
        {
            ++failCounter;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Register",
                "[%i/%i] Register type FAILED",
                i + 1,
                _pxECSRegisterListAmount
            );
#endif
        }
    }

    PXSize success = _pxECSRegisterListAmount - failCounter;
    PXSize successRate = 100 * (success / _pxECSRegisterListAmount);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXECSText,
        "Register",
        "<%i>/<%i> OK (%i)",
        success,
        _pxECSRegisterListAmount,
        successRate
    );
#endif

    PXLockCreateInfo pxLockCreateInfo;
    PXClear(PXLockCreateInfo, &pxLockCreateInfo);
    pxLockCreateInfo.Type = PXLockTypeProcessOnly;

    PXLockCreate(&_pxECS.ResourceLock, &pxLockCreateInfo);

    return PXResultOK;
}

PXResult PXAPI PXECSPropertyIO(PXECSProperty PXREF pxECSProperty)
{
    // NULL Check
    if(!pxECSProperty)
    {
        return PXResultRefusedParameterNull;
    }

    if(pxECSProperty->DoWrite)
    {
        const PXBool callValid = 0;// name && !(0 == nameSize || (PXSize)-1 == nameSize);

        if(!callValid)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Store-Name",
                "PXID:%i, Name can't be stored!",
                0//PXECSInfo->ID
            );
#endif

            return PXResultRefusedParameterInvalid;
        }


        PXSize length = 0;// nameSize;

        //if(-1 == nameSize)
        {
            length = 0;// PXTextLengthA(name, nameSize);
        }

#if PXLogEnable && 0
        char buffer[256];

        //  PXTextCopyA(name, nameSize, buffer, length);

        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceText,
            "Store-Name",
            "PXID:%i, %s (%i)",
            0//PXECSInfo->ID,
            buffer,
            length
        );
#endif

        //PXListDynamicAdd(&_pxResourceManager.NameCache, &PXECSInfo->ID, name, length);

       // PXECSInfo->Behaviour |= PXECSInfoHasName;

        return PXResultOK;
    }

    return PXResultInvalid;
}

PXResult PXAPI PXECSElementRefCheck(PXDictionary PXREF pxDictionary, PXECSElementRef PXREF pxECSElementRef)
{
    if(!(pxDictionary && pxECSElementRef)) // Is this NULL?
    {
        return PXResultRefusedParameterNull; // Illegal call
    }

    // Is adress in data range?
    PXECSInfo* pxECSElement = pxECSElementRef->Element;

    const PXBool isInRange = PXListIsAddresValid(&pxDictionary->List, pxECSElementRef->Element);

    if(isInRange)
    {
        // Because adress is valid, we can check now
        const PXBool isDirectMatch = pxECSElementRef->ExpectedID == pxECSElement->ID;

        if(isDirectMatch)
        {
            // Pointer is valid as expected and does not need to be updated
            return PXResultOK;
        }

        // Pointer is in a valid range but seems to be moved, we need to search..
    }

    // [STALE REFERENCE DETECTED]
    // Search to detect valid object


    PXResult pxResult = PXDictionaryEntryFind
    (
        pxDictionary, 
        &pxECSElementRef->ExpectedID, 
        &pxECSElementRef->Element
    );

    if(PXResultOK != pxResult)
    {
        // Not found, object was deleted

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXECSText,
            "Ref-Check",
            "PXID:%i, stale reference detected! Object was deleted",
            pxECSElementRef->ExpectedID
        );
#endif

        return pxResult;
    }

    // We found the object!
    // Reference got updated to new object
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXECSText,
        "Ref-Check",
        "PXID:%i, stale reference detected! Object was moved!",
        pxECSElementRef->ExpectedID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXECSCreate(PXECSInfo** pxECSInfoREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    // NULL Parameter safequard 
    if(!(pxECSInfoREF && pxECSCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    // Is the size of the object valid? 
    // A size of 0 is not possible.
    // Also a size smaler then "PXECSInfo" means the dev forgot to add the base object.
    const PXSize typeSize = pxECSCreateInfo->Static->TypeSize;
    const PXBool hasValidSize = sizeof(PXECSInfo) < typeSize;

    if(!hasValidSize)
    {
        return PXActionRefusedObjectSizeZero;
    }

    // Create ID
    PXID pxID = PXIDGenerate();

    // Get memory?
    PXECSInfo* pxECSInfo = (PXECSInfo*)PXMemoryHeapCalloc(PXNull, 1, typeSize);

    // Allocations can fail, safely eject
    if(!pxECSInfo)
    {
        return PXActionFailedMemoryAllocation;
    }

    pxECSInfo->ID = pxID;

    *pxECSInfoREF = pxECSInfo;

    // Register?

#if PXLogEnable
    char name[128];
    PXText pxTextName;
    PXTextFromAdressA(&pxTextName, name, 0, sizeof(name));

    PXECSElementToString(&pxTextName, pxECSInfo, pxECSCreateInfo->Static, pxECSCreateInfo->Dynamic);

    PXLogPrint
    (
        PXLoggingAllocation,
        PXECSText,
        "Create",
        "%s (PXID:%4i, %s)",
        name,
        pxID,
        pxECSCreateInfo->Name.A
    );
#endif 

    return PXResultOK;
}

/*
PXResult PXAPI PXECSAssetAddaaaaa(PXECSRegistrationInfo PXREF pxECSRegistrationInfo)
{
    if(!pxResourceCreateInfo)
    {
        return PXResultRefusedParameterNull;
    }

    //-----------------------------------------------------
    // Pre-create Object
    //-----------------------------------------------------

    // TODO: Bad provider funcion!
    const PXResourceEntry PXREF pxResourceEntry = PXResourceEntryGet(pxResourceCreateInfo->Type);

    if(!pxResourceEntry)
    {
        return PXResultRefusedParameterInvalid;
    }

    // fixing text
    {
        pxResourceCreateInfo->Name.SizeAllocated = 0xFF;
        pxResourceCreateInfo->FilePath.SizeAllocated = 0xFF;
        PXTextRevise(&pxResourceCreateInfo->Name);
        PXTextRevise(&pxResourceCreateInfo->FilePath);
    }


    const PXBool isResourceAllocated = *pxResourceCreateInfo->ObjectReference > 0;

    if(!isResourceAllocated)
    {
        // Validation
        {
            if(!pxResourceEntry->LookupTable)
            {
                // TODO: Add behaviour to handle missing lookup

            }
        }

        PXLockEngage(_pxResourceManager.AsyncLock, PXTrue);

        // Special behaviour if we have an object size of 0.
        // Creating 0 objects does not make sense but if we dont set this, it will yield problems
        // To reduce useless definition of "1 object size", we assume 0 means we want one item
        if(pxResourceCreateInfo->ObjectAmount == 0)
        {
            ++pxResourceCreateInfo->ObjectAmount; // Asumme we want one item it we didnt set it to it,
        }

        PXResourceEntry* pxResourceEntryList = (PXResourceEntry*)PXMemoryHeapCalloc(PXNull, pxResourceEntry->TypeSize, pxResourceCreateInfo->ObjectAmount);

        if(!pxResourceEntryList)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Register",
                "Failed to allocate space for : Size:%-4i (%i) <%s>",
                pxResourceEntry->TypeSize,
                pxResourceCreateInfo->ObjectAmount,
                pxResourceEntry->Name
            );
#endif

            return PXActionFailedMemoryAllocation;
        }

        *pxResourceCreateInfo->ObjectReference = pxResourceEntryList;

        for(PXSize i = 0; i < pxResourceCreateInfo->ObjectAmount; ++i)
        {
            const PXI32U resourceID = PXIDGenerate();

            // Get currect objects is we have multible
            void* object = (char*)pxResourceEntryList + (pxResourceEntry->TypeSize * i);

            // UNSTANBLE CAST?
            PXECSInfo PXREF PXECSInfo = (PXECSInfo*)object;
            PXECSInfo->ID = resourceID;
            PXECSInfo->Behaviour |= PXECSInfoExist;

            // Store myself, so we can cast back with hirachy
            PXECSInfo->Hierarchy.Yourself = object;

            PXDictionaryEntryAdd(pxResourceEntry->LookupTable, &resourceID, *pxResourceCreateInfo->ObjectReference);

            PXECSProperty pxResourceProperty;
            PXClear(PXECSProperty, &pxResourceProperty);
            pxResourceProperty.Text = pxResourceCreateInfo->Name;
            PXECSPropertyIO(PXECSInfo, &pxResourceProperty, PXResourcePropertyName, PXTrue);

            PXClear(PXECSProperty, &pxResourceProperty);
            pxResourceProperty.Text = pxResourceCreateInfo->FilePath;
            PXECSPropertyIO(PXECSInfo, &pxResourceProperty, PXResourcePropertyPath, PXTrue);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXECSText,
                "Register",
                "PXID:<%-4i> Size:%-4i (%i/%i) <%s>",
                resourceID,
                pxResourceEntry->TypeSize,
                i + 1,
                pxResourceCreateInfo->ObjectAmount,
                pxResourceEntry->Name
            );
#endif
        }

        PXLockRelease(_pxResourceManager.AsyncLock);



        //-----------------------------------------------------
        // Create ASYNC pathway
        //-----------------------------------------------------
        const PXBool createSubCall =
            (PXResourceCreateBehaviourLoadASYNC & pxResourceCreateInfo->Flags) &&
            !(PXResourceCreateBehaviourIsASYNCCall & pxResourceCreateInfo->Flags);

        if(createSubCall && 0)
        {
            // We want to load the resouce in an async way.
            // Start a thread and call this function again?

            PXResourceCreateInfo* pxResourceCreateInfoASYNC = PXMemoryHeapCallocT(PXResourceCreateInfo, 1);
            PXCopy(PXResourceCreateInfo, pxResourceCreateInfo, pxResourceCreateInfoASYNC);
            pxResourceCreateInfoASYNC->Flags |= PXResourceCreateBehaviourIsASYNCCall;

            PXThreadPoolQueueWork
            (
                PXNull,
                0,
                //PXResourceManagerAdd,
                pxResourceCreateInfoASYNC,
                PXNull,
                PXTaskParameterRelease | PXTaskExecuteASYNC
            );

            return PXResultOK;
        }
    }
    //-----------------------------------------------------

    // Invoke creation
    if(!pxResourceEntry->CreateFunction)
    {
        // If there is no handle function, we cant do anything. 
        // We regard this as an error
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXECSText,
            "Register",
            "[PXID:%-4i] <%s> Has no function to handle creation.",
            -1,
            pxResourceEntry->Name
        );
#endif

        return PXActionRefusedObjectCreateCallbackMissing;
    }

    const PXResult pxActionCreate = pxResourceEntry->CreateFunction(pxResourceCreateInfo, *pxResourceCreateInfo->ObjectReference);

    if(PXResultOK != pxActionCreate)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXECSText,
            "Create",
            "[PXID:%-4i] <%s> failed creation.",
            -1,
            pxResourceEntry->Name
        );
#endif
    }

    return PXResultOK;
}

PXResult PXAPI //PXResourceManagerAddV(PXResourceCreateInfo PXREF pxResourceCreateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXResourceCreateInfo PXREF pxResourceCreateInfo = &pxResourceCreateInfoList[i];

        //PXResourceManagerAdd(pxResourceCreateInfo);
    }

    return PXResultOK;
}*/