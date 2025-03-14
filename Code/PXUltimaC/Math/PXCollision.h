#ifndef PXCollisionINCLUDE
#define PXCollisionINCLUDE

#include <Media/PXType.h>
#include <Math/PXVector.h>

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
    float PullForce;
    PXVector3F PullDirection;
    PXVector3F IgnoreAxis;
}
PXCollisionGravityField;

PXPublic PXBool PXAPI PXCollisionAABB(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);


// Start from a position, cast a ray into a direction
// if hit, return the range. If nothing hit, return -1 
PXPublic float PXAPI PXRayTriangleIntersect
(
    PXVector3F* const rayOriginPositon,
    PXVector3F* const rayOriginDirection,
    PXVector3F* const triangleVertexList
);

#endif
