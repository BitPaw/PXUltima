#ifndef PXCPPXMLINCLUDE
#define PXCPPXMLINCLUDE

#include <Media/XML/PXXML.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class XML
	{
		public:
		static PX::ActionResult FileCompile(PX::File& inputStream, PX::File& outputStream);
	};
}

#endif