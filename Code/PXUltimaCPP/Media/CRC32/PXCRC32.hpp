#pragma once

#include <Media/CRC32/PXCRC32.h>

namespace PX
{
	class CRC32
	{
		public:	
		static PXInt32U Generate(const PXByte* const data, const PXSize length);
	};
}