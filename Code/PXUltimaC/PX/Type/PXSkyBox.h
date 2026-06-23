#pragma once

#ifndef PXSkyBoxIncluded
#define PXSkyBoxIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXTexture.h>
#include <PX/Type/PXModel.h>

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

PXPublic void PXAPI PXSkyBoxRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSkyboxCreate(PXSkyBox** pxSkyBox, PXSkyBoxCreateInfo PXREF pxSkyBoxCreateInfo);
PXPublic PXResult PXAPI PXSkyboxDraw(PXSkyBox PXREF pxSkyBox, PXDrawInfo PXREF pxDrawInfo);

#endif