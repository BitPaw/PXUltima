#define PXClientIncluded
#ifndef PXClientIncluded
#define PXClientIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Network/PXSocket.h>

#if PXSocketUSE

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct PXClient_ PXClient;

// 584 Byte
typedef struct PXClient_
{
    PXSocketEventList EventList;
    PXSocket SocketClient;
    PXSocket SocketServer;
    void* Owner;
}
PXClient;

PXPublic void PXAPI PXClientConstruct(PXClient PXREF pxClient);
PXPublic void PXAPI PXClientDestruct(PXClient PXREF pxClient);

PXPublic PXResult PXAPI PXClientSendData(PXClient PXREF pxClient, const void PXREF data, const PXSize dataSize);

PXPublic PXResult PXAPI PXClientConnectToSelf(PXClient PXREF client, const PXI16U port);
PXPublic PXResult PXAPI PXClientConnectToServer(PXClient PXREF client, const PXText PXREF ip, const PXI16U port);
PXPublic PXResult PXAPI PXClientDisconnectFromServer(PXClient PXREF client);

PXPublic PXThreadResult PXOSAPI PXClientCommunicationThread(PXSocket PXREF pxSocket);

#ifdef __cplusplus
}
#endif

#endif
#endif
