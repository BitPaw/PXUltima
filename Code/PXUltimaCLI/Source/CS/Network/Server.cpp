#include "Server.hpp"

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