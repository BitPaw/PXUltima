#pragma once

#include <Media/PXColor.h>

namespace PX
{
	template<typename Number>
	class ColorRGB
	{
		Number Red;
		Number Green;
		Number Blue;
	};

	template<typename Number>
	class ColorRGBA
	{
		Number Red;
		Number Green;
		Number Blue;
		Number Alpha;
	};
	
	template<typename Number>
	class PXColorYCbCr
	{
		Number Y;
		Number Cb;
		Number Cr;
	};
}