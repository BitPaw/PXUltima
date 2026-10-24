#pragma once

#ifndef PXVectorIncluded
#define PXVectorIncluded

#include <PX/Type/PXType.h>

typedef struct PXVector2_I8U_
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
PXVector2_I8U;

typedef struct PXVector2_I8S_
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
PXVector2_I8S;

typedef struct PXVector2_I16S_
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
PXVector2_I16S;

typedef struct PXVector2_I16U_
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
PXVector2_I16U;

typedef struct PXVector2_I32S_
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
PXVector2_I32S;

typedef struct PXVector2_I32U_
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
PXVector2_I32U;

typedef struct PXVector2_F32_
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
PXVector2_F32;
//---------------------------------------------------------


//---------------------------------------------------------
// Vector 3D
//---------------------------------------------------------
typedef struct PXVector3_I8U_
{
    union
    {
        struct
        {
            PXI8U X;
            PXI8U Y;
            PXI8U Z;
        };

        PXI8U Data[3];
    };
}
PXVector3_I8U;

typedef struct PXVector3_F16_
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
PXVector3_F16;

typedef struct PXVector3_F32_
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
PXVector3_F32;

typedef struct PXVector3_F64_
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
PXVector3_F64;
//---------------------------------------------------------



//---------------------------------------------------------
typedef struct PXVector4_I64U_
{
    union
    {
        struct
        {
            PXI64U X;
            PXI64U Y;
            PXI64U Z;
            PXI64U W;
        };

        PXI64U Data[4];
    };
}
PXVector4_I64U;


typedef struct PXVector4_I64S_
{
    union
    {
        struct
        {
            PXI64S X;
            PXI64S Y;
            PXI64S Z;
            PXI64S W;
        };

        PXI64S Data[4];
    };
}
PXVector4_I64S;

typedef struct PXVector4_F32_
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

        PXVector3_F32 V3;
    };
}
PXVector4_F32;

// SET
PXPublic void PXAPI PXVector2Set_F32(PXVector2_F32 PXREF pxVector3, const PXVector2_F32 PXREF vectorSet);
PXPublic void PXAPI PXVector2SetXY_F32(PXVector2_F32 PXREF pxVector3, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3Set_F32(PXVector3_F32 PXREF pxVector3, const PXVector3_F32 PXREF pxVector3Source);
PXPublic void PXAPI PXVector3SetXYZ_F32(PXVector3_F32 PXREF pxVector3, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4Set_F32(PXVector4_F32 PXREF pxVector4, const PXVector4_F32 PXREF pxVector3Source);
PXPublic void PXAPI PXVector4SetXYZW_F32(PXVector4_F32 PXREF pxVector3, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);

// ADD
PXPublic void PXAPI PXVector2Add_F32(PXVector2_F32 PXREF vector2, const PXVector2_F32 PXREF pxVectorData);
PXPublic void PXAPI PXVector2AddXY_F32(PXVector2_F32 PXREF vector2, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3Add_F32(PXVector3_F32 PXREF pxVector3, const PXVector3_F32 PXREF vectorB);
PXPublic void PXAPI PXVector3AddS_F32(PXVector3_F32 PXREF pxVector3, const PXF32 scalar);
PXPublic void PXAPI PXVector3AddXYZ_F32(PXVector3_F32 PXREF pxVector3, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4AddXYZW_F32(const PXVector4_F32 PXREF vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w, PXVector4_F32 PXREF vectorResult);

// SUB
PXPublic void PXAPI PXVector3Sub_F32(PXVector3_F32 PXREF pxVector3, const PXVector3_F32 PXREF pxVector3Input);
PXPublic void PXAPI PXVector4Sub_F32(PXVector4_F32 PXREF pxVector4, const PXVector4_F32 PXREF pxVector4Input);

// MUL
PXPublic void PXAPI PXVector2Mul_F32(PXVector2_F32 PXREF vector, const PXVector2_F32 PXREF pxVectorData);
PXPublic void PXAPI PXVector2MulXY_F32(PXVector2_F32 PXREF vector, const PXF32 x, const PXF32 y);

PXPublic void PXAPI PXVector3Mul_F32(PXVector3_F32 PXREF pxVector3, PXVector3_F32 PXREF vectorResult);
PXPublic void PXAPI PXVector3MulXYZ_F32(PXVector3_F32 PXREF pxVector3, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector3MulScalar_F32(PXVector3_F32 PXREF pxVector3, const PXF32 scalar);

PXPublic void PXAPI PXVector4MulXYZW_F32(PXVector4_F32 PXREF PXVector4, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);
PXPublic void PXAPI PXVector4MulScalar_F32(PXVector4_F32 PXREF PXVector4, const PXF32 scalar);

// DIV
PXPublic void PXAPI PXVector3Div_F32(PXVector3_F32 PXREF pxVector3, const PXVector3_F32 PXREF vectorB);
PXPublic void PXAPI PXVector3DivScalar_F32(PXVector3_F32 PXREF pxVector3, const PXF32 scalar);



PXPublic PXF32 PXAPI PXVector3LengthSquared_F32(const PXVector3_F32 PXREF pxVector3); // lsq = x*x + y*y + z*z
PXPublic PXF32 PXAPI PXVector4LengthSquared_F32(const PXVector4_F32 PXREF pxVector4); // lsq = x*x + y*y + z*z + w*w
PXPublic PXF32 PXAPI PXVector3Length_F32(const PXVector3_F32 PXREF pxVector3); // l = sqrt(x*x + y*y + z*z)
PXPublic PXF32 PXAPI PXVector4Length_F32(const PXVector4_F32 PXREF pxVector4); // l = sqrt(x*x + y*y + z*z + w*w)

PXPublic void PXAPI PXVector3Zero_F32(PXVector3_F32 PXREF pxVector3);
PXPublic void PXAPI PXVector4Zero_F32(PXVector4_F32 PXREF pxVector4);




PXPublic PXF32 PXAPI PXVector2DistanceTo_F32(PXVector2_F32 PXREF vectorA, PXVector2_F32 PXREF vectorB);
PXPublic PXF32 PXAPI PXVector2DotProduct_F32(const PXVector2_F32 PXREF vectorA, const PXVector2_F32 PXREF vectorB);

PXPublic void PXAPI PXVector3Absolute_F32(PXVector3_F32 PXREF pxVector3);

PXPublic void PXAPI PXVector3Normalize_F32(PXVector3_F32 PXREF pxVector3);

PXPublic void PXAPI PXVector3CrossProduct_F32(PXVector3_F32 PXREF vectorResult, const PXVector3_F32 PXREF vectorA, const PXVector3_F32 PXREF vectorB);


PXPublic void PXAPI PXVector3CrossProductScaled_F32
(
    PXVector3_F32 PXREF vectorResult,
    const PXVector3_F32 PXREF r, 
    const PXVector3_F32 PXREF p,
    const PXF32 scalar
);


// x = x1*x2 + y1*y2 + z1*z2 
PXPublic PXF32 PXAPI PXVector3DotProduct_F32(const PXVector3_F32 PXREF vectorA, const PXVector3_F32 PXREF vectorB);

PXPublic void PXAPI PXVector3Interpolate_F32(PXVector3_F32 PXREF vectorA, const PXVector3_F32 PXREF vectorB, PXF32 speed);

PXPublic void PXAPI PXVector4Demote_F32(const PXVector4_F32 PXREF pxVectorIn, PXVector3_F32 PXREF pxVectorOut, const PXF32 distance);
PXPublic void PXAPI PXVector3Demote_F32(const PXVector3_F32 PXREF pxVectorIn, PXVector2_F32 PXREF pxVectorOut, const PXF32 distance);


PXPublic void PXAPI PXVector3Normal_F32
(
    PXVector3_F32 PXREF res,
    const PXVector3_F32 PXREF a,
    const PXVector3_F32 PXREF b,
    const PXVector3_F32 PXREF c
);

PXPublic void PXAPI PXVector4Normal_F32
(
    PXVector3_F32 PXREF res,
    const PXVector4_F32 PXREF a,
    const PXVector4_F32 PXREF b,
    const PXVector4_F32 PXREF c,
    const PXF32 distance
);



#endif