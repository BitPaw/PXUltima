#include "PXRGeometricMap.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>


const char PXRGeometricMapText[] = "RGeometricMap";

const char PXRGeometricMapSignature[4] = "OEGM";

const PXI32U PXRGeometricMapHeaderList[] =
{
    PXTypeDatax4, // PXTypeSignatureCheck
    PXTypeInt32U,
};
const PXI8U PXRGeometricMapHeaderListSize = sizeof(PXRGeometricMapHeaderList) / sizeof(PXI32U);



typedef struct BufferSourceInfo_
{
    PXSize OffsetInFile;
    PXI32U BufferSize;
    PXI8U EnvironmentVisibility;// Flag list
}
BufferSourceInfo;







void PXBufferSourceInfoRead(PXFile PXREF pxFile, BufferSourceInfo PXREF bufferSourceInfoList, const PXI32U amount, const PXI8U version)
{
    for(PXI32U i = 0; i < amount; ++i)
    {
        BufferSourceInfo* vertexSourceInfo = &bufferSourceInfoList[i];

        if(version >= 13)
        {
            PXFileReadI8U(pxFile, &vertexSourceInfo->EnvironmentVisibility);
        }
        else
        {
            vertexSourceInfo->EnvironmentVisibility = 0xFF;
        }

        PXFileReadI32U(pxFile, &vertexSourceInfo->BufferSize);

        vertexSourceInfo->OffsetInFile = PXFileDataPosition(pxFile);

        PXFileCursorAdvance(pxFile, vertexSourceInfo->BufferSize); // Skip reading actuall data for now..
    }
};


void PXAPI PXRGeometricMapMeshEnvironmentAssetChannel(PXEnvironmentAssetChannel PXREF pxEnvironmentAssetChannel, PXFile PXREF pxFile)
{
    PXFileReadI32U(pxFile, &pxEnvironmentAssetChannel->TextureNameSize);
    PXFileReadA(pxFile, pxEnvironmentAssetChannel->TextureName, pxEnvironmentAssetChannel->TextureNameSize);

    PXFileReadB(pxFile, pxEnvironmentAssetChannel->TextureScale.Data, sizeof(PXVector2F32));
    PXFileReadB(pxFile, pxEnvironmentAssetChannel->TextureBias.Data, sizeof(PXVector2F32));
}


void PXAPI PXRGeometricMapBucketedGeometryLoad(PXRGeometricMap PXREF pxRGeometricMap, PXRGeometricMapSceneGraph PXREF pxRGeometricMapSceneGraph, PXFile PXREF pxFile)
{
    if(!pxRGeometricMapSceneGraph->IsLegacy)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMapSceneGraph->VisibilityControllerPathHash);
    }

    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MinX);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MinZ);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MaxX);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MaxZ);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MaxStickOutX);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->MaxStickOutZ);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->BucketSizeX);
    PXFileReadF(pxFile, &pxRGeometricMapSceneGraph->BucketSizeZ);

    PXFileReadI16U(pxFile, &pxRGeometricMapSceneGraph->BucketsPerSide);
    PXFileReadI8U(pxFile, &pxRGeometricMapSceneGraph->IsDisabled);
    PXFileReadI8U(pxFile, &pxRGeometricMapSceneGraph->Flags);
    PXFileReadI32U(pxFile, &pxRGeometricMapSceneGraph->VertexCount);
    PXFileReadI32U(pxFile, &pxRGeometricMapSceneGraph->IndexCount);

    if(pxRGeometricMapSceneGraph->IsDisabled)
    {
        return; 
    }

    pxRGeometricMapSceneGraph->FaceCount = pxRGeometricMapSceneGraph->IndexCount / 3;

    // Alloc
    pxRGeometricMapSceneGraph->VertexData = PXMemoryHeapCallocT(PXVector3F32, pxRGeometricMapSceneGraph->VertexCount);
    pxRGeometricMapSceneGraph->IndexData = PXMemoryHeapCallocT(PXI16U, pxRGeometricMapSceneGraph->IndexCount);
    pxRGeometricMapSceneGraph->GeometryBucket = PXMemoryHeapCallocT(PXRGeometricMapSceneGraphGeometryBucket, pxRGeometricMapSceneGraph->BucketsPerSide * pxRGeometricMapSceneGraph->BucketsPerSide);


    PXFileReadB(pxFile, pxRGeometricMapSceneGraph->VertexData, sizeof(PXVector3F32) * pxRGeometricMapSceneGraph->VertexCount);
    PXFileReadB(pxFile, pxRGeometricMapSceneGraph->IndexData, sizeof(PXI16U) * pxRGeometricMapSceneGraph->IndexCount);

    for(PXI16U y = 0; y < pxRGeometricMapSceneGraph->BucketsPerSide; ++y)
    {
        for(PXI16U x = 0; x < pxRGeometricMapSceneGraph->BucketsPerSide; ++x)
        {
            PXRGeometricMapSceneGraphGeometryBucket* geometryBucket = &pxRGeometricMapSceneGraph->GeometryBucket[pxRGeometricMapSceneGraph->BucketsPerSide * y + x];

            PXFileReadF(pxFile, &geometryBucket->MaxStickOutX);
            PXFileReadF(pxFile, &geometryBucket->MaxStickOutZ);
            PXFileReadI32U(pxFile, &geometryBucket->StartIndex);
            PXFileReadI32U(pxFile, &geometryBucket->BaseVertex);
            PXFileReadI16U(pxFile, &geometryBucket->InsideFaceCount);
            PXFileReadI16U(pxFile, &geometryBucket->StickingOutFaceCount);
        }
    }

    if(PXRGeometricMapSceneGraphHasFaceVisibilityFlags & pxRGeometricMapSceneGraph->Flags)
    {
        pxRGeometricMapSceneGraph->IndexData = PXMemoryHeapCallocT(PXByte, pxRGeometricMapSceneGraph->FaceCount);

        PXFileReadB(pxFile, pxRGeometricMapSceneGraph->IndexData, sizeof(PXByte) * pxRGeometricMapSceneGraph->FaceCount);
    }
}

void PXAPI PXRGeometricMapMeshLoad(PXRGeometricMap PXREF pxRGeometricMap, PXRGeometricMapMesh PXREF pxRGeometricMapMesh, PXFile PXREF pxFile)
{
    if(pxRGeometricMap->Header.Version <= 11)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMapMesh->NameSize);
        PXFileReadA(pxFile, pxRGeometricMapMesh->Name, pxRGeometricMapMesh->NameSize);
    }
    else
    {
        // We dont have a name, generate one?
        pxRGeometricMapMesh->NameSize = PXTextPrintA(pxRGeometricMapMesh->Name, 260, "**NoName**");
    }


    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->VertexCount);
    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->VertexDeclarationCount);
    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->VertexDeclarationID);


    // IVertexBufferView[] vertexBufferViews = new IVertexBufferView[vertexDeclarationCount];
    for(PXI32U i = 0; i < pxRGeometricMapMesh->VertexDeclarationCount; ++i)
    {
        PXI32U vertexBufferID = 0;

        PXFileReadI32U(pxFile, &vertexBufferID);

        // Go to vertexData offset and read, then jump back here

        /*
        vertexBufferViews[i] = environmentAsset.ProvideVertexBuffer(
            vertexBufferId,
            vertexDeclarations[vertexDeclarationId + i],
            vertexCount,
            br,
            vertexBufferOffsets[vertexBufferId]
        );
        */
    }

    //this.VerticesView = new(vertexCount, vertexBufferViews);



    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->IndexCount);
    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->IndexBufferID);

    // Get index array
    //this.Indices = environmentAsset.ProvideIndexBuffer(indexBufferId);

    if(pxRGeometricMap->Header.Version >= 13)
    {
        PXFileReadI8U(pxFile, &pxRGeometricMapMesh->EnvironmentVisibilityFlags);
    }

    if(pxRGeometricMap->Header.Version >= 15)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMapMesh->VisibilityControllerPathHash);
    }


    //-----------------------------------------------------
    // Mesh
    //-----------------------------------------------------
    PXFileReadI32U(pxFile, &pxRGeometricMapMesh->SubMeshCount);

    if(pxRGeometricMapMesh->SubMeshCount == 1)
    {
        pxRGeometricMapMesh->SubMeshElementList = &pxRGeometricMapMesh->SubMeshElementPrime;
    }
    else
    {
#if 0
        pxRGeometricMapMesh->SubMeshElementList = PXMemoryVirtualAllocate
        (
            sizeof(PXRGeometricMapMeshElement) * pxRGeometricMapMesh->SubMeshCount,
            0,
            PXAccessModeReadAndWrite
        );
#else
        pxRGeometricMapMesh->SubMeshElementList = PXMemoryHeapCallocT(PXRGeometricMapMeshElement, pxRGeometricMapMesh->SubMeshCount);
#endif
    }
 

    for(PXI32U i = 0; i < pxRGeometricMapMesh->SubMeshCount; ++i)
    {
        PXRGeometricMapMeshElement PXREF pxRGeometricMapMeshElement = &pxRGeometricMapMesh->SubMeshElementList[i];

        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->Hash);
        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->MaterialNameLength);
        pxRGeometricMapMeshElement->MaterialNameOffset = PXFileDataPosition(pxFile); // Store string position
        PXFileCursorAdvance(pxFile, pxRGeometricMapMeshElement->MaterialNameLength); // Skip over string, no store
        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->StartIndex);
        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->IndexCount);
        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->MinVertex);
        PXFileReadI32U(pxFile, &pxRGeometricMapMeshElement->MaxVertex);

#if PXLogEnable
        char buffer[128];

        const PXSize oldPos = PXFileDataPosition(pxFile);

        PXFileCursorMoveTo(pxFile, pxRGeometricMapMeshElement->MaterialNameOffset);
        PXFileReadA(pxFile, buffer, pxRGeometricMapMeshElement->MaterialNameLength);
        PXFileCursorMoveTo(pxFile, oldPos);


        PXLogPrint
        (
            PXLoggingInfo,
            PXRGeometricMapText,
            "Load",
            "MapMeshElement (%i/%i)\n"
            "%20s : %i\n"
            "%20s : %s (%i)\n"
            "%20s : %i\n"
            "%20s : %i\n"
            "%20s : %i",
            i + 1,
            pxRGeometricMapMesh->SubMeshCount,
            "Hash", pxRGeometricMapMeshElement->Hash,
            "MaterialName", buffer, pxRGeometricMapMeshElement->MaterialNameLength,
            "StartIndex", pxRGeometricMapMeshElement->StartIndex,
            "IndexCount", pxRGeometricMapMeshElement->IndexCount,
            "MinVertex", pxRGeometricMapMeshElement->MinVertex,
            "MaxVertex", pxRGeometricMapMeshElement->MaxVertex
        );
#endif
    }
    //-----------------------------------------------------



    if(pxRGeometricMap->Header.Version != 5)
    {
        PXFileReadI8U(pxFile, &pxRGeometricMapMesh->DisableBackfaceCulling);
    }

    PXFileReadB(pxFile, pxRGeometricMapMesh->BoundingBox, sizeof(PXVector3F32) * 2); // 6x float
    PXFileReadB(pxFile, pxRGeometricMapMesh->Transform.Data, sizeof(PXF32) * 16); // 16x float
    PXFileReadI8U(pxFile, &pxRGeometricMapMesh->EnvironmentQualityFilterFlags);

    if(pxRGeometricMap->Header.Version >= 7 && pxRGeometricMap->Header.Version <= 12) 
    {
        PXFileReadI8U(pxFile, &pxRGeometricMapMesh->VisibilityFlags);
    }

    if(pxRGeometricMap->Header.Version >= 11 && pxRGeometricMap->Header.Version < 14)
    {
        PXFileReadI8U(pxFile, &pxRGeometricMapMesh->RenderFlags);

       // this.LayerTransitionBehavior = this.RenderFlags.HasFlag(EnvironmentAssetMeshRenderFlags.IsDecal)
       //     ? EnvironmentVisibilityTransitionBehavior.TurnVisibleDoesMatchNewLayerFilter
        //    : EnvironmentVisibilityTransitionBehavior.Unaffected;
    }
    else if(pxRGeometricMap->Header.Version >= 14)
    {
        PXFileReadI8U(pxFile, &pxRGeometricMapMesh->LayerTransitionBehavior);

        if(pxRGeometricMap->Header.Version < 16)
        {
            PXI8U x = 0;

            PXFileReadI8U(pxFile, &x); // 16I int 8i?

            pxRGeometricMapMesh->EnvironmentAssetMeshRenderFlags = x;
        }
        else
        {
            PXFileReadI16U(pxFile, &pxRGeometricMapMesh->EnvironmentAssetMeshRenderFlags);
        }
    }

    if(pxRGeometricMap->UseSeparatePointLights && pxRGeometricMap->Header.Version < 7)
    {
        PXFileReadB(pxFile, pxRGeometricMapMesh->PointLight.Data, sizeof(PXVector3F32));
    }

    if(pxRGeometricMap->Header.Version < 9)
    {
        PXFileReadB(pxFile, pxRGeometricMapMesh->SphericalHarmonics, sizeof(PXVector3F32)*9);

        PXRGeometricMapMeshEnvironmentAssetChannel(&pxRGeometricMapMesh->BakedLight, pxFile);

        return; // DONE, PXActionSuccessful
    }

    PXRGeometricMapMeshEnvironmentAssetChannel(&pxRGeometricMapMesh->BakedLight, pxFile);
    PXRGeometricMapMeshEnvironmentAssetChannel(&pxRGeometricMapMesh->StationaryLight, pxFile);


    //-----------------------------------------------------
    // TextureOverrides
    //-----------------------------------------------------
    if(pxRGeometricMap->Header.Version >= 12 && pxRGeometricMap->Header.Version < 17)
    {
        PXRGeometricMapMeshEnvironmentAssetChannel(&pxRGeometricMapMesh->BakedPaint, pxFile);

      //  this._textureOverrides.Add(new(0, bakedPaint.Texture));
    }
    
    if(pxRGeometricMap->Header.Version >= 17)
    {
        PXI32U textureOverrideCount = 0;

        PXFileReadI32U(pxFile, &textureOverrideCount);

        for(PXI32U i = 0; i < textureOverrideCount; i++)
        {
            PXI32U index = 0;
            PXI32U nameLength = 0;
            char name[128];

            PXFileReadI32U(pxFile, &index);
            PXFileReadI32U(pxFile, &nameLength);
            PXFileReadA(pxFile, name, nameLength);
        }

        PXFileReadB(pxFile, pxRGeometricMapMesh->BakedPaintScale.Data, sizeof(PXVector2F32));
        PXFileReadB(pxFile, pxRGeometricMapMesh->BakedPaintBias.Data, sizeof(PXVector2F32));
    }
    //-----------------------------------------------------
}




PXResult PXAPI PXRGeometricMapLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    return PXActionSuccessful;

    PXRGeometricMap pxRGeometricMap;
    PXClear(PXRGeometricMap, &pxRGeometricMap);

    PXMesh* pxMesh = PXNull;

    PXMeshCreate(&pxMesh, PXNull);

    const PXSize amount = PXFileBinding
    (
        pxFile,
        &pxRGeometricMap.Header,
        PXRGeometricMapHeaderList,
        PXRGeometricMapHeaderListSize,
        PXFileBindingRead
    );

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRGeometricMapText,
        "Load",
        "Version: %i",
        pxRGeometricMap.Header.Version
    );
#endif

    if(pxRGeometricMap.Header.Version < 7)
    {
        PXFileReadI8U(pxFile, &pxRGeometricMap.UseSeparatePointLights);
    }

    // ReadSamplerDefs
    if(pxRGeometricMap.Header.Version >= 17)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMap.ShaderEntryAmount);

        PXRGeometricMapShaderEntry* pxRGeometricMapShaderList;

        if(pxRGeometricMap.ShaderEntryAmount <= 2)
        {
            pxRGeometricMapShaderList = pxRGeometricMap.ShaderEntryPrime;
        }
        else
        {
            pxRGeometricMapShaderList = PXMemoryHeapCallocT(PXRGeometricMapShaderEntry, pxRGeometricMap.ShaderEntryAmount);
        }

        for(PXI32U i = 0; i < pxRGeometricMap.ShaderEntryAmount; ++i)
        {
            PXRGeometricMapShaderEntry PXREF pxRGeometricMapShaderEntry = &pxRGeometricMapShaderList[i];

            PXFileReadI32U(pxFile, &pxRGeometricMapShaderEntry->Index);
            PXFileReadI32U(pxFile, &pxRGeometricMapShaderEntry->NameLength);
            PXFileReadA(pxFile, pxRGeometricMapShaderEntry->Name, pxRGeometricMapShaderEntry->NameLength);
        }
    }
    else
    {
        if(pxRGeometricMap.Header.Version >= 9)
        {
            PXRGeometricMapShaderEntry PXREF pxRGeometricMapShaderEntry = &pxRGeometricMap.ShaderEntryPrime[0];

            ++pxRGeometricMap.ShaderEntryAmount;

            pxRGeometricMapShaderEntry->Index = 0;
            PXFileReadI32U(pxFile, &pxRGeometricMapShaderEntry->NameLength);
            PXFileReadA(pxFile, pxRGeometricMapShaderEntry->Name, pxRGeometricMapShaderEntry->NameLength);
        }

        if(pxRGeometricMap.Header.Version >= 11)
        {
            PXRGeometricMapShaderEntry PXREF pxRGeometricMapShaderEntry = &pxRGeometricMap.ShaderEntryPrime[1];

            ++pxRGeometricMap.ShaderEntryAmount;

            pxRGeometricMapShaderEntry->Index = 1;
            PXFileReadI32U(pxFile, &pxRGeometricMapShaderEntry->NameLength);
            PXFileReadA(pxFile, pxRGeometricMapShaderEntry->Name, pxRGeometricMapShaderEntry->NameLength);
        } 
    }

#if PXLogEnable
    for(PXI32U i = 0; i < pxRGeometricMap.ShaderEntryAmount; ++i)
    {
        PXRGeometricMapShaderEntry* pxRGeometricMapShaderEntry = PXNull;

        if(pxRGeometricMap.ShaderEntryAmount <= 2)
        {
            pxRGeometricMapShaderEntry = &pxRGeometricMap.ShaderEntryPrime[i];
        }
        else
        {
            pxRGeometricMapShaderEntry = &pxRGeometricMap.ShaderEntryList[i];
        }

        PXLogPrint
        (
            PXLoggingInfo,
            PXRGeometricMapText,
            "Load",
            "ShaderEntry : %2i/%2i - Index:%i, Name:%s (%i)",
            i + 1,
            pxRGeometricMap.ShaderEntryAmount,
            pxRGeometricMapShaderEntry->Index,      
            pxRGeometricMapShaderEntry->Name,
            pxRGeometricMapShaderEntry->NameLength
        );
    }
#endif



    // Read vertex declarations
    PXFileReadI32U(pxFile, &pxRGeometricMap.VertexDeclarationCount);

    pxRGeometricMap.VertexDeclarationList = PXMemoryHeapCallocT(PXRGeometricMapVertexDeclaration, pxRGeometricMap.VertexDeclarationCount);

    for(PXI32U i = 0; i < pxRGeometricMap.VertexDeclarationCount; ++i)
    {
        PXRGeometricMapVertexDeclaration PXREF pxRGeometricMapVertexDeclaration = &pxRGeometricMap.VertexDeclarationList[i];

        PXFileReadI32U(pxFile, &pxRGeometricMapVertexDeclaration->VertexBufferUsage);
        PXFileReadI32U(pxFile, &pxRGeometricMapVertexDeclaration->VertexElementCount);

        for(PXI32U j = 0; j < pxRGeometricMapVertexDeclaration->VertexElementCount; ++j)
        {
            PXI32U name;
            PXI32U format;

            PXFileReadI32U(pxFile, &name);
            PXFileReadI32U(pxFile, &format);
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXRGeometricMapText,
            "Load",
            "VertexDeclaration : %2i/%2i - Usage:%i, Count:%i",
            i+1,
            pxRGeometricMap.VertexDeclarationCount,
            pxRGeometricMapVertexDeclaration->VertexBufferUsage,
            pxRGeometricMapVertexDeclaration->VertexElementCount
        );
#endif

        PXFileCursorAdvance(pxFile, 8 * (15 - pxRGeometricMapVertexDeclaration->VertexElementCount));
    }

    // Reading of vertex buffers is deferred until we start reading meshes
    PXFileReadI32U(pxFile, &pxRGeometricMap.VertexBufferCount);

    BufferSourceInfo* vertexSourceInfoList = PXMemoryHeapCallocT(BufferSourceInfo, pxRGeometricMap.VertexBufferCount);
        
    PXBufferSourceInfoRead(pxFile, vertexSourceInfoList, pxRGeometricMap.VertexBufferCount, pxRGeometricMap.Header.Version);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRGeometricMapText,
        "Load",
        "VertexBuffer Amount: %i",
        pxRGeometricMap.VertexBufferCount
    );
#endif
   

    PXFileReadI32U(pxFile, &pxRGeometricMap.IndexBufferCount);

    BufferSourceInfo* indexSourceInfoList = PXMemoryHeapCallocT(BufferSourceInfo, pxRGeometricMap.IndexBufferCount);

    PXBufferSourceInfoRead(pxFile, indexSourceInfoList, pxRGeometricMap.IndexBufferCount, pxRGeometricMap.Header.Version);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRGeometricMapText,
        "Load",
        "IndeBuffer Amount: %i",
        pxRGeometricMap.IndexBufferCount
    );
#endif

    //-----------------------------------------------------
    // Read meshes
    //-----------------------------------------------------
    PXFileReadI32U(pxFile, &pxRGeometricMap.MeshCount);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRGeometricMapText,
        "Load",
        "Mesh Amount : %i",
        pxRGeometricMap.MeshCount
    );
#endif

    PXRGeometricMapMesh PXREF pxRGeometricMapMeshList = PXMemoryHeapCallocT(PXRGeometricMapMesh, pxRGeometricMap.MeshCount);

    for(PXI32U i = 0; i < pxRGeometricMap.MeshCount; ++i)
    {
        PXRGeometricMapMesh* pxRGeometricMapMesh = &pxRGeometricMapMeshList[i];

        PXRGeometricMapMeshLoad(&pxRGeometricMap, pxRGeometricMapMesh, pxFile);
    }   
    //-----------------------------------------------------


    // Read bucketed geometry
    if(pxRGeometricMap.Header.Version >= 15)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMap.SceneGraphCount);

        pxRGeometricMap.SceneGraphList = PXMemoryHeapCallocT(PXRGeometricMapSceneGraph, pxRGeometricMap.SceneGraphCount);

        for(PXI32U i = 0; i < pxRGeometricMap.SceneGraphCount; ++i)
        {
            PXRGeometricMapBucketedGeometryLoad(&pxRGeometricMap, pxRGeometricMap.SceneGraphList, pxFile);
        }
    }
    else
    {
        PXRGeometricMapSceneGraph* sceneGraph = &pxRGeometricMap.SceneGraphPrime;
        sceneGraph->IsLegacy = PXTrue;

        pxRGeometricMap.SceneGraphCount = 1;

        PXRGeometricMapBucketedGeometryLoad(&pxRGeometricMap, sceneGraph, pxFile);
    }

    // Read reflection planes
    if(pxRGeometricMap.Header.Version >= 13)
    {
        PXFileReadI32U(pxFile, &pxRGeometricMap.PlanarReflectorCount);

        for(PXI32U i = 0; i < pxRGeometricMap.PlanarReflectorCount; ++i)
        {

            PXMatrix4x4F transform;
            PXVector3F32 plane[2];
            PXVector3F32 normal;

            PXFileReadB(pxFile, transform.Data, sizeof(PXMatrix4x4F));
            PXFileReadB(pxFile, transform.Data, sizeof(PXVector3F32)*2);
            PXFileReadB(pxFile, normal.Data, sizeof(PXVector3F32));

           // this._planarReflectors.Add(PlanarReflector.ReadFromMapGeometry(br));
        }        
    }

    // DONE, now translate! 

    return PXActionSuccessful;
}

PXResult PXAPI PXRGeometricMapSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}