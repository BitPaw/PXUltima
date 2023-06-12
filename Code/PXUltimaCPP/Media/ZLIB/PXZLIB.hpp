#ifndef PXCPPZLIBINCLUDE
#define PXCPPZLIBINCLUDE

#include <Media/ZLIB/PXZLIB.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class ZLIB : public PXZLIB
	{
		static PX::ActionResult Decompress(PX::File& pxInputSteam, PX::File& pxOutputSteam);
		static PX::ActionResult Compress(PX::File& pxInputSteam, PX::File& pxOutputSteam);
	};
}

#endif