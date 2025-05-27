#ifndef PXPhysicINCLUDE
#define PXPhysicINCLUDE

#include <PX/Media/PXType.h>
#include <PX/Math/PXVector.h>
#include <PX/Math/PXMatrix.h>

#define PXGravityForceSun       274
#define PXGravityForceMercury   3.7
#define PXGravityForceVenus     8.87
#define PXGravityForceEarth     9.807
#define PXGravityForceEarthMoon 1.62
#define PXGravityForceMars      3.721
#define PXGravityForceJupiter   24.79
#define PXGravityForceSaturn    10.44
#define PXGravityForceNeptune   11.15
#define PXGravityForceUranus    8.87
#define PXGravityForcePluto     0.62

typedef struct PXPhysicProperties_
{
    PXMatrix4x4F MatrixModel;
    PXVector3F32 Velocity;
    PXVector3F32 Force;
    PXF32 Mass;
    PXF32 Friction;
    // Frriction static / dynamic
    // elastistity / boncyness

    PXBool EnablePhysics;
    PXBool EnableGravity;
    PXBool IsSolid;
}
PXPhysicProperties;

PXPublic void PXAPI PXPhysicGravityApply(PXPhysicProperties* const physicProperties, const PXVector3F32* const gravityDircetion, const PXF32 gravityForce, const PXF32 deltaTime);

#endif
