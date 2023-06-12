#pragma once

#include <Media/C/PXC.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class C
	{
		public:
		static PX::ActionResult LexicalAnalysis(PX::File& inputStream, PX::File& outputStream);
		static PX::ActionResult Compile(PX::File& inputStream, PX::File& outputStream);
	};
}