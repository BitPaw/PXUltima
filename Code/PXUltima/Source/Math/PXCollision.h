#ifndef PXCollisionINCLUDE
#define PXCollisionINCLUDE

#include <Media/Type.h>
#include <Math/PXVector.h>

#ifdef __cplusplus
extern "C"
{
#endif

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


#ifdef __cplusplus
}
#endif

#endif