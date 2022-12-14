#include "Color.h"

void ConvertColorRGBToYCbCr(const ColorRGB* const colorRGB, ColorYCbCr* const ColorYCbCr)
{
	ColorYCbCr->Y = 0.299 * colorRGB->Red + 0.587 * colorRGB->Green + 0.114 * colorRGB->Blue;
	ColorYCbCr->Cb = -0.1687 * colorRGB->Red - 0.3313 * colorRGB->Green + 0.5 * colorRGB->Blue + 128u;
	ColorYCbCr->Cr = 0.5 * colorRGB->Red - 0.4187 * colorRGB->Green - 0.0813 * colorRGB->Blue + 128u;
}

void ConvertColorYCbCrToRGB(const ColorYCbCr* const colorYCbCr, ColorRGB* const colorRGB)
{
	colorRGB->Red = colorYCbCr->Y + 1.402 * (colorYCbCr->Cr - 128u);
	colorRGB->Green = colorYCbCr->Y - 0.34414 * (colorYCbCr->Cb - 128u) - 0.71414 * (colorYCbCr->Cr - 128u);
	colorRGB->Blue = colorYCbCr->Y + 1.772 * (colorYCbCr->Cb - 128u);
}

float ConvertColorRGBToY(const float red, const float green, const float blue)
{
	return +0.299f * red + 0.587f * green + 0.114f * blue;
}

float ConvertColorRGBToCb(const float red, const float green, const float blue)
{
	return -0.16874f * red - 0.33126f * green + 0.5f * blue;
} // ITU: -0.168736f * r -0.331264f * g +0.5f      * b

float ConvertColorRGBToCr(const float red, const float green, const float blue)
{
	return +0.5f * red - 0.41869f * green - 0.08131f * blue;
} // ITU: +0.5f      * r -0.418688f * g -0.081312f * b
