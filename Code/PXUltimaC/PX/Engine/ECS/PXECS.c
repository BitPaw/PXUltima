#include "PXECS.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/Engine/ECS/PXNamePool.h>
#include <PX/Container/Dictionary/PXDictionary.h>

#include <PX/OS/PXOS.h>


// Resources
#include <PX/Container/ListDynamic/PXListDynamic.h>
#include <PX/Type/PXAudioDevice.h>
#include <PX/Type/PXSound.h>
#include <PX/Type/PXBrush.h>
#include <PX/Type/PXDisplay.h>
#include <PX/Type/PXFont.h>
#include <PX/Type/PXFrameBuffer.h>
#include <PX/Type/PXIcon.h>
#include <PX/Type/PXMesh.h>
#include <PX/Type/PXMonitor.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXTexture.h>
#include <PX/Type/PXTimer.h>
#include <PX/Type/PXVideo.h>
#include <PX/Type/PXWindow.h>
#include <PX/Type/PXGraphicsCard.h>
#include <PX/Type/PXAudioEmitter.h>
#include <PX/Type/PXAudioListener.h>
#include <PX/Type/PXCamera.h>
#include <PX/Type/PXFieldEffect.h>
#include <PX/Type/PXGizmo.h>
#include <PX/Type/PXIconAudio.h>
#include <PX/Type/PXIconSun.h>
#include <PX/Type/PXLightDirectional.h>
#include <PX/Type/PXLightPoint.h>
#include <PX/Type/PXLightSpot.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXSkyBox.h>
#include <PX/Type/PXSpaceGrid.h>
#include <PX/Type/PXSprite.h>
#include <PX/Type/PXTesseract.h>
#include <PX/Engine/UI/PXECSEntityInfo.h>
#include <PX/Engine/UI/PXTabList.h>
#include <PX/Engine/UI/PXTransformView.h>
#include <PX/Engine/UI/PXFileDirectory.h>
#include <PX/Engine/UI/PXColorPicker.h>
#include <PX/Engine/UI/PXFooter.h>
#include <PX/Engine/UI/PXECSEntityList.h>

#include <PX/Engine/PXEngine.h>
#include <PX/Engine/PXRenderingSystem.h>
#include <PX/Engine/PXAudioSystem.h>



// Components

// Assets?

// Private class
// Stores all components
// Additional lookup and managing of entity to component relation
typedef struct PXECS_
{
    // To know what types we know
    PXDictionaryT(PXID, PXECSTypeInfo)* LookupType;

    // Entity to Component
    PXDictionaryT(PXID, PXECSReferece)* LookupEntry;

    // For multithreading we need protection
    PXLock* ResourceLock;
    PXLock* AsyncLock;

    PXListDynamic NameCache;
    PXListDynamic SourcePathCache;

    PXI32U Flags;
}
PXECS;

const char PXECSText[] = "ECS-PX";

PXECS _pxECS;

const PXECSRegisterFunction pxECSRegisterList[] =
{
    PXAudioListenerRegisterToECS,
    PXAudioEmitteRegisterToECS,
    PXCameraRegisterToECS,
    PXFieldEffectRegisterToECS,
    PXGizmoRegisterToECS,
    PXIconAudioRegisterToECS,
    PXIconSunRegisterToECS,
    PXLightDirectionalRegisterToECS,
    PXLightPointRegisterToECS,
    PXLightSpotRegisterToECS,
    PXModelRegisterToECS,
    PXSkyBoxRegisterToECS,
    PXSpaceGridRegisterToECS,
    PXSpriteRegisterToECS,
    PXTesseractRegisterToECS,

    PXSoundRegisterToECS,

    PXVertexBufferRegisterToECS,
    PXIndexBufferRegisterToECS,
    PXAudioDeviceRegisterToECS,
    PXTexturePoolRegisterToECS,
    PXFooterRegisterToECS,
    PXMeshGeometryRegisterToECS,
    PXColorPickerRegisterToECS,
    PXTabListRegisterToECS,
    PXGraphicsCardRegisterToECS,
    PXSkyBoxRegisterToECS,
    PXECSEntityInfoRegisterToECS,
  
    PXTransformViewRegisterToECS,
    PXFileDirectoryRegisterToECS,
    PXLockRegisterToECS,
    PXThreadRegisterToECS,
    PXBrushRegisterToECS,
    PXFontRegisterToECS,
    PXFrameBuffeRegisterToECS,
    PXIconRegisterToECS,
    PXMeshRegisterToECS,

    PXECSEntityListRegisterToECS,
    PXDictionaryRegisterToECS,
    PXTextureRegisterToECS,

    PXWindowRegisterToECS,

    //PXAudioSystemRegisterToECS,
    PXEngineRegisterToECS,
    PXRenderingSystemRegisterToECS,
    PXAudioSystemRegisterToECS
    
};
const PXI16U _pxECSRegisterListAmount = sizeof(pxECSRegisterList) / sizeof(PXECSRegisterFunction);

const char* PXECSTypeToString(const PXECSType pxECSType)
{
    switch(pxECSType)
    {
        case PXECSTypeEntity:   
            return "Entity";
        case PXECSTypeComponent:   
            return "Component";
        case PXECSTypeResource:
            return "Resource";
        case PXECSTypeSystem: 
            return "System";
        case PXECSTypeContainer:
            return "Container";
        case PXECSTypeInvalid:
        default:
            return "Invalid";
    }
}

void PXAPI PXECSInfoIDGenerate(PXECSInfo PXREF pxECSInfo)
{
    pxECSInfo->ID = PXIDGenerate();
}

void PXAPI PXECSInfoFlagStateAdd(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    pxECSInfo->FlagListState |= pxECSFlagList;
}

void PXAPI PXECSInfoFlagStateRemove(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    pxECSInfo->FlagListState &= ~pxECSFlagList;
}

PXBool PXAPI PXECSInfoFlagStateCheck(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    return pxECSInfo->FlagListState & pxECSFlagList > 0;
}

void PXAPI PXECSInfoFlagSettingsAdd(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    pxECSInfo->FlagListSettings |= pxECSFlagList;
}

void PXAPI PXECSInfoFlagSettingsRemove(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    pxECSInfo->FlagListSettings &= ~pxECSFlagList;
}

PXBool PXAPI PXECSInfoFlagSettingsCheck(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    return pxECSInfo->FlagListSettings & pxECSFlagList > 0;
}

PXI8U PXAPI PXECSInfoStateEqual(const PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    return PXECSInfoStateGet(pxECSInfo) == pxECSFlagList;
}

PXI8U PXAPI PXECSInfoStateGet(const PXECSInfo PXREF pxECSInfo)
{
    return pxECSInfo->FlagListState & PXECSFlagStateMask;
}

void PXAPI PXECSInfoStateSet(PXECSInfo PXREF pxECSInfo, const PXECSFlagList pxECSFlagList)
{
    pxECSInfo->FlagListState &= ~PXECSFlagStateMask;
    pxECSInfo->FlagListState |= pxECSFlagList;
}

const char* PXECSStateList[] = 
{
 "INVALID"
 "INITIALIZING",
 "READY",
 "RUNNING",
 "WAITING",
 "SUSPENDED",
 "COMPLETED",
 "FAILED",
 "MARKED_FOR_DELETE",
 "DELETED",
 "UNLOADED",
 "LOADING",
 "STALE",
 "RECOVERING",
 "DISABLED",
 "RESERVED",
 "RESERVED0",
 "RESERVED1"
};


const char* PXAPI PXExecuteStateToString(const PXI32U behaviour)
{
    const PXI8U index = PXECSFlagStateMask & behaviour;

    return PXECSStateList[index];
}

PXResult PXAPI PXECSRegister(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    const PXECSRegisterInfoStatic PXREF infoStatic = pxECSRegisterInfo->InfoStatic;
    PXECSRegisterInfoDynamic PXREF infoDynamic = pxECSRegisterInfo->InfoDynamic;

    // We need a uniqe ID to register
    infoDynamic->ID = PXIDGenerate();

    // We cant trust the pointer from beeing safe to store
    // It must be stored seperately
    PXNamePoolStore
    (
        infoDynamic->ID,
        &infoStatic->NameOfType,
        &infoStatic->NameOfType
    );

    PXAssert(infoStatic->TypeSize >= sizeof(PXECSInfo), "Used ECS object is missing info");



    // Registration of the type in our system
    PXECSRegisterInfo pxECSTypeInfo;
    pxECSTypeInfo.InfoStatic = infoStatic;
    pxECSTypeInfo.InfoDynamic = infoDynamic;

    PXResult pxResult = PXDictionaryEntryAdd(_pxECS.LookupType, &infoDynamic->ID, &pxECSTypeInfo);


    //-----------------------

#if PXLogEnable
    char buffer[128];
    PXText pxTextModule;
    PXTextFromAdressA(&pxTextModule, buffer, 0, sizeof(buffer));

    PXLibraryNameFromAdress(PXNull, &pxTextModule, infoStatic->CreateCallback);

    const char* typeName = PXECSTypeToString(infoStatic->Type);

    PXLogPrint
    (
        PXLoggingAllocation,
        PXECSText,
        "Register",
        "0d%4.4i_%s:%4.4i_%-20s - %s",
        infoDynamic->ID,
        pxTextModule.A,
        infoStatic->TypeSize,
        infoStatic->NameOfType.A,
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

    PXDictionaryCreateInfo pxDictionaryCreateInfo;
    PXClear(PXDictionaryCreateInfo, &pxDictionaryCreateInfo);
    pxDictionaryCreateInfo.KeySize = sizeof(PXID);
    pxDictionaryCreateInfo.ValueSize = sizeof(PXECSRegisterInfo);
    pxDictionaryCreateInfo.ValueLocality = PXDictionaryValueLocalityInternalEmbedded;

    PXDictionaryCreate(&_pxECS.LookupType, &pxDictionaryCreateInfo);

    pxDictionaryCreateInfo.ValueSize = sizeof(PXECSReferece);
    PXDictionaryCreate(&_pxECS.LookupEntry, &pxDictionaryCreateInfo);

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
#if PXLogEnable && 0
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

        PXECSRegisterInfo pxECSRegisterInfo;
        PXClear(PXECSRegisterInfo, &pxECSRegisterInfo);

        // Fetch static and dynamic type info
        pxECSRegisterList[i](&pxECSRegisterInfo);

        // register data
        PXResult pxResult = PXECSRegister(&pxECSRegisterInfo);
     

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

            DebugBreak();
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

PXResult PXAPI PXECSRefCheck(PXDictionary PXREF pxDictionary, PXECSReferece PXREF pxECSReferece)
{
    if(!(pxDictionary && pxECSReferece)) // Is this NULL?
    {
        return PXResultRefusedParameterNull; // Illegal call
    }

    // Is adress in data range?
    PXECSInfo* pxECSElement = pxECSReferece->Element;

    const PXBool isInRange = PXListIsAddresValid(&pxDictionary->List, pxECSReferece->Element);

    if(isInRange)
    {
        // Because adress is valid, we can check now
        const PXBool isDirectMatch = pxECSReferece->ExpectedID == pxECSElement->ID;

        if(isDirectMatch)
        {
            // Pointer is valid as expected and does not need to be updated
            return PXResultOK;
        }

        // Pointer is in a valid range but seems to be moved, we need to search..
    }

    // [STALE REFERENCE DETECTED]
    // Search to detect valid object

    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = &pxECSReferece->ExpectedID;
    pxDictionaryEntry.KeySize = sizeof(PXID);
    pxDictionaryEntry.ValueAdress = &pxECSReferece->Element;
    pxDictionaryEntry.ValueSize = sizeof(PXECSInfo*);

    PXResult pxResult = PXDictionaryEntryFind(pxDictionary, &pxDictionaryEntry);

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
            pxECSReferece->ExpectedID
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
        pxECSReferece->ExpectedID
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


    PXECSReferece pxECSReferece;
    pxECSReferece.ExpectedID = pxID;
    pxECSReferece.Element = pxECSInfo;

    PXResult pxResult = PXDictionaryEntryAdd(_pxECS.LookupEntry, &pxID, &pxECSReferece);


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
        "%s (PXID:%4i, %.*s)",
        name,
        pxID,
        pxECSCreateInfo->Name.SizeUsed,
        pxECSCreateInfo->Name.A
    );
#endif 

    return PXResultOK;
}

PXSize PXAPI PXECSTypeAmount(void)
{
    return PXDictionaryEntryAmount(_pxECS.LookupType);
}

PXResult PXAPI PXECSTypeGet(PXECSRegisterInfo PXREF pxECSTypeInfo, const PXSize index)
{
    PXDictionaryEntry pxDictionaryEntry;

    PXResult pxResult = PXDictionaryIndex(_pxECS.LookupType, index, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *pxECSTypeInfo = *(PXECSRegisterInfo*)pxDictionaryEntry.ValueAdress;

    return PXResultOK;
}

PXSize PXAPI PXECSEntryAmount(void)
{
    return PXDictionaryEntryAmount(_pxECS.LookupEntry);
}

PXResult PXAPI PXECSEntryGet(PXECSReferece PXREF pxECSReferece, const PXSize index)
{
    PXDictionaryEntry pxDictionaryEntry;

    PXResult pxResult = PXDictionaryIndex(_pxECS.LookupEntry, index, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *pxECSReferece = *(PXECSReferece*)pxDictionaryEntry.ValueAdress;

    return PXResultOK;
}

PXResult PXAPI PXECSLoad(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    if(!pxECSCreateInfo)
    {
        return PXResultRefusedParameterNull;
    }

    PXText* pxFilePath = &pxECSCreateInfo->FilePath;

    // parameter valid check
    {
        PXBool textValid = 
            pxFilePath->Data4 &&
            pxFilePath->SizeUsed;

        if(!textValid)
        {
            return PXResultRefusedParameterInvalid;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXECSText,
        "Load",
        "Begin for <%s>",
        pxFilePath->A
    );
#endif

    // try to detect format over file extension
    PXFileTypeInfoProbe(&pxECSCreateInfo->FormatInfo, pxFilePath);


    // if we dont even have a clue how to load it, let the OS try to load it.
    {
        if(pxECSCreateInfo->FormatInfo.Flags == PXFileFormatUnkown)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Load",
                "Refused : Format not detected"
            );
#endif

            return PXActionRefusedNotSupportedByLibrary;
        }

        if(!pxECSCreateInfo->FormatInfo.ResourceLoad)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Load",
                "Refused : Not implemented"
            );
#endif

            return PXActionRefusedNotImplemented;
        }
    }

    // Loading and map file if possible
    {
        PXFileCreateInfo pxFileOpenInfo;
        PXClear(PXFileCreateInfo, &pxFileOpenInfo);
        pxFileOpenInfo.FilePath = *pxFilePath;
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        const PXResult fileLoadingResult = PXFileCreate(&pxECSCreateInfo->FileCurrent, &pxFileOpenInfo);

        if(PXResultOK != fileLoadingResult)
            return fileLoadingResult;
    }

    PXPerformanceInfo pxPerformanceInfo;
    pxPerformanceInfo.UpdateCounter = 0;

    // If a peek method exists, execute it, if not, strait to loading
    if(pxECSCreateInfo->FormatInfo.ResourcePeek)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-Peek",
            "Start..."
        );
#endif

        PXPerformanceInfoGet(&pxPerformanceInfo);
        const PXResult pxPeekResult = pxECSCreateInfo->FormatInfo.ResourcePeek(pxECSInfo, pxECSCreateInfo);
        const PXBool success = PXResultOK == pxPeekResult;

        if(!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "File-Peek",
                "Failed"
            );
#endif
            return pxPeekResult;
        }

        PXPerformanceInfoGet(&pxPerformanceInfo);
        pxECSCreateInfo->TimePeek = pxPerformanceInfo.TimeDelta;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-Peek",
            "Done! Took:%6.3fs",
            pxECSCreateInfo->TimePeek
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-Peek",
            "No peek function implemented. procceed.."
        );
#endif
    }

    // Preallocate on device
    if(pxECSCreateInfo->OnDeviceDataRegister)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-PreAlloc",
            "Preallocate resource on device"
        );
#endif

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxECSCreateInfo->OnDeviceDataRegister(pxECSInfo, pxECSCreateInfo);

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxECSCreateInfo->TimeDeviceDataRegister = pxPerformanceInfo.TimeDelta;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-PreAlloc",
            "Preallocate took:<%6.3f>",
            pxECSCreateInfo->TimeDeviceDataRegister
        );
#endif
    }

    // Try to load assumed format
    if(pxECSCreateInfo->FormatInfo.ResourceLoad)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-Extract",
            "Start..."
        );
#endif

        //pxECSCreateInfo->FileReference = pxFile;

        PXPerformanceInfoGet(&pxPerformanceInfo);

        const PXResult fileParsingResult = pxECSCreateInfo->FormatInfo.ResourceLoad(pxECSInfo, pxECSCreateInfo);

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxECSCreateInfo->TimeTransphere = pxPerformanceInfo.TimeDelta;



        PXFileClose(pxECSCreateInfo->FileCurrent);

        if(PXResultOK != fileParsingResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Load-Extract",
                "Failed. Took:%6.3f  ROPs:%-7i <%s>",
                pxECSCreateInfo->TimeTransphere,
                PXFileOperationsRead(pxECSCreateInfo->FileCurrent),
                pxFilePath->A
            );
#endif

            return fileParsingResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Load-Extract",
            "OK! Took:%6.3f, ROPs:%-7i PageFaults:%-7i <%s>",
            pxECSCreateInfo->TimeTransphere,
            PXFileOperationsRead(pxECSCreateInfo->FileCurrent),
            pxPerformanceInfo.PageFaultCount,
            pxFilePath->A
        );
#endif

        return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }
}

PXResult PXAPI PXECSSave(PXECSInfo PXREF pxECSInfo, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    if(!pxECSCreateInfo)
    {
        return PXResultRefusedParameterNull;
    }

    PXText* pxFilePath = &pxECSCreateInfo->FilePath;

    PXFile* pxFile = PXNull;

    // Loading file
    {
        PXFileCreateInfo pxFileIOInfo;
        PXClear(PXFileCreateInfo, &pxFileIOInfo);
        pxFileIOInfo.FilePath = *pxFilePath;
        pxFileIOInfo.AccessMode = PXAccessModeWriteOnly;
        pxFileIOInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileIOInfo.FlagList = PXFileIOInfoFilePhysical | PXFileIOInfoAllowMapping | PXFileIOInfoCreateIfNotExist;

        const PXResult fileLoadingResult = PXFileCreate(&pxFile, &pxFileIOInfo);

        if(PXResultOK != fileLoadingResult)
            return fileLoadingResult;
    }


    // try to detect format over file extension
    PXFileTypeInfoProbe(&pxECSCreateInfo->FormatInfo, pxFilePath);


    // Try to load assumed format
    {
        if(PXFileFormatUnkown == pxECSCreateInfo->FormatInfo.Flags)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Save",
                "Refused : Format not known"
            );
#endif

            return PXActionRefusedNotSupportedByLibrary;
        }

        if(PXNull == pxECSCreateInfo->FormatInfo.ResourceSave)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXECSText,
                "Save",
                "Refused : Not implemented"
            );
#endif

            return PXActionRefusedNotImplemented;
        }

#if PXLogEnable
        const PXI64U timeStampA = PXTimeCounterStampGet();
#endif

       // pxECSCreateInfo->FileReference = pxFile;

        const PXResult fileParsingResult = pxECSCreateInfo->FormatInfo.ResourceSave(pxECSInfo, pxECSCreateInfo);

#if PXLogEnable
        const PXI64U timeStampB = PXTimeCounterStampGet() - timeStampA;
        const PXF32 timeDelta = PXTimeCounterStampToSecoundsF(timeStampB);

        PXLogPrint
        (
            PXLoggingInfo,
            PXECSText,
            "Save",
            "%6.3fms SOPs:%-7i <%s>",
            timeDelta,
            PXFileOperationsWrite(pxFile),
            pxFilePath->A
        );
#endif

        // PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try

        // return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

    PXFileClose(pxFile);

    //pxECSCreateInfo->FileReference = PXNull;

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