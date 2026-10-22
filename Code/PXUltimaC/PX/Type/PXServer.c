#include "PXServer.h"
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
    /*
    for(PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket PXREF serverSocket = &server->ServerSocketList[i];
        const PXBool isTarget = socketID == serverSocket->ID;

        if(isTarget)
        {
            *pxSocket = serverSocket;

            return PXTrue;
        }
    }*/

    return PXFalse;
}

PXResult PXAPI PXServerStart(PXServer PXREF server, const PXI16U port, const PXProtocolMode protocolMode)
{
    /*

    // Setup address info
    {
        const PXSocketAddressSetupInfo pxSocketAddressSetupInfoList[2] =
        {
            { {0,0,0,0,TextFormatUTF8}, port, IPAddressFamilyINET,PXSocketTypeStream, protocolMode},
            { {0,0,0,0,TextFormatUTF8}, port, IPAddressFamilyINET6,PXSocketTypeStream, protocolMode }
        };
        const PXSize pxSocketAddressSetupInfoListSize = 1;// sizeof(pxSocketAddressSetupInfoList) / sizeof(PXSocketAddressSetupInfo);

        server->ServerSocketListSize = pxSocketAddressSetupInfoListSize;
        server->ServerSocketListSizeAllocated = pxSocketAddressSetupInfoListSize;
        server->ServerSocketList = PXNewList(PXSocket, pxSocketAddressSetupInfoListSize);

        for(PXSize i = 0; i < pxSocketAddressSetupInfoListSize; ++i)
        {
            PXSocketConstruct(&server->ServerSocketList[i]);
        }

        const PXResult addressResult = PXSocketSetupAddress
        (
            server->ServerSocketList,
            server->ServerSocketListSizeAllocated,
            &server->ServerSocketListSize,
            pxSocketAddressSetupInfoList,
            pxSocketAddressSetupInfoListSize
        );

        PXResultReturnOnError(addressResult);
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
            const PXResult socketCreateResult = PXNetworkSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);

            PXResultReturnOnError(socketCreateResult);
        }

        // Set Socket Options
        {
            const PXResult actionResult = PXSocketOptionsSet(pxSocket);

            PXResultReturnOnError(actionResult);
        }

        // Bind Socket
        {
            const PXResult actionResult = PXSocketBind(pxSocket);

            PXResultReturnOnError(actionResult);
        }

        // Listen
        {
            const PXResult actionResult = PXSocketListen(pxSocket);

            PXResultReturnOnError(actionResult);
        }

        PXSocketStateChange(pxSocket, SocketIDLE);

        const PXResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, "PXServerClientListening", PXServerClientListeningThread, pxSocket);
    }

    */

    return PXResultOK;
}

PXResult PXAPI PXServerStop(PXServer PXREF server)
{
    /*
    if(server->ServerSocketListSize == 0)
    {
        return PXResultRefusedSocketNotConnected;
    }

    PXSocket* ServerSocketList;
    PXSize ServerSocketListSize;

    for(PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket PXREF serverSocket = &server->ServerSocketList[i];

        PXSocketClose(serverSocket);
    }

    server->ServerSocketListSize = 0;

    PXLockDelete(&server->PollingLock);
    */
    return PXResultOK;
}

PXResult PXAPI PXServerKickClient(PXServer PXREF server, const PXSocketID socketID)
{
    return PXResultOK;
}

PXResult PXAPI PXServerSendToAll(PXServer PXREF server, const void PXREF data, const PXSize dataSize)
{
    /*
    for(PXSize i = 0; i < server->ServerSocketListSize; i++)
    {
        PXSocket PXREF serverSubSocket = &server->ServerSocketList[i];

        for(PXSize i = 0; i < serverSubSocket->SocketLookup.EntryAmountCurrent; i++) // All sockets
        {
            PXDictionaryEntry pxDictionaryEntry;

            PXDictionaryIndex(&serverSubSocket->SocketLookup, i, &pxDictionaryEntry);

            const PXSocketID clientID = *(PXSocketID*)pxDictionaryEntry.Key;

            PXBufferConstruct(&serverSubSocket->BufferOutput, (void*)data, dataSize, PXBufferTypeStack);

            PXSocketSend(serverSubSocket, clientID);
        }

        PXBufferDestruct(&serverSubSocket->BufferOutput);
    }*/

    return PXResultOK;
}

PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket PXREF serverSocket)
{
    PXThreadResult pxThreadResult;

    /*
    char pollBuffer[PXSocketBufferSize];

    if(!serverSocket)
    {
        return PXResultSuccessful;
    }

    PXDictionaryEntryAdd(&serverSocket->SocketLookup, &serverSocket->ID, serverSocket);

    PXBufferConstruct(&serverSocket->BufferInput, pollBuffer, PXSocketBufferSize, PXBufferTypeStack);

    while(serverSocket->State == SocketIDLE)
    {
        serverSocket->BufferInput.SizeCurrent = 0;
        serverSocket->BufferInput.SizeOffset = 0;

        PXSocketEventPull(serverSocket);
    }

    PXBufferDestruct(&serverSocket->BufferInput);
    */
    return pxThreadResult;
}