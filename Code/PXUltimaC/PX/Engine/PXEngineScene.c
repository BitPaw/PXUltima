#include "PXEngineScene.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXEngineRender.h>

#include <PX/Engine/ECS/PXNamePool.h>

const char PXEngineSceneText[] = "SceneSystem";
const PXI8U PXEngineSceneTextLength = sizeof(PXEngineSceneText);
const PXECSRegisterInfoStatic PXEngineSceneRegisterInfoStatic =
{
    {sizeof(PXEngineSceneText), sizeof(PXEngineSceneText), PXEngineSceneText, TextFormatASCII},
    sizeof(PXEngineScene),
    __alignof(PXEngineScene),
    PXECSTypeSystem,
    PXEngineSceneCreate,
    PXEngineSceneRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineSceneRegisterInfoDynamic;

void PXAPI PXEngineSceneRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXEngineSceneRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXEngineSceneRegisterInfoDynamic;
}

PXResult PXAPI PXEngineSceneCreate(PXEngineScene** pxEngineSceneREF, PXEngineSceneCreateInfo PXREF pxEngineSceneCreateInfo)
{
    PXEngineScene* pxEngineScene = PXNull;

    if(!(pxEngineSceneREF && pxEngineSceneCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineSceneCreateInfo->Info.Static = &PXEngineSceneRegisterInfoStatic;
    pxEngineSceneCreateInfo->Info.Dynamic = &PXEngineSceneRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineSceneREF, pxEngineSceneCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineScene = *pxEngineSceneREF;
    pxEngineScene->ModuleInfo.OnActivate = PXEngineSceneOnActivate;
    pxEngineScene->ModuleInfo.OnDeactivate = PXEngineSceneOnDeactivate;
    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineScene->ModuleInfo, PXEngineSceneUpdate);

    PXDictionaryECSCreate(&pxEngineScene->SceneLookup);

    return pxResult;
}

PXResult PXAPI PXEngineSceneRelease(PXEngineScene PXREF pxEngineScene)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineSceneUpdate(PXEngineScene PXREF pxEngineScene, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineSceneOnActivate(PXEngineScene PXREF pxEngineScene, PXEngine PXREF pxEngine)
{
    PXResult pxResult = PXEngineSceneSceneLoad(pxEngineScene, pxEngineScene->SceneCurrent);

    return pxResult;
}

PXResult PXAPI PXEngineSceneOnDeactivate(PXEngineScene PXREF pxEngineScene, PXEngine PXREF pxEngine)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineSceneSceneAdd(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineSceneSceneRemove(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineSceneSceneLoad(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    if(!(pxEngineScene && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    PXScene PXREF oldScene = PXEngineSceneSceneCurrent(pxEngineScene);

    if(oldScene)
    {
        const PXBool isAlreadyLoaded = oldScene->Info.ID == pxScene->Info.ID;

        if(isAlreadyLoaded)
        {
            return PXResultOK;
        }
    }

    PXSize oldID = -1;

    if(oldScene)
    {
        oldID = oldScene->Info.ID;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "PXID:<%4i> -> PXID:<%4i>\n"
        "%20s : %ix\n"
        "%20s : %ix\n"
        "%20s : PXID:<%4i>",
        oldID,
        pxScene->Info.ID,
        "Models", PXSceneModelAmount(pxScene),
        "Cameras", PXSceneCameraAmount(pxScene),
        "SkyBox", pxScene->SkyBox ? pxScene->SkyBox->Info.ID : -1
    );
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "Start to load missing entrys.\n"
        "Following is all objects aviable.\n"
        "Some IDs might be empty, those are not registered yet"
    );
#endif

    PXEngineScenePrint(pxEngineScene, oldScene);

    PXEngineSceneResourceMarkAllToUnload(pxEngineScene, oldScene);
    PXEngineSceneResourceMarkReqiredToLoad(pxEngineScene, pxScene);
    PXEngineSceneResourceFetch(pxEngineScene, pxScene);
    PXEngineSceneResourceEvict(pxEngineScene, pxScene);

    // Current scene is now the new one!
    pxEngineScene->SceneCurrent = pxScene;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "Finished loading scene."
    );
#endif

    PXEngineScenePrint(pxEngineScene, pxScene);

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneSceneUnload(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineScenePrint(const PXEngineScene PXREF pxEngineScene, const PXScene PXREF pxScene)
{
    const PXSize modelAmount = PXSceneModelAmount(pxScene);

    for(PXSize modelIndex = 0; modelIndex < modelAmount; modelIndex++)
    {
        const PXModel PXREF pxModel = PXSceneModelGet(pxScene, modelIndex);

        PXID pxID = ((PXECSInfo*)pxModel)->ID;

        PXText pxTextModel;
        PXNamePoolGet(pxID, &pxTextModel);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            PXEngineSceneText,
            "Load",
            "[%2.2i/%2.2i] %10s : PXID:<%4i> Name:<%s>",
            modelIndex + 1,
            modelAmount,
            "Model",
            pxID,
            pxTextModel.A
        );
#endif
        const PXSize amount = PXModelMeshAmount(pxModel);

        for(PXSize meshIndex = 0; meshIndex < amount; meshIndex++)
        {
            PXMesh PXREF pxMesh = PXModelMeshGet(pxModel, meshIndex);

            if(!pxMesh)
            {
                continue;
            }

            PXMaterial* pxMaterial = PXModelMaterialGetForMeshIndex(pxModel, meshIndex, 0);

            PXShaderProgram* pxShaderProgram = PXNull;

            if(pxMaterial)
            {
                pxShaderProgram = pxMaterial->Data.ShaderProgram;
            }

#if PXLogEnable && 0
            PXID pxIDMesh = pxMesh->Info.ID;
            PXText pxTextMesh;
            PXID pxIDMaterial = -1;
            PXText pxTextModel;

            PXID pxGLIDTextureCube = -1;
            PXID pxIDTextureCube = -1;
            PXText pxTextTextureCube;
            PXID pxGLIDTexture2DBase = -1;
            PXID pxIDTexture2DBase = -1;
            PXText pxTextTexture2DBase;

            PXText pxTextVBO;
            PXText pxTextIBO;
            PXText pxTextShader;

  
   
            PXI32S vertexBufferID = -1;
            PXI32S indeBufferID = -1;
            PXI32S shaderID = -1;

            PXI32S vbo = -1;
            PXI32S ibo = -1;
            PXI32S shaderGLID = -1;

            if(pxMesh->VertexBuffer)
            {
                vbo = pxMesh->VertexBuffer->VBO;
                vertexBufferID = pxMesh->VertexBuffer->Info.ID;
            }

            if(pxMesh->IndexBuffer)
            {
                ibo = pxMesh->IndexBuffer->IBO;
                indeBufferID = pxMesh->IndexBuffer->Info.ID;
            }

            if(pxShaderProgram)
            {
                shaderGLID = pxShaderProgram->OpenGLID;
                shaderID = pxShaderProgram->Info.ID;   
            }

            if(pxMaterial)
            {
                pxIDMaterial = pxMaterial->Info.ID;

                if(pxMaterial->Data.TextureCube)
                {
                    pxGLIDTextureCube = pxMaterial->Data.TextureCube->OpenGLID;
                    pxIDTextureCube = pxMaterial->Data.TextureCube->Info.ID;
                }

                if(pxMaterial->Data.TextureBase)
                {
                    pxGLIDTexture2DBase = pxMaterial->Data.TextureBase->OpenGLID;
                    pxIDTexture2DBase = pxMaterial->Data.TextureBase->Info.ID;
                }
            }
       

            PXNamePoolGet(pxIDMaterial, &pxTextModel);
            PXNamePoolGet(pxIDMesh, &pxTextMesh);
            PXNamePoolGet(vertexBufferID, &pxTextVBO);
            PXNamePoolGet(indeBufferID, &pxTextIBO);
            PXNamePoolGet(shaderID, &pxTextShader);
            PXNamePoolGet(pxIDTextureCube, &pxTextTextureCube);
            PXNamePoolGet(pxIDTexture2DBase, &pxTextTexture2DBase);


            PXLogPrint
            (
                PXLoggingEvent,
                PXEngineSceneText,
                "Load",
                "[%2.2i/%2.2i] Mesh\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>\n"
                "%20s : PXID:<%4i> GLID:<%2i> Name:<%s>",
                meshIndex + 1,
                amount,
                "VAO", pxIDMesh, pxMesh->VAO, pxTextModel.A,
                "VBO", vertexBufferID, vbo, pxTextVBO.A,
                "IBO", indeBufferID, ibo, pxTextIBO.A,
                "Shader", shaderID, shaderGLID, pxTextShader.A,
                "TextureCube", pxIDTextureCube, pxGLIDTextureCube, pxTextTextureCube.A,
                "TextureBase", pxIDTexture2DBase, pxGLIDTexture2DBase,  pxTextTexture2DBase.A
            );
#endif

        }
    }

    return PXResultOK;
}

PXScene* PXAPI PXEngineSceneSceneCurrent(const PXEngineScene PXREF pxEngineScene)
{
    if(!pxEngineScene)
    {
        return PXNull;
    }

    PXScene PXREF pxScene = pxEngineScene->SceneCurrent;

    return pxScene;
}

PXResult PXAPI PXEngineSceneResourceMarkAllToUnload(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    if(!(pxEngineScene && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceMarkReqiredToLoad(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    if(!(pxEngineScene && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceFetch(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    if(!(pxEngineScene && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    const PXSize modelAmount = PXSceneModelAmount(pxScene);

    for(PXSize i = 0; i < modelAmount; i++)
    {
        PXModel PXREF pxModel = PXSceneModelGet(pxScene, i);

        PXEngineSceneResourceFetchModel(pxEngineScene, pxScene, pxModel);
    }    

    PXEngineSceneResourceFetchSkyBox(pxEngineScene, pxScene, pxScene->SkyBox);

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceEvict(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene)
{
    if(!(pxEngineScene && pxScene))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceFetchModel
(
    PXEngineScene PXREF pxEngineScene,
    PXScene PXREF pxScene, 
    PXModel PXREF pxModel
)
{
    PXNamePoolEntryRow pxNamePoolEntryRow;

    PXText pxTextName;

    PXID pxID = ((PXECSInfo*)pxModel)->ID;

    PXNamePoolGet(pxID, &pxTextName);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "Scene:<%4i> Model: PXID:<%4i> Name:<%s>",
        pxScene->Info.ID,
        pxID,
        pxTextName.A
    );
#endif

    pxEngineScene->RenderSystem->ModelRegister(pxEngineScene->RenderSystem->API, pxModel);

    PXMaterial PXREF pxMaterial = PXModelMaterialGetForMeshIndex(pxModel, 0, 0);

    PXEngineSceneResourceFetchMaterial(pxEngineScene, pxMaterial);

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceFetchSkyBox(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox)
{
    if(!(pxEngineScene && pxSkyBox))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "Scene:<%4i> SkyBox: PXID:<%4i>",
        pxScene->Info.ID,
        ((PXECSInfo*)pxSkyBox)->ID
    );
#endif

    pxEngineScene->RenderSystem->ModelRegister(pxEngineScene->RenderSystem->API, pxSkyBox->Model);

    return PXResultOK;
}

PXResult PXAPI PXEngineSceneResourceFetchMaterial(PXEngineScene PXREF pxEngineScene, PXMaterial PXREF pxMaterial)
{
    if(!(pxEngineScene && pxMaterial))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineSceneText,
        "Load",
        "Material: PXID:<%4i>",
        ((PXECSInfo*)pxMaterial)->ID
    );
#endif

    pxEngineScene->RenderSystem->ShaderProgramCreate(pxEngineScene->RenderSystem->API, pxMaterial->Data.ShaderProgram);

    pxEngineScene->RenderSystem->TextureCubeRegister(pxEngineScene->RenderSystem->API, pxMaterial->Data.TextureCube);
    pxEngineScene->RenderSystem->Texture2DRegister(pxEngineScene->RenderSystem->API, pxMaterial->Data.TextureBase);
    pxEngineScene->RenderSystem->Texture2DRegister(pxEngineScene->RenderSystem->API, pxMaterial->Data.AmbientTexture);
    pxEngineScene->RenderSystem->Texture2DRegister(pxEngineScene->RenderSystem->API, pxMaterial->Data.SpecularTexture);
    pxEngineScene->RenderSystem->Texture2DRegister(pxEngineScene->RenderSystem->API, pxMaterial->Data.EmissionTexture);

    return PXResultOK;
}
