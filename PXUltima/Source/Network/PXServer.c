#include "PXServer.h"

#include <Event/Event.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

#if OSUnix
#include <poll.h>
#define OSSocketPoll poll
#elif OSWindows
#define OSSocketPoll WSAPoll
#endif

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

void PXSocketReadPending(PXServer* const server, const PXSocketID socketID)
{
    PXSocket* serverSocket = &server->ServerSocketList[0];

    if (socketID == serverSocket->ID) // Is Server
    {
        PXSocket clientSocket;

        PXSocketConstruct(&clientSocket);

        // Set Events

        const PXActionResult actionResult = PXSocketAccept(serverSocket, &clientSocket);
        const PXBool successful = PXActionSuccessful == actionResult;

        if (!successful)
        {
            InvokeEvent(server->PXClientAcceptFailureCallback, serverSocket);
            return;
        }

        InvokeEvent(server->PXClientConnectedCallback, serverSocket, &clientSocket);

        PXSocketEventReadRegister(server, clientSocket.ID);

        PXServerRegisterPXClient(server, &clientSocket);
    }
    else
    {
        PXSocket clientSocket;

        PXServerConstruct(&clientSocket);

        clientSocket.ID = socketID;
        clientSocket.EventList = server->SocketEventListener;

        char inputBuffer[1024];
        PXSize wrrit = 0;

        PXSocketEventReadUnregister(server, clientSocket.ID);

        PXSocketReceive(&clientSocket, inputBuffer, 1024, &wrrit);
    }
}



void PXSocketEventPull(PXServer* const server, void* const buffer, const PXSize bufferSize)
{

#if 1 // Use optimised OS function

    struct pollfd* socketDataList = buffer;
    PXSize socketDataListSize = server->SocketPollingReadListSize;
    int timeout = 0;

    for (PXSize i = 0; i < server->SocketPollingReadListSize; ++i)
    {
        socketDataList->fd = server->SocketPollingReadList[i];
        socketDataList->events = POLLRDNORM;
        socketDataList->revents = 0;
    }

    const int amount = OSSocketPoll(socketDataList, socketDataListSize, timeout);

    for (PXSize i = 0; i < amount; i++)
    {
        const WSAPOLLFD* const currentPollData = &socketDataList[i];

        switch (currentPollData->revents)
        {
            case POLLERR: // An error has occurred.
            case POLLHUP: // A stream - oriented connection was either disconnected or aborted.
            {
                PXSocketEventReadUnregister(server, currentPollData->fd);
                break;
            }
            case POLLNVAL: // An invalid socket was used.
            case POLLPRI: // Priority data may be read without blocking.This flag is not returned by the Microsoft Winsock provider.
            case POLLRDBAND: // Priority band(out - of - band) data may be read without blocking.
            case POLLRDNORM: // Normal data may be read without blocking.
            {
                PXSocketReadPending(server, currentPollData->fd);
                break;
            }
            case POLLWRNORM: // Normal data may be written without blocking.

            default:
                break;
        }
    }

#else

    const PXSize neededFetches = (server->SocketPollingReadListSize / FD_SETSIZE) + 1;

    const TIMEVAL time = { 3,0 };

    PXSize restValues = server->SocketPollingReadListSize;
    fd_set selectListenRead;

    for (PXSize i = 0; i < neededFetches; ++i)
    {
        const PXSocketID* const socketIDList = &server->SocketPollingReadList[i * FD_SETSIZE];
        const PXSize fdBlockSize = MathMinimum(restValues, FD_SETSIZE);
        const PXSize fdBlockSizeBytes = fdBlockSize * sizeof(PXSocketID);

        restValues -= fdBlockSize;

        selectListenRead.fd_count = fdBlockSize;

        MemoryCopy(socketIDList, fdBlockSizeBytes, &selectListenRead.fd_array, fdBlockSizeBytes);

        const int numberOfSocketEvents = select(0, &selectListenRead, 0, 0, 0);

        for (PXSize l = 0; l < numberOfSocketEvents; ++l)
        {
            const PXSocketID socketID = selectListenRead.fd_array[i];

            PXSocketReadPending(server, socketID);
        }
    }
#endif
}

void PXSocketEventReadRegister(PXServer* const server, const PXSocketID socketID)
{
    PXSocketID* const socketIDRef = &server->SocketPollingReadList[server->SocketPollingReadListSize++];

    *socketIDRef = socketID;
}

void PXSocketEventReadUnregister(PXServer* const server, const PXSocketID socketID)
{
    PXSize offset = 0;
    PXSocketID* socketIDRef = 0;

    for (offset = 0; offset < server->SocketPollingReadListSize; ++offset)
    {
        const PXSocketID compareID = server->SocketPollingReadList[offset];

        if (compareID == socketID)
        {
            // found
            socketIDRef = &server->SocketPollingReadList[offset];
            break;
        }
    }

    if (!socketIDRef)
    {
        return;
    }

    PXBool endOfList = offset + 1 == server->SocketPollingReadListSize;

    if (endOfList)
    {
        *socketIDRef = 0;
    }
    else
    {
        PXSize copySize = server->SocketPollingReadListSize - offset - 1;

        MemoryMove(socketIDRef + 1, copySize, socketIDRef, copySize);
    }

    --(server->SocketPollingReadListSize);
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

    server->SocketPollingReadList = MemoryAllocateClear(sizeof(PXSocketID) * 1024);

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

        InvokeEvent(pxSocket->EventList.ConnectionListeningCallback, pxSocket);

        PXSocketEventReadRegister(server, pxSocket->ID);

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

    if(!serverSocket)
    {
        return PXThreadSucessful;
    }

    PXSize pollBufferSize = 1024;
    char pollBuffer[1024];

    while(PXSocketIsCurrentlyUsed(serverSocket))
    {
        PXSocketEventPull(server, pollBuffer, pollBufferSize);
    }

    return PXThreadSucessful;
}
