#pragma once
#include "ECS/PXECS.h"

#include <PX/OS/Async/PXProcess.h>

typedef struct PXEngine_ PXEngine;
typedef struct PXEngineCreateInfo_ PXEngineCreateInfo;
typedef struct PXEngineModuleRegisterInfo_ PXEngineModuleRegisterInfo;
typedef struct PXEngineModule_ PXEngineModule;

// Function to 
typedef PXResult(PXAPI* PXEngineModuleInitializeFunction)(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pcEngineCreateInfo);
typedef PXResult(PXAPI* PXEngineModuleActivateFuntion)(PXEngineModule PXREF pxEngineModule, PXEngine PXREF pxEngine);
typedef PXResult(PXAPI* PXEngineModuleUpdateFuntion)(PXEngine PXREF pxEngine);
typedef PXResult(PXAPI* PXEngineModuleRegisterFuntion)(PXEngine PXREF pxEngine, PXEngineModuleRegisterInfo PXREF pxEngineModuleRegisterInfo);


typedef struct PXEngineModuleUpdatePoint_
{
    PXI64U TimeTaken; // Time used for the current or last update
    PXEngineModuleUpdateFuntion OnUpdate;
    PXBool DoUpdate;
}
PXEngineModuleUpdatePoint;

typedef struct PXEngineModule_
{
    //---------------------------------
    // Managed by engine
    PXECSInfo Info;

    // If set, interaction to this module 
    // can only happen within this Thread.
    PXBool UseThreadOwnership;
    PXThreadID ThreadIDOwner;

    PXEngineModuleActivateFuntion OnActivate;
    PXEngineModuleActivateFuntion OnDeactivate;

    PXBool IsActive; // On a bigger failure, deactive this.

    // Will be executed by the main thread
    // Use for I/O that needs to be synced
    PXEngineModuleUpdatePoint Serial;

    // Will be executed by any thread
    // Use for computation
    PXEngineModuleUpdatePoint ASYNC;
}
PXEngineModule;

PXPublic PXResult PXAPI PXEngineModuleIsActive(const PXEngineModule PXREF pxEngineModule);
PXPublic PXResult PXAPI PXEngineModuleActivate(PXEngineModule PXREF pxEngineModule, PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineModuleDeactivate(PXEngineModule PXREF pxEngineModule, PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineModuleCallbackUpdateSerialSet(PXEngineModule PXREF pxEngineModule, PXEngineModuleUpdateFuntion pxEngineModuleUpdatePoint);
PXPublic PXResult PXAPI PXEngineModuleCallbackUpdateASYNCSet(PXEngineModule PXREF pxEngineModule, PXEngineModuleUpdateFuntion pxEngineModuleUpdatePoint);

PXPublic PXResult PXAPI PXEngineModuleUpdateSerial(PXEngineModule PXREF pxEngineModule, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);
PXPublic PXResult PXAPI PXEngineModuleUpdateASYNC(PXEngineModule PXREF pxEngineModule, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);

typedef struct PXEngineModuleRegisterInfo_
{
    PXECSCreateInfo Info;

    PXEngineModuleUpdateFuntion OnActivate;
    PXEngineModuleUpdateFuntion OnDeactivate;
    PXEngineModuleUpdateFuntion Update;
}
PXEngineModuleCreateInfo;