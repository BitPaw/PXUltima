#include "PXPhysic.h"

void PXPhysicGravityApply(PXPhysicProperties* const physicProperties, const PXVector3F* const gravityDircetion, const float gravityForce, const float deltaTime)
{
	// TODO: Rewrite code

	// Force = Mass * Acceleration

	//assert(Mass > 0);
	//assert(gravityForce != 0);

	//---<Gravity>----------------------------
	{
		PXVector3F gravityMovement;

		PXVector3FMultiplyS(gravityDircetion, gravityForce, &gravityMovement); // gravity diection + gravity power
		PXVector3FAdd(&physicProperties->Force, &gravityMovement, &physicProperties->Force); // add that to applyed force
	}

	//---<Acceleration>-----------------------
	{
		PXVector3F acceleration;

		PXVector3FDivideS(&physicProperties->Force, physicProperties->Mass, &acceleration); // Acceleration = Force / Mass

		PXVector3FAdd(&physicProperties->Velocity, &acceleration, &physicProperties->Velocity);

		PXVector3FSetXYZ(&physicProperties->Force, 0, 0, 0);
	}


	// Friction = -1 * FrictionFector * N *** Velocity-Normalized
	float friction = 1;
	float normalForce = physicProperties->Mass;
	float frictionFactor = -1 * friction * normalForce;
	PXVector3F frictionDirection;

	PXVector3FNormalize(&physicProperties->Velocity, &frictionDirection);
	PXVector3FMultiplyS(&frictionDirection, frictionFactor, &frictionDirection);
	PXVector3FMultiplyS(&frictionDirection, normalForce, &frictionDirection);

	//Velocity -= FrictionDirection;

	// Fake friction
	PXVector3FMultiplyS(&physicProperties->Velocity, 0.75f, &physicProperties->Velocity);

	PXMatrix4x4FMove3F(&physicProperties->MatrixModel, &physicProperties->Velocity, &physicProperties->MatrixModel);
}