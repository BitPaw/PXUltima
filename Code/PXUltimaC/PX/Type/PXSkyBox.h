#pragma once

#ifndef PXSkyBoxIncluded
#define PXSkyBoxIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXTexture.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXShaderVertex.h>
#include <PX/Type/PXShaderPixelFragment.h>
#include <PX/Type/PXShaderProgram.h>
#include <PX/Type/PXMaterial.h>


typedef struct PXSkyBox_
{
    PXECSInfo Info;

    PXModel* Model;
    PXTexture2D* TextureCube;

    PXShaderVertex* ShaderVertex;
    PXShaderPixelFragment* ShaderPixelFragment;
    PXShaderProgram* ShaderProgramReference;

    PXMaterial* Material;
}
PXSkyBox;

typedef struct PXSkyBoxCreateInfo_
{
    PXECSCreateInfo Info;

    PXTexture2D* TextureCube;
    PXShaderProgram* ShaderProgram;
}
PXSkyBoxCreateInfo;

PXPublic void PXAPI PXSkyBoxRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSkyboxCreate(PXSkyBox** pxSkyBox, PXSkyBoxCreateInfo PXREF pxSkyBoxCreateInfo);
PXPublic PXResult PXAPI PXSkyboxRelease(PXSkyBox PXREF pxSkyBox);

#endif