#include "PXColor.h"

PXInt8U PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat)
{
	switch (imageDataFormat)
	{
		default:
		case PXColorFormatInvalid:
			return 0;

		case PXColorFormatAlphaBinary:
		case PXColorFormatAlphaByte:
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

PXInt8U PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat)
{
	return PXColorFormatBytePerPixel(imageDataFormat) * 8u;
}

void PXColorRGBToYCbCr(const PXColorRGBI8* const colorRGB, PXColorYCbCrI8* const ColorYCbCr)
{
	ColorYCbCr->Y = 0.299 * colorRGB->Red + 0.587 * colorRGB->Green + 0.114 * colorRGB->Blue;
	ColorYCbCr->Cb = -0.1687 * colorRGB->Red - 0.3313 * colorRGB->Green + 0.5 * colorRGB->Blue + 128u;
	ColorYCbCr->Cr = 0.5 * colorRGB->Red - 0.4187 * colorRGB->Green - 0.0813 * colorRGB->Blue + 128u;
}

void PXColorYCbCrToRGB(const PXColorYCbCrI8* const colorYCbCr, PXColorRGBI8* const colorRGB)
{
	colorRGB->Red = colorYCbCr->Y + 1.402 * (colorYCbCr->Cr - 128u);
	colorRGB->Green = colorYCbCr->Y - 0.34414 * (colorYCbCr->Cb - 128u) - 0.71414 * (colorYCbCr->Cr - 128u);
	colorRGB->Blue = colorYCbCr->Y + 1.772 * (colorYCbCr->Cb - 128u);
}

float PXColorRGBToY(const float red, const float green, const float blue)
{
	return +0.299f * red + 0.587f * green + 0.114f * blue;
}

float PXColorRGBToCb(const float red, const float green, const float blue)
{
	return -0.16874f * red - 0.33126f * green + 0.5f * blue;
} // ITU: -0.168736f * r -0.331264f * g +0.5f      * b

float PXColorRGBToCr(const float red, const float green, const float blue)
{
	return +0.5f * red - 0.41869f * green - 0.08131f * blue;
} // ITU: +0.5f      * r -0.418688f * g -0.081312f * b
