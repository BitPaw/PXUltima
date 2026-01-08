#pragma once

#ifndef PXHitBoxIncluded
#define PXHitBoxIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXHitBoxForm_
{
    PXHitBoxTypeInvalid,
    PXHitBoxTypeBox,
    PXHitBoxTypeCube,
    PXHitBoxTypeCircle
}
PXHitBoxForm;

// Collidable entity that can be defined for different behaviours
typedef struct PXHitBox_ PXHitBox;

//---------------------------------------------------------
// BehaviourFlag
//---------------------------------------------------------
#define PXHitBoxBehaviourKeepOut 0b00000001 // Prevent from entering
#define PXHitBoxBehaviourKeepIn  0b00000010 // Prevent from leaving
#define PXHitBoxBehaviourDetect  0b00000100 // Trigger if in inside
#define PXHitBoxBehaviourPhysics 0b00001000 // Apply physics
#define PXHitBoxBehaviourGravity 0b00010000 // Apply Gravity

#define PXHitBoxBehaviourWallStatic PXHitBoxBehaviourKeepOut | PXHitBoxBehaviourKeepIn
//---------------------------------------------------------

typedef void(PXAPI* PXHitBoxCollisionDetect)(void* owner, PXHitBox PXREF pxHitBox);

typedef struct PXHitBox_
{
    PXECSInfo Info;

   // PXModel* Model;

    PXHitBoxForm Form;

    PXHitBox* ColliderChild;
    PXHitBox* ColliderParent;

    void* CallBackOwner;
    PXHitBoxCollisionDetect CollisionDetectCallBack;
}
PXHitBox;

typedef struct PXHitboxCreateInfo_
{
    PXECSCreateInfo Info;

    PXHitBox* HitBox;

    // Mode
    PXI32U Behaviour;

   // PXModel* Model;
}
PXHitboxCreateInfo;

PXPublic PXResult PXAPI PXHitBoxCreate(PXHitBox** pxHitBoxREF, PXHitboxCreateInfo PXREF pxHitboxCreateInfo);

#endif