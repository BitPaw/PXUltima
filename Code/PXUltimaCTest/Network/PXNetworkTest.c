#include "PXNetworkTest.h"

#include <PX/OS/Network/PXNetwork.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>

void PXTestNetworkAll()
{
    PXConsoleWrite(0, "[i] Network testing start\n");

    //PXTestNetworkClient();
    PXTestNetworkServer();

    PXConsoleWrite(0, "[i] Network testing done\n");
}

void PXTestNetworkServer()
{
    PXNetwork pxNetwork;
    PXSocket pxServer;
    PXSocket pxClient;

    PXNetworkInitialize(&pxNetwork);


    // Create server
    {
        PXSocketCreateInfo pxSocketCreateInfo;
        pxSocketCreateInfo.SocketReference = &pxServer;
        pxSocketCreateInfo.AdressFamily = IPAdressFamilyINET;
        pxSocketCreateInfo.Type = PXSocketTypeStream;
        pxSocketCreateInfo.ProtocolMode = PXProtocolModeTCP;

        PXNetworkSocketCreate(&pxNetwork, &pxSocketCreateInfo);
    }



    // Bind Server
    {
        PXSocketBindInfo pxSocketBindInfo;
        pxSocketBindInfo.SocketReference = &pxServer;
        pxSocketBindInfo.Port = 25565;
        pxSocketBindInfo.IP = PXNull;
        //pxSocketBindInfo.AdressInfo = ;
        //pxSocketBindInfo.AdressInfoSize;

        PXNetworkSocketBind(&pxNetwork, &pxSocketBindInfo);
    }


    // Listen server
    {
        PXSocketListenInfo pxSocketListenInfo;
        pxSocketListenInfo.SocketReference = &pxServer;
        pxSocketListenInfo.ClientInQueueMaximal = 10;

        PXNetworkSocketListen(&pxNetwork, &pxSocketListenInfo);
    }

    while(1)
    {
        {
            PXSocketAcceptInfo pxSocketAcceptInfo;
            pxSocketAcceptInfo.SocketServerReference = &pxServer;
            pxSocketAcceptInfo.SocketClientReference = &pxClient;

            PXNetworkSocketAccept(&pxNetwork, &pxSocketAcceptInfo);
        }

        char messageBuffer[128];

        while(1)
        {
            PXSocketReadInfo pxSocketReadInfo;
            PXClear(PXSocketReadInfo, &pxSocketReadInfo);
            pxSocketReadInfo.SocketReciverReference = &pxServer;
            pxSocketReadInfo.SocketSenderReference = &pxClient;
            pxSocketReadInfo.DataInfo.Buffer = messageBuffer;
            pxSocketReadInfo.DataInfo.BufferSize = 128;
            pxSocketReadInfo.DataInfo.BufferOffset = 0;
            pxSocketReadInfo.DataInfo.SegmentSize = 128;

            PXNetworkSocketReceive(&pxNetwork, &pxSocketReadInfo);

            if(pxSocketReadInfo.DataInfo.SocketDestroyed)
            {
                break;
            }
        }
    }    

    PXNetworkRelease(&pxNetwork);
}

void PXTestNetworkClient()
{
    PXNetwork pxNetwork;
    PXSocket pxClient;
    PXSocket pxServer;


    PXNetworkInitialize(&pxNetwork);


    // Create client
    {
        PXSocketCreateInfo pxSocketCreateInfo;
        PXClear(PXSocketCreateInfo, &pxSocketCreateInfo);
        pxSocketCreateInfo.SocketReference = &pxClient;
        pxSocketCreateInfo.AdressFamily = IPAdressFamilyINET;
        pxSocketCreateInfo.Type = PXSocketTypeStream;
        pxSocketCreateInfo.ProtocolMode = PXProtocolModeTCP;

        PXNetworkSocketCreate(&pxNetwork, &pxSocketCreateInfo);
    }



    // Connect
    {
        PXSocketConnectInfo pxSocketConnectInfo;
        PXClear(PXSocketConnectInfo, &pxSocketConnectInfo);
        pxSocketConnectInfo.SocketReference = &pxClient;
        pxSocketConnectInfo.Port = 25565;
        pxSocketConnectInfo.AdressFamily = IPAdressFamilyINET;
        pxSocketConnectInfo.Type = PXSocketTypeStream;
        pxSocketConnectInfo.ProtocolMode = PXProtocolModeTCP;

        PXNetworkSocketConnect(&pxNetwork, &pxSocketConnectInfo);
    }    

    while(1)
    {
        char buffer[64];

        PXSocketReadInfo pxSocketReadInfo;
        PXClear(PXSocketReadInfo, &pxSocketReadInfo);
        pxSocketReadInfo.DataInfo.Buffer = buffer;
        pxSocketReadInfo.DataInfo.BufferSize = 64;
        pxSocketReadInfo.SocketSenderReference = &pxClient;

        PXNetworkSocketReceive(&pxNetwork, &pxSocketReadInfo);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Network",
            "Socket-Connect",
            "\n"          
        );

        for(size_t i = 0; i < pxSocketReadInfo.DataInfo.BufferOffset; i++)
        {
            char data = ((char*)pxSocketReadInfo.DataInfo.Buffer)[i];

            PXConsoleWrite(1, &data);
        }

        PXConsoleWrite(1, "\n");
#endif
    }

    while(1)
    {
        char messge[] = "Hello, this is a test messsage!";
        const int size = sizeof(messge);

        while(1)
        {
            PXSocketSendInfo pxSocketSendInfo;
            PXClear(PXSocketSendInfo, &pxSocketSendInfo);
            pxSocketSendInfo.SocketRecieverReference = &pxClient;
            pxSocketSendInfo.SocketSenderReference = PXNull;
            pxSocketSendInfo.DataInfo.Buffer = messge;
            pxSocketSendInfo.DataInfo.BufferSize = size;
            pxSocketSendInfo.DataInfo.BufferOffset = 0;
            pxSocketSendInfo.DataInfo.SegmentSize = 2;
            pxSocketSendInfo.DataInfo.SegmentDelay = 1000;

            PXNetworkSocketSend(&pxNetwork, &pxSocketSendInfo);

            if(pxSocketSendInfo.DataInfo.SocketDestroyed)
            {
                break;
            }        
        }
    }

    PXNetworkRelease(&pxNetwork);
}