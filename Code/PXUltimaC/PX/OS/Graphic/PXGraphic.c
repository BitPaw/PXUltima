#include "PXGraphic.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>
#include <PX/OS/Graphic/DirectX/PXDirectX.h>
#include <PX/OS/Graphic/Vulcan/PXVulcan.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/PXGUI.h>

#include <assert.h>

#define QuadSkybox 0


const char PXGraphicText[] = "Graphic";
const char PXGraphicInitializeText[] = "Initialize";


void PXAPI PXGraphicModelShaderSet(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXShaderProgram PXREF pxShaderProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment PXREF pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = pxShaderProgram->OpenGLID;
    }
}

PXResult PXAPI PXGraphicUIRectangleCreate(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
{
    PXMatrix4x4FMoveXY(&renderable->MatrixModel, x, y);
    // PXMatrix4x4FScaleXYZSet(&renderable->MatrixModel, sidth, height, 1);

    // Register rectangle
    {
        //PXF32

        //GraphicModelRegisterFromData(pxGraphic, renderable, , );

    }

    return PXResultOK;
}

/*

PXResult PXAPI PXGraphicUIElementCreate(PXGraphic PXREF pxGraphic, PXWindow* PXREF pxWindow, const PXSize amount, PXWindow PXREF pxUIElementParrent)
{


    //PXClear(PXWindow, *pxWindow);



    //*pxWindow->Type = pxUIElementType;
    //*pxWindow->TextureReference = PXNull;
    //*pxWindow->ShaderReference = PXNull;

    //*pxWindow->NameTextScale = 1;

    PXUIElementColorSet4F(*pxWindow, 1, 1, 1, 1);

   // PXTextCopyA("[N/A]", 5, pxWindow->Name, 32);

    if (pxUIElementParrent)
    {
        // Impossible if already has child

        (*pxWindow)->Parent = pxUIElementParrent; // "It's alway my parent"

        const PXBool alreadyHasAFirstBorn = pxUIElementParrent->Child != PXNull;

        if (alreadyHasAFirstBorn)
        {
            // Add as sibling for firstborn
            PXWindow* targetSibling = (*pxWindow)->Parent->Child;

            // Search for last sibling to be
            while (targetSibling->Sibling != PXNull)
            {
                targetSibling = targetSibling->Sibling;
            }

            targetSibling->Sibling = (*pxWindow); // Add sibling
        }
        else
        {
            // Add as child
            pxUIElementParrent->Child = (*pxWindow);
        }
    }
    else
    {
        if (pxGraphic->UIElementBase.Child) // if Has child, everyone will be sibling from child
        {
            PXWindow* insertionNode = pxGraphic->UIElementBase.Child;

            while (insertionNode->Sibling != PXNull)
            {
                insertionNode = insertionNode->Sibling;
            }

            insertionNode->Sibling = (*pxWindow); // I am your sibbling
        }
        else
        {
            (*pxWindow)->Parent = &pxGraphic->UIElementBase; // You are my parent now
            pxGraphic->UIElementBase.Child = (*pxWindow); // As I am your child
        }
    }

    return PXResultOK;
}
*/


void PXAPI PXRenderableConstruct(PXRenderable PXREF pxRenderable)
{
    PXClear(PXRenderable, pxRenderable);

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXAPI PXUIElementColorSet4F(PXWindow PXREF pxWindow, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha)
{
    PXColorRGBAF* color = PXMemoryHeapCallocT(PXColorRGBAF, 1);

    color->Red = red;
    color->Green = green;
    color->Blue = blue;
    color->Alpha = alpha;

    //pxWindow->ColorTintReference = color;
}

void PXAPI PXUIElementSizeSet(PXWindow PXREF pxWindow, const PXF32 x, const PXF32 y, const PXF32 width, const PXF32 height, const PXI32U  pxUIElementPositionMode)
{
    //pxWindow->X = x;
    //pxWindow->Y = y;
    // pxWindow->Width = width;
    // pxWindow->Height = height;

    // pxWindow->AncerFlagList = pxUIElementPositionMode;

    //PXRectangleOffsetSet(&pxWindow->Margin, x, y, width, height);

    // pxWindow->PositionMode = pxUIElementPositionMode;
}


void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment PXREF pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = PXDrawModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}


PXGraphic* _pxGraphicGLOBAL;

PXGraphic* PXAPI PXGraphicInstantiateGET(void)
{
    return _pxGraphicGLOBAL;
}

PXResult PXAPI PXGraphicInstantiate(PXGraphic PXREF pxGraphic, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo)
{
    _pxGraphicGLOBAL = pxGraphic;

    assert(pxGraphic);
    assert(pxGraphicInitializeInfo);


    PXWindow* pxWindow = pxGraphicInitializeInfo->WindowReference;

    assert(pxWindow);

    pxGraphic->WindowReference = pxGraphicInitializeInfo->WindowReference;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGraphicText,
        PXGraphicInitializeText,
        "Creating context on..\n"
        "%20s : %i\n"
        "%20s : %p\n"
        "%20s : %p",
        "PXID", 0,// pxWindow->Info.ID,
        "HANDLE", 0,//pxWindow->Info.Handle.WindowHandle,
        "DC", 0//pxWindow->DeviceContextHandle

    );
#endif

    //pxGraphic->AttachedWindow = pxGraphicInitializeInfo->WindowReference;
    pxGraphic->GraphicSystem = pxGraphicInitializeInfo->GraphicSystem;

    pxGraphicInitializeInfo->Graphic = pxGraphic;

#if OSUnix
    pxGraphic->DisplayConnection = pxGraphicInitializeInfo->DisplayConnection;
#endif


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


#if 0
    // Create GPU affinity
    int selecvtDeviceID = 0;
    HDC dc = CreateDCA(graphicDevicePhysical[selecvtDeviceID].DeviceDisplay, graphicDevicePhysical[selecvtDeviceID].DeviceName, NULL, NULL); // "DISPLAY"

    auto xxxx = GetLastError();

    pxWindow->HandleDeviceContext = dc;
#endif

    // Set pixel system
    {
        PXWindowPixelSystemInfo pxWindowPixelSystemInfo;
        PXClear(PXWindowPixelSystemInfo, &pxWindowPixelSystemInfo);
        pxWindowPixelSystemInfo.BitPerPixel = 32;
        pxWindowPixelSystemInfo.OpenGL = PXTrue;
        pxWindowPixelSystemInfo.DirectX = PXTrue;
        pxWindowPixelSystemInfo.GDI = PXFalse;

        const PXResult pixelSystem = PXWindowPixelSystemSet(pxWindow, &pxWindowPixelSystemInfo);

        if(PXResultOK != pixelSystem)
        {
            return pixelSystem;
        }
    }


#if 0

    // void choose_ogl_vendor(const char* vendor_id)
    // {
    int idx;

    HDC dc;
    PIXELFORMATDESCRIPTOR pfd;

    dd.cb = sizeof(dd);
    idx = 0;
    while (1)
    {
        if (!EnumDisplayDevicesA(NULL, idx, &dd, 0))
            return; // not found!
        if (strstr(dd.DeviceID, vendor_pxOpenGL->Binding.AttachedWindowid))
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
    switch(pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            pxGraphic->EventOwner = &pxGraphic->OpenGLInstance;
            pxGraphic->Initialize = (PXGraphicInitializeFunction)PXOpenGLInitialize;
            break;
        }
        case PXGraphicSystemDirectX:
        {
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;
            pxGraphic->Initialize = (PXGraphicInitializeFunction)PXDirectXInitialize;
            break;
        }
        case PXGraphicSystemVulcan:
        {
            pxGraphic->EventOwner = &pxGraphic->VulcanInstance;
            pxGraphic->Initialize = (PXGraphicInitializeFunction)PXVulcanInitialize;
            break;
        }

        default:
            return PXResultRefusedParameterInvalid;
    }

    // Graphic initialize
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXGraphicText,
            PXGraphicInitializeText,
            "Invoke spesific API..."
        );

        PXResult pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

        if(PXResultOK != pxActionResult) // if loading targeted API failed, load plan B
        {
            PXLogPrint
            (
                PXLoggingWarning,
                PXGraphicText,
                PXGraphicInitializeText,
                "Failure initializing API. Thinking about plan B"
            );

#if OSUnix
            return pxActionResult; // On linux systems, dont try a plan B, if opengl does not work, nothing will be
#elif OSWindows
            // On windows, try DirectX. some version will work, hopefully.


            pxGraphic->GraphicSystem = PXGraphicSystemDirectX;
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;
            pxGraphic->Initialize = (PXGraphicInitializeFunction)PXDirectXInitialize;

            pxGraphicInitializeInfo->GraphicSystem = PXGraphicSystemDirectX;
            pxGraphicInitializeInfo->DirectXVersion = PXDirectXVersionNewest;

            pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

            if(PXResultOK != pxActionResult)
            {
                PXClear(PXGraphic, pxGraphic);

                PXLogPrint
                (
                    PXLoggingError,
                    PXGraphicText,
                    PXGraphicInitializeText,
                    "Failure initializing plan B. Exiting engine..."
                );

                return pxActionResult;
            }
#endif

        }
    }

    //-------------------------------------------------------------------------

#if 0
    for (size_t i = 0; i < pxGraphic->DevicePhysicalListSize; i++)
    {
        PXGraphicDevicePhysical PXREF pxGraphicDevicePhysical = &pxGraphic->DevicePhysicalList[i];

        char targetBuffer[64];

        if (pxGraphicDevicePhysical->Driver[0] != '\0')
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
            i+1,
            pxGraphic->DevicePhysicalListSize,
            pxGraphicDevicePhysical->DeviceName,
            targetBuffer,
            pxGraphicDevicePhysical->Vendor,
            pxGraphicDevicePhysical->Renderer,
            pxGraphicDevicePhysical->Shader
        );
    }
#endif



#if 0
    PXViewPort pxViewPort;
    pxViewPort.X = 0;
    pxViewPort.Y = 0;
    pxViewPort.Width = pxGraphic->AttachedWindow->Width;
    pxViewPort.Height = pxGraphic->AttachedWindow->Height;
    pxViewPort.ClippingMinimum = 0;
    pxViewPort.ClippingMaximum = 1;

    pxGraphic->ViewPortSet(pxGraphic->EventOwner, &pxViewPort);
#endif

    //PXMatrix4x4FIdentity(&pxGraphic->SpriteScaled.ModelMatrix);
    // PXMatrix4x4FIdentity(&pxGraphic->SpriteUnScaled.ModelMatrix);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGraphicText,
        PXGraphicInitializeText,
        "Finished"
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXGraphicRelease(PXGraphic PXREF pxGraphic)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXGraphicHotSwap(PXGraphic PXREF pxGraphic, const PXGraphicSystem pxGraphicSystem)
{
    // 1.) Store all elements in a cache

    // 2.) Release all elements

    // 3.) Release session

    // 4.) create new session

    // 5.) Restore all thing from cache

    // 6.) Delete cache

    return PXActionRefusedNotImplemented;
}