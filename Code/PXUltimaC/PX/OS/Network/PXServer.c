#include "PXServer.h"

#if PXSocketUSE

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

#if SocketDebug
#include <stdio.h>
#endif

void PXAPI PXServerConstruct(PXServer* const server)
{
    PXMemoryClear(server, sizeof(PXServer));
}

void PXAPI PXServerDestruct(PXServer* const server)
{

}

PXBool PXAPI PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket)
{
    for (PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket* const serverSocket = &server->ServerSocketList[i];
        const PXBool isTarget = socketID == serverSocket->ID;

        if (isTarget)
        {
            *pxSocket = serverSocket;

            return PXTrue;
        }
    }

    return PXFalse;
}

PXActionResult PXAPI PXServerStart(PXServer* const server, const PXInt16U port, const PXProtocolMode protocolMode)
{
    // Setup adress info
    {
        const PXSocketAdressSetupInfo pxSocketAdressSetupInfoList[2] =
        {
            { {0,0,0,0,TextFormatUTF8}, port, IPAdressFamilyINET,PXSocketTypeStream, protocolMode},
            { {0,0,0,0,TextFormatUTF8}, port, IPAdressFamilyINET6,PXSocketTypeStream, protocolMode }
        };
        const PXSize pxSocketAdressSetupInfoListSize = 1;// sizeof(pxSocketAdressSetupInfoList) / sizeof(PXSocketAdressSetupInfo);

        server->ServerSocketListSize = pxSocketAdressSetupInfoListSize;
        server->ServerSocketListSizeAllocated = pxSocketAdressSetupInfoListSize;
        server->ServerSocketList = PXNewList(PXSocket, pxSocketAdressSetupInfoListSize);

        for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
        {
            PXSocketConstruct(&server->ServerSocketList[i]);
        }

        const PXActionResult adressResult = PXSocketSetupAdress
                                            (
                                                server->ServerSocketList,
                                                server->ServerSocketListSizeAllocated,
                                                &server->ServerSocketListSize,
                                                pxSocketAdressSetupInfoList,
                                                pxSocketAdressSetupInfoListSize
                                            );

        PXActionReturnOnError(adressResult);
    }

    PXLockCreate(&server->PollingLock, PXLockTypeProcessOnly);

    for(PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &server->ServerSocketList[i];

        pxSocket->EventList = server->EventList;
        pxSocket->Owner = server->Owner;
        pxSocket->PollingLock = &server->PollingLock;

        // Create socket
        {
            const PXActionResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);

            PXActionReturnOnError(socketCreateResult);
        }

        // Set Socket Options
        {
            const PXActionResult actionResult = PXSocketOptionsSet(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        // Bind Socket
        {
            const PXActionResult actionResult = PXSocketBind(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        // Listen
        {
            const PXActionResult actionResult = PXSocketListen(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        PXSocketStateChange(pxSocket, SocketIDLE);

        const PXActionResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, "PXServerClientListening", PXServerClientListeningThread, pxSocket);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXServerStop(PXServer* const server)
{
    if (server->ServerSocketListSize == 0)
    {
        return PXActionRefusedSocketNotConnected;
    }

    PXSocket* ServerSocketList;
    PXSize ServerSocketListSize;

    for (PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket* const serverSocket = &server->ServerSocketList[i];

        PXSocketClose(serverSocket);
    }

    server->ServerSocketListSize = 0;

    PXLockDelete(&server->PollingLock);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXServerKickClient(PXServer* const server, const PXSocketID socketID)
{
    return PXActionInvalid;
}

PXActionResult PXAPI PXServerSendToAll(PXServer* const server, const void* const data, const PXSize dataSize)
{
    for (PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket* const serverSubSocket = &server->ServerSocketList[i];

        for (PXSize i = 0; i < serverSubSocket->SocketLookup.EntryAmountCurrent; i++) // All sockets
        {
            PXDictionaryEntry pxDictionaryEntry;

            PXDictionaryIndex(&serverSubSocket->SocketLookup, i, &pxDictionaryEntry);

            const PXSocketID clientID = *(PXSocketID*)pxDictionaryEntry.Key;

            PXBufferConstruct(&serverSubSocket->BufferOutput, (void*)data, dataSize, PXBufferTypeStack);

            PXSocketSend(serverSubSocket, clientID);
        }

        PXBufferDestruct(&serverSubSocket->BufferOutput);
    }

    return PXActionSuccessful;
}

PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket* const serverSocket)
{
    char pollBuffer[PXSocketBufferSize];

    if(!serverSocket)
    {
        return PXActionSuccessful;
    }

    PXDictionaryAdd(&serverSocket->SocketLookup, &serverSocket->ID, serverSocket);

    PXBufferConstruct(&serverSocket->BufferInput, pollBuffer, PXSocketBufferSize, PXBufferTypeStack);

    while (serverSocket->State == SocketIDLE)
    {
        serverSocket->BufferInput.SizeCurrent = 0;
        serverSocket->BufferInput.SizeOffset = 0;

        PXSocketEventPull(serverSocket);
    }

    PXBufferDestruct(&serverSocket->BufferInput);

    return PXActionSuccessful;
}
#endif
