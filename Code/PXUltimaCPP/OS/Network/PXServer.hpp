#ifndef PXCPPServerINCLUDE
#define PXCPPServerINCLUDE

#include <OS/Network/PXServer.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class Server : public PXServer
	{
		public:
		Server();
		~Server();

		PX::ActionResult Start(const PXInt16U port, const ProtocolMode protocolMode);
		PX::ActionResult Stop();
		PX::ActionResult KickClient(const PXSocketID socketID);
	};
}

#endif