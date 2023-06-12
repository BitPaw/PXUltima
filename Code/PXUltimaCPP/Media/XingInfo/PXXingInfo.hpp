#ifndef PXCPPXingInfoINCLUDE
#define PXCPPXingInfoINCLUDE

#include <Media/XingInfo/PXXingInfo.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class XingInfo : public PXXingInfo
	{
		public:
		PX::ActionResult Parse(PX::File& file);
	};
}

#endif