#include "PXSBP.hpp"

PX::SBPClient::SBPClient()
{
	PXSBPClientConstruct(this);
}

PX::SBPClient::~SBPClient()
{
	PXSBPClientDestruct(this);
}

PX::ActionResult PX::SBPClient::ConnectToServer(PX::Text& ip, const PXInt16U port)
{
	return (PX::ActionResult)PXSBPClientConnectToServer(this, &ip, port);
}

PX::ActionResult PX::SBPClient::DisconnectFromServer()
{
	return (PX::ActionResult)PXSBPClientDisconnectFromServer(this);
}

PX::SBPServer::SBPServer()
{
	PXSBPServerConstruct(this);
}

PX::SBPServer::~SBPServer()
{
	PXSBPServerDestruct(this);
}

PX::ActionResult PX::SBPServer::Start(const PXInt16U port)
{
	return (PX::ActionResult)PXSBPServerStart(this, port);
}

PX::ActionResult PX::SBPServer::Stop()
{
	return (PX::ActionResult)PXSBPServerStop(this);
}