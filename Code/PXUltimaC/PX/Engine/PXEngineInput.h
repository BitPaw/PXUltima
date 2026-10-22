#pragma once

#include "ECS/PXECS.h"
#include "PXEngineModule.h"

typedef struct PXEngineInput_
{
    PXEngineModule ModuleInfo;
}
PXEngineInput;

typedef struct PXEngineInputCreateInfo_
{
    PXECSCreateInfo Info;

    PXBool MouseEnable;
    PXBool KeyboardEnable;
}
PXEngineInputCreateInfo;

PXPublic PXResult PXAPI PXEngineInputCreate(PXEngineInput** pxEngineInputREF, PXEngineInputCreateInfo PXREF pxEngineInputCreateInfo);
PXPublic PXResult PXAPI PXEngineInputUpdate(PXEngineInput PXREF pxEngineInput, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);