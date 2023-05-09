#include "TestSBP.h"

#include <stdio.h>

#include <Network/SBP/PXSBPProtocol.h>

void TestSBPAll()
{
	printf("[#] Starting test for SBP\n...");

	TestSBPClientServerResponse();

	printf("[i] Testing dinished\n");
}

void TestSBPClientServerResponse()
{
	PXSBPServer server;
	PXSBPClient client;

	PXSBPServerConstruct(&server);
	PXSBPClientConstruct(&client);

	PXServerStart(&server.Server, 13370, ProtocolModeTCP);


	PXClientConnectToServer(&client.Client, "127.0.0.1", 13370, &client.Client, CommunicationFunctionAsync);


	char hello[] = "Hello, this is a message";
	
	PXSBPClientSendMessage(&client, hello, sizeof(hello));


	while (1)
	{

	}
}