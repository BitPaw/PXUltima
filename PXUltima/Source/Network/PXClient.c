#include "PXClient.h"

#include <Memory/PXMemory.h>
#include <Event/Event.h>

PXActionResult PXClientConnectToServer(PXClient* client, const char* ip, unsigned short port, const void* threadObject, const ThreadFunction threadFunction)
{
    IPAdressFamily ipAdressFamily = IPAdressFamilyUnspecified;
    PXSocketType socketType = PXSocketTypeStream;
    ProtocolMode protocolMode = ProtocolModeTCP;

    PXSocket PXSocketList[5];
    PXSize PXSocketListSizeMax = 5;
    PXSize PXSocketListSize = 0;

    MemoryClear(PXSocketList,sizeof(PXSocket)* PXSocketListSizeMax);

    PXSocketSetupAdress
    (
        PXSocketList,
        PXSocketListSizeMax,
        &PXSocketListSize,
        ip,
        port,
        ipAdressFamily,
        socketType,
        protocolMode
    );

    PXBool wasSucessful = 0;

    for (PXSize i = 0; i < PXSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &PXSocketList[i];
        const PXActionResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);
        const PXBool creationSuccesful = PXActionSuccessful == socketCreateResult;

        if (creationSuccesful)
        {
            const PXActionResult connectResult = PXSocketConnect(pxSocket);
            const PXBool connected = PXActionSuccessful == connectResult;

            if (connected)
            {  
                InvokeEvent(pxSocket->ConnectionEstablishedCallback, pxSocket);

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