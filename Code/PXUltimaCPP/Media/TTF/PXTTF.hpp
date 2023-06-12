#ifndef PXCPPPXTTFINCLUDE
#define PXCPPPXTTFINCLUDE

#include <Media/TTF/PXTTF.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	// Tag Image File Format
	class TTF : public PXTTF
	{
        public:
        TTF();
        ~TTF();

		PX::ActionResult Parse(PX::File& const file);
	};
}

#endif