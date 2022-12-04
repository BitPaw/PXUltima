#include "PXServer.h"

#include <Event/Event.h>
#include <Memory/PXMemory.h>

#if SocketDebug
#include <stdio.h>
#endif

void PXServerConstruct(PXServer* const server)
{
    MemorySet(server, sizeof(PXServer), 0);
}

void PXServerDestruct(PXServer* const server)
{

}

ActionResult PXServerStart(PXServer* const server, const unsigned short port, const ProtocolMode protocolMode)
{
    PXSize adressInfoListSize = 0;

    // Setup adress info
    {
        const ActionResult adressResult = PXSocketSetupAdress
        (
            server->ServerSocketList,
            -1,
            &server->ServerSocketListSize,
            0, // IP
            port,
            IPAdressFamilyUnspecified,
            PXSocketTypeStream,
            protocolMode
        );
        const unsigned char adressSetupSucessful = ActionSuccessful == adressResult;

        if(!adressSetupSucessful)
        {
            return adressResult;
        }
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
            const ActionResult socketCreateResult = PXSocketCreate(&pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);
            const unsigned char sucessful = ActionSuccessful == socketCreateResult;

            if(!sucessful)
            {
                return socketCreateResult;
            }
        }

        // Set Socket Options
        {
            const ActionResult actionResult = PXSocketOptionsSet(&pxSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        // Bind Socket
        {
            const ActionResult actionResult = PXSocketBind(&pxSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        // Listen
        {
            const ActionResult actionResult = PXSocketListen(&pxSocket);
            const unsigned char sucessful = ActionSuccessful == actionResult;

            if(!sucessful)
            {
                return actionResult;
            }
        }

        InvokeEvent(pxSocket->ConnectionListeningCallback, pxSocket);

        const ActionResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, PXServerPXClientListeningThread, pxSocket);
    }

    return ActionSuccessful;
}

ActionResult PXServerStop(PXServer* const server)
{
	return ActionInvalid;
}

ActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID)
{
	return ActionInvalid;
}

PXSocket* PXServerGetPXClientViaID(PXServer* const server, const PXSocketID socketID)
{
    for(PXSize i = 0; i < server->PXClientSocketListSize; i++)
    {
        const PXSocket* clientSocket = &server->PXClientSocketList[i];
        const PXSocketID clientSocketID = clientSocket->ID;
        const unsigned char foundTarget = clientSocketID == socketID;

        if(foundTarget)
        {
            return clientSocket;
        }
    }

    return 0;
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

ActionResult PXServerSendMessageToAll(PXServer* server, const unsigned char* data, const PXSize dataSize)
{
    for(PXSize i = 0; i < server->PXClientSocketListSize; ++i)
    {
        PXSocket* serverSocket = &server->PXClientSocketList[i];
        PXSize writtenBytes = 0;
        const ActionResult actionResult = PXSocketSend(serverSocket, data, dataSize, &writtenBytes);
    }
}

ActionResult PXServerSendMessageToPXClient(PXServer* server, const PXSocketID clientID, const unsigned char* data, const PXSize dataSize)
{
    PXSocket* clientSocket = PXServerGetPXClientViaID(server, clientID);

    if(!clientSocket)
    {
        return NoPXClientWithThisID;
    }

    PXSize writtenBytes = 0;
    const ActionResult actionResult = PXSocketSend(clientSocket, data, dataSize, &writtenBytes);

    return actionResult;
}

PXThreadResult PXServerPXClientListeningThread(void* serverAdress)
{
    PXServer* server = serverAdress;
    PXSocket* serverSocket = 0;

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

        const ActionResult actionResult = PXSocketAccept(serverSocket, &clientSocket);
        const unsigned char successful = ActionSuccessful == actionResult;


        if(!successful)
        {
            InvokeEvent(server->PXClientAcceptFailureCallback, &serverSocket);

            continue; // failed.. retry?
        }

#if SocketDebug
        printf("[i][Server] New client accepted <%zi>\n", clientSocket.ID);
#endif

        InvokeEvent(server->PXClientConnectedCallback, serverSocket, &clientSocket);

        PXServerRegisterPXClient(serverSocket , &clientSocket);
    }

    return PXThreadSucessful;
}
