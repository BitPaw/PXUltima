#include "PXSocket.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Event/PXEvent.h>
#include <Math/PXMath.h>
#include <stdio.h>

#if OSUnix
#include <sys/select.h>
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
    PXMemoryClear(pxSocket, sizeof(PXSocket));

    PXDictionaryConstruct(&pxSocket->SocketLookup, sizeof(PXSocketID), sizeof(PXSocket), PXDictionaryValueLocalityInternalEmbedded);
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
    const PXSize socketListSizeMax,
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

    *PXSocketListSize = 0;

    for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
    {
        PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo = &pxSocketAdressSetupInfoList[i];

        PXText portText;
        PXTextConstructWithBufferNamedA(&portText, portTextBuffer, 30);
        char* portTextAdress = 0;


        portText.Format = pxSocketAdressSetupInfo->IP.Format;


        if (pxSocketAdressSetupInfo->Port != -1)
        {
            PXTextFromInt(pxSocketAdressSetupInfo->Port, &portText);

            portTextAdress = portText.TextA;
        }





        switch (pxSocketAdressSetupInfo->IP.Format)
        {
            case TextFormatASCII:
            case TextFormatUTF8:
            {
#if OSUnix
                struct addrinfo addressInfoHint;
                struct addrinfo addressInfoResult;

                PXMemoryClear(&addressInfoHint, sizeof(struct addrinfo));
                addressInfoHint.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
                addressInfoHint.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
                addressInfoHint.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHint.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);

                const int adressInfoResult = getaddrinfo(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHint, &addressInfoResult);
                const PXBool validAdressInfo = adressInfoResult == 0;

#elif OSWindows
                ADDRINFOA addressInfoHintA;
                ADDRINFOA* addressInfoListA = 0;

                PXMemoryClear(&addressInfoHintA, sizeof(ADDRINFOA));
                addressInfoHintA.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
                addressInfoHintA.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
                addressInfoHintA.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHintA.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);


                const int adressInfoResult = GetAddrInfoA(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHintA, &addressInfoListA);
                const PXBool validAdressInfo = adressInfoResult == 0;

                // Yeet?

                for (ADDRINFOA* adressInfoCurrent = addressInfoListA; adressInfoCurrent; adressInfoCurrent = adressInfoCurrent->ai_next)
                {
                    PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
                    struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfoCurrent->ai_addr;
                    const char* result = 0;

                    PXMemoryClear(pxSocket->IP, IPv6LengthMax);

                    result = inet_ntop(adressInfoCurrent->ai_family, &ipv6->sin6_addr, pxSocket->IP, IPv6LengthMax);

                    switch (adressInfoCurrent->ai_family)
                    {
                    case AF_INET:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfoCurrent->ai_addr)->sin_port));
                        break;
                    }
                    case AF_INET6:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfoCurrent->ai_addr)->sin6_port));
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
                    pxSocket->IPSize = adressInfoCurrent->ai_addrlen;

                    PXMemoryCopy(adressInfoCurrent->ai_addr, adressInfoCurrent->ai_addrlen, pxSocket->IP, IPv6LengthMax);

                    pxSocket->State = SocketInitialised;
                }

                FreeAddrInfoA(addressInfoListA);

#endif
                break;
            }
            case TextFormatUNICODE:
            {
#if OSUnix
#elif OSWindows
                ADDRINFOW addressInfoHintW;
                ADDRINFOW* addressInfoListW = 0;

                PXMemoryClear(&addressInfoHintW, sizeof(ADDRINFOA));
                addressInfoHintW.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
                addressInfoHintW.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
                addressInfoHintW.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHintW.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);

                const int adressInfoResultID = GetAddrInfoW(pxSocketAdressSetupInfo->IP.TextW, portTextAdress, &addressInfoHintW, &addressInfoListW); // Windows Vista, Ws2_32.dll, ws2tcpip.h
                const PXBool validAdressInfo = adressInfoResultID == 0;


                for (ADDRINFOW* adressInfoCurrent = addressInfoListW; adressInfoCurrent; adressInfoCurrent = adressInfoCurrent->ai_next)
                {
                    PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
                    struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfoCurrent->ai_addr;
                    const char* result = 0;

                    PXMemoryClear(pxSocket->IP, IPv6LengthMax);

                    result = inet_ntop(adressInfoCurrent->ai_family, &ipv6->sin6_addr, pxSocket->IP, IPv6LengthMax);

                    switch (adressInfoCurrent->ai_family)
                    {
                    case AF_INET:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfoCurrent->ai_addr)->sin_port));
                        break;
                    }
                    case AF_INET6:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfoCurrent->ai_addr)->sin6_port));
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
                    pxSocket->IPSize = adressInfoCurrent->ai_addrlen;

                    PXMemoryCopy(adressInfoCurrent->ai_addr, adressInfoCurrent->ai_addrlen, pxSocket->IP, IPv6LengthMax);

                    pxSocket->State = SocketInitialised;
                }

                FreeAddrInfoW(addressInfoListW);

#endif

                break;
            }


        }




       /*

        if (!validAdressInfo)
        {
           // ...
        }



        if(i == 0)
        {


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
        }*/



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
    printf("[PXSocket] <%i> Terminated\n", (int)pxSocket->ID);
#endif

    pxSocket->ID = SocketIDOffline;
}

void PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState)
{
    pxSocket->State = socketState;
}

PXActionResult PXSocketEventPull(PXSocket* const pxSocket, void* const buffer, const PXSize bufferSize)
{
    PXSocketStateChange(pxSocket, SocketEventPolling);

    if (pxSocket->PollingLock)
    {
        PXLockEngage(pxSocket->PollingLock);
    }


#if 0 // Use optimised OS function

    const PXSize socketDataListSize = pxSocket->SocketPollingReadListSize;
    struct pollfd* socketDataList = buffer;
    int timeout = 50;

    for (PXSize i = 0; i < socketDataListSize; ++i)
    {
        struct pollfd* pollingEntry = &socketDataList[i];

        pollingEntry->fd = pxSocket->SocketPollingReadList[i]; // SocketID
        pollingEntry->events = POLLRDNORM; // Poll-Read-Normal
        pollingEntry->revents = 0; // Will be the result of the call
    }


    int amount = OSSocketPoll(socketDataList, socketDataListSize, timeout); // Not thread safe??
    const PXBool success = amount != -1;


    if (!success)
    {
        amount = 0;
#if 0

        PXActionOnErrorFetchAndExit(!success);
        #endif
    }






    for (PXSize i = 0; i < amount; i++)
    {
        const struct pollfd* const currentPollData = &socketDataList[i];

        switch (currentPollData->revents)
        {
            case 0: // Illegal state, undefined
            {
                const PXSocketID clientID = currentPollData->fd;
                const PXBool isRegistered = PXSocketIsRegistered(pxSocket, clientID);

                if (isRegistered)
                {
                    PXSocket clientSocket;
                    PXSocketConstruct(&clientSocket);

                    clientSocket.Owner = pxSocket->Owner;
                    clientSocket.ID = clientID;
                    clientSocket.EventList = pxSocket->EventList;

                    char inputBuffer[1024];
                    PXSize wrrit = 0;

                    const PXActionResult succ = PXSocketReceive(&clientSocket, inputBuffer, 1024, &wrrit);

                    if (succ != PXThreadSucessful)
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

    if (pxSocket->PollingLock)
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

    for (PXSize i = 0; i < neededFetches; ++i) // All sockets
    {
        fd_set selectListenRead; // is a struct containing an int and a fixed list of int[64]
        PXSize j = 0;
        PXSize highestSocketID = 0;

        PXMemoryClear(&selectListenRead, sizeof(selectListenRead));

        for ( ; restValues > 0 && j < FD_SETSIZE; ++j) // all in 64x blocks
        {
            const PXSize index = i * FD_SETSIZE + j;

            PXDictionaryEntry pxDictionaryEntry;
            PXDictionaryIndex(&pxSocket->SocketLookup, socketIDIndex++, &pxDictionaryEntry);

            const PXSocketID socketID = *(PXSocketID*)pxDictionaryEntry.Key;

            if (socketID == (PXSocketID)-1)
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

        int numberOfSocketEvents = select(highestSocketID + 1, &selectListenRead, 0, 0, 0); // first parameter is ignored under windows
        const PXBool success = -1 != numberOfSocketEvents;

        if (!success)
        {
            int errotID = WSAGetLastError();

            numberOfSocketEvents = 0;
        }

        //PXActionOnErrorFetchAndExit(!success);

        for (PXSize readableSocketIndex = 0; readableSocketIndex < numberOfSocketEvents; ++readableSocketIndex)
        {
             #if OSUnix
              const PXSocketID socketID = 0;
        #elif OSWindows
          const PXSocketID socketID = selectListenRead.fd_array[readableSocketIndex];
        #endif


            const PXBool isServer = pxSocket->ID == socketID;

            if (isServer)
            {
                PXSocketAccept(pxSocket);
            }
            else
            {
                PXSocketReceiveAsServer(pxSocket, socketID);
            }
        }
    }

#else

for (PXSize i = 0; i < pxSocket->SocketPollingReadListSize; ++i)
{
    PXSocketReadPendingHandle(pxSocket, pxSocket->SocketPollingReadList[i]); // Will block
}

#endif

    if (pxSocket->PollingLock)
    {
        PXLockRelease(pxSocket->PollingLock);
    }

    PXSocketStateChange(pxSocket, SocketIDLE);

    return PXActionSuccessful;
}

PXActionResult PXSocketBind(PXSocket* const pxSocket)
{
    const int bindingResult = bind(pxSocket->ID, (struct sockaddr*)pxSocket->IP, pxSocket->IPSize);
    const PXBool sucessful = bindingResult == 0; //-1

    if(!sucessful)
    {
#if SocketDebug
        printf("[PXSocket] Binding <%i> failed!\n", (int)pxSocket->ID);
#endif
        return PXActionFailedSocketBinding;
    }

#if SocketDebug
    printf("[PXSocket] Bound <%i>.\n", (int)pxSocket->ID);
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

PXActionResult PXSocketAccept(PXSocket* const server)
{
    PXSocket clientSocket;
    PXSocketConstruct(&clientSocket);

    clientSocket.Owner = server->Owner;
    clientSocket.EventList = server->EventList;

    clientSocket.IPSize = IPv6LengthMax; // Needed for accept(), means 'length i can use'. 0 means "I canot perform"
    clientSocket.ID = accept
    (
        server->ID,
        (struct sockaddr*)clientSocket.IP,
#if OSUnix
        (socklen_t*)&clientSocket.IPSize
#elif OSWindows
        (int*)&clientSocket.IPSize
#endif
    );

    const PXBool sucessful = clientSocket.ID != -1;

    if(!sucessful)
    {
#if SocketDebug
        printf("[PXSocket] Connection accept failed!\n");
#endif

        return PXActionFailedSocketBinding;
    }

#if SocketDebug
    printf("[PXSocket] <%i> accepted <%i>\n", (int)server->ID, (int)clientSocket.ID);
#endif

    // Register Client
    {
        PXDictionaryAdd(&server->SocketLookup, &clientSocket.ID, &clientSocket);

        // Erros on add?
    }

    return PXActionSuccessful;
}

PXActionResult PXSocketSendAsServerToClient(PXSocket* const serverSocket, const PXSocketID clientID, const void* inputBuffer, const PXSize inputBufferSize)
{
    // Do we even send anything? If not, quit
    {
        const PXBool hasDataToSend = inputBuffer && inputBufferSize > 0; // if NULL or 0 Bytes, return

        if (!hasDataToSend)
        {
            return PXActionSuccessful; // Do not send anything if the message is empty
        }
    }

    PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo;
    pxSocketDataMoveEventInfo.SocketSending = serverSocket;
    pxSocketDataMoveEventInfo.SocketReceiving = clientID;
    pxSocketDataMoveEventInfo.Data = inputBuffer;
    pxSocketDataMoveEventInfo.DataSize = inputBufferSize;

    InvokeEvent(serverSocket->EventList.MessageSendCallback, serverSocket->Owner, &pxSocketDataMoveEventInfo);

#if SocketDebug
    printf("[PXSocket] <%i> --> <%i> %i Bytes\n", (int)serverSocket->ID, (int)clientID, (int)inputBufferSize);
#endif

    // Send data
    {
        const char* data = (const char*)inputBuffer;

        const int writtenBytes =
#if OSUnix
            write(clientID, data, inputBufferSize);
#elif OSWindows
            send(clientID, data, inputBufferSize, 0);
#endif
        const PXBool sucessfulSend = writtenBytes != -1;

        if (!sucessfulSend)
        {
            return PXActionFailedSocketSend;
        }

        //if (inputBytesWritten) *inputBytesWritten = writtenBytes;
    }

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

    {
        PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo;
        pxSocketDataMoveEventInfo.SocketSending = pxSocket;
        pxSocketDataMoveEventInfo.SocketReceiving = pxSocket->ID;
        pxSocketDataMoveEventInfo.Data = inputBuffer;
        pxSocketDataMoveEventInfo.DataSize = inputBufferSize;

        InvokeEvent(pxSocket->EventList.MessageSendCallback, pxSocket->Owner, &pxSocketDataMoveEventInfo);
    }

#if SocketDebug
    printf("[PXSocket] You --> <%i> %i Bytes\n", (int)pxSocket->ID, (int)inputBufferSize);
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
                printf("[PXSocket] Connection <%i> close signal detected!\n", (int)pxSocket->ID);
#endif

                PXSocketClose(pxSocket);

                return PXActionRefusedSocketNotConnected; // How to handle, 0 means connected but this is not the terminating phase.
            }
            default:
            {
                *outputBytesWritten = byteRead;

#if SocketDebug
                printf("[PXSocket] You <-- <%li> %i Bytes\n", (int)pxSocket->ID, (int)byteRead);
#endif

                PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo;
                pxSocketDataMoveEventInfo.SocketSending = pxSocket;
                pxSocketDataMoveEventInfo.SocketReceiving = pxSocket->ID;
                pxSocketDataMoveEventInfo.Data = outputBuffer;
                pxSocketDataMoveEventInfo.DataSize = outputBufferSize;

                InvokeEvent(pxSocket->EventList.MessageReceiveCallback, pxSocket->Owner, &pxSocketDataMoveEventInfo);
            }
        }
    }
    return PXActionSuccessful;
}

PXActionResult PXSocketReceiveAsServer(PXSocket* const serverSocket, const PXSocketID clientID)
{
    char buffer[1024];
    PXSize buffserSize = 1024;

    // Read data
    {
        //StateChange(SocketStateDataReceiving);

        const unsigned int byteRead =
#if OSUnix
            read(clientID, buffer, buffserSize);
#elif OSWindows
            recv(clientID, buffer, buffserSize, 0);
#endif

        // StateChange(SocketStateIDLE);

        switch (byteRead)
        {
            case (unsigned int)-1:
                return PXActionFailedSocketRecieve;

            case 0:// endOfFile
            {
#if SocketDebug
                printf("[PXSocket] Connection <%i> close signal detected!\n", (int)clientID);
#endif

                PXSocketClientRemove(serverSocket, clientID);

                return PXActionRefusedSocketNotConnected; // How to handle, 0 means connected but this is not the terminating phase.
            }
            default:
            {

#if SocketDebug
                printf("[PXSocket] <%i> <-- <%i> %i Bytes\n", (int)serverSocket->ID, (int)clientID, (int)byteRead);
#endif

                PXSocketDataMoveEventInfo pxSocketDataMoveEventInfo;
                pxSocketDataMoveEventInfo.SocketSending = serverSocket;
                pxSocketDataMoveEventInfo.SocketReceiving = clientID;
                pxSocketDataMoveEventInfo.Data = buffer;
                pxSocketDataMoveEventInfo.DataSize = byteRead;

                InvokeEvent(serverSocket->EventList.MessageReceiveCallback, serverSocket->Owner, &pxSocketDataMoveEventInfo);
            }
        }
    }
    return PXActionSuccessful;
}

PXActionResult PXSocketClientRemove(PXSocket* const serverSocket, const PXSocketID clientID)
{
    PXSocket clientSockket;

    // Extract client socket
    {
        const PXSocketID clientIDCopy = clientID;
        const PXBool success = PXDictionaryExtract(&serverSocket->SocketLookup, &clientID, &clientSockket);

        if (!success)
        {
            return PXActionFailedElementNotFound;
        }
    }

#if SocketDebug
    printf("[PXSocket] <%i> remove <%i>\n", (int)serverSocket->ID, (int)clientSockket.ID);
#endif

    PXSocketClose(&clientSockket);

    return PXActionSuccessful;
}

#if OSWindows
PXActionResult WindowsSocketAgentStartup(void)
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    PXMemoryClear(&wsaData, sizeof(WSADATA));
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
