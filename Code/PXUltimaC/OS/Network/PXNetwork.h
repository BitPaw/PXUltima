#ifndef PXNetworkINCLUDE
#define PXNetworkINCLUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

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
	PXSocketTypeStream,// stream socket */
	PXSocketTypeDatagram, // datagram socket */
	PXSocketTypeRaw, // raw-protocol interface */
	PXSocketTypeRDM, // reliably-delivered message */
	PXSocketTypeSeqPacket // sequenced packet stream */
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
typedef	PXSocketID(PXSTDCALL* PXSocketCreate)(int af, int type, int protocol); // socket
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
typedef PXInt32U(PXSTDCALL* PXhtonl)(PXInt32U hostlong);
typedef PXInt16U(PXSTDCALL* PXhtons)(PXInt16U hostshort);

#if OSUnix
typedef int (PXSTDCALL* PXpoll)(struct pollfd* fds, nfds_t nfds, int timeout);
#elif OSWindows
typedef int (WINAPI* PXWSAPoll)(void* fdArray, ULONG fds, INT timeout);
typedef int	(WINAPI* PXWSAGetLastError)(void);
typedef int	(WINAPI* PXWSAStartup)(WORD wVersionRequested, void* lpWSAData);
typedef int	(WINAPI* PXWSACleanup)(void);
#endif

typedef struct PXSocket_
{
	PXSocketID ID;
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

typedef struct PXSocketConnectInfo_
{
	PXSocket* SocketReference;

	char* IP; // if NULL, use 127.0.0.1
	PXInt16U Port;

	IPAdressFamily AdressFamily;
	PXSocketType Type;
	PXProtocolMode ProtocolMode;
}
PXSocketConnectInfo;

typedef struct PXSocketListenInfo_
{
	PXSocket* SocketReference;
	PXInt32U ClientInQueueMaximal;
}
PXSocketListenInfo;

typedef struct PXSocketBindInfo_
{
	PXSocket* SocketReference;
	PXInt32U Port;
	char* IP;
}
PXSocketBindInfo;


typedef struct PXSocketDataInfo_
{
	void* Buffer; // Data source/target
	PXSize BufferSize; // How much can we transphere	
	PXSize BufferOffset;

	PXSize SegmentSize; // Size of each action of send/read
	PXSize SegmentDelay;

	PXSize MovedCurrent;
	PXSize MovedTotal;

	PXInt8U Percentage;
	PXBool SocketDestroyed;
}
PXSocketDataInfo;

typedef struct PXSocketReadInfo_
{
	PXSocketDataInfo DataInfo;

	PXSocket* SocketReciverReference;
	PXSocket* SocketSenderReference;
}
PXSocketReadInfo;

typedef struct PXSocketSendInfo_
{
	PXSocketDataInfo DataInfo;

	PXSocket* SocketSenderReference;
	PXSocket* SocketRecieverReference;
}
PXSocketSendInfo;



typedef struct PXSocketWriteInfo_
{
	PXSocket* SocketReference;
	PXInt32U Port;
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


PXPrivate PXProtocolMode PXAPI PXProtocolModeFromID(const PXInt8U protocolMode);
PXPrivate PXInt8U PXAPI PXProtocolModeToID(const PXProtocolMode protocolMode);

PXPrivate PXSocketType PXAPI PXSocketTypeFromID(const PXInt8U socketType);
PXPrivate PXInt8U PXAPI PXSocketTypeToID(const PXSocketType socketType);

PXPrivate IPAdressFamily PXAPI PXIPAdressFamilyFromID(const PXInt8U ipMode);
PXPrivate PXInt8U PXAPI PXIPAdressFamilyToID(const IPAdressFamily ipMode);


PXPublic PXActionResult PXAPI PXNetworkInitialize(PXNetwork* const pxNetwork);
PXPublic PXActionResult PXAPI PXNetworkRelease(PXNetwork* const pxNetwork);

PXPublic PXActionResult PXAPI PXNetworkSocketCreate(PXNetwork* const pxNetwork, PXSocketCreateInfo* const pxSocketCreateInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketDestroy(PXNetwork* const pxNetwork, PXSocketDestroyInfo* const pxSocketDestroyInfo);

PXPublic PXActionResult PXAPI PXNetworkSocketConnect(PXNetwork* const pxNetwork, PXSocketConnectInfo* const pxSocketConnectInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketListen(PXNetwork* const pxNetwork, PXSocketListenInfo* const pxSocketListenInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketAccept(PXNetwork* const pxNetwork, PXSocketAcceptInfo* const pxSocketAcceptInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketBind(PXNetwork* const pxNetwork, PXSocketBindInfo* const pxSocketBindInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketReceive(PXNetwork* const pxNetwork, PXSocketReadInfo* const pxSocketReadInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketSend(PXNetwork* const pxNetwork, PXSocketSendInfo* const pxSocketSendInfo);
PXPublic PXActionResult PXAPI PXNetworkSocketPoll(PXNetwork* const pxNetwork);



#if OSWindows
PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFetch(PXNetwork* const pxNetwork);
PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFromID(const PXInt32S errorID);
#endif

#endif