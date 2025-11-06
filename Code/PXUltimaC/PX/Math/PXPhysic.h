#pragma once

#ifndef PXPhysicIncluded
#define PXPhysicIncluded

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




typedef struct PXVoxelParticle_
{
    PXF32 Mass;
    PXF32 Velocity;
    PXF32 Pressure;
}
PXVoxelParticle;

// A Space of elemets with spesific properties.
// Can be used to simpulate probalitic behaviour.
typedef struct PXVoxelParticleSystem_
{
    PXF32 Mass;
    PXF32 Velocity;
    PXF32 Pressure;
}
PXVoxelParticleSystem;


// Hooke’s Law for elasticity
PXPublic void PXAPI PXPhysicVoxelParticleDeform();

// Computational Fluid Dynamics (CFD), Navier-Stokes
PXPublic void PXAPI PXPhysicVoxelParticleDeform();



// Provide a voxel/particle system, start with empty particles.
// We add selected tiles, all other tiles might collapse into a solution.
PXPublic void PXAPI PXPhysicWaveFunctionCollapse();






PXPublic void PXAPI PXPhysicGravityApply(PXPhysicProperties PXREF physicProperties, const PXVector3F32 PXREF gravityDircetion, const PXF32 gravityForce, const PXF32 deltaTime);

#endif
