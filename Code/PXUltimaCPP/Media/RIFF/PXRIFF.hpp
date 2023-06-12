#ifndef PXCPPRIFFINCLUDE
#define PXCPPRIFFINCLUDE

#include <Media/RIFF/PXRIFF.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	// Resource Interchange File Format. 
	class RIFF : public PXRIFF
	{
		public:
		PX::ActionResult Parse(PX::File& file);
		PX::ActionResult Serialize(PX::File& file);
	};
}

#endif