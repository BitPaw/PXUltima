#include "PXIndexBuffer.h"

#include <PX/Type/PXMesh.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Math/PXMath.h>

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

void PXAPI PXIndexBufferRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXIndexBufferRegisterInfo;
    pxECSRegisterInfo->InfoDynamic = &PXIndexBufferRegisterInfoDynamic;
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

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIndexBufferText,
        "Create",
        "\n"
        "%20s : %i\n"
        "%20s : %p (IsConst:%s)\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i",
        "PXID", pxIndexBuffer->Info.ID,
        "IndexDataAddress", pxIndexBufferCreateInfo->IndexDataAddress, pxIndexBufferCreateInfo->IsConstData ? "YES" : " NO",
        "IndexDataSize", pxIndexBufferCreateInfo->IndexAmount,
        "Dimensions", pxIndexBufferCreateInfo->Dimensions,
        "Bit per index", pxIndexBufferCreateInfo->BitPerIndex,
        "Segments", pxIndexBufferCreateInfo->AmountSegments
    );
#endif


    PXAssert(pxIndexBufferCreateInfo->Dimensions > 0, "Dimension is not set, should probably be 3");
    PXAssert(pxIndexBufferCreateInfo->BitPerIndex > 0, "BitPerIndex is not set, needs to be 8, 16, 32, 64, ...");


#if 0 // Debug
    pxIndexBuffer->DrawModeID |= PXDrawModeIDPoint | PXDrawModeIDLineLoop;
#endif

    switch(pxIndexBufferCreateInfo->Dimensions)
    {
        case 3:        
        {
            pxIndexBuffer->DrawModeID |= PXDrawModeIDTriangle;
            break;
        }
        case 4:
        {
            pxIndexBuffer->DrawModeID |= PXDrawModeIDSquare;
            break;
        }
        default:
        {
            break;
        }          
    }

    PXSize indexBytesRequired =
        pxIndexBufferCreateInfo->Dimensions *
        pxIndexBufferCreateInfo->IndexAmount *
        pxIndexBufferCreateInfo->BitPerIndex / 8u;
    
    pxResult = PXBufferStore
    (
        &pxIndexBuffer->Data,
        pxIndexBufferCreateInfo->IndexDataAddress,
        indexBytesRequired,
        pxIndexBufferCreateInfo->IsConstData
    );

    pxIndexBuffer->BitPerIndex = pxIndexBufferCreateInfo->BitPerIndex;

    if(PXResultOK != pxResult)
    {
        PXDebugBreak();
        return pxResult;
    }

    // Setup segment
    PXIndexSegmentPrepare(pxIndexBuffer, pxIndexBufferCreateInfo->AmountSegments);

    const PXSize amount = PXIndexSegmentAmount(pxIndexBuffer);


     PXIndexSegmentSetViaIndex
     (
         pxIndexBuffer, 
         0,
         pxIndexBufferCreateInfo->IndexAmount * pxIndexBufferCreateInfo->Dimensions
     );   


#if 0
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

#endif

    return PXResultOK;
}

PXResult PXAPI PXIndexBufferRelease(PXIndexBuffer PXREF pxIndexBuffer)
{
    return PXResultRefusedNotImplemented;
}

PXSize PXAPI PXIndexBufferAmount(const PXIndexBuffer PXREF pxIndexBuffer)
{
    return 0;// (pxIndexBuffer->Data.CursorOffsetByte / PXTypeSizeGet(pxIndexBuffer->DataType)); // / 3
}

PXResult PXAPI PXIndexSegmentPrepare(PXIndexBuffer PXREF pxIndexBuffer, const PXSize amount)
{
    if(0 == amount || 1 == amount)
    {
        pxIndexBuffer->SegmentAmount = 1;
        return;
    }

    // only when we have more than a single segment we allocate.
    // This safes us an allocation per object.
    pxIndexBuffer->SegmentAmount = amount;
    pxIndexBuffer->SegmentList = PXMemoryHeapCallocT(PXIndexSegment, amount);
}

PXIndexSegment* PXAPI PXIndexSegmentGetViaIndex(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize index)
{
    if(!pxIndexBuffer)
    {
        return PXNull;
    }

    PXAssert(0 != pxIndexBuffer->SegmentAmount, "This isn't a legal state, forgot to init correctly!");

    if(1 == pxIndexBuffer->SegmentAmount)
    {
        return &pxIndexBuffer->SegmentPrime;
    }

    PXBool isInBounds = index < pxIndexBuffer->SegmentAmount;

    if(!isInBounds)
    {
        return 0;
    }

    return &pxIndexBuffer->SegmentList[index];
}

PXResult PXAPI PXIndexSegmentSetViaIndex(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize range)
{
    PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, index);

    if(!pxIndexSegment)
    {
        return PXResultInvalid;
    }

    pxIndexSegment->Range = range;
    //pxIndexSegment->Material = pxMaterial;
}

PXSize PXAPI PXIndexSegmentAmount(const PXIndexBuffer PXREF pxIndexBuffer)
{
    if(!pxIndexBuffer)
    {
        return 0;
    }

    return pxIndexBuffer->SegmentAmount;
}

PXIndexSegment* PXAPI PXIndexSegmentGetViaMaterial(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize materialID)
{
    const PXSize segmentAmount = PXIndexSegmentAmount(pxIndexBuffer);

    for(PXSize i = 0; i < segmentAmount; i++)
    {
        PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, i);
        PXBool isMaterialTarget = pxIndexSegment->MaterialID == materialID;

        if(isMaterialTarget)
        {
            return pxIndexSegment;
        }
    }

    return PXNull;
}

void* PXAPI PXIndexSegmentDataGetViaMaterial(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize materialID)
{
    const PXSize segmentAmount = PXIndexSegmentAmount(pxIndexBuffer);

    for(PXSize i = 0; i < segmentAmount; i++)
    {
        PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, i);
        PXBool isMaterialTarget = pxIndexSegment->MaterialID == materialID;

        if(isMaterialTarget)
        {
            void* ind = PXIndexSegmentDataGet(pxIndexBuffer, i);

            //ind = (PXSize)ind + (pxIndexBuffer->BitPerIndex / 8u) * ;

            return ind;
        }
    }

    return PXNull;
}

void* PXAPI PXIndexSegmentDataGet(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize segmentID)
{
    PXSize segmentAmount = PXIndexSegmentAmount(pxIndexBuffer);
    PXSize totalOffset = 0;

    if(segmentID == 0)
    {
        return pxIndexBuffer->Data.Data;
    }

    segmentAmount = PXMathMinimumIU(segmentAmount, segmentID);

    for(size_t i = 0; i < segmentAmount; i++)
    {
        PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, i);

        totalOffset += pxIndexSegment->Range;
    }

    void* data = 
        pxIndexBuffer->Data.Data +
        (pxIndexBuffer->BitPerIndex / 8) * totalOffset;

    return data;
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
    switch(pxIndexBuffer->BitPerIndex)
    {
        case 8:
        {
            return PXIndexBufferIndex08SET;
        }
        case 16:
        {
            return PXIndexBufferIndex16SET;
        }
        case 32:
        {
            return PXIndexBufferIndex32SET;
        }
        case 64:
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
    PXI8U PXREF indexInsert = pxIndexBuffer->Data.I8U;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}

void PXAPI PXIndexBufferIndex16SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI16U PXREF indexInsert = pxIndexBuffer->Data.I16U;

    indexInsert[index * 3+0] = indexData[0];
    indexInsert[index * 3+1] = indexData[1];
    indexInsert[index * 3+2] = indexData[2];
}

void PXAPI PXIndexBufferIndex32SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI32U PXREF indexInsert = pxIndexBuffer->Data.I32U;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}

void PXAPI PXIndexBufferIndex64SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData)
{
    PXI64U PXREF indexInsert = pxIndexBuffer->Data.I64U;

    indexInsert[index * 3 + 0] = indexData[0];
    indexInsert[index * 3 + 1] = indexData[1];
    indexInsert[index * 3 + 2] = indexData[2];
}