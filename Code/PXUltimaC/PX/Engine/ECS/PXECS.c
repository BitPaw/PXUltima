#include "PXECS.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXLock.h>


// Private class
// Stores all components
// Additional lookup and managing of entity to component relation
typedef struct PXComponentRegistry_
{
    // To know what types we know
    PXDictionaryT(PXID, PXComponentType*)* ComponentTypeLookup;

    // Entity to Component
    PXDictionaryT(PXID, PXComponent*)* ComponentLookup;

    PXLock* ResourceLock;
}
PXComponentRegistry;





PXComponentRegistry _pxComponentRegistry;

PXResult PXAPI PXECSInit(void)
{
    PXClear(PXComponentRegistry, &_pxComponentRegistry);

    PXLockCreate(&_pxComponentRegistry.ResourceLock, PXLockTypeProcessOnly);





    PXResourceCreateInfo pxResourceCreateInf;


    // Register Asserts
    PXClear(PXResourceCreateInfo, &pxResourceCreateInf);
    PXTextFromAdressA(&pxResourceCreateInf.FilePath, "Sound", );
    PXECSAssetAdd(&pxResourceCreateInf);

    // Register resources


    // Register components
 



    const PXResourceEntry _GlobalResourceEntryList[] =
    {
        {PXNull,                                PXNull,                                 "Invalid", PXResourceTypeInvalid, 0 },
        {PXNull,                                PXNull,                                 "Custom", PXResourceTypeCustom, 0 },
        {&_pxResourceManager.SoundLookUp,   PXResourceCreateSound,                  "Sound",PXResourceTypeSound, sizeof(PXSound) },
        {PXNull,                                PXNull,                                 "Video", PXResourceTypeVideo,sizeof(PXVideo) },
        {&_pxResourceManager.ModelLookUp,   PXResourceCreateModel,                  "Model",PXResourceTypeModel, sizeof(PXModel) },
        {&_pxResourceManager.FontLookUp,    PXResourceCreateFont,                   "Font",PXResourceTypeFont, sizeof(PXFont) },
        {&_pxResourceManager.MaterialLookUp, PXResourceCreateMaterial,              "Material", PXResourceTypeMaterial, sizeof(PXMaterial) },
        {&_pxResourceManager.BrushLookUp,   PXResourceCreateBrush,                  "Brush",PXResourceTypeBrush, sizeof(PXWindowBrush) },
        {&_pxResourceManager.TextureLookUp, PXResourceCreateTexture,              "Texture",PXResourceTypeTexture2D, sizeof(PXTexture) },
        {&_pxResourceManager.ShaderProgramLookup, PXResourceCreateShaderProgram,    "ShaderProgram",PXResourceTypeShaderProgram, sizeof(PXShaderProgram) },
        {&_pxResourceManager.SkyBoxLookUp,  PXResourceCreateSkybox,                 "SkyBox",PXResourceTypeSkybox, sizeof(PXSkyBox) },
        {&_pxResourceManager.SpritelLookUp, PXResourceCreateSprite,                 "Sprite",PXResourceTypeSprite, sizeof(PXSprite) },
        {&_pxResourceManager.IconLookUp,    PXResourceCreateIcon,                   "Icon",PXResourceTypeIcon, sizeof(PXIcon) },
        {&_pxResourceManager.IconAtlasLookUp, PXResourceCreateIconAtlas,              "IconAtlas",PXResourceTypeIconAtlas, sizeof(PXIconAtlas) },
        {&_pxResourceManager.SpriteAnimator, PXResourceCreateSpriteAnimator,        "SpriteAnimator",PXResourceTypeSpriteAnimator, sizeof(PXSpriteAnimator) },
        {PXNull,                                PXNull,                                 "Text",PXResourceTypeText, sizeof(PXText) },
        {&_pxResourceManager.TimerLookUp,   PXResourceCreateTimer,                  "Timer",PXResourceTypeTimer, sizeof(PXTimer) },
        {PXNull,                                PXNull,                                 "---",PXResourceTypeEngineSound, 0 },
        {&_pxResourceManager.GUIElementLookup, PXResourceCreateWindow,              "Window", PXResourceTypeGUIElement, sizeof(PXWindow) },
        {&_pxResourceManager.HitBoxLookUp,  PXResourceCreateHitBox,                 "---",PXResourceTypeHitBox, sizeof(PXHitBox) },
        {&_pxResourceManager.MaterialLookUp, PXNull,                                "Material", PXResourceTypeMaterialList, PXNull},
        {PXNull, PXNull, "---", PXResourceTypeCodeDocument, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeDocument, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeBinary, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeStructuredText, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeInstaller, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeArchiv, sizeof(PXWindowBrush) },
        {PXNull, PXNull, "---", PXResourceTypeDialogBox, sizeof(PXWindowBrush) },
        {&_pxResourceManager.SpriteMapAtlasLookUp, PXResourceCreateSpriteMap, "SpriteMap", PXResourceTypeSpriteMap, sizeof(PXSpriteMap) },
    };






}

PXResult PXAPI PXECSComponentRefCheck(PXDictionary PXREF pxDictionary, PXComponentRef PXREF pxComponentRef)
{
    if(!(pxDictionary && pxComponentRef)) // Is this NULL?
    {
        return PXActionRefusedArgumentNull; // Illegal call
    }

    // Is adress in data range?
    const PXBool isInRange = PXListIsAddresValid(&pxDictionary->List, pxComponentRef->Component);

    if(isInRange)
    {
        // Because adress is valid, we can check now
        PXComponent* pxComponent = pxComponentRef->Component;

        const PXBool isDirectMatch = pxComponent->ID == pxComponentRef->ExpectedID;

        if(isDirectMatch)
        {
            // Pointer is valid as expected and does not need to be updated
            return PXActionSuccessful;
        }

        // Pointer is in a valid range but seems to be moved, we need to search..
    }

    // [STALE REFERENCE DETECTED]
    // Search to detect valid object


    PXResult pxResult = PXDictionaryEntryFind(pxDictionary, &pxComponentRef->ExpectedID, &pxComponentRef->Component);

    if(PXActionSuccessful != pxResult)
    {
        // Not found
        return pxResult;
    }

    // We found the object!
    // Reference got updated to new object

    return PXActionSuccessful;
}


PXResult PXAPI PXECSAssetAdd(PXECSRegistrationInfo PXREF pxECSRegistrationInfo)
{

}


PXResult PXAPI PXECSAssetAddaaaaa(PXECSRegistrationInfo PXREF pxECSRegistrationInfo)
{
    if(!pxResourceCreateInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    //-----------------------------------------------------
    // Pre-create Object
    //-----------------------------------------------------

    // TODO: Bad provider funcion!
    const PXResourceEntry PXREF pxResourceEntry = PXResourceEntryGet(pxResourceCreateInfo->Type);

    if(!pxResourceEntry)
    {
        return PXActionRefusedArgumentInvalid;
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
            PXResourceInfo PXREF pxResourceInfo = (PXResourceInfo*)object;
            pxResourceInfo->ID = resourceID;
            pxResourceInfo->Behaviour |= PXResourceInfoExist;

            // Store myself, so we can cast back with hirachy
            pxResourceInfo->Hierarchy.Yourself = object;

            PXDictionaryEntryAdd(pxResourceEntry->LookupTable, &resourceID, *pxResourceCreateInfo->ObjectReference);

            PXResourceProperty pxResourceProperty;
            PXClear(PXResourceProperty, &pxResourceProperty);
            pxResourceProperty.Text = pxResourceCreateInfo->Name;

            PXResourcePropertyIO(pxResourceInfo, &pxResourceProperty, PXResourcePropertyName, PXTrue);

            PXClear(PXResourceProperty, &pxResourceProperty);
            pxResourceProperty.Text = pxResourceCreateInfo->FilePath;
            PXResourcePropertyIO(pxResourceInfo, &pxResourceProperty, PXResourcePropertyPath, PXTrue);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXResourceManagerText,
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
                PXResourceManagerAdd,
                pxResourceCreateInfoASYNC,
                PXNull,
                PXTaskParameterRelease | PXTaskExecuteASYNC
            );

            return PXActionSuccessful;
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
            PXResourceManagerText,
            "Register",
            "[PXID:%-4i] <%s> Has no function to handle creation.",
            -1,
            pxResourceEntry->Name
        );
#endif

        return PXActionRefusedObjectCreateCallbackMissing;
    }

    const PXResult pxActionCreate = pxResourceEntry->CreateFunction(pxResourceCreateInfo, *pxResourceCreateInfo->ObjectReference);

    if(PXActionSuccessful != pxActionCreate)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Create",
            "[PXID:%-4i] <%s> failed creation.",
            -1,
            pxResourceEntry->Name
        );
#endif
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXResourceManagerAddV(PXResourceCreateInfo PXREF pxResourceCreateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXResourceCreateInfo PXREF pxResourceCreateInfo = &pxResourceCreateInfoList[i];

        PXResourceManagerAdd(pxResourceCreateInfo);
    }

    return PXActionSuccessful;
}