#include "Server.h"

#include <Event/Event.h>
#include <Memory/PXMemory.h>

#if SocketDebug
#include <stdio.h>
#endif

void ServerConstruct(CServer* const server)
{
    MemorySet(server, sizeof(CServer), 0);
}

void ServerDestruct(CServer* const server)
{

}

ActionResult ServerStart(CServer* const server, const unsigned short port, const ProtocolMode protocolMode)
{
    size_t adressInfoListSize = 0;

    // Setup adress info
    {
        const ActionResult adressResult = CSocketSetupAdress
        (
            server->ServerSocketList,
            -1,
            &server->ServerSocketListSize,
            0, // IP
            port,
            IPAdressFamilyUnspecified,
            CSocketTypeStream,
            protocolMode
        );
        const unsigned char adressSetupSucessful = ActionSuccessful == adressResult;

        if(!adressSetupSucessful)
        {
            return adressResult;
        }
    }

    for(size_t i = 0; i < server->ServerSocketListSize; ++i)
    {
        CSocket* cSocket = &server->ServerSocketList[i];

        //cSocket->SocketCreatingCallback = ;
        //cSocket->SocketCreatedCallback;
        //cSocket->MessageSendCallback;
        //cSocket->MessageReceiveCallback;
        //cSocket->ConnectionListeningCallback;
        //cSocket->ConnectionLinkedCallback;
        //cSocket->ConnectionEstablishedCallback;
        //cSocket->ConnectionTerminatedCallback;

        // Create socket
        {
            const ActionResult socketCreateResult = CSocketCreate(&cSocket, cSocket->Family, cSocket->Type, cSocket->Protocol);
            const unsigned char sucessful = ActionSuccessful == socketCreateResult;

            if(!sucessful)
            {
                return socketCreateResult;
            }
        }

        // Set Socket Options
        {
            const ActionResult actionResult = CSocketOptionsSet(&cSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        // Bind Socket
        {
            const ActionResult actionResult = CSocketBind(&cSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        // Listen
        {
            const ActionResult actionResult = CSocketListen(&cSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        InvokeEvent(cSocket->ConnectionListeningCallback, cSocket);

        cSocket->CommunicationThread = ThreadRun(ServerClientListeningThread, cSocket);
    }

    return ActionSuccessful;
}

ActionResult ServerStop(CServer* const server)
{
	return ActionInvalid;
}

ActionResult ServerKickClient(CServer* server, const CSocketID socketID)
{
	return ActionInvalid;
}

CSocket* ServerGetClientViaID(CServer* server, const CSocketID socketID)
{
    for(size_t i = 0; i < server->ClientSocketListSize; i++)
    {
        const CSocket* clientSocket = &server->ClientSocketList[i];
        const CSocketID clientSocketID = clientSocket->ID;
        const unsigned char foundTarget = clientSocketID == socketID;

        if(foundTarget)
        {
            return clientSocket;
        }
    }

    return 0;
}

void ServerRegisterClient(CServer* server, Client* client)
{
    //_clientListLock.Lock();
   // Client* indexedClient = GetNextClient();
    //_clientListLock.Release();

    //indexedClient->EventCallBackSocket = clientSocket->EventCallBackSocket;
    //indexedClient->AdressInfo = clientSocket->AdressInfo;

  //  NumberOfConnectedClients++;

   // if(EventCallBackServer)
   // {
   //     EventCallBackServer->OnClientConnected(*indexedClient);
   // }

    //indexedClient->CommunicationThread = ThreadRun(Client::CommunicationFunctionAsync, indexedClient);
}

ActionResult ServerSendMessageToAll(CServer* server, const unsigned char* data, const size_t dataSize)
{
    for(size_t i = 0; i < server->ClientSocketListSize; ++i)
    {
        CSocket* serverSocket = &server->ClientSocketList[i];
        size_t writtenBytes = 0;
        const ActionResult actionResult = CSocketSend(serverSocket, data, dataSize, &writtenBytes);
    }
}

ActionResult ServerSendMessageToClient(CServer* server, const CSocketID clientID, const unsigned char* data, const size_t dataSize)
{
    CSocket* clientSocket = ServerGetClientViaID(server, clientID);

    if(!clientSocket)
    {
        return NoClientWithThisID;
    }

    size_t writtenBytes = 0;
    const ActionResult actionResult = CSocketSend(clientSocket, data, dataSize, &writtenBytes);

    return actionResult;
}

ThreadResult ServerClientListeningThread(void* serverAdress)
{
    CServer* server = serverAdress;
    CSocket* serverSocket = 0;

    // Seek Socket
    {
        ThreadID threadID = ThreadCurrentGet();

        for(size_t i = 0; i < server->ServerSocketListSize; ++i)
        {
            CSocket* serverSocket = &server->ServerSocketList[i];

            if(serverSocket->CommunicationThread == threadID)
            {
                serverSocket = serverSocket;
                break;
            }
        }
    }

    if(!serverSocket)
    {
        return ThreadSucessful;
    }

    while(CSocketIsCurrentlyUsed(serverSocket))
    {
        CSocket clientSocket;

        CSocketConstruct(&clientSocket);

        // Set Events

        const ActionResult actionResult = CSocketAccept(serverSocket, &clientSocket);
        const unsigned char successful = ActionSuccessful == actionResult;


        if(!successful)
        {
            InvokeEvent(server->ClientAcceptFailureCallback, &serverSocket);

            continue; // failed.. retry?
        }

#if SocketDebug
        printf("[i][Server] New client accepted <%zi>\n", clientSocket.ID);
#endif

        InvokeEvent(server->ClientConnectedCallback, serverSocket, &clientSocket);

        ServerRegisterClient(serverSocket , &clientSocket);
    }

    return ThreadSucessful;
}
