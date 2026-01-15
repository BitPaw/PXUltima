#pragma once

#ifndef PXSpriteIncluded
#define PXSpriteIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Engine/ECS/Component/Material/PXMaterial.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Engine/ECS/Entity/HitBox/PXHitBox.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXSprite_
{
    PXECSInfo Info; // PX-Engine register info

    PXVector2F32 TextureScalePositionOffset;
    PXVector2F32 TextureScalePointOffset;

    PXModel* Model;
    PXTexture* Texture;
    PXMaterial* Material; // Use this instand of a texture, right?
    PXHitBox* HitBox;
    PXShaderProgram* ShaderProgarm;
}
PXSprite;

typedef struct PXSpriteCreateInfo_
{
    PXECSCreateInfo Info;

    PXTexture* TextureCurrent;
    PXShaderProgram* ShaderProgramCurrent;

    PXVector2F32 TextureScalingPoints[4];

    PXVector3F32 Position;
    PXVector2F32 Scaling;

    PXBool ViewRotationIgnore;
    PXBool ViewPositionIgnore;
    PXBool ContainsMultible; // Sprite is not a single texture but has multible
    PXI16U CellSize;



    // If any vale is set, we will generate a hitbox
    PXI32U HitboxBehaviour;
}
PXSpriteCreateInfo;


PXPublic PXResult PXAPI PXSpriteCreate(PXSprite PXREF pxSprite, PXSpriteCreateInfo PXREF pxSpriteCreateInfo);

#endif
