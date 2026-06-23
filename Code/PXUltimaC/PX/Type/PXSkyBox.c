#include "PXSkyBox.h"
#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Reflection/PXReflection.h>


const char PXSkyBoxName[] = "SkyBox";
const PXI8U PXSkyBoxNameLength = sizeof(PXSkyBoxName);
const PXECSRegisterInfoStatic PXSkyBoxRegisterInfoStatic =
{
    {sizeof(PXSkyBoxNameLength), sizeof(PXSkyBoxNameLength), PXSkyBoxName, TextFormatASCII},
    sizeof(PXSkyBox),
    __alignof(PXSkyBox),
    PXECSTypeEntity,
    PXSkyboxCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXSkyBoxRegisterInfoDynamic;

void PXAPI PXSkyBoxRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXSkyBoxRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXSkyBoxRegisterInfoDynamic;
}

PXResult PXAPI PXSkyboxCreate(PXSkyBox** pxSkyBoxREF, PXSkyBoxCreateInfo PXREF pxSkyBoxCreateInfo)
{
    if(!(pxSkyBoxREF && pxSkyBoxCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXSkyBox* pxSkyBox = PXNull;

    pxSkyBoxCreateInfo->Info.Static = &PXSkyBoxRegisterInfoStatic;
    pxSkyBoxCreateInfo->Info.Dynamic = &PXSkyBoxRegisterInfoDynamic;
    PXResult pxResult =  PXECSCreate(pxSkyBoxREF, pxSkyBoxCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxSkyBox = *pxSkyBoxREF;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSkyBoxName,
        "Create",
        "PXID:%i\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s",
        pxSkyBox->Info.ID,
        "Top", pxSkyBoxCreateInfo->TextureCube.Cube.Top.A,
        "Left", pxSkyBoxCreateInfo->TextureCube.Cube.Left.A,
        "Right", pxSkyBoxCreateInfo->TextureCube.Cube.Right.A,
        "Back", pxSkyBoxCreateInfo->TextureCube.Cube.Back.A,
        "Bottom", pxSkyBoxCreateInfo->TextureCube.Cube.Bottom.A,
        "Forward", pxSkyBoxCreateInfo->TextureCube.Cube.Front.A,
        "Vertex Shader", pxSkyBoxCreateInfo->ShaderProgram.ShaderVertexFilePath,
        "Pixel Shader", pxSkyBoxCreateInfo->ShaderProgram.ShaderPixelFilePath
    );
#endif

    PXTextureCreate(&pxSkyBox->TextureCube, &pxSkyBoxCreateInfo->TextureCube);
  
    PXShaderProgramCreate(&pxSkyBox->ShaderProgramReference, &pxSkyBoxCreateInfo->ShaderProgram);

    PXModelCreate(&pxSkyBox->Model, &pxSkyBoxCreateInfo->Model);

    return PXResultOK;
}

PXResult PXAPI PXSkyboxDraw(PXSkyBox PXREF pxSkyBox, PXDrawInfo PXREF pxDrawInfo)
{
    return PXResultOK;
}