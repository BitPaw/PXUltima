#define PXClientInclude
#ifndef PXClientInclude
#define PXClientInclude

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Network/PXSocket.h>

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

	PXPublic PXActionResult PXAPI PXClientSendData(PXClient* const pxClient, const void* const data, const PXSize dataSize);

	PXPublic PXActionResult PXAPI PXClientConnectToSelf(PXClient* const client, const PXInt16U port);
	PXPublic PXActionResult PXAPI PXClientConnectToServer(PXClient* const client, const PXText* const ip, const PXInt16U port);
	PXPublic PXActionResult PXAPI PXClientDisconnectFromServer(PXClient* const client);

	PXPublic PXThreadResult PXOSAPI PXClientCommunicationThread(PXSocket* const pxSocket);

#ifdef __cplusplus
}
#endif

#endif
#endif