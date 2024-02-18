#include "PXPhysic.h"

void PXAPI PXPhysicGravityApply(PXPhysicProperties* const physicProperties, const PXVector3F* const gravityDircetion, const float gravityForce, const float deltaTime)
{
	// TODO: Rewrite code

	// Force = Mass * Acceleration

	//assert(Mass > 0);
	//assert(gravityForce != 0);

	//---<Gravity>----------------------------
	{
		PXVector3F gravityMovement = { 0,0,0 };

		PXVector3FAdd(&gravityMovement, gravityDircetion);
		PXVector3FMultiplyS(&gravityMovement, gravityForce); // gravity diection + gravity power
		PXVector3FAdd(&gravityMovement, &physicProperties->Force); // add that to applyed force
	}

	//---<Acceleration>-----------------------
	{
		PXVector3F acceleration = { 0,0,0 };

		PXVector3FAdd(&acceleration, &physicProperties->Force);
		PXVector3FDivideS(&acceleration, physicProperties->Mass); // Acceleration = Force / Mass
		PXVector3FAdd(&acceleration, &acceleration);

		PXVector3FSetXYZ(&physicProperties->Force, 0, 0, 0);
	}


	// Friction = -1 * FrictionFector * N *** Velocity-Normalized
	float friction = 1;
	float normalForce = physicProperties->Mass;
	float frictionFactor = -1 * friction * normalForce;
	PXVector3F frictionDirection;

	PXVector3FAdd(&frictionDirection, &physicProperties->Velocity);
	PXVector3FNormalize(&frictionDirection);
	PXVector3FMultiplyS(&frictionDirection, frictionFactor * normalForce);

	//Velocity -= FrictionDirection;

	// Fake friction
	PXVector3FMultiplyS(&physicProperties->Velocity, 0.75f);

	PXMatrix4x4FMove3F(&physicProperties->MatrixModel, &physicProperties->Velocity);
}