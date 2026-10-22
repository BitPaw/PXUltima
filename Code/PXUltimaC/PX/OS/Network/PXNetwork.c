#include "PXNetwork.h"

const char PXNetworkText[] = "Network";
const char PXAdapterText[] = "Adapter";
const char PXWindowsWinSockDLL[] = "WS2_32.DLL";

const char PXNetworkLocalIP[] = "127.0.0.1";

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
#include <sys/ioctl.h>
#include <net/if.h> // IFNAMSIZ
#include <ifaddrs.h>

typedef struct addrinfo AddressInfoType; //#define AddressInfoType (struct addrinfo)
#define AddressInfoDelete freeaddrinfo
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
#define AddressInfoType ADDRINFOA
#define AddressInfoDelete FreeAddrInfoA
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


PXResult PXAPI PXNetworkModulState(PXNetwork* PXREF pxNetworkREF, const PXI32U flags)
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
                PXText pxText;
                PXTextFromAddressA(&pxText, PXWindowsWinSockDLL, sizeof(PXWindowsWinSockDLL), sizeof(PXWindowsWinSockDLL));
                const PXResult openResult = PXLibraryOpen(&_pxNetwork.NetworkLibrary, &pxText);

                if(PXResultOK != openResult)
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

                    return PXResultRefusedNotSupportedByOperatingSystem;
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
                const PXLibraryFunctionEntry PXLibraryFunctionEntry[] =
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
                    { &_pxNetwork.AddressInfoGet, "getaddrinfo"},
                    { &_pxNetwork.HostByNameGet, "gethostbyname"},

                    // Windows WSA
        #if OSWindows
                    { &_pxNetwork.SocketSystemErrorLastGet, "WSAGetLastError"},
                    { &_pxNetwork.SocketSystemStartup, "WSAStartup"},
                    { &_pxNetwork.SocketSystemCleanup, "WSACleanup"},
                    { &_pxNetwork.SocketPoll, "WSAPoll"},
        #endif

                };

                const PXSize amounnt = sizeof(PXLibraryFunctionEntry) / sizeof(PXLibraryFunctionEntry);

                PXLibraryGetSymbolListA(&_pxNetwork.NetworkLibrary, PXLibraryFunctionEntry, amounnt);
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
                    return PXResultInvalid;
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
                return PXResultOK;
            }
#endif

            PXLibraryClose(&_pxNetwork.NetworkLibrary);

            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXNetworkMACFromIPv4A(PXMAC PXREF pxMAC, char PXREF ipv4Text)
{
    PXIPv4 pxIPv4;
    pxIPv4.ID = inet_addr(ipv4Text); // Target IP;

    return PXNetworkMACFromIPv4(pxMAC, &pxIPv4);
}

PXResult PXAPI PXNetworkMACFromIPv4(PXMAC PXREF pxMAC, PXIPv4 PXREF pxIPv4)
{
    PXClear(PXMAC, pxMAC);

#if OSUnix
#elif OSWindows

    ULONG maxAddressLength = sizeof(pxMAC->Data);

    //IPAddr DestIp = inet_addr("192.168.1.1"); // Target IP

    const DWORD result = SendARP
    (
        pxIPv4->ID,
        0, // optional
        pxMAC->Data,
        &maxAddressLength
    ); // Windows 2000, Iphlpapi.dll, iphlpapi.h
    const PXBool success = NO_ERROR == result;

    switch(result) {
        case ERROR_GEN_FAILURE:

            PXConsoleWrite(0," (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_INVALID_PARAMETER:
            PXConsoleWrite(0, " (ERROR_INVALID_PARAMETER)\n");
            break;
        case ERROR_INVALID_USER_BUFFER:
            PXConsoleWrite(0, " (ERROR_INVALID_USER_BUFFER)\n");
            break;
        case ERROR_BAD_NET_NAME:
            PXConsoleWrite(0, " (ERROR_GEN_FAILURE)\n");
            break;
        case ERROR_BUFFER_OVERFLOW:
            PXConsoleWrite(0, " (ERROR_BUFFER_OVERFLOW)\n");
            break;
        case ERROR_NOT_FOUND:
            PXConsoleWrite(0, " (ERROR_NOT_FOUND)\n");
            break;
        default:
            PXConsoleWrite(0, "\n");
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
        PXNetworkText,
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

    return PXResultInvalid;
}

PXResult PXAPI PXNetworkMACFromIPv6A(PXMAC PXREF pxMAC, char PXREF ipv6Text)
{
#if OSUnix

#elif OSWindows
    PXIPv6 pxIPv6;
    const INT result = InetPtonA(AF_INET6, ipv6Text, pxIPv6.Data);

    return PXNetworkMACFromIPv6(pxMAC, &pxIPv6);
#endif
}

PXResult PXAPI PXNetworkMACFromIPv6(PXMAC PXREF pxMAC, PXIPv6 PXREF pxIPv6)
{
    PXClear(PXMAC, pxMAC);

#if OSUnix

#elif OSWindows
    MIB_IPNET_ROW2 ipList;
    PXClear(MIB_IPNET_ROW2, &ipList);

    // Set the target IP address
    ipList.Address.Ipv6.sin6_family = AF_INET6;

    PXMemoryCopy(pxIPv6->Data, ipList.Address.Ipv6.sin6_addr.u.Byte, 16);

    // Resolve the MAC address
    DWORD result = ResolveIpNetEntry2(&ipList, NULL); // Windows Vista, netioapi.h

    if(NO_ERROR != result)
    {
        return PXResultInvalid;
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

#endif

    return PXResultInvalid;
}

PXResult PXAPI PXNetworkNameFromIPv4A(char PXREF name, char PXREF ipv4Text)
{
#if OSUnix

#elif OSWindows
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ipv4Text);

    char hostname[NI_MAXHOST];
    INT result = getnameinfo((struct sockaddr*)&sa, sizeof(sa), hostname, NI_MAXHOST, NULL, 0, 0) == 0;
#endif

    return PXResultInvalid;
}

PXResult PXAPI PXNetworkNameFromIPv4(char PXREF name, PXIPv4 PXREF pxIPv4)
{
#if OSUnix

#elif OSWindows
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("8.8.8.8"); // Example: Google's DNS server

    char hostname[NI_MAXHOST];
    INT result = getnameinfo((struct sockaddr*)&sa, sizeof(sa), hostname, NI_MAXHOST, NULL, 0, 0) == 0;
#endif

    return PXResultInvalid;
}

PXResult PXAPI PXNetworkAdapterFetch()
{
#if OSUnix

#elif OSWindows
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
    PIP_ADAPTER_ADDRESSES pAddresses = (PIP_ADAPTER_ADDRESSES)PXMemoryHeapCalloc(PXNull, 1, outBufLen);

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
        return PXResultInvalid;
    }


    PXNetworkAdapter* pxNetworkAdapterList = PXMemoryHeapCallocT(PXNetworkAdapter, 10);



    // If successful, output some information from the data we received
    PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;

    for(PXSize index = 0; pCurrAddresses; ++index)
    {
        PXNetworkAdapter PXREF pxNetworkAdapter = &pxNetworkAdapterList[index];
        pxNetworkAdapter->MTU = pCurrAddresses->Mtu;

        pxNetworkAdapter->SpeedTransmit = pCurrAddresses->TransmitLinkSpeed;
        pxNetworkAdapter->SpeedRecieve = pCurrAddresses->ReceiveLinkSpeed;


        PXTextCopyWA(pCurrAddresses->Description, PXTextUnkownLength, pxNetworkAdapter->Description, 64);
        PXTextCopyWA(pCurrAddresses->FriendlyName, PXTextUnkownLength, pxNetworkAdapter->Name, 32);

        PXMemoryCopyX(pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength, pxNetworkAdapter->MACAddress.Data, 6);


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
            pxNetworkAdapter->MACAddress.Data[0],
            pxNetworkAdapter->MACAddress.Data[1],
            pxNetworkAdapter->MACAddress.Data[2],
            pxNetworkAdapter->MACAddress.Data[3],
            pxNetworkAdapter->MACAddress.Data[4],
            pxNetworkAdapter->MACAddress.Data[5],
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

            PXConsoleWrite(0, 0);
        }

        //  std::cout << "Gateway: ";
        for(PIP_ADAPTER_GATEWAY_ADDRESS_LH gateway = pCurrAddresses->FirstGatewayAddress; gateway; gateway = gateway->Next) {
            char gatewayStr[46];
            inet_ntop(gateway->Address.lpSockaddr->sa_family, &((struct sockaddr_in*)gateway->Address.lpSockaddr)->sin_addr, gatewayStr, sizeof(gatewayStr));
            // std::cout << gatewayStr << "\n";

            PXConsoleWrite(0, 0);
        }



        for(PIP_ADAPTER_UNICAST_ADDRESS unicast = pCurrAddresses->FirstUnicastAddress; unicast; unicast = unicast->Next)
        {
            char* ipTypeName = 0;
            char ipStr[46]; // IPv6 max size (IPv4 fits too)
            LPSOCKADDR socketAddress = unicast->Address.lpSockaddr;

            int family = socketAddress->sa_family;

            switch(family)
            {
                case AF_INET:
                {
                    inet_ntop(AF_INET, &((struct sockaddr_in*)socketAddress)->sin_addr, ipStr, sizeof(ipStr));
                    ipTypeName = "IPv4";
                    break;
                }
                case AF_INET6:
                {
                    inet_ntop(AF_INET6, &((struct sockaddr_in6*)socketAddress)->sin6_addr, ipStr, sizeof(ipStr));
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

#endif

    return PXResultOK;
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

IPAddressFamily PXAPI PXIPAddressFamilyFromID(const PXI8U ipMode)
{
    switch(ipMode)
    {
        case IPAF_UNSPEC:
            return IPAddressFamilyUnspecified;
        case IPAF_UNIX:
            return IPAddressFamilyUNIX;
        case IPAF_INET:
            return IPAddressFamilyINET;
        case IPAF_IMPLINK:
            return IPAddressFamilyIMPLINK;
        case IPAF_PUP:
            return IPAddressFamilyPUP;
        case IPAF_CHAOS:
            return IPAddressFamilyCHAOS;
        case IPAF_NS:
            return IPAddressFamilyNS;
            //case IPAF_IPX: return IPAddressFamilyIPX;
        case IPAF_ISO:
            return IPAddressFamilyISO;
            //case IPAF_OSI: return IPAddressFamilyOSI;
        case IPAF_ECMA:
            return IPAddressFamilyECMA;
        case IPAF_DATAKIT:
            return IPAddressFamilyDATAKIT;
        case IPAF_CCITT:
            return IPAddressFamilyCCITT;
        case IPAF_SNA:
            return IPAddressFamilySNA;
        case IPAF_DECnet:
            return IPAddressFamilyDECnet;
        case IPAF_DLI:
            return IPAddressFamilyDLI;
        case IPAF_LAT:
            return IPAddressFamilyLAT;
        case IPAF_HYLINK:
            return IPAddressFamilyHYLINK;
        case IPAF_APPLETALK:
            return IPAddressFamilyAPPLETALK;
        case IPAF_NETBIOS:
            return IPAddressFamilyNETBIOS;
        case IPAF_VOICEVIEW:
            return IPAddressFamilyVOICEVIEW;
        case IPAF_FIREFOX:
            return IPAddressFamilyFIREFOX;
        case IPAF_UNKNOWN1:
            return IPAddressFamilyUNKNOWN1;
        case IPAF_BAN:
            return IPAddressFamilyBAN;
            //case IPAF_ATM: return IPAddressFamilyATM;
        case IPAF_INET6:
            return IPAddressFamilyINET6;
        case IPAF_CLUSTER:
            return IPAddressFamilyCLUSTER;
        case IPAF_12844:
            return IPAddressFamilyIEEE12844;
        case IPAF_IRDA:
            return IPAddressFamilyIRDA;
        case IPAF_NETDES:
            return IPAddressFamilyNETDES;

        default:
            return IPAddressFamilyInvalid;
    }
}

PXI8U PXAPI PXIPAddressFamilyToID(const IPAddressFamily ipMode)
{
    switch(ipMode)
    {
        default:
        case IPAddressFamilyInvalid:
            return IPAF_Invalid;

        case IPAddressFamilyUnspecified:
            return IPAF_UNSPEC;

        case IPAddressFamilyUNIX:
            return IPAF_UNIX;

        case IPAddressFamilyINET:
            return IPAF_INET;

        case IPAddressFamilyIMPLINK:
            return IPAF_IMPLINK;

        case IPAddressFamilyPUP:
            return IPAF_PUP;

        case IPAddressFamilyCHAOS:
            return IPAF_CHAOS;

        case IPAddressFamilyNS:
            return IPAF_NS;

        case IPAddressFamilyIPX:
            return IPAF_IPX;

        case IPAddressFamilyISO:
            return IPAF_ISO;

        case IPAddressFamilyOSI:
            return IPAF_OSI;

        case IPAddressFamilyECMA:
            return IPAF_ECMA;

        case IPAddressFamilyDATAKIT:
            return IPAF_DATAKIT;

        case IPAddressFamilyCCITT:
            return IPAF_CCITT;

        case IPAddressFamilySNA:
            return IPAF_SNA;

        case IPAddressFamilyDECnet:
            return IPAF_DECnet;

        case IPAddressFamilyDLI:
            return IPAF_DLI;

        case IPAddressFamilyLAT:
            return IPAF_LAT;

        case IPAddressFamilyHYLINK:
            return IPAF_HYLINK;

        case IPAddressFamilyAPPLETALK:
            return IPAF_APPLETALK;

        case IPAddressFamilyNETBIOS:
            return IPAF_NETBIOS;

        case IPAddressFamilyVOICEVIEW:
            return IPAF_VOICEVIEW;

        case IPAddressFamilyFIREFOX:
            return IPAF_FIREFOX;

        case IPAddressFamilyUNKNOWN1:
            return IPAF_UNKNOWN1;

        case IPAddressFamilyBAN:
            return IPAF_BAN;

        case IPAddressFamilyATM:
            return IPAF_ATM;

        case IPAddressFamilyINET6:
            return IPAF_INET6;

        case IPAddressFamilyCLUSTER:
            return IPAF_CLUSTER;

        case IPAddressFamilyIEEE12844:
            return IPAF_12844;

        case IPAddressFamilyIRDA:
            return IPAF_IRDA;

        case IPAddressFamilyNETDES:
            return IPAF_NETDES;

        case IPAddressFamilyMAX:
            return IPAF_MAX;

        case IPAddressFamilyLINK:
            return IPAF_LINK;

        case IPAddressFamilyHYPERV:
            return IPAF_HYPERV;

        case IPAddressFamilyBTH:
            return IPAF_BTH;

        case IPAddressFamilyTCNPROCESS:
            return IPAF_TCNPROCESS;

        case IPAddressFamilyTCNMESSAGE:
            return IPAF_TCNMESSAGE;

        case IPAddressFamilyICLFXBM:
            return IPAF_ICLFXBM;
    }
}

#if OSWindows
PXResult PXAPI PXWindowsSocketAgentErrorFetch(PXNetwork PXREF pxNetwork)
{
    const int wsaErrorID = pxNetwork->SocketSystemErrorLastGet();
    const PXResult pxResult = PXWindowsSocketAgentErrorFromID(wsaErrorID);

    return pxResult;
}

PXResult PXAPI PXWindowsSocketAgentErrorFromID(const PXI32S errorID)
{
    switch(errorID)
    {
        case WSAEBADF: // fall through
            //  case WSA_INVALID_HANDLE: return PXResultRefusedNotFound; // Specified event object handle is invalid.
        case WSA_NOT_ENOUGH_MEMORY:
            return PXResultFailedMemoryAllocation; // Insufficient memory available.
        case WSAEINVAL: // fall through
        case WSA_INVALID_PARAMETER:
            return PXResultRefuedInputInvalid; // One or more parameters are invalid.
            //case WSA_OPERATION_ABORTED:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_IO_INCOMPLETE: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
            //case WSA_IO_PENDING: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSAEINTR:
            return PXResultInterruptedByFunctionCall; // xxxxxxxxxxxxxxxxx
            //   case WSAEACCES: return PXResultRefuedPermissionDenied;
        case WSAEFAULT:
            return PXResultRefuedAddressInvalid;
        case WSAEMFILE:
            return PXResultFailedResourcedNotEnough;
        case WSAEWOULDBLOCK:
            return PXResultWouldBlock; // xxxxxxxxxxxxxxxxx
        case WSAEINPROGRESS:
            return PXResultNowInProgress; // xxxxxxxxxxxxxxxxx
        case WSAEALREADY:
            return PXResultAlreadyInProgress; // xxxxxxxxxxxxxxxxx
            //  case WSAENOTSOCK: return PXResultRefusedNotFound; // Socket operation on nonsocket.
            //case WSAEDESTADDRREQ:  return xxxxxxxxxxxx; // Destination address required.
        case WSAEMSGSIZE:
            return PXResultRefuedInputBufferTooBig; // Message too long.
        case WSAEPROTOTYPE:
            return PXResultRefuedProtocolTypeInvalid; // Protocol wrong type for socket.
        case WSAENOPROTOOPT:
            return PXResultRefuedProtocolOptionInvalid; // Bad protocol option.
        case WSAEPROTONOSUPPORT:
            return PXResultRefuedProtocolNotSupported; // Protocol not supported.
            // case WSAESOCKTNOSUPPORT: return PXResultRefuedObjectTypeNotSupported; // Socket type not supported.
        case WSAEOPNOTSUPP:
            return PXResultRefuedOperationNotSupported; // Operation not supported.
        case WSAEPFNOSUPPORT:
            return PXResultRefuedProtocolFamilyNotSupported; // Protocol family not supported.
        case WSAEAFNOSUPPORT:
            return PXResultRefuedAddressFamilyNotSupportedByProtocol; // Address family not supported by protocol family.
        case WSAEADDRINUSE:
            return PXResultRefuedAlreadyInUse; // Address already in use.
            //case WSAEADDRNOTAVAIL:   return xxxxxxxxxxxx; // Cannot assign requested address.
        case WSAENETDOWN:
            return PXResultRefuedNetworkNotConnected; // Network is down.
        case WSAENETUNREACH:
            return PXResultRefuedNetworkNotReachable; // Network is unreachable.
        case WSAENETRESET:
            return PXResultFailedConnectionTerminatedByNetwork; // Network dropped connection on reset.
        case WSAECONNABORTED:
            return PXResultFailedConnectionTerminatedByOwnSoftware; // Software caused connection abort.
        case WSAECONNRESET:
            return PXResultFailedConnectionTerminatedByPeer; // Connection reset by peer.
        case WSAENOBUFS:
            return PXResultFailedMemoryAllocationInternal; // No buffer space available.
        case WSAEISCONN:
            return PXResultRefusedAlreadyConnected; // Socket is already connected.
        case WSAENOTCONN:
            return PXResultFailedNotConnected; // Socket is not connected.
            //case WSAESHUTDOWN:  return WindowsSocketSystemWasShutdown; // Cannot send after socket shutdown.
            //case WSAETOOMANYREFS:   return xxxxxxxxxxxx; // Too many references.
            //case WSAETIMEDOUT:   return xxxxxxxxxxxx; // Connection timed out.
        case WSAECONNREFUSED:
            return PXResultRefuedServiceNotRunning; // Connection refused.
            //case WSAELOOP:   return xxxxxxxxxxxx; // Cannot translate name.
            //case WSAENAMETOOLONG:   return xxxxxxxxxxxx; // Name too long.
            //case WSAEHOSTDOWN:   return xxxxxxxxxxxx; // Host is down.
            //case WSAEHOSTUNREACH:   return xxxxxxxxxxxx; // No route to host.
        case WSAENOTEMPTY:
            return PXResultFailedDirectoryIsNotEmpty; // Directory not empty.
        case WSAEPROCLIM:
            return PXResultFailedTooManyProcesses; // Too many processes.
        case WSAEUSERS:
            return PXResultFailedUserQuotaExceeded; // User quota exceeded.
        case WSAEDQUOT:
            return PXResultFailedDiskQuotaExceeded; // Disk quota exceeded.
        case WSAESTALE:
            return PXResultFailedHandleIsStale; // Stale file handle reference.
        case WSAEREMOTE:
            return PXResultFailedResourceNotAvailableLocally; // Item is remote.
            //case WSASYSNOTREADY:   return PXResultFailedNetworkSubsystemNotReady; // Network subsystem is unavailable.
            //case WSAVERNOTSUPPORTED:   return WindowsSocketVersionNotSupported; // Winsock.dll version out of range.
            //case WSANOTINITIALISED:   return WindowsSocketSystemNotInitialized; // Successful WSAStartup not yet performed.
            //case WSAEDISCON:   return PXResultRefusedResourceIsShuttingdown; // Graceful shutdown in progress.
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
            return PXResultFailedHostNotFound; // Host not found.
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
            return PXResultInvalid;
    }
}
#endif
