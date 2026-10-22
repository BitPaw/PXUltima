#ifndef PXNetworkIncluded
#define PXNetworkIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

#if OSUnix
#include <sys/poll.h>
#elif OSWindows
#endif

typedef enum IPAddressFamily_
{
    IPAddressFamilyInvalid,
    IPAddressFamilyUnspecified, // unspecified
    IPAddressFamilyUNIX, // local to host (pipes, portals)
    IPAddressFamilyINET, // internetwork: UDP, TCP, etc.
    IPAddressFamilyIMPLINK, // arpanet imp addresses
    IPAddressFamilyPUP, // pup protocols: e.g. BSP
    IPAddressFamilyCHAOS, // mit CHAOS protocols
    IPAddressFamilyNS, // XEROX NS protocols
    IPAddressFamilyIPX, // IPX protocols: IPX, SPX, etc.
    IPAddressFamilyISO, // ISO protocols
    IPAddressFamilyOSI, // OSI is ISO
    IPAddressFamilyECMA, // european computer manufacturers
    IPAddressFamilyDATAKIT, // datakit protocols
    IPAddressFamilyCCITT, // CCITT protocols, X.25 etc
    IPAddressFamilySNA, // IBM SNA
    IPAddressFamilyDECnet, // DECnet
    IPAddressFamilyDLI, // Direct data link interface
    IPAddressFamilyLAT, // LAT
    IPAddressFamilyHYLINK, // NSC Hyperchannel
    IPAddressFamilyAPPLETALK, // AppleTalk
    IPAddressFamilyNETBIOS, // NetBios-style addresses
    IPAddressFamilyVOICEVIEW, // VoiceView
    IPAddressFamilyFIREFOX, // Protocols from Firefox
    IPAddressFamilyUNKNOWN1, // Somebody is using this!
    IPAddressFamilyBAN, // Banyan
    IPAddressFamilyATM, // Native ATM Services
    IPAddressFamilyINET6, // Internetwork Version 6
    IPAddressFamilyCLUSTER, // Microsoft Wolfpack
    IPAddressFamilyIEEE12844, // IEEE 1284.4 WG AF
    IPAddressFamilyIRDA, // IrDA
    IPAddressFamilyNETDES, // Network Designers OSI & gateway
    IPAddressFamilyMAX, // depends
    IPAddressFamilyLINK,
    IPAddressFamilyHYPERV,
    IPAddressFamilyBTH, // Bluetooth RFCOMM/L2CAP protocols
    IPAddressFamilyTCNPROCESS,
    IPAddressFamilyTCNMESSAGE,
    IPAddressFamilyICLFXBM
}
IPAddressFamily;

typedef enum PXSocketType_
{
    PXSocketTypeInvalid,
    PXSocketTypeStream,// stream socket
    PXSocketTypeDatagram, // datagram socket
    PXSocketTypeRaw, // raw-protocol interface
    PXSocketTypeRDM, // reliably-delivered message
    PXSocketTypeSeqPacket // sequenced packet stream
}
PXSocketType;

typedef enum PXProtocolMode_
{
    PXProtocolModeInvalid,
    PXProtocolModeHOPOPTS,  // IPv6 Hop-by-Hop options
    PXProtocolModeICMP,
    PXProtocolModeIGMP,
    PXProtocolModeGGP,
    PXProtocolModeIPV4,
    PXProtocolModeST,
    PXProtocolModeTCP,
    PXProtocolModeCBT,
    PXProtocolModeEGP,
    PXProtocolModeIGP,
    PXProtocolModePUP,
    PXProtocolModeUDP,
    PXProtocolModeIDP,
    PXProtocolModeRDP,
    PXProtocolModeIPV6, // IPv6 header
    PXProtocolModeROUTING, // IPv6 Routing header
    PXProtocolModeFRAGMENT, // IPv6 fragmentation header
    PXProtocolModeESP, // encapsulating security payload
    PXProtocolModeAH, // authentication header
    PXProtocolModeICMPV6, // ICMPv6
    PXProtocolModeNONE, // IPv6 no next header
    PXProtocolModeDSTOPTS, // IPv6 Destination options
    PXProtocolModeND,
    PXProtocolModeICLFXBM,
    PXProtocolModePIM,
    PXProtocolModePGM,
    PXProtocolModeL2TP,
    PXProtocolModeSCTP,
    PXProtocolModeRAW,
    PXProtocolModeMAX,
    //
    //  These are reserved for private use by Windows.
    //
    PXProtocolModeWindowsRAW,
    PXProtocolModeWindowsIPSEC,
    PXProtocolModeWindowsIPSECOFFLOAD,
    PXProtocolModeWindowsWNV,
    PXProtocolModeWindowsMAX
}
PXProtocolMode;

typedef enum PXSocketState_
{
    SocketNotInitialised,

    SocketInitialised,

    SocketIDLE, // Waiting for action, currently doing nothing
    SocketEventPolling, // Polling for events on multible sockets
    SocketOffline,

    SocketFailed,

    // PXClient only
    SocketConnecting,
    SocketConnected,
    SocketDataReceiving,
    SocketDataSending,
    SocketFileReceiving,
    SocketFileSending,

    // PXServer only
}
PXSocketState;

#if OSUnix
typedef PXSize PXSocketID;
#elif OSWindows
typedef UINT_PTR PXSocketID;
#endif

typedef PXSocketID(PXSTDCALL* PXacceptFunction)(PXSocketID s, struct sockaddr* addr, int* addrlen); // accept
typedef    PXSocketID(PXSTDCALL* PXSocketCreateFunction)(int af, int type, int protocol); // socket
typedef int (PXSTDCALL* PXioctlsocketFunction)(PXSocketID s, long cmd, int* argp); // ioctlsocket
typedef int (PXSTDCALL* PXclosesocketFunction)(PXSocketID s); // closesocket
typedef int (PXSTDCALL* PXshutdownFunction)(PXSocketID s, int how); // shutdown
typedef int (PXSTDCALL* PXlistenFunction)(PXSocketID s, int backlog);
typedef int (PXSTDCALL* PXbindFunction)(PXSocketID s, const struct sockaddr* name, int namelen);
typedef int (PXSTDCALL* PXconnectFunction)(PXSocketID s, const struct sockaddr* name, int namelen);
typedef int (PXSTDCALL* PXrecvFunction)(PXSocketID s, char* buf, int len, int flags);
typedef int (PXSTDCALL* PXsendFunction)(PXSocketID s, const char* buf, int len, int flags);
typedef int (PXSTDCALL* PXsetsockoptFunction)(PXSocketID s, int level, int optname, const char* optval, int optlen);
typedef int (PXSTDCALL* PXgetaddrinfoFunction)(char* pNodeName, char* pServiceName, const void* pHints, void* ppResult);
typedef struct hostent* (PXSTDCALL* PXgethostbynameFunction)(const char* name);
typedef int (PXSTDCALL* PXselectFunction)(int nfds, void* readfds, void* writefds, void* exceptfds, const struct timeval* timeout);
typedef PXI32U(PXSTDCALL* PXhtonlFunction)(PXI32U hostlong);
typedef PXI16U(PXSTDCALL* PXhtonsFunction)(PXI16U hostshort);

#if OSUnix
typedef int (PXSTDCALL* PXpollFunction)(struct pollfd* fds, nfds_t nfds, int timeout);
#elif OSWindows
typedef int (WINAPI* PXWSAPollFunction)(void* fdArray, ULONG fds, INT timeout);
typedef int    (WINAPI* PXWSAGetLastErrorFunction)(void);
typedef int    (WINAPI* PXWSAStartupFunction)(WORD wVersionRequested, void* lpWSAData);
typedef int    (WINAPI* PXWSACleanupFunction)(void);
#endif


#define  PXIPAddressModeNone (1<<0)
#define  PXIPAddressModeIPv4 (1<<1)
#define  PXIPAddressModeIPv6 (1<<2)
#define  PXIPAddressModeText (1<<3)


// MAC - Media-Access-Control-Address
typedef struct PXMAC_
{
    PXI8U Data[6]; // Somewhere i saw it can be up to 32 Bytes?
}
PXMAC;

// IP - Version 4
typedef struct PXIPv4_
{
    PXI8U Data[4];   // Use to directy update each byte
    PXI32U ID;    // To use it directly as an int

    // IPAddr
}
PXIPv4;

// IP - Version 6
typedef struct PXIPv6_
{
    PXI8U Data[16]; // 128-Bit
    // Cant do IDs
}
PXIPv6;


typedef struct PXIPAddress_
{
    // Union, to combine IP versions
    union
    {
        PXIPv4 IPv4;
        PXIPv6 IPv6;    
    };

    PXText Text;
}
PXIPAddress;

typedef struct PXNetwork_
{
    PXLibrary NetworkLibrary;

    PXacceptFunction SocketAccept;
    PXbindFunction SocketBind;
    PXclosesocketFunction SocketClose;
    PXconnectFunction SocketConnect;
    void* getpeername;
    void* getsockname;
    void* getsockopt;
    PXhtonlFunction Converthtonl;
    PXhtonsFunction Converthtons;
    PXioctlsocketFunction SocketControlIO;
    void* inet_addr;
    void* inet_ntoa;
    PXlistenFunction SocketListen;
    PXrecvFunction SocketRecive;
    void* recvfrom;
    PXselectFunction SocketSelect;
    PXsendFunction SocketSend;
    void* sendto;
    PXsetsockoptFunction SocketOptionSet;
    PXshutdownFunction SocketShutdown;
    PXSocketCreateFunction SocketCreate;

    PXgetaddrinfoFunction AddressInfoGet;
    PXgethostbynameFunction HostByNameGet;

#if OSUnix
#elif OSWindows
    PXWSAGetLastErrorFunction SocketSystemErrorLastGet;
    PXWSAStartupFunction SocketSystemStartup;
    PXWSACleanupFunction SocketSystemCleanup;
    PXWSAPollFunction SocketPoll;
#endif
}
PXNetwork;


PXInternal PXProtocolMode PXAPI PXProtocolModeFromID(const PXI8U protocolMode);
PXInternal PXI8U PXAPI PXProtocolModeToID(const PXProtocolMode protocolMode);

PXInternal PXSocketType PXAPI PXSocketTypeFromID(const PXI8U socketType);
PXInternal PXI8U PXAPI PXSocketTypeToID(const PXSocketType socketType);

PXInternal IPAddressFamily PXAPI PXIPAddressFamilyFromID(const PXI8U ipMode);
PXInternal PXI8U PXAPI PXIPAddressFamilyToID(const IPAddressFamily ipMode);


PXPublic PXResult PXAPI PXNetworkModulState(PXNetwork* PXREF pxNetworkREF, const PXI32U flags);



PXPublic PXResult PXAPI PXNetworkMACFromIPv4A(PXMAC PXREF pxMAC, char PXREF ipv4Text);
PXPublic PXResult PXAPI PXNetworkMACFromIPv4(PXMAC PXREF pxMAC, PXIPv4 PXREF pxIPv4); // ARP, SendARP

PXPublic PXResult PXAPI PXNetworkMACFromIPv6A(PXMAC PXREF pxMAC, char PXREF ipv6Text);
PXPublic PXResult PXAPI PXNetworkMACFromIPv6(PXMAC PXREF pxMAC, PXIPv6 PXREF pxIPv6); // NDP, 




PXPublic PXResult PXAPI PXNetworkNameFromIPv4A(char PXREF name, char PXREF ipv4Text);
PXPublic PXResult PXAPI PXNetworkNameFromIPv4(char PXREF name, PXIPv4 PXREF pxIPv4);



typedef struct PXNetworkAdapter_
{
    char Description[64];
    char Name[32];

    PXMAC MACAddress;

    PXI16U MTU; // Transmiited block size

    PXSize SpeedTransmit;
    PXSize SpeedRecieve;
}
PXNetworkAdapter;


PXPublic PXResult PXAPI PXNetworkAdapterFetch();



// IP addresses are commonly globally assigned by country and city.
// With this, we can broadly guess where a connection is comming from.
// Because of reassignments and sharing of ranges, this info can
// never be exact and should only be used as a hint and not fact.
PXPublic PXResult PXAPI PXNetworkIPLocate(const PXIPAddress PXREF pxIPAddress);

#if OSWindows
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFetch(PXNetwork PXREF pxNetwork);
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFromID(const PXI32S errorID);
#endif

#endif
