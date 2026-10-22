#pragma once

#include <PX/Engine/ECS/PXECS.h>
#include "PXEngineModule.h"

typedef struct PXEnginePhysics_
{
    PXEngineModule ModuleInfo;
}
PXEnginePhysics;

PXPublic void PXAPI PXEngineCollsisionSolve(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineUpdateCollision(PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEnginePhysicsUpdate(PXEnginePhysics PXREF pxEnginePhysics, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);