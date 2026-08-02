#pragma once

#ifndef PXBufferDEFINED
#define PXBufferDEFINED

#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXVector.h>
#include <PX/Type/PXColor.h>

// Container to store data.
// Basically just an array with combined length
typedef struct PXBuffer_
{
    union
    {
        void* Address;

        PXByte* Data; // Contains the whole data.
        char* TextA;
        const char* TextAConst;
        wchar_t* TextW;
        
        PXI8U* I8U;
        PXI16U* I16U;
        PXI32U* I32U;
        PXI64U* I64U;

        PXF32* F32;
        PXF64* F64;

        PXColorRGBI8* ColorRGBI8;

        //PXVector2F16* F16V2;
        PXVector2F32* F32V2;
       // PXVector2F64* F64V2;

        PXVector3F16* F16V3;
        PXVector3F32* F32V3;
        PXVector3F64* F64V3;
    };
   
    PXSize CursorOffsetByte;
    PXSize CursorOffsetBit;

    // Size allowed or used by buffer
    PXSize SizeAllowedToUse;

    // The size of the data pace in which you can move 
    // without triggering an invalid access.
    PXSize SizeAllocated;

    PXBool IsOwned;
}
PXBuffer;

PXPublic PXBool PXAPI PXBufferIsInBounce(const PXBuffer PXREF pxBuffer);
PXPublic PXBool PXAPI PXBufferIsInBounceWithOffset(const PXBuffer PXREF pxBuffer, const PXSize offset);
PXPublic PXByte* PXAPI PXBufferData(const PXBuffer PXREF pxBuffer);
PXPublic PXByte* PXAPI PXBufferDataWithOffset(const PXBuffer PXREF pxBuffer, const PXSize offset);
PXPublic PXSize PXAPI PXBufferRemaining(const PXBuffer PXREF pxBuffer);

PXPublic PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferEnsureTotal(PXBuffer PXREF pxBuffer, const PXSize sizeTotal);
PXPublic PXResult PXAPI PXBufferEnsureAdditional(PXBuffer PXREF pxBuffer, const PXSize sizeAdditionalData);
PXPublic PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer);
PXPublic PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size);
PXPublic PXResult PXAPI PXBufferCopy(PXBuffer PXREF pxBuffer, void* data, const PXSize size);
PXPublic PXResult PXAPI PXBufferAppend(PXBuffer PXREF pxBuffer, void* data, const PXSize size, void** out);

PXPublic PXResult PXAPI PXBufferStore
(
    PXBuffer PXREF pxBuffer,
    const void* data,
    const PXSize sizeTotal,
    const PXBool isDataConst
);

#endif