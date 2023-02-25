#include "TestSBP.h"

#include <stdio.h>

#include <Network/SBP/SBPClient.h>
#include <Network/SBP/SBPServer.h>

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

	SBPServerConstruct(&server);
	PXSBPClientConstruct(&client);

	SBPServerStart(&server, 13370);

	SBPPXClientConnectToServer(&client, "127.0.0.1", 13370);

	while (1)
	{

	}
}