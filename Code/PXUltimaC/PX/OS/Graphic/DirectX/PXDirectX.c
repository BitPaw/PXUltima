#include "PXDirectX.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Type/PXText.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Graphic/DirectX/PXDirectX9.h>
#include <PX/OS/Graphic/DirectX/PXDirectX10.h>
#include <PX/OS/Graphic/DirectX/PXDirectX10x1.h>
#include <PX/OS/Graphic/DirectX/PXDirectX11.h>
#include <PX/OS/Graphic/DirectX/PXDirectX12.h>


#if OSUnix
#elif OSWindows
#include <windows.h>
#include <ddraw.h>
#include <dxgi.h>
#pragma comment(lib, "Ddraw.lib")
#endif


const char PXDirectXText[] = "DirectX";

const char PXDXGILibraryName[] = "dxgi.dll";
typedef HRESULT(WINAPI* CREATE_DXGI_FACTORY1)(REFIID, void**);

typedef struct PXDXGI_
{
    PXLibrary Library;

    CREATE_DXGI_FACTORY1 pCreateDXGIFactory1;
}
PXDXGI;


const PXGPUInitializeFunction _directXSolutions[5] =
{
    PXDirectX12Initialize,
    PXDirectX11Initialize,
    PXDirectX10x1Initialize,
    PXDirectX10Initialize,
    PXDirectX9Initialize
};
const PXSize _directXSolutionsAmount = sizeof(_directXSolutions) / sizeof(PXGPUInitializeFunction);


BOOL WINAPI PXDDEnumCallBackExW
(
    const GUID* directDrawID,
    const LPWSTR driverName,
    const LPWSTR driverDesciption,
    PXEngineRender PXREF pxEngineRender,
    const HMONITOR monitorHandle
)
{
    PXGPU pxGPU;
    PXClear(PXGPU, &pxGPU);

    if(monitorHandle) 
    {
        MONITORINFOEXW mi;
        mi.cbSize = sizeof(MONITORINFOEXW);
        PXBool isOK = GetMonitorInfoW(monitorHandle, &mi);

        if(isOK) 
        {
            PXTextCopyWU(mi.szDevice, CCHDEVICENAME, pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));
        }
    }

    pxGPU.MonitorHandle = monitorHandle;

    PXTextCopyWU(driverName, PXTextLengthUnkown, pxGPU.DriverName, sizeof(pxGPU.DriverName));
    PXTextCopyWU(driverDesciption, PXTextLengthUnkown, pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));

    PXEngineRenderGPUMerge(pxEngineRender, &pxGPU);

    return PXTrue; // for next value
}
BOOL WINAPI PXDDEnumCallBackExA
(
    const GUID* directDrawID,
    const LPSTR driverName,
    const LPSTR driverDesciption,
    PXEngineRender PXREF pxEngineRender,
    const HMONITOR monitorHandle
)
{
    PXGPU pxGPU;
    PXClear(PXGPU, &pxGPU);

    if(monitorHandle)
    {
        MONITORINFOEXW mi;
        mi.cbSize = sizeof(MONITORINFOEXW);
        PXBool isOK = GetMonitorInfoW(monitorHandle, &mi);

        if(isOK)
        {
            PXTextCopyWU(mi.szDevice, CCHDEVICENAME, pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));
        }
    }

    pxGPU.MonitorHandle = monitorHandle;

    PXTextCopyA(driverName, PXTextLengthUnkown, pxGPU.Name, sizeof(pxGPU.Name));
    PXTextCopyA(driverDesciption, PXTextLengthUnkown, pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));

    PXEngineRenderGPUMerge(pxEngineRender, &pxGPU);

    return PXTrue; // for next value
}







PXDXGI pxDXGI;

PXResult PXAPI PXDirectXInitialize(PXDirectX PXREF pxDirectX, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectXText,
        "Initialize",
        "---Start---"
    );
#endif

    PXClear(PXDirectX, pxDirectX);

#if PXOSWindowsDestop


    PXDirectXGPUListRefresh(pxEngineRenderCreateInfo->RenderSystem);

    //HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    //UINT              Flags = 0;

    pxDirectX->DirectXVersion = pxEngineRenderCreateInfo->DirectXVersion;

    PXBool directXCreated = PXFalse;

    if(PXDirectXVersionNewest == pxDirectX->DirectXVersion)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Brute force newest version, testing each one."
        );
#endif

        void PXREF directXSolutionsObject[5] =
        {
            &pxDirectX->X12,
            &pxDirectX->X11,
            &pxDirectX->X10x1,
            &pxDirectX->X10,
            &pxDirectX->X9
        };

        for(PXSize i = 0; i < _directXSolutionsAmount; ++i)
        {
            PXClear(PXDirectX, pxDirectX);

            pxEngineRenderCreateInfo->RenderSystem->Initialize = _directXSolutions[i];
            pxDirectX->DXTargetAPI = directXSolutionsObject[i];

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDirectXText,
                "Initialize",
                "Calling spesific API. Try <%i/%i>",
                i+1,
                _directXSolutionsAmount
            );
#endif

            const PXResult initializeResult = pxEngineRenderCreateInfo->RenderSystem->Initialize(pxDirectX->DXTargetAPI, pxEngineRenderCreateInfo);

            if(PXResultOK == initializeResult)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXDirectXText,
                    "Initialize",
                    "Creation try successful <%i/%i>",
                    i + 1,
                    _directXSolutionsAmount
                );
#endif

                directXCreated = PXTrue;

                return PXResultOK;
            }

            PXLogPrint
            (
                PXLoggingWarning,
                PXDirectXText,
                "Initialize",
                "Failed try <%i/%i>",
                i + 1,
                _directXSolutionsAmount
            );
        }

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXNull;
        pxDirectX->DXTargetAPI = PXNull;
    }

    if(!directXCreated)
    {
        PXLogPrint
        (
            PXLoggingError,
            PXDirectXText,
            "Initialize",
            "Failed all attempts! DirectX API is not reachable. A supported device or drivers might be missing."
        );

        return PXResultFailedInitialization;
    }

    switch(pxDirectX->DirectXVersion)
    {
    case PXDirectXVersionNewest:
    case PXDirectXVersion12Emulate1x0Core:
    case PXDirectXVersion12Emulate9x1:
    case PXDirectXVersion12Emulate9x2:
    case PXDirectXVersion12Emulate9x3:
    case PXDirectXVersion12Emulate10x0:
    case PXDirectXVersion12Emulate10x1:
    case PXDirectXVersion12Emulate11x0:
    case PXDirectXVersion12Emulate11x1:
    case PXDirectXVersion12Emulate12x0:
    case PXDirectXVersion12Emulate12x1:
    case PXDirectXVersion12Emulate12x2:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Select DirectX 12"
        );

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXDirectX12Initialize;
        pxDirectX->DXTargetAPI = &pxDirectX->X12;
        break;
    }
    case PXDirectXVersion11Emulate1x0Core:
    case PXDirectXVersion11Emulate9x1:
    case PXDirectXVersion11Emulate9x2:
    case PXDirectXVersion11Emulate9x3:
    case PXDirectXVersion11Emulate10x0:
    case PXDirectXVersion11Emulate10x1:
    case PXDirectXVersion11Emulate11x0:
    case PXDirectXVersion11Emulate11x1:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Select DirectX 11"
        );

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXDirectX11Initialize;
        pxDirectX->DXTargetAPI = &pxDirectX->X11;
        break;
    }
    case PXDirectXVersion10x1Simulate10x0:
    case PXDirectXVersion10x1Simulate9x1:
    case PXDirectXVersion10x1Simulate9x2:
    case PXDirectXVersion10x1Simulate9x3:
    case PXDirectXVersion10x1:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Select DirectX 10.1"
        );

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXDirectX10x1Initialize;
        pxDirectX->DXTargetAPI = &pxDirectX->X10x1;
        break;
    }
    case PXDirectXVersion10x0:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Select DirectX 10"
        );

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXDirectX10Initialize;
        pxDirectX->DXTargetAPI = &pxDirectX->X10;
        break;
    }
    case PXDirectXVersion9:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectXText,
            "Initialize",
            "Select DirectX 9"
        );

        pxEngineRenderCreateInfo->RenderSystem->Initialize = PXDirectX9Initialize;
        pxDirectX->DXTargetAPI = &pxDirectX->X9;
        break;
    }
    default:
        return PXResultRefusedFormatNotSupported;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectXText,
        "Initialize",
        "Calling spesific API"
    );

    const PXResult initializeResult = pxEngineRenderCreateInfo->RenderSystem->Initialize(pxDirectX->DXTargetAPI, pxEngineRenderCreateInfo);

    return initializeResult;
#else
    return PXResultRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXDirectXRelease(PXDirectX PXREF pxDirectX)
{
    return PXResultInvalid;

    // return  pxDirectX->Release(pxDirectX->DXTargetAPI);
}

PXResult PXAPI PXDirectXGPUListRefresh(PXEngineRender PXREF pxEngineRender)
{
    HRESULT resultID = 0;
    PXResult pxResult = PXResultOK;


    // Legacy pathway for older DirectDraw devices.
    // DirectX6
    DWORD flags = DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_DETACHEDSECONDARYDEVICES;

    resultID = DirectDrawEnumerateExW(PXDDEnumCallBackExW, pxEngineRender, flags); // Windows xxx, ddraw.dll, ddraw.h

    if(E_NOTIMPL == resultID)
    {
        resultID = DirectDrawEnumerateExA(PXDDEnumCallBackExA, pxEngineRender, flags);
    }


    // Modern pathway for new

   // CreateDXGIFactory1(); // Windows, dxgi.dll, dxgi.h

    PXText pxText;
    PXTextFromAddressA(&pxText, PXDXGILibraryName, sizeof(PXDXGILibraryName), sizeof(PXDXGILibraryName));
    pxResult = PXLibraryOpen(&pxDXGI.Library, &pxText);

    pxResult = PXLibraryGetSymbolA(&pxDXGI.Library, &pxDXGI.pCreateDXGIFactory1, "CreateDXGIFactory1", PXFalse);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    IDXGIFactory1* pFactory = PXNull;

    resultID = pxDXGI.pCreateDXGIFactory1(&IID_IDXGIFactory1, &pFactory);
    pxResult = PXErrorFromHRESULT(resultID);
    
    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    UINT i = 0;
    IDXGIAdapter1* pAdapter = 0;

    while(pFactory->lpVtbl->EnumAdapters1(pFactory, i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC1 desc;
        pAdapter->lpVtbl->GetDesc1(pAdapter, &desc);

        PXGPU pxGPU;
        PXClear(PXGPU, &pxGPU);
        PXTextCopyWU(desc.Description, sizeof(desc.Description), pxGPU.Name, sizeof(pxGPU.Name));
        pxGPU.VendorID = desc.VendorId;
        pxGPU.DeviceID = desc.DeviceId;
        pxGPU.SubSysID = desc.SubSysId;
        pxGPU.Revision = desc.Revision;
        pxGPU.VideoMemoryDedicated = desc.DedicatedVideoMemory;
        pxGPU.VideoMemoryAllocatedOnCPUExlusive = desc.DedicatedSystemMemory;
        pxGPU.VideoMemoryAllocatedOnCPUShared = desc.SharedSystemMemory;
        pxGPU.AdapterLUID = desc.AdapterLuid;


        for(UINT oIdx = 0; ; ++oIdx)
        {
            IDXGIOutput* pOutput = 0;

            resultID = pAdapter->lpVtbl->EnumOutputs(pAdapter, oIdx, &pOutput);

            if(S_OK != resultID)
            {
                break;
            }

            DXGI_OUTPUT_DESC outputDesc = {0};
            pOutput->lpVtbl->GetDesc(pOutput, &outputDesc);

            pxGPU.MonitorHandle = outputDesc.Monitor;
            //= outputDesc.DesktopCoordinates;
            //= outputDesc.AttachedToDesktop;
            //= outputDesc.Rotation;

            PXTextCopyWU(outputDesc.DeviceName, sizeof(outputDesc.DeviceName), pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));

            pOutput->lpVtbl->Release(pOutput);

            // This can trigger nultible times?
            PXEngineRenderGPUMerge(pxEngineRender, &pxGPU);
        }  

        pAdapter->lpVtbl->Release(pAdapter);
        i++;
    }
    pFactory->lpVtbl->Release(pFactory);

    return pxResult;
}