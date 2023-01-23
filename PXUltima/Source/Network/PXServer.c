#include "PXServer.h"

#include <Event/Event.h>
#include <Memory/PXMemory.h>

#if SocketDebug
#include <stdio.h>
#endif

void PXServerConstruct(PXServer* const server)
{    
    MemoryClear(server, sizeof(PXServer));
}

void PXServerDestruct(PXServer* const server)
{

}

PXActionResult PXServerStart(PXServer* const server, const unsigned short port, const ProtocolMode protocolMode)
{
    PXSize adressInfoListSize = 0;

    // Setup adress info
    {
        const PXActionResult adressResult = PXSocketSetupAdress
        (
            server->ServerSocketList,
            -1,
            &server->ServerSocketListSize,
            0, // IP
            port,
            IPAdressFamilyINET,
            PXSocketTypeStream,
            protocolMode
        );

        PXActionExitOnError(adressResult);
    }

    for(PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &server->ServerSocketList[i];

        //pxSocket->SocketCreatingCallback = ;
        //pxSocket->SocketCreatedCallback;
        //pxSocket->MessageSendCallback;
        //pxSocket->MessageReceiveCallback;
        //pxSocket->ConnectionListeningCallback;
        //pxSocket->ConnectionLinkedCallback;
        //pxSocket->ConnectionEstablishedCallback;
        //pxSocket->ConnectionTerminatedCallback;

        // Create socket
        {
            const PXActionResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);

            PXActionExitOnError(socketCreateResult);
        }

        // Set Socket Options
        {
            const PXActionResult actionResult = PXSocketOptionsSet(pxSocket);

            PXActionExitOnError(actionResult);
        }

        // Bind Socket
        {
            const PXActionResult actionResult = PXSocketBind(pxSocket);

            PXActionExitOnError(actionResult);
        }

        // Listen
        {
            const PXActionResult actionResult = PXSocketListen(pxSocket);

            PXActionExitOnError(actionResult);
        }

        InvokeEvent(pxSocket->ConnectionListeningCallback, pxSocket);

        const PXActionResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, PXServerPXClientListeningThread, server);

    }

    return PXActionSuccessful;
}

PXActionResult PXServerStop(PXServer* const server)
{
	return PXActionInvalid;
}

PXActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID)
{
	return PXActionInvalid;
}

PXSocket* PXServerGetPXClientViaID(PXServer* const server, const PXSocketID socketID)
{
    for(PXSize i = 0; i < server->PXClientSocketListSize; i++)
    {
        PXSocket* const clientSocket = &server->PXClientSocketList[i];
        const PXSocketID clientSocketID = clientSocket->ID;
        const PXBool foundTarget = clientSocketID == socketID;

        if(foundTarget)
        {
            return clientSocket;
        }
    }

    return PXNull;
}

void PXServerRegisterPXClient(PXServer* const server, PXClient* const client)
{
    //_clientListLock.Lock();
   // PXClient* indexedPXClient = GetNextPXClient();
    //_clientListLock.Release();

    //indexedPXClient->EventCallBackSocket = clientSocket->EventCallBackSocket;
    //indexedPXClient->AdressInfo = clientSocket->AdressInfo;

  //  NumberOfConnectedPXClients++;

   // if(EventCallBackServer)
   // {
   //     EventCallBackServer->OnPXClientConnected(*indexedPXClient);
   // }

    //indexedPXClient->CommunicationThread = ThreadRun(PXClient::CommunicationFunctionAsync, indexedPXClient);
}

PXActionResult PXServerSendMessageToAll(PXServer* server, const unsigned char* data, const PXSize dataSize)
{
    for(PXSize i = 0; i < server->PXClientSocketListSize; ++i)
    {
        PXSocket* serverSocket = &server->PXClientSocketList[i];
        PXSize writtenBytes = 0;
        const PXActionResult actionResult = PXSocketSend(serverSocket, data, dataSize, &writtenBytes);
    }
}

PXActionResult PXServerSendMessageToPXClient(PXServer* server, const PXSocketID clientID, const unsigned char* data, const PXSize dataSize)
{
    PXSocket* const clientSocket = PXServerGetPXClientViaID(server, clientID);

    if(!clientSocket)
    {
        return NoPXClientWithThisID;
    }

    PXSize writtenBytes = 0;
    const PXActionResult actionResult = PXSocketSend(clientSocket, data, dataSize, &writtenBytes);

    return actionResult;
}

PXThreadResult PXServerPXClientListeningThread(void* serverAdress)
{
    PXServer* server = serverAdress;
    PXSocket* serverSocket = &server->ServerSocketList[0];

    // Seek Socket
    {
        /* What does this do ??
        ThreadID threadID = PXThreadCurrentGet();

        for(PXSize i = 0; i < server->ServerSocketListSize; ++i)
        {
            PXSocket* serverSocket = &server->ServerSocketList[i];

            if(serverSocket->CommunicationThread == threadID)
            {
                serverSocket = serverSocket;
                break;
            }
        }*/
    }

    if(!serverSocket)
    {
        return PXThreadSucessful;
    }

    while(PXSocketIsCurrentlyUsed(serverSocket))
    {
        PXSocket clientSocket;

        PXSocketConstruct(&clientSocket);

        // Set Events

        const PXActionResult actionResult = PXSocketAccept(serverSocket, &clientSocket);
        const PXBool successful = PXActionSuccessful == actionResult;

        if(!successful)
        {
            InvokeEvent(server->PXClientAcceptFailureCallback, serverSocket);

            continue; // failed.. retry?
        }

#if SocketDebug
        printf("[i][Server] New client accepted <%zi>\n", clientSocket.ID);
#endif

        InvokeEvent(server->PXClientConnectedCallback, serverSocket, &clientSocket);

        PXServerRegisterPXClient(server, &clientSocket);
    }

    return PXThreadSucessful;
}
