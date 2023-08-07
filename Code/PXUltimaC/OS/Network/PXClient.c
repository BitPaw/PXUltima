#include "PXClient.h"

#if PXSocketUSE

#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXEvent.h>

void PXClientConstruct(PXClient* const pxClient)
{
    PXObjectClear(PXClient, pxClient);

    PXSocketConstruct(&pxClient->SocketServer);
    PXSocketConstruct(&pxClient->SocketClient);
}

void PXClientDestruct(PXClient* const pxClient)
{
    PXSocketDestruct(&pxClient->SocketServer);
    PXSocketDestruct(&pxClient->SocketClient);

    PXClientConstruct(pxClient);
}

PXActionResult PXClientSendData(PXClient* const pxClient, const void* const data, const PXSize dataSize)
{
    if (pxClient->SocketClient.ID == PXSocketUnused)
    {
        return PXActionRefuedObjectIDInvalid;
    }

    PXBufferConstruct(&pxClient->SocketClient.BufferOutput, (void*)data, dataSize, PXBufferExtern); 

    const PXActionResult sendResult = PXSocketSend(&pxClient->SocketClient, pxClient->SocketClient.ID);

    PXBufferDestruct(&pxClient->SocketClient.BufferOutput);

    return sendResult;
}

PXActionResult PXClientConnectToSelf(PXClient* const client, const PXInt16U port)
{
    PXText ip;
    PXTextMakeFixedA(&ip, "127.0.0.1");

    return PXClientConnectToServer(client, &ip, port);
}

PXActionResult PXClientConnectToServer(PXClient* const client, const PXText* const ip, const PXInt16U port)
{
    PXSocket pxSocketList[3];
    const PXSize pxSocketListSizeMax = 3;
    PXSize socketListSize = 0;

    const int x = sizeof(PXSocket) *pxSocketListSizeMax;

    PXMemoryClear(pxSocketList,sizeof(PXSocket)* pxSocketListSizeMax);

    // Setup
    {
        const PXSocketAdressSetupInfo pxSocketAdressSetupInfoList[1] =
        {
              *ip, port, IPAdressFamilyUnspecified, PXSocketTypeStream, ProtocolModeTCP
        };
        const PXSize pxSocketAdressSetupInfoListSize = sizeof(pxSocketAdressSetupInfoList) / sizeof(PXSocketAdressSetupInfo);

        const PXActionResult setupResult = PXSocketSetupAdress
        (
            pxSocketList,
            pxSocketListSizeMax,
            &socketListSize,
            pxSocketAdressSetupInfoList,
            pxSocketAdressSetupInfoListSize
        );

        PXActionReturnOnError(setupResult);

        if (socketListSize == 0)
        {
            // We dont have valid connections, cancel
            return AudioResultMoreData;
        }
    }

    client->SocketServer.EventList = client->EventList;
    client->SocketServer.Owner = client->Owner;

    PXActionResult lastError = PXActionInvalid;

    for (PXSize i = 0; i < socketListSize; ++i)
    {
        PXSocket* const pxSocketTemp = &pxSocketList[i];
        pxSocketTemp->EventList = client->EventList;
        pxSocketTemp->Owner = client->Owner;

        lastError = PXSocketCreate(pxSocketTemp, pxSocketTemp->Family, pxSocketTemp->Type, pxSocketTemp->Protocol);
       
        PXActionContinueOnError(lastError);
        
        lastError = PXSocketConnect(pxSocketTemp, &client->SocketServer);

        PXActionContinueOnError(lastError);

        // Copy buffered socket and do not use stack reference
        PXSocket* const pxSocketClient = &client->SocketClient;
        PXMemoryCopy(pxSocketTemp, sizeof(PXSocket), pxSocketClient, sizeof(PXSocket));

        InvokeEvent(pxSocketClient->EventList.SocketConnectedCallBack, client->Owner, &client->SocketClient, &client->SocketServer);

        lastError = PXThreadRun(&pxSocketClient->CommunicationThread, (ThreadFunction)PXClientCommunicationThread, pxSocketClient);

        PXActionContinueOnError(lastError);

        lastError = PXActionSuccessful;
        break; // Connect only once. If this is not here, we would connect more than once (with different protocol)
    }

    return lastError;
}

PXActionResult PXClientDisconnectFromServer(PXClient* const client)
{
    PXSocketClose(&client->SocketClient);
    PXSocketConstruct(&client->SocketServer);

    return PXActionSuccessful;
}

PXThreadResult PXOSAPI PXClientCommunicationThread(PXSocket* const pxSocket)
{
    PXByte buffer[PXSocketBufferSize];

    PXBufferConstruct(&pxSocket->BufferInput, buffer, PXSocketBufferSize, PXBufferTypeStack);

    while (PXSocketIsCurrentlyUsed(pxSocket))
    {   
        const PXActionResult receiveingResult = PXSocketReceive(pxSocket, pxSocket->ID);
        const PXBool sucessful = PXActionSuccessful == receiveingResult;

        if (!sucessful)
        {
            pxSocket->CommunicationThread.Mode = PXThreadModeFinished;
          
            return 1;
        }
    }

    pxSocket->CommunicationThread.Mode = PXThreadModeFinished;

    return 0;
}
#endif