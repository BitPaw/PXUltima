#ifndef PXColorINCLUDE
#define PXColorINCLUDE

#include <Media/PXType.h>

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
	float Red;
	float Green;
	float Blue;
}
PXColorRGBF;

typedef struct PXColorRGBAF_
{
	float Red;
	float Green;
	float Blue;
	float Alpha;
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


PXPublic PXInt8U PXAPI PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat);
PXPublic PXInt8U PXAPI PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat);

PXPublic void PXAPI PXColorRGBToYCbCr(const PXColorRGBI8* const colorRGB, PXColorYCbCrI8* const ColorYCbCr);
PXPublic void PXAPI PXColorYCbCrToRGB(const PXColorYCbCrI8* const colorYCbCr, PXColorRGBI8* const colorRGB);

PXPublic float PXAPI PXColorRGBToY(const float red, const float green, const float blue);
PXPublic float PXAPI PXColorRGBToCb(const float red, const float green, const float blue);
PXPublic float PXAPI PXColorRGBToCr(const float red, const float green, const float blue);

#endif