#include "TestSocket.h"

#include <Media/HTTP/PXHTTP.h>
#include <OS/Network/PXServer.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Debug/PXDebug.h>
#include <OS/Network/PXClient.h>

#include <stdio.h>

void OnSocketDataSend(void* owner, const PXSocketDataSendEventData* const pxSocketDataSendEventData)
{

}
void OnSocketDataReceive(void* owner, const PXSocketDataReceivedEventData* const pxSocketDataReceivedEventData)
{
    //PXHTTPRequest pxHTTPRequest;
    //PXFile PXFile;

    //PXFileBufferExternal(&PXFile, message, messageSize);

    //const PXActionResult actionResult = PXHTTPRequestParse(&pxHTTPRequest, &PXFile);

    const char exportBuffer[2048];

    const char buffer[] =
        "HTTP/1.1 200 OK\r\n\r\n"
        "<!DOCTYPE html>"
        "<html lang = \"en\">"
        "<head>"
        "<title>PXUltima - Websocket</title>"
        "<style>"
        "body {font-family: Arial, Helvetica, sans-serif;}"
        "</style>"
        "</head>"
        "<body>"
        "<h1>Test socket site</h1>"
        "<h1>Socket Info</h1>"
        "<p>ID: %i</p>"
        "<h1>Recieved data</h1>"
        "<div>%s</div>"
        "</body>"
        "</html>";

    int exportSize = sprintf
    (
        exportBuffer,
        buffer,
        (int)pxSocketDataReceivedEventData->SocketSending,
        pxSocketDataReceivedEventData->Data
    );
    PXBufferConstruct(&pxSocketDataReceivedEventData->SocketReceiving->BufferOutput, exportBuffer, exportSize, PXBufferTypeStack);
    PXSocketSend(pxSocketDataReceivedEventData->SocketReceiving, pxSocketDataReceivedEventData->SocketSending);

    PXSocketClientRemove(pxSocketDataReceivedEventData->SocketReceiving, pxSocketDataReceivedEventData->SocketSending);
}


void OnClientConnected(const PXSocket* serverSocket, const PXSocket* clientSocket)
{
	//printf("[Server] Client connected.\n");
}
void OnClientDisconnected(const PXSocket* serverSocket, const PXSocket* clientSocket)
{
	//printf("[Server] Client disconnected.\n");
}
void OnClientAcceptFailure(const PXSocket* serverSocket)
{
	//printf("[Server] Client accepted failure.\n");
}


void TestSocket()
{
    while (1)
    {
        TestSocketClientGeneral();
        PXThreadSleep(0, 10);
    }

	TestSocketServerGeneral();
}

void TestSocketClientGeneral()
{
    PXClient client;
    PXClientConstruct(&client);

    PXActionResult connectResult = PXClientConnectToSelf(&client, 25565);

    if (connectResult == PXActionSuccessful)
    {
        printf("[C] Connect OK\n");

        connectResult = PXClientSendData(&client, "Hello my name is bob", 21);

        if (connectResult == PXActionSuccessful)
        {
            printf("[C] Send OK\n");
        }
        else
        {
            printf("[C] Send FAILED\n");
        }
    }
    else
    {
        printf("[C] Connect FAILED\n");
    }

    PXClientDestruct(&client);
}

void TestSocketServerGeneral()
{
	PXServer server;

	PXServerConstruct(&server);

    server.EventList.SocketDataSendCallBack = OnSocketDataSend;
    server.EventList.SocketDataReceiveCallBack = OnSocketDataReceive;
	//server.SocketEventListener.ConnectionLinkedCallback = OnClientConnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientDisconnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientAcceptFailure;

	const PXActionResult startResult = PXServerStart(&server, 25565, PXProtocolModeTCP);


    PXDebug pxDebug;
    PXDebugStackTrace(&pxDebug);

	while (1)
	{
		// wait?
	}
}
