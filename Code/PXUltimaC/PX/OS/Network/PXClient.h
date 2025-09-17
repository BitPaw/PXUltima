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

PXPublic void PXAPI PXClientConstruct(PXClient* const pxClient);
PXPublic void PXAPI PXClientDestruct(PXClient* const pxClient);

PXPublic PXResult PXAPI PXClientSendData(PXClient* const pxClient, const void* const data, const PXSize dataSize);

PXPublic PXResult PXAPI PXClientConnectToSelf(PXClient* const client, const PXI16U port);
PXPublic PXResult PXAPI PXClientConnectToServer(PXClient* const client, const PXText* const ip, const PXI16U port);
PXPublic PXResult PXAPI PXClientDisconnectFromServer(PXClient* const client);

PXPublic PXThreadResult PXOSAPI PXClientCommunicationThread(PXSocket* const pxSocket);

#ifdef __cplusplus
}
#endif

#endif
#endif
