#pragma once

#include <Media/ADLER/PXAdler32.h>

namespace PX
{
	class Adler32
	{
		static PXSize Check(const void* const data, const PXSize length);
		static PXSize Create(const PXSize adler, const unsigned char* data, PXSize length);
	};
}