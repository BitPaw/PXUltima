#include "PXPhysic.h"

void PXAPI PXPhysicGravityApply(PXPhysicProperties* const physicProperties, const PXVector3F32* const gravityDircetion, const PXF32 gravityForce, const PXF32 deltaTime)
{
    // TODO: Rewrite code

    // Force = Mass * Acceleration

    //assert(Mass > 0);
    //assert(gravityForce != 0);

    //---<Gravity>----------------------------
    {
        PXVector3F32 gravityMovement = { 0,0,0 };

        PXVector3F32Add(&gravityMovement, gravityDircetion);
        PXVector3F32MultiplyS(&gravityMovement, gravityForce); // gravity diection + gravity power
        PXVector3F32Add(&gravityMovement, &physicProperties->Force); // add that to applyed force
    }

    //---<Acceleration>-----------------------
    {
        PXVector3F32 acceleration = { 0,0,0 };

        PXVector3F32Add(&acceleration, &physicProperties->Force);
        PXVector3F32DivideS(&acceleration, physicProperties->Mass); // Acceleration = Force / Mass
        PXVector3F32Add(&acceleration, &acceleration);

        PXVector3F32SetXYZ(&physicProperties->Force, 0, 0, 0);
    }


    // Friction = -1 * FrictionFector * N *** Velocity-Normalized
    PXF32 friction = 1;
    PXF32 normalForce = physicProperties->Mass;
    PXF32 frictionFactor = -1 * friction * normalForce;
    PXVector3F32 frictionDirection;

    PXVector3F32Add(&frictionDirection, &physicProperties->Velocity);
    PXVector3F32Normalize(&frictionDirection);
    PXVector3F32MultiplyS(&frictionDirection, frictionFactor * normalForce);

    //Velocity -= FrictionDirection;

    // Fake friction
    PXVector3F32MultiplyS(&physicProperties->Velocity, 0.75f);

    PXMatrix4x4FMove3F(&physicProperties->MatrixModel, &physicProperties->Velocity);
}
