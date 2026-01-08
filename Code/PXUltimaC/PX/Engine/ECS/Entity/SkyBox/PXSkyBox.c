#include "PXSkyBox.h"
#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/ECS/Reflection/PXReflection.h>



const char PXSkyBoxName[] = "SkyBox";
const PXI8U PXSkyBoxNameLength = sizeof(PXSkyBoxName);
const PXECSRegisterInfoStatic PXSkyBoxRegisterInfoStatic =
{
    {sizeof(PXSkyBoxNameLength), sizeof(PXSkyBoxNameLength), PXSkyBoxName, TextFormatASCII},
    sizeof(PXSkyBox),
    __alignof(PXSkyBox),
    PXECSTypeResource
};
PXECSRegisterInfoDynamic PXSkyBoxRegisterInfoDynamic;

PXResult PXAPI PXSkyBoxRegisterToECS()
{
    PXECSRegister(&PXSkyBoxRegisterInfoStatic, &PXSkyBoxRegisterInfoDynamic);

    return PXActionSuccessful;
}

PXResult PXAPI PXSkyboxCreate(PXSkyBox PXREF pxSkyBox, PXSkyBoxCreateInfo PXREF pxSkyBoxCreateInfo)
{
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

    // Load Textures
    {
        PXECSCreateInfo pxECSCreateInfoList[2];
        PXClearList(PXECSCreateInfo, &pxECSCreateInfoList, 2);

        // Skybox CubeTexture
        pxECSCreateInfoList[0].Type = PXResourceTypeTextureCube;
        pxECSCreateInfoList[0].Flags = PXResourceCreateBehaviourSpawnInScene | PXResourceCreateBehaviourLoadASYNC;
        pxECSCreateInfoList[0].ObjectReference = (PXECSInfo**)&pxSkyBox->TextureCube; // Request object
       // pxECSCreateInfoList[0].Texture = pxSkyBoxCreateEventData->TextureCube;

        // Skybox Shader
        pxECSCreateInfoList[1].Type = PXResourceTypeShaderProgram;
        pxECSCreateInfoList[1].ObjectReference = (PXECSInfo**)&pxSkyBox->ShaderProgramReference; // Request object

       // PXCopy(PXShaderProgramCreateInfo, pxSkyBoxCreateEventData, &pxECSCreateInfoList[1].ShaderProgram);

        // Skybox model
        //pxEngineResourceActionInfoList[1].Type = PXEngineResourceActionTypeCreate;
        //pxEngineResourceActionInfoList[1].Create.CreateType = PXEngineCreateTypeModel;
        //pxEngineResourceActionInfoList[1].Create.SpawnEnabled = PXTrue;
        //pxEngineResourceActionInfoList[1].Create.Model.ModelReference = &pxSkyBox->Model; // Request object
        //pxEngineResourceActionInfoList[1].Create.Model.ModelFilePath = pxSkyBoxCreateEventData->;

       // //PXResourceManagerAddV(pxECSCreateInfoList, 2);

        //PXCopy(PXShaderProgramCreateInfo, &pxECSCreateInfoList[1].ShaderProgram, pxSkyBoxCreateEventData);
    }

    return PXActionSuccessful;
}