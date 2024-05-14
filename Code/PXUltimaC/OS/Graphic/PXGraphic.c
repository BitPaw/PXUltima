#include "PXGraphic.h"

#include <Media/PXText.h>
#include <Media/ADLER/PXAdler32.h>
#include <Math/PXMath.h>
#include <OS/File/PXFile.h>
#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>
#include <OS/GUI/PXGUI.h>
#include <OS/Console/PXConsole.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Hardware/PXMonitor.h>

#include <stdarg.h>

#define QuadSkybox 0

void PXAPI PXGraphicModelShaderSet(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->Info.OpenGLID;
    }
}

PXActionResult PXAPI PXGraphicUIRectangleCreate(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
{
    PXMatrix4x4FMoveXY(&renderable->MatrixModel, x, y);
   // PXMatrix4x4FScaleXYZSet(&renderable->MatrixModel, sidth, height, 1);

    // Register rectangle
    {
        //float

        //GraphicModelRegisterFromData(pxGraphic, renderable, , );

    }

    return PXActionSuccessful;
}

void PXAPI PXTextureConstruct(PXTexture2D* const texture)
{
    PXClear(PXTexture2D, texture);

    texture->Info.OpenGLID = -1;

    texture->Filter = PXGraphicRenderFilterNoFilter;
    texture->LayoutNear = PXGraphicImageLayoutNearest;
    texture->LayoutFar = PXGraphicImageLayoutNearest;
    texture->WrapHeight = PXGraphicImageWrapRepeat;
    texture->WrapWidth = PXGraphicImageWrapRepeat;
}

/*

PXActionResult PXAPI PXGraphicUIElementCreate(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement, const PXSize amount, PXUIElement* const pxUIElementParrent)
{


    //PXClear(PXUIElement, *pxUIElement);



    //*pxUIElement->Type = pxUIElementType;
    //*pxUIElement->TextureReference = PXNull;
    //*pxUIElement->ShaderReference = PXNull;

    //*pxUIElement->NameTextScale = 1;

    PXUIElementColorSet4F(*pxUIElement, 1, 1, 1, 1);

   // PXTextCopyA("[N/A]", 5, pxUIElement->Name, 32);

    if (pxUIElementParrent)
    {
        // Impossible if already has child

        (*pxUIElement)->Parent = pxUIElementParrent; // "It's alway my parent"

        const PXBool alreadyHasAFirstBorn = pxUIElementParrent->Child != PXNull;

        if (alreadyHasAFirstBorn)
        {
            // Add as sibling for firstborn
            PXUIElement* targetSibling = (*pxUIElement)->Parent->Child;

            // Search for last sibling to be
            while (targetSibling->Sibling != PXNull)
            {
                targetSibling = targetSibling->Sibling;
            }

            targetSibling->Sibling = (*pxUIElement); // Add sibling
        }
        else
        {
            // Add as child
            pxUIElementParrent->Child = (*pxUIElement);
        }    
    }
    else
    { 
        if (pxGraphic->UIElementBase.Child) // if Has child, everyone will be sibling from child
        {
            PXUIElement* insertionNode = pxGraphic->UIElementBase.Child;

            while (insertionNode->Sibling != PXNull)
            {
                insertionNode = insertionNode->Sibling;
            }

            insertionNode->Sibling = (*pxUIElement); // I am your sibbling
        }
        else
        {
            (*pxUIElement)->Parent = &pxGraphic->UIElementBase; // You are my parent now
            pxGraphic->UIElementBase.Child = (*pxUIElement); // As I am your child
        }
    }

    return PXActionSuccessful;
}
*/


void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable)
{
    PXClear(PXRenderable, pxRenderable);

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXAPI PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha)
{
    PXColorRGBAF* color = PXNull;
    PXNewZerod(PXColorRGBAF, &color);

    color->Red = red;
    color->Green = green;
    color->Blue = blue;
    color->Alpha = alpha;

    pxUIElement->ColorTintReference = color;
}

void PXAPI PXUIElementSizeSet(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXInt32U  pxUIElementPositionMode)
{
    //pxUIElement->X = x;
    //pxUIElement->Y = y;
    // pxUIElement->Width = width;
   // pxUIElement->Height = height;

   // pxUIElement->AncerFlagList = pxUIElementPositionMode;

    //PXRectangleOffsetSet(&pxUIElement->Margin, x, y, width, height);

   // pxUIElement->PositionMode = pxUIElementPositionMode;
}


void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = PXGraphicDrawModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}

PXActionResult PXAPI PXGraphicInstantiate(PXGraphic* const pxGraphic, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Graphic",
        "Instantiate",
        "Creating context on <%x>",
        pxGraphicInitializeInfo->WindowReference->Info.WindowID
    );
#endif

    //pxGraphic->AttachedWindow = pxGraphicInitializeInfo->WindowReference;
    pxGraphic->GraphicSystem = pxGraphicInitializeInfo->GraphicSystem;
    pxGraphic->WindowReference = pxGraphicInitializeInfo->WindowReference;
    pxGraphicInitializeInfo->Graphic = pxGraphic;

    // Get Device context if not already done
    if(!pxGraphicInitializeInfo->HandleDeviceContext)
    {
        pxGraphicInitializeInfo->HandleDeviceContext = GetDC(pxGraphic->WindowReference->Info.WindowID);
    }


    
    /*
    for (size_t displayID = 0; displayID < EnumDisplayMonitors(PXNull, ); ++displayID)
    {

    }*/

    // EnumDisplayDevicesA

    // Fetch all graphical devices 

    PXMonitorDeviceAmount(&pxGraphic->DevicePhysicalListSize);

    if(0)
    {      
        // Allocate space for needed devices
        PXNewList(PXGraphicDevicePhysical, pxGraphic->DevicePhysicalListSize, &pxGraphic->DevicePhysicalList, &pxGraphic->DevicePhysicalListSize);

        PXPhysicalDeviceFetchAll(pxGraphic->DevicePhysicalList, pxGraphic->DevicePhysicalListSize);
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
        pxWindowPixelSystemInfo.HandleDeviceContext = pxGraphicInitializeInfo->HandleDeviceContext;
        pxWindowPixelSystemInfo.HandleWindow = pxGraphic->WindowReference->Info.WindowID;
        pxWindowPixelSystemInfo.BitPerPixel = 32;
        pxWindowPixelSystemInfo.OpenGL = PXTrue;
        pxWindowPixelSystemInfo.DirectX = PXTrue;
        pxWindowPixelSystemInfo.GDI = PXFalse;

        const PXActionResult pixelSystem = PXWindowPixelSystemSet(&pxWindowPixelSystemInfo);

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
            if (strstr(dd.DeviceID, vendor_pxOpenGL->AttachedWindowid))
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
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            pxGraphic->EventOwner = &pxGraphic->OpenGLInstance;
            pxGraphic->Initialize = PXOpenGLInitialize;
            break;
        }
        case PXGraphicSystemDirectX:
        {
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;
            pxGraphic->Initialize = PXDirectXInitialize;
            break;
        }
        case PXGraphicSystemVulcan:
        {
            pxGraphic->EventOwner = &pxGraphic->VulcanInstance;
            pxGraphic->Initialize = PXVulcanInitialize;
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
            "Graphic",
            "Initialize",
            "Invoke spesific API..."
        );

        PXActionResult pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

        if(PXActionSuccessful != pxActionResult) // if loading targeted API failed, load plan B
        {
            PXLogPrint
            (
                PXLoggingWarning,
                "Graphic",
                "Initialize",
                "Failure initializing API. Thinking about plan B"
            );

#if OSUnix 
            return pxActionResult; // On linux systems, dont try a plan B, if opengl does not work, nothing will be
#elif OSWindows
            // On windows, try DirectX. some version will work, hopefully.


            pxGraphic->GraphicSystem = PXGraphicSystemDirectX;
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;
            pxGraphic->Initialize = PXDirectXInitialize;

            pxGraphicInitializeInfo->GraphicSystem = PXGraphicSystemDirectX;
            pxGraphicInitializeInfo->DirectXVersion = PXDirectXVersionNewest;

            pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

            if(PXActionSuccessful != pxActionResult)
            {
                PXClear(PXGraphic, pxGraphic);

                PXLogPrint
                (
                    PXLoggingError,
                    "Graphic",
                    "Initialize",
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
        PXGraphicDevicePhysical* const pxGraphicDevicePhysical = &pxGraphic->DevicePhysicalList[i];

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
        "Graphic",
        "Init",
        "Finished"
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicRelease(PXGraphic* const pxGraphic)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXGraphicHotSwap(PXGraphic* const pxGraphic, const PXGraphicSystem pxGraphicSystem)
{
    // 1.) Store all elements in a cache

    // 2.) Release all elements

    // 3.) Release session

    // 4.) create new session

    // 5.) Restore all thing from cache

    // 6.) Delete cache

    return PXActionRefusedNotImplemented;
}

void PXAPI PXGraphicResourceRegister(PXGraphic* const pxGraphic, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{

}

PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphic* const pxGraphic, PXSprite* const pxSprite)
{
    PXClear(PXSprite, pxSprite);

    PXModelConstruct(&pxSprite->Model);


    //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
    //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

    PXVector2FSetXY(&pxSprite->TextureScalePositionOffset, 1, 1);

  //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
}

PXActionResult PXAPI PXGraphicSpriteDraw(PXGraphic* const pxGraphic, const PXSprite* const pxSprite, const PXCamera* const pxCamera)
{
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            return PXOpenGLSpriteDraw(&pxGraphic->OpenGLInstance, pxSprite, pxCamera);
        }
        case PXGraphicSystemDirectX:
        {
            return PXActionNotSupportedByLibrary;
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXAPI PXCameraConstruct(PXCamera* const camera)
{
    PXClear(PXCamera, camera);

    camera->WalkSpeed = 0.2;
    camera->ViewSpeed = 0.4;
    camera->FollowSpeed = 0.98f;
    camera->FieldOfView = 90;
    camera->Height = 1;
    camera->Width = 1;
    camera->Near = -0.001;
    camera->Far = 100000;

    PXMatrix4x4FIdentity(&camera->MatrixModel);
    PXMatrix4x4FIdentity(&camera->MatrixView);
    PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(camera, PXCameraPerspective3D);

    const PXVector3F position = {0,0,0};
    PXCameraRotate(camera, &position);
}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera* const camera, const PXSize width, const PXSize height, const float nearPlane, const float farPlane)
{
    const float scaling = 0.005;
    const float left = -(width / 2.0f) * scaling;
    const float right = (width / 2.0f) * scaling;
    const float bottom = -(height / 2.0f) * scaling;
    const float top = (height / 2.0f) * scaling;

    camera->Perspective = PXCameraPerspective2D;
    camera->Width = width;
    camera->Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera* const camera, const PXSize width, const PXSize height)
{
    camera->Width = width;
    camera->Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

float PXAPI PXCameraAspectRatio(const PXCamera* const camera)
{
    return (float)camera->Width / (float)camera->Height;
}

void PXAPI PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective)
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
            const float aspectRatio = PXCameraAspectRatio(camera);

            PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

            break;
        }
    }
}

void PXAPI PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F)
{
    const float maxValue = 85.0f;
    const float minValue = -85.0f;

    camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
    camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

    camera->CurrentRotation.Y = PXMathLimit(camera->CurrentRotation.Y, minValue, maxValue);

    const float pitchRAD = PXMathDegreeToRadians(camera->CurrentRotation.Y);
    const float yawRAD = PXMathDegreeToRadians(camera->CurrentRotation.X);
    const float rx = PXMathCosinus(pitchRAD) * PXMathCosinus(yawRAD);
    const float ry = PXMathSinus(pitchRAD);
    const float rz = PXMathCosinus(pitchRAD) * PXMathSinus(yawRAD);

    PXVector3FSetXYZ(&camera->LookAtPosition, rx, ry, rz);
    PXVector3FNormalize(&camera->LookAtPosition);
}

void PXAPI PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F)
{
    PXVector3F xAxis = { 0,0,0 };
    const PXVector3F yAxis = { 0, vector3F->Y, 0 };
    PXVector3F zAxis = { 0,0,0 };

    // ...
    {
        const PXVector3F up = { 0, 1, 0 };
        const PXVector3F lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

        PXVector3FCrossProduct(&lookAtPosition, &up, &xAxis);
        PXVector3FNormalize(&xAxis);
        PXVector3FMultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X);

        zAxis = lookAtPosition;

        PXVector3FNormalize(&zAxis);
        PXVector3FMultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z);
    }

    {
        PXVector3F targetedMovement = { 0,0,0 };

        PXVector3FAdd(&targetedMovement, &xAxis);
        PXVector3FAdd(&targetedMovement, &yAxis);
        PXVector3FAdd(&targetedMovement, &zAxis);
        PXVector3FMultiplyS(&targetedMovement, camera->WalkSpeed);

        PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement);
    }
}

void PXAPI PXCameraFollow(PXCamera* const camera, const float deltaTime)
{
    PXVector3F positionCurrent;
    PXVector3F positionDesired;

    PXVector3F rotationCurrent;
    PXVector3F rotationDesired;

    PXVector3F positionDelta;
    PXVector3F rotationDelta;

    if (!camera->Target)
    {
        return;
    }

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &positionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &positionDesired); // get current target pos



    PXMatrix4x4FRotationGet(&camera->MatrixView, &rotationCurrent); // Get current camera pos
    PXMatrix4x4FRotationGet(camera->Target, &rotationDesired); // get current target pos

    camera->FollowSpeed = 12.3;

    //PXVector3FAdd(&positionCurrent, &camera->Offset); // add offset to target pos
    PXVector3FAdd(&positionDesired, &camera->Offset); // add offset to target pos


    PXVector3F eye = {0,0,0};
    PXVector3F center = {0,0,0};
    PXVector3F up = {0,1,0};

   // PXMatrix4x4FLookAt(&camera->MatrixModel, &eye, &desiredPosition, &up);

    PXVector3FInterpolate(&positionDesired, &positionCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement
    PXVector3FInterpolate(&rotationDesired, &rotationCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement

#if 0 
    // Not how i want it. 
    // Problem: it snaps to strong if you get in range, then no movement until were too far away again.
    // We need a deadzone- then a softstart, then a rampup the further away we are

    PXVector3FSet(&positionDelta, &positionCurrent);
    PXVector3FSubstract(&positionDelta, &positionDesired);
    PXVector3FAbsolute(&positionDelta);

    PXVector3FSet(&rotationDelta, &rotationCurrent);
    PXVector3FSubstract(&rotationDelta, &rotationDesired);
    PXVector3FAbsolute(&positionDelta);

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


    PXVector3F rotation;

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

void PXAPI PXCameraUpdate(PXCamera* const camera, const float deltaTime)
{
    const float walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
    const float viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
    const PXVector3F up = { 0,1,0 };
    PXVector3F currentPosition;
    PXVector3F centerPosition;

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    centerPosition = currentPosition;
    PXVector3FAdd(&centerPosition, &camera->LookAtPosition);

    PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up);
}