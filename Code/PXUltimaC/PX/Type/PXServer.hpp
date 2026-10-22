#include "PXServer.h"
#include <PX/OS/Error/PXResult.hpp>
namespace PX
{
    class Server
    {
        private:
        PXServer* _pxServer;

        public:
        Server() { PXServerConstruct(_pxServer); }
        ~Server() { PXServerDestruct(_pxServer); }
        PX::Result Start(const PXI16U port, const PXProtocolMode protocolMode) { return (PX::Result)PXServerStart(_pxServer, port, protocolMode); }
        PX::Result Stop() { return (PX::Result)PXServerStop(_pxServer); }
        PX::Result KickClient(const PXSocketID socketID) { return (PX::Result)PXServerKickClient(_pxServer, socketID); }
        PX::Result SendToAll(const void PXREF data, const PXSize dataSize) { return (PX::Result)PXServerSendToAll(_pxServer, data, dataSize); }
    };
}