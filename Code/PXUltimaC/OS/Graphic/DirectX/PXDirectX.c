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
    PXGraphic* const pxGraphic = pxGraphicInitializeInfo->Graphic;

    PXClear(PXDirectX, pxDirectX);

#if PXOSWindowsDestop
    //HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    //UINT              Flags = 0;

    pxDirectX->DirectXVersion = pxGraphicInitializeInfo->DirectXVersion;

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
            pxGraphic->Initialize = PXDirectX10x1Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X10x1;
            break;
        }
        case PXDirectXVersion10x0:
        {
            pxGraphic->Initialize = PXDirectX10Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X10;
            break;
        }
        case PXDirectXVersion9:
        {
            pxGraphic->Initialize = PXDirectX9Initialize;
            pxDirectX->DXTargetAPI = &pxDirectX->X9;
            break;
        }
        default:
            return PXActionRefusedFormatNotSupported;
    }

    const PXActionResult initializeResult = pxGraphic->Initialize(pxDirectX->DXTargetAPI, pxGraphicInitializeInfo);

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXDirectXRelease(PXDirectX* const pxDirectX)
{
    return PXActionNotSupportedByOperatingSystem;

   // return  pxDirectX->Release(pxDirectX->DXTargetAPI);
}