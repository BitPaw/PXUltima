#pragma once

#ifndef PXTriangleIncluded
#define PXTriangleIncluded

#include "PXVector.h"

typedef struct PXTriangleF16_
{
    union
    {
        struct
        {
            PXVector3F16 A;
            PXVector3F16 B;
            PXVector3F16 C;
        };

        PXVector3F16 Data[3];
    };
}
PXTriangleF16;

typedef struct PXTriangleF32_
{
    union
    {
        struct
        {
            PXVector3F32 A;
            PXVector3F32 B;
            PXVector3F32 C;
        };

        PXVector3F32 Data[3];
    };
}
PXTriangleF32;

typedef struct PXTriangleF64_
{
    union
    {
        struct
        {
            PXVector3F64 A;
            PXVector3F64 B;
            PXVector3F64 C;
        };

        PXVector3F64 Data[3];
    };
}
PXTriangleF64;


PXPublic void PXAPI PXTriangleCentroidF32(const PXTriangleF32 PXREF pxTriangle, PXVector3F32 PXREF pxVector3);

#endif