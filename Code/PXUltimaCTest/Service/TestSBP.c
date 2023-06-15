#include "TestSBP.h"

#include <stdio.h>

#include <Service/SBP/PXSBP.h>

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

	PXSBPReceiverEventList pxSBPReceiverEventList;
	pxSBPReceiverEventList.OnMessageUpdatedCallBack = TestPXSBPOnMessageUpdated;
	pxSBPReceiverEventList.OnMessageReceivedCallBack = TestPXSBPOnMessageReceived;
	pxSBPReceiverEventList.OnChunkSegmentUpdatedCallBack = PXNull;
	pxSBPReceiverEventList.OnChunkReceivedCallBack = PXNull;
	pxSBPReceiverEventList.OnMessageInvalidCallBack = PXNull;

	PXSBPServerReceiverEventListSet(&server, &pxSBPReceiverEventList);

	PXServerStart(&server.Server, 13370, ProtocolModeTCP);

	PXText ip;
	PXTextMakeFixedA(&ip, "127.0.0.1");

	PXSBPClientConnectToServer(&client, &ip, 13370);


	char hello[] = "Hello, this is a message";
	
	PXSBPClientSendMessage(&client, hello, sizeof(hello));


	while (1)
	{

	}
}