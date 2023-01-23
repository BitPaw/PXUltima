#include "TestSocket.h"

#include <Network/PXServer.h>
#include <Memory/PXMemory.h>

#include <stdio.h>

void OnClientConnected(const PXSocket* serverSocket, const PXSocket* clientSocket)
{
	printf("[Server] Client connected.\n");
}
void OnClientDisconnected(const PXSocket* serverSocket, const PXSocket* clientSocket)
{
	printf("[Server] Client disconnected.\n");
}
void OnClientAcceptFailure(const PXSocket* serverSocket)
{
	printf("[Server] Client accepted failure.\n");
}


void TestSocket()
{
	TestSocketServerGeneral();
}

void TestSocketServerGeneral()
{
	PXServer server;

	PXServerConstruct(&server);

	server.PXClientConnectedCallback = OnClientConnected;
	server.PXClientDisconnectedCallback = OnClientDisconnected;
	server.PXClientAcceptFailureCallback = OnClientAcceptFailure;
	server.ServerSocketListSize = 1;
	server.ServerSocketList = MemoryAllocateClear(sizeof(PXSocket) * server.ServerSocketListSize);
	server.PXClientSocketListSize = 5;
	server.PXClientSocketList = MemoryAllocateClear(sizeof(PXSocket) * server.PXClientSocketListSize);

	const PXActionResult startResult = PXServerStart(&server, 25565, ProtocolModeTCP);

	while (1)
	{
		// wait?
	}
}