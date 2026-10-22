#include "PXSocket.h"

#include <PX/Type/PXText.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/PXOS.h>
#include <stdio.h>

#if OSUnix
#include <sys/select.h>
#include <poll.h>
#define OSSocketPoll poll
#elif OSWindows
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <mstcpip.h> // Required for TCP_INFO_v0
#pragma comment(lib, "Ws2_32.lib")


// Windows 7+
#include <iphlpapi.h>
#include <tcpestats.h>
#pragma comment(lib, "iphlpapi.lib")



#define OSSocketPoll WSAPoll
#endif


// Do we need to attach
#define UseDirectCall 1 


const char PXSocketName[] = "Socket";
const PXI8U PXSocketNameLength = sizeof(PXSocketName);
const PXECSRegisterInfoStatic PXSocketRegisterInfoStatic =
{
    {sizeof(PXSocketName), sizeof(PXSocketName), PXSocketName, TextFormatASCII},
    sizeof(PXSocket),
    __alignof(PXSocket),
    PXECSTypeEntity,
    PXSocketCreate,
    PXSocketRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXSocketRegisterInfoDynamic;

void PXAPI PXSocketRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXSocketRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXSocketRegisterInfoDynamic;
}

PXResult PXAPI PXSocketCreate(PXSocket** pxSocketREF, PXSocketCreateInfo PXREF pxSocketCreateInfo)
{
    PXSocket* pxSocket = PXNull;

    if(!(pxSocketREF && pxSocketCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxSocketCreateInfo->Info.Static = &PXSocketRegisterInfoStatic;
    pxSocketCreateInfo->Info.Dynamic = &PXSocketRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxSocketREF, pxSocketCreateInfo);

    if(pxResult != PXResultOK)
    {
        return pxResult;
    }

    pxSocket = *pxSocketREF;
    pxSocket->ID = PXSocketUnused;

    //PXThreadConstruct(&pxSocket->CommunicationThread);
    //PXDictionaryConstruct(&pxSocket->SocketLookup, sizeof(PXSocketID), sizeof(PXSocket), PXDictionaryValueLocalityInternalEmbedded);


    int resultID = 0;

#if 0
    if(!_pxNetwork.SocketCreate)
    {
        return PXResultRefusedNotInitialized;
    }
#endif

    const int ipAddressFamilyID = PXIPAddressFamilyToID(pxSocketCreateInfo->IPMode);
    const int socketTypeID = PXSocketTypeToID(pxSocketCreateInfo->SocketType);
    const int protocolModeID = PXProtocolModeToID(pxSocketCreateInfo->ProtocolMode);

    // Create socket
    {
#if UseDirectCall
        pxSocket->ID = socket(ipAddressFamilyID, socketTypeID, protocolModeID);
#else
        pxSocket->ID = _pxNetwork.SocketCreate(ipAddressFamilyID, socketTypeID, protocolModeID);;
#endif

        const PXResult createResult = PXErrorCurrent(-1 != pxSocket->ID); // PXWindowsSocketAgentErrorFetch

        if(PXResultOK != createResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXSocketName,
                "Create",
                "PXID:<%4i> failed to create socket",
                pxSocket->Info.ID,
                pxSocket->ID
            );
#endif

            return createResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXSocketName,
            "Create",
            "PXID:<%4i> created new ID:<%i>",
            pxSocket->Info.ID,
            pxSocket->ID
        );
#endif
    }

    // Socket config
    {
        const int level = SOL_SOCKET;

        const int optionName =
#if OSUnix
            SO_REUSEADDR;      // Do not use SO_REUSEADDR, else the port can be hacked. SO_REUSEPORT
#elif OSWindows
            SO_EXCLUSIVEADDRUSE;
#endif
        char opval = 1;
        int optlen = sizeof(int);

#if UseDirectCall
        resultID = getsockopt(pxSocket->ID, level, optionName, &opval, &optlen);
#else
        resultID = _pxNetwork.SocketOptionSet(pxSocket->ID, level, optionName, &opval, &optlen);
#endif

        const PXBool sucessful = resultID == 0;
    }

    //pxSocket->Family = addressFamily;
    //pxSocket->Protocol = protocolMode;
    //pxSocket->Type = socketType;

#if OSWindows
    // make non blocking
    u_long ul = 0;

#if UseDirectCall
    resultID = ioctlsocket(pxSocket->ID, FIONBIO, &ul);
#else
    resultID = _pxNetwork.SocketControlIO(pxSocket->ID, FIONBIO, &ul);
#endif

#endif

    return pxResult;
}

PXResult PXAPI PXSocketRelease(PXSocket PXREF pxSocket)
{
    if(!pxSocket)
    {
        return PXResultRefusedParameterNull;
    }

    printf("[C] Socket destruct %i\n", pxSocket->ID);

    //PXSocketDestroyViaID(pxSocket->ID);

    PXSocketClose(pxSocket);
    //PXThreadDestruct(&pxSocket->CommunicationThread);
    //PXDictionaryDestruct(&pxSocket->SocketLookup);

    return PXResultOK;
}

PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket PXREF pxSocket)
{
    return pxSocket->ID != PXSocketUnused;
}

void PXAPI PXSocketStateChange(PXSocket PXREF pxSocket, const PXSocketState socketState)
{
    const PXSocketState oldState = pxSocket->State;

    pxSocket->State = socketState;

    PXFunctionInvoke(pxSocket->EventList.SocketStateChangedCallBack, pxSocket->Owner, pxSocket, oldState, socketState);
}

PXResult PXAPI PXSocketClientRemove(PXSocket PXREF serverSocket, const PXSocketID clientID)
{
    PXSocket clientSockket;

    // Extract client socket
    {
        const PXSocketID clientIDCopy = clientID;
        const PXBool success = 0;// PXDictionaryExtract(&serverSocket->SocketLookup, &clientID, &clientSockket);

        if(!success)
        {
            return PXResultFailedElementNotFound;
        }
    }

#if SocketDebug
    printf("[PXSocket] <%i> remove <%i>\n", (int)serverSocket->ID, (int)clientSockket.ID);
#endif

    PXSocketClose(&clientSockket);

    return PXResultOK;
}

PXResult PXAPI PXNetworkIPLocate(const PXIPAddress PXREF pxIPAddress)
{
    return PXResultRefusedNotImplemented;
}

typedef struct PXSocketAddressInfo_
{
#if OSUnix
#elif OSWindows

    union
    {
        struct
        {
            ADDRINFOA* AddressResultA;
            ADDRINFOA AddressInputA;
        };

        struct
        {
            ADDRINFOW* AddressResultW;
            ADDRINFOW AddressInputW;
        };
    };
  
#endif
}
PXSocketAddressInfo;

PXResult PXAPI PXSocketConnect(PXSocket PXREF pxSocket, PXSocketConnectInfo PXREF pxSocketConnectInfo)
{
    if(!(pxSocket && pxSocketConnectInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXI16U port = pxSocketConnectInfo->Port;
    char localIP[] = "127.0.0.1";
    PXText ipLocal;
    PXTextFromAddressA(&ipLocal, localIP, sizeof(localIP), sizeof(localIP));

    PXText* ipTarget = &pxSocketConnectInfo->IP;

    // If we dont have any IP selected, we set a default one
    if(!ipTarget)
    {
        ipTarget = &ipLocal;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Connect",
        "Connecting PXID:<%4i> SocketID:<%i> to <IP:%s, Port:%i>",
        pxSocket->Info.ID,
        pxSocket->ID,
        ipTarget->A,
        port
    );
#endif

    int resultID = -1;
    PXResult pxResult = PXResultInvalid;

    // Hostname

#if OSWindows

    PXSocketAddressInfo pxSocketAddressInfo;
    PXClear(PXSocketAddressInfo, &pxSocketAddressInfo);

    PXText portText;
    PXTextConstructNamedBufferA(&portText, portTextBuffer, 32);
    PXTextFromInt(&portText, port);

    pxSocketAddressInfo.AddressInputA.ai_family = PXIPAddressFamilyToID(pxSocketConnectInfo->AddressFamily);
    pxSocketAddressInfo.AddressInputA.ai_socktype = PXSocketTypeToID(pxSocketConnectInfo->Type);
    pxSocketAddressInfo.AddressInputA.ai_protocol = PXProtocolModeToID(pxSocketConnectInfo->ProtocolMode);
#if OSWindows
    pxSocketAddressInfo.AddressInputA.ai_flags = AI_CANONNAME;
#endif

    switch(ipTarget->Format)
    {
        case TextFormatASCII:
        {
            resultID = getaddrinfo
            (
                ipTarget->A,
                portText.A,
                &pxSocketAddressInfo.AddressInputA,
                &pxSocketAddressInfo.AddressResultA
            );

            for(ADDRINFOA* addressInfoA = pxSocketAddressInfo.AddressResultA; addressInfoA; addressInfoA = addressInfoA->ai_next)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXSocketName,
                    "Socket",
                    "Name: %s",
                    addressInfoA->ai_canonname
                );
#endif

#if UseDirectCall
                resultID = connect(pxSocket->ID, addressInfoA->ai_addr, addressInfoA->ai_addrlen);
#else
                resultID = _pxNetwork.SocketConnect(pxSocket->ID, addressInfoA->ai_addr, addressInfoA->ai_addrlen);
#endif
                pxResult = PXErrorCurrent(0 == resultID);

                if(PXResultOK == resultID)
                {
                    break; // CONNECT SUCCESS
                }
            }

            break;
        }
        case TextFormatUNICODE:
        {
            GetAddrInfoW
            (
                ipTarget->W,
                portText.W,
                &pxSocketAddressInfo.AddressInputW,
                &pxSocketAddressInfo.AddressResultW
            );

            for(ADDRINFOW* addressInfoW = pxSocketAddressInfo.AddressResultW; addressInfoW; addressInfoW = addressInfoW->ai_next)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXSocketName,
                    "Socket",
                    "Name: %ls",
                    addressInfoW->ai_canonname
                );
#endif


#if UseDirectCall
                resultID = connect(pxSocket->ID, addressInfoW->ai_addr, addressInfoW->ai_addrlen);
#else
                resultID = _pxNetwork.SocketConnect(pxSocket->ID, addressInfoW->ai_addr, addressInfoW->ai_addrlen);
#endif
                pxResult = PXErrorCurrent(0 == resultID);

                if(PXResultOK == resultID)
                {
                    break; // CONNECT SUCCESS
                }
            }

            break;
        }

        default:
            break;
    }


    if(PXResultOK != pxResult)
    {
        // Failed
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXSocketName,
            "Connect",
            "PXID:<%i> ID:<%i> Connect to server failed! <IP:%s, Port:%i>",
            pxSocket->Info.ID,
            pxSocket->ID,
            ipTarget->A,
            port
        );
#endif

        return pxResult;
    }



    PXSocketStatistics pxSocketStatistics;

    wchar_t pxTextBufferA[256];
    wchar_t pxTextBufferB[256];

    PXText pxTextNameSelf;
    PXText pxTextNamePeer;

    PXTextFromAddressW(&pxTextNameSelf, pxTextBufferA, sizeof(pxTextBufferA), sizeof(pxTextBufferA));
    PXTextFromAddressW(&pxTextNamePeer, pxTextBufferB, sizeof(pxTextBufferB), sizeof(pxTextBufferB));

    PXSocketNameSelfGet(pxSocket, &pxTextNameSelf);
    PXSocketNamePeerGet(pxSocket, &pxTextNamePeer);


    PXSocketStatisticsGet(pxSocket, &pxSocketStatistics);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Connect",
        "PXID:<%i> ID:<%i> Connected to server!\n"
        "%20s : %ls\n"
        "%20s : %ls\n"
        "%20s : Current:%4i Minimal:%4i\n"
        "%20s : %i\n"
        "%20s : %i",

        pxSocket->Info.ID,
        pxSocket->ID,

        "Self", pxTextNameSelf.W,
        "Peer", pxTextNamePeer.W,
        "RTT", pxSocketStatistics.RoundTripTimeCurrentUS, pxSocketStatistics.RoundTripTimeMinimalUS,
        "MSS", pxSocketStatistics.SegmentSizeMaximum,
        "MTU", pxSocketStatistics.TransmissionUnitMaximum
    );
#endif

#endif




#if 0
    // Use legacy
    // [Warning - Deprecated]: Can't understand and resolve IPv6 addresses.
    // Will only utilize and understand IPv4.
    // Use getaddrinfo() because it is protocol-independent.
    struct hostent* host = _pxNetwork.HostByNameGet(pxSocketConnectInfo->IP.Text);

    if(!host)
    {
        return PXResultInvalid;
    }

    PXMemoryCopy(host->h_addr_list, &server.sin_addr, host->h_length);
    server.sin_port = pxSocketConnectInfo->Port;
    serverAddressLength = sizeof(struct sockaddr_in);

    connectResultID = _pxNetwork.SocketConnect(pxSocket->ID, &server, serverAddressLength);
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketListen(PXSocket PXREF pxSocket, PXSocketListenInfo PXREF pxSocketListenInfo)
{
    if(!(pxSocket && pxSocketListenInfo))
    {
        return PXResultRefusedParameterNull;
    }

  //  if(!_pxNetwork.SocketListen)
  //  {
  //      return PXResultRefusedNotInitialized;
 //   }

#if UseDirectCall
    const int listeningResult = listen(pxSocket->ID, pxSocketListenInfo->ClientInQueueMaximal);
#else
    const int listeningResult = _pxNetwork.SocketListen(pxSocket->ID, pxSocketListenInfo->ClientInQueueMaximal);
#endif

    const PXResult listenResult = PXErrorCurrent(0 == listeningResult);

    if(PXResultOK != listenResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXSocketName,
            "Listen",
            "PXID:<%i> ID:<%i> Failure to listen",
            pxSocket->Info.ID,
            pxSocket->ID
        );
#endif

        return listenResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Listen",
        "PXID:<%i> ID:<%i> now listening...",
        pxSocket->Info.ID,
        pxSocket->ID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketAccept(PXSocket PXREF pxSocket, PXSocketAcceptInfo PXREF pxSocketAcceptInfo)
{
    if(!(pxSocket && pxSocketAcceptInfo))
    {
        return PXResultRefusedParameterNull;
    }

#if 0
    if(!_pxNetwork.SocketAccept)
    {
        return PXResultRefusedNotInitialized;
    }
#endif

    PXSocket PXREF pxSocketClient = pxSocketAcceptInfo->SocketClientReference;

    PXClear(PXSocket, pxSocketClient);


    union
    {
        struct sockaddr SocketAddressInfo;
        struct sockaddr_in SocketAddressInfoIPv4;
        struct sockaddr_in6 SocketAddressInfoIPv6;
    }
    socketAddressInfo;


    int socketAddressInfoSize = sizeof(socketAddressInfo);
    PXClear(struct sockaddr, &socketAddressInfo);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Accept",
        "PXID:<%i> ID:<%i> wait for client...",
        pxSocket->Info.ID,
        pxSocket->ID
    );
#endif


#if UseDirectCall
    pxSocketClient->ID = accept(pxSocket->ID, &socketAddressInfo.SocketAddressInfo, &socketAddressInfoSize);
#else
    pxSocketClient->ID = _pxNetwork.SocketAccept(pxSocket->ID, &socketAddressInfo.SocketAddressInfo, &socketAddressInfoSize);
#endif


    const PXResult acceptResult = PXErrorCurrent(-1 != pxSocketClient->ID);

    if(PXResultOK != acceptResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXSocketName,
            "Accept",
            "PXID:<%i> ID:<%i> failed client accept",
            pxSocket->Info.ID,
            pxSocket->ID
        );
#endif

        return acceptResult;
    }

    char ipname[128];
    DWORD ipNameLength = 128;

    char hostname[300];
    char servInfo[NI_MAXSERV];


    // We have a socket now, get info
    {
        switch(socketAddressInfo.SocketAddressInfo.sa_family) // Depending on the protocol, we interpret the struct differently
        {
            // IPv4
            case AF_INET:
            {
                struct sockaddr_in* socketAddressInfoIPv4 = &socketAddressInfo.SocketAddressInfoIPv4;

                // const char* ipv4 = inet_ntoa(socketAddressInfoIPv4->sin_addr);
                // PXTextCopyA(ipv4, 128, ipname, 128);

#if OSWindows
                const INT errorID = WSAAddressToStringA
                (
                    &socketAddressInfo.SocketAddressInfoIPv4,
                    socketAddressInfoSize,
                    PXNull,
                    ipname,
                    &ipNameLength
                );
#endif

                break;
            }
            // IPv6
            case AF_INET6:
            {
                struct sockaddr_in6* socketAddressInfoIPv6 = &socketAddressInfo.SocketAddressInfoIPv6;

                // Copy data and set data to format

#if OSWindows
                const INT errorID = WSAAddressToStringA
                (
                    &socketAddressInfo.SocketAddressInfoIPv6,
                    socketAddressInfoSize,
                    PXNull,
                    ipname,
                    &ipNameLength
                );
#endif

                break;
            }
            default:
                break;
        }

        // Reverse DNS search for connected peer, does this work? On clients this should not quite work well.

        const int dwRetval = getnameinfo
        (
            &socketAddressInfo.SocketAddressInfo,
            socketAddressInfoSize,
            hostname,
            300,
            servInfo,
            NI_MAXSERV,
            0
        );
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Accept",
        "PXID:<%i> ID:<%i> accepted client <%i> @ %s:%s",
        pxSocket->Info.ID,
        pxSocket->ID,
        (int)pxSocketClient->ID,
        hostname,
        servInfo
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketBind(PXSocket PXREF pxSocket, PXSocketBindInfo PXREF pxSocketBindInfo)
{
    if(!(pxSocket && pxSocketBindInfo))
    {
        return PXResultRefusedParameterNull;
    }

    struct sockaddr_in bindAddressInfo;
    PXClear(struct sockaddr, &bindAddressInfo);
    bindAddressInfo.sin_family = AF_INET;

#if UseDirectCall
    bindAddressInfo.sin_addr.s_addr = htonl(pxSocketBindInfo->IP->A); // INADDR_ANY
    bindAddressInfo.sin_port = htonl(pxSocketBindInfo->Port);
#else
    bindAddressInfo.sin_addr.s_addr = _pxNetwork.Converthtonl(pxSocketBindInfo->IP); // INADDR_ANY
    bindAddressInfo.sin_port = _pxNetwork.Converthtons(pxSocketBindInfo->Port);
#endif

    const int bindAddressInfoSize = sizeof(struct sockaddr_in);


#if UseDirectCall
    const int bindingResultID = bind(pxSocket->ID, &bindAddressInfo, bindAddressInfoSize);
#else
    const int bindingResultID = _pxNetwork.SocketBind(pxSocket->ID, &bindAddressInfo, bindAddressInfoSize);
#endif

    const PXResult bindingResult = PXErrorCurrent(-1 != bindingResultID);

    if(PXResultOK != bindingResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXSocketName,
            "Bind",
            "PXID:<%i> ID:<%i> bind failed for <%s, %i>",
            pxSocket->Info.ID,
            pxSocket->ID,
            pxSocketBindInfo->IP,
            pxSocketBindInfo->Port
        );
#endif

        return bindingResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Bind",
        "PXID:<%i> ID:<%i> bond to <%s, %i>",
        pxSocket->Info.ID,
        pxSocket->ID,
        pxSocketBindInfo->IP,
        pxSocketBindInfo->Port
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketReceive(PXSocket PXREF pxSocket, PXSocketDataInfo PXREF pxSocketDataInfo)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxSocketDataInfo))
    {
        return PXResultRefusedParameterNull;
    }

    char* data = (char*)PXBufferData(&pxSocketDataInfo->Buffer);
    int length = PXBufferRemaining(&pxSocketDataInfo->Buffer) + 1;

    int readResult = 0;

    struct sockaddr_in sockAddressInfo;
    int sockAddressInfoLength = sizeof(sockAddressInfo);

    // Get type of socket
    int socketType;
    socklen_t len = sizeof(socketType);

    const int typeFetch = getsockopt(pxSocket->ID, SOL_SOCKET, SO_TYPE, &socketType, &len);
    const PXBool isUDP = SOCK_DGRAM == socketType;

    if(isUDP) // No Connection
    {
        // UDP socket
        readResult = recvfrom
        (
            pxSocket->ID,
            data,
            length,
            0,
            &sockAddressInfo,
            &sockAddressInfoLength
        );
    }
    else // Contection
    {
        // TCP socket
#if OSUnix
        readResult = read(socketID, data, length);
#elif OSWindows
            //_pxNetwork.SocketRecive(socketID, data, availableSize, 0); // MSG_PEEK

        readResult = recv(pxSocket->ID, data, length, 0);
#endif
    }

    // Handle...
    switch(readResult)
    {
        case 0:
        {
            pxResult = PXResultRefusedConnectionClosed;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXSocketName,
                "Receive",
                "PXID:<%i> ID:<%i> close signal detected!",
                pxSocket->Info.ID,
                pxSocket->ID
            );
#endif

            PXSocketClose(pxSocket);

            break;
        }
#if OSUnix
        case SO_ERROR:
#elif OSWindows
        case SOCKET_ERROR:
#endif // OSUnix#

        {
            pxResult = PXErrorCurrent(0);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXSocketName,
                "Receive",
                "PXID:<%i> ID:<%i> Read failure",
                pxSocket->Info.ID,
                pxSocket->ID
            );
#endif

            break;
        }
        default:
        {
            // We already inserted the data, we now just move the cursor where we should be already
            PXBufferCursorMove(&pxSocketDataInfo->Buffer, readResult);

            // Note, we can add a terminating char here, but this corrupts data, possibly

            // because we are connectionless, we need to
            // translate the socket we are connected to

            PXSocket PXREF pxSocketPeer = pxSocketDataInfo->SocketPeer;

            PXBool fetchExtendedData = isUDP && pxSocketPeer;

            if(fetchExtendedData)
            {
                pxSocket->Port = ntohs(sockAddressInfo.sin_port);

                inet_ntop
                (
                    sockAddressInfo.sin_family,
                    &sockAddressInfo.sin_addr,
                    pxSocket->IP,
                    sizeof(pxSocket->IP)
                );

                getnameinfo
                (
                    (struct sockaddr*)&sockAddressInfo,
                    sockAddressInfoLength,
                    pxSocket->Name,
                    sizeof(pxSocket->Name),
                    NULL,
                    0,
                    0
                );
            }

#if PXLogEnable
            char printableText[64];

            PXTextFromNonTerminated(printableText, 64, pxSocketDataInfo->Buffer.TextA, readResult);

            PXLogPrint
            (
                PXLoggingInfo,
                PXSocketName,
                "Receive",
                "PXID:<%i> ID:<%i> <-- PXID:<%i> ID:<%i> %20s:%-5i - %-15s - %4i B, %s",
                pxSocket->Info.ID,
                pxSocket->ID,
                pxSocketPeer->Info.ID,
                pxSocketPeer->ID,
                0,
                pxSocket->IP,
                pxSocket->Port,
                pxSocket->Name,
                readResult,
                printableText
            );
#endif


            break;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXSocketSend(PXSocket PXREF pxSocket, PXSocketDataInfo PXREF pxSocketDataInfo)
{
#if 0
    PXSocket PXREF pxSocketReciever = pxSocketSendInfo->SocketRecieverReference;
    PXSocket PXREF pxSocketSender = pxSocketSendInfo->SocketSenderReference;

#if 0
    // Check if socket is active and ready to send
    {
        const PXBool isSenderReady = PXSocketIsCurrentlyUsed(pxSocketSender);

        if(!isSenderReady)
        {
            return PXResultRefusedObjectNotReady;
        }

        const PXBool isReceiverReady = pxSocketReceiverID != -1;

        if(!isReceiverReady)
        {
            return PXResultRefusedSocketNotConnected;
        }
    }

    // Do we even send anything? If not, quit
    {
        if(!pxSocketSender->BufferOutput.Data)
        {
            return PXResultRefuedOutputBufferNull;
        }

        const PXBool hasDataToSend = pxSocketSender->BufferOutput.SizeMaximum > 0;

        if(!hasDataToSend)
        {
            return PXResultDidNothing; // Do not send anything if the message is empty
        }
    }
#endif

    // Send data
    PXBool done = PXFalse;

    do
    {
        const char* dataAddress = (PXByte*)pxSocketSendInfo->DataInfo.Buffer + pxSocketSendInfo->DataInfo.BufferOffset;

        int dataSize = pxSocketSendInfo->DataInfo.BufferSize - pxSocketSendInfo->DataInfo.BufferOffset;

        dataSize = PXMathMinimumI(dataSize, pxSocketSendInfo->DataInfo.SegmentSize);


        const int writtenBytes =
#if OSUnix
            write(pxSocketReciever->ID, dataAddress, dataSize);
#elif OSWindows
            _pxNetwork.SocketSend(pxSocketReciever->ID, dataAddress, dataSize, 0);
#endif

        const PXResult sendResult = PXErrorCurrent(-1 != writtenBytes);

        if(PXResultOK != sendResult)
        {
            return sendResult;
        }

        pxSocketSendInfo->DataInfo.MovedCurrent = writtenBytes;
        pxSocketSendInfo->DataInfo.MovedTotal += writtenBytes;
        pxSocketSendInfo->DataInfo.BufferOffset += writtenBytes;
        pxSocketSendInfo->DataInfo.Percentage = ((PXF32)pxSocketSendInfo->DataInfo.BufferOffset / (PXF32)pxSocketSendInfo->DataInfo.BufferSize) * 100.0f;

        if(pxSocketSendInfo->DataInfo.Percentage < 100 && pxSocketSendInfo->DataInfo.SegmentDelay > 0)
        {
            PXThreadSleep(PXNull, pxSocketSendInfo->DataInfo.SegmentDelay);
        }

#if 0
        {
            PXSocketDataSendEventData pxSocketDataSendEventData;
            pxSocketDataSendEventData.SocketSending = pxSocketSender;
            pxSocketDataSendEventData.SocketReceiving = pxSocketReceiverID;
            pxSocketDataSendEventData.Data = dataAddress;
            pxSocketDataSendEventData.DataSize = writtenBytes;

            PXFunctionInvoke(pxSocketSender->EventList.SocketDataSendCallBack, pxSocketSender->Owner, &pxSocketDataSendEventData);
        }
#endif



#if PXLogEnable
        PXText pxTextTotal;
        PXText pxTextTotalGain;

        PXTextConstructNamedBufferA(&pxTextTotal, pxTextTotalBuffer, 32);
        PXTextConstructNamedBufferA(&pxTextTotalGain, pxTextTotalGainBuffer, 32);

        PXTextFormatSize(&pxTextTotalGain, pxSocketSendInfo->DataInfo.MovedCurrent);
        PXTextFormatSize(&pxTextTotal, pxSocketSendInfo->DataInfo.MovedTotal);

        if(pxSocketSender)
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Network",
                "Socket-Send",
                "<%i> --> <%i> %-3i%% (+ %s), %s",
                pxSocketSender->ID,
                pxSocketReciever->ID,
                pxSocketSendInfo->DataInfo.Percentage,
                pxTextTotalGain.A,
                pxTextTotal.A
            );
        }
        else
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Network",
                "Socket-Send",
                "<%i> --> Server %3i%% (+ %s), %s",
                pxSocketReciever->ID,
                pxSocketSendInfo->DataInfo.Percentage,
                pxTextTotalGain.A,
                pxTextTotal.A
            );
        }

#endif

        // Done with sending?
        done = pxSocketSendInfo->DataInfo.BufferOffset >= pxSocketSendInfo->DataInfo.BufferSize;
    } while(!done);

#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketPoll(PXSocket PXREF pxSocket)
{
#if 0
    /*
    PXSocketStateChange(pxSocket, SocketEventPolling);

    if(pxSocket->PollingLock)
    {
        PXLockEngage(pxSocket->PollingLock);
    }*/


#if 0 // Use optimised OS function

    const PXSize socketDataListSize = pxSocket->SocketPollingReadListSize;
    struct pollfd* socketDataList = buffer;
    int timeout = 50;

    for(PXSize i = 0; i < socketDataListSize; ++i)
    {
        struct pollfd* pollingEntry = &socketDataList[i];

        pollingEntry->fd = pxSocket->SocketPollingReadList[i]; // SocketID
        pollingEntry->events = POLLRDNORM; // Poll-Read-Normal
        pollingEntry->revents = 0; // Will be the result of the call
    }


    int amount = pxNetwork->SocketPoll(socketDataList, socketDataListSize, timeout); // Not thread safe??
    const PXBool success = amount != -1;


    if(!success)
    {
        amount = 0;
#if 0

        PXResultOnErrorFetchAndExit(!success);
#endif
    }






    for(PXSize i = 0; i < amount; i++)
    {
        const struct pollfd PXREF currentPollData = &socketDataList[i];

        switch(currentPollData->revents)
        {
            case 0: // Illegal state, undefined
            {
                const PXSocketID clientID = currentPollData->fd;
                const PXBool isRegistered = PXSocketIsRegistered(pxSocket, clientID);

                if(isRegistered)
                {
                    PXSocket clientSocket;
                    PXSocketConstruct(&clientSocket);

                    clientSocket.Owner = pxSocket->Owner;
                    clientSocket.ID = clientID;
                    clientSocket.EventList = pxSocket->EventList;

                    char inputBuffer[1024];
                    PXSize wrrit = 0;

                    const PXResult succ = PXSocketReceive(&clientSocket, inputBuffer, 1024, &wrrit);

                    if(succ != PXThreadSucessful)
                    {
                        PXSocketEventReadUnregister(pxSocket, currentPollData->fd);
                        PXSocketClose(&clientSocket);
                    }

                }
                else
                {
                    PXSocketEventReadRegister(pxSocket, clientID);
                }

                break;
            }
            case POLLERR: // An error has occurred.
            case POLLHUP: // A stream - oriented connection was either disconnected or aborted.
            {
                PXSocketEventReadUnregister(pxSocket, currentPollData->fd);
                break;
            }
            case POLLNVAL: // An invalid socket was used.
            case POLLPRI: // Priority data may be read without blocking.This flag is not returned by the Microsoft Winsock provider.
            case POLLRDBAND: // Priority band(out - of - band) data may be read without blocking.
            case POLLRDNORM: // Normal data may be read without blocking.
            {
                PXSocketReadPendingHandle(pxSocket, currentPollData->fd);
                break;
            }
            case POLLWRNORM: // Normal data may be written without blocking.

            default:
                break;
        }
    }

    if(pxSocket->PollingLock)
    {
        PXLockRelease(&pxSocket->PollingLock);
    }

#elif 1

    // FD_SETSIZE is stanard value 64

    const PXSize registeredSocketIDs = pxSocket->SocketLookup.EntryAmountCurrent;
    const PXSize neededFetches = (registeredSocketIDs / FD_SETSIZE) + 1;

    //const TIMEVAL time = { 3,0 };

    PXSize restValues = registeredSocketIDs;
    PXSize socketIDIndex = 0;

    for(PXSize i = 0; i < neededFetches; ++i) // All sockets
    {
        fd_set selectListenRead; // is a struct containing an int and a fixed list of int[64]
        PXSize j = 0;
        PXSize highestSocketID = 0;

        PXMemoryClear(&selectListenRead, sizeof(selectListenRead));

        for(; restValues > 0 && j < FD_SETSIZE; ++j) // all in 64x blocks
        {
            const PXSize index = i * FD_SETSIZE + j;

            PXDictionaryEntry pxDictionaryEntry;
            PXDictionaryIndex(&pxSocket->SocketLookup, socketIDIndex++, &pxDictionaryEntry);

            const PXSocketID socketID = *(PXSocketID*)pxDictionaryEntry.Key;

            if(socketID == (PXSocketID)-1)
            {
                --j;
                continue;
            }

            highestSocketID = PXMathMaximumIU(highestSocketID, socketID);

#if OSUnix
#elif OSWindows
            selectListenRead.fd_array[j] = socketID;
#endif

            --restValues;
        }

#if OSUnix
#elif OSWindows
        selectListenRead.fd_count = j;
#endif

        int numberOfSocketEvents = pxNetwork->SocketSelect(highestSocketID + 1, &selectListenRead, 0, 0, 0); // first parameter is ignored under windows
        const PXBool success = -1 != numberOfSocketEvents;

        if(!success)
        {
#if OSUnix
            PXResult selectResult = PXErrorCurrent();
#elif OSWindows
            PXResult selectResult = PXWindowsSocketAgentErrorFetch();
#endif

            numberOfSocketEvents = 0;
        }

        //PXResultOnErrorFetchAndExit(!success);

        for(PXSize readableSocketIndex = 0; readableSocketIndex < numberOfSocketEvents; ++readableSocketIndex)
        {
#if OSUnix
            const PXSocketID socketID = 0;
#elif OSWindows
            const PXSocketID socketID = selectListenRead.fd_array[readableSocketIndex];
#endif
            const PXBool isServer = pxSocket->ID == socketID;

            if(isServer)
            {
                PXSocketAccept(pxSocket);
            }
            else
            {
                PXSocketReceive(pxSocket, socketID);
            }
        }
    }

#else

    for(PXSize i = 0; i < pxSocket->SocketPollingReadListSize; ++i)
    {
        PXSocketReadPendingHandle(pxSocket, pxSocket->SocketPollingReadList[i]); // Will block
    }

#endif

    if(pxSocket->PollingLock)
    {
        PXLockRelease(pxSocket->PollingLock);
    }

    PXSocketStateChange(pxSocket, SocketIDLE);
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketClose(PXSocket PXREF pxSocket)
{
    if(!pxSocket)
    {
        return PXResultRefusedParameterNull;
    }

    int resultID = 0;
    PXResult pxResult = PXResultInvalid;


#if OSUnix
    resultID = close(pxSocket->ID);
#elif OSWindows

#if UseDirectCall
    resultID = shutdown(pxSocket->ID, SD_SEND);
    pxResult = PXErrorCurrent(0 == resultID);

    resultID = closesocket(pxSocket->ID);
    pxResult = PXErrorCurrent(0 == resultID);

#else
    _pxNetwork.SocketShutdown(pxSocketID, SD_SEND);
    _pxNetwork.SocketClose(pxSocketID);
#endif

#endif

    // PXFunctionInvoke(pxSocket->EventList.SocketClosedCallBack, pxSocket->Owner, pxSocket);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSocketName,
        "Close",
        "PXID:<%i> ID:<%i> closed",
        pxSocket->Info.ID,
        pxSocket->ID
    );
#endif

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxSocket->ID = -1;

    return pxResult;
}

PXResult PXAPI PXSocketNameSelfGet(const PXSocket PXREF pxSocket, PXText PXREF pxText)
{
    PXResult pxResult = PXResultInvalid;
    DWORD lengthText = pxText->SizeAllocated; 

    int lengthStorage = sizeof(SOCKADDR_STORAGE);
    SOCKADDR_STORAGE socketAddress = {0};

    int resultID = getsockname(pxSocket->ID, (struct sockaddr*)&socketAddress, &lengthStorage);
    pxResult = PXErrorCurrent(0 == resultID);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    resultID = WSAAddressToStringW
    (
        (LPSOCKADDR)&socketAddress,
        lengthStorage,
        0,
        pxText->W,
        &lengthText
    );
    pxResult = PXErrorCurrent(0 < resultID);

    pxText->Format = TextFormatUNICODE;
    pxText->SizeUsed = lengthText;
}

PXResult PXAPI PXSocketNamePeerGet(const PXSocket PXREF pxSocket, PXText PXREF pxText)
{
    if(!(pxSocket && pxText))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    DWORD lengthText = pxText->SizeAllocated;

    int lengthStorage = sizeof(SOCKADDR_STORAGE);
    SOCKADDR_STORAGE socketAddress = { 0 };

    int resultID = getpeername(pxSocket->ID, (struct sockaddr*)&socketAddress, &lengthStorage);
    pxResult = PXErrorCurrent(0 == resultID);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    resultID = WSAAddressToStringW
    (
        (LPSOCKADDR)&socketAddress, 
        lengthStorage,
        0, 
        pxText->W,
        &lengthText
    );
    pxResult = PXErrorCurrent(0 < resultID);

    pxText->Format = TextFormatUNICODE;
    pxText->SizeUsed = lengthText;

    return PXResultOK;
}

PXI32U PXAPI PXSocketMTUGet(const PXSocket PXREF pxSocket)
{
#if OSUnix
    return 0;

#elif OSWindows
    DWORD val = 0;
    const INT errorID = WSAGetIPUserMtu(pxSocket->ID, &val); // Windows 10, Ws2_32.dll, ws2tcpip.h, WSAGetLastError

    return val;
#endif
}

PXResult PXAPI PXSocketMTUSet(const PXSocket PXREF pxSocket, PXI32U value)
{
#if OSUnix || OSForcePOSIXForWindows
    char nameBuffer[IFNAMSIZ];
    struct ifreq ifr;

    PXNetworkSocketInterfaceName(pxSocket, nameBuffer, IFNAMSIZ);

    strncpy(ifr.ifr_name, nameBuffer, IFNAMSIZ - 1);  // Replace "eth0" with your interface

    const int resultID = ioctl(pxSocket->ID, SIOCGIFMTU, &ifr); // sys/ioctl.h
    error = PXErrorCurrent(-1 != resultID);

#elif OSWindows
    const INT errorID = WSASetIPUserMtu(pxSocket->ID, value); // Windows 10, Ws2_32.dll, ws2tcpip.h, WSAGetLastError
#endif

    return PXResultOK;
}

PXResult PXAPI PXSocketStatisticsGet(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics)
{
    if(!(pxSocket && pxSocketStatistics))
    {
        return PXResultRefusedParameterNull;
    }

    return PXSocketStatisticsGet_W10(pxSocket, pxSocketStatistics);
}

PXResult PXAPI PXSocketStatisticsGet_W10(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics)
{
    PXClear(PXSocketStatistics, pxSocketStatistics);

    TCP_INFO_v0 tcpInfo;
    DWORD bytesReturned = 0;

    int ioctlResult = WSAIoctl
    (
        pxSocket->ID,
        SIO_TCP_INFO, // This is only available starting with windows 10!
        &tcpInfo, sizeof(tcpInfo),
        &tcpInfo, sizeof(tcpInfo),
        &bytesReturned,
        NULL,
        NULL
    ); // Windows NT 4.0, mstcpip.h

    if(0 != ioctlResult)
    {
        return PXResultInvalid;
    }

    pxSocketStatistics->RoundTripTimeCurrentUS = tcpInfo.RttUs;
    pxSocketStatistics->RoundTripTimeMinimalUS = tcpInfo.MinRttUs;
    pxSocketStatistics->SegmentSizeMaximum = tcpInfo.Mss;
    pxSocketStatistics->TransmissionUnitMaximum = PXSocketMTUGet(pxSocket);

    return PXResultOK;
}

PXResult PXAPI PXSocketStatisticsGet_W7(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics)
{
    PXI32U rrt = 0;

    // Older version
    // 1. Establish the lookup row identifiers for your socket
    MIB_TCPROW tcpRow;
    tcpRow.dwState = MIB_TCP_STATE_ESTAB;
    tcpRow.dwLocalAddr = 0;// localBinaryIp;   // From getsockname
    tcpRow.dwLocalPort = 0;//localBinaryPort;
    tcpRow.dwRemoteAddr = 0;//remoteBinaryIp; // From getpeername
    tcpRow.dwRemotePort = 0;//remoteBinaryPort;

    // 2. Enable statistics tracking on this connection row
    TCP_BOOLEAN_OPTIONAL enableStats = TcpBoolOptEnabled;
    SetPerTcpConnectionEStats
    (
        &tcpRow,
        TcpConnectionEstatsPath,
        (PUCHAR)&enableStats,
        0,
        sizeof(enableStats),
        0
    );

    // 3. Query the connection statistics (Path layout contains round trip metrics)
    TCP_ESTATS_PATH_ROD_v0 pathRod;
    ULONG status = GetPerTcpConnectionEStats
    (
        &tcpRow,
        TcpConnectionEstatsPath,
        NULL, 0, 0,              // Read-write data (not needed)
        NULL, 0, 0,              // Static data (not needed)
        (PUCHAR)&pathRod,
        0,
        sizeof(pathRod) // Dynamic data (where metrics live)
    ); // Windows 7, iphlpapi.dll, iphlpapi.h

    if(status != NO_ERROR)
    {
        return PXResultInvalid;
    }

    rrt = pathRod.SampleRtt; // in ms!

    return PXResultOK;

}

PXResult PXAPI PXSocketStatisticsGet_NT(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics)
{
    PXI32U rrt = 0;

    ULONG bufferSize = 0;

    // First call to get required buffer size to allocate for the table
    ULONG resultID = GetTcpTable(NULL, &bufferSize, TRUE);

    // We do expect resultID == ERROR_INSUFFICIENT_BUFFER

    PMIB_TCPTABLE tcpTable = (PMIB_TCPTABLE)PXMemoryHeapCalloc(PXNull, 1, bufferSize);

    if(!tcpTable)
    {
        return PXResultFailedMemoryAllocation;
    }

    // Second call retrieves the actual connection table
    resultID = GetTcpTable(tcpTable, &bufferSize, TRUE);

    if(NO_ERROR != resultID)
    {
        PXMemoryHeapFree(PXNull, tcpTable);
        return PXResultInvalid;
    }

    for(DWORD i = 0; i < tcpTable->dwNumEntries; i++)
    {
#if 0
        // Find your specific local port/remote port signature matching your socket
        if(tcpTable->table[i].dwLocalPort == localBinaryPort &&
           tcpTable->table[i].dwRemotePort == remoteBinaryPort)
        {
            // tcpTable->table[i].dwState, this is only the socket state, useless!

            // Note: Windows XP does not expose raw performance metrics like RTT in this table.
            // Why is this here, if not useful??
            break;
        }
#endif
    }

    PXMemoryHeapFree(PXNull, tcpTable);

    return PXResultOK;
}

PXResult PXAPI PXSocketNameGet(const PXSocket PXREF pxSocket, PXText PXREF pxText)
{
    if(!(pxSocket && pxText))
    {
        return PXResultRefusedParameterNull;
    }

    struct sockaddr_in addressInfo;
    socklen_t addr_len = sizeof(addressInfo);

    const int res = getsockname(pxSocket->ID, (struct sockaddr*)&addressInfo, &addr_len);

    return PXResultOK;
}

PXResult PXAPI PXSocketInterfaceName(const PXSocket PXREF pxSocket, char* name, const PXSize nameSize)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix

    int resultID = 0;

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    resultID = getsockname(pxSocket->ID, (struct sockaddr*)&addr, &addr_len);
    pxResult = PXErrorCurrent(0 != resultID);

    if(pxResult != PXResultOK)
    {
        return pxResult;
    }

    struct ifaddrs* ifaddr, * ifa;

    resultID = getifaddrs(&ifaddr);
    pxResult = PXErrorCurrent(-1 != resultID);

    if(pxResult != PXResultOK)
    {
        return pxResult;
    }

    const char* result = NULL;

    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        struct sockaddr_in* in = (struct sockaddr_in*)ifa->ifa_addr;

        if(in->sin_addr.s_addr == addr.sin_addr.s_addr)
        {
            result = ifa->ifa_name;
            break;
        }
    }

    freeifaddrs(ifaddr);

#elif OSWindows

#endif

    return PXResultOK;
}