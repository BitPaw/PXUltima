#include "TestSBP.h"

#include <stdio.h>

#include <Network/SBP/PXSBPProtocol.h>

void TestPXSBPOnMessageUpdated(const PXSBPMessage* const pxSBPMessage)
{
	printf("Message iupdated %i Bytes\n", pxSBPMessage->MessageSize);

}
void TestPXSBPOnMessageReceived(const PXSBPMessage* const pxSBPMessage)
{
	printf("Message Received %i Bytes\n", pxSBPMessage->MessageSize);
}

void TestSBPOnChunkReceived(const PXSBPChunk* const pxSBPChunk)
{
	printf("chunk Received %i Bytes\n", pxSBPChunk->DataSize);
}

void TestSBPAll()
{
	printf("[#] Starting test for SBP\n...");

	TestSBPClientServerResponse();

	printf("[i] Testing dinished\n");
}

void TestSBPClientServerResponse()
{
	PXSBPServer server; PXSBPServerConstruct(&server);
	PXSBPClient client; PXSBPClientConstruct(&client);


	server.Receiver.OnMessageUpdatedCallBack = TestPXSBPOnMessageUpdated;
	server.Receiver.OnMessageReceivedCallBack = TestPXSBPOnMessageReceived;	
	server.Receiver.OnChunkReceivedCallBack = TestSBPOnChunkReceived;
	

	PXServerStart(&server.Server, 13370, ProtocolModeTCP);


	PXClientConnectToServer(&client.Client, "127.0.0.1", 13370, &client.Client, CommunicationFunctionAsync);


	char hello[] = "Hello, this is a message";
	
	PXSBPClientSendMessage(&client, hello, sizeof(hello));


	while (1)
	{

	}
}