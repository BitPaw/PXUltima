#include "PXSocket.hpp"

PX::Socket::Socket()
{
}

PX::Socket::~Socket()
{
}

PX::ActionResult PX::Socket::Create(const IPAdressFamily adressFamily, const PXSocketType socketType, const ProtocolMode protocolMode)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Connect()
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::SetupAdress(const PXSize PXSocketListSizeMax, PXSize* PXSocketListSize, PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo, const PXSize pxSocketAdressSetupInfoSize)
{
    return PX::ActionResult();
}

bool PX::Socket::IsCurrentlyUsed(PXSocket* const pxSocket)
{
    return PXBool();
}

void PX::Socket::Close(PXSocket* const pxSocket)
{
}

PX::ActionResult PX::Socket::EventPull(void* const buffer, const PXSize bufferSize)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Bind()
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::OptionsSet()
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Listen()
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Accept()
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::SendAsServerToClient(const PXSocketID clientID, const void* inputBuffer, const PXSize inputBufferSize)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Send(const void* inputBuffer, const PXSize inputBufferSize, PXSize* inputBytesWritten)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::Receive(const void* outputBuffer, const PXSize outputBufferSize, PXSize* outputBytesWritten)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::ReceiveAsServer(const PXSocketID clientID)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Socket::ClientRemove(const PXSocketID clientID)
{
    return PX::ActionResult();
}
