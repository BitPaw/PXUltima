#pragma once

#ifndef PXEngineNetworkIncluded
#define PXEngineNetworkIncluded

#include <PX/Engine/ECS/PXECS.h>
#include "PXEngineModule.h"

typedef struct PXEngineNetwork_
{
    PXEngineModule ModuleInfo;
}
PXEngineNetwork;

typedef struct PXEngineNetworkCreateInfo_
{
    PXECSCreateInfo Info;
}
PXEngineNetworkCreateInfo;

PXPublic PXResult PXAPI PXEngineNetworkCreate(PXEngineNetwork** pxEngineNetworkREF, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);
PXPublic PXResult PXAPI PXEngineNetworkUpdate(PXEngineNetwork PXREF pxEngineNetwork, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);

#endif