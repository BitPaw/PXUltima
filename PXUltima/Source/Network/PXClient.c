#include "PXClient.h"

#include <Memory/PXMemory.h>
#include <Event/Event.h>

ActionResult PXClientConnectToServer(PXClient* client, const char* ip, unsigned short port, const void* threadObject, const ThreadFunction threadFunction)
{
    IPAdressFamily ipAdressFamily = IPAdressFamilyUnspecified;
    PXSocketType socketType = PXSocketTypeStream;
    ProtocolMode protocolMode = ProtocolModeTCP;

    PXSocket PXSocketList[5];
    PXSize PXSocketListSizeMax = 5;
    PXSize PXSocketListSize = 0;

    MemorySet(PXSocketList,sizeof(PXSocket)* PXSocketListSizeMax, 0);

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

    unsigned char wasSucessful = 0;

    for (PXSize i = 0; i < PXSocketListSize; ++i)
    {
        PXSocket* const pxSocket = &PXSocketList[i];
        const ActionResult socketCreateResult = PXSocketCreate(pxSocket, pxSocket->Family, pxSocket->Type, pxSocket->Protocol);
        const unsigned char creationSuccesful = ActionSuccessful == socketCreateResult;

        if (creationSuccesful)
        {
            const ActionResult connectResult = PXSocketConnect(pxSocket);
            const unsigned char connected = ActionSuccessful == connectResult;

            if (connected)
            {  
                InvokeEvent(pxSocket->ConnectionEstablishedCallback, pxSocket);

                const ActionResult ActionResult = PXThreadRun(&pxSocket->CommunicationThread, threadFunction, threadObject);

                wasSucessful = 1u;
                break; // Connect only once. If this is not here, we would connect more than once (with different protocol)
            }
        }
    }

    if (!wasSucessful)
    {
        return SocketConnectionFailure;
    }

    return ActionSuccessful;
}

#define PXClientBufferSize 2048u

PXThreadResult CommunicationFunctionAsync(void* PXSocketAdress)
{
    PXSocket* const pxSocket = (PXSocket*)PXSocketAdress;

    while (PXSocketIsCurrentlyUsed(pxSocket))
    {
        const PXSize bufferSizeMax = 2048;
        PXSize bufferSize = 0;
        unsigned char buffer[PXClientBufferSize];

        MemorySet(buffer, sizeof(unsigned char) * PXClientBufferSize, 0);

        const ActionResult receiveingResult = PXSocketReceive(pxSocket, buffer, bufferSizeMax, &bufferSize);
        const unsigned char sucessful = ActionSuccessful == receiveingResult;

        if (!sucessful)
        {
            PXSocketDestruct(pxSocket);
        }
    }

    return 0;
}