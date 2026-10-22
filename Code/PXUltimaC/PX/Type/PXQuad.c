#include "PXQuad.h"

#include <PX/OS/PXOS.h>

const PXF32 PXQuadVertexData[] =
{
    // 0: Top-Left
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

    // 1: Top-Right
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

    // 2: Bottom-Right
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,

    // 3: Bottom-Left
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
};

const PXI8U PXQuadIndexData[] =
{
    0,1,2,
    2,3,0
};

PXIndexBuffer* PXQuadIndexBuffer = 0;
PXVertexBuffer* PXQuadVertexBuffer = 0;
PXMesh* PXQuadMesh = 0;


const char PXQuadName[] = "Quad";
const PXI8U PXQuadLength = sizeof(PXQuadName);
const PXECSRegisterInfoStatic PXQuadRegisterInfoStatic =
{
    {sizeof(PXQuadName), sizeof(PXQuadName), PXQuadName, TextFormatASCII},
    sizeof(PXQuad),
    __alignof(PXQuad),
    PXECSTypeContainer,
    PXQuadCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXQuadRegisterInfoDynamic;

void PXAPI PXQuadRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXQuadRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXQuadRegisterInfoDynamic;
}

PXResult PXAPI PXQuadCreate(PXQuad** pxQuadREF)
{
    PXQuad* pxQuad = PXNull;

    PXECSCreateInfo pxECSCreateInfo;
    PXClear(PXECSCreateInfo, &pxECSCreateInfo);
    pxECSCreateInfo.Static = &PXQuadRegisterInfoStatic;
    pxECSCreateInfo.Dynamic = &PXQuadRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxQuadREF, &pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxQuad = *pxQuadREF;


    if(!PXQuadIndexBuffer)
    {
        PXIndexBufferCreateInfo pxIndexBufferCreateInfo;
        PXClear(PXIndexBufferCreateInfo, &pxIndexBufferCreateInfo);
        pxIndexBufferCreateInfo.IsConstData = PXTrue;
        pxIndexBufferCreateInfo.IndexDataAddressConst = PXQuadIndexData;
        pxIndexBufferCreateInfo.IndexAmount = sizeof(PXQuadIndexData);
        pxIndexBufferCreateInfo.Dimensions = 3;
        pxIndexBufferCreateInfo.AmountSegments = 1;

        PXIndexBufferCreate(&PXQuadIndexBuffer, &pxIndexBufferCreateInfo);
    }

    if(!PXQuadVertexBuffer)
    {
        PXVertexBufferCreateInfo pxVertexBufferCreateInfo;
        PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);
        pxVertexBufferCreateInfo.VertexDataSize = sizeof(PXQuadVertexData);
        pxVertexBufferCreateInfo.VertexDataAddressConst = PXQuadVertexData;
        pxVertexBufferCreateInfo.IsConstData = PXTrue;

        PXVertexBufferCreate(&PXQuadVertexBuffer, &pxVertexBufferCreateInfo);
    }

    if(!PXQuadMesh)
    {
        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        pxMeshCreateInfo.VertexBuffer = PXQuadVertexBuffer;
        pxMeshCreateInfo.IndexBuffer = PXQuadIndexBuffer;

        PXMeshCreate(&PXQuadMesh, &pxMeshCreateInfo);
    }

    PXModelCreateInfo pxModelCreateInfo;
    pxModelCreateInfo.MeshList = PXQuadMesh;
    pxModelCreateInfo.MeshListAmount = 1;

    PXClear(PXModelCreateInfo, &pxModelCreateInfo);

    PXModelCreate(&pxQuad->Model, &pxModelCreateInfo);

    return PXResultOK;
}