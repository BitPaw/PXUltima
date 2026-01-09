#pragma once

#ifndef PXSkyBoxIncluded
#define PXSkyBoxIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>

typedef struct PXSkyBox_ PXSkyBox;

typedef struct PXSkyBox_
{
    PXECSInfo Info;

    PXModel* Model;
    PXTexture* TextureCube;
    PXShaderProgram* ShaderProgramReference;
}
PXSkyBox;

typedef struct PXSkyBoxCreateInfo_
{
    PXECSCreateInfo Info;

    PXModelCreateInfo Model;
    PXShaderProgramCreateInfo ShaderProgram;
    PXTextureCreateInfo TextureCube;
}
PXSkyBoxCreateInfo;

PXPublic PXResult PXAPI PXSkyBoxRegisterToECS();

PXPublic PXResult PXAPI PXSkyboxCreate(PXSkyBox** pxSkyBox, PXSkyBoxCreateInfo PXREF pxSkyBoxCreateInfo);



#endif