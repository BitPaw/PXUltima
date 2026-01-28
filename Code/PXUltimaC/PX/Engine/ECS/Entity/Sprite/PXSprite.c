#include "PXSprite.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/ECS/Entity/FieldEffect/PXFieldEffect.h>

const char PXSpriteName[] = "Sprite";
const PXI8U PXSpriteNameLength = sizeof(PXSpriteName);
const PXECSRegisterInfoStatic PXSpriteRegisterInfoStatic =
{
    {sizeof(PXSpriteName), sizeof(PXSpriteName), PXSpriteName, TextFormatASCII},
    sizeof(PXSprite),
    __alignof(PXSprite),
    PXECSTypeEntity,
    PXModelCreate,
    PXModelRelease,
    PXModelDraw
};
PXECSRegisterInfoDynamic PXSpriteRegisterInfoDynamic;


PXResult PXAPI PXSpriteRegisterToECS()
{
    PXECSRegister(&PXSpriteRegisterInfoStatic, &PXSpriteRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXSpriteCreate(PXSprite** pxSpriteREF, PXSpriteCreateInfo PXREF pxSpriteCreateInfo)
{
    PXSprite* pxSprite = PXNull;

    if(!(pxSpriteREF && pxSpriteCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxSpriteCreateInfo->Info.Static = &PXSpriteRegisterInfoStatic;
    pxSpriteCreateInfo->Info.Dynamic = &PXSpriteRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxSpriteREF, pxSpriteCreateInfo);
     
    if(pxResult != PXResultOK)
    {
        return pxResult;
    }

    pxSprite = *pxSpriteREF;
    pxSprite->Info.Behaviour |= PXECSInfoRender;

    // Create hitbox if requested
    if(pxSpriteCreateInfo->HitboxBehaviour > 0)
    {
        PXFieldEffectCreateInfo pxFieldEffectCreateInfo;
        PXClear(PXECSCreateInfo, &pxFieldEffectCreateInfo);

        // Skybox CubeTexture
        pxFieldEffectCreateInfo.Info.Type = PXResourceTypeHitBox;
        pxFieldEffectCreateInfo.Info.Flags = PXResourceCreateBehaviourSpawnInScene | PXResourceCreateBehaviourLoadASYNC;
        pxFieldEffectCreateInfo.Info.ObjectReference = (PXECSInfo**)&pxSprite->HitBox;
        // pxHitboxCreateInfo.Model = pxSprite->Model;
        pxFieldEffectCreateInfo.Behaviour = 0;

        PXFieldEffectCreate(&pxSprite->HitBox, &pxFieldEffectCreateInfo);
    }

    // Scaling?

    // pxSprite->Model = pxResourceManager->ModelFailback;

#if 0
            // Clear sprite //     PXGraphicSpriteConstruct(&pxEngine->Graphic, pxSprite);
    {
        PXModelConstruct(&pxSprite->Model);

        //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
        //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

        PXVector2F32SetXY(&pxSprite->TextureScalePositionOffset, 1, 1);
        PXVector2F32SetXY(&pxSprite->TextureScalePointOffset, 1, 1);

        //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
    }

    // PXTextCopyA(pxEngineResourceCreateInfo->FilePath, 20, pxSprite->Name, 50);

    pxSprite->TextureScalePositionOffset.X = pxSpriteCreateEventData->TextureScalingPoints[0].X;
    pxSprite->TextureScalePositionOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[0].Y;
    pxSprite->TextureScalePointOffset.X = pxSpriteCreateEventData->TextureScalingPoints[1].X;
    pxSprite->TextureScalePointOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[1].Y;


    pxSprite->Model.ShaderProgramReference = pxSpriteCreateEventData->ShaderProgramCurrent;
    pxSprite->Model.IgnoreViewRotation = pxSpriteCreateEventData->ViewRotationIgnore;
    pxSprite->Model.IgnoreViewPosition = pxSpriteCreateEventData->ViewPositionIgnore;
    //pxSprite->Model.
    pxSprite->Model.RenderBothSides = PXTrue;



    pxSprite->Model.MaterialContaierList = PXNew(PXMaterialContainer);
    pxSprite->Model.MaterialContaierListSize = 1u;

    pxSprite->Model.MaterialContaierList->MaterialList = PXNew(PXMaterial);
    pxSprite->Model.MaterialContaierList->MaterialListSize = 1u;

    PXMaterial* materiial = &pxSprite->Model.MaterialContaierList->MaterialList[0];

    PXClear(PXMaterial, materiial);

    materiial->DiffuseTexture = pxSprite->Texture;
#endif


#if 0



    const PXBool hasScaling = pxSprite->TextureScalePositionOffset.X != 0 || pxSprite->TextureScalePositionOffset.Y != 0;

    if(hasScaling)
    {
        if(pxEngine->SpriteScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->Model.StructureOverride = &pxEngine->SpriteScaled;
        }
    }
    else
    {
        if(pxEngine->SpriteUnScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->Model.StructureOverride = &pxEngine->SpriteUnScaled;
        }
    }

    if(pxSpriteCreateEventData->Scaling.X == 0)
    {
        pxSpriteCreateEventData->Scaling.X = 1;
    }

    if(pxSpriteCreateEventData->Scaling.Y == 0)
    {
        pxSpriteCreateEventData->Scaling.Y = 1;
    }



    PXF32 aspectRationX = 1;

    if(pxSprite->Texture)
    {
        if(pxSprite->Texture->Image->Width && pxSprite->Texture->Image->Height)
        {
            aspectRationX = (PXF32)pxSprite->Texture->Image->Width / (PXF32)pxSprite->Texture->Image->Height;
        }
    }



    PXMatrix4x4FScaleSetXY(&pxSprite->Model.ModelMatrix, aspectRationX, 1);

    PXMatrix4x4FScaleByXY
    (
        &pxSprite->Model.ModelMatrix,
        pxSpriteCreateEventData->Scaling.X,
        pxSpriteCreateEventData->Scaling.Y
    );


    PXMatrix4x4FPositionSet(&pxSprite->Model.ModelMatrix, &pxSpriteCreateEventData->Position);

#endif

    return PXResultOK;
}

PXResult PXAPI PXSpriteRelease(PXSprite PXREF pxSprite)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXSpriteDraw(PXSprite PXREF pxSprite, PXDrawInfo PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}