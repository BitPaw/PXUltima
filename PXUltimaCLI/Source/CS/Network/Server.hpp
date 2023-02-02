#pragma once

#include <Network/PXServer.h>
#include <CS/Error/ActionResult.hpp>

namespace PX
{
	public ref class Server
	{
		private:
		PXServer* _pxServer;

		public:
		delegate void PXDataStreamEventHandler(Object^ sender, unsigned int e);
		event PXDataStreamEventHandler^ E;

		Server()// : _dataSteam
		{
			_pxServer = new PXServer();
			PXServerConstruct(_pxServer);
		}

		~Server()
		{
			PXServerDestruct(_pxServer);
		}

		PX::ActionResult Start(const unsigned short port);
		PX::ActionResult Stop();
		PX::ActionResult ClientKick(const PXSocketID socketID);
	};
}