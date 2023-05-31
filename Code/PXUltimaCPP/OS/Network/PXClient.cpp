#include "PXClient.hpp"

PX::Client::Client()
{
}

PX::Client::~Client()
{
}

PX::ActionResult PX::Client::SendData(const void* const data, const PXSize dataSize)
{
	return PX::ActionResult();
}

PX::ActionResult PX::Client::ConnectToServer(const PX::Text& const ip, const PXInt16U port, const void* threadObject, const ThreadFunction threadFunction)
{
	return PX::ActionResult();
}

PX::ActionResult PX::Client::DisconnectFromServer()
{
	return PX::ActionResult();
}
