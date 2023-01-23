#ifndef PXClientInclude
#define PXClientInclude

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <Network/PXSocket.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXClient_ PXClient;

	typedef void (*ConnectedToServerEvent)(const PXClient* client);

	typedef struct PXClient_
	{
		PXSocket SocketPXClient;
		PXSocket SocketConnectedServer;

		ConnectedToServerEvent ConnectedToServerCallback;
	}
	PXClient;

	PXPublic PXActionResult PXClientConnectToServer(PXClient* client, const char* ip, unsigned short port, const void* threadObject, const ThreadFunction threadFunction);

	PXPublic PXThreadResult CommunicationFunctionAsync(void* ioSocket);

#ifdef __cplusplus
}
#endif

#endif