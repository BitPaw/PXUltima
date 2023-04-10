#ifndef PXPhysicINCLUDE
#define PXPhysicINCLUDE

#include <Media/Type.h>
#include <Math/PXVector.h>
#include <Math/PXMatrix.h>

#define GravityForceSun 274
#define GravityForceMercury 3.7
#define GravityForceVenus 8.87
#define GravityForceEarth 9.807
#define GravityForceEarthMoon 1.62
#define GravityForceMars 3.721
#define GravityForceJupiter 24.79
#define GravityForceSaturn 10.44
#define GravityForceNeptune 11.15
#define GravityForceUranus 8.87
#define GravityForcePluto 0.62


#ifdef __cplusplus
extern "C"
{ 
#endif

	typedef struct PXPhysicProperties_
	{
		PXMatrix4x4F MatrixModel;
		PXVector3F Velocity;
		PXVector3F Force;
		float Mass;
		float Friction;
		// Frriction static / dynamic
		// elastistity / boncyness

		PXBool EnablePhysics;
		PXBool EnableGravity;
		PXBool IsSolid;
	}
	PXPhysicProperties;

    PXPublic void PXPhysicGravityApply(PXPhysicProperties* const physicProperties, const PXVector3F* const gravityDircetion, const float gravityForce, const float deltaTime);

    
#ifdef __cplusplus
}
#endif

#endif