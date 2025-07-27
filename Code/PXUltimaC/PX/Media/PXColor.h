#ifndef PXColorINCLUDE
#define PXColorINCLUDE

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
    PXInt8U Red;
    PXInt8U Green;
    PXInt8U Blue;
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
    PXInt8U Red;
    PXInt8U Green;
    PXInt8U Blue;
    PXInt8U Alpha;
}
PXColorRGBAI8;

typedef struct PXColorRGBI16_
{
    PXInt16U Red;
    PXInt16U Green;
    PXInt16U Blue;
}
PXColorRGBI16;

typedef struct PXColorRGBI32_
{
    PXInt32U Red;
    PXInt32U Green;
    PXInt32U Blue;
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
    double Red;
    double Green;
    double Blue;
}
PXColorRGBD;

typedef struct PXColorYCbCrI8_
{
    PXInt8U Y;
    PXInt8U Cb;
    PXInt8U Cr;
}
PXColorYCbCrI8;


typedef struct PXColorYUV_
{
    PXInt8U Y;
    PXInt8U U;
    PXInt8U V;
}
PXColorYUV;


PXPublic PXInt32U PXAPI PXColorI32FromBGR(const PXInt8U r, const PXInt8U g, const PXInt8U b);
PXPublic PXInt32U PXAPI PXColorI32FromBGRA(const PXInt8U r, const PXInt8U g, const PXInt8U b, const PXInt8U a);

PXPublic PXInt32U PXAPI PXColorI32FromRGB(const PXInt8U r, const PXInt8U g, const PXInt8U b);
PXPublic PXInt32U PXAPI PXColorI32FromRGBA(const PXInt8U r, const PXInt8U g, const PXInt8U b, const PXInt8U a);


PXPublic void PXAPI PXColorYUYToRGB(PXColorYUV* const pxColorYUV, PXColorRGBI8* const pxColorRGBI8);


PXPublic PXInt8U PXAPI PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat);
PXPublic PXInt8U PXAPI PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat);

PXPublic void PXAPI PXColorRGBToYCbCr(const PXColorRGBI8* const colorRGB, PXColorYCbCrI8* const ColorYCbCr);
PXPublic void PXAPI PXColorYCbCrToRGB(const PXColorYCbCrI8* const colorYCbCr, PXColorRGBI8* const colorRGB);

PXPublic PXF32 PXAPI PXColorRGBToY(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCb(const PXF32 red, const PXF32 green, const PXF32 blue);
PXPublic PXF32 PXAPI PXColorRGBToCr(const PXF32 red, const PXF32 green, const PXF32 blue);

PXPublic void PXAPI PXColorHSVToRGBAF(PXColorHSV* const pxColorHSV, PXColorRGBF* const pxColorRGBF);
PXPublic void PXAPI PXColorHSVToRGBAI8(PXColorHSV* const pxColorHSV, PXColorRGBAI8* const pxColorRGBAI8);

#endif
