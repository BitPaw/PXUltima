#pragma once

#ifndef PXCollisionIncluded
#define PXCollisionIncluded

#include <PX/Media/PXType.h>
#include <PX/Math/PXVector.h>

typedef enum PXCollisionBodyType_
{
    PXCollisionBodyTypeInvalid,
    PXCollisionBodyTypePoint, // 0D
    PXCollisionBodyTypeLine, // 1D
    PXCollisionBodyTypePlane, // 2D
    PXCollisionBodyTypeBody // 3D
}
PXCollisionBodyType;

typedef enum PXCollisionEffectType_
{
    PXCollisionEffectTypeInvalid,
    PXCollisionEffectTypeGravity, // Pulling or pushing fields
    PXCollisionEffectTypeSolid, // Used for walls and floor
    PXCollisionEffectTypeTrigger // Event triggers for interactions
}
PXCollisionEffectType;

typedef struct PXCollisionElement_
{
    PXCollisionBodyType BodyType;
    PXCollisionEffectType EffectType;
}
PXCollisionElement;

typedef struct PXCollisionGravityField_
{
    PXF32 PullForce;
    PXVector3F32 PullDirection;
    PXVector3F32 IgnoreAxis;
}
PXCollisionGravityField;



typedef struct PXPhysicEffector_
{
    PXI32U FlagList;
}
PXPhysicEffector;

PXPublic PXBool PXAPI PXCollisionAABB(PXF32 ax, PXF32 ay, PXF32 bx, PXF32 by, PXF32 cx, PXF32 cy, PXF32 dx, PXF32 dy);


// Start from a position, cast a ray into a direction
// if hit, return the range. If nothing hit, return -1 
PXPublic PXF32 PXAPI PXRayTriangleIntersect
(
    PXVector3F32 PXREF rayOriginPositon,
    PXVector3F32* rayOriginDirection,
    const PXVector3F32 PXREF triangleVertexList,
    const PXSize amountTriangles
);

#endif
