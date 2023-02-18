#include "PXSocket.h"

#include <Memory/PXMemory.h>
#include <Text/Text.h>
#include <Event/Event.h>
#include <Math/PXMath.h>
#include <stdio.h>

#if OSUnix
#include <poll.h>
#define OSSocketPoll poll
#elif OSWindows
#pragma comment(lib, "Ws2_32.lib")
#define OSSocketPoll WSAPoll
#endif


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

ProtocolMode ConvertToProtocolMode(const unsigned int protocolMode)
{
    switch(protocolMode)
    {
        case ProtocolHOPOPTS:
            return ProtocolModeHOPOPTS;

        case ProtocolICMP:
            return ProtocolModeICMP;

        case ProtocolIGMP:
            return ProtocolModeIGMP;

        case ProtocolGGP:
            return ProtocolModeGGP;

        case ProtocolIPV4:
            return ProtocolModeIPV4;

        case ProtocolST:
            return ProtocolModeST;

        case ProtocolTCP:
            return ProtocolModeTCP;

        case ProtocolCBT:
            return ProtocolModeCBT;

        case ProtocolEGP:
            return ProtocolModeEGP;

        case ProtocolIGP:
            return ProtocolModeIGP;

        case ProtocolPUP:
            return ProtocolModePUP;

        case ProtocolUDP:
            return ProtocolModeUDP;

        case ProtocolIDP:
            return ProtocolModeIDP;

        case ProtocolRDP:
            return ProtocolModeRDP;

        case ProtocolIPV6:
            return ProtocolModeIPV6;

        case ProtocolROUTING:
            return ProtocolModeROUTING;

        case ProtocolFRAGMENT:
            return ProtocolModeFRAGMENT;

        case ProtocolESP:
            return ProtocolModeESP;

        case ProtocolAH:
            return ProtocolModeAH;

        case ProtocolICMPV6:
            return ProtocolModeICMPV6;

        case ProtocolNONE:
            return ProtocolModeNONE;

        case ProtocolDSTOPTS:
            return ProtocolModeDSTOPTS;

        case ProtocolND:
            return ProtocolModeND;

        case ProtocolICLFXBM:
            return ProtocolModeICLFXBM;

        case ProtocolPIM:
            return ProtocolModePIM;

        case ProtocolPGM:
            return ProtocolModePGM;

        case ProtocolL2TP:
            return ProtocolModeL2TP;

        case ProtocolSCTP:
            return ProtocolModeSCTP;

        case ProtocolRAW:
            return ProtocolModeRAW;

        case ProtocolMAX:
            return ProtocolModeMAX;

        case ProtocolWindowsRAW:
            return ProtocolModeWindowsRAW;

        case ProtocolWindowsIPSEC:
            return ProtocolModeWindowsIPSEC;

        case ProtocolWindowsIPSECOFFLOAD:
            return ProtocolModeWindowsIPSECOFFLOAD;

        case ProtocolWindowsWNV:
            return ProtocolModeWindowsWNV;

        case ProtocolWindowsMAX:
            return ProtocolModeWindowsMAX;

        default:
            return ProtocolModeInvalid;
    }
}

unsigned int ConvertFromProtocolMode(const ProtocolMode protocolMode)
{
    switch(protocolMode)
    {
        default:
        case ProtocolModeInvalid:
            return ProtocolInvalid;

        case ProtocolModeHOPOPTS:
            return ProtocolHOPOPTS;

        case ProtocolModeICMP:
            return ProtocolICMP;

        case ProtocolModeIGMP:
            return ProtocolIGMP;

        case ProtocolModeGGP:
            return ProtocolGGP;

        case ProtocolModeIPV4:
            return ProtocolIPV4;

        case ProtocolModeST:
            return ProtocolST;

        case ProtocolModeTCP:
            return ProtocolTCP;

        case ProtocolModeCBT:
            return ProtocolCBT;

        case ProtocolModeEGP:
            return ProtocolEGP;

        case ProtocolModeIGP:
            return ProtocolIGP;

        case ProtocolModePUP:
            return ProtocolPUP;

        case ProtocolModeUDP:
            return ProtocolUDP;

        case ProtocolModeIDP:
            return ProtocolIDP;

        case ProtocolModeRDP:
            return ProtocolRDP;

        case ProtocolModeIPV6:
            return ProtocolIPV6;

        case ProtocolModeROUTING:
            return ProtocolROUTING;

        case ProtocolModeFRAGMENT:
            return ProtocolFRAGMENT;

        case ProtocolModeESP:
            return ProtocolESP;

        case ProtocolModeAH:
            return ProtocolAH;

        case ProtocolModeICMPV6:
            return ProtocolICMPV6;

        case ProtocolModeNONE:
            return ProtocolNONE;

        case ProtocolModeDSTOPTS:
            return ProtocolDSTOPTS;

        case ProtocolModeND:
            return ProtocolND;

        case ProtocolModeICLFXBM:
            return ProtocolICLFXBM;

        case ProtocolModePIM:
            return ProtocolPIM;

        case ProtocolModePGM:
            return ProtocolPGM;

        case ProtocolModeL2TP:
            return ProtocolL2TP;

        case ProtocolModeSCTP:
            return ProtocolSCTP;

        case ProtocolModeRAW:
            return ProtocolRAW;

        case ProtocolModeMAX:
            return ProtocolMAX;

        case ProtocolModeWindowsRAW:
            return ProtocolWindowsRAW;

        case ProtocolModeWindowsIPSEC:
            return ProtocolWindowsIPSEC;

        case ProtocolModeWindowsIPSECOFFLOAD:
            return ProtocolWindowsIPSECOFFLOAD;

        case ProtocolModeWindowsWNV:
            return ProtocolWindowsWNV;

        case ProtocolModeWindowsMAX:
            return ProtocolWindowsMAX;
    }
}

PXSocketType ConvertToSocketType(const unsigned int socketType)
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

unsigned int ConvertFromSocketType(const PXSocketType socketType)
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

IPAdressFamily ConvertToIPAdressFamily(const unsigned int ipMode)
{
    switch(ipMode)
    {
        case IPAF_UNSPEC: return IPAdressFamilyUnspecified;
        case IPAF_UNIX: return IPAdressFamilyUNIX;
        case IPAF_INET: return IPAdressFamilyINET;
        case IPAF_IMPLINK: return IPAdressFamilyIMPLINK;
        case IPAF_PUP: return IPAdressFamilyPUP;
        case IPAF_CHAOS: return IPAdressFamilyCHAOS;
        case IPAF_NS: return IPAdressFamilyNS;
            //case IPAF_IPX: return IPAdressFamilyIPX;
        case IPAF_ISO: return IPAdressFamilyISO;
            //case IPAF_OSI: return IPAdressFamilyOSI;
        case IPAF_ECMA: return IPAdressFamilyECMA;
        case IPAF_DATAKIT: return IPAdressFamilyDATAKIT;
        case IPAF_CCITT: return IPAdressFamilyCCITT;
        case IPAF_SNA: return IPAdressFamilySNA;
        case IPAF_DECnet: return IPAdressFamilyDECnet;
        case IPAF_DLI: return IPAdressFamilyDLI;
        case IPAF_LAT: return IPAdressFamilyLAT;
        case IPAF_HYLINK: return IPAdressFamilyHYLINK;
        case IPAF_APPLETALK: return IPAdressFamilyAPPLETALK;
        case IPAF_NETBIOS: return IPAdressFamilyNETBIOS;
        case IPAF_VOICEVIEW: return IPAdressFamilyVOICEVIEW;
        case IPAF_FIREFOX: return IPAdressFamilyFIREFOX;
        case IPAF_UNKNOWN1: return IPAdressFamilyUNKNOWN1;
        case IPAF_BAN: return IPAdressFamilyBAN;
            //case IPAF_ATM: return IPAdressFamilyATM;
        case IPAF_INET6: return IPAdressFamilyINET6;
        case IPAF_CLUSTER: return IPAdressFamilyCLUSTER;
        case IPAF_12844: return IPAdressFamilyIEEE12844;
        case IPAF_IRDA: return IPAdressFamilyIRDA;
        case IPAF_NETDES: return IPAdressFamilyNETDES;

        default:
            return IPAdressFamilyInvalid;
    }
}

unsigned int ConvertFromIPAdressFamily(const IPAdressFamily ipMode)
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

void PXSocketConstruct(PXSocket* const pxSocket)
{
    MemoryClear(pxSocket, sizeof(PXSocket));
}

void PXSocketDestruct(PXSocket* const pxSocket)
{
}

PXActionResult PXSocketCreate
(
    PXSocket* const pxSocket,
    const IPAdressFamily adressFamily,
    const PXSocketType socketType,
    const ProtocolMode protocolMode
)
{
    const int ipAdressFamilyID = ConvertFromIPAdressFamily(adressFamily);
    const int socketTypeID = ConvertFromSocketType(socketType);
    const int protocolModeID = ConvertFromProtocolMode(protocolMode);

#if OSWindows
    {
        const PXActionResult permissionGranted = WindowsSocketAgentStartup();

        PXActionExitOnError(permissionGranted);
    }
#endif

    {
        const PXSize socketIDResult = socket(ipAdressFamilyID, socketTypeID, protocolModeID);
        const PXBool wasSucessful = socketIDResult != -1;

        if(!wasSucessful)
        {
#if SocketDebug
            printf("[PXSocket] Failed create.\n");
#endif

            return PXActionFailedSocketCreation;
        }

        pxSocket->ID = socketIDResult;

#if SocketDebug
        printf("[PXSocket] Created <%i>\n", pxSocket->ID);
#endif
    }

    pxSocket->Family = adressFamily;
    pxSocket->Protocol = protocolMode;
    pxSocket->Type = socketType;

    return PXActionSuccessful;
}

PXActionResult PXSocketConnect(PXSocket* const pxSocket)
{
    const int serverSocketID = connect(pxSocket->ID, (struct sockaddr*)pxSocket->IP, pxSocket->IPSize);
    const PXBool connected = serverSocketID != -1;

    if(!connected)
    {
#if SocketDebug
        printf("[PXSocket] Connect to server failed!\n");
#endif

        return PXActionFailedSocketConnect;
    }

#if SocketDebug
    printf("[PXSocket] Connected to server.\n");
#endif

    return PXActionSuccessful;
}

PXActionResult PXSocketSetupAdress
(
    PXSocket* const pxSocketList,
    const PXSize PXSocketListSizeMax,
    PXSize* PXSocketListSize,
    PXSocketAdressSetupInfo* const pxSocketAdressSetupInfoList,
    const PXSize pxSocketAdressSetupInfoListSize
)
{
#if OSWindows
    {
        const PXActionResult wsaResult = WindowsSocketAgentStartup();

        PXActionExitOnError(wsaResult);
    }
#endif

    MemoryClear(pxSocketList, sizeof(PXSocket) * PXSocketListSizeMax);

    *PXSocketListSize = 0;

    for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
    {
        PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo = &pxSocketAdressSetupInfoList[i];

        char portNumberString[30];
        char* portNumberStringAdress = 0;
        AdressInfoType adressHints;
        AdressInfoType* adressResult = 0;
        // ADRRINFOW?


        if (pxSocketAdressSetupInfo->Port != -1)
        {
            TextFromIntA(pxSocketAdressSetupInfo->Port, portNumberString, 30u);

            portNumberStringAdress = portNumberString;
        }

        adressHints.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
        adressHints.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
        adressHints.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
        adressHints.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);
        adressHints.ai_addrlen = 0;
        adressHints.ai_canonname = 0;
        adressHints.ai_addr = 0;
        adressHints.ai_next = 0;

        int adressInfoResult = getaddrinfo(pxSocketAdressSetupInfo->IP, portNumberString, &adressHints, &adressResult);

        switch (adressInfoResult)
        {
            case 0:
                break; // OK - Sucess

#if OSWindows
            case EAI_ADDRFAMILY:
                return PXActionRefusedHostHasNoNetworkAddresses;
#endif
            case EAI_AGAIN:
                return PXActionRefusedNameServerIsTemporaryOffline;

            case EAI_BADFLAGS:
                return PXActionRefusedSocketInvalidFlags;

            case EAI_FAIL:
                return PXActionRefusedNameServerIsPermanentOffline;

            case EAI_FAMILY:
                return PXActionRefusedRequestedAddressFamilyNotSupported;

            case EAI_MEMORY:
                return PXActionFailedAllocation;

                // case EAI_NODATA:
                 //    return SocketPXActionResultHostExistsButHasNoData;

                 //case EAI_NONAME:
                   //  return SocketPXActionResultIPOrPortNotKnown;

            case EAI_SERVICE:
                return RequestedServiceNotAvailableForSocket;

            case EAI_SOCKTYPE:
                return PXActionRefusedSocketTypeNotSupported;

#if OSWindows
            case WSANOTINITIALISED:
                return WindowsSocketSystemNotInitialized;
#endif
            default:
                // case EAI_SYSTEM:
            {
                // ErrorCode error = GetCurrentError();

                break;
            }
        }

        for (AdressInfoType* adressInfo = adressResult; adressInfo; adressInfo = adressInfo->ai_next)
        {
            PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfo->ai_addr;
            const char* result = 0;

            MemoryClear(pxSocket->IP, IPv6LengthMax);

            result = inet_ntop(adressInfo->ai_family, &ipv6->sin6_addr, pxSocket->IP, IPv6LengthMax);

            switch (adressInfo->ai_family)
            {
                case AF_INET:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfo->ai_addr)->sin_port));
                    break;
                }
                case AF_INET6:
                {
                    pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfo->ai_addr)->sin6_port));
                    break;
                }
                default:
                {
                    pxSocket->Port = 0;
                    break;
                }
            }

            pxSocket->Protocol = pxSocketAdressSetupInfo->ProtocolMode;
            pxSocket->Family = pxSocketAdressSetupInfo->IPMode;
            pxSocket->Type = pxSocketAdressSetupInfo->SocketType;
            pxSocket->IPSize = adressInfo->ai_addrlen;

            MemoryCopy(adressInfo->ai_addr, adressInfo->ai_addrlen, pxSocket->IP, IPv6LengthMax);
        }

        AdressInfoDelete(adressResult);
    }  

    return PXActionSuccessful;
}

PXBool PXSocketIsCurrentlyUsed(PXSocket* const pxSocket)
{
    return pxSocket->ID != -1;
}

void PXSocketClose(PXSocket* const pxSocket)
{
    // If not used, we cant close.
    {
        const PXBool isSocketUsed = PXSocketIsCurrentlyUsed(pxSocket);

        if(!isSocketUsed)
        {
            return;
        }
    }

#if OSUnix
    close(pxSocket->ID);
#elif OSWindows
    shutdown(pxSocket->ID, SD_SEND);
    closesocket(pxSocket->ID);
#endif

    InvokeEvent(pxSocket->EventList.ConnectionTerminatedCallback, pxSocket);

#if SocketDebug
    printf("[PXSocket] Closed <%i>\n", pxSocket->ID);
#endif

    pxSocket->ID = SocketIDOffline;
}

void PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState)
{
    pxSocket->State = socketState;
}

void PXSocketEventPull(PXSocket* const pxSocket, void* const buffer, const PXSize bufferSize)
{
    PXSocketStateChange(pxSocket, SocketEventPolling);

#if 0 // Use optimised OS function

    struct pollfd* socketDataList = buffer;
    PXSize socketDataListSize = pxSocket->SocketPollingReadListSize;
    int timeout = 50;

    for (PXSize i = 0; i < socketDataListSize; ++i)
    {
        struct pollfd* socketDataListCurrent = &socketDataList[i];

        socketDataListCurrent->fd = pxSocket->SocketPollingReadList[i];
        socketDataListCurrent->events = POLLRDNORM;
        socketDataListCurrent->revents = 0;
    }

    const int amount = OSSocketPoll(socketDataList, socketDataListSize, timeout); // Not thread safe??
    const PXBool success = amount != -1;

#if OSWindows
    if (!success)
    {
        const int wasError = WSAGetLastError();

        switch (wasError)
        {

            case  WSAENETDOWN: //              The network subsystem has failed.
                printf("[Polling] WSAENETDOWN\n");
                break;


            case      WSAEFAULT: //                An exception occurred while reading user input parameters.

                printf("[Polling] WSAEFAULT\n");
                break;

            case  WSAEINVAL: //                 An invalid parameter was passed.This error is returned if the WSAPOLLFD structures pointed to by the fdarray parameter when requesting socket status.This error is also returned if none of the sockets specified in the fd member of any of the WSAPOLLFD structures pointed to by the fdarray parameter were valid.

                printf("[Polling] WSAEINVAL\n");
                break;

            case  WSAENOBUFS: // The function was unable to allocate sufficient memory. 

                printf("[Polling] WSAENOBUFS\n");
                break;

            default:
                printf("[Polling] Error\n");
                break;
        }

        // PXActionResult action = GetCurrentError();


        return;
    }
#endif 

    for (PXSize i = 0; i < amount; i++)
    {
        const struct pollfd* const currentPollData = &socketDataList[i];

        switch (currentPollData->revents)
        {
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

#else

    const PXSize neededFetches = (pxSocket->SocketPollingReadListSize / FD_SETSIZE) + 1;

    const TIMEVAL time = { 3,0 };

    PXSize restValues = pxSocket->SocketPollingReadListSize;
    fd_set selectListenRead;

    for (PXSize i = 0; i < neededFetches; ++i)
    {
        const PXSocketID* const socketIDList = &pxSocket->SocketPollingReadList[i * FD_SETSIZE];
        const PXSize fdBlockSize = MathMinimumIU(restValues, FD_SETSIZE);
        const PXSize fdBlockSizeBytes = fdBlockSize * sizeof(PXSocketID);

        restValues -= fdBlockSize;

        selectListenRead.fd_count = fdBlockSize;

        MemoryCopy(socketIDList, fdBlockSizeBytes, &selectListenRead.fd_array, fdBlockSizeBytes);

        const int numberOfSocketEvents = select(0, &selectListenRead, 0, 0, 0);

        for (PXSize l = 0; l < numberOfSocketEvents; ++l)
        {
            const PXSocketID socketID = selectListenRead.fd_array[i];

            PXSocketReadPendingHandle(pxSocket, socketID);
        }
    }
#endif

    PXSocketStateChange(pxSocket, SocketIDLE);
}

void PXSocketEventReadRegister(PXSocket* const pxSocket, const PXSocketID socketID)
{
    PXSocketID* const socketIDRef = &pxSocket->SocketPollingReadList[pxSocket->SocketPollingReadListSize++];

    *socketIDRef = socketID;
}

void PXSocketEventReadUnregister(PXSocket* const pxSocket, const PXSocketID socketID)
{
    PXSize offset = 0;
    PXSocketID* socketIDRef = 0;

    for (offset = 0; offset < pxSocket->SocketPollingReadListSize; ++offset)
    {
        const PXSocketID compareID = pxSocket->SocketPollingReadList[offset];

        if (compareID == socketID)
        {
            // found
            socketIDRef = &pxSocket->SocketPollingReadList[offset];
            break;
        }
    }

    if (!socketIDRef)
    {
        return;
    }

    PXBool endOfList = offset + 1 == pxSocket->SocketPollingReadListSize;

    if (endOfList)
    {
        *socketIDRef = 0;
    }
    else
    {
        PXSize copySize = pxSocket->SocketPollingReadListSize - offset - 1;

        MemoryMove(socketIDRef + 1, copySize, socketIDRef, copySize);
    }

    --(pxSocket->SocketPollingReadListSize);
}

void PXSocketReadPendingHandle(PXSocket* const pxSocket, const PXSocketID socketID)
{
    if (pxSocket->ID == socketID)
    {
        PXSocket clientSocket;

        PXSocketConstruct(&clientSocket);

        // Set Events

        const PXActionResult actionResult = PXSocketAccept(pxSocket, &clientSocket);
        const PXBool successful = PXActionSuccessful == actionResult;

        if (!successful)
        {
            //InvokeEvent(server->PXClientAcceptFailureCallback, serverSocket);
            return;
        }

        // InvokeEvent(server->PXClientConnectedCallback, serverSocket, &clientSocket);

        PXSocketEventReadRegister(pxSocket, clientSocket.ID);
    }
    else
    {
        PXSocket clientSocket;

        PXSocketConstruct(&clientSocket);

        clientSocket.Owner = pxSocket->Owner;
        clientSocket.ID = socketID;
        clientSocket.EventList = pxSocket->EventList;

        char inputBuffer[1024];
        PXSize wrrit = 0;

        PXSocketEventReadUnregister(pxSocket, clientSocket.ID);

        PXSocketReceive(&clientSocket, inputBuffer, 1024, &wrrit);
    }
}

PXActionResult PXSocketBind(PXSocket* const pxSocket)
{
    const int bindingResult = bind(pxSocket->ID, (struct sockaddr*)pxSocket->IP, pxSocket->IPSize);
    const PXBool sucessful = bindingResult == 0; //-1

    if(!sucessful)
    {
#if SocketDebug
        printf("[PXSocket] Binding <%i> failed!\n", pxSocket->ID);
#endif
        return PXActionFailedSocketBinding;
    }

#if SocketDebug
    printf("[PXSocket] Bound <%i>.\n", pxSocket->ID);
#endif

    return PXActionSuccessful;
}

PXActionResult PXSocketOptionsSet(PXSocket* const pxSocket)
{
    if(pxSocket->ID == -1)
    {
        return PXActionInvalid;
    }

    const int level = SOL_SOCKET;

    const int optionName =
#if OSUnix
        SO_REUSEADDR;      // Do not use SO_REUSEADDR, else the port can be hacked. SO_REUSEPORT
#elif OSWindows
        SO_EXCLUSIVEADDRUSE;
#endif
    const int opval = 1;
    const int optionsocketResult = setsockopt(pxSocket->ID, level, optionName, &opval, sizeof(int));
    const PXBool sucessful = optionsocketResult == 0;

    PXActionOnErrorFetchAndExit(!sucessful)

    return PXActionSuccessful;
}

PXActionResult PXSocketListen(PXSocket* const pxSocket)
{
    const int maximalPXClientsWaitingInQueue = 10;
    const int listeningResult = listen(pxSocket->ID, maximalPXClientsWaitingInQueue);
    const PXBool sucessful = listeningResult == 0;

    if(listeningResult == -1)
    {
#if SocketDebug
        printf("[PXSocket] Listening failed!\n");
#endif

        return PXActionFailedSocketListening;
    }

#if SocketDebug
    printf("[PXSocket] Listening for connections...\n");
#endif

    return PXActionSuccessful;
}

PXActionResult PXSocketAccept(PXSocket* server, PXSocket* client)
{
    client->IPSize = IPv6LengthMax; // Needed for accept(), means 'length i can use'. 0 means "I canot perform"
    client->ID = accept
    (
        server->ID,
        (struct sockaddr*)client->IP,
#if OSUnix
        (socklen_t*)&client->IPSize
#elif OSWindows
        (int*)&client->IPSize
#endif
    );

    const PXBool sucessful = client->ID != -1;

    if(!sucessful)
    {
#if SocketDebug
        printf("[PXSocket] Connection accept failed!\n");
#endif

        return PXActionFailedSocketBinding;
    }

#if SocketDebug
    printf("[PXSocket] Connection accepted <%i>\n", client->ID);
#endif

    return PXActionSuccessful;
}

PXActionResult PXSocketSend(PXSocket* const pxSocket, const void* inputBuffer, const PXSize inputBufferSize, PXSize* inputBytesWritten)
{
    // Check if socket is active and ready to send
    {
        const PXBool isReady = PXSocketIsCurrentlyUsed(pxSocket);

        if(!isReady)
        {
            return PXActionRefusedSocketNotConnected;
        }
    }

    // Do we even send anything? If not, quit
    {
        const PXBool hasDataToSend = inputBuffer && inputBufferSize > 0; // if NULL or 0 Bytes, return

        if(!hasDataToSend)
        {
            return PXActionSuccessful; // Do not send anything if the message is empty
        }
    }

    InvokeEvent(pxSocket->EventList.MessageSendCallback, pxSocket, inputBuffer, inputBufferSize);

#if SocketDebug
    printf("[PXSocket] You --> <%zi> %li Bytes\n", pxSocket->ID, inputBufferSize);
#endif

    // Send data
    {
        const char* data = (const char*)inputBuffer;

        const int writtenBytes =
#if OSUnix
            write(pxSocket->ID, data, inputBufferSize);
#elif OSWindows
            send(pxSocket->ID, data, inputBufferSize, 0);
#endif
        const PXBool sucessfulSend = writtenBytes != -1;

        if(!sucessfulSend)
        {
            return PXActionFailedSocketSend;
        }

        if(inputBytesWritten) *inputBytesWritten = writtenBytes;
    }

    return PXActionSuccessful;
}

PXActionResult PXSocketReceive(PXSocket* const pxSocket, const void* outputBuffer, const PXSize outputBufferSize, PXSize* outputBytesWritten)
{
    // I did not read any data yet
    *outputBytesWritten = 0;

    // Check if socket is active and ready to send
    {
        const PXBool isReady = PXSocketIsCurrentlyUsed(pxSocket);

        if(!isReady)
        {
            return PXActionRefusedSocketNotConnected;
        }
    }

    // Read data
    {
        char* data = (char*)outputBuffer;
        int length = outputBufferSize;

        //StateChange(SocketStateDataReceiving);

        const unsigned int byteRead =
#if OSUnix
            read(pxSocket->ID, data, length);
#elif OSWindows
            recv(pxSocket->ID, data, length, 0);
#endif

        // StateChange(SocketStateIDLE);

        switch(byteRead)
        {
            case (unsigned int)-1:
                return PXActionFailedSocketRecieve;

            case 0:// endOfFile
            {
#if SocketDebug
                printf("[PXSocket] Connection <%i> close signal detected!\n", pxSocket->ID);
#endif

                PXSocketClose(pxSocket);

                return PXActionRefusedSocketNotConnected; // How to handle, 0 means connected but this is not the terminating phase.
            }
            default:
            {
                *outputBytesWritten = byteRead;

#if SocketDebug
                printf("[PXSocket] You <-- <%li> %i Bytes\n", pxSocket->ID, byteRead);
#endif

                InvokeEvent(pxSocket->EventList.MessageReceiveCallback, pxSocket, outputBuffer, byteRead);
            }
        }
    }
    return PXActionSuccessful;
}

#if OSWindows
PXActionResult WindowsSocketAgentStartup(void)
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    MemoryClear(&wsaData, sizeof(WSADATA));
    const int result = WSAStartup(wVersionRequested, &wsaData);

    switch(result)
    {
        case WSASYSNOTREADY:
            return SubSystemNotReady;

        case WSAVERNOTSUPPORTED:
            return VersionNotSupported;

        case WSAEINPROGRESS:
            return BlockedByOtherOperation;

        case WSAEPROCLIM:
            return LimitReached;

        case WSAEFAULT:
            return InvalidParameter;

        case 0:
        default:
            return PXActionSuccessful;
    }
}

PXActionResult WindowsSocketAgentShutdown(void)
{
    const int result = WSACleanup();

    switch(result)
    {
        case WSANOTINITIALISED:
        {
            return SubSystemNotInitialised;
        }
        case WSAENETDOWN:
        {
            return SubSystemNetworkFailed;
        }
        case WSAEINPROGRESS:
        {
            return SocketIsBlocking;
        }
        case 0:
        default:
            return PXActionSuccessful;
    }
}
#endif
