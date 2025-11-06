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


void PXAPI PXGraphicModelShaderSet(PXGraphic PXREF pxGraphic, PXRenderable PXREF renderable, const PXShaderProgram PXREF shaderPXProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment PXREF pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->Info.Handle.OpenGLID;
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

    return PXActionSuccessful;
}

void PXAPI PXTextureConstruct(PXTexture PXREF texture)
{
    PXClear(PXTexture, texture);

    texture->Info.Handle.OpenGLID = -1;

    texture->Filter = PXRenderFilterNoFilter;
    texture->LayoutNear = PXTextureLayoutNearest;
    texture->LayoutFar = PXTextureLayoutNearest;
    texture->WrapHeight = PXTextureWrapRepeat;
    texture->WrapWidth = PXTextureWrapRepeat;
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

    return PXActionSuccessful;
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

PXResult PXAPI PXFrameBufferCreate(PXFrameBuffer PXREF pxFrameBuffer, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo)
{
    switch(pxFrameBufferCreateInfo->System)
    {
        case PXGraphicSystemNative:
        {
            PXNativDrawFrameBufferCreate(PXNull, pxFrameBuffer, pxFrameBufferCreateInfo);           
            break;
        }
        default:
        {
            break;
        }
    }

    return PXActionSuccessful;
}

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
    assert(pxWindow->Info.Handle.WindowHandle);

    pxGraphic->WindowReference = pxGraphicInitializeInfo->WindowReference;

       
#if OSWindows
    // Under windows, get the device context handle, the access token to render into
    if(!pxWindow->DeviceContextHandle)
    {
        pxWindow->DeviceContextHandle = GetDC(pxWindow->Info.Handle.WindowHandle);
    }
#endif


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
        "PXID", pxWindow->Info.ID,
        "HANDLE", pxWindow->Info.Handle.WindowHandle,
        "DC", pxWindow->DeviceContextHandle

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

#if OSUnix
#elif OSWindows
        pxWindowPixelSystemInfo.HandleDeviceContext = pxWindow->DeviceContextHandle;
        pxWindowPixelSystemInfo.HandleWindow = pxWindow->Info.Handle.WindowHandle;
#endif

        

        const PXResult pixelSystem = PXGUIWindowPixelSystemSet(&pxWindowPixelSystemInfo);

        if(PXActionSuccessful != pixelSystem)
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
            return PXActionRefusedArgumentInvalid;
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

        PXActionResult pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

        if(PXActionSuccessful != pxActionResult) // if loading targeted API failed, load plan B
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

            if(PXActionSuccessful != pxActionResult)
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

    return PXActionSuccessful;
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

void PXAPI PXGraphicResourceRegister(PXGraphic PXREF pxGraphic, PXGraphicResourceInfo PXREF pxGraphicResourceInfo)
{

}

PXResult PXAPI PXGraphicSpriteConstruct(PXGraphic PXREF pxGraphic, PXSprite PXREF pxSprite)
{
    PXClear(PXSprite, pxSprite);

    PXModelConstruct(pxSprite->Model);


    //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
    //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

    PXVector2F32SetXY(&pxSprite->TextureScalePositionOffset, 1, 1);

    //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
}

void PXAPI PXCameraConstruct(PXCamera PXREF camera)
{
    PXClear(PXCamera, camera);

    camera->WalkSpeed = 2;
    camera->ViewSpeed = 1;
    camera->FollowSpeed = 0.98f;
    camera->FieldOfView = 90;
    camera->Height = 1;
    camera->Width = 1;
    camera->Near = -0.001;
    camera->Far = 100000;

    camera->LockMovement = 0;
    camera->LockView = 0;


    PXMatrix4x4FIdentity(&camera->MatrixModel);
    PXMatrix4x4FIdentity(&camera->MatrixView);
    PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(camera, PXCameraPerspective3D);

    const PXVector3F32 position = {0,0,0};
    PXCameraRotate(camera, &position);
}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera PXREF camera, const PXSize width, const PXSize height, const PXF32 nearPlane, const PXF32 farPlane)
{
    const PXF32 scaling = 0.005;
    const PXF32 left = -(width / 2.0f) * scaling;
    const PXF32 right = (width / 2.0f) * scaling;
    const PXF32 bottom = -(height / 2.0f) * scaling;
    const PXF32 top = (height / 2.0f) * scaling;

    camera->Perspective = PXCameraPerspective2D;
    camera->Width = width;
    camera->Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera PXREF camera, const PXF32 fieldOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane)
{
    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height)
{
    camera->Width = width;
    camera->Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

PXF32 PXAPI PXCameraAspectRatio(const PXCamera PXREF camera)
{
    return (PXF32)camera->Width / (PXF32)camera->Height;
}

void PXAPI PXCameraViewChange(PXCamera PXREF camera, const PXCameraPerspective cameraPerspective)
{
    camera->Perspective = cameraPerspective;

    switch (cameraPerspective)
    {
    case PXCameraPerspective2D:
    {
        PXCameraViewChangeToOrthographic(camera, camera->Width, camera->Height, camera->Near, camera->Far);
        break;
    }

    case PXCameraPerspective3D:
    {
        const PXF32 aspectRatio = PXCameraAspectRatio(camera);

        PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

        break;
    }
    }
}

void PXAPI PXCameraRotate(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F)
{
    if(camera->LockView)
    {
        return;
    }

    const PXF32 maxValue = 85.0f;
    const PXF32 minValue = -85.0f;

    camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
    camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

    camera->CurrentRotation.Y = PXMathLimit(camera->CurrentRotation.Y, minValue, maxValue);

    const PXF32 pitchRAD = PXMathDegreeToRadians(camera->CurrentRotation.Y);
    const PXF32 yawRAD = PXMathDegreeToRadians(camera->CurrentRotation.X);
    const PXF32 rx = PXMathCosinusRADF32(pitchRAD) * PXMathCosinusRADF32(yawRAD);
    const PXF32 ry = PXMathSinusRADF32(pitchRAD);
    const PXF32 rz = PXMathCosinusRADF32(pitchRAD) * PXMathSinusRADF32(yawRAD);

    PXVector3F32SetXYZ(&camera->LookAtPosition, rx, ry, rz);
    PXVector3F32Normalize(&camera->LookAtPosition);
}

void PXAPI PXCameraRotateXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z)
{
    const PXVector3F32 vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z)
{
    const PXVector3F32 vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F)
{
    if(camera->LockMovement)
    {
        return;
    }

    PXVector3F32 xAxis = { 0,0,0 };
    const PXVector3F32 yAxis = { 0, vector3F->Y, 0 };
    PXVector3F32 zAxis = { 0,0,0 };

    // ...
    {
        const PXVector3F32 up = { 0, 1, 0 };
        const PXVector3F32 lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

        PXVector3F32CrossProduct(&xAxis, &lookAtPosition, &up);
        PXVector3F32Normalize(&xAxis);
        PXVector3F32MultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X);

        zAxis = lookAtPosition;

        PXVector3F32Normalize(&zAxis);
        PXVector3F32MultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z);
    }

    {
        PXVector3F32 targetedMovement = { 0,0,0 };

        PXVector3F32Add(&targetedMovement, &xAxis);
        PXVector3F32Add(&targetedMovement, &yAxis);
        PXVector3F32Add(&targetedMovement, &zAxis);
        PXVector3F32MultiplyS(&targetedMovement, camera->WalkSpeed);

        PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement);
    }
}

void PXAPI PXCameraFollow(PXCamera PXREF camera, const PXF32 deltaTime)
{
    PXVector3F32 positionCurrent;
    PXVector3F32 positionDesired;

    PXVector3F32 rotationCurrent;
    PXVector3F32 rotationDesired;

    PXVector3F32 positionDelta;
    PXVector3F32 rotationDelta;

    if (!camera->Target)
    {
        return;
    }

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &positionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &positionDesired); // get current target pos



    PXMatrix4x4FRotationGet(&camera->MatrixView, &rotationCurrent); // Get current camera pos
    PXMatrix4x4FRotationGet(camera->Target, &rotationDesired); // get current target pos

    camera->FollowSpeed = 12.3;

    //PXVector3F32Add(&positionCurrent, &camera->Offset); // add offset to target pos
    PXVector3F32Add(&positionDesired, &camera->Offset); // add offset to target pos


    PXVector3F32 eye = {0,0,0};
    PXVector3F32 center = {0,0,0};
    PXVector3F32 up = {0,1,0};

    // PXMatrix4x4FLookAt(&camera->MatrixModel, &eye, &desiredPosition, &up);

    PXVector3F32Interpolate(&positionDesired, &positionCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement
    PXVector3F32Interpolate(&rotationDesired, &rotationCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement

#if 0
    // Not how i want it.
    // Problem: it snaps to strong if you get in range, then no movement until were too far away again.
    // We need a deadzone- then a softstart, then a rampup the further away we are

    PXVector3F32Set(&positionDelta, &positionCurrent);
    PXVector3F32Substract(&positionDelta, &positionDesired);
    PXVector3F32Absolute(&positionDelta);

    PXVector3F32Set(&rotationDelta, &rotationCurrent);
    PXVector3F32Substract(&rotationDelta, &rotationDesired);
    PXVector3F32Absolute(&positionDelta);

    if(positionDelta.X < camera->DeadZone.X)
    {
        // Cancel X movement
        positionDesired.X = positionCurrent.X;
    }

    if(positionDelta.Y < camera->DeadZone.Y)
    {
        // Cancel Y movement
        positionDesired.Y = positionCurrent.Y;
    }

    if(positionDelta.Z < camera->DeadZone.Z)
    {
        // Cancel Z movement
        positionDesired.Z = positionCurrent.Z;
    }
#endif



    PXMatrix4x4FPositionSet(&camera->MatrixModel, &positionDesired); // Set delte movement
    PXMatrix4x4FRotationSet(&camera->MatrixView, &rotationDesired);


    PXVector3F32 rotation;

    PXMatrix4x4FRotationGet(&camera->MatrixView, &rotation);

#if 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Camera",
        "Follow",
        "x:%6.2f y:%6.2f z:%6.2f, Yaw:%6.2f Pitch:%6.2f Roll:%6.2f",
        camera->Target->Data[TransformX],
        camera->Target->Data[TransformY],
        camera->Target->Data[TransformZ],
        PXMathRadiansToDegree(rotation.X),
        PXMathRadiansToDegree(rotation.Y),
        PXMathRadiansToDegree(rotation.Z)
    );
#endif
}

void PXAPI PXCameraUpdate(PXCamera PXREF camera, const PXF32 deltaTime)
{
    const PXF32 walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
    const PXF32 viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
    const PXVector3F32 up = { 0,1,0 };
    PXVector3F32 currentPosition;
    PXVector3F32 centerPosition;

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    centerPosition = currentPosition;
    PXVector3F32Add(&centerPosition, &camera->LookAtPosition);

    PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up);
}
