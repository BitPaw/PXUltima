#include "PXD8G.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

const char PXD8G[] = "D8G";

void PXAPI PXD8GGeometryVertexFVFParse(PXD8GeometryVertexFVF PXREF pxD8GeometryVertexFVF, PXFile PXREF pxFile)
{
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Flag);
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Offset);
    PXFileReadI16U(pxFile, &pxD8GeometryVertexFVF->Format);
    PXFileReadI8U(pxFile, &pxD8GeometryVertexFVF->VertexType);
    PXFileReadI8U(pxFile, &pxD8GeometryVertexFVF->UVChannelIndex);
}

PXResult PXAPI PXD8GLoadFromFile(PXModel** pxModelREF, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXD8Geometry dummy;
    PXClear(PXD8Geometry, &dummy);

    PXD8Geometry* pxD8Geometry = &dummy;

    PXFileReadI32U(pxFile, &pxD8Geometry->Header.VertexType);
    PXFileReadI32U(pxFile, &pxD8Geometry->Header.FaceType);
    PXFileReadI32U(pxFile, &pxD8Geometry->Header.MeshAmount);
    
    // Hard fix to one mesh
#if 0
    pxD8Geometry->Header.MeshAmount = 2;
#endif // 1


    pxD8Geometry->MeshList = PXMemoryHeapCallocT(PXD8GeometryMesh, pxD8Geometry->Header.MeshAmount);



    // If we dont have a model jet, make it.

    PXModelCreateInfo pxModelCreateInfo;
    PXClear(PXModelCreateInfo, &pxModelCreateInfo);
    pxModelCreateInfo.MeshAmount = pxD8Geometry->Header.MeshAmount;

    PXModelCreate(pxModelREF, &pxModelCreateInfo);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXD8G,
        "Mesh",
        "VertexType:%i, FaceType:%i, MeshAmount:%i",
        pxD8Geometry->Header.VertexType,
        pxD8Geometry->Header.FaceType,
        pxD8Geometry->Header.MeshAmount
    );
#endif

    for(PXSize meshIndex = 0; meshIndex < pxD8Geometry->Header.MeshAmount; ++meshIndex)
    { 
        PXD8GeometryMesh* mesh = &pxD8Geometry->MeshList[meshIndex];

        //-------------------------------------------------
        // Header
        PXFileReadI32U(pxFile, &mesh->TextureID);
        PXFileReadB(pxFile, mesh->Position.Data, sizeof(float) * 4);
        PXFileReadI32U(pxFile, &mesh->RadiusBoundingSphere);
        PXFileReadI32U(pxFile, &mesh->DistanceLevelOfDetailMin);
        PXFileReadI32U(pxFile, &mesh->DistanceLevelOfDetailMax);
        PXFileReadI32U(pxFile, &mesh->NumberOfUnknownStructures);

        

        PXBool skipRead = mesh->NumberOfUnknownStructures < 1 || mesh->NumberOfUnknownStructures > 32;

        if(skipRead)
        {
            PXFileCursorRewind(pxFile, sizeof(PXI32U));

            PXFileReadI32U(pxFile, &mesh->UnknownA);
            PXFileReadI32U(pxFile, &mesh->UnknownB);
            PXFileReadI32U(pxFile, &mesh->UnknownC);
            PXFileReadI32U(pxFile, &mesh->UnknownD);

            PXFileReadI32U(pxFile, &mesh->NumberOfUnknownStructures);
        }
        //-------------------------------------------------


        //-------------------------------------------------
        // Vertex structure info
        mesh->VertexBlock.definition_count = mesh->NumberOfUnknownStructures;

        if(0 != mesh->VertexBlock.definition_count)
        {
            mesh->VertexBlock.definitions = PXMemoryHeapCallocT(PXD8GeometryVertexFVF, mesh->VertexBlock.definition_count);

            for(PXI32U i = 0; i < mesh->VertexBlock.definition_count; ++i)
            {
                PXD8GeometryVertexFVF* pxD8GeometryVertexFVFsSub = &mesh->VertexBlock.definitions[i];

                PXD8GGeometryVertexFVFParse(pxD8GeometryVertexFVFsSub, pxFile);
            }
        }
        else
        {
            for(PXI32U i = 0; ; i++)
            {
                ++mesh->VertexBlock.definition_count;
                mesh->VertexBlock.definitions = PXMemoryHeapReallocT(PXD8GeometryVertexFVF, mesh->VertexBlock.definitions, mesh->VertexBlock.definition_count);

                PXD8GeometryVertexFVF* pxD8GeometryVertexFVFsSub = &mesh->VertexBlock.definitions[i];

                PXD8GGeometryVertexFVFParse(pxD8GeometryVertexFVFsSub, pxFile);
                            
                if(0xFF == pxD8GeometryVertexFVFsSub->Flag)
                {
                    break;
                }
            }
        }

        PXFileReadI32U(pxFile, &mesh->VertexBlock.BytesBetweenVertices);
        PXFileReadI32U(pxFile, &mesh->VertexBlock.NumberOfVertices);       
        //-------------------------------------------------


        //-------------------------------------------------
        // Vertex
        const PXSize currentPosition = PXFileDataPosition(pxFile);
        const PXSize expectedEndPosition = currentPosition + mesh->VertexBlock.NumberOfVertices * mesh->VertexBlock.BytesBetweenVertices;

        // Alloc?
        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        pxMeshCreateInfo.VertexStride = mesh->VertexBlock.BytesBetweenVertices;
        pxMeshCreateInfo.VertexAmount = mesh->VertexBlock.NumberOfVertices;

        PXMesh** pxMeshREF = PXModelMeshGet(*pxModelREF, meshIndex);


        PXMeshCreate(pxMeshREF, &pxMeshCreateInfo);

        PXMesh* pxMesh = *pxMeshREF;

        pxMesh->Position = mesh->Position;
        pxMesh->TextureID = mesh->TextureID;

        PXVector3F32* positions = pxMeshCreateInfo.Positions;
        PXVector3F32* normals = pxMeshCreateInfo.Normals;
        PXVector2F32* uVs = pxMeshCreateInfo.UVs;

        // Read vertex data
        for(PXI32U v = 0; v < mesh->VertexBlock.NumberOfVertices; v++)
        {
            for(PXI32U u = 0; u < mesh->NumberOfUnknownStructures; u++)
            {
                PXD8GeometryVertexFVF* pxD8GeometryVertexFVFsSub = &mesh->VertexBlock.definitions[u];

                if(pxD8GeometryVertexFVFsSub->Flag == 0xFF)
                    continue;

                PXI32U offset =
                    currentPosition + (v * mesh->VertexBlock.BytesBetweenVertices) + pxD8GeometryVertexFVFsSub->Offset;

                PXFileCursorMoveTo(pxFile, offset);

                PXVector4F32 data;

                switch(pxD8GeometryVertexFVFsSub->Format)
                {
                    case 1: // 2 floats
                        PXFileReadFV(pxFile, &data, 2);
                        break;

                    case 2: // 3 floats
                        PXFileReadFV(pxFile, &data, 3);
                        break;

                    case 4: // 4 bytes normalized
                    {
                        PXByte dataSub[4];

                        PXFileReadB(pxFile, &dataSub, 4);

                        data.X = dataSub[0] / 255.0f;
                        data.Y = dataSub[1] / 255.0f;
                        data.Z = dataSub[2] / 255.0f;
                        data.W = dataSub[3] / 255.0f;

                        break;
                    }                  

                    default:
                        break;
                }

                PXI8U semantic = pxD8GeometryVertexFVFsSub->VertexType & 0xFF;

                switch(semantic)
                {
                    case 0x00: // Position
                        positions[v] = data.V3;
                        break;

                    case 0x03: // Normal
                        normals[v] = data.V3;
                        break;

                    case 0x05: // UV
                        uVs[v].X = data.X;
                        uVs[v].Y = 1.0f - data.Y;
                        break;
                }
            }
        }
        PXFileCursorMoveTo(pxFile, expectedEndPosition);
        //-------------------------------------------------




        //-------------------------------------------------
        // Index
        PXFileReadI32U(pxFile, &mesh->face_count);
        mesh->IndexListArray = PXFileDataPosition(pxFile);
        mesh->IndexListSize = sizeof(PXI16U) * mesh->face_count;

        pxMesh->IndexAmount = mesh->face_count;
        pxMesh->Index = PXMemoryHeapCallocT(PXI16U, pxMesh->IndexAmount);

        //pxMesh->IndexAmount = mesh->face_count;

        PXFileReadB(pxFile, pxMesh->Index, mesh->IndexListSize);

       // PXFileCursorAdvance(pxFile, mesh->IndexListSize);
        //-------------------------------------------------



        PXFileReadI32U(pxFile, &mesh->MatrixListSize);
        mesh->MatrixListAdress = PXMemoryHeapCallocT(PXMatrix4x4F, mesh->MatrixListSize);
        PXFileReadB(pxFile, mesh->MatrixListAdress, sizeof(PXMatrix4x4F) * mesh->MatrixListSize);





#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXD8G,
            "Mesh",
            "\n"
            "%20s : %i\n"
            "%20s : %-6.2f %-6.2f %-6.2f %-6.2f\n"
            "%20s : %i\n"
            "%20s : MIN:%i, MAX:%i\n"
            "%20s : %i, %i, %i, %i\n"
            "%20s : %i\n"
            "%20s : %i\n",
            "TextureID", mesh->TextureID,
            "Position", mesh->Position.X, mesh->Position.Y, mesh->Position.W, mesh->Position.W,
            "RadiusBoundingSphere", mesh->RadiusBoundingSphere,
            "DistanceLOD", mesh->DistanceLevelOfDetailMin, mesh->DistanceLevelOfDetailMax,
            "???", mesh->UnknownA, mesh->UnknownB, mesh->UnknownC, mesh->UnknownD,
            "face_count", mesh->face_count,
            "node_count", mesh->MatrixListSize
        );
#endif
    }

    return PXResultOK;
}