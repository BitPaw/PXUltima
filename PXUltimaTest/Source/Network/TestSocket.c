#include "TestSocket.h"

#include <Network/PXServer.h>
#include <Memory/PXMemory.h>
#include <Format/HTTP/HTTP.h>

#include <stdio.h>

void OnSocketDataSend(const PXSocket* const pxSocket, const void* message, const PXSize messageSize)
{

}
void OnSocketDataReceive(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
    //PXHTTPRequest pxHTTPRequest;
    //PXDataStream pxDataStream;

    //PXDataStreamFromExternal(&pxDataStream, message, messageSize);

    //const PXActionResult actionResult = PXHTTPRequestParse(&pxHTTPRequest, &pxDataStream);

    const char exportBuffer[1024];

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
        "<p>%s</p>"
        "</body>"
        "</html>";
    PXSize size = 0;

    int exportSize = sprintf
    (
        exportBuffer,
        buffer,
        pxSocket->ID,
        message
    );

    PXSocketSend(pxSocket, exportBuffer, exportSize, &size);

    PXSocketClose(pxSocket);
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
	TestSocketServerGeneral();
}

void TestSocketServerGeneral()
{
	PXServer server;

	PXServerConstruct(&server);

    server.SocketEventListener.MessageSendCallback = OnSocketDataSend;
    server.SocketEventListener.MessageReceiveCallback = OnSocketDataReceive;
	//server.SocketEventListener.ConnectionLinkedCallback = OnClientConnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientDisconnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientAcceptFailure;

	const PXActionResult startResult = PXServerStart(&server, 12345, ProtocolModeTCP);

	while (1)
	{
		// wait?
	}
}
