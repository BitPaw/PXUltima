#include "PXGPU.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
//#include <d3dkmthk.h> // Buggy to compile
#endif
#include <dxgi1_4.h> 

const char PXGraphicsCardName[] = "GPU";
const PXI8U PXGraphicsCardNameLength = sizeof(PXGraphicsCardName);
const PXECSRegisterInfoStatic PXGraphicsCardRegisterInfoStatic =
{
    {sizeof(PXGraphicsCardName), sizeof(PXGraphicsCardName), PXGraphicsCardName, TextFormatASCII},
    sizeof(PXGPU),
    __alignof(PXGPU),
    PXECSTypeResource,
    PXNull,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXGraphicsCardRegisterInfoDynamic;


void PXAPI PXGPURegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXGraphicsCardRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXGraphicsCardRegisterInfoDynamic;
}

PXResult PXAPI PXGPUFetchList()
{
#if OSUnix
#elif OSWindows
   

    


#endif

    return PXResultOK;
}

PXResult PXAPI PXGPUUsageUpdate(PXGPU PXREF pxGPU)
{
#if 0
    static ULONGLONG prevTime = 0;

    double usage = 0;
    D3DKMT_QUERYSTATISTICS stats;
    PXClear(D3DKMT_QUERYSTATISTICS, &stats);
    stats.Type = D3DKMT_QUERYSTATISTICS_NODE;
    stats.AdapterLuid = pxGPU->AdapterLUID;
    stats.QueryNode.NodeId = 0; // Node index, usually 0 for GPU

    NTSTATUS ntStatus = D3DKMTQueryStatistics(&stats);
    PXBool isOK = 0 == ntStatus;

    if(!isOK)
    {
        return PXResultFailedUnkownError;
    }

    ULONGLONG newTime = stats.QueryResult.NodeInformation.GlobalInformation.RunningTime.QuadPart;

    if(prevTime != 0)
    {
        ULONGLONG delta = newTime - prevTime;
        usage = (double)delta / 10000000.0 * 100.0;
    }
    prevTime = newTime;
    
    pxGPU->Usage = usage;
#endif

    return PXResultOK;
}