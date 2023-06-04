#include "PXClient.h"

#if PXSocketUSE

#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXEvent.h>

PXActionResult PXClientConstruct(PXClient* const pxClient)
{
    PXMemoryClear(pxClient, sizeof(PXClient));
}

PXActionResult PXClientDestruct(PXClient* const pxClient)
{
    PXSocketDestruct(&pxClient->SocketServer);
    PXSocketDestruct(&pxClient->SocketClient);
}

PXActionResult PXClientSendData(PXClient* const pxClient, const void* const data, const PXSize dataSize)
{
    PXBufferConstruct(&pxClient->SocketClient.BufferOutput, data, dataSize, PXBufferExtern); 

    const PXActionResult sendResult = PXSocketSend(&pxClient->SocketClient, pxClient->SocketClient.ID);

    PXBufferDestruct(&pxClient->SocketClient.BufferOutput);

    return sendResult;
}

PXActionResult PXClientConnectToServer(PXClient* const client, const PXText* const ip, const PXInt16U port)
{
    PXSocket pxSocketList[5];
    PXSize pxSocketListSizeMax = 5;
    PXSize PXSocketListSize = 0;

    PXMemoryClear(pxSocketList,sizeof(PXSocket)* pxSocketListSizeMax);

    const PXSocketAdressSetupInfo pxSocketAdressSetupInfoList[1] =
    {
          *ip, port, IPAdressFamilyUnspecified, PXSocketTypeStream, ProtocolModeTCP
    };
    const PXSize pxSocketAdressSetupInfoListSize = sizeof(pxSocketAdressSetupInfoList) / sizeof(PXSocketAdressSetupInfo);

    PXSocketSetupAdress
    (
        pxSocketList,
        pxSocketListSizeMax,
        &PXSocketListSize,
        pxSocketAdressSetupInfoList,
        pxSocketAdressSetupInfoListSize
    );

    PXBool wasSucessful = 0;

    client->SocketServer.EventList = client->EventList;
    client->SocketServer.Owner = client->Owner;

    for (PXSize i = 0; i < PXSocketListSize; ++i)
    {
        PXSocket* const pxSocketTemp = &pxSocketList[i];
        pxSocketTemp->EventList = client->EventList;
        pxSocketTemp->Owner = client->Owner;

        const PXActionResult socketCreateResult = PXSocketCreate(pxSocketTemp, pxSocketTemp->Family, pxSocketTemp->Type, pxSocketTemp->Protocol);
        const PXBool creationSuccesful = PXActionSuccessful == socketCreateResult;

        if (creationSuccesful)
        {
            const PXActionResult connectResult = PXSocketConnect(pxSocketTemp, &client->SocketServer);
            const PXBool connected = PXActionSuccessful == connectResult; 

            if (connected)
            {  
                // Copy buffered socket and do not use stack reference
                PXSocket* const pxSocketClient = &client->SocketClient;
                PXMemoryCopy(pxSocketTemp, sizeof(PXSocket), pxSocketClient, sizeof(PXSocket));

                InvokeEvent(pxSocketClient->EventList.SocketConnectedCallBack, client->Owner, &client->SocketClient, &client->SocketServer);

                const PXActionResult PXActionResult = PXThreadRun(&pxSocketClient->CommunicationThread, PXClientCommunicationThread, pxSocketClient);

                wasSucessful = 1u;
                break; // Connect only once. If this is not here, we would connect more than once (with different protocol)
            }
        }
    }

    if (!wasSucessful)
    {
        return PXActionFailedSocketConnect;
    }

    return PXActionSuccessful;
}

PXActionResult PXClientDisconnectFromServer(PXClient* const client)
{
    PXSocketClose(&client->SocketClient);
    PXSocketConstruct(&client->SocketServer);

    return PXActionSuccessful;
}

PXThreadResult PXClientCommunicationThread(PXSocket* const pxSocket)
{
    while (PXSocketIsCurrentlyUsed(pxSocket))
    {      
        PXByte buffer[PXSocketBufferSize];
        PXSize bufferSize = 0;

        const PXActionResult receiveingResult = PXSocketReceive(pxSocket, buffer, PXSocketBufferSize, &bufferSize);
        const PXBool sucessful = PXActionSuccessful == receiveingResult;

        if (!sucessful)
        {
            PXSocketDestruct(pxSocket);

            return 1;
        }
    }

    return 0;
}
#endif