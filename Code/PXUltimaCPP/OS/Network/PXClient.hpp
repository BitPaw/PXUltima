#ifndef PXCPPClientInclude
#define PXCPPClientInclude

#include <OS/Network/PXClient.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	class Client : public PXClient
	{
		public:
		PXDLLExport Client();
		PXDLLExport ~Client();

		PXDLLExport PX::ActionResult SendData(const void* const data, const PXSize dataSize);

		PXDLLExport PX::ActionResult ConnectToServer(const PX::Text& const ip, const PXInt16U port, const void* threadObject, const ThreadFunction threadFunction);
		PXDLLExport PX::ActionResult DisconnectFromServer();
	};
}

#endif