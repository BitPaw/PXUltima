#include "PXCamera.h"
#include <PX/Math/PXVector.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXResource.h>


const char PXCameraName[] = "Camera";
const PXI8U PXCameraNameLength = sizeof(PXCameraName);
const PXECSRegisterInfoStatic PXCameraRegisterInfoStatic =
{
    {sizeof(PXCameraName), sizeof(PXCameraName), PXCameraName, TextFormatASCII},
    sizeof(PXCamera),
    __alignof(PXCamera),
    PXECSTypeEntity,
    PXCameraCreate,
    PXCameraRelease,
    PXCameraDraw
};
PXECSRegisterInfoDynamic PXCameraRegisterInfoDynamic;

PXResult PXAPI PXCameraRegisterToECS(void)
{
    PXECSRegister(&PXCameraRegisterInfoStatic, &PXCameraRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXCameraCreate(PXCamera** pxCameraREF, PXCameraCreateInfo PXREF pxCameraCreateInfo)
{
    PXCamera* pxCamera = PXNull;

    pxCameraCreateInfo->Info.Static = &PXCameraRegisterInfoStatic;
    pxCameraCreateInfo->Info.Dynamic = &PXCameraRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxCameraREF, pxCameraCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxCamera = *pxCameraREF;
    pxCamera->WalkSpeed = 2;
    pxCamera->ViewSpeed = 1;
    pxCamera->FollowSpeed = 0.98f;
    pxCamera->FieldOfView = 90;
    pxCamera->Height = 1;
    pxCamera->Width = 1;
    pxCamera->Near = 0.1;
    pxCamera->Far = 100000;

    //PXMatrix4x4FIdentity(&camera->MatrixModel);
    //PXMatrix4x4FIdentity(&camera->MatrixView);
    //PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(pxCamera, pxCameraCreateInfo->Perspective);

    const PXVector3F32 position = { 0,0,0 };
    PXCameraRotate(pxCamera, &position);

    return PXResultOK;
}

PXResult PXAPI PXCameraRelease(const PXCamera PXREF pxCamera)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXCameraDraw(const PXCamera PXREF pxCamera, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    return PXActionRefusedNotImplemented;
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

   // PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera PXREF camera, const PXF32 fieldOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane)
{
    if(!camera)
    {
        return;
    }

    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    //PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height)
{
    if(!camera)
    {
        return;
    }

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

    switch(cameraPerspective)
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
    if(!camera)
    {
        return;
    }

    const PXVector3F32 vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z)
{
    if(!camera)
    {
        return;
    }

    const PXVector3F32 vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F)
{
    if(!camera)
    {
        return;
    }

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

        PXVector3F32Add(&camera->Position, &targetedMovement);

       // PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement);
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

    if(!camera->Target)
    {
        return;
    }

   // PXMatrix4x4FPositionGet(&camera->MatrixModel, &positionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &positionDesired); // get current target pos



   // PXMatrix4x4FRotationGet(&camera->MatrixView, &rotationCurrent); // Get current camera pos
    PXMatrix4x4FRotationGet(camera->Target, &rotationDesired); // get current target pos

    camera->FollowSpeed = 12.3;

    //PXVector3F32Add(&positionCurrent, &camera->Offset); // add offset to target pos
    PXVector3F32Add(&positionDesired, &camera->Offset); // add offset to target pos


    PXVector3F32 eye = { 0,0,0 };
    PXVector3F32 center = { 0,0,0 };
    PXVector3F32 up = { 0,1,0 };

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



   // PXMatrix4x4FPositionSet(&camera->MatrixModel, &positionDesired); // Set delte movement
   // PXMatrix4x4FRotationSet(&camera->MatrixView, &rotationDesired);


    PXVector3F32 rotation;

    //PXMatrix4x4FRotationGet(&camera->MatrixView, &rotation);

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

   // PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    centerPosition = currentPosition;
   // PXVector3F32Add(&centerPosition, &camera->LookAtPosition);

   // PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up);
}