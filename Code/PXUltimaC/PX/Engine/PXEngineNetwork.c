#include "PXEngineNetwork.h"

PXResult PXAPI PXEngineNetworkCreate(PXEngineNetwork** pxEngineNetworkREF, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineNetworkUpdate(PXEngineNetwork PXREF pxEngineNetwork, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
    //if(!pxEngine->OnNetworkUpdate)
    {
        return PXResultOK;
    }

    return PXResultOK;// pxEngine->OnNetworkUpdate(pxEngine->Owner, pxEngine);
}