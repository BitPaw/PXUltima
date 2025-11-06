#pragma once

#ifndef PXColorIncluded
#define PXColorIncluded

#include <PX/Media/PXType.h>

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


    PXColorFormatYCbCrI8,

    PXColorFormatARGBI8,
    PXColorFormatARGBF16,
    PXColorFormatARGBF32,

    PXColorFormatXRGBI8,

    PXColorFormatRGBI5,

    PXColorFormatLuminance,
    PXColorFormatLuminanceAlpha,
    PXColorFormatColorIndex,
    PXColorFormatStencilIndex,
    PXColorFormatDepthComponent
}
PXColorFormat;

typedef struct PXColorRGBI8_
{
    PXI8U Red;
    PXI8U Green;
    PXI8U Blue;
}
PXColorRGBI8;

typedef struct PXColorHSV_
{
    PXF32 Hue;
    PXF32 Saturation;
    PXF32 Value;
}
PXColorHSV;

typedef struct PXColorRGBAI8_
{
    PXI8U Red;
    PXI8U Green;
    PXI8U Blue;
    PXI8U Alpha;
}
PXColorRGBAI8;

typedef struct PXColorRGBI16_
{
    PXI16U Red;
    PXI16U Green;
    PXI16U Blue;
}
PXColorRGBI16;

typedef struct PXColorRGBI32_
{
    PXI32U Red;
    PXI32U Green;
    PXI32U Blue;
}
PXColorRGBI32;

typedef struct PXColorRGBF_
{
    PXF32 Red;
    PXF32 Green;
    PXF32 Blue;
}
PXColorRGBF;

typedef struct PXColorRGBAF_
{
    PXF32 Red;
    PXF32 Green;
    PXF32 Blue;
    PXF32 Alpha;
}
PXColorRGBAF;

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


PXPublic void PXAPI PXColorYUYToRGB(PXColorYUV PXREF pxColorYUV, PXColorRGBI8 PXREF pxColorRGBI8);


PXPublic PXI8U PXAPI PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat);
PXPublic PXI8U PXAPI PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat);

PXPublic void PXAPI PXColorRGBToYCbCr(const PXColorRGBI8 PXREF colorRGB, PXColorYCbCrI8 PXREF ColorYCbCr);
PXPublic void PXAPI PXColorYCbCrToRGB(const PXColorYCbCrI8 PXREF colorYCbCr, PXColorRGBI8 PXREF colorRGB);

PXPublic PXF32 PXAPI PXColorRGBToY(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCb(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCr(const PXF32 red, const PXF32 green, const PXF32 blue);

PXPublic void PXAPI PXColorHSVToRGBAF(PXColorHSV PXREF pxColorHSV, PXColorRGBF PXREF pxColorRGBF);
PXPublic void PXAPI PXColorHSVToRGBAI8(PXColorHSV PXREF pxColorHSV, PXColorRGBAI8 PXREF pxColorRGBAI8);

#endif
