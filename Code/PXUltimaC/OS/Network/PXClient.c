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
    return PXSocketSend(&pxClient->SocketClient, data, dataSize, 0);
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

    for (PXSize i = 0; i < PXSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &pxSocketList[i];
        const PXActionResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);
        const PXBool creationSuccesful = PXActionSuccessful == socketCreateResult;

        if (creationSuccesful)
        {
            const PXActionResult connectResult = PXSocketConnect(pxSocket);
            const PXBool connected = PXActionSuccessful == connectResult;

            if (connected)
            {  
                PXMemoryCopy(pxSocket, sizeof(PXSocket), &client->SocketClient, sizeof(PXSocket));

                client->SocketClient.EventList = client->EventList;

                InvokeEvent(pxSocket->EventList.SocketConnectedCallBack, client->Owner, pxSocket, PXNull);

                const PXActionResult PXActionResult = PXThreadRun(&pxSocket->CommunicationThread, PXClientCommunicationThread, pxSocket);

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

void PXClientDisconnectFromServer(PXClient* const client)
{
    PXSocketClose(&client->SocketClient);
    PXSocketConstruct(&client->SocketServer);
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