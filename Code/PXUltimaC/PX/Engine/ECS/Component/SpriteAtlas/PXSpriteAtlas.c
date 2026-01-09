#include "PXSpriteAtlas.h"
#include <PX/Engine/ECS/Resource/Mesh/PXBufferLayout.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include <PX/OS/PXOS.h>

PXResult PXAPI PXSpriteMapCreate(PXSpriteMap PXREF pxSpriteMap, PXSpriteMapInfo PXREF pxSpriteMapInfo)
{
    // Create texture
    {
        PXECSCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXECSCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeTexture2D;
        pxResourceCreateInfoList[0].ObjectReference = (PXECSInfo**)&pxSpriteMap->Texture;

      //  //PXResourceManagerAdd(pxResourceCreateInfoList);
    }

    // Fetch model, a rectangle as a base.
    {  
        PXECSCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXECSCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeModel;
        pxResourceCreateInfoList[0].ObjectReference = (PXECSInfo**)&pxSpriteMap->Model;
       // pxResourceCreateInfoList[0].Model.Form = PXModelFormRectangleTX;

       // //PXResourceManagerAdd(pxResourceCreateInfoList);
    }

    // Add a instanced vertex buffer.
    PXSize amountOfUniqeCells =
        pxSpriteMap->Texture->Width *
        pxSpriteMap->Texture->Height;

    PXSize amountOfLevelCells =
        pxSpriteMapInfo->MapSize.X * pxSpriteMapInfo->MapSize.Y;

    PXSize size = sizeof(PXSpriteMapEntity) * amountOfLevelCells;
    pxSpriteMap->SpriteMapEntityList = PXMemoryHeapCallocT(PXSpriteMapEntity, amountOfLevelCells);


    if(pxSpriteMap->Model)
    {
        PXModel PXREF pxModel = pxSpriteMap->Model;

        PXBufferLayout pxVertexBufferLayout[3];
        pxVertexBufferLayout[0].Format = PXTypeF32;
        pxVertexBufferLayout[0].AmountOfElements = 2;
        pxVertexBufferLayout[0].UpdatePerPrimitive = 1;

        pxVertexBufferLayout[1].Format = PXTypeF32;
        pxVertexBufferLayout[1].AmountOfElements = 2;
        pxVertexBufferLayout[1].UpdatePerPrimitive = 1;

        pxVertexBufferLayout[2].Format = PXTypeF32;
        pxVertexBufferLayout[2].AmountOfElements = 2;
        pxVertexBufferLayout[2].UpdatePerPrimitive = 1;

        /*
        PXMeshVertexArrayAdd
        (
            &pxModel->Mesh,
            pxSpriteMap->SpriteMapEntityList,
            size,
            pxVertexBufferLayout,
            3
        );*/

        // amountOfLevelCells
    }

    return PXResultOK;
}