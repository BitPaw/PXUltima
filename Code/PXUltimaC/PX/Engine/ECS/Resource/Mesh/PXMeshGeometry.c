#include "PXMeshGeometry.h"

#include <PX/OS/PXOS.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/Console/PXConsole.h>

const char PXMeshGeometryName[] = "MeshGeometry";
const PXI8U PXMeshGeometryNameLength = sizeof(PXMeshGeometryName);
const PXECSRegisterInfoStatic PXMeshGeometryRegisterInfoStatic =
{
    {sizeof(PXMeshGeometryName), sizeof(PXMeshGeometryName), PXMeshGeometryName, TextFormatASCII},
    sizeof(PXMeshGeometry),
    __alignof(PXMeshGeometry),
    PXECSTypeResource,
    PXMeshGeometryCreate,
    PXMeshGeometryRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXMeshGeometryRegisterInfoDynamic;

PXResult PXAPI PXMeshGeometryRegisterToECS(void)
{
    PXECSRegister(&PXMeshGeometryRegisterInfoStatic, &PXMeshGeometryRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXMeshGeometryCreate(PXMeshGeometry** pxMeshGeometryREF, PXMeshGeometryCreateInfo PXREF pxMeshGeometryCreateInfo)
{
    PXMeshGeometry* pxMeshGeometry = PXNull;

    pxMeshGeometryCreateInfo->Info.Static = &PXMeshGeometryRegisterInfoStatic;
    pxMeshGeometryCreateInfo->Info.Dynamic = &PXMeshGeometryRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxMeshGeometryREF, pxMeshGeometryCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxMeshGeometry = *pxMeshGeometryREF;



    // Vertex
    PXSize vertexAmountList = pxMeshGeometryCreateInfo->VertexBufferLayoutAmount;
    PXVertexBuffer** pxVertexBufferList = PXMeshGeometryVertexBufferListSET(pxMeshGeometry, vertexAmountList);

    for(PXSize i = 0; i < vertexAmountList; ++i)
    {
        PXVertexBuffer* pxVertexBuffer = pxVertexBufferList[i];
        const PXBufferLayoutEntry PXREF pxBufferLayoutEntry = &pxMeshGeometryCreateInfo->VertexBufferLayoutList[i];

        PXBufferLayout* pxBufferLayout = &pxVertexBuffer->Layout;

        PXBufferLayoutInfo pxBufferLayoutInfo;
        pxBufferLayoutInfo.Amount = 1;

        PXBufferLayoutSET(pxBufferLayout, &pxBufferLayoutInfo);

        PXSize size = pxBufferLayoutEntry->Entrys * PXTypeSizeGet(pxBufferLayoutEntry->Format) * pxBufferLayoutEntry->AmountOfElements;

        PXBufferAllocate(&pxVertexBuffer->VertexData, size);
    }

    // INdex
    PXIndexBufferCreateInfo pxIndexBufferCreateInfo;
    PXClear(PXIndexBufferCreateInfo, &pxIndexBufferCreateInfo);
    pxIndexBufferCreateInfo.AmountSegments = pxMeshGeometryCreateInfo->SegmentAmount;
    pxIndexBufferCreateInfo.AmountVertex = pxMeshGeometryCreateInfo->PrimitveAmount;

    PXIndexBufferCreate(&pxMeshGeometry->IndexBuffer, &pxIndexBufferCreateInfo);

    return PXResultOK;
}

PXResult PXAPI PXMeshGeometryRelease(PXMeshGeometry PXREF pxMeshGeometry)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMeshGeometryVertexBufferLayoutPrint(const PXMeshGeometry PXREF pxMeshGeometry)
{
    PXVertexBuffer** pxVertexBufferList = PXMeshGeometryVertexBufferListGET(pxMeshGeometry);

    for(PXSize i = 0; i < pxMeshGeometry->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer* pxVertexBuffer = pxVertexBufferList[i];

        PXVertexBufferLayoutPrint(pxVertexBuffer);
    }

    return PXResultOK;
}

PXVertexBuffer** PXAPI PXMeshGeometryVertexBufferListGET(const PXMeshGeometry PXREF pxMeshGeometry)
{
    if(pxMeshGeometry->VertexBufferListAmount <= PXEmbeddedArraySize)
    {
        return pxMeshGeometry->VertexBufferPrime;
    }
    else
    {
        return pxMeshGeometry->VertexBufferList;
    }
}

PXVertexBuffer** PXAPI PXMeshGeometryVertexBufferListSET(PXMeshGeometry PXREF pxMeshGeometry, const PXSize amount)
{
    PXVertexBufferCreateInfo pxVertexBufferCreateInfo;

    PXAssert(0 != amount, "Not expected");

    if(amount <= PXEmbeddedArraySize)
    {
        pxMeshGeometry->VertexBufferListAmount = amount;
        
        for(PXI8U i = 0; i < amount; ++i)
        {
            PXVertexBuffer** pxVertexBuffer = &pxMeshGeometry->VertexBufferPrime[i];

            PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);

            PXVertexBufferCreate(pxVertexBuffer, &pxVertexBufferCreateInfo);
        }

        return pxMeshGeometry->VertexBufferPrime;
    }
    else
    {
        pxMeshGeometry->VertexBufferListAmount = amount;
        pxMeshGeometry->VertexBufferList = PXMemoryHeapCallocT(PXVertexBuffer*, amount);

        for(PXSize i = 0; i < amount; ++i)
        {
            PXVertexBuffer** pxVertexBuffer = &pxMeshGeometry->VertexBufferList[i];

            PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);

            PXVertexBufferCreate(pxVertexBuffer, &pxVertexBufferCreateInfo);
        }

        return pxMeshGeometry->VertexBufferList;
    }
}
