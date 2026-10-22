#include "PXShaderProgram.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>


const char PXShaderProgramText[] = "ShaderProgram";
const PXI8U PXShaderProgramTextLength = sizeof(PXShaderProgramText);
const PXECSRegisterInfoStatic PXShaderProgramRegisterInfoStatic =
{
    {sizeof(PXShaderProgramText), sizeof(PXShaderProgramText), PXShaderProgramText, TextFormatASCII},
    sizeof(PXShaderProgram),
    __alignof(PXShaderProgram),
    PXECSTypeSystem,
    PXShaderProgramCreate,
    PXShaderProgramRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXShaderProgramRegisterInfoDynamic;


void PXAPI PXShaderProgramRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXShaderProgramRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXShaderProgramRegisterInfoDynamic;
}

PXResult PXAPI PXShaderProgramCreate(PXShaderProgram** pxShaderProgramREF, PXShaderProgramCreateInfo PXREF pxShaderProgramCreateInfo)
{
    PXShaderProgram* pxShaderProgram = PXNull;

    if(!(pxShaderProgramREF && pxShaderProgramCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxShaderProgramCreateInfo->Info.Static = &PXShaderProgramRegisterInfoStatic;
    pxShaderProgramCreateInfo->Info.Dynamic = &PXShaderProgramRegisterInfoDynamic;

    PXResult pxResult = PXECSCreate(pxShaderProgramREF, pxShaderProgramCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxShaderProgram = *pxShaderProgramREF;
    pxShaderProgram->Compute = pxShaderProgramCreateInfo->Compute;
    pxShaderProgram->Geometry = pxShaderProgramCreateInfo->Geometry;
    pxShaderProgram->PixelFragment = pxShaderProgramCreateInfo->PixelFragment;
    pxShaderProgram->TessellationControlHull = pxShaderProgramCreateInfo->TessellationControlHull;
    pxShaderProgram->TessellationEvaluationDomain = pxShaderProgramCreateInfo->TessellationEvaluationDomain;
    pxShaderProgram->Vertex = pxShaderProgramCreateInfo->Vertex;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXShaderProgramText,
        "Create",
        "ShaderProgram\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>\n"
        "%30s : PXID:<%4i>",
        "PXID", pxShaderProgram->Info.ID,
        "Compute", pxShaderProgram->Compute ? pxShaderProgram->Compute->ShaderBase.Info.ID : 0,
        "Geometry", pxShaderProgram->Geometry ? pxShaderProgram->Geometry->ShaderBase.Info.ID : 0,
        "PixelFragment", pxShaderProgram->PixelFragment ? pxShaderProgram->PixelFragment->ShaderBase.Info.ID : 0,
        "TessellationControlHull", pxShaderProgram->TessellationControlHull ? pxShaderProgram->TessellationControlHull->ShaderBase.Info.ID : 0,
        "TessellationEvaluationDomain", pxShaderProgram->TessellationEvaluationDomain ? pxShaderProgram->TessellationEvaluationDomain->ShaderBase.Info.ID : 0,
        "Vertex", pxShaderProgram->Vertex ? pxShaderProgram->Vertex->ShaderBase.Info.ID : 0
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXShaderProgramRelease(PXShaderProgram PXREF pxShaderProgram)
{
    return PXResultOK;
}

PXSize PXAPI PXShaderProgramAmountOfShader(const PXShaderProgram PXREF pxShaderProgram)
{
    return 2;
}