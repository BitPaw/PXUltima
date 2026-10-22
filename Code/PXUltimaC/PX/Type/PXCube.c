#include "PXCube.h"
#include <PX/Type/PXVertex.h>
#include <PX/Type/PXMesh.h>
#include <PX/OS/PXOS.h>

const PXVertexP3N3T2F32 PXVertexDataCube[] =
{
    // Front (+Z)
    {{-1.0f,-1.0f,+1.0f}, { 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{+1.0f,-1.0f,+1.0f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{+1.0f,+1.0f,+1.0f}, { 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-1.0f,+1.0f,+1.0f}, { 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

    // Back (-Z)
    {{-1.0f,-1.0f,-1.0f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 0.0f}},
    {{+1.0f,-1.0f,-1.0f}, { 0.0f, 0.0f,-1.0f}, {0.0f, 0.0f}},
    {{+1.0f,+1.0f,-1.0f}, { 0.0f, 0.0f,-1.0f}, {0.0f, 1.0f}},
    {{-1.0f,+1.0f,-1.0f}, { 0.0f, 0.0f,-1.0f}, {1.0f, 1.0f}},

    // Left (-X)
    {{-1.0f,-1.0f,-1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-1.0f,-1.0f,+1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-1.0f,+1.0f,+1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{-1.0f,+1.0f,-1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

    // Right (+X)
    {{+1.0f,-1.0f,-1.0f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{+1.0f,-1.0f,+1.0f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{+1.0f,+1.0f,+1.0f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    {{+1.0f,+1.0f,-1.0f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

    // Top (+Y)
    {{-1.0f,+1.0f,-1.0f}, { 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{+1.0f,+1.0f,-1.0f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{+1.0f,+1.0f,+1.0f}, { 0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-1.0f,+1.0f,+1.0f}, { 0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

    // Bottom (-Y)
    {{-1.0f,-1.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}, {0.0f, 1.0f}},
    {{+1.0f,-1.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 1.0f}},
    {{+1.0f,-1.0f,+1.0f}, { 0.0f,-1.0f, 0.0f}, {1.0f, 0.0f}},
    {{-1.0f,-1.0f,+1.0f}, { 0.0f,-1.0f, 0.0f}, {0.0f, 0.0f}}
};
const PXI16U PXVertexDataCubeSize = sizeof(PXVertexDataCube);

const PXI8U PXIndexDataCube[] =
{
#if 0
    0,1,2,3, // Left
    0,2,4,6, // Back
    2,3,7,6, // Top
    4,5,6,7, // Right
    0,1,4,5, // Bottom
    1,3,5,7  // Font
#else
     0, 1, 2,   2, 3, 0,          // front
     4, 5, 6,   6, 7, 4,          // back
     8, 9,10,  10,11, 8,          // left
    12,13,14,  14,15,12,          // right
    16,17,18,  18,19,16,          // top
    20,21,22,  22,23,20           // bottom
#endif
};
const PXI8U PXIndexDataCubeSize = sizeof(PXIndexDataCube);


PXVertexBuffer* _cubeVertexBuffer = 0;
PXIndexBuffer* _cubeIndexBuffer = 0;
PXMesh* _cubeMesh = 0;

PXResult PXAPI PXCubeCreate(PXModel** pxModel)
{
    PXResult pxResult = PXResultInvalid;

    // Create index buffer if not exist
    if(!_cubeVertexBuffer)
    {
        PXVertexBufferCreateInfo pxVertexBufferCreateInfo;
        PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);
        PXTextFromAddressA(&pxVertexBufferCreateInfo.Info.Name, "Cube_VertexBuffer", 18, 18);
        pxVertexBufferCreateInfo.VertexDataAddressConst = PXVertexDataCube;
        pxVertexBufferCreateInfo.VertexDataSize = PXVertexDataCubeSize;
        pxVertexBufferCreateInfo.IsConstData = PXTrue;

        pxResult = PXVertexBufferCreate(&_cubeVertexBuffer, &pxVertexBufferCreateInfo);
    }

    // Create vertex buffer if not exist
    if(!_cubeIndexBuffer)
    {
        PXIndexBufferCreateInfo pxIndexBufferCreateInfo;
        PXClear(PXIndexBufferCreateInfo, &pxIndexBufferCreateInfo);
        PXTextFromAddressA(&pxIndexBufferCreateInfo.Info.Name, "Cube_IndexBuffer", 17, 17);
        pxIndexBufferCreateInfo.IndexDataAddressConst = PXIndexDataCube;
        pxIndexBufferCreateInfo.IndexAmount = PXIndexDataCubeSize;
        pxIndexBufferCreateInfo.IsConstData = PXTrue;
        pxIndexBufferCreateInfo.Dimensions = 3u;
        pxIndexBufferCreateInfo.BitPerIndex = 8;
        pxIndexBufferCreateInfo.AmountSegments = 1u;

        pxResult = PXIndexBufferCreate(&_cubeIndexBuffer, &pxIndexBufferCreateInfo);
    }

    // Create mesh if not exist
    if(!_cubeMesh)
    {
        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        PXTextFromAddressA(&pxMeshCreateInfo.Info.Name, "Cube_Mesh", 10, 10);
        pxMeshCreateInfo.IndexBuffer = _cubeIndexBuffer;
        pxMeshCreateInfo.VertexBuffer = _cubeVertexBuffer;

        pxResult = PXMeshCreate(&_cubeMesh, &pxMeshCreateInfo);
    }


    // Create model, always.
    {
        PXModelCreateInfo pxModelCreateInfo;
        PXClear(PXModelCreateInfo, &pxModelCreateInfo);
        PXTextFromAddressA(&pxModelCreateInfo.Info.Name, "Cube_Model", 11, 11);
        pxModelCreateInfo.MeshListAmount = 1;
        pxModelCreateInfo.MeshList = _cubeMesh;

        pxResult = PXModelCreate(pxModel, &pxModelCreateInfo);
    }

    return pxResult;
}