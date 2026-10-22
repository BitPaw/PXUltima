#include "PXClient.h"

#include <PX/OS/Memory/PXMemory.h>

void PXAPI PXClientConstruct(PXClient PXREF pxClient)
{
    PXClear(PXClient, pxClient);

   // PXSocketConstruct(&pxClient->SocketServer);
   // PXSocketConstruct(&pxClient->SocketClient);
}

void PXAPI PXClientDestruct(PXClient PXREF pxClient)
{
   // PXSocketDestruct(&pxClient->SocketServer);
   // PXSocketDestruct(&pxClient->SocketClient);

   // PXClientConstruct(pxClient);
}

PXResult PXAPI PXClientSendData(PXClient PXREF pxClient, const void PXREF data, const PXSize dataSize)
{
    if(pxClient->SocketClient.ID == PXSocketUnused)
    {
        return 0;
    }

   // PXBufferConstruct(&pxClient->SocketClient.BufferOutput, (void*)data, dataSize, PXBufferExtern);

    const PXResult sendResult = PXSocketSend(&pxClient->SocketClient, pxClient->SocketClient.ID);

    //PXBufferDestruct(&pxClient->SocketClient.BufferOutput);

    return sendResult;
}

PXResult PXAPI PXClientConnectToSelf(PXClient PXREF client, const PXI16U port)
{
    PXText ip;
    PXTextMakeFixedA(&ip, "127.0.0.1");

    return PXClientConnectToServer(client, &ip, port);
}

PXResult PXAPI PXClientConnectToServer(PXClient PXREF client, const PXText PXREF ip, const PXI16U port)
{
    PXSocket pxSocketList[3];
    const PXSize pxSocketListSizeMax = 3;
    PXSize socketListSize = 0;

    const int x = sizeof(PXSocket) * pxSocketListSizeMax;

    PXMemoryClear(pxSocketList, sizeof(PXSocket) * pxSocketListSizeMax);


    PXSocketConnectInfo pxSocketConnectInfo;
    PXClear(PXSocketConnectInfo, &pxSocketConnectInfo);
    pxSocketConnectInfo.IP = *ip;
    pxSocketConnectInfo.Port = port;
    pxSocketConnectInfo.AddressFamily = IPAddressFamilyUnspecified;
    pxSocketConnectInfo.Type = PXSocketTypeStream;
    pxSocketConnectInfo.ProtocolMode = PXProtocolModeTCP;

    PXSocketConnect(client->Server, &pxSocketConnectInfo);

    client->SocketServer.EventList = client->EventList;
    client->SocketServer.Owner = client->Owner; 

    return PXResultOK;
}

PXResult PXAPI PXClientDisconnectFromServer(PXClient PXREF client)
{
    PXSocketClose(&client->SocketClient);
  //  PXSocketConstruct(&client->SocketServer);

    return PXResultOK;;
}