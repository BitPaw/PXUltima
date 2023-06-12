#pragma once

#include <Service/SBP/PXSBP.h>
#include <Media/PXText.hpp>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class SBPClient : public PXSBPClient
	{
		public:
		SBPClient();
		~SBPClient();

		PX::ActionResult ConnectToServer(PX::Text& ip, const PXInt16U port);
		PX::ActionResult DisconnectFromServer();
	};

	class SBPServer : public PXSBPServer
	{
		public:
		SBPServer();
		~SBPServer();

		PX::ActionResult Start(const PXInt16U port);
		PX::ActionResult Stop();
	};
}