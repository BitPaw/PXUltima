#pragma once

#include <OS/Network/PXServer.h>
#include <OS/Error/PXActionResult.hpp>
#include "PXSocket.hpp"

namespace PX
{
	class Server : public PXServer
	{
		public:
		PXDLLExport Server();
		PXDLLExport ~Server();

		PXDLLExport PX::ActionResult Start(const PXInt16U port, const ProtocolMode protocolMode);
		PXDLLExport PX::ActionResult Stop();
		PXDLLExport PX::ActionResult KickClient(const PXSocketID socketID);
	};
}