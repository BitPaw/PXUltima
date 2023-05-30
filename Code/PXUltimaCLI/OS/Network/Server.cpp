#include "Server.hpp"

PX::Server::Server()
{
	_pxServer = new PXServer();
	PXServerConstruct(_pxServer);

	OnSocketDataRawReceiveManaged = gcnew SocketDataRawReceiveEvent(this, &Server::OnSocketDataRawReceive);
	GCHandle gch = GCHandle::Alloc(OnSocketDataRawReceiveManaged);
	IntPtr ip = Marshal::GetFunctionPointerForDelegate(OnSocketDataRawReceiveManaged);
	PXSocketDataReceiveEvent cb = static_cast<PXSocketDataReceiveEvent>(ip.ToPointer());


	//_pxServer->SocketEventListener.MessageSendCallback = OnSocketDataRawSendManaged;
	_pxServer->EventListener.MessageReceiveCallback = cb;
}

PX::Server::~Server()
{
	PXServerDestruct(_pxServer);
	delete _pxServer;
}

PX::ActionResult PX::Server::Start(const unsigned short port)
{
    return (PX::ActionResult)PXServerStart(_pxServer, port, ProtocolModeTCP);
}

PX::ActionResult PX::Server::Stop()
{
    return (PX::ActionResult)PXServerStop(_pxServer);
}

PX::ActionResult PX::Server::ClientKick(const PXSocketID socketID)
{
    return (PX::ActionResult)PXActionInvalid;
}