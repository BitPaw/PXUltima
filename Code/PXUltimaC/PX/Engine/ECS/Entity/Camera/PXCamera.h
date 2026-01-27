#pragma once

#ifndef PXCameraIncluded
#define PXCameraIncluded


#include <PX/Engine/ECS/PXECS.h>
#include <PX/Math/PXMatrix.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Math/PXRotation.h>

#define PXCameraFollowPosition 0
#define PXCameraFollowRotation 0
#define PXCameraFollowRotationY 0

typedef enum PXCameraPerspective_
{
    PXCameraPerspectiveInvalid,
    PXCameraPerspective2D,
    PXCameraPerspective3D // Perspective
}
PXCameraPerspective;

typedef struct PXCamera_ PXCamera;

typedef struct PXCamera_
{
    PXECSInfo Info;

    PXVector4F32 Position;
    PXRotor Rotation;

    PXVector3F32 LookAtPosition;
    PXVector3F32 CurrentRotation;

    //---<Follow>---
    PXVector3F32 Offset;
    PXVector3F32 DeadZone;
    //PXI8U TargetFollowFlag;
    PXMatrix4x4F* Target;
    PXF32 FollowSpeed; // Ranges from 0 to 1 .. FollowSpeed; = 0.98f

    PXCameraPerspective Perspective;

    PXF32 FieldOfView;
    PXI32S Height;
    PXI32S Width;
    PXF32 Near;
    PXF32 Far;

    PXF32 WalkSpeed;
    PXF32 ViewSpeed;

    PXBool LockMovement;
    PXBool LockView;
}
PXCamera;

typedef struct PXCameraCreateInfo_
{
    PXECSCreateInfo Info;

    PXCameraPerspective Perspective;
}
PXCameraCreateInfo;


PXPublic PXResult PXAPI PXCameraRegisterToECS(void);

PXPublic PXResult PXAPI PXCameraCreate(PXCamera** pxCameraREF, PXCameraCreateInfo PXREF pxCameraCreateInfo);
PXPublic PXResult PXAPI PXCameraRelease(PXCamera PXREF pxCamera);
PXPublic PXResult PXAPI PXCameraDraw(PXCamera PXREF pxCamera, PXDrawInfo PXREF pxDrawInfo);

//-----------
PXPublic PXF32 PXAPI PXCameraAspectRatio(const PXCamera PXREF camera);
PXPublic void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height);

PXPublic void PXAPI PXCameraViewChange(PXCamera PXREF camera, const PXCameraPerspective cameraPerspective);
PXPublic void PXAPI PXCameraViewChangeToOrthographic(PXCamera PXREF camera, const PXSize width, const PXSize height, const PXF32 nearPlane, const PXF32 farPlane);
PXPublic void PXAPI PXCameraViewChangeToPerspective(PXCamera PXREF camera, const PXF32 fieldOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane);
//-----------

//---<Transform>-----------------------------------------------------------
PXPublic void PXAPI PXCameraRotate(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F);
PXPublic void PXAPI PXCameraRotateXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXCameraMove(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F);
PXPublic void PXAPI PXCameraMoveXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXCameraFollow(PXCamera PXREF camera, const PXF32 deltaTime);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic void PXAPI PXCameraUpdate(PXCamera PXREF camera, const PXF32 deltaTime);
//-------------------------------------------------------------------------


PXPublic void PXAPI PXCameraForward(const PXCamera PXREF camera, PXVector3F32 PXREF forward);
PXPublic void PXAPI PXCameraRight(const PXCamera PXREF camera, PXVector3F32 PXREF right);
PXPublic void PXAPI PXCameraUp(const PXCamera PXREF camera, PXVector3F32 PXREF up);

#endif