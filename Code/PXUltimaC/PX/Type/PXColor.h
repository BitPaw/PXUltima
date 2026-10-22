#pragma once

#ifndef PXColorIncluded
#define PXColorIncluded

#include <PX/Type/PXType.h>

typedef enum PXColorFormat_
{
    PXColorFormatInvalid,

    PXColorFormatAlphaBinary,

    // Single channel
    PXColorFormatR8,
    PXColorFormatG8,
    PXColorFormatB8,
    PXColorFormatA8,

    PXColorFormatR16,
    PXColorFormatG16,
    PXColorFormatB16,
    PXColorFormatA16,

    PXColorFormatR32,
    PXColorFormatG32,
    PXColorFormatB32,
    PXColorFormatA32,

    PXColorFormatRF,
    PXColorFormatGF,
    PXColorFormatBF,
    PXColorFormatAF,

    PXColorFormatRD,
    PXColorFormatGD,
    PXColorFormatBD,
    PXColorFormatAD,



    // RGB
    PXColorFormatRGBI8,
    PXColorFormatRGBI16,
    PXColorFormatRGBI32,
    PXColorFormatRGBF,
    PXColorFormatRGBD,

    // BGR
    PXColorFormatBGRI8,
    PXColorFormatBGRI16,
    PXColorFormatBGRI32,
    PXColorFormatBGRF,
    PXColorFormatBGRD,

    // RGBA
    PXColorFormatRGBAI8,
    PXColorFormatRGBAI16,
    PXColorFormatRGBAI32,
    PXColorFormatRGBAF,
    PXColorFormatRGBAD,

    // BGRA
    PXColorFormatBGRAI8,
    PXColorFormatBGRAI16,
    PXColorFormatBGRAI32,
    PXColorFormatBGRAF,
    PXColorFormatBGRAD,

    // Direct Draw Surface extras
    PXColorFormatDXI1,
    PXColorFormatDXI3,
    PXColorFormatDXI5,


    PXColorFormatYCbCrI8,

    PXColorFormatARGBI8,
    PXColorFormatARGBF16,
    PXColorFormatARGBF32,

    PXColorFormatXRGBI8,

    PXColorFormatRGBI5,

    // Color index
    PXColorFormatColorIndex_x1,
    PXColorFormatColorIndex_x4,
    PXColorFormatColorIndex_x8,
    PXColorFormatColorIndex_x16,

    PXColorFormatLuminance,
    PXColorFormatLuminanceAlpha,

    PXColorFormatStencilIndex,
    PXColorFormatDepthComponent
}
PXColorFormat;


//_________________________________________________________
// RGBA
//---------------------------------------------------------
typedef struct PXColorRGBA_U8_
{
    union
    {
        PXI8U Data[4];

        struct
        {
            PXI8U Red;
            PXI8U Green;
            PXI8U Blue;
            PXI8U Alpha;
        };
    };
}
PXColorRGBA_U8;

typedef struct PXColorRGBA_U16_
{
    union
    {
        PXI16U Data[4];

        struct
        {
            PXI16U Red;
            PXI16U Green;
            PXI16U Blue;
            PXI16U Alpha;
        };
    };
}
PXColorRGBA_U16;

typedef struct PXColorRGBA_F32_
{
    union
    {
        PXF32 Data[4];

        struct
        {
            PXF32 Red;
            PXF32 Green;
            PXF32 Blue;
            PXF32 Alpha;
        };
    };
}
PXColorRGBA_F32;

typedef struct PXColorRGBA_F64_
{
    union
    {
        struct
        {
            PXF64 Red;
            PXF64 Green;
            PXF64 Blue;
            PXF64 Alpha;
        };

        PXF64 Data[4];
    };
}
PXColorRGBA_F64;
//---------------------------------------------------------


//_________________________________________________________
// HSV
//---------------------------------------------------------
typedef struct PXColorHSV_
{
    union
    {
        PXF32 Data[3];

        struct
        {
            PXF32 Hue;
            PXF32 Saturation;
            PXF32 Value;
        };
    };
}
PXColorHSV;
//---------------------------------------------------------


//_________________________________________________________
// RGB
//---------------------------------------------------------
typedef struct PXColorRGB_U8_
{
    union
    {
        PXI8U Data[3];

        struct
        {
            PXI8U Red;
            PXI8U Green;
            PXI8U Blue;
        };
    };
}
PXColorRGB_U8;

typedef struct PXColorRGB_U16_
{
    union
    {
        PXI16U Data[3];

        struct
        {
            PXI16U Red;
            PXI16U Green;
            PXI16U Blue;
        };
    };
}
PXColorRGB_U16_;

typedef struct PXColorRGB_U32_
{
    union
    {
        PXI32U Data[3];

        struct
        {
            PXI32U Red;
            PXI32U Green;
            PXI32U Blue;
        };
    };
}
PXColorRGB_U32;

typedef struct PXColorRGB_F32_
{
    union
    {
        PXF32 Data[3];

        struct
        {
            PXF32 Red;
            PXF32 Green;
            PXF32 Blue;
        };
    };
}
PXColorRGB_F32;

typedef struct PXColorRGB_F64_
{
    union
    {
        PXF64 Data[3];

        struct
        {
            PXF64 Red;
            PXF64 Green;
            PXF64 Blue;
        };
    };
}
PXColorRGB_F64;
//---------------------------------------------------------




PXPublic PXBool PXAPI PXColorRGBFIsBlack(const PXColorRGB_F32 PXREF pxColorRGBF);



typedef struct PXColorRGBD_
{
    PXF64 Red;
    PXF64 Green;
    PXF64 Blue;
}
PXColorRGBD;

typedef struct PXColorYCbCrI8_
{
    PXI8U Y;
    PXI8U Cb;
    PXI8U Cr;
}
PXColorYCbCrI8;


typedef struct PXColorYUV_
{
    PXI8U Y;
    PXI8U U;
    PXI8U V;
}
PXColorYUV;


PXPublic PXI32U PXAPI PXColorI32FromBGR(const PXI8U r, const PXI8U g, const PXI8U b);
PXPublic PXI32U PXAPI PXColorI32FromBGRA(const PXI8U r, const PXI8U g, const PXI8U b, const PXI8U a);

PXPublic PXI32U PXAPI PXColorI32FromRGB(const PXI8U r, const PXI8U g, const PXI8U b);
PXPublic PXI32U PXAPI PXColorI32FromRGBA(const PXI8U r, const PXI8U g, const PXI8U b, const PXI8U a);


PXPublic inline int clip(int val);

PXPublic void PXAPI PXColorYUYToRGB(PXColorYUV PXREF pxColorYUV, PXColorRGB_U8 PXREF PXColorRGB_U8);


PXPublic PXI8U PXAPI PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat);
PXPublic PXI8U PXAPI PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat);

PXPublic void PXAPI PXColorRGBToYCbCr(const PXColorRGB_U8 PXREF colorRGB, PXColorYCbCrI8 PXREF ColorYCbCr);
PXPublic void PXAPI PXColorYCbCrToRGB(const PXColorYCbCrI8 PXREF colorYCbCr, PXColorRGB_U8 PXREF colorRGB);

PXPublic PXF32 PXAPI PXColorRGBToY(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCb(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCr(const PXF32 red, const PXF32 green, const PXF32 blue);

PXPublic void PXAPI PXColorHSVToRGBF(PXColorHSV PXREF pxColorHSV, PXColorRGB_F32 PXREF pxColorRGBF);
PXPublic void PXAPI PXColorHSVToRGBAF(PXColorHSV PXREF pxColorHSV, PXColorRGBA_F32 PXREF pxColorRGBF);
PXPublic void PXAPI PXColorHSVToRGBAI8(PXColorHSV PXREF pxColorHSV, PXColorRGBA_U8 PXREF PXColorRGBA_U8);

#endif
