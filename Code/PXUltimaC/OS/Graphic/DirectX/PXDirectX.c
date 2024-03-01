#include "PXDirectX.h"

#if OSUnix
#elif OSWindows
#include <windows.h>
#endif

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Window/PXWindow.h>
#include <OS/Console/PXConsole.h>

#include "PXDirectX9.h"
#include "PXDirectX10.h"
#include "PXDirectX10x1.h"
#include "PXDirectX11.h"
#include "PXDirectX12.h"

PXActionResult PXAPI PXDirectXInitialize(PXDirectX* const pxDirectX, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX",
        "Initialize",
        "---Start---"
    );

    PXGraphic* const pxGraphic = pxGraphicInitializeInfo->Graphic;

    PXClear(PXDirectX, pxDirectX);

#if PXOSWindowsDestop
    //HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    //UINT              Flags = 0;

    pxDirectX->DirectXVersion = pxGraphicInitializeInfo->DirectXVersion;

    PXBool directXCreated = PXFalse;

    if(PXDirectXVersionNewest == pxDirectX->DirectXVersion)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX",
            "Initialize",
            "Brute force newest version, testing each one."
        );

        const PXSize amountOfSolutions = 5;
        const PXGraphicInitializeFunction directXSolutions[5] =
        {
            PXDirectX12Initialize,
            PXDirectX11Initialize,
            PXDirectX10x1Initialize,
            PXDirectX10Initialize,
            PXDirectX9Initialize
        };
        const void* directXSolutionsObject[5] =
        {
            &pxDirectX->X12,
            &pxDirectX->X11,
            &pxDirectX->X10x1,
            &pxDirectX->X10,
            &pxDirectX->X9
        };

        for(PXSize i = 0; i < 5; ++i)
        {
            pxGraphic->Initialize = directXSolutions[i];
            pxDirectX->DXTargetAPI = directXSolutionsObject[i];

            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX",
                "Initialize",
                "Calling spesific API. Try <%i%i>",
                i+1,
                amountOfSolutions
            );

            const PXActionResult initializeResult = pxGraphic->Initialize(pxDirectX->DXTargetAPI, pxGraphicInitializeInfo);

            if(PXActionSuccessful == initializeResult)
            {
                directXCreated = PXTrue;
                break;
            }
        }

        pxGraphic->Initialize = PXNull;
        pxDirectX->DXTargetAPI = PXNull;
    }

    if(!directXCreated)
    {
        return PXActionFailedInitialization;
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
                "DirectX",
                "Initialize",
                "Select DirectX 12"
            );

            pxGraphic->Initialize = PXDirectX12Initialize;
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
                "DirectX",
                "Initialize",
                "Select DirectX 11"
            );

            pxGraphic->Initialize = PXDirectX11Initialize;
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
                "DirectX",
                "Initialize",
                "Select DirectX 10.1"
            );

            pxGraphic->Initialize = PXDirectX10x1Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X10x1;
            break;
        }
        case PXDirectXVersion10x0:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX",
                "Initialize",
                "Select DirectX 10"
            );

            pxGraphic->Initialize = PXDirectX10Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X10;
            break;
        }
        case PXDirectXVersion9:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX",
                "Initialize",
                "Select DirectX 9"
            );

            pxGraphic->Initialize = PXDirectX9Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X9;
            break;
        }
        default:
            return PXActionRefusedFormatNotSupported;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX",
        "Initialize",
        "Calling spesific API"
    );

    const PXActionResult initializeResult = pxGraphic->Initialize(pxDirectX->DXTargetAPI, pxGraphicInitializeInfo);

    return initializeResult;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXDirectXRelease(PXDirectX* const pxDirectX)
{
    return PXActionNotSupportedByOperatingSystem;

   // return  pxDirectX->Release(pxDirectX->DXTargetAPI);
}