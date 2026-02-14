#pragma once

#ifndef PXVectorIncluded
#define PXVectorIncluded

#include <PX/Media/PXType.h>

typedef struct PXVec2I8U_
{
    union
    {
        struct
        {
            PXI8U X;
            PXI8U Y;
        };

        PXI8U Data[2];
    };
}
PXVec2I8U;

typedef struct PXVector2I8S_
{
    union
    {
        struct
        {
            PXI8S X;
            PXI8S Y;
        };

        PXI8S Data[2];
    };
}
PXVector2I8S;

typedef struct PXVector2I16S_
{
    union
    {
        struct
        {
            PXI16S X;
            PXI16S Y;
        };

        PXI16S Data[2];
    };
}
PXVector2I16S;

typedef struct PXVector2I16U_
{
    union
    {
        struct
        {
            PXI16U X;
            PXI16U Y;
        };

        PXI16U Data[2];
    };
}
PXVector2I16U;

typedef struct PXVector2I32S_
{
    union
    {
        struct
        {
            PXI32S X;
            PXI32S Y;
        };

        PXI32S Data[2];
    };
}
PXVector2I32S;

typedef struct PXVector2I32U_
{
    union
    {
        struct
        {
            PXI32U X;
            PXI32U Y;
        };

        PXI32U Data[2];
    };
}
PXVector2I32U;

typedef struct PXVector2F32_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
        };

        PXF32 Data[2];
    };
}
PXVector2F32;










//---------------------------------------------------------
// Vector 3D
//---------------------------------------------------------
typedef struct PXVector3F16_
{
    union
    {
        struct
        {
            PXF16 X;
            PXF16 Y;
            PXF16 Z;
        };

        PXF16 Data[3];
    };
}
PXVector3F16;

typedef struct PXVector3F32_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
        };

        PXF32 Data[3];
    };
}
PXVector3F32;

typedef struct PXVector3F64_
{
    union
    {
        struct
        {
            PXF64 X;
            PXF64 Y;
            PXF64 Z;
        };

        PXF64 Data[3];
    };
}
PXVector3F64;





//---------------------------------------------------------





















typedef struct PXVector4F32_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
            PXF32 W;
        };

        PXF32 Data[4];

        PXVector3F32 V3;
    };
}
PXVector4F32;



















PXPublic void PXAPI PXVector2F32Set(PXVector2F32 PXREF pxVector3F32, const PXVector2F32 PXREF vectorSet);
PXPublic void PXAPI PXVector2F32SetXY(PXVector2F32 PXREF pxVector3F32, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Set(PXVector3F32 PXREF pxVector3F32, const PXVector3F32 PXREF pxVector3F32Source);
PXPublic void PXAPI PXVector3F32SetXYZ(PXVector3F32 PXREF pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4F32SetXYZW(PXVector4F32 PXREF pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);

PXPublic void PXAPI PXVector2F32Add(PXVector2F32 PXREF vector, const PXVector2F32 PXREF pxVectorData);
PXPublic void PXAPI PXVector2F32AddXY(PXVector2F32 PXREF vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Add(PXVector3F32 PXREF pxVector3F32, const PXVector3F32 PXREF vectorB);
PXPublic void PXAPI PXVector3F32AddXYZ(PXVector3F32 PXREF pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4F32AddXYZW(const PXVector4F32 PXREF vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w, PXVector4F32 PXREF vectorResult);

PXPublic void PXAPI PXVector3F32Substract(PXVector3F32 PXREF pxVector3F32, const PXVector3F32 PXREF pxVector3F32Input);

PXPublic void PXAPI PXVector2F32Multiply(PXVector2F32 PXREF vector, const PXVector2F32 PXREF pxVectorData);
PXPublic void PXAPI PXVector2F32MultiplyXY(PXVector2F32 PXREF vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Multiply(PXVector3F32 PXREF pxVector3F32, PXVector3F32 PXREF vectorResult);
PXPublic void PXAPI PXVector3F32MultiplyXYZ(PXVector3F32 PXREF pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector3F32MultiplyS(PXVector3F32 PXREF pxVector3F32, const PXF32 scalar);
PXPublic void PXAPI PXVector4F32MultiplyXYZW(PXVector4F32 PXREF PXVector4F32, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);
 
PXPublic void PXAPI PXVector3F32Divide(PXVector3F32 PXREF pxVector3F32, const PXVector3F32 PXREF vectorB);
PXPublic void PXAPI PXVector3F32DivideS(PXVector3F32 PXREF pxVector3F32, const PXF32 scalar);

PXPublic PXF32 PXAPI PXVector2F32DistanceTo(PXVector2F32 PXREF vectorA, PXVector2F32 PXREF vectorB);
PXPublic PXF32 PXAPI PXVector2F32DotProduct(const PXVector2F32 PXREF vectorA, const PXVector2F32 PXREF vectorB);

PXPublic void PXAPI PXVector3F32Absolute(PXVector3F32 PXREF pxVector3F32);

PXPublic void PXAPI PXVector3F32Normalize(PXVector3F32 PXREF pxVector3F32);
PXPublic PXF32 PXAPI PXVector3F32Length(const PXVector3F32 PXREF vector);

PXPublic void PXAPI PXVector3F32CrossProduct(PXVector3F32 PXREF vectorResult, const PXVector3F32 PXREF vectorA, const PXVector3F32 PXREF vectorB);
PXPublic PXF32 PXAPI PXVector3F32DotProduct(const PXVector3F32 PXREF vectorA, const PXVector3F32 PXREF vectorB);
PXPublic void PXAPI PXVector3F32Interpolate(PXVector3F32 PXREF vectorA, const PXVector3F32 PXREF vectorB, PXF32 speed);

PXPublic void PXAPI PXVector4Demote(PXVector4F32 PXREF pxVectorIn, PXVector3F32* pxVectorOut, const PXF32 distance);
PXPublic void PXAPI PXVector3Demote(PXVector3F32 PXREF pxVectorIn, PXVector2F32* pxVectorOut, const PXF32 distance);


PXPublic void PXAPI PXVector3FNormal
(
    PXVector3F32 PXREF res,
    const PXVector3F32 PXREF a,
    const PXVector3F32 PXREF b,
    const PXVector3F32 PXREF c
);

PXPublic void PXAPI PXVector4FNormal
(
    PXVector3F32 PXREF res,
    const PXVector4F32 PXREF a,
    const PXVector4F32 PXREF b,
    const PXVector4F32 PXREF c,
    const PXF32 distance
);

#endif
