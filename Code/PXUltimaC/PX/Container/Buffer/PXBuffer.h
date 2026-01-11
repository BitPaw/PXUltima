#pragma once

#ifndef PXBufferDEFINED
#define PXBufferDEFINED

#include <PX/OS/Error/PXResult.h>

// Container to store data.
// Basically just an array with combined length
typedef struct PXBuffer_
{
    union
    {
        void* Adress;
        PXByte* Data; // Contains the whole data.
        char* TextA;
        wchar_t* TextW;
    };
   
    PXSize CursorOffsetByte;
    PXSize CursorOffsetBit;

    // Size allowed or used by buffer
    PXSize SizeAllowedToUse;

    // The size of the data pace in which you can move 
    // without triggering an invalid access.
    PXSize SizeAllocated;
}
PXBuffer;

PXPublic PXResult PXAPI PXBufferAllocate(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferEnsure(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferResize(PXBuffer PXREF pxBuffer, const PXSize size);
PXPublic PXResult PXAPI PXBufferRelese(PXBuffer PXREF pxBuffer);
PXPublic PXResult PXAPI PXBufferSet(PXBuffer PXREF pxBuffer, void* data, const PXSize size);
PXPublic PXResult PXAPI PXBufferCopy(PXBuffer PXREF pxBuffer, void* data, const PXSize size);

#endif