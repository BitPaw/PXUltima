#include "PXServer.h"

#include <Event/PXEvent.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

#if SocketDebug
#include <stdio.h>
#endif

void PXServerConstruct(PXServer* const server)
{
    PXMemoryClear(server, sizeof(PXServer)); 
}

void PXServerDestruct(PXServer* const server)
{

}

PXBool PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket)
{
    for (PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        const PXSocket* const serverSocket = &server->ServerSocketList[i];
        const PXBool isTarget = socketID == serverSocket->ID;

        if (isTarget)
        {
            *pxSocket = serverSocket;

            return PXTrue;
        }
    }

    return PXFalse;
}

PXActionResult PXServerStart(PXServer* const server, const PXInt16U port, const ProtocolMode protocolMode)
{
    // Setup adress info
    {
        const PXSocketAdressSetupInfo pxSocketAdressSetupInfoList[2] =
        {
            { {0,0,0,0,1}, port, IPAdressFamilyINET,PXSocketTypeStream, protocolMode},
            { {0,0,0,0,1}, port, IPAdressFamilyINET6,PXSocketTypeStream, protocolMode }
        };
        const PXSize pxSocketAdressSetupInfoListSize = 1;// sizeof(pxSocketAdressSetupInfoList) / sizeof(PXSocketAdressSetupInfo);

        server->ServerSocketListSize = pxSocketAdressSetupInfoListSize;
        server->ServerSocketListSizeAllocated = pxSocketAdressSetupInfoListSize;
        server->ServerSocketList = PXMemoryAllocateClear(sizeof(PXSocket) * pxSocketAdressSetupInfoListSize);

        for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
        {
            PXSocketConstruct(&server->ServerSocketList[i]);
        }

        const PXActionResult adressResult = PXSocketSetupAdress
        (
            server->ServerSocketList,
            server->ServerSocketListSizeAllocated,
            &server->ServerSocketListSize,
            &pxSocketAdressSetupInfoList,
            pxSocketAdressSetupInfoListSize
        );

        PXActionExitOnError(adressResult);
    }

    PXLockCreate(&server->PollingLock, PXLockTypeProcessOnly);

    for(PXSize i = 0; i < server->ServerSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &server->ServerSocketList[i];

        pxSocket->EventList = server->EventListener;
        pxSocket->Owner = server->Owner;
        pxSocket->PollingLock = &server->PollingLock;

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

        PXSocketStateChange(pxSocket, SocketIDLE);

        InvokeEvent(pxSocket->EventList.ConnectionListeningCallback, pxSocket);   

        const PXActionResult actionResult = PXThreadRun(&pxSocket->CommunicationThread, PXServerPXClientListeningThread, pxSocket);       
    }

    return PXActionSuccessful;
}

PXActionResult PXServerStop(PXServer* const server)
{
    PXLockDelete(&server->PollingLock);

	return PXActionInvalid;
}

PXActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID)
{
	return PXActionInvalid;
}

PXThreadResult PXServerPXClientListeningThread(void* serverAdress)
{
    PXSocket* const serverSocket = (PXSocket*)serverAdress; 

    if(!serverSocket)
    {
        return PXThreadSucessful;
    }

    PXDictionaryAdd(&serverSocket->SocketLookup, &serverSocket->ID, serverSocket);

    while (serverSocket->State == SocketIDLE)
    {
        PXSize pollBufferSize = 1024;
        char pollBuffer[1024];

        PXSocketEventPull(serverSocket, pollBuffer, pollBufferSize);
    }

    return PXThreadSucessful;
}
