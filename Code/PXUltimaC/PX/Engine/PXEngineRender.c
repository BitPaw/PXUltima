#include "PXEngineRender.h"

#include <PX/OS/Console/PXConsole.h>

#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>
#include <PX/Type/PXWindow.h>


#include <PX/Math/PXMath.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>
#include <PX/OS/Graphic/DirectX/PXDirectX.h>
#include <PX/OS/Graphic/Vulcan/PXVulcan.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/PXEngine.h>


#include <assert.h>

// #define QuadSkybox 0

const char PXInstantiateText[] = "Instantiate";

const char PXRenderingSystemText[] = "Renderer-PX";
const PXI8U PXRenderingSystemTextLength = sizeof(PXRenderingSystemText);
const PXECSRegisterInfoStatic PXRenderingSystemRegisterInfoStatic =
{
    {sizeof(PXRenderingSystemText), sizeof(PXRenderingSystemText), PXRenderingSystemText, TextFormatASCII},
    sizeof(PXEngineRender),
    __alignof(PXEngineRender),
    PXECSTypeSystem,
    PXEngineRenderCreate,
    PXEngineRenderRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXRenderingSystemRegisterInfoDynamic;


void PXAPI PXRenderingSystemRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXRenderingSystemRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXRenderingSystemRegisterInfoDynamic;
}

PXResult PXAPI PXEngineRenderCreate(PXEngineRender** pxEngineRenderREF, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo)
{
    PXEngineRender* pxEngineRender = PXNull;

    if(!(pxEngineRenderREF && pxEngineRenderCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineRenderCreateInfo->Info.Static = &PXRenderingSystemRegisterInfoStatic;
    pxEngineRenderCreateInfo->Info.Dynamic = &PXRenderingSystemRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineRenderREF, pxEngineRenderCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineRenderCreateInfo->RenderSystem = *pxEngineRenderREF;

    pxEngineRender = *pxEngineRenderREF;  

    PXThread PXREF pxThread = PXEngineRootThread(pxEngineRenderCreateInfo->Info.Engine);

    pxEngineRender->ModuleInfo.OnActivate = PXEngineRenderOnActivate;
    pxEngineRender->ModuleInfo.OnDeactivate = PXEngineRenderOnDeactivate;
    pxEngineRender->ModuleInfo.UseThreadOwnership = PXTrue;
    pxEngineRender->ModuleInfo.ThreadIDOwner = pxThread->ID;
    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineRender->ModuleInfo, PXEngineRenderUpdate);


    PXDictionaryECSCreate(&pxEngineRender->GPULookup);

    PXEngineRenderDisplayListRefresh(pxEngineRender);

    PXEngineRenderGPUListRefresh(pxEngineRender);

    PXEngineRenderGPUAffinityCreate(pxEngineRender, 0);

    PXEngineRenderHookAPI(pxEngineRender, pxEngineRenderCreateInfo);
  


    

    PXWindow PXREF pxWindow = pxEngineRenderCreateInfo->WindowReference;

    PXEngineUIEventPoll();
    PXWindowHide(pxWindow);
    PXWindowShow(pxWindow);
    PXEngineUIEventPoll();

    if(pxEngineRender->Select)
    {
        pxResult = pxEngineRender->Select(pxEngineRender->API, pxWindow);
    }     

    if(pxEngineRender->SwapIntervalSet)
    {
        pxResult = pxEngineRender->SwapIntervalSet(pxEngineRender->API, pxWindow, 1);
    }
       
    {
        PXRectangleXYWHI32 pxRectangleXYWH;
        PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWH);

        PXViewPort pxViewPort;
        pxViewPort.X = pxRectangleXYWH.X;
        pxViewPort.Y = pxRectangleXYWH.Y;
        pxViewPort.Width = pxRectangleXYWH.Width;
        pxViewPort.Height = pxRectangleXYWH.Height;
        pxViewPort.ClippingMinimum = 0;
        pxViewPort.ClippingMaximum = 1;

        if(pxEngineRender->ViewPortSet)
        {
            pxResult = pxEngineRender->ViewPortSet(pxEngineRender->API, &pxViewPort);
        }       
    }

    if(pxEngineRender->Clear)
    {
        PXColorRGBA_F32 color;
        color.Red = 0;
        color.Green = 1;
        color.Blue = 0;
        color.Alpha = 1;

        pxResult = pxEngineRender->Clear(pxEngineRender->API, &color);
    }

    PXWindowHandle windowHandle = PXWindowHandleGet(pxWindow);
    SetWindowPos(windowHandle.Data, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);


    // Unlink from this thread
    if(pxEngineRender->Deselect)
    {
        pxResult = pxEngineRender->Deselect(pxEngineRender->API, pxWindow);
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineRenderHookAPI(PXEngineRender PXREF pxEngineRender, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo)
{
    PXWindow PXREF pxWindow = pxEngineRenderCreateInfo->WindowReference;
    PXResult pxResult = PXResultInvalid;

#if PXLogEnable
    PXECSInfo* pxECSInfo = (PXECSInfo*)pxWindow;
    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    PXLogPrint
    (
        PXLoggingInfo,
        PXRenderingSystemText,
        PXInstantiateText,
        "Attempting to hook graphical API to access GPU.\n"
        "%20s : %i\n"
        "%20s : %p",
        "PXID", pxECSInfo->ID,
        "HANDLE", pxWindowHandle.Data
    );
#endif

    // Set pixel system
    {
        PXWindowPixelSystemInfo pxWindowPixelSystemInfo;
        PXClear(PXWindowPixelSystemInfo, &pxWindowPixelSystemInfo);
        pxWindowPixelSystemInfo.BitPerPixel = 32;
        pxWindowPixelSystemInfo.OpenGL = PXTrue;
        pxWindowPixelSystemInfo.DirectX = PXTrue;
        pxWindowPixelSystemInfo.GDI = PXFalse;

        pxResult = PXWindowPixelSystemSet(pxWindow, &pxWindowPixelSystemInfo);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }

    pxEngineRender->GraphicSystem = pxEngineRenderCreateInfo->GraphicSystem;

#if 0

    // void choose_ogl_vendor(const char* vendor_id)
    // {
    int idx;

    HDC dc;
    PIXELFORMATDESCRIPTOR pfd;

    dd.cb = sizeof(dd);
    idx = 0;
    while(1)
    {
        if(!EnumDisplayDevicesA(NULL, idx, &dd, 0))
            return; // not found!
        if(strstr(dd.DeviceID, vendor_pxOpenGL->Binding.AttachedWindowid))
            break; // there we go
        idx += 1;
    }

    dc = CreateDCA(dd.DeviceName, NULL, NULL, NULL);
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    // those flags are not important, they just need to be valid (and nondemanding, just in case).
    // later you will use whatever flags you wish when you are creating your actual gl context
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
    ChoosePixelFormat(dc, &pfd);
    DeleteDC(dc);
    //  }
#endif

    //-------------------------------------------------------------------------
    // Setup references
    //-------------------------------------------------------------------------
    switch(pxEngineRender->GraphicSystem)
    {
        case PXGraphicAPIOpenGL:
        {
            pxEngineRender->Initialize = (PXGPUInitializeFunction)PXOpenGLInitialize;
            break;
        }
        case PXGraphicAPIDirectX:
        {
            pxEngineRender->Initialize = (PXGPUInitializeFunction)PXDirectXInitialize;
            break;
        }
        case PXGraphicAPIVulcan:
        {
            pxEngineRender->Initialize = (PXGPUInitializeFunction)PXVulcanInitialize;
            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }

    // Graphic initialize
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXRenderingSystemText,
            PXInstantiateText,
            "Invoke specific API..."
        );

        pxResult = pxEngineRender->Initialize(pxEngineRender->API, pxEngineRenderCreateInfo);

        if(PXResultOK != pxResult) // if loading targeted API failed, load plan B
        {
            PXLogPrint
            (
                PXLoggingWarning,
                PXRenderingSystemText,
                PXInstantiateText,
                "Failure initializing API. Thinking about plan B"
            );

#if OSUnix
            return pxResult; // On linux systems, dont try a plan B, if opengl does not work, nothing will be
#elif OSWindows
            // On windows, try DirectX. some version will work, hopefully.

            pxEngineRender->GraphicSystem = PXGraphicAPIDirectX;
            pxEngineRender->Initialize = (PXGPUInitializeFunction)PXDirectXInitialize;

            pxEngineRenderCreateInfo->GraphicSystem = PXGraphicAPIDirectX;
            pxEngineRenderCreateInfo->DirectXVersion = PXDirectXVersionNewest;

            pxResult = pxEngineRender->Initialize(pxEngineRender->API, pxEngineRenderCreateInfo);

            if(PXResultOK != pxResult)
            {
                PXLogPrint
                (
                    PXLoggingError,
                    PXRenderingSystemText,
                    "Init",
                    "Failure initializing plan B. Exiting engine..."
                );

                return pxResult;
            }
#endif

        }
    }
    //-------------------------------------------------------------------------

    if(PXResultOK != pxResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXRenderingSystemText,
            PXInstantiateText,
            "Failed to create graphical instance!\n"
            "Graphics card driver is not able to provide a rendering context.\n"
            "Either the card itself is not capable or a driver is missing and needs to be installed."
        );
#endif

        return pxResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRenderingSystemText,
        PXInstantiateText,
        "OK, Created graphical instance"
    );
#endif

    return pxResult;
}

PXResult PXAPI PXEngineRenderRelease(PXEngineRender PXREF pxEngineRender)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineRenderOnActivate(PXEngineRender PXREF pxEngineRender, PXEngine PXREF pxEngine)
{
    PXEngineContext PXREF pxEngineContext = PXEngineContextGet(pxEngine);
    PXWindow PXREF pxWindow = PXEngineUIWindowRootGet(pxEngineContext->UI);

    PXResult pxResult = pxEngineRender->Select(pxEngineRender->API, pxWindow);

    return pxResult;
}

PXResult PXAPI PXEngineRenderOnDeactivate(PXEngineRender PXREF pxEngineRender, PXEngine PXREF pxEngine)
{
    PXEngineContext PXREF pxEngineContext = PXEngineContextGet(pxEngine);
    PXWindow PXREF pxWindow = PXEngineUIWindowRootGet(pxEngineContext->UI);

    PXResult pxResult = pxEngineRender->Deselect(pxEngineRender->API, pxWindow);

    return pxResult;
}

PXResult PXAPI PXEngineRenderUpdate(PXEngineRender PXREF pxEngineRender, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    PXResult pxResult = PXResultInvalid;

    PXEngineContext PXREF pxEngineContext = PXEngineContextGet(pxEngineUpdateInfo->Engine);
    PXWindow PXREF pxWindow = PXEngineUIWindowRootGet(pxEngineContext->UI);

    if(!pxWindow)
    {
        PXEngineModuleDeactivate(pxEngineRender, pxEngineUpdateInfo->Engine);
        return PXResultRefusedWindowDoesNotExist;
    }

   // pxResult = PXWindowDraw(pxWindow);

    return pxResult;
}

PXResult PXAPI PXEngineRenderDrawScene(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene)
{
    if(!(pxEngineRender && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    // We draw the skybox last.
    // Because filling pixels is very expensive and pixels
    // that are already "infront" of the skybox stay and will not be overritten.
    // This saves performance.

    // Update camera
    PXCamera PXREF pxCamera = PXSceneCameraCurrent(pxScene);

#if 0
    if(pxCamera)
    {
        PXRectangleXYWHI32 pxRectangleXYWHI32;

        PXWindow PXREF pxWindow = PXEngineWindowGet(pxEngineRender->Engine);

        PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWHI32);

        pxCamera->Rectangle.Width = pxRectangleXYWHI32.Width;
        pxCamera->Rectangle.Height = pxRectangleXYWHI32.Height;
    }
#endif

    // Render all models
    const PXSize modelAmount = PXSceneModelAmount(pxScene);

    for(PXSize i = 0; i < modelAmount; i++)
    {
        PXModel PXREF pxModel = PXSceneModelGet(pxScene, i);

        PXEngineRenderDrawModel(pxEngineRender, pxScene, pxModel);
    }

    PXSkyBox PXREF pxSkyBox = PXSceneSkyBoxGet(pxScene);

    PXEngineRenderDrawSkyBox(pxEngineRender, pxScene, pxSkyBox);

    return PXResultOK;
}

PXResult PXAPI PXEngineRenderDrawModel(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene, PXModel PXREF pxModel)
{
    pxEngineRender->ModelDraw(pxEngineRender->API, pxScene, pxModel);

    return PXResultOK;
}

PXResult PXAPI PXEngineRenderDrawSkyBox(PXEngineRender PXREF pxEngineRender, PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox)
{
    if(!(pxEngineRender && pxScene && pxSkyBox))
    {
        return PXResultRefusedParameterNull;
    }

    // 1. Allow the depth test to pass if pixels are LESS OR EQUAL (1.0)
    glDepthFunc(GL_LEQUAL);

    // 2. Disable depth writing so the skybox doesn't corrupt the depth buffer
    glDepthMask(GL_FALSE);

    // 3. Disable backface culling because the camera is INSIDE the cube mesh
    glDisable(GL_CULL_FACE);

    pxEngineRender->ModelDraw(pxEngineRender->API, pxScene, pxSkyBox->Model);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);

    return PXResultOK;
}







void PXAPI PXEngineUIElementColorSet4F(PXWindow PXREF pxWindow, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha)
{
    PXColorRGBA_F32* color = PXMemoryHeapCallocT(PXColorRGBA_F32, 1);

    color->Red = red;
    color->Green = green;
    color->Blue = blue;
    color->Alpha = alpha;

    //pxWindow->ColorTintReference = color;
}

void PXAPI PXEngineUIElementSizeSet(PXWindow PXREF pxWindow, const PXF32 x, const PXF32 y, const PXF32 width, const PXF32 height, const PXI32U  pxUIElementPositionMode)
{
    //pxWindow->X = x;
    //pxWindow->Y = y;
    // pxWindow->Width = width;
    // pxWindow->Height = height;

    // pxWindow->AncerFlagList = pxUIElementPositionMode;

    //PXRectangleOffsetSet(&pxWindow->Margin, x, y, width, height);

    // pxWindow->PositionMode = pxUIElementPositionMode;
}

PXResult PXAPI PXEngineRenderHotSwap(PXEngineRender PXREF pxEngineRender, const PXGraphicAPI pxGraphicAPI)
{
    // 1.) Store all elements in a cache

    // 2.) Release all elements

    // 3.) Release session

    // 4.) create new session

    // 5.) Restore all thing from cache

    // 6.) Delete cache

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXEngineRenderGPUMerge(PXEngineRender PXREF pxEngineRender, PXGPU PXREF pxGPU)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRenderingSystemText,
        "List",
        "GPU\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s",
        "Name", pxGPU->Name,
        "DisplayString", pxGPU->DeviceDisplayName,
        "DeviceID", pxGPU->DeviceIDName,
        "DeviceKey", pxGPU->DeviceKeyName,
        "Active", pxGPU->IsActive ? "YES" : "NO",
        "Connected", pxGPU->IsConnectedToMonitor ? "YES" : "NO"
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineRenderDisplayListRefresh(PXEngineRender PXREF pxEngineRender)
{
    if(!pxEngineRender)
    {
        return PXResultRefusedParameterNull;
    }

#if 0
    for(size_t i = 0; i < pxGraphic->DevicePhysicalListSize; i++)
    {
        PXGraphicDevicePhysical PXREF pxGraphicDevicePhysical = &pxGraphic->DevicePhysicalList[i];

        char targetBuffer[64];

        if(pxGraphicDevicePhysical->Driver[0] != '\0')
        {
            PXTextPrintA(targetBuffer, 64, "%s, %s (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->Driver, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }
        else
        {
            PXTextPrintA(targetBuffer, 64, "%s, (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }

        printf
        (
            "+---------------------------------------------------------+\n"
            "| Graphics Card (%2i/%-2i) - Information                     |\n"
            "+---------------------------------------------------------+\n"
            "| Model         : %-39s |\n"
            "| Target        : %-39s |\n"
            "| Vendor        : %-39s |\n"
            "| Renderer      : %-39s |\n"
            "| Shader-OpenGL : %-39s |\n"
            "+---------------------------------------------------------+\n\n",
            i + 1,
            pxGraphic->DevicePhysicalListSize,
            pxGraphicDevicePhysical->DeviceName,
            targetBuffer,
            pxGraphicDevicePhysical->Vendor,
            pxGraphicDevicePhysical->Renderer,
            pxGraphicDevicePhysical->Shader
        );
    }
#endif

    return PXResultOK;
}


PXResult PXAPI PXEngineRenderGPUListRefresh(PXEngineRender PXREF pxEngineRender)
{
    if(!pxEngineRender)
    {
        return PXResultRefusedParameterNull;
    }



#if OSUnix
#elif OSWindows
    DISPLAY_DEVICEW displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICEW);
    DWORD displayDeviceIndex = 0;

    for(;;)
    {
        const BOOL isOK = EnumDisplayDevicesW(NULL, displayDeviceIndex, &displayDevice, 0);

        if(!isOK)
        {
            break;
        }

        // Device detected!
        PXGPU pxGPU;
        PXClear(PXGPU, &pxGPU);
        PXTextCopyWU(displayDevice.DeviceString, sizeof(displayDevice.DeviceString), pxGPU.Name, sizeof(pxGPU.Name));
        PXTextCopyWU(displayDevice.DeviceName, sizeof(displayDevice.DeviceName), pxGPU.DeviceDisplayName, sizeof(pxGPU.DeviceDisplayName));
        PXTextCopyWU(displayDevice.DeviceID, sizeof(displayDevice.DeviceID), pxGPU.DeviceIDName, sizeof(pxGPU.DeviceIDName));
        PXTextCopyWU(displayDevice.DeviceKey, sizeof(displayDevice.DeviceKey), pxGPU.DeviceKeyName, sizeof(pxGPU.DeviceKeyName));

        pxGPU.IsActive = (DISPLAY_DEVICE_ACTIVE & displayDevice.StateFlags) > 0;
        pxGPU.IsConnectedToMonitor = (DISPLAY_DEVICE_ATTACHED_TO_DESKTOP & displayDevice.StateFlags) > 0;

        PXEngineRenderGPUMerge(pxEngineRender, &pxGPU);

        ++displayDeviceIndex;
    }

    PXDirectXGPUListRefresh(pxEngineRender);
#endif





    PXSize amount = PXEngineRenderGPUListAmount(pxEngineRender);
    for(PXSize i = 0; i < amount; i++)
    {
        PXGPU PXREF pxGPU = PXEngineRenderGPUGetViaIndex(pxEngineRender, i);


    }







    /*
for (size_t displayID = 0; displayID < EnumDisplayMonitors(PXNull, ); ++displayID)
{

}*/

// EnumDisplayDevicesA

// Fetch all graphical devices

//PXMonitorDeviceAmount(&pxGraphic->DevicePhysicalListSize);

    if(0)
    {
        // Allocate space for needed devices
        //PXNewList(PXGraphicDevicePhysical, pxGraphic->DevicePhysicalListSize, &pxGraphic->DevicePhysicalList, &pxGraphic->DevicePhysicalListSize);

        //PXPhysicalDeviceFetchAll(pxGraphic->DevicePhysicalList, pxGraphic->DevicePhysicalListSize);
    }


    return PXResultOK;
}
PXSize PXAPI PXEngineRenderGPUListAmount(const PXEngineRender PXREF pxEngineRender)
{
    return PXDictionaryECSAmount(&pxEngineRender->GPULookup);
}
PXGPU* PXAPI PXEngineRenderGPUGetViaIndex(const PXEngineRender PXREF pxEngineRender, const PXSize index)
{
    return (PXGPU*)PXDictionaryECSGetViaIndex(&pxEngineRender->GPULookup, index);
}
PXResult PXAPI PXEngineRenderGPUAffinityCreate(PXEngineRender PXREF pxEngineRender, const PXSize index)
{
    if(!pxEngineRender)
    {
        return PXResultRefusedParameterNull;
    }

#if 0
    // Create GPU affinity
    int selecvtDeviceID = 0;
    HDC dc = CreateDCA(graphicDevicePhysical[selecvtDeviceID].DeviceDisplay, graphicDevicePhysical[selecvtDeviceID].DeviceName, NULL, NULL); // "DISPLAY"

    auto xxxx = GetLastError();

    pxWindow->HandleDeviceContext = dc;
#endif

    return PXResultOK;
}



















