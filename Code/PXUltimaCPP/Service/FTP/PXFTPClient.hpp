#pragma once

#include <Service/FTP/PXFTPClient.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	class FTPClient : PXFTPClient
	{
		public:
		PX::ActionResult ConnectToServer(const PX::Text& ip, const PXInt16U port);
	};
}