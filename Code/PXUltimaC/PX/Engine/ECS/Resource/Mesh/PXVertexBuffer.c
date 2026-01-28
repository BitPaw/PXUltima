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

PXResult PXAPI PXVertexBufferRegisterToECS(void)
{
    PXECSRegister(&PXVertexBufferRegisterInfoStatic, &PXVertexBufferRegisterInfoDynamic);

    return PXResultOK;
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

    return PXResultOK;
}

PXResult PXAPI PXVertexBufferRelease(PXVertexBuffer PXREF pxVertexBuffer)
{
    return PXResultRefusedParameterNull;
}

PXResult PXAPI PXVertexBufferLayoutPrint(const PXVertexBuffer PXREF pxVertexBuffer)
{
    PXBufferLayoutEntry PXREF pxVertexBufferLayoutList = PXBufferLayoutGET(&pxVertexBuffer->Layout);

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
        "UpdatePerPrimitive", pxVertexBufferLayoutList->UpdatePerPrimitive
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

    return PXResultOK;
}