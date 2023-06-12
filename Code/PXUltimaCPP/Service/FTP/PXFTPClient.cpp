#include "PXFTPClient.hpp"

PX::ActionResult PX::FTPClient::ConnectToServer(const PX::Text& ip, const PXInt16U port)
{
    return (PX::ActionResult)PXFTPClientConnectToServer(this, &ip, port);;
}