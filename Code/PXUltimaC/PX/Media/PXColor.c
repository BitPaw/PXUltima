#include "PXColor.h"

#include <PX/Math/PXMath.h>

PXInt32U PXAPI PXColorI32FromBGR(const PXInt8U r, const PXInt8U g, const PXInt8U b)
{
    return b + (g << 8) | (r << 16);
}

PXInt32U PXAPI PXColorI32FromBGRA(const PXInt8U r, const PXInt8U g, const PXInt8U b, const PXInt8U a)
{
    return b + (g << 8) | (r << 16) | (a << 32);
}

PXInt32U PXAPI PXColorI32FromRGB(const PXInt8U r, const PXInt8U g, const PXInt8U b)
{
    return r | (g << 8) | (b << 16);
}

PXInt32U PXAPI PXColorI32FromRGBA(const PXInt8U r, const PXInt8U g, const PXInt8U b, const PXInt8U a)
{
    return b | (g << 8) | (r << 16) | (a << 32);
}


inline int clip(int val) {
    return (val < 0) ? 0 : (val > 255) ? 255 : val;
}   

void PXAPI PXColorYUYToRGB(PXColorYUV* const pxColorYUV, PXColorRGBI8* const pxColorRGBI8)
{
    int C = pxColorYUV->Y - 16;
    int D = pxColorYUV->U - 128;
    int E = pxColorYUV->V - 128;

    pxColorRGBI8->Red = clip((298 * C + 409 * E + 128) >> 8);
    pxColorRGBI8->Green = clip((298 * C - 100 * D - 208 * E + 128) >> 8);
    pxColorRGBI8->Blue = clip((298 * C + 516 * D + 128) >> 8);
}

PXInt8U PXAPI PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
    default:
    case PXColorFormatInvalid:
        return 0;

    case PXColorFormatAlphaBinary:
    case PXColorFormatA8:
        return 1;

    case PXColorFormatRGBI8:
    case PXColorFormatBGRI8:
        return sizeof(PXInt8U) * 3u;

    case PXColorFormatRGBAI8:
    case PXColorFormatBGRAI8:
        return sizeof(PXInt8U) * 4u;

    case PXColorFormatRGBI16:
    case PXColorFormatBGRI16:
        return sizeof(PXInt16U) * 3u;

    case PXColorFormatRGBAI16:
    case PXColorFormatBGRAI16:
        return sizeof(PXInt16U) * 4u;

    case PXColorFormatRGBI32:
    case PXColorFormatBGRI32:
    case PXColorFormatRGBF:
    case PXColorFormatBGRF:
        return sizeof(PXInt32U) * 3u;

    case PXColorFormatRGBAF:
    case PXColorFormatBGRAF:
    case PXColorFormatRGBAI32:
    case PXColorFormatBGRAI32:
        return sizeof(PXInt32U) * 4u;

    case PXColorFormatRGBD:
    case PXColorFormatBGRD:
        return sizeof(double) * 3u;

    case PXColorFormatRGBAD:
    case PXColorFormatBGRAD:
        return sizeof(double) * 4u;
    }
}

PXInt8U PXAPI PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat)
{
    return PXColorFormatBytePerPixel(imageDataFormat) * 8u;
}

void PXAPI PXColorRGBToYCbCr(const PXColorRGBI8* const colorRGB, PXColorYCbCrI8* const ColorYCbCr)
{
    ColorYCbCr->Y = 0.299 * colorRGB->Red + 0.587 * colorRGB->Green + 0.114 * colorRGB->Blue;
    ColorYCbCr->Cb = -0.1687 * colorRGB->Red - 0.3313 * colorRGB->Green + 0.5 * colorRGB->Blue + 128u;
    ColorYCbCr->Cr = 0.5 * colorRGB->Red - 0.4187 * colorRGB->Green - 0.0813 * colorRGB->Blue + 128u;
}

void PXAPI PXColorYCbCrToRGB(const PXColorYCbCrI8* const colorYCbCr, PXColorRGBI8* const colorRGB)
{
    colorRGB->Red = colorYCbCr->Y + 1.402 * (colorYCbCr->Cr - 128u);
    colorRGB->Green = colorYCbCr->Y - 0.34414 * (colorYCbCr->Cb - 128u) - 0.71414 * (colorYCbCr->Cr - 128u);
    colorRGB->Blue = colorYCbCr->Y + 1.772 * (colorYCbCr->Cb - 128u);
}

PXF32 PXAPI PXColorRGBToY(const PXF32 red, const PXF32 green, const PXF32 blue)
{
    return +0.299f * red + 0.587f * green + 0.114f * blue;
}

PXF32 PXAPI PXColorRGBToCb(const PXF32 red, const PXF32 green, const PXF32 blue)
{
    return -0.16874f * red - 0.33126f * green + 0.5f * blue;
} // ITU: -0.168736f * r -0.331264f * g +0.5f      * b

PXF32 PXAPI PXColorRGBToCr(const PXF32 red, const PXF32 green, const PXF32 blue)
{
    return +0.5f * red - 0.41869f * green - 0.08131f * blue;
} // ITU: +0.5f      * r -0.418688f * g -0.081312f * b


void PXAPI PXColorHSVToRGBAF(PXColorHSV* const pxColorHSV, PXColorRGBF* const pxColorRGBF)
{
    PXF32 baseColor = pxColorHSV->Hue / 60.0f;
    int baseColorInterval = PXMathFloor(baseColor);
    PXF32 f = baseColor - baseColorInterval;

    const PXF32 p = pxColorHSV->Value * (1 - pxColorHSV->Saturation);
    const PXF32 q = pxColorHSV->Value * (1 - pxColorHSV->Saturation * f);
    const PXF32 t = pxColorHSV->Value * (1 - pxColorHSV->Saturation * (1 - f));

    switch(baseColorInterval)
    {
    case 1:
    {
        pxColorRGBF->Red = q;
        pxColorRGBF->Green = pxColorHSV->Value;
        pxColorRGBF->Blue = p;
        break;
    }
    case 2:
    {
        pxColorRGBF->Red = p;
        pxColorRGBF->Green = pxColorHSV->Value;
        pxColorRGBF->Blue = t;
        break;
    }
    case 3:
    {
        pxColorRGBF->Red = p;
        pxColorRGBF->Green = q;
        pxColorRGBF->Blue = pxColorHSV->Value;
        break;
    }
    case 4:
    {
        pxColorRGBF->Red = t;
        pxColorRGBF->Green = p;
        pxColorRGBF->Blue = pxColorHSV->Value;
        break;
    }
    case 5:
    {
        pxColorRGBF->Red = pxColorHSV->Value;
        pxColorRGBF->Green = p;
        pxColorRGBF->Blue = q;
        break;
    }
    default: // 0 or 6
    {
        pxColorRGBF->Red = pxColorHSV->Value;
        pxColorRGBF->Green = t;
        pxColorRGBF->Blue = p;
        break;
    }
    }
}

void PXAPI PXColorHSVToRGBAI8(PXColorHSV* const pxColorHSV, PXColorRGBAI8* const pxColorRGBAI8)
{
    PXColorRGBF pxColorRGBF;

    PXColorHSVToRGBAF(pxColorHSV, &pxColorRGBF);

    pxColorRGBAI8->Red = pxColorRGBF.Red * 0xFF;
    pxColorRGBAI8->Green = pxColorRGBF.Green * 0xFF;
    pxColorRGBAI8->Blue = pxColorRGBF.Blue * 0xFF;
}
