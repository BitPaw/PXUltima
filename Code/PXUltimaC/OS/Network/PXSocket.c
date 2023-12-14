#include "PXSocket.h"

#if PXSocketUSE

#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
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

PXProtocolMode PXAPI PXProtocolModeFromID(const PXInt8U protocolMode)
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

PXInt8U PXAPI ConvertFromProtocolMode(const PXProtocolMode protocolMode)
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

PXSocketType PXAPI PXSocketTypeFromID(const PXInt8U socketType)
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

PXInt8U PXAPI PXSocketTypeToID(const PXSocketType socketType)
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

IPAdressFamily PXAPI PXIPAdressFamilyFromID(const PXInt8U ipMode)
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

PXInt8U PXAPI PXIPAdressFamilyToID(const IPAdressFamily ipMode)
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

void PXAPI PXSocketConstruct(PXSocket* const pxSocket)
{
    PXMemoryClear(pxSocket, sizeof(PXSocket));
    PXThreadConstruct(&pxSocket->CommunicationThread);
    PXDictionaryConstruct(&pxSocket->SocketLookup, sizeof(PXSocketID), sizeof(PXSocket), PXDictionaryValueLocalityInternalEmbedded);

    pxSocket->ID = PXSocketUnused;
}

void PXAPI PXSocketDestruct(PXSocket* const pxSocket)
{
    printf("[C] Socket destruct %i\n", pxSocket->ID);

    PXSocketClose(pxSocket);
    PXThreadDestruct(&pxSocket->CommunicationThread);
    //PXDictionaryDestruct(&pxSocket->SocketLookup);

    PXSocketConstruct(pxSocket);
}

PXActionResult PXAPI PXSocketCreate
(
    PXSocket* const pxSocket,
    const IPAdressFamily adressFamily,
    const PXSocketType socketType,
    const PXProtocolMode protocolMode
)
{
    const int ipAdressFamilyID = PXIPAdressFamilyToID(adressFamily);
    const int socketTypeID = PXSocketTypeToID(socketType);
    const int protocolModeID = ConvertFromProtocolMode(protocolMode);

#if OSWindows
    {
        const PXActionResult permissionGranted = WindowsSocketAgentStartup();

        PXActionReturnOnError(permissionGranted);
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

#if OSUnix
            return PXErrorCurrent();
#elif OSWindows
            return PXWindowsSocketAgentErrorFetch();
#endif
        }

        pxSocket->ID = socketIDResult;

#if SocketDebug
        printf("[PXSocket] Created <%i>\n", pxSocket->ID);
#endif
    }

    pxSocket->Family = adressFamily;
    pxSocket->Protocol = protocolMode;
    pxSocket->Type = socketType;

#if OSWindows
    // make non blocking
    u_long ul = 0;
    int nRet = ioctlsocket(pxSocket->ID, FIONBIO, &ul);
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketConnect(PXSocket* const pxClient, PXSocket* const pxServer)
{
    const int serverSocketID = connect(pxClient->ID, (struct sockaddr*)pxClient->IP, pxClient->IPSize);
    const PXBool connected = serverSocketID != -1;

    if(!connected)
    {
#if SocketDebug
        printf("[PXSocket] Connect to server failed!\n");
#endif

#if OSUnix
        return PXErrorCurrent();
#elif OSWindows
        return PXWindowsSocketAgentErrorFetch();
#endif
    }

#if SocketDebug
    printf("[PXSocket] Connected to server.\n");
#endif

    pxServer->ID = serverSocketID;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketSetupAdress
(
    PXSocket* const pxSocketList,
    const PXSize socketListSizeMax,
    PXSize* PXSocketListSize,
    const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfoList,
    const PXSize pxSocketAdressSetupInfoListSize
)
{
#if OSWindows
    {
        const PXActionResult wsaResult = WindowsSocketAgentStartup();

        PXActionReturnOnError(wsaResult);
    }
#endif


#if 1 // DEACTIAVTE when compile for windows XP

    *PXSocketListSize = 0;

    for (PXSize i = 0; i < pxSocketAdressSetupInfoListSize; ++i)
    {
        const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo = &pxSocketAdressSetupInfoList[i];

        PXText portText;
        PXTextConstructNamedBufferA(&portText, portTextBuffer, 30);
        char* portTextAdress = 0;


        portText.Format = pxSocketAdressSetupInfo->IP.Format;

        if (pxSocketAdressSetupInfo->Port != -1)
        {
            PXTextFromInt(&portText, pxSocketAdressSetupInfo->Port);

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
                addressInfoHint.ai_family = PXIPAdressFamilyToID(pxSocketAdressSetupInfo->IPMode);
                addressInfoHint.ai_socktype = PXSocketTypeToID(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHint.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);

                const int adressInfoResult = getaddrinfo(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHint, &addressInfoResult);
                const PXBool validAdressInfo = adressInfoResult == 0;

#elif OSWindow
                ADDRINFOA addressInfoHintA;
                ADDRINFOA* addressInfoListA = 0;

                PXMemoryClear(&addressInfoHintA, sizeof(ADDRINFOA));
                addressInfoHintA.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
                addressInfoHintA.ai_family = ConvertFromIPAdressFamily(pxSocketAdressSetupInfo->IPMode);
                addressInfoHintA.ai_socktype = ConvertFromSocketType(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHintA.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);


                const int adressInfoResult = GetAddrInfoA(pxSocketAdressSetupInfo->IP.TextA, portTextAdress, &addressInfoHintA, &addressInfoListA);
                const PXBool validAdressInfo = adressInfoResult == 0;

                if (!validAdressInfo)
                {
#if OSUnix
                    return PXErrorCurrent();
#elif OSWindows
                    return PXWindowsSocketAgentErrorFetch();
#endif
                }


                for (ADDRINFOA* adressInfoCurrent = addressInfoListA; adressInfoCurrent; adressInfoCurrent = adressInfoCurrent->ai_next)
                {
                    PXSocket* const pxSocket = &pxSocketList[(*PXSocketListSize)++];
                    struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)adressInfoCurrent->ai_addr;
                    const char* result = 0;

                    PXMemoryClear(pxSocket->IP, IPv6LengthMax);

                    result = inet_ntop(adressInfoCurrent->ai_family, &ipv6->sin6_addr, pxSocket->IP, IPv6LengthMax); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h 

                    switch (adressInfoCurrent->ai_family)
                    {
                    case AF_INET:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in*)adressInfoCurrent->ai_addr)->sin_port)); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h 
                        break;
                    }
                    case AF_INET6:
                    {
                        pxSocket->Port = ntohs((((struct sockaddr_in6*)adressInfoCurrent->ai_addr)->sin6_port)); // Windows Vista (+UWP), Ws2_32.dll, ws2tcpip.h 
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

                PXMemoryClear(&addressInfoHintW, sizeof(ADDRINFOW));
                addressInfoHintW.ai_flags = AI_PASSIVE;    // For wildcard IP address (AI_NUMERICHOST | AI_PASSIVE;)
                addressInfoHintW.ai_family = PXIPAdressFamilyToID(pxSocketAdressSetupInfo->IPMode);
                addressInfoHintW.ai_socktype = PXSocketTypeToID(pxSocketAdressSetupInfo->SocketType); // Datagram socket
                addressInfoHintW.ai_protocol = ConvertFromProtocolMode(pxSocketAdressSetupInfo->ProtocolMode);

                const int adressInfoResultID = GetAddrInfoW(pxSocketAdressSetupInfo->IP.TextW, (wchar_t*)portTextAdress, &addressInfoHintW, &addressInfoListW); // Windows Vista, Ws2_32.dll, ws2tcpip.h
                const PXBool validAdressInfo = adressInfoResultID == 0;

                if (!validAdressInfo)
                {
#if OSUnix
                    return PXErrorCurrent();
#elif OSWindows
                    return PXWindowsSocketAgentErrorFetch();
#endif
                }

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
    }
#endif

    return PXActionSuccessful;
}

PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket* const pxSocket)
{
    return pxSocket->ID != PXSocketUnused;
}

void PXAPI PXSocketClose(PXSocket* const pxSocket)
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

    PXFunctionInvoke(pxSocket->EventList.SocketClosedCallBack, pxSocket->Owner, pxSocket);

#if SocketDebug
    printf("[PXSocket] <%i> Terminated\n", (int)pxSocket->ID);
#endif

    //pxSocket->ID = -1;
    PXMemorySet(&pxSocket->ID, 0xFF, 4);
}

void PXAPI PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState)
{
    const PXSocketState oldState = pxSocket->State;

    pxSocket->State = socketState;

    PXFunctionInvoke(pxSocket->EventList.SocketStateChangedCallBack, pxSocket->Owner, pxSocket, oldState, socketState);
}

PXActionResult PXAPI PXSocketEventPull(PXSocket* const pxSocket)
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
#if OSUnix
            PXActionResult selectResult = PXErrorCurrent();
#elif OSWindows
            PXActionResult selectResult = PXWindowsSocketAgentErrorFetch();
#endif

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
                PXSocketReceive(pxSocket, socketID);
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

PXActionResult PXAPI PXSocketBind(PXSocket* const pxSocket)
{
    const int bindingResult = bind(pxSocket->ID, (struct sockaddr*)pxSocket->IP, pxSocket->IPSize);
    const PXBool sucessful = bindingResult == 0; //-1

    if(!sucessful)
    {
#if SocketDebug
        printf("[PXSocket] Binding <%i> failed!\n", (int)pxSocket->ID);
#endif

#if OSUnix
        return PXErrorCurrent();
#elif OSWindows
        return PXWindowsSocketAgentErrorFetch();
#endif
    }

#if SocketDebug
    printf("[PXSocket] Bound <%i>.\n", (int)pxSocket->ID);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketOptionsSet(PXSocket* const pxSocket)
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
    const char opval = 1;
    const int optionsocketResult = setsockopt(pxSocket->ID, level, optionName, &opval, sizeof(int));
    const PXBool sucessful = optionsocketResult == 0;

    if (!sucessful)
    {
#if OSUnix
        return PXErrorCurrent();
#elif OSWindows
        return PXWindowsSocketAgentErrorFetch();
#endif
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketListen(PXSocket* const pxSocket)
{
    const int maximalPXClientsWaitingInQueue = 10;
    const int listeningResult = listen(pxSocket->ID, maximalPXClientsWaitingInQueue);
    const PXBool sucessful = listeningResult == 0;

    if(listeningResult == -1)
    {
#if SocketDebug
        printf("[PXSocket] Listening failed!\n");
#endif

#if OSUnix
        return PXErrorCurrent();
#elif OSWindows
        return PXWindowsSocketAgentErrorFetch();
#endif
    }

#if SocketDebug
    printf("[PXSocket] Listening for connections...\n");
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketAccept(PXSocket* const server)
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

#if OSUnix
        return PXErrorCurrent();
#elif OSWindows
        return PXWindowsSocketAgentErrorFetch();
#endif
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

PXActionResult PXAPI PXSocketSend(PXSocket* const pxSocketSender, const PXSocketID pxSocketReceiverID)
{
    // Check if socket is active and ready to send
    {
        const PXBool isSenderReady = PXSocketIsCurrentlyUsed(pxSocketSender);

        if (!isSenderReady)
        {
            return PXActionRefuedObjectNotReady;
        }

        const PXBool isReceiverReady = pxSocketReceiverID != -1;

        if (!isReceiverReady)
        {
            return PXActionRefusedSocketNotConnected;
        }
    }

    // Do we even send anything? If not, quit
    {
        if (!pxSocketSender->BufferOutput.Data)
        {
            return PXActionRefuedOutputBufferNull;
        }

        const PXBool hasDataToSend = pxSocketSender->BufferOutput.SizeMaximum > 0;

        if (!hasDataToSend)
        {
            return PXActionDidNothing; // Do not send anything if the message is empty
        }
    }

    // Send data
    do
    {
        const char* dataAdress = (char*)((PXAdress)pxSocketSender->BufferOutput.Data + pxSocketSender->BufferOutput.SizeOffset);
        const int dataSize = pxSocketSender->BufferOutput.SizeMaximum - pxSocketSender->BufferOutput.SizeOffset;

        const int writtenBytes =
#if OSUnix
            write(pxSocketReceiverID, dataAdress, dataSize);
#elif OSWindows
            send(pxSocketReceiverID, dataAdress, dataSize, 0);
#endif
        const PXBool sucessfulSend = writtenBytes != -1;

        if (!sucessfulSend)
        {
#if OSUnix
            return PXErrorCurrent();
#elif OSWindows
            return PXWindowsSocketAgentErrorFetch();
#endif
        }

        pxSocketSender->BufferOutput.SizeOffset += writtenBytes;

        {
            PXSocketDataSendEventData pxSocketDataSendEventData;
            pxSocketDataSendEventData.SocketSending = pxSocketSender;
            pxSocketDataSendEventData.SocketReceiving = pxSocketReceiverID;
            pxSocketDataSendEventData.Data = dataAdress;
            pxSocketDataSendEventData.DataSize = writtenBytes;

            PXFunctionInvoke(pxSocketSender->EventList.SocketDataSendCallBack, pxSocketSender->Owner, &pxSocketDataSendEventData);
        }

        pxSocketSender->BufferOutput.SizeCurrent += writtenBytes;

#if SocketDebug
        printf("[PXSocket] You --> <%i> %i Bytes\n", (int)pxSocket->ID, (int)inputBufferSize);
#endif
    }
    while (pxSocketSender->BufferOutput.SizeMaximum != pxSocketSender->BufferOutput.SizeCurrent);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketReceive(PXSocket* const pxSocketReceiver, const PXSocketID pxSocketSenderID)
{
    // Check if socket is active and ready to send
    {
        const PXBool isReceiverReady = PXSocketIsCurrentlyUsed(pxSocketReceiver);

        if (!isReceiverReady)
        {
            return PXActionRefuedObjectNotReady;
        }

        const PXBool isSenderReady = pxSocketSenderID != -1;

        if (!isSenderReady)
        {
            return PXActionRefusedSocketNotConnected;
        }
    }

    // Read data
    {
        //StateChange(SocketStateDataReceiving);
        char* data = (char*)((PXAdress)pxSocketReceiver->BufferInput.Data + pxSocketReceiver->BufferInput.SizeCurrent);
        const int availableSize = pxSocketReceiver->BufferInput.SizeMaximum - pxSocketReceiver->BufferInput.SizeCurrent;

        PXMemoryClear(data, availableSize);

        const int sizeRead =
#if OSUnix
            read(pxSocketSenderID, data, availableSize);
#elif OSWindows
            recv(pxSocketSenderID, data, availableSize, 0); // MSG_PEEK
#endif



        // StateChange(SocketStateIDLE);

        switch (sizeRead)
        {
            case -1:
            {
#if OSUnix
                return PXErrorCurrent();
#elif OSWindows
                return PXWindowsSocketAgentErrorFetch();
#endif
            }

            case 0:// endOfFile
            {
#if SocketDebug
                printf("[PXSocket] Connection <%i> close signal detected!\n", (int)pxSocketSenderID);
#endif

                PXSocketClientRemove(pxSocketReceiver, pxSocketSenderID);

                return PXActionRefusedSocketNotConnected; // How to handle, 0 means connected but this is not the terminating phase.
            }
            default:
            {
                pxSocketReceiver->BufferInput.SizeCurrent += sizeRead;

#if SocketDebug
                printf("[PXSocket] <%i> <-- <%i> %i Bytes\n", (int)serverSocket->ID, (int)clientID, (int)byteRead);
#endif

                PXSocketDataReceivedEventData pxSocketDataReceivedEventData;
                pxSocketDataReceivedEventData.SocketSending = pxSocketSenderID;
                pxSocketDataReceivedEventData.SocketReceiving = pxSocketReceiver;
                pxSocketDataReceivedEventData.Data = data;
                pxSocketDataReceivedEventData.DataSize = sizeRead;

                PXFunctionInvoke(pxSocketReceiver->EventList.SocketDataReceiveCallBack, pxSocketReceiver->Owner, &pxSocketDataReceivedEventData);
            }
        }
    }
    return PXActionSuccessful;
}

PXActionResult PXAPI PXSocketClientRemove(PXSocket* const serverSocket, const PXSocketID clientID)
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
PXActionResult PXAPI WindowsSocketAgentStartup(void)
{
    const WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    PXMemoryClear(&wsaData, sizeof(WSADATA));
    const int result = WSAStartup(wVersionRequested, &wsaData);
    const PXBool successful = result == 0;

    if (successful)
    {
        return PXActionSuccessful;
    }

    return PXWindowsSocketAgentErrorFromID(result);
}

PXActionResult PXAPI WindowsSocketAgentShutdown(void)
{
    const int result = WSACleanup();
    const PXBool successful = result == 0;

    if (successful)
    {
        return PXActionSuccessful;
    }

    return PXWindowsSocketAgentErrorFromID(result);
}

PXActionResult PXAPI PXWindowsSocketAgentErrorFetch(void)
{
    const int wsaErrorID = WSAGetLastError();
    const PXActionResult pxActionResult = PXWindowsSocketAgentErrorFromID(wsaErrorID);

    return pxActionResult;
}

PXActionResult PXAPI PXWindowsSocketAgentErrorFromID(const PXInt32S errorID)
{
    switch (errorID)
    {
        case WSAEBADF: // fall through
        case WSA_INVALID_HANDLE: return PXActionRefusedObjectNotFound; // Specified event object handle is invalid.
        case WSA_NOT_ENOUGH_MEMORY: return PXActionFailedMemoryAllocation; // Insufficient memory available.
        case WSAEINVAL: // fall through
        case WSA_INVALID_PARAMETER: return PXActionRefuedInputInvalid; // One or more parameters are invalid.
        //case WSA_OPERATION_ABORTED:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        //case WSA_IO_INCOMPLETE: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        //case WSA_IO_PENDING: return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSAEINTR: return PXActionInterruptedByFunctionCall; // xxxxxxxxxxxxxxxxx
        case WSAEACCES: return PXActionRefuedPermissionDenied;
        case WSAEFAULT: return PXActionRefuedAdressInvalid;
        case WSAEMFILE: return PXActionFailedResourcedNotEnough;
        case WSAEWOULDBLOCK:  return PXActionWouldBlock; // xxxxxxxxxxxxxxxxx
        case WSAEINPROGRESS: return PXActionNowInProgress; // xxxxxxxxxxxxxxxxx
        case WSAEALREADY: return PXActionAlreadyInProgress; // xxxxxxxxxxxxxxxxx
        case WSAENOTSOCK: return PXActionRefusedObjectNotFound; // Socket operation on nonsocket.
        //case WSAEDESTADDRREQ:  return xxxxxxxxxxxx; // Destination address required.
        case WSAEMSGSIZE: return PXActionRefuedInputBufferTooBig; // Message too long.
        case WSAEPROTOTYPE: return PXActionRefuedProtocolTypeInvalid; // Protocol wrong type for socket.
        case WSAENOPROTOOPT: return PXActionRefuedProtocolOptionInvalid; // Bad protocol option.
        case WSAEPROTONOSUPPORT: return PXActionRefuedProtocolNotSupported; // Protocol not supported.
        case WSAESOCKTNOSUPPORT: return PXActionRefuedObjectTypeNotSupported; // Socket type not supported.
        case WSAEOPNOTSUPP: return PXActionRefuedOperationNotSupported; // Operation not supported.
        case WSAEPFNOSUPPORT:  return PXActionRefuedProtocolFamilyNotSupported; // Protocol family not supported.
        case WSAEAFNOSUPPORT:   return PXActionRefuedAddressFamilyNotSupportedByProtocol; // Address family not supported by protocol family.
        case WSAEADDRINUSE:   return PXActionRefuedAlreadyInUse; // Address already in use.
        //case WSAEADDRNOTAVAIL:   return xxxxxxxxxxxx; // Cannot assign requested address.
        case WSAENETDOWN:   return PXActionRefuedNetworkNotConnected; // Network is down.
        case WSAENETUNREACH:   return PXActionRefuedNetworkNotReachable; // Network is unreachable.
        case WSAENETRESET:   return PXActionFailedConnectionTerminatedByNetwork; // Network dropped connection on reset.
        case WSAECONNABORTED:   return PXActionFailedConnectionTerminatedByOwnSoftware; // Software caused connection abort.
        case WSAECONNRESET:   return PXActionFailedConnectionTerminatedByPeer; // Connection reset by peer.
        case WSAENOBUFS:   return PXActionFailedMemoryAllocationInternal; // No buffer space available.
        case WSAEISCONN:   return PXActionRefusedAlreadyConnected; // Socket is already connected.
        case WSAENOTCONN:   return PXActionFailedNotConnected; // Socket is not connected.
        case WSAESHUTDOWN:  return WindowsSocketSystemWasShutdown; // Cannot send after socket shutdown.
        //case WSAETOOMANYREFS:   return xxxxxxxxxxxx; // Too many references.
        //case WSAETIMEDOUT:   return xxxxxxxxxxxx; // Connection timed out.
        case WSAECONNREFUSED:   return PXActionRefuedServiceNotRunning; // Connection refused.
        //case WSAELOOP:   return xxxxxxxxxxxx; // Cannot translate name.
        //case WSAENAMETOOLONG:   return xxxxxxxxxxxx; // Name too long.
        //case WSAEHOSTDOWN:   return xxxxxxxxxxxx; // Host is down.
        //case WSAEHOSTUNREACH:   return xxxxxxxxxxxx; // No route to host.
        case WSAENOTEMPTY:   return PXActionFailedDirectoryIsNotEmpty; // Directory not empty.
        case WSAEPROCLIM:   return PXActionFailedTooManyProcesses; // Too many processes.
        case WSAEUSERS:   return PXActionFailedUserQuotaExceeded; // User quota exceeded.
        case WSAEDQUOT:   return PXActionFailedDiskQuotaExceeded; // Disk quota exceeded.
        case WSAESTALE:   return PXActionFailedHandleIsStale; // Stale file handle reference.
        case WSAEREMOTE:   return PXActionFailedResourceNotAvailableLocally; // Item is remote.
        case WSASYSNOTREADY:   return PXActionFailedNetworkSubsystemNotReady; // Network subsystem is unavailable.
        case WSAVERNOTSUPPORTED:   return WindowsSocketVersionNotSupported; // Winsock.dll version out of range.
        case WSANOTINITIALISED:   return WindowsSocketSystemNotInitialized; // Successful WSAStartup not yet performed.
        case WSAEDISCON:   return PXActionRefusedResourceIsShuttingdown; // Graceful shutdown in progress.
        case WSAENOMORE:   return xxxxxxxxxxxx; // No more results.
        case WSAECANCELLED:   return xxxxxxxxxxxx; // Call has been canceled.
        case WSAEINVALIDPROCTABLE:   return xxxxxxxxxxxx; // Procedure call table is invalid.
        case WSAEINVALIDPROVIDER:   return xxxxxxxxxxxx; // Service provider is invalid.
        case WSAEPROVIDERFAILEDINIT:   return xxxxxxxxxxxx; // Service provider failed to initialize
        case WSASYSCALLFAILURE:   return xxxxxxxxxxxx; // System call failure.
        case WSASERVICE_NOT_FOUND:   return xxxxxxxxxxxx; // Service not found.
        case WSATYPE_NOT_FOUND:   return xxxxxxxxxxxx; // Class type not found.
        case WSA_E_NO_MORE:   return xxxxxxxxxxxx; // No more results.
        case WSA_E_CANCELLED:   return xxxxxxxxxxxx; // Call was canceled.
        case WSAEREFUSED:   return xxxxxxxxxxxx; // Database query was refused.
        case WSAHOST_NOT_FOUND:   return PXActionFailedHostNotFound; // Host not found.
        case WSATRY_AGAIN:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSANO_RECOVERY:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSANO_DATA:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_RECEIVERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_SENDERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_NO_SENDERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_NO_RECEIVERS:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_REQUEST_CONFIRMED:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_ADMISSION_FAILURE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_POLICY_FAILURE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_BAD_STYLE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_BAD_OBJECT:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_TRAFFIC_CTRL_ERROR:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_GENERIC_ERROR:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_ESERVICETYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFLOWSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EPROVSPECBUF:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFILTERSTYLE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFILTERTYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFILTERCOUNT:    return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EOBJLENGTH:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFLOWCOUNT:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EUNKOWNPSOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EPOLICYOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EFLOWDESC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EPSFLOWSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_EPSFILTERSPEC:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_ESDMODEOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_ESHAPERATEOBJ:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx
        case WSA_QOS_RESERVED_PETYPE:   return xxxxxxxxxxxx; // xxxxxxxxxxxxxxxxx

        default:
            return PXActionInvalid;
    }
}
#endif
#endif
