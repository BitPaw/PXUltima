#include "PXClient.h"
#include <PX/OS/Error/PXResult.hpp>
#include "PXText.hpp"
namespace PX
{
    class Client
    {
        private:
        PXClient* _pxClientHandle;

        public:

        Client() { PXClientConstruct(_pxClientHandle); }
        ~Client() { PXClientDestruct(_pxClientHandle); }
        PX::Result SendData(const void PXREF data, const PXSize dataSize) { return (PX::Result)PXClientSendData(_pxClientHandle, data, dataSize); }
        PX::Result ConnectToSelf(const PXI16U port) { return (PX::Result)PXClientConnectToSelf(_pxClientHandle, port); }
        PX::Result ConnectToServer(const PX::Text& ip, const PXI16U port) { return (PX::Result)PXClientConnectToServer(_pxClientHandle, &ip, port); }
        PX::Result DisconnectFromServer() { return (PX::Result)PXClientDisconnectFromServer(_pxClientHandle); }
    };
}