#include "PXEngineModule.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Type/PXTime.h>
#include <PX/Engine/PXEngine.h>

const char PXEngineModuleText[] = "Module-PX";

PXResult PXAPI PXEngineModuleIsActive(const PXEngineModule PXREF pxEngineModule)
{
    if(!pxEngineModule)
    {
        return PXNull;
    }

    return pxEngineModule->IsActive;
}

#include <PX/OS/Async/PXThread.h>

PXResult PXAPI PXEngineModuleActivate(PXEngineModule PXREF pxEngineModule, PXEngine PXREF pxEngine)
{
    PXResult pxResult = PXResultOK;

    if(!(pxEngine && pxEngineModule))
    {
        return PXResultRefusedParameterNull;
    }

    if(pxEngineModule->IsActive)
    {
        return PXResultRefusedStateAlreadyLikeRequested;
    }

    if(pxEngineModule->UseThreadOwnership)
    {
        PXThreadID pxThreadID = PXThreadCurrentID();
        PXBool isSameThread = pxEngineModule->ThreadIDOwner.Data == pxThreadID.Data;

        if(!isSameThread)
        {
            return PXResultRefusedThreadOwnershipViolation;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineModuleText,
        "Activate",
        "PXID:<%4i>",
        pxEngineModule->Info.ID
    );
#endif

    if(pxEngineModule->OnActivate)
    {
        pxResult = pxEngineModule->OnActivate(pxEngineModule, pxEngine);
    }

    pxEngineModule->IsActive = PXTrue;

    return pxResult;
}

PXResult PXAPI PXEngineModuleDeactivate(PXEngineModule PXREF pxEngineModule, PXEngine PXREF pxEngine)
{
    PXResult pxResult = PXResultOK;

    if(!(pxEngine && pxEngineModule))
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxEngineModule->IsActive)
    {
        return PXResultRefusedStateAlreadyLikeRequested;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineModuleText,
        "Deactivate",
        "PXID:<%4i>",
        pxEngineModule->Info.ID
    );
#endif

    if(pxEngineModule->OnActivate)
    {
        pxResult = pxEngineModule->OnDeactivate(pxEngineModule, pxEngine);
    }

    pxEngineModule->IsActive = PXFalse;

    return pxResult;
}

PXResult PXAPI PXEngineModuleCallbackUpdateSerialSet(PXEngineModule PXREF pxEngineModule, PXEngineModuleUpdateFuntion pxEngineModuleUpdatePoint)
{
    pxEngineModule->Serial.OnUpdate = pxEngineModuleUpdatePoint;
    pxEngineModule->Serial.DoUpdate = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXEngineModuleCallbackUpdateASYNCSet(PXEngineModule PXREF pxEngineModule, PXEngineModuleUpdateFuntion pxEngineModuleUpdatePoint)
{
    pxEngineModule->ASYNC.OnUpdate = pxEngineModuleUpdatePoint;
    pxEngineModule->ASYNC.DoUpdate = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXEngineModuleUpdateSerial(PXEngineModule PXREF pxEngineModule, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    if(!(pxEngineModule && pxEngineUpdateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxEngineModule->Serial.DoUpdate)
    {
        return PXResultOK;
    }

    const PXTimeStamp timeStart = PXTimeStampNow();
    PXResult pxResult = PXSafeCallx2(pxEngineModule->Serial.OnUpdate, pxEngineModule, pxEngineUpdateInfo);

    if(PXResultOK != pxResult)
    {
        //pxEngineModule->IsActive = PXFalse; // Deactivate module
    }

    pxEngineModule->Serial.TimeTaken = PXTimeStampDeltaNSF(timeStart);

    return pxResult;
}

PXResult PXAPI PXEngineModuleUpdateASYNC(PXEngineModule PXREF pxEngineModule, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    if(!(pxEngineModule && pxEngineUpdateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxEngineModule->ASYNC.DoUpdate)
    {
        return PXResultOK;
    }

    const PXTimeStamp timeStart = PXTimeStampNow();

    PXResult pxResult = PXSafeCallx2(pxEngineModule->ASYNC.OnUpdate, pxEngineModule, pxEngineUpdateInfo);

    if(PXResultOK != pxResult)
    {
        //pxEngineModule->IsActive = PXFalse; // Deactivate module
    }

    pxEngineModule->ASYNC.TimeTaken = PXTimeStampDeltaNSF(timeStart);

    return pxResult;
}