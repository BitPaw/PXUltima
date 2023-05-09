#include "PXClient.h"

#include <OS/Memory/PXMemory.h>
#include <Event/PXEvent.h>

PXActionResult PXClientConstruct(PXClient* const pxClient)
{
    MemoryClear(pxClient, sizeof(PXClient));
}

PXActionResult PXClientDestruct(PXClient* const pxClient)
{
    
}

PXActionResult PXClientSendData(PXClient* const pxClient, const void* const data, const PXSize dataSize)
{
    return PXSocketSend(&pxClient->SocketPXClient, data, dataSize, 0);
}

PXActionResult PXClientConnectToServer(PXClient* const client, const char* ip, unsigned short port, const void* threadObject, const ThreadFunction threadFunction)
{
    PXSocket pxSocketList[5];
    PXSize pxSocketListSizeMax = 5;
    PXSize PXSocketListSize = 0;

    MemoryClear(pxSocketList,sizeof(PXSocket)* pxSocketListSizeMax);

    const PXSocketAdressSetupInfo pxSocketAdressSetupInfoList[1] =
    {
          ip, port, IPAdressFamilyUnspecified, PXSocketTypeStream, ProtocolModeTCP
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
                MemoryCopy(pxSocket, sizeof(PXSocket), &client->SocketPXClient, sizeof(PXSocket));

                client->SocketPXClient.EventList = client->EventListener;

                InvokeEvent(pxSocket->EventList.ConnectionEstablishedCallback, pxSocket);

                const PXActionResult PXActionResult = PXThreadRun(&pxSocket->CommunicationThread, threadFunction, threadObject);

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
    return PXActionInvalid;
}

#define PXClientBufferSize 2048u

PXThreadResult CommunicationFunctionAsync(void* PXSocketAdress)
{
    PXSocket* const pxSocket = (PXSocket*)PXSocketAdress;

    while (PXSocketIsCurrentlyUsed(pxSocket))
    {
        const PXSize bufferSizeMax = 2048;
        PXSize bufferSize = 0;
        PXByte buffer[PXClientBufferSize];

        MemoryClear(buffer, sizeof(unsigned char) * PXClientBufferSize);

        const PXActionResult receiveingResult = PXSocketReceive(pxSocket, buffer, bufferSizeMax, &bufferSize);
        const PXBool sucessful = PXActionSuccessful == receiveingResult;

        if (!sucessful)
        {
            PXSocketDestruct(pxSocket);
        }
    }

    return 0;
}