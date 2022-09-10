#ifndef ClientInclude
#define ClientInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#include "CSocket.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct Client_ Client;

	typedef void (*ConnectedToServerEvent)(const Client* client);	

	typedef struct Client_
	{
		CSocket SocketClient;
		CSocket SocketConnectedServer;

		ConnectedToServerEvent ConnectedToServerCallback;
	}
	Client;

	CPublic ActionResult ClientConnectToServer(Client* client, const char* ip, unsigned short port, const void* threadObject, const ThreadFunction threadFunction);

	CPublic ThreadResult CommunicationFunctionAsync(void* ioSocket);

#ifdef __cplusplus
}
#endif

#endif