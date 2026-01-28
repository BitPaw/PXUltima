#pragma once

#ifndef PXFieldEffectIncluded
#define PXFieldEffectIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXFieldEffectForm_
{
    PXFieldEffectypeInvalid,
    PXFieldEffectTypeBox,
    PXFieldEffectTypeCube,
    PXFieldEffectTypeCircle
}
PXFieldEffectForm;

// Collidable entity that can be defined for different behaviours
typedef struct PXFieldEffect_ PXFieldEffect;

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

typedef void(PXAPI* PXFieldEffectCollisionDetect)(void* owner, PXFieldEffect PXREF pxFieldEffect);

typedef struct PXFieldEffect_
{
    PXECSInfo Info;

   // PXModel* Model;

    PXFieldEffectForm Form;

    PXFieldEffect* ColliderChild;
    PXFieldEffect* ColliderParent;

    void* CallBackOwner;
    PXFieldEffectCollisionDetect CollisionDetectCallBack;
}
PXFieldEffect;

typedef struct PXFieldEffectCreateInfo_
{
    PXECSCreateInfo Info;

    PXFieldEffect* HitBox;

    // Mode
    PXI32U Behaviour;

   // PXModel* Model;
}
PXFieldEffectCreateInfo;

PXPublic PXResult PXAPI PXFieldEffectRegisterToECS(void);
PXPublic PXResult PXAPI PXFieldEffectCreate(PXFieldEffect** pxFieldEffectREF, PXFieldEffectCreateInfo PXREF pxFieldEffectCreateInfo);
PXPublic PXResult PXAPI PXFieldEffectRelease(PXFieldEffect PXREF pxFieldEffect);
PXPublic PXResult PXAPI PXFieldEffectDraw(PXFieldEffect PXREF pxFieldEffect, PXDrawInfo PXREF pxDrawInfo);

#endif