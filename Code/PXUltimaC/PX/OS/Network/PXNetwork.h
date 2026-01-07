#ifndef PXNetworkIncluded
#define PXNetworkIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

#if OSUnix
#include <sys/poll.h>
#elif OSWindows
#endif

typedef enum IPAdressFamily_
{
    IPAdressFamilyInvalid,
    IPAdressFamilyUnspecified, // unspecified
    IPAdressFamilyUNIX, // local to host (pipes, portals)
    IPAdressFamilyINET, // internetwork: UDP, TCP, etc.
    IPAdressFamilyIMPLINK, // arpanet imp addresses
    IPAdressFamilyPUP, // pup protocols: e.g. BSP
    IPAdressFamilyCHAOS, // mit CHAOS protocols
    IPAdressFamilyNS, // XEROX NS protocols
    IPAdressFamilyIPX, // IPX protocols: IPX, SPX, etc.
    IPAdressFamilyISO, // ISO protocols
    IPAdressFamilyOSI, // OSI is ISO
    IPAdressFamilyECMA, // european computer manufacturers
    IPAdressFamilyDATAKIT, // datakit protocols
    IPAdressFamilyCCITT, // CCITT protocols, X.25 etc
    IPAdressFamilySNA, // IBM SNA
    IPAdressFamilyDECnet, // DECnet
    IPAdressFamilyDLI, // Direct data link interface
    IPAdressFamilyLAT, // LAT
    IPAdressFamilyHYLINK, // NSC Hyperchannel
    IPAdressFamilyAPPLETALK, // AppleTalk
    IPAdressFamilyNETBIOS, // NetBios-style addresses
    IPAdressFamilyVOICEVIEW, // VoiceView
    IPAdressFamilyFIREFOX, // Protocols from Firefox
    IPAdressFamilyUNKNOWN1, // Somebody is using this!
    IPAdressFamilyBAN, // Banyan
    IPAdressFamilyATM, // Native ATM Services
    IPAdressFamilyINET6, // Internetwork Version 6
    IPAdressFamilyCLUSTER, // Microsoft Wolfpack
    IPAdressFamilyIEEE12844, // IEEE 1284.4 WG AF
    IPAdressFamilyIRDA, // IrDA
    IPAdressFamilyNETDES, // Network Designers OSI & gateway
    IPAdressFamilyMAX, // depends
    IPAdressFamilyLINK,
    IPAdressFamilyHYPERV,
    IPAdressFamilyBTH, // Bluetooth RFCOMM/L2CAP protocols
    IPAdressFamilyTCNPROCESS,
    IPAdressFamilyTCNMESSAGE,
    IPAdressFamilyICLFXBM
}
IPAdressFamily;

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

typedef PXSize PXSocketID;

typedef PXSocketID(PXSTDCALL* PXaccept)(PXSocketID s, struct sockaddr* addr, int* addrlen); // accept
typedef    PXSocketID(PXSTDCALL* PXSocketCreate)(int af, int type, int protocol); // socket
typedef int (PXSTDCALL* PXioctlsocket)(PXSocketID s, long cmd, int* argp); // ioctlsocket
typedef int (PXSTDCALL* PXclosesocket)(PXSocketID s); // closesocket
typedef int (PXSTDCALL* PXshutdown)(PXSocketID s, int how); // shutdown
typedef int (PXSTDCALL* PXlisten)(PXSocketID s, int backlog);
typedef int (PXSTDCALL* PXbind)(PXSocketID s, const struct sockaddr* name, int namelen);
typedef int (PXSTDCALL* PXconnect)(PXSocketID s, const struct sockaddr* name, int namelen);
typedef int (PXSTDCALL* PXrecv)(PXSocketID s, char* buf, int len, int flags);
typedef int (PXSTDCALL* PXsend)(PXSocketID s, const char* buf, int len, int flags);
typedef int (PXSTDCALL* PXsetsockopt)(PXSocketID s, int level, int optname, const char* optval, int optlen);
typedef int (PXSTDCALL* PXgetaddrinfo)(char* pNodeName, char* pServiceName, const void* pHints, void* ppResult);
typedef struct hostent* (PXSTDCALL* PXgethostbyname)(const char* name);
typedef int (PXSTDCALL* PXselect)(int nfds, void* readfds, void* writefds, void* exceptfds, const struct timeval* timeout);
typedef PXI32U(PXSTDCALL* PXhtonl)(PXI32U hostlong);
typedef PXI16U(PXSTDCALL* PXhtons)(PXI16U hostshort);

#if OSUnix
typedef int (PXSTDCALL* PXpoll)(struct pollfd* fds, nfds_t nfds, int timeout);
#elif OSWindows
typedef int (WINAPI* PXWSAPoll)(void* fdArray, ULONG fds, INT timeout);
typedef int    (WINAPI* PXWSAGetLastError)(void);
typedef int    (WINAPI* PXWSAStartup)(WORD wVersionRequested, void* lpWSAData);
typedef int    (WINAPI* PXWSACleanup)(void);
#endif

typedef struct PXSocket_
{
    PXSocketID ID;

    char IP[44];
    char Name[32]; // NI_MAXHOST
    PXI16U Port;
}
PXSocket;

typedef struct PXSocketCreateInfo_
{
    PXSocket* SocketReference;
    IPAdressFamily AdressFamily;
    PXSocketType Type;
    PXProtocolMode ProtocolMode;
}
PXSocketCreateInfo;

typedef struct PXSocketDestroyInfo_
{
    PXSocket* SocketReference;
}
PXSocketDestroyInfo;


#define  PXIPAdressModeNone (1<<0)
#define  PXIPAdressModeIPv4 (1<<1)
#define  PXIPAdressModeIPv6 (1<<2)
#define  PXIPAdressModeText (1<<3)




// MAC - Media-Access-Control-Adress
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


typedef struct PXIPAdress_
{
    // Union, to combine IP versions
    union
    {
        PXIPv4 IPv4;
        PXIPv6 IPv6;    
    };

    char* Text; // if NULL, use 127.0.0.1
    PXSize TextSize;
    PXSize TextUsed;
}
PXIPAdress;

typedef struct PXSocketConnectionInfo_
{
    PXSocketID ID;
    PXIPAdress IP;
    PXI16U Port;

    IPAdressFamily AdressFamily;
    PXSocketType Type;
    PXProtocolMode ProtocolMode;
}
PXSocketConnectionInfo;


typedef struct PXSocketConnectInfo_
{
    PXSocket* SocketReference;

    PXIPAdress IP;
    PXI16U Port;

    IPAdressFamily AdressFamily;
    PXSocketType Type;
    PXProtocolMode ProtocolMode;
}
PXSocketConnectInfo;

typedef struct PXSocketListenInfo_
{
    PXSocket* SocketReference;
    PXI32U ClientInQueueMaximal;
}
PXSocketListenInfo;

typedef struct PXSocketBindInfo_
{
    PXSocket* SocketReference;
    PXI32U Port;
    char* IP;
}
PXSocketBindInfo;


typedef struct PXSocketDataInfo_
{
    PXSocketID SocketIDCurrent;

    void* Buffer; // Data source/target
    PXSize BufferSize; // How much can we transphere
    PXSize BufferUsed;

    PXSocket* SocketPeer;


    PXSize SegmentSize; // Size of each action of send/read
    PXSize SegmentDelay;

    PXSize MovedCurrent;
    PXSize MovedTotal;

    PXI8U Percentage;
    PXBool SocketDestroyed;
}
PXSocketDataInfo;

typedef struct PXSocketWriteInfo_
{
    PXSocket* SocketReference;
    PXI32U Port;
}
PXSocketWriteInfo;

typedef struct PXSocketAcceptInfo_
{
    PXSocket* SocketServerReference;
    PXSocket* SocketClientReference;
}
PXSocketAcceptInfo;


typedef struct PXNetwork_
{
    PXLibrary NetworkLibrary;

    PXaccept SocketAccept;
    PXbind SocketBind;
    PXclosesocket SocketClose;
    PXconnect SocketConnect;
    void* getpeername;
    void* getsockname;
    void* getsockopt;
    PXhtonl Converthtonl;
    PXhtons Converthtons;
    PXioctlsocket SocketControlIO;
    void* inet_addr;
    void* inet_ntoa;
    PXlisten SocketListen;
    PXrecv SocketRecive;
    void* recvfrom;
    PXselect SocketSelect;
    PXsend SocketSend;
    void* sendto;
    PXsetsockopt SocketOptionSet;
    PXshutdown SocketShutdown;
    PXSocketCreate SocketCreate;

    PXgetaddrinfo AdressInfoGet;
    PXgethostbyname HostByNameGet;

#if OSUnix
#elif OSWindows
    PXWSAGetLastError SocketSystemErrorLastGet;
    PXWSAStartup SocketSystemStartup;
    PXWSACleanup SocketSystemCleanup;
    PXWSAPoll SocketPoll;
#endif
}
PXNetwork;


PXPrivate PXProtocolMode PXAPI PXProtocolModeFromID(const PXI8U protocolMode);
PXPrivate PXI8U PXAPI PXProtocolModeToID(const PXProtocolMode protocolMode);

PXPrivate PXSocketType PXAPI PXSocketTypeFromID(const PXI8U socketType);
PXPrivate PXI8U PXAPI PXSocketTypeToID(const PXSocketType socketType);

PXPrivate IPAdressFamily PXAPI PXIPAdressFamilyFromID(const PXI8U ipMode);
PXPrivate PXI8U PXAPI PXIPAdressFamilyToID(const IPAdressFamily ipMode);


PXPublic PXResult PXAPI PXNetworkModulState(PXNetwork* PXREF pxNetworkREF, const PXI32U flags);

PXPublic PXResult PXAPI PXNetworkSocketCreate(PXSocketCreateInfo PXREF pxSocketCreateInfo);
PXPublic PXResult PXAPI PXNetworkSocketDestroy(const PXSocketID pxSocketID);

PXPublic PXResult PXAPI PXNetworkSocketConnect(PXSocketConnectInfo PXREF pxSocketConnectInfo);
PXPublic PXResult PXAPI PXNetworkSocketListen(PXSocketListenInfo PXREF pxSocketListenInfo);
PXPublic PXResult PXAPI PXNetworkSocketAccept(PXSocketAcceptInfo PXREF pxSocketAcceptInfo);
PXPublic PXResult PXAPI PXNetworkSocketBind(PXSocketBindInfo PXREF pxSocketBindInfo);
PXPublic PXResult PXAPI PXNetworkSocketReceive(PXSocketDataInfo PXREF pxSocketDataInfo);
PXPublic PXResult PXAPI PXNetworkSocketSend(PXSocketDataInfo PXREF pxSocketDataInfo);
PXPublic PXResult PXAPI PXNetworkSocketPoll();


PXPublic PXResult PXAPI PXNetworkPeerName(const PXSocket PXREF pxSocket);

PXPublic PXResult PXAPI PXNetworkSocketMTU(const PXSocket PXREF pxSocket, PXI32U* value, const PXBool doWrite);
PXPublic PXResult PXAPI PXNetworkSocketName(const PXSocket PXREF pxSocket, char* name);
PXPublic PXResult PXAPI PXNetworkSocketInterfaceName(const PXSocket PXREF pxSocket, char* name, const PXSize nameSize);


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

    PXMAC MACAdress;

    PXI16U MTU; // Transmiited block size

    PXSize SpeedTransmit;
    PXSize SpeedRecieve;
}
PXNetworkAdapter;

PXPublic PXResult PXAPI PXNetworkAdapterFetch();



// IP adresses are commonly globally assigned by country and city.
// With this, we can broadly guess where a connection is comming from.
// Because of reassignments and sharing of ranges, this info can
// never be exact and should only be used as a hint and not fact.
PXPublic PXResult PXAPI PXNetworkIPLocate(const PXIPAdress PXREF pxIPAdress);

// After connecting or accepting a socket, use this function to
// gather information about the connected peer.
PXPublic PXResult PXAPI PXNetworkSocketPeerGet(PXNetwork PXREF pxNetwork, PXSocketConnectionInfo PXREF pxSocketConnectionInfo);


#if OSWindows
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFetch(PXNetwork PXREF pxNetwork);
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFromID(const PXI32S errorID);
#endif

#endif
