#include "PXNetwork.h"

const char PXNetworkText[] = "Network";
const char PXAdapterText[] = "Adapter";
const char PXWindowsWinSockDLL[] = "WS2_32.DLL";




#if OSUnix
#ifndef _XOPEN_SOURCE
// Needed for some reason, as without the "netdb.h" does not get Includedd properly
// Some say that there a flags that disable the code.
// This statement here.. counters this.
#define _XOPEN_SOURCE 600
#endif // _XOPEN_SOURCE
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct addrinfo AdressInfoType; //#define AdressInfoType (struct addrinfo)
#define AdressInfoDelete freeaddrinfo
#elif OSWindows

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1u
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#define AdressInfoType ADDRINFOA
#define AdressInfoDelete FreeAddrInfoA
#endif

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/PXOS.h>

#pragma comment(lib, "Ws2_32.lib")


#define ProtocolInvalid (unsigned int)-1
#define ProtocolHOPOPTS 0
#define ProtocolICMP 1
#define ProtocolIGMP 2
#define ProtocolGGP 3
#define ProtocolIPV4 4
#define ProtocolST 5
#define ProtocolTCP 6
#define ProtocolCBT 7
#define ProtocolEGP 8
#define ProtocolIGP 9
#define ProtocolPUP 12
#define ProtocolUDP 17
#define ProtocolIDP 22
#define ProtocolRDP 27
#define ProtocolIPV6 41
#define ProtocolROUTING 43
#define ProtocolFRAGMENT 44
#define ProtocolESP 50
#define ProtocolAH 51
#define ProtocolICMPV6 58
#define ProtocolNONE 59
#define ProtocolDSTOPTS 60
#define ProtocolND 77
#define ProtocolICLFXBM 78
#define ProtocolPIM 103
#define ProtocolPGM 113
#define ProtocolL2TP 115
#define ProtocolSCTP 132
#define ProtocolRAW 255
#define ProtocolMAX 256
#define ProtocolWindowsRAW 257
#define ProtocolWindowsIPSEC 258
#define ProtocolWindowsIPSECOFFLOAD 259
#define ProtocolWindowsWNV 260
#define ProtocolWindowsMAX 261






#define IPAF_UNSPEC 0 // Unspecified
#define IPAF_LOCAL 1 // Local to host (pipes and file-domain).
#define IPAF_UNIX IPAF_LOCAL // POSIX name for IPAF_LOCAL.
#define IPAF_FILE IPAF_LOCAL // Another non-standard name for IPAF_LOCAL.
#define IPAF_INET 2 // IP protocol family.
#define IPAF_AX25 3 // Amateur Radio AX.25.
#define IPAF_IMPLINK 3 // arpanet imp addresses
#define IPAF_IPX 4 // Novell Internet Protocol.
#define IPAF_PUP 4 // pup protocols: e.g. BSP
#define IPAF_APPLETALK 5 // Appletalk DDP.
#define IPAF_CHAOS 5 // mit CHAOS protocols
#define IPAF_NETROM 6 // Amateur radio NetROM.
#define IPAF_NS 6 // XEROX NS protocols
#define IPAF_IPX IPAF_NS // IPX protocols: IPX, SPX, etc.
#define IPAF_BRIDGE 7 // Multiprotocol bridge.
#define IPAF_ISO 7 // ISO protocols
#define IPAF_OSI IPAF_ISO // OSI is ISO
#define IPAF_ATMPVC 8 // ATM PVCs.
#define IPAF_ECMA 8 // european computer manufacturers
#define IPAF_X25 9 // Reserved for X.25 project.
#define IPAF_DATAKIT 9 // datakit protocols
#define IPAF_INET6 10 // IP version 6.
#define IPAF_CCITT 10 // CCITT protocols, X.25 etc
#define IPAF_ROSE 11 // Amateur Radio X.25 PLP.
#define IPAF_SNA 11 // IBM SNA
#define IPAF_DECnet 12 // Reserved for DECnet project.
#define IPAF_DECnet 12 // DECnet
#define IPAF_NETBEUI 13 // Reserved for 802.2LLC project.
#define IPAF_DLI 13 // Direct data link interface
#define IPAF_SECURITY 14 // Security callback pseudo AF.
#define IPAF_LAT 14 // LAT
#define IPAF_KEY 15 // IPAF_KEY key management API.
#define IPAF_HYLINK 15 // NSC Hyperchannel
#define IPAF_NETLINK 16
#define IPAF_APPLETALK 16 // AppleTalk
#define IPAF_ROUTE IPAF_NETLINK // Alias to emulate 4.4BSD.
#define IPAF_PACKET 17 // Packet family.
#define IPAF_NETBIOS 17 // NetBios-style addresses
#define IPAF_ASH 18 // Ash.
#define IPAF_VOICEVIEW 18 // VoiceView
#define IPAF_ECONET 19 // Acorn Econet.
#define IPAF_FIREFOX 19 // Protocols from Firefox
#define IPAF_ATMSVC 20 // ATM SVCs.
#define IPAF_UNKNOWN1 20 // Somebody is using this!
#define IPAF_RDS 21 // RDS sockets.
#define IPAF_BAN 21 // Banyan
#define IPAF_SNA 22 // Linux SNA Project
#define IPAF_ATM 22 // Native ATM Services
#define IPAF_IRDA 23 // IRDA sockets.
#define IPAF_INET6 23 // Internetwork Version 6
#define IPAF_PPPOX 24 // PPPoX sockets.
#define IPAF_CLUSTER 24 // Microsoft Wolfpack
#define IPAF_WANPIPE 25 // Wanpipe API sockets.
#define IPAF_12844 25 // IEEE 1284.4 WG AF
#define IPAF_LLC 26 // Linux LLC.
#define IPAF_IRDA 26 // IrDA
#define IPAF_IB 27 // Native InfiniBand address.
#define IPAF_MPLS 28 // MPLS.
#define IPAF_NETDES 28 // Network Designers OSI & gateway
#define IPAF_CAN 29 // Controller Area Network.
#define IPAF_TIPC 30 // TIPC sockets.
#define IPAF_BLUETOOTH 31 // Bluetooth sockets.
#define IPAF_IUCV 32 // IUCV sockets.
#define IPAF_RXRPC 33 // RxRPC sockets.
#define IPAF_ISDN 34 // mISDN sockets.
#define IPAF_PHONET 35 // Phonet sockets.
#define IPAF_IEEE802154 36 // IEEE 802.15.4 sockets.
#define IPAF_CAIF 37 // CAIF sockets.
#define IPAF_ALG 38 // Algorithm sockets.
#define IPAF_NFC 39 // NFC sockets.
#define IPAF_VSOCK 40 // vSockets.
#define IPAF_KCM 41 // Kernel Connection Multiplexor.
#define IPAF_QIPCRTR 42 // Qualcomm IPC Router.
#define IPAF_SMC 43 // SMC sockets.
#define IPAF_XDP 44 // XDP sockets.
#define IPAF_MAX 45 // For now..

#define IPAF_TCNPROCESS   29
#define IPAF_TCNMESSAGE   30
#define IPAF_ICLFXBM      31
#define IPAF_BTH          32              // Bluetooth RFCOMM/L2CAP protocols
#define IPAF_LINK         33
#define IPAF_HYPERV       34
#define IPAF_MAX          35

#define IPAF_Invalid 0xFF

#if OSWindows
#define EAI_ADDRFAMILY WSAHOST_NOT_FOUND
#endif

#define SOCK_Invalid 0xFF



PXNetwork _pxNetwork;


PXResult PXAPI  PXNetworkModulState(PXNetwork** const pxNetworkREF, const PXI32U flags)
{
    if(pxNetworkREF)
    {
        *pxNetworkREF = &_pxNetwork;
    }

    switch(flags)
    {
        case PXModuleStateDoInitialize:
        {
            PXClear(PXNetwork, &_pxNetwork);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Initialize",
                "---Start---"
            );
#endif


            // Connect
            {
                const PXActionResult openResult = PXLibraryOpenA(&_pxNetwork.NetworkLibrary, PXWindowsWinSockDLL);

                if(PXActionSuccessful != openResult)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        PXNetworkText,
                        "Initialize",
                        "Library not found"
                    );
#endif

                    return PXActionRefusedNotSupportedByOperatingSystem;
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNetworkText,
                    "Initialize",
                    "Library load successful"
                );
#endif
            }

            // Fetch functions
            {
                const PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
                {
                    { &_pxNetwork.SocketAccept, "accept"},
                    { &_pxNetwork.SocketBind, "bind"},
                    { &_pxNetwork.SocketClose, "closesocket"},
                    { &_pxNetwork.SocketConnect, "connect"},
                    { &_pxNetwork.getpeername, "getpeername"},
                    { &_pxNetwork.getsockname, "getsockname"},
                    { &_pxNetwork.getsockopt, "getsockopt"},
                    { &_pxNetwork.SocketControlIO, "ioctlsocket"},
                    { &_pxNetwork.inet_addr, "inet_addr"},
                    { &_pxNetwork.inet_ntoa, "inet_ntoa"},
                    { &_pxNetwork.SocketListen, "listen"},
                    { &_pxNetwork.Converthtonl, "ntohl"},
                    { &_pxNetwork.Converthtons, "ntohs"},
                    { &_pxNetwork.SocketRecive, "recv"},
                    { &_pxNetwork.recvfrom, "recvfrom"},
                    { &_pxNetwork.SocketSelect, "select"},
                    { &_pxNetwork.SocketSend, "send"},
                    { &_pxNetwork.sendto, "sendto"},
                    { &_pxNetwork.SocketOptionSet, "setsockopt"},
                    { &_pxNetwork.SocketShutdown, "shutdown"},
                    { &_pxNetwork.SocketCreate, "socket"},
                    { &_pxNetwork.AdressInfoGet, "getaddrinfo"},
                    { &_pxNetwork.HostByNameGet, "gethostbyname"},

                    // Windows WSA
        #if OSWindows
                    { &_pxNetwork.SocketSystemErrorLastGet, "WSAGetLastError"},
                    { &_pxNetwork.SocketSystemStartup, "WSAStartup"},
                    { &_pxNetwork.SocketSystemCleanup, "WSACleanup"},
                    { &_pxNetwork.SocketPoll, "WSAPoll"},
        #endif

                };

                const PXSize amounnt = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

                PXLibraryGetSymbolListA(&_pxNetwork.NetworkLibrary, pxLibraryFuntionEntry, amounnt);
            }


#if OSWindows
            // WSA startup
            {
                const WORD wVersionRequested = MAKEWORD(2, 2);
                WSADATA wsaData;
                PXClear(WSADATA, &wsaData);
                const int result = _pxNetwork.SocketSystemStartup(wVersionRequested, &wsaData);
                const PXBool successful = 0 != result;

                if(!successful)
                {
                    return PXActionInvalid;
                }
            }
#endif


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Initialize",
                "---Done---"
            );
#endif

            break;
        }
        case PXModuleStateDoRelease:
        {
#if OSWindows
            const int result = _pxNetwork.SocketSystemCleanup(); // Windows socket agent cleanup
            const PXBool successful = 0 != result;

            if(!successful)
            {
                return PXActionSuccessful;
            }
#endif

            PXLibraryClose(&_pxNetwork.NetworkLibrary);

            break;
        }
        default:
        {
            return PXActionRefusedArgumentInvalid;
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketCreate(PXSocketCreateInfo* const pxSocketCreateInfo)
{
    PXSocket* const pxSocket = pxSocketCreateInfo->SocketReference;

    const int ipAdressFamilyID = PXIPAdressFamilyToID(pxSocketCreateInfo->AdressFamily);
    const int socketTypeID = PXSocketTypeToID(pxSocketCreateInfo->Type);
    const int protocolModeID = PXProtocolModeToID(pxSocketCreateInfo->ProtocolMode);

    // Create socket
    {
        const PXSocketID socketIDResult = _pxNetwork.SocketCreate(ipAdressFamilyID, socketTypeID, protocolModeID);
        const PXActionResult createResult = PXErrorCurrent(-1 != socketIDResult); // PXWindowsSocketAgentErrorFetch

        if(PXActionSuccessful != createResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Socket-Create",
                "Failed"
            );
#endif

            return createResult;
        }

        pxSocket->ID = socketIDResult;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXNetworkText,
            "Socket-Create",
            "New <%i>",
            (int)pxSocket->ID
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
        const char opval = 1;
        const int optionsocketResult = _pxNetwork.SocketOptionSet(pxSocket->ID, level, optionName, &opval, sizeof(int));
        const PXBool sucessful = optionsocketResult == 0;
    }

    //pxSocket->Family = adressFamily;
    //pxSocket->Protocol = protocolMode;
    //pxSocket->Type = socketType;

#if OSWindows
    // make non blocking
    u_long ul = 0;
    int nRet = _pxNetwork.SocketControlIO(pxSocket->ID, FIONBIO, &ul);
#endif


    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkIPLocate(const PXIPAdress* const pxIPAdress)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXNetworkSocketPeerGet(PXNetwork* const pxNetwork, PXSocketConnectionInfo* const pxSocketConnectionInfo)
{
#if OSUnix || OSWindows
    struct sockaddr socketAdress;
    int socketAdressSize = sizeof(struct sockaddr);

    const int resultID = 0;// getpeername(socketID, &socketAdress, &socketAdressSize);






    return PXActionInvalid;
#else
    return PXActionInvalid;
#endif
}

PXResult PXAPI  PXNetworkSocketDestroy(const PXSocketID pxSocketID)
{
#if OSUnix
    close(pxSocket->ID);
#elif OSWindows
    _pxNetwork.SocketShutdown(pxSocketID, SD_SEND);
    _pxNetwork.SocketClose(pxSocketID);
#endif

    // PXFunctionInvoke(pxSocket->EventList.SocketClosedCallBack, pxSocket->Owner, pxSocket);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNetworkText,
        "Socket-Destroy",
        "Terminated <%i>",
        pxSocketID
    );
#endif
}

PXResult PXAPI  PXNetworkSocketConnect(PXSocketConnectInfo* const pxSocketConnectInfo)
{
    PXSocket* const pxSocket = pxSocketConnectInfo->SocketReference;

    int serverAdressLength = 0;
    struct sockaddr_in server;
    PXClear(struct sockaddr_in, &server);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNetworkText,
        "Socket-Connect",
        "Connecting <%i> to <IP:%s, Port:%i>",
        (int)pxSocket->ID,
        pxSocketConnectInfo->IP,
        pxSocketConnectInfo->Port
    );
#endif

    int connectResultID = -1;

    // Hostname
    {

        if(_pxNetwork.AdressInfoGet) // Use modern
        {
#if OSWindows
            ADDRINFOA* adressResult = PXNull;
            ADDRINFOA adressInput;
            PXClear(ADDRINFOA, &adressInput);

            PXText portText;
            PXTextConstructNamedBufferA(&portText, portTextBuffer, 32);
            PXTextFromInt(&portText, pxSocketConnectInfo->Port);

            adressInput.ai_family = PXIPAdressFamilyToID(pxSocketConnectInfo->AdressFamily);
            adressInput.ai_socktype = PXSocketTypeToID(pxSocketConnectInfo->Type);
            adressInput.ai_protocol = PXProtocolModeToID(pxSocketConnectInfo->ProtocolMode);
#if OSWindows
            adressInput.ai_flags = AI_CANONNAME;
#endif

            char localIP[] = "127.0.0.1";
            char* adresspoint = pxSocketConnectInfo->IP.Text ? pxSocketConnectInfo->IP.Text : localIP;

            const int resultID = _pxNetwork.AdressInfoGet
            (
                adresspoint,
                portText.A,
                &adressInput,
                &adressResult
            );
            const PXBool success = 0 == resultID;

#if OSWindows
            if(!success)
            {
                const PXActionResult xxxx = PXWindowsSocketAgentErrorFromID(resultID);

                return xxxx;
            }
#endif

            for(ADDRINFOA* adress = adressResult; adress; adress = adressResult->ai_next)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNetworkText,
                    "Socket",
                    "Name: %s",
                    adress->ai_canonname
                );
#endif
            }


            connectResultID = _pxNetwork.SocketConnect(pxSocket->ID, adressResult->ai_addr, adressResult->ai_addrlen);


            int length = sizeof(ADDRINFOA);
            ADDRINFOA aaaaa;

            char budder[255];

            PXClear(ADDRINFOA, &aaaaa);
            PXClearList(char, budder, 255);
            aaaaa.ai_addrlen = 255;
            aaaaa.ai_addr = budder;

            int x = getpeername(pxSocket->ID, &aaaaa, &length);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Socket-Connect",
                "Connected to %s",
                adressResult->ai_canonname
            );
#endif

#endif

        }
        else // Use legacy
        {
            // [Warning - Deprecated]: Can't understand and resolve IPv6 adresses. Will only utilize and understand IPv4
            // Use getaddrinfo() because it is protocol-independent.
            struct hostent* host = _pxNetwork.HostByNameGet(pxSocketConnectInfo->IP.Text);

            if(!host)
            {
                return 0;
            }

            PXMemoryCopy(host->h_addr_list, &server.sin_addr, host->h_length);
            server.sin_port = pxSocketConnectInfo->Port;
            serverAdressLength = sizeof(struct sockaddr_in);

            connectResultID = _pxNetwork.SocketConnect(pxSocket->ID, &server, serverAdressLength);
        }
    }

    const PXActionResult connectResult = PXErrorCurrent(-1 != connectResult);

    if(PXActionSuccessful != connectResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXNetworkText,
            "Socket-Connect",
            "Connect to server failed! <%i> -> <IP:%s, Port:%i>",
            (int)pxSocket->ID,
            pxSocketConnectInfo->IP,
            pxSocketConnectInfo->Port
        );
#endif

        return connectResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNetworkText,
        "Socket-Connect",
        "Connecting <%i> successful",
        (int)pxSocket->ID
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketListen(PXSocketListenInfo* const pxSocketListenInfo)
{
    PXSocket* const pxSocket = pxSocketListenInfo->SocketReference;
    const int listeningResult = _pxNetwork.SocketListen(pxSocket->ID, pxSocketListenInfo->ClientInQueueMaximal);
    const PXActionResult listenResult = PXErrorCurrent(0 == listeningResult);

    if(PXActionSuccessful != listenResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXNetworkText,
            "Socket-Listen",
            "Failure <%i>",
            (int)pxSocket->ID
        );
#endif

        return listenResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNetworkText,
        "Socket-Listen",
        "Server <%i> now listening...",
        (int)pxSocket->ID
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketAccept(PXSocketAcceptInfo* const pxSocketAcceptInfo)
{
    PXSocket* const pxSocketServer = pxSocketAcceptInfo->SocketServerReference;
    PXSocket* const pxSocketClient = pxSocketAcceptInfo->SocketClientReference;

    PXClear(PXSocket, pxSocketClient);


    union
    {
        struct sockaddr SocketAdressInfo;
        struct sockaddr_in SocketAdressInfoIPv4;
        struct sockaddr_in6 SocketAdressInfoIPv6;
    }
    socketAdressInfo;


    int socketAdressInfoSize = sizeof(socketAdressInfo);
    PXClear(struct sockaddr, &socketAdressInfo);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNetworkText,
        "Socket-Accept",
        "Server <%i> wait for client...",
        (int)pxSocketServer->ID
    );
#endif


    pxSocketClient->ID = _pxNetwork.SocketAccept(pxSocketServer->ID, &socketAdressInfo.SocketAdressInfo, &socketAdressInfoSize);
    const PXActionResult acceptResult = PXErrorCurrent(-1 != pxSocketClient->ID);

    if(PXActionSuccessful != acceptResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXNetworkText,
            "Socket-Accept",
            "Server <%i> failed client accept",
            (int)pxSocketServer->ID
        );
#endif

        return acceptResult;
    }

    char ipname[128];
    PXI8U ipNameLength = 128;

    char hostname[300];
    char servInfo[NI_MAXSERV];


    // We have a socket now, get info
    {
        switch(socketAdressInfo.SocketAdressInfo.sa_family) // Depending on the protocol, we interpret the struct differently
        {
            // IPv4
            case AF_INET:
            {
                struct sockaddr_in* socketAdressInfoIPv4 = &socketAdressInfo.SocketAdressInfoIPv4;

                // const char* ipv4 = inet_ntoa(socketAdressInfoIPv4->sin_addr);
                // PXTextCopyA(ipv4, 128, ipname, 128);

#if OSWindows
                const INT errorID = WSAAddressToStringA
                (
                    &socketAdressInfo.SocketAdressInfoIPv4,
                    socketAdressInfoSize,
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
                struct sockaddr_in6* socketAdressInfoIPv6 = &socketAdressInfo.SocketAdressInfoIPv6;

                // Copy data and set data to format

#if OSWindows
                const INT errorID = WSAAddressToStringA
                (
                    &socketAdressInfo.SocketAdressInfoIPv6,
                    socketAdressInfoSize,
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
            &socketAdressInfo.SocketAdressInfo,
            socketAdressInfoSize,
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
        PXNetworkText,
        "Socket-Accept",
        "Server <%i> accepted client <%i> @ %s:%s",
        (int)pxSocketServer->ID,
        (int)pxSocketClient->ID,
        hostname,
        servInfo
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketBind(PXSocketBindInfo* const pxSocketBindInfo)
{
    if(!pxSocketBindInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    PXSocket* const pxSocket = pxSocketBindInfo->SocketReference;

    struct sockaddr_in bindAdressInfo;
    PXClear(struct sockaddr, &bindAdressInfo);
    bindAdressInfo.sin_family = AF_INET;
    bindAdressInfo.sin_addr.s_addr = _pxNetwork.Converthtonl(pxSocketBindInfo->IP); // INADDR_ANY
    bindAdressInfo.sin_port = _pxNetwork.Converthtons(pxSocketBindInfo->Port);
    const int bindAdressInfoSize = sizeof(struct sockaddr_in);


    const int bindingResultID = _pxNetwork.SocketBind(pxSocket->ID, &bindAdressInfo, bindAdressInfoSize);
    const PXActionResult bindingResult = PXErrorCurrent(-1 != bindingResultID);

    if(PXActionSuccessful != bindingResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXNetworkText,
            "Socket-Bind",
            "Server <%i> bind failed for <%s, %i>",
            (int)pxSocket->ID,
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
        PXNetworkText,
        "Socket-Bind",
        "Server <%i> bond to <%s, %i>",
        (int)pxSocket->ID,
        pxSocketBindInfo->IP,
        pxSocketBindInfo->Port
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketReceive(PXSocketDataInfo* const pxSocketDataInfo)
{
    PXActionResult pxActionResult = PXActionInvalid;

    if(!(pxSocketDataInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    const SOCKET socketID = pxSocketDataInfo->SocketIDCurrent;

    PXByte* data = (PXByte*)pxSocketDataInfo->Buffer + pxSocketDataInfo->BufferUsed;
    int length = pxSocketDataInfo->BufferSize - (pxSocketDataInfo->BufferUsed + 1);

    int readResult = 0;

    struct sockaddr_in sockAdressInfo;
    int sockAdressInfoLength = sizeof(sockAdressInfo);

    // Get type of socket
    int socketType;
    socklen_t len = sizeof(socketType);

    const int typeFetch = getsockopt(socketID, SOL_SOCKET, SO_TYPE, &socketType, &len);
    const PXBool isUDP = SOCK_DGRAM == socketType;

    if(isUDP) // No Connection
    {
        // UDP socket
        readResult = recvfrom
        (
            socketID,
            data,
            length,
            0,
            &sockAdressInfo,
            &sockAdressInfoLength
        );
    }
    else // Contection
    {
        // TCP socket
#if OSUnix
            readResult = read(socketID, data, availableSize);
#elif OSWindows
            //_pxNetwork.SocketRecive(socketID, data, availableSize, 0); // MSG_PEEK

            readResult = recv(socketID, data, length, 0);
#endif
    }

    // Handle...
    switch(readResult)
    {
        case 0:
        {
            pxActionResult = PXActionRefusedConnectionClosed;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Socket-Recive",
                "Connection <%i> close signal detected!",
                socketID
            );
#endif

            PXNetworkSocketDestroy(socketID);

            break;
        }
        case SOCKET_ERROR:
        {
            pxActionResult = PXErrorCurrent(0);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXNetworkText,
                "Socket-Recive",
                "Read failure <%i>",
                socketID
            );
#endif

            break;
        }
        default:
        {
            pxSocketDataInfo->BufferUsed = readResult;
            ((char*)pxSocketDataInfo->Buffer)[readResult] = 0;

            // because we are connectionless, we need to
            // translate the socket we are connected to

            PXSocket* const pxSocket = pxSocketDataInfo->SocketPeer;

            PXBool fetchExtendedData = isUDP && pxSocket;

            if(fetchExtendedData)
            {
                pxSocket->Port = ntohs(sockAdressInfo.sin_port);
                inet_ntop(sockAdressInfo.sin_family, &sockAdressInfo.sin_addr, pxSocket->IP, sizeof(pxSocket->IP));

                getnameinfo((struct sockaddr*)&sockAdressInfo, sockAdressInfoLength, pxSocket->Name, sizeof(pxSocket->Name), NULL, 0, 0);
            }

#if PXLogEnable
            char printableText[64];

            PXTextFromNonTerminated(printableText, 64, pxSocketDataInfo->Buffer, readResult);

            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                "Socket-Recive",
                "<%i> <-- <%i> %20s:%-5i - %-15s - %4i B, %s",
                socketID,
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
    
    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketSend(PXSocketDataInfo* const pxSocketDataInfo)
{
#if 0
    PXSocket* const pxSocketReciever = pxSocketSendInfo->SocketRecieverReference;
    PXSocket* const pxSocketSender = pxSocketSendInfo->SocketSenderReference;

#if 0
    // Check if socket is active and ready to send
    {
        const PXBool isSenderReady = PXSocketIsCurrentlyUsed(pxSocketSender);

        if(!isSenderReady)
        {
            return PXActionRefusedObjectNotReady;
        }

        const PXBool isReceiverReady = pxSocketReceiverID != -1;

        if(!isReceiverReady)
        {
            return PXActionRefusedSocketNotConnected;
        }
    }

    // Do we even send anything? If not, quit
    {
        if(!pxSocketSender->BufferOutput.Data)
        {
            return PXActionRefuedOutputBufferNull;
        }

        const PXBool hasDataToSend = pxSocketSender->BufferOutput.SizeMaximum > 0;

        if(!hasDataToSend)
        {
            return PXActionDidNothing; // Do not send anything if the message is empty
        }
    }
#endif

    // Send data
    PXBool done = PXFalse;

    do
    {
        const char* dataAdress = (PXByte*)pxSocketSendInfo->DataInfo.Buffer + pxSocketSendInfo->DataInfo.BufferOffset;

        int dataSize = pxSocketSendInfo->DataInfo.BufferSize - pxSocketSendInfo->DataInfo.BufferOffset;

        dataSize = PXMathMinimumI(dataSize, pxSocketSendInfo->DataInfo.SegmentSize);


        const int writtenBytes =
#if OSUnix
            write(pxSocketReciever->ID, dataAdress, dataSize);
#elif OSWindows
            _pxNetwork.SocketSend(pxSocketReciever->ID, dataAdress, dataSize, 0);
#endif

        const PXActionResult sendResult = PXErrorCurrent(-1 != writtenBytes);

        if(PXActionSuccessful != sendResult)
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
            pxSocketDataSendEventData.Data = dataAdress;
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

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketPoll()
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

        PXActionOnErrorFetchAndExit(!success);
#endif
    }






    for(PXSize i = 0; i < amount; i++)
    {
        const struct pollfd* const currentPollData = &socketDataList[i];

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

                    const PXActionResult succ = PXSocketReceive(&clientSocket, inputBuffer, 1024, &wrrit);

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
            PXActionResult selectResult = PXErrorCurrent();
#elif OSWindows
            PXActionResult selectResult = PXWindowsSocketAgentErrorFetch();
#endif

            numberOfSocketEvents = 0;
        }

        //PXActionOnErrorFetchAndExit(!success);

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

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketMTU(const PXSocket* const pxSocket, PXI32U* value, const PXBool doWrite)
{
    PXActionResult error = PXActionInvalid;

    if(doWrite)
    {
#if OSUnix || OSForcePOSIXForWindows
        char nameBuffer[IFNAMSIZ];
        struct ifreq ifr;

        PXNetworkSocketInterfaceName(pxSocket, nameBuffer, IFNAMSIZ);

        strncpy(ifr.ifr_name, nameBuffer, IFNAMSIZ - 1);  // Replace "eth0" with your interface

        const int resultID = ioctl(sock, SIOCGIFMTU, &ifr); // sys/ioctl.h
        error = PXErrorCurrent(-1 != resultID);

#elif OSWindows
        const DWORD val = *value;
        const INT errorID = WSASetIPUserMtu(pxSocket->ID, val); // Windows 10, Ws2_32.dll, ws2tcpip.h, WSAGetLastError 
#endif
    }
    else
    {
#if OSUnix
#elif OSWindows
        DWORD val = 0;
        const INT errorID = WSAGetIPUserMtu(pxSocket->ID, &val); // Windows 10, Ws2_32.dll, ws2tcpip.h, WSAGetLastError 

        *value = val;
#endif
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketName(const PXSocket* const pxSocket, char* name)
{
    struct sockaddr_in adressInfo;
    socklen_t addr_len = sizeof(adressInfo);

    const int res = getsockname(pxSocket->ID, (struct sockaddr*)&adressInfo, &addr_len);

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkSocketInterfaceName(const PXSocket* const pxSocket, char* name, const PXSize nameSize)
{
#if OSUnix

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    const int res = getsockname(pxSocket->ID, (struct sockaddr*)&addr, &addr_len);

    if(res != 0)
    {
        return NULL;
    }

    struct ifaddrs* ifaddr, * ifa;

    if(getifaddrs(&ifaddr) == -1)
    {
        return NULL;
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

    return PXActionSuccessful;
}

PXResult PXAPI  PXNetworkMACFromIPv4A(PXMAC* const pxMAC, char* const ipv4Text)
{
    PXIPv4 pxIPv4;
    pxIPv4.ID = inet_addr(ipv4Text); // Target IP;

    return PXNetworkMACFromIPv4(pxMAC, &pxIPv4);
}

PXResult PXAPI  PXNetworkMACFromIPv4(PXMAC* const pxMAC, PXIPv4* const pxIPv4)
{
    PXClear(PXMAC, pxMAC);

#if OSUnix
#elif OSWindows

    ULONG maxAdressLength = sizeof(pxMAC->Data);

    //IPAddr DestIp = inet_addr("192.168.1.1"); // Target IP

    const DWORD result = SendARP
    (
        pxIPv4->ID,
        0, // optional
        pxMAC->Data,
        &maxAdressLength
    ); // Windows 2000, Iphlpapi.dll, iphlpapi.h
    const PXBool success = NO_ERROR == result;

    switch(result) {
        case ERROR_GEN_FAILURE:
            printf(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_INVALID_PARAMETER:
            printf(" (ERROR_INVALID_PARAMETER)\n");
            break;
        case ERROR_INVALID_USER_BUFFER:
            printf(" (ERROR_INVALID_USER_BUFFER)\n");
            break;
        case ERROR_BAD_NET_NAME:
            printf(" (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_BUFFER_OVERFLOW:
            printf(" (ERROR_BUFFER_OVERFLOW)\n");
            break;
        case ERROR_NOT_FOUND:
            printf(" (ERROR_NOT_FOUND)\n");
            break;
        default:
            printf("\n");
            break;
    }


#if PXLogEnable

    // Reverse IP again.
//    pxIPv4->ID = htonl(pxIPv4->ID); // inet_ntoa, ntohl

    struct in_addr addr;
    addr.S_un.S_addr = pxIPv4->ID;
    const char* ipStr = inet_ntoa(addr);

    //   inet_pton(AI_INPU);

    PXLogPrint
    (
        PXLoggingInfo,
        "Network",
        "Translate",
        "IPv4:%s is MAC:%02x-%02x-%02x-%02x-%02x-%02x",
        ipStr,
        pxMAC->Data[0],
        pxMAC->Data[1],
        pxMAC->Data[2],
        pxMAC->Data[3],
        pxMAC->Data[4],
        pxMAC->Data[5]
    );
#endif

#else
#endif

    return PXActionInvalid;
}

PXResult PXAPI  PXNetworkMACFromIPv6A(PXMAC* const pxMAC, char* const ipv6Text)
{
    PXIPv6 pxIPv6;
    const INT result = InetPtonA(AF_INET6, ipv6Text, pxIPv6.Data);

    return PXNetworkMACFromIPv6(pxMAC, &pxIPv6);
}

PXResult PXAPI  PXNetworkMACFromIPv6(PXMAC* const pxMAC, PXIPv6* const pxIPv6)
{
    PXClear(PXMAC, pxMAC);

    MIB_IPNET_ROW2 ipList;
    PXClear(MIB_IPNET_ROW2, &ipList);

    // Set the target IP address
    ipList.Address.Ipv6.sin6_family = AF_INET6;

    PXMemoryCopy(pxIPv6->Data, ipList.Address.Ipv6.sin6_addr.u.Byte, 16);

    // Resolve the MAC address
    DWORD result = ResolveIpNetEntry2(&ipList, NULL);// Windows Vista, netioapi.h

    if(NO_ERROR != result)
    {
        return PXActionInvalid;
    }

    PXMemoryCopyX(ipList.PhysicalAddress, ipList.PhysicalAddressLength, pxMAC->Data, 6);


#if PXLogEnable

    char ipStr[46]; // IPv6 max length (46 characters)

    struct sockaddr_in6 addr;
    addr.sin6_family = AF_INET6;

    PXMemoryCopy(ipList.PhysicalAddress, addr.sin6_addr.u.Byte, 16);

    PCSTR resultAA = InetNtopA(AF_INET6, &addr.sin6_addr, ipStr, sizeof(ipStr) / sizeof(ipStr[0]));

    PXLogPrint
    (
        PXLoggingInfo,
        "Network",
        "Translate",
        "IPv4:%s is MAC:%02x-%02x-%02x-%02x-%02x-%02x",
        ipStr,
        pxMAC->Data[0],
        pxMAC->Data[1],
        pxMAC->Data[2],
        pxMAC->Data[3],
        pxMAC->Data[4],
        pxMAC->Data[5]
    );
#endif


    return PXActionInvalid;
}

PXResult PXAPI  PXNetworkNameFromIPv4A(char* const name, char* const ipv4Text)
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ipv4Text);

    char hostname[NI_MAXHOST];
    INT result = getnameinfo((struct sockaddr*)&sa, sizeof(sa), hostname, NI_MAXHOST, NULL, 0, 0) == 0;

    return PXActionInvalid;
}

PXResult PXAPI  PXNetworkNameFromIPv4(char* const name, PXIPv4* const pxIPv4)
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("8.8.8.8"); // Example: Google's DNS server

    char hostname[NI_MAXHOST];
    INT result = getnameinfo((struct sockaddr*)&sa, sizeof(sa), hostname, NI_MAXHOST, NULL, 0, 0) == 0;

    return PXActionInvalid;
}

PXResult PXAPI  PXNetworkAdapterFetch()
{
    WSADATA wsaData;
    int x = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initialize Winsock


    /* Declare and initialize variables */

    DWORD dwRetVal = 0;

    unsigned int i = 0;

    LPVOID lpMsgBuf = NULL;

    ULONG outBufLen = 0;
    ULONG Iterations = 0;


    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = NULL;
    IP_ADAPTER_PREFIX* pPrefix = NULL;

    // Documentation states that we should preallocate atlest 15KB for this function. 
    // As it is extremly slow and a call to just get the size will still result in this loading time.
    // So we need to avoid any more then this call
    outBufLen = 1024 * 16 * 2;
    PIP_ADAPTER_ADDRESSES pAddresses = PXMemoryHeapCalloc(PXNull, 1, outBufLen);

    do
    {
        //ULONG outBufLen = 0;
        dwRetVal = 0;// GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_Included_PREFIX, NULL, pAddresses, &outBufLen);

        if(ERROR_BUFFER_OVERFLOW == dwRetVal)
        {
            // Not enough memory! Resize and try again
            pAddresses = NULL;
        }
        else
        {
            break;
        }

        Iterations++;
    } while((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 5));


    if(dwRetVal != NO_ERROR)
    {
        return;
    }


    PXNetworkAdapter* pxNetworkAdapterList = PXMemoryHeapCallocT(PXNetworkAdapter, 10);



    // If successful, output some information from the data we received
    PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;

    for(PXSize index = 0; pCurrAddresses; ++index)
    {
        PXNetworkAdapter* const pxNetworkAdapter = &pxNetworkAdapterList[index];
        pxNetworkAdapter->MTU = pCurrAddresses->Mtu;

        pxNetworkAdapter->SpeedTransmit = pCurrAddresses->TransmitLinkSpeed;
        pxNetworkAdapter->SpeedRecieve = pCurrAddresses->ReceiveLinkSpeed;


        PXTextCopyWA(pCurrAddresses->Description, PXTextUnkownLength, pxNetworkAdapter->Description, 64);
        PXTextCopyWA(pCurrAddresses->FriendlyName, PXTextUnkownLength, pxNetworkAdapter->Name, 32);

        PXMemoryCopyX(pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength, pxNetworkAdapter->MACAdress.Data, 6);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXNetworkText,
            PXAdapterText,
            "\n"
            "%20s : %s\n"
            "%20s : %s\n"
            "%20s : %02x-%02x-%02x-%02x-%02x-%02x\n"
            "%20s : %i\n"
            "%20s : %i",

            "Description", pxNetworkAdapter->Description,
            "Name", pxNetworkAdapter->Name,
            "MAC",
            pxNetworkAdapter->MACAdress.Data[0],
            pxNetworkAdapter->MACAdress.Data[1],
            pxNetworkAdapter->MACAdress.Data[2],
            pxNetworkAdapter->MACAdress.Data[3],
            pxNetworkAdapter->MACAdress.Data[4],
            pxNetworkAdapter->MACAdress.Data[5],
            "SpeedTransmit", pxNetworkAdapter->SpeedTransmit,
            "SpeedRecieve", pxNetworkAdapter->SpeedRecieve
        );
#endif









        // std::cout << "DHCP Enabled: " << (adapter->Dhcpv4Enabled ? "Yes" : "No") << "\n";
         //std::cout << "DNS Servers:\n";
        for(PIP_ADAPTER_DNS_SERVER_ADDRESS dns = pCurrAddresses->FirstDnsServerAddress; dns; dns = dns->Next) {
            char dnsStr[46];
            inet_ntop(dns->Address.lpSockaddr->sa_family, &((struct sockaddr_in*)dns->Address.lpSockaddr)->sin_addr, dnsStr, sizeof(dnsStr));
            // std::cout << " - " << dnsStr << "\n";

            printf("");
        }

        //  std::cout << "Gateway: ";
        for(PIP_ADAPTER_GATEWAY_ADDRESS_LH gateway = pCurrAddresses->FirstGatewayAddress; gateway; gateway = gateway->Next) {
            char gatewayStr[46];
            inet_ntop(gateway->Address.lpSockaddr->sa_family, &((struct sockaddr_in*)gateway->Address.lpSockaddr)->sin_addr, gatewayStr, sizeof(gatewayStr));
            // std::cout << gatewayStr << "\n";

            printf("");
        }



        for(PIP_ADAPTER_UNICAST_ADDRESS unicast = pCurrAddresses->FirstUnicastAddress; unicast; unicast = unicast->Next)
        {
            char* ipTypeName = 0;
            char ipStr[46]; // IPv6 max size (IPv4 fits too)
            LPSOCKADDR socketAdress = unicast->Address.lpSockaddr;

            int family = socketAdress->sa_family;

            switch(family)
            {
                case AF_INET:
                {
                    inet_ntop(AF_INET, &((struct sockaddr_in*)socketAdress)->sin_addr, ipStr, sizeof(ipStr));
                    ipTypeName = "IPv4";
                    break;
                }
                case AF_INET6:
                {
                    inet_ntop(AF_INET6, &((struct sockaddr_in6*)socketAdress)->sin6_addr, ipStr, sizeof(ipStr));
                    ipTypeName = "IPv6";
                    break;
                }
                default:
                {
                    break;
                }
            }


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNetworkText,
                PXAdapterText,
                "%s : %s",
                ipTypeName,
                ipStr
            );
#endif
        }




#if 0

        printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n", pCurrAddresses->Length);
        printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
        printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

        pUnicast = pCurrAddresses->FirstUnicastAddress;
        if(pUnicast != NULL) {
            for(i = 0; pUnicast != NULL; i++)
                pUnicast = pUnicast->Next;
            printf("\tNumber of Unicast Addresses: %d\n", i);
        }
        else
            printf("\tNo Unicast Addresses\n");

        pAnycast = pCurrAddresses->FirstAnycastAddress;
        if(pAnycast) {
            for(i = 0; pAnycast != NULL; i++)
                pAnycast = pAnycast->Next;
            printf("\tNumber of Anycast Addresses: %d\n", i);
        }
        else
            printf("\tNo Anycast Addresses\n");

        pMulticast = pCurrAddresses->FirstMulticastAddress;
        if(pMulticast) {
            for(i = 0; pMulticast != NULL; i++)
                pMulticast = pMulticast->Next;
            printf("\tNumber of Multicast Addresses: %d\n", i);
        }
        else
            printf("\tNo Multicast Addresses\n");

        pDnServer = pCurrAddresses->FirstDnsServerAddress;
        if(pDnServer) {
            for(i = 0; pDnServer != NULL; i++)
                pDnServer = pDnServer->Next;
            printf("\tNumber of DNS Server Addresses: %d\n", i);
        }
        else
            printf("\tNo DNS Server Addresses\n");

        printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
        printf("\tDescription: %wS\n", pCurrAddresses->Description);
        printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

        if(pCurrAddresses->PhysicalAddressLength != 0) {
            printf("\tPhysical address: ");
            for(i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
                i++) {
                if(i == (pCurrAddresses->PhysicalAddressLength - 1))
                    printf("%.2X\n",
                           (int)pCurrAddresses->PhysicalAddress[i]);
                else
                    printf("%.2X-",
                           (int)pCurrAddresses->PhysicalAddress[i]);
            }
        }
        printf("\tFlags: %ld\n", pCurrAddresses->Flags);
        printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
        printf("\tIfType: %ld\n", pCurrAddresses->IfType);
        printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
        printf("\tIpv6IfIndex (IPv6 interface): %u\n", pCurrAddresses->Ipv6IfIndex);
        printf("\tZoneIndices (hex): ");
        for(i = 0; i < 16; i++)
            printf("%lx ", pCurrAddresses->ZoneIndices[i]);
        printf("\n");

        printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
        printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

        pPrefix = pCurrAddresses->FirstPrefix;
        if(pPrefix) {
            for(i = 0; pPrefix != NULL; i++)
                pPrefix = pPrefix->Next;
            printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
        }
        else
            printf("\tNumber of IP Adapter Prefix entries: 0\n");

        printf("\n");
#endif

        pCurrAddresses = pCurrAddresses->Next;
    }

    if(pAddresses) {
        //FREE(pAddresses);
    }

    return PXActionSuccessful;
}













PXProtocolMode PXAPI PXProtocolModeFromID(const PXI8U protocolMode)
{
    switch(protocolMode)
    {
        case ProtocolHOPOPTS:
            return PXProtocolModeHOPOPTS;

        case ProtocolICMP:
            return PXProtocolModeICMP;

        case ProtocolIGMP:
            return PXProtocolModeIGMP;

        case ProtocolGGP:
            return PXProtocolModeGGP;

        case ProtocolIPV4:
            return PXProtocolModeIPV4;

        case ProtocolST:
            return PXProtocolModeST;

        case ProtocolTCP:
            return PXProtocolModeTCP;

        case ProtocolCBT:
            return PXProtocolModeCBT;

        case ProtocolEGP:
            return PXProtocolModeEGP;

        case ProtocolIGP:
            return PXProtocolModeIGP;

        case ProtocolPUP:
            return PXProtocolModePUP;

        case ProtocolUDP:
            return PXProtocolModeUDP;

        case ProtocolIDP:
            return PXProtocolModeIDP;

        case ProtocolRDP:
            return PXProtocolModeRDP;

        case ProtocolIPV6:
            return PXProtocolModeIPV6;

        case ProtocolROUTING:
            return PXProtocolModeROUTING;

        case ProtocolFRAGMENT:
            return PXProtocolModeFRAGMENT;

        case ProtocolESP:
            return PXProtocolModeESP;

        case ProtocolAH:
            return PXProtocolModeAH;

        case ProtocolICMPV6:
            return PXProtocolModeICMPV6;

        case ProtocolNONE:
            return PXProtocolModeNONE;

        case ProtocolDSTOPTS:
            return PXProtocolModeDSTOPTS;

        case ProtocolND:
            return PXProtocolModeND;

        case ProtocolICLFXBM:
            return PXProtocolModeICLFXBM;

        case ProtocolPIM:
            return PXProtocolModePIM;

        case ProtocolPGM:
            return PXProtocolModePGM;

        case ProtocolL2TP:
            return PXProtocolModeL2TP;

        case ProtocolSCTP:
            return PXProtocolModeSCTP;

        case ProtocolRAW:
            return PXProtocolModeRAW;

        case ProtocolMAX:
            return PXProtocolModeMAX;

        case ProtocolWindowsRAW:
            return PXProtocolModeWindowsRAW;

        case ProtocolWindowsIPSEC:
            return PXProtocolModeWindowsIPSEC;

        case ProtocolWindowsIPSECOFFLOAD:
            return PXProtocolModeWindowsIPSECOFFLOAD;

        case ProtocolWindowsWNV:
            return PXProtocolModeWindowsWNV;

        case ProtocolWindowsMAX:
            return PXProtocolModeWindowsMAX;

        default:
            return PXProtocolModeInvalid;
    }
}

PXI8U PXAPI PXProtocolModeToID(const PXProtocolMode protocolMode)
{
    switch(protocolMode)
    {
        default:
        case PXProtocolModeInvalid:
            return ProtocolInvalid;

        case PXProtocolModeHOPOPTS:
            return ProtocolHOPOPTS;

        case PXProtocolModeICMP:
            return ProtocolICMP;

        case PXProtocolModeIGMP:
            return ProtocolIGMP;

        case PXProtocolModeGGP:
            return ProtocolGGP;

        case PXProtocolModeIPV4:
            return ProtocolIPV4;

        case PXProtocolModeST:
            return ProtocolST;

        case PXProtocolModeTCP:
            return ProtocolTCP;

        case PXProtocolModeCBT:
            return ProtocolCBT;

        case PXProtocolModeEGP:
            return ProtocolEGP;

        case PXProtocolModeIGP:
            return ProtocolIGP;

        case PXProtocolModePUP:
            return ProtocolPUP;

        case PXProtocolModeUDP:
            return ProtocolUDP;

        case PXProtocolModeIDP:
            return ProtocolIDP;

        case PXProtocolModeRDP:
            return ProtocolRDP;

        case PXProtocolModeIPV6:
            return ProtocolIPV6;

        case PXProtocolModeROUTING:
            return ProtocolROUTING;

        case PXProtocolModeFRAGMENT:
            return ProtocolFRAGMENT;

        case PXProtocolModeESP:
            return ProtocolESP;

        case PXProtocolModeAH:
            return ProtocolAH;

        case PXProtocolModeICMPV6:
            return ProtocolICMPV6;

        case PXProtocolModeNONE:
            return ProtocolNONE;

        case PXProtocolModeDSTOPTS:
            return ProtocolDSTOPTS;

        case PXProtocolModeND:
            return ProtocolND;

        case PXProtocolModeICLFXBM:
            return ProtocolICLFXBM;

        case PXProtocolModePIM:
            return ProtocolPIM;

        case PXProtocolModePGM:
            return ProtocolPGM;

        case PXProtocolModeL2TP:
            return ProtocolL2TP;

        case PXProtocolModeSCTP:
            return ProtocolSCTP;

        case PXProtocolModeRAW:
            return ProtocolRAW;

        case PXProtocolModeMAX:
            return ProtocolMAX;

        case PXProtocolModeWindowsRAW:
            return ProtocolWindowsRAW;

        case PXProtocolModeWindowsIPSEC:
            return ProtocolWindowsIPSEC;

        case PXProtocolModeWindowsIPSECOFFLOAD:
            return ProtocolWindowsIPSECOFFLOAD;

        case PXProtocolModeWindowsWNV:
            return ProtocolWindowsWNV;

        case PXProtocolModeWindowsMAX:
            return ProtocolWindowsMAX;
    }
}

PXSocketType PXAPI PXSocketTypeFromID(const PXI8U socketType)
{
    switch(socketType)
    {
        case SOCK_STREAM:
            return PXSocketTypeStream;

        case SOCK_DGRAM:
            return PXSocketTypeDatagram;

        case SOCK_RAW:
            return PXSocketTypeRaw;

        case SOCK_RDM:
            return PXSocketTypeRDM;

        case SOCK_SEQPACKET:
            return PXSocketTypeSeqPacket;

        default:
            return PXSocketTypeInvalid;
    }
}

PXI8U PXAPI PXSocketTypeToID(const PXSocketType socketType)
{
    switch(socketType)
    {
        default:
        case PXSocketTypeInvalid:
            return SOCK_Invalid;

        case PXSocketTypeStream:
            return SOCK_STREAM;

        case PXSocketTypeDatagram:
            return SOCK_DGRAM;

        case PXSocketTypeRaw:
            return SOCK_RAW;

        case PXSocketTypeRDM:
            return SOCK_RDM;

        case PXSocketTypeSeqPacket:
            return SOCK_SEQPACKET;
    }
}

IPAdressFamily PXAPI PXIPAdressFamilyFromID(const PXI8U ipMode)
{
    switch(ipMode)
    {
        case IPAF_UNSPEC:
            return IPAdressFamilyUnspecified;
        case IPAF_UNIX:
            return IPAdressFamilyUNIX;
        case IPAF_INET:
            return IPAdressFamilyINET;
        case IPAF_IMPLINK:
            return IPAdressFamilyIMPLINK;
        case IPAF_PUP:
            return IPAdressFamilyPUP;
        case IPAF_CHAOS:
            return IPAdressFamilyCHAOS;
        case IPAF_NS:
            return IPAdressFamilyNS;
            //case IPAF_IPX: return IPAdressFamilyIPX;
        case IPAF_ISO:
            return IPAdressFamilyISO;
            //case IPAF_OSI: return IPAdressFamilyOSI;
        case IPAF_ECMA:
            return IPAdressFamilyECMA;
        case IPAF_DATAKIT:
            return IPAdressFamilyDATAKIT;
        case IPAF_CCITT:
            return IPAdressFamilyCCITT;
        case IPAF_SNA:
            return IPAdressFamilySNA;
        case IPAF_DECnet:
            return IPAdressFamilyDECnet;
        case IPAF_DLI:
            return IPAdressFamilyDLI;
        case IPAF_LAT:
            return IPAdressFamilyLAT;
        case IPAF_HYLINK:
            return IPAdressFamilyHYLINK;
        case IPAF_APPLETALK:
            return IPAdressFamilyAPPLETALK;
        case IPAF_NETBIOS:
            return IPAdressFamilyNETBIOS;
        case IPAF_VOICEVIEW:
            return IPAdressFamilyVOICEVIEW;
        case IPAF_FIREFOX:
            return IPAdressFamilyFIREFOX;
        case IPAF_UNKNOWN1:
            return IPAdressFamilyUNKNOWN1;
        case IPAF_BAN:
            return IPAdressFamilyBAN;
            //case IPAF_ATM: return IPAdressFamilyATM;
        case IPAF_INET6:
            return IPAdressFamilyINET6;
        case IPAF_CLUSTER:
            return IPAdressFamilyCLUSTER;
        case IPAF_12844:
            return IPAdressFamilyIEEE12844;
        case IPAF_IRDA:
            return IPAdressFamilyIRDA;
        case IPAF_NETDES:
            return IPAdressFamilyNETDES;

        default:
            return IPAdressFamilyInvalid;
    }
}

PXI8U PXAPI PXIPAdressFamilyToID(const IPAdressFamily ipMode)
{
    switch(ipMode)
    {
        default:
        case IPAdressFamilyInvalid:
            return IPAF_Invalid;

        case IPAdressFamilyUnspecified:
            return IPAF_UNSPEC;

        case IPAdressFamilyUNIX:
            return IPAF_UNIX;

        case IPAdressFamilyINET:
            return IPAF_INET;

        case IPAdressFamilyIMPLINK:
            return IPAF_IMPLINK;

        case IPAdressFamilyPUP:
            return IPAF_PUP;

        case IPAdressFamilyCHAOS:
            return IPAF_CHAOS;

        case IPAdressFamilyNS:
            return IPAF_NS;

        case IPAdressFamilyIPX:
            return IPAF_IPX;

        case IPAdressFamilyISO:
            return IPAF_ISO;

        case IPAdressFamilyOSI:
            return IPAF_OSI;

        case IPAdressFamilyECMA:
            return IPAF_ECMA;

        case IPAdressFamilyDATAKIT:
            return IPAF_DATAKIT;

        case IPAdressFamilyCCITT:
            return IPAF_CCITT;

        case IPAdressFamilySNA:
            return IPAF_SNA;

        case IPAdressFamilyDECnet:
            return IPAF_DECnet;

        case IPAdressFamilyDLI:
            return IPAF_DLI;

        case IPAdressFamilyLAT:
            return IPAF_LAT;

        case IPAdressFamilyHYLINK:
            return IPAF_HYLINK;

        case IPAdressFamilyAPPLETALK:
            return IPAF_APPLETALK;

        case IPAdressFamilyNETBIOS:
            return IPAF_NETBIOS;

        case IPAdressFamilyVOICEVIEW:
            return IPAF_VOICEVIEW;

        case IPAdressFamilyFIREFOX:
            return IPAF_FIREFOX;

        case IPAdressFamilyUNKNOWN1:
            return IPAF_UNKNOWN1;

        case IPAdressFamilyBAN:
            return IPAF_BAN;

        case IPAdressFamilyATM:
            return IPAF_ATM;

        case IPAdressFamilyINET6:
            return IPAF_INET6;

        case IPAdressFamilyCLUSTER:
            return IPAF_CLUSTER;

        case IPAdressFamilyIEEE12844:
            return IPAF_12844;

        case IPAdressFamilyIRDA:
            return IPAF_IRDA;

        case IPAdressFamilyNETDES:
            return IPAF_NETDES;

        case IPAdressFamilyMAX:
            return IPAF_MAX;

        case IPAdressFamilyLINK:
            return IPAF_LINK;

        case IPAdressFamilyHYPERV:
            return IPAF_HYPERV;

        case IPAdressFamilyBTH:
            return IPAF_BTH;

        case IPAdressFamilyTCNPROCESS:
            return IPAF_TCNPROCESS;

        case IPAdressFamilyTCNMESSAGE:
            return IPAF_TCNMESSAGE;

        case IPAdressFamilyICLFXBM:
            return IPAF_ICLFXBM;
    }
}

#if OSWindows
PXResult PXAPI  PXWindowsSocketAgentErrorFetch(PXNetwork* const pxNetwork)
{
    const int wsaErrorID = pxNetwork->SocketSystemErrorLastGet();
    const PXActionResult pxActionResult = PXWindowsSocketAgentErrorFromID(wsaErrorID);

    return pxActionResult;
}

PXResult PXAPI  PXWindowsSocketAgentErrorFromID(const PXI32S errorID)
{
    switch(errorID)
    {
        case WSAEBADF: // fall through
            //  case WSA_INVALID_HANDLE: return PXActionRefusedNotFound; // Specified event object handle is invalid.
        case WSA_NOT_ENOUGH_MEMORY:
            return PXActionFailedMemoryAllocation; // Insufficient memory available.
        case WSAEINVAL: // fall through
        case WSA_INVALID_PARAMETER:
            return PXActionRefuedInputInvalid; // One or more parameters are invalid.
            //case WSA_OPERATION_ABORTED:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_IO_INCOMPLETE: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_IO_PENDING: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSAEINTR:
            return PXActionInterruptedByFunctionCall; // xxxxxxxxxxxxxxxxx
            //   case WSAEACCES: return PXActionRefuedPermissionDenied;
        case WSAEFAULT:
            return PXActionRefuedAdressInvalid;
        case WSAEMFILE:
            return PXActionFailedResourcedNotEnough;
        case WSAEWOULDBLOCK:
            return PXActionWouldBlock; // xxxxxxxxxxxxxxxxx
        case WSAEINPROGRESS:
            return PXActionNowInProgress; // xxxxxxxxxxxxxxxxx
        case WSAEALREADY:
            return PXActionAlreadyInProgress; // xxxxxxxxxxxxxxxxx
            //  case WSAENOTSOCK: return PXActionRefusedNotFound; // Socket operation on nonsocket.
            //case WSAEDESTADDRREQ:  return xxxxxxxxxxxx; // Destination address required.
        case WSAEMSGSIZE:
            return PXActionRefuedInputBufferTooBig; // Message too long.
        case WSAEPROTOTYPE:
            return PXActionRefuedProtocolTypeInvalid; // Protocol wrong type for socket.
        case WSAENOPROTOOPT:
            return PXActionRefuedProtocolOptionInvalid; // Bad protocol option.
        case WSAEPROTONOSUPPORT:
            return PXActionRefuedProtocolNotSupported; // Protocol not supported.
            // case WSAESOCKTNOSUPPORT: return PXActionRefuedObjectTypeNotSupported; // Socket type not supported.
        case WSAEOPNOTSUPP:
            return PXActionRefuedOperationNotSupported; // Operation not supported.
        case WSAEPFNOSUPPORT:
            return PXActionRefuedProtocolFamilyNotSupported; // Protocol family not supported.
        case WSAEAFNOSUPPORT:
            return PXActionRefuedAddressFamilyNotSupportedByProtocol; // Address family not supported by protocol family.
        case WSAEADDRINUSE:
            return PXActionRefuedAlreadyInUse; // Address already in use.
            //case WSAEADDRNOTAVAIL:   return xxxxxxxxxxxx; // Cannot assign requested address.
        case WSAENETDOWN:
            return PXActionRefuedNetworkNotConnected; // Network is down.
        case WSAENETUNREACH:
            return PXActionRefuedNetworkNotReachable; // Network is unreachable.
        case WSAENETRESET:
            return PXActionFailedConnectionTerminatedByNetwork; // Network dropped connection on reset.
        case WSAECONNABORTED:
            return PXActionFailedConnectionTerminatedByOwnSoftware; // Software caused connection abort.
        case WSAECONNRESET:
            return PXActionFailedConnectionTerminatedByPeer; // Connection reset by peer.
        case WSAENOBUFS:
            return PXActionFailedMemoryAllocationInternal; // No buffer space available.
        case WSAEISCONN:
            return PXActionRefusedAlreadyConnected; // Socket is already connected.
        case WSAENOTCONN:
            return PXActionFailedNotConnected; // Socket is not connected.
            //case WSAESHUTDOWN:  return WindowsSocketSystemWasShutdown; // Cannot send after socket shutdown.
            //case WSAETOOMANYREFS:   return xxxxxxxxxxxx; // Too many references.
            //case WSAETIMEDOUT:   return xxxxxxxxxxxx; // Connection timed out.
        case WSAECONNREFUSED:
            return PXActionRefuedServiceNotRunning; // Connection refused.
            //case WSAELOOP:   return xxxxxxxxxxxx; // Cannot translate name.
            //case WSAENAMETOOLONG:   return xxxxxxxxxxxx; // Name too long.
            //case WSAEHOSTDOWN:   return xxxxxxxxxxxx; // Host is down.
            //case WSAEHOSTUNREACH:   return xxxxxxxxxxxx; // No route to host.
        case WSAENOTEMPTY:
            return PXActionFailedDirectoryIsNotEmpty; // Directory not empty.
        case WSAEPROCLIM:
            return PXActionFailedTooManyProcesses; // Too many processes.
        case WSAEUSERS:
            return PXActionFailedUserQuotaExceeded; // User quota exceeded.
        case WSAEDQUOT:
            return PXActionFailedDiskQuotaExceeded; // Disk quota exceeded.
        case WSAESTALE:
            return PXActionFailedHandleIsStale; // Stale file handle reference.
        case WSAEREMOTE:
            return PXActionFailedResourceNotAvailableLocally; // Item is remote.
            //case WSASYSNOTREADY:   return PXActionFailedNetworkSubsystemNotReady; // Network subsystem is unavailable.
            //case WSAVERNOTSUPPORTED:   return WindowsSocketVersionNotSupported; // Winsock.dll version out of range.
            //case WSANOTINITIALISED:   return WindowsSocketSystemNotInitialized; // Successful WSAStartup not yet performed.
            //case WSAEDISCON:   return PXActionRefusedResourceIsShuttingdown; // Graceful shutdown in progress.
            //case WSAENOMORE:   return xxxxxxxxxxxx; // No more results.
            //case WSAECANCELLED:   return xxxxxxxxxxxx; // Call has been canceled.
            //case WSAEINVALIDPROCTABLE:   return xxxxxxxxxxxx; // Procedure call table is invalid.
            //case WSAEINVALIDPROVIDER:   return xxxxxxxxxxxx; // Service provider is invalid.
            //case WSAEPROVIDERFAILEDINIT:   return xxxxxxxxxxxx; // Service provider failed to initialize
            //case WSASYSCALLFAILURE:   return xxxxxxxxxxxx; // System call failure.
            //case WSASERVICE_NOT_FOUND:   return xxxxxxxxxxxx; // Service not found.
            //case WSATYPE_NOT_FOUND:   return xxxxxxxxxxxx; // Class type not found.
            //case WSA_E_NO_MORE:   return xxxxxxxxxxxx; // No more results.
            //case WSA_E_CANCELLED:   return xxxxxxxxxxxx; // Call was canceled.
            //case WSAEREFUSED:   return xxxxxxxxxxxx; // Database query was refused.
        case WSAHOST_NOT_FOUND:
            return PXActionFailedHostNotFound; // Host not found.
            //case WSATRY_AGAIN:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSANO_RECOVERY:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSANO_DATA:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_RECEIVERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_SENDERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_NO_SENDERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_NO_RECEIVERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_REQUEST_CONFIRMED:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_ADMISSION_FAILURE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //        case WSA_QOS_POLICY_FAILURE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_BAD_STYLE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_BAD_OBJECT:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_TRAFFIC_CTRL_ERROR:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_GENERIC_ERROR:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_ESERVICETYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFLOWSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EPROVSPECBUF:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFILTERSTYLE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFILTERTYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFILTERCOUNT:    return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EOBJLENGTH:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFLOWCOUNT:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EUNKOWNPSOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EPOLICYOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EFLOWDESC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EPSFLOWSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_EPSFILTERSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_QOS_ESDMODEOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            // case WSA_QOS_ESHAPERATEOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            // case WSA_QOS_RESERVED_PETYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx

        default:
            return PXActionInvalid;
    }
}
#endif
