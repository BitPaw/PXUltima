#pragma once

#ifndef PXSpriteAnimationIncluded
#define PXSpriteAnimationIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Math/PXVector.h>
#include <PX/Engine/ECS/Entity/Sprite/PXSprite.h>

typedef struct PXSpriteFrame_
{
    int x;
}
PXSpriteFrame;


// KeyFrame, info about each animated frame
typedef struct PXSpriteAnimatorTimeStamp_
{
    PXTexture* Texture;
    PXF32 DeltaTime; // The time until we swap to the next screen
}
PXSpriteAnimatorTimeStamp;


#define PXSpriteAnimatorBehaviourAnimationEnable    (1 << 0)
#define PXSpriteAnimatorBehaviourPlayOnce            (1 << 1)
#define PXSpriteAnimatorBehaviourUseUpdateRate        (1 << 2)

// Object to handle a sprite animation by switching the attached texture
typedef struct PXSpriteAnimator_
{
    PXECSInfo Info;

    PXSprite* SpriteTarget;

    PXI32U LastUpdate;
    PXI32U RateUpdate;

    PXSpriteAnimatorTimeStamp* TimeStampList;
    PXSize TimeStampAmount;
    PXSize TimeStampCurrent;
}
PXSpriteAnimator;


// Info to create this
typedef struct PXSpriteAnimatorInfo_
{
    PXSpriteAnimatorTimeStamp* TimeStampList;
    PXSize TimeStampAmount;
    PXI32U Behaviour;
    PXSprite* SpriteTarget;
    PXF32 UpdateRate;
}
PXSpriteAnimatorInfo;

PXPublic PXResult PXAPI PXSpriteAnimatorCreate(PXSpriteAnimator PXREF pxSpriteAnimator, PXSpriteAnimatorInfo PXREF pxSpriteAnimatorInfo);

#endif