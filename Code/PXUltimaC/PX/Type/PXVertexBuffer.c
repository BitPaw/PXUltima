#include "PXVertexBuffer.h"

#include <PX/OS/Console/PXConsole.h>

const char PXVertexBufferName[] = "VertexBuffer";
const PXI8U PXVertexBufferNameLength = sizeof(PXVertexBufferName);
const PXECSRegisterInfoStatic PXVertexBufferRegisterInfoStatic =
{
    {sizeof(PXVertexBufferName), sizeof(PXVertexBufferName), PXVertexBufferName, TextFormatASCII},
    sizeof(PXVertexBuffer),
    __alignof(PXVertexBuffer),
    PXECSTypeResource,
    PXVertexBufferCreate,
    PXVertexBufferRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXVertexBufferRegisterInfoDynamic;

void PXAPI PXVertexBufferRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXVertexBufferRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXVertexBufferRegisterInfoDynamic;
}

PXResult PXAPI PXVertexBufferCreate(PXVertexBuffer** pxVertexBufferREF, PXVertexBufferCreateInfo PXREF pxVertexBufferCreateInfo)
{
    PXVertexBuffer* pxVertexBuffer = PXNull;

    pxVertexBufferCreateInfo->Info.Static = &PXVertexBufferRegisterInfoStatic;
    pxVertexBufferCreateInfo->Info.Dynamic = &PXVertexBufferRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxVertexBufferREF, pxVertexBufferCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxVertexBuffer = *pxVertexBufferREF;

    pxResult = PXBufferStore
    (
        &pxVertexBuffer->Data,
        pxVertexBufferCreateInfo->VertexDataAddress,
        pxVertexBufferCreateInfo->VertexDataSize,
        pxVertexBufferCreateInfo->IsConstData
    );


    pxResult = PXBufferLayoutPrepare(&pxVertexBuffer->Layout, 3);

    PXSize amount = PXBufferLayoutAmount(&pxVertexBuffer->Layout);

    if(3== amount)
    {
        PXBufferLayoutEntry* pxBufferLayoutPosition = PXBufferLayoutGetViaIndex(&pxVertexBuffer->Layout, 0);
        PXBufferLayoutEntry* pxBufferLayoutNormal = PXBufferLayoutGetViaIndex(&pxVertexBuffer->Layout, 1);
        PXBufferLayoutEntry* pxBufferLayoutTexture = PXBufferLayoutGetViaIndex(&pxVertexBuffer->Layout, 2);

        pxBufferLayoutPosition->AmountOfElements = 3;
        pxBufferLayoutPosition->TypeData = PXTypeF32;
        pxBufferLayoutPosition->TypeUse = PXVertexBufferLayoutTypePosition;
        pxBufferLayoutPosition->UpdatesPerPrimitive = 0;
        pxBufferLayoutPosition->RelativeOffsetInBytes = 0;

        pxBufferLayoutNormal->AmountOfElements = 3;
        pxBufferLayoutNormal->TypeData = PXTypeF32;
        pxBufferLayoutNormal->TypeUse = PXVertexBufferLayoutTypeNormal;
        pxBufferLayoutNormal->UpdatesPerPrimitive = 0;
        pxBufferLayoutNormal->RelativeOffsetInBytes = sizeof(PXF32) * 3;

        pxBufferLayoutTexture->AmountOfElements = 2;
        pxBufferLayoutTexture->TypeData = PXTypeF32;
        pxBufferLayoutTexture->TypeUse = PXVertexBufferLayoutTypeTexturePos;
        pxBufferLayoutTexture->UpdatesPerPrimitive = 0;
        pxBufferLayoutTexture->RelativeOffsetInBytes = sizeof(PXF32) * (3 + 3);
    }

    for(PXSize i = 0; i < amount; i++)
    {
        PXBufferLayoutEntry PXREF pxBufferLayoutEntry = PXBufferLayoutGetViaIndex(&pxVertexBuffer->Layout, i);
      

    }

    
    return pxResult;
}

PXResult PXAPI PXVertexBufferRelease(PXVertexBuffer PXREF pxVertexBuffer)
{
    return PXResultRefusedParameterNull;
}

PXResult PXAPI PXVertexBufferLayoutPrint(const PXVertexBuffer PXREF pxVertexBuffer)
{
    PXBufferLayoutEntry PXREF pxVertexBufferLayoutList = 0;// PXBufferLayoutGetViaIndex(&pxVertexBuffer->Layout);
    /*
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXVertexBufferName,
        "Layout",
        "\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i",
        "PXID", pxVertexBuffer->Info.ID,
        "VBO", pxVertexBuffer->VBO,
        "AmountOfElements", pxVertexBufferLayoutList->AmountOfElements,
        "Format", pxVertexBufferLayoutList->Format,
        "Type", pxVertexBufferLayoutList->Type,
        "UpdatePerPrimitive", pxVertexBufferLayoutList->UpdatesPerPrimitive
    );
#endif

    for(PXSize w = 0; w < pxVertexBuffer->Layout.LayoutAmount; w++)
    {
        PXBufferLayoutEntry* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

        PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
        PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXVertexBufferName,
            "Layout",
            "- (%i/%i) Amount:%i, TypeSize:%i, Type:%i",
            w + 1,
            pxVertexBuffer->Layout.LayoutAmount,
            amountOfElement,
            sizeOfSingleElement,
            pxVertexBufferLayout->Type
        );
#endif
    }
    */

    return PXResultOK;
}