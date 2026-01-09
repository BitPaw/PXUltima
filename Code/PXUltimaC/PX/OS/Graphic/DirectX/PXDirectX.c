#include "PXDirectX.h"

#if OSUnix
#elif OSWindows
#include <windows.h>
#endif

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>

#include "PXDirectX9.h"
#include "PXDirectX10.h"
#include "PXDirectX10x1.h"
#include "PXDirectX11.h"
#include "PXDirectX12.h"

const char PXDirectXText[] = "DirectX";

PXResult PXAPI PXDirectXInitialize(PXDirectX PXREF pxDirectX, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo)
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

    PXGraphic PXREF pxGraphic = pxGraphicInitializeInfo->Graphic;

    PXClear(PXDirectX, pxDirectX);

#if PXOSWindowsDestop
    //HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    //UINT              Flags = 0;

    pxDirectX->DirectXVersion = pxGraphicInitializeInfo->DirectXVersion;

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

        const PXSize amountOfSolutions = 2;
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

        for(PXSize i = 0; i < amountOfSolutions; ++i)
        {
            PXClear(PXDirectX, pxDirectX);

            pxGraphic->Initialize = directXSolutions[i];
            pxDirectX->DXTargetAPI = directXSolutionsObject[i];

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDirectXText,
                "Initialize",
                "Calling spesific API. Try <%i/%i>",
                i+1,
                amountOfSolutions
            );
#endif

            const PXResult initializeResult = pxGraphic->Initialize(pxDirectX->DXTargetAPI, pxGraphicInitializeInfo);

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
                    amountOfSolutions
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
                amountOfSolutions
            );
        }

        pxGraphic->Initialize = PXNull;
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
            PXDirectXText,
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
            PXDirectXText,
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
            PXDirectXText,
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
            PXDirectXText,
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
            PXDirectXText,
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
        PXDirectXText,
        "Initialize",
        "Calling spesific API"
    );

    const PXResult initializeResult = pxGraphic->Initialize(pxDirectX->DXTargetAPI, pxGraphicInitializeInfo);

    return initializeResult;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXDirectXRelease(PXDirectX PXREF pxDirectX)
{
    return PXResultInvalid;

    // return  pxDirectX->Release(pxDirectX->DXTargetAPI);
}
