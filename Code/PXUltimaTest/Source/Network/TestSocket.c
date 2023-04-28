#include "TestSocket.h"

#include <Media/HTTP/HTTP.h>

#include <OS/Network/PXServer.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Debug/PXDebug.h>

#include <stdio.h>

void OnSocketDataSend(const PXSocket* const sendingSocket, const PXSocketID clientSocketID, const void* const message, const PXSize messageSize)
{

}
void OnSocketDataReceive(const PXSocket* const receiveSocket, const PXSocketID clientSocketID, const void* const message, const PXSize messageSize)
{
    //PXHTTPRequest pxHTTPRequest;
    //PXFile PXFile;

    //PXFileBufferExternal(&PXFile, message, messageSize);

    //const PXActionResult actionResult = PXHTTPRequestParse(&pxHTTPRequest, &PXFile);

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

    int exportSize = sprintf
    (
        exportBuffer,
        buffer,
        clientSocketID,
        message
    );

    PXSocketSendAsServerToClient(receiveSocket, clientSocketID, exportBuffer, exportSize);
    PXSocketClientRemove(receiveSocket, clientSocketID);
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

    server.EventListener.MessageSendCallback = OnSocketDataSend;
    server.EventListener.MessageReceiveCallback = OnSocketDataReceive;
	//server.SocketEventListener.ConnectionLinkedCallback = OnClientConnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientDisconnected;
	//server.SocketEventListener.ConnectionTerminatedCallback = OnClientAcceptFailure;

	const PXActionResult startResult = PXServerStart(&server, 25565, ProtocolModeTCP);


    PXDebug pxDebug;
    PXDebugStackTrace(&pxDebug);

	while (1)
	{
		// wait?
	}
}
