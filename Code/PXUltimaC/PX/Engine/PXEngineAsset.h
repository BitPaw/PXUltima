#pragma once

#include <PX/Engine/PXEngineModule.h>

typedef struct PXEngineAsset_
{
    int x;
}
PXEngineAsset;

PXPublic PXResult PXAPI PXEngineAssetUpdate(PXEngineAsset PXREF pxEngineAsset, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);

PXPublic PXResult PXAPI PXEngineAssetLoad(PXEngineAsset PXREF pxEngineAsset, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);