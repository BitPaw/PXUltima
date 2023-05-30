#ifndef PXColorINCLUDE
#define PXColorINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXColorFormat_
	{
		PXColorFormatInvalid,

		PXColorFormatAlphaBinary,
		PXColorFormatAlphaByte,

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

		PXColorFormatYCbCrI8
	}
	PXColorFormat;

	PXPublic PXInt8U PXColorFormatBytePerPixel(const PXColorFormat imageDataFormat);
	PXPublic PXInt8U PXColorFormatBitsPerPixel(const PXColorFormat imageDataFormat);

	typedef struct PXColorRGBI8_
	{
		PXInt8U Red;
		PXInt8U Green;
		PXInt8U Blue;
	}
	PXColorRGBI8;

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

	//---[ Public-Functions ]----------------------------------------------

	PXPublic void PXColorRGBToYCbCr(const PXColorRGBI8* const colorRGB, PXColorYCbCrI8* const ColorYCbCr);
	PXPublic void PXColorYCbCrToRGB(const PXColorYCbCrI8* const colorYCbCr, PXColorRGBI8* const colorRGB);

	PXPublic float PXColorRGBToY(const float red, const float green, const float blue);
	PXPublic float PXColorRGBToCb(const float red, const float green, const float blue);
	PXPublic float PXColorRGBToCr(const float red, const float green, const float blue);


#ifdef __cplusplus
}
#endif

#endif