#pragma once

#ifndef PXCameraIncluded
#define PXCameraIncluded

#include <PX/Math/PXVector.h>
#include <PX/Engine/ECS/PXECS.h>

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

typedef struct PXCameraCreateInfo_ 
{
    PXECSCreateInfo Info;

    PXCameraPerspective Perspective;
}
PXCameraCreateInfo;


PXPublic PXResult PXAPI PXCameraRegisterToECS();

PXPublic PXResult PXAPI PXCameraCreate(PXCamera** pxCameraREF, PXCameraCreateInfo PXREF pxCameraCreateInfo);


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


#endif