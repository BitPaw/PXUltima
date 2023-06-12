#pragma once

#include <Media/HTTP/PXHTTP.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class HTTP
	{
		public:	
		static PX::ActionResult RequestParse(PXHTTPRequest& pxHTTPRequest, PX::File& PXFile);
		static PX::ActionResult RequestSerialize(PXHTTPRequest& pxHTTPRequest, PX::File& PXFile);

		static PX::ActionResult ResponseParse(PXHTTPResponse& pxHTTPResponse, PX::File& PXFile);
		static PX::ActionResult ResponseSerialize(PXHTTPResponse& pxHTTPResponse, PX::File& PXFile);
	};
}