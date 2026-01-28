#include "PXIndexBuffer.h"

#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

const char PXIndexBufferText[] = "IndexBuffer";
const PXI8U PXIndexBufferTextLength = sizeof(PXIndexBufferText);
const PXECSRegisterInfoStatic PXIndexBufferRegisterInfo =
{
    {sizeof(PXIndexBufferText), sizeof(PXIndexBufferText), PXIndexBufferText, TextFormatASCII},
    sizeof(PXIndexBuffer),
    __alignof(PXIndexBuffer),
    PXECSTypeResource,
    PXIndexBufferCreate,
    PXIndexBufferRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXIndexBufferRegisterInfoDynamic;


void PXAPI PXIndexSegmentSET(PXIndexSegment PXREF pxIndexSegment, const PXSize amount)
{
    pxIndexSegment->SegmentListAmount = amount;

    if(0 == amount || 1 == amount)
    {
        pxIndexSegment->SegmentListAmount = 1;
        return;
    }

    pxIndexSegment->SegmentListAmount = amount;
    pxIndexSegment->SegmentList = PXMemoryHeapCallocT(PXIndexSegmentEntry, amount);
}

PXIndexSegmentEntry* PXAPI PXIndexSegmentGET(PXIndexSegment PXREF pxIndexSegment)
{
    if(1 == pxIndexSegment->SegmentListAmount)
    {
        return &pxIndexSegment->SegmentPrime;
    }
    else
    {
        return pxIndexSegment->SegmentList;
    }
}

PXResult PXAPI PXIndexBufferRegisterToECS(void)
{
    PXECSRegister(&PXIndexBufferRegisterInfo, &PXIndexBufferRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXIndexBufferCreate(PXIndexBuffer** pxIndexBufferREF, PXIndexBufferCreateInfo PXREF pxIndexBufferCreateInfo)
{
    PXIndexBuffer* pxIndexBuffer = PXNull;
    PXResult pxResult = PXResultInvalid;

    if(!(pxIndexBufferREF && pxIndexBufferCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxIndexBufferCreateInfo->Info.Static = &PXIndexBufferRegisterInfo;
    pxIndexBufferCreateInfo->Info.Dynamic = &PXIndexBufferRegisterInfoDynamic;
    pxResult = PXECSCreate(pxIndexBufferREF, pxIndexBufferCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxIndexBuffer = *pxIndexBufferREF;
    pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;

    const PXSize amountVertex = pxIndexBufferCreateInfo->AmountVertex;

    // Index array data Type    
    if(amountVertex <= 0xFFu)
    {
        // 8-Bit
        pxIndexBuffer->DataType = PXTypeI08U;
    }
    else if(amountVertex <= 0xFFFFu)
    {
        // 16-Bit
        pxIndexBuffer->DataType = PXTypeI16U;
    }
    else if(amountVertex <= 0xFFFFFFFFu)
    {
        // 32-Bit
        pxIndexBuffer->DataType = PXTypeI32U;
    }
    else
    {
        // 64-Bit
        pxIndexBuffer->DataType = PXTypeI64U;
    }

    const PXSize dataSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    pxResult = PXBufferAllocate(&pxIndexBuffer->Data, amountVertex * dataSize * 3);

    if(PXResultOK != pxResult)
    {
        DebugBreak();
        return pxResult;
    }

    PXSize indexAmount = 3;


    PXBufferLayoutInfo pxBufferLayoutInfo;
    PXClear(PXBufferLayoutInfo, &pxBufferLayoutInfo);
    pxBufferLayoutInfo.Amount = 3;

    PXBufferLayoutSET(&pxIndexBuffer->Layout, &pxBufferLayoutInfo);
    PXBufferLayoutEntry* pxBufferLayoutEntry = PXBufferLayoutGET(&pxIndexBuffer->Layout);

    PXIndexSegmentSET(&pxIndexBuffer->Segment, pxIndexBufferCreateInfo->AmountSegments);

    if(1== pxIndexBuffer->Segment.SegmentListAmount)
    {
        pxIndexBuffer->Segment.SegmentPrime.DataRange = pxIndexBufferCreateInfo->AmountVertex;
    }

    pxBufferLayoutEntry[0].Format = pxIndexBuffer->DataType;
    pxBufferLayoutEntry[0].AmountOfElements = 1;
    pxBufferLayoutEntry[0].Type = PXVertexBufferLayoutTypePosition;

    pxBufferLayoutEntry[1].Format = pxIndexBuffer->DataType;
    pxBufferLayoutEntry[1].AmountOfElements = 1;
    pxBufferLayoutEntry[1].Type = PXVertexBufferLayoutTypeNormal;

    pxBufferLayoutEntry[2].Format = pxIndexBuffer->DataType;
    pxBufferLayoutEntry[2].AmountOfElements = 1;
    pxBufferLayoutEntry[2].Type = PXVertexBufferLayoutTypeTexturePos;





#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIndexBufferText,
        "Create",
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i",
        "PXID", pxIndexBuffer->Info.ID,
        "TypeSize", dataSize,
        "IndexAmount", indexAmount,
        "VertexAmount", amountVertex,
        "Segments", pxIndexBufferCreateInfo->AmountSegments
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXIndexBufferRelease(PXIndexBuffer PXREF pxIndexBuffer)
{
    return PXActionRefusedNotImplemented;
}

PXSize PXAPI PXIndexBufferAmount(const PXIndexBuffer PXREF pxIndexBuffer)
{
    return (pxIndexBuffer->Data.CursorOffsetByte / PXTypeSizeGet(pxIndexBuffer->DataType)); // / 3
}

PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer PXREF pxIndexBuffer)
{
    return pxIndexBuffer->IBO != 0;
}

PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer PXREF pxIndexBuffer, const PXI8U type)
{
#if 1



#else


    PXBufferLayout PXREF pxBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);

    for(size_t i = 0; i < pxIndexBuffer->LayoutListAmount; i++)
    {
        PXBufferLayout PXREF pxBufferLayout = &pxBufferLayoutList[i];

        if(type != pxBufferLayout->Type)
        {
            continue;
        }

        pxBufferLayout->
    }

    return 0;
#endif
}

PXIndexBufferIndexSET PXAPI PXIndexBufferIndexFunc(const PXIndexBuffer PXREF pxIndexBuffer)
{
    const PXI8U typeSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    switch(typeSize)
    {
        case PXTypeSize08:
        {
            return PXIndexBufferIndex08SET;
        }
        case PXTypeSize16:
        {
            return PXIndexBufferIndex16SET;
        }
        case PXTypeSize32:
        {
            return PXIndexBufferIndex32SET;
        }
        case PXTypeSize64:
        {
            return PXIndexBufferIndex64SET;
        }
        default:
        {
            return PXNull;
        }
    }
}

void PXAPI PXIndexBufferIndex08SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI8U PXREF indexInsert = pxIndexBuffer->Data.Data;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}

void PXAPI PXIndexBufferIndex16SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI16U PXREF indexInsert = pxIndexBuffer->Data.Data;

    indexInsert[index * 3+0] = indexData[0];
    indexInsert[index * 3+1] = indexData[1];
    indexInsert[index * 3+2] = indexData[2];
}

void PXAPI PXIndexBufferIndex32SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI32U PXREF indexInsert = pxIndexBuffer->Data.Data;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}

void PXAPI PXIndexBufferIndex64SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI64U PXREF indexInsert = pxIndexBuffer->Data.Data;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}