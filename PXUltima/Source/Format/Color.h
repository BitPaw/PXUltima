#ifndef ColorInclude
#define ColorInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum ColorFormat_
	{
		ColorFormatInvalid,
		ColorFormatRGB,
		ColorFormatYCbCr
	}
	ColorFormat;

	typedef struct ColorRGB_
	{
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;
	}
	ColorRGB;

	typedef struct ColorYCbCr_
	{
		unsigned char Y;
		unsigned char Cb;
		unsigned char Cr;
	}
	ColorYCbCr;

	//---[ Public-Functions ]----------------------------------------------

	CPublic void ConvertColorRGBToYCbCr(const ColorRGB* const colorRGB, ColorYCbCr* const ColorYCbCr);
	CPublic void ConvertColorYCbCrToRGB(const ColorYCbCr* const colorYCbCr, ColorRGB* const colorRGB);

	CPublic float ConvertColorRGBToY(const float red, const float green, const float blue);
	CPublic float ConvertColorRGBToCb(const float red, const float green, const float blue);
	CPublic float ConvertColorRGBToCr(const float red, const float green, const float blue);


#ifdef __cplusplus
}
#endif

#endif