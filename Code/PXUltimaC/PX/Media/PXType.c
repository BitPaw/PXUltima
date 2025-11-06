#include "PXType.h"

#include <PX/Media/PXText.h>

#define PX_X86_BSWAP_ALLOW OS32B

#define PXTypeInt08ULimit 255 // 0xFF
#define PXTypeInt16ULimit 65535 // 0xFFFF
#define PXTypeInt32ULimit 4294967295 // 0xFFFFFFFF
#define PXTypeInt64ULimit 0xFFFFFFFFFFFFFFFF // 0xFFFFFFFFFFFFFFFF

void PXAPI PXWorkSetCounterCalc(PXWorkSetCounter PXREF pxWorkSetCounter)
{
    if(!(pxWorkSetCounter->WorkToDo && pxWorkSetCounter->BatchSize))
    {
        // We have no work or the batch size is 0, work can't be done like this
        pxWorkSetCounter->AmountBatchFull = 0;
        pxWorkSetCounter->AmountBatchRest = 0;
        return;
    }

    if(pxWorkSetCounter->WorkToDo <= pxWorkSetCounter->BatchSize)
    {
        pxWorkSetCounter->AmountBatchFull = 0;
        pxWorkSetCounter->AmountBatchRest = pxWorkSetCounter->WorkToDo;
        return;
    }

    // return ceil(amount / (PXF32)step);
    pxWorkSetCounter->AmountBatchFull = pxWorkSetCounter->WorkToDo / pxWorkSetCounter->BatchSize;
    pxWorkSetCounter->AmountBatchRest = pxWorkSetCounter->WorkToDo % pxWorkSetCounter->BatchSize;
}

PXSize PXAPI PXWorkSetCounterPull(PXWorkSetCounter PXREF pxWorkSetCounter, const PXSize index)
{
    if(index < pxWorkSetCounter->AmountBatchFull)
    {
        return pxWorkSetCounter->BatchSize;
    }

    return pxWorkSetCounter->AmountBatchRest;
}

void PXAPI PXTypeToString(const PXI32U dataType, char* buffer)
{
//    PXText pxText;
    //PXTextConstructBufferA(&pxText, 256);

    //char bufferBinData[64];

    PXTextFromIntToBinary32U(buffer, 64, dataType);

#if 0
    PXTextPrintA
    (
        buffer,
        256,
        "%s\n"

    );
#endif
}

PXI32U PXAPI PXTypeIntFitting(const PXSize expectedSize)
{
    {
        const PXBool fitIn08Bit = expectedSize <= (PXSize)PXTypeInt08ULimit;

        if (fitIn08Bit)
        {
            return PXTypeInt08U;
        }
    }

    {
        const PXBool fitIn16Bit = expectedSize <= (PXSize)PXTypeInt16ULimit;

        if (fitIn16Bit)
        {
            return PXTypeInt16U;
        }
    }

    {
        const PXBool fitIn32Bit = expectedSize <= (PXSize)PXTypeInt32ULimit;

        if (fitIn32Bit)
        {
            return PXTypeInt32U;
        }
    }

    return PXTypeInt64U;
}

void PXAPI PXEndianSwapI32U(PXI32U PXREF value)
{



#if PX_X86_BSWAP_ALLOW

    PXI32U wurst = *value;

    __asm
    {
        mov EAX, 0x0
        mov EDX, 0x0

        mov EAX, wurst
        mov EDX, [wurst] // Load variable into register

        BSWAP EDX        // Execute byte swap in register 'EAX'
        mov [wurst], EDX
    }

    *value = wurst;

#else

#endif // 0
}

void PXAPI PXEndianSwapI16U(PXI16U PXREF value)
{

#if PX_X86_BSWAP_ALLOW

    __asm
    {
        mov ax, 4
        mov bx, 8
        xchg ax, bx
    }

#else

#endif // 0




}

void PXAPI PXEndianSwap(void PXREF data, const PXSize dataSize, const PXEndian endianFrom, const PXEndian endianTo)
{
    {
        const PXBool inoutIsOutput = endianFrom == endianTo;

        if (inoutIsOutput)
        {
            return;
        }
    }
    const PXSize runntime = dataSize / 2;
    const PXByte* endAdress = (PXByte*)data + (dataSize - 1);

    for (PXSize i = 0; i < runntime; ++i)
    {
        PXByte* a = (PXByte*)data + i;
        PXByte* b = (PXByte*)endAdress - i;

        PXByte c = *a; // Backup a
        *a = *b; // b -> a
        *b = c; // a -> b
    }
}

void PXAPI PXEndianSwapV(void* PXREF data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo)
{
    const PXSize amount = dataSize / elementSize;

    for (PXSize i = 0; i < amount; ++i)
    {
        void PXREF insertPoint = data[i];

        PXEndianSwap(insertPoint, elementSize, endianFrom, endianTo);
    }
}
