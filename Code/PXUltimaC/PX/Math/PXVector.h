#ifndef PXVectorINCLUDE
#define PXVectorINCLUDE

#include <PX/Media/PXType.h>

typedef struct PXVector2I8S_
{
    union
    {
        struct
        {
            PXInt8S X;
            PXInt8S Y;
        };

        PXInt8S Data[2];
    };
}
PXVector2I8S;

typedef struct PXVector2I16S_
{
    union
    {
        struct
        {
            PXInt16S X;
            PXInt16S Y;
        };

        PXInt16S Data[2];
    };
}
PXVector2I16S;

typedef struct PXVector2I16U_
{
    union
    {
        struct
        {
            PXInt16U X;
            PXInt16U Y;
        };

        PXInt16U Data[2];
    };
}
PXVector2I16U;

typedef struct PXVector2I32S_
{
    union
    {
        struct
        {
            PXInt32S X;
            PXInt32S Y;
        };

        PXInt32S Data[2];
    };
}
PXVector2I32S;

typedef struct PXVector2I32U_
{
    union
    {
        struct
        {
            PXInt32U X;
            PXInt32U Y;
        };

        PXInt32U Data[2];
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
    };
}
PXVector4F32;



















PXPublic void PXAPI PXVector2F32Set(PXVector2F32* const pxVector3F32, const PXVector2F32* const vectorSet);
PXPublic void PXAPI PXVector2F32SetXY(PXVector2F32* const pxVector3F32, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Set(PXVector3F32* const pxVector3F32, const PXVector3F32* const pxVector3F32Source);
PXPublic void PXAPI PXVector3F32SetXYZ(PXVector3F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4F32SetXYZW(PXVector4F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);

PXPublic void PXAPI PXVector2F32Add(PXVector2F32* const vector, const PXVector2F32* const pxVectorData);
PXPublic void PXAPI PXVector2F32AddXY(PXVector2F32* const vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Add(PXVector3F32* const pxVector3F32, const PXVector3F32* const vectorB);
PXPublic void PXAPI PXVector3F32AddXYZ(PXVector3F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4F32AddXYZW(const PXVector4F32* const vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w, PXVector4F32* const vectorResult);

PXPublic void PXAPI PXVector3F32Substract(PXVector3F32* const pxVector3F32, const PXVector3F32* const pxVector3F32Input);

PXPublic void PXAPI PXVector2F32Multiply(PXVector2F32* const vector, const PXVector2F32* const pxVectorData);
PXPublic void PXAPI PXVector2F32MultiplyXY(PXVector2F32* const vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3F32Multiply(PXVector3F32* const pxVector3F32, PXVector3F32* const vectorResult);
PXPublic void PXAPI PXVector3F32MultiplyXYZ(PXVector3F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector3F32MultiplyS(PXVector3F32* const pxVector3F32, const PXF32 scalar);
PXPublic void PXAPI PXVector4F32MultiplyXYZW(PXVector4F32* const PXVector4F32, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);
 
PXPublic void PXAPI PXVector3F32Divide(PXVector3F32* const pxVector3F32, const PXVector3F32* const vectorB);
PXPublic void PXAPI PXVector3F32DivideS(PXVector3F32* const pxVector3F32, const PXF32 scalar);

PXPublic PXF32 PXAPI PXVector2F32DistanceTo(PXVector2F32* const vectorA, PXVector2F32* const vectorB);
PXPublic PXF32 PXAPI PXVector2F32DotProduct(const PXVector2F32* const vectorA, const PXVector2F32* const vectorB);

PXPublic void PXAPI PXVector3F32Absolute(PXVector3F32* const pxVector3F32);

PXPublic void PXAPI PXVector3F32Normalize(PXVector3F32* const pxVector3F32);
PXPublic PXF32 PXAPI PXVector3F32Length(const PXVector3F32* const vector);

PXPublic void PXAPI PXVector3F32CrossProduct(PXVector3F32* const vectorResult, const PXVector3F32* const vectorA, const PXVector3F32* const vectorB);
PXPublic PXF32 PXAPI PXVector3F32DotProduct(const PXVector3F32* const vectorA, const PXVector3F32* const vectorB);
PXPublic void PXAPI PXVector3F32Interpolate(PXVector3F32* const vectorA, const PXVector3F32* const vectorB, PXF32 speed);

#endif
