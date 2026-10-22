#ifndef PXClientIncluded
#define PXClientIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Network/PXSocket.h>

typedef struct PXClient_
{
    PXECSInfo Info;

    PXSocket* Server;

    PXSocketEventList EventList;
    PXSocket SocketClient;
    PXSocket SocketServer;
    void* Owner;
}
PXClient;

typedef struct PXClientCreateInfo_
{
    PXECSCreateInfo Info;
}
PXClientCreateInfo;

PXPublic void PXAPI PXClientConstruct(PXClient PXREF pxClient);
PXPublic void PXAPI PXClientDestruct(PXClient PXREF pxClient);

PXPublic PXResult PXAPI PXClientSendData(PXClient PXREF pxClient, const void PXREF data, const PXSize dataSize);

PXPublic PXResult PXAPI PXClientConnectToSelf(PXClient PXREF client, const PXI16U port);
PXPublic PXResult PXAPI PXClientConnectToServer(PXClient PXREF client, const PXText PXREF ip, const PXI16U port);
PXPublic PXResult PXAPI PXClientDisconnectFromServer(PXClient PXREF client);

#endif