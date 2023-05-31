#include "PXServer.hpp"

PX::ActionResult PX::Server::Start(const PXInt16U port, const ProtocolMode protocolMode)
{
	return (PX::ActionResult)PXServerStart(this, port, protocolMode);
}

PX::ActionResult PX::Server::Stop()
{
	return (PX::ActionResult)PXServerStop(this);
}

PX::ActionResult PX::Server::KickClient(const PXSocketID socketID)
{
	return (PX::ActionResult)PXServerKickClient(this, socketID);
}