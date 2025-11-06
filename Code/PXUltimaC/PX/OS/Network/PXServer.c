#include "PXServer.h"

#if PXSocketUSE

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

#if SocketDebug
#include <stdio.h>
#endif

void PXAPI PXServerConstruct(PXServer PXREF server)
{
    PXMemoryClear(server, sizeof(PXServer));
}

void PXAPI PXServerDestruct(PXServer PXREF server)
{

}

PXBool PXAPI PXServerSocketIDIsServer(const PXServer PXREF server, const PXSocketID socketID, PXSocket* PXREF pxSocket)
{
    for (PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket PXREF serverSocket = &server->ServerSocketList[i];
        const PXBool isTarget = socketID == serverSocket->ID;

        if (isTarget)
        {
            *pxSocket = serverSocket;

            return PXTrue;
        }
    }

    return PXFalse;
}

PXResult PXAPI PXServerStart(PXServer PXREF server, const PXI16U port, const PXProtocolMode protocolMode)
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

        const PXResult adressResult = PXSocketSetupAdress
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
        PXSocket PXREF pxSocket = &server->ServerSocketList[i];

        pxSocket->EventList = server->EventList;
        pxSocket->Owner = server->Owner;
        pxSocket->PollingLock = &server->PollingLock;

        // Create socket
        {
            const PXResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);

            PXActionReturnOnError(socketCreateResult);
        }

        // Set Socket Options
        {
            const PXResult actionResult = PXSocketOptionsSet(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        // Bind Socket
        {
            const PXResult actionResult = PXSocketBind(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        // Listen
        {
            const PXResult actionResult = PXSocketListen(pxSocket);

            PXActionReturnOnError(actionResult);
        }

        PXSocketStateChange(pxSocket, SocketIDLE);

        const PXResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, "PXServerClientListening", PXServerClientListeningThread, pxSocket);
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXServerStop(PXServer PXREF server)
{
    if (server->ServerSocketListSize == 0)
    {
        return PXActionRefusedSocketNotConnected;
    }

    PXSocket* ServerSocketList;
    PXSize ServerSocketListSize;

    for (PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket PXREF serverSocket = &server->ServerSocketList[i];

        PXSocketClose(serverSocket);
    }

    server->ServerSocketListSize = 0;

    PXLockDelete(&server->PollingLock);

    return PXActionSuccessful;
}

PXResult PXAPI PXServerKickClient(PXServer PXREF server, const PXSocketID socketID)
{
    return PXActionInvalid;
}

PXResult PXAPI PXServerSendToAll(PXServer PXREF server, const void PXREF data, const PXSize dataSize)
{
    for (PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket PXREF serverSubSocket = &server->ServerSocketList[i];

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

PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket PXREF serverSocket)
{
    char pollBuffer[PXSocketBufferSize];

    if(!serverSocket)
    {
        return PXActionSuccessful;
    }

    PXDictionaryEntryAdd(&serverSocket->SocketLookup, &serverSocket->ID, serverSocket);

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
