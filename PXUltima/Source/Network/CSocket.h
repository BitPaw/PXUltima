#ifndef CSocketInclude
#define CSocketInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <OS/OSVersion.h>

#if defined(OSUnix)
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define AdressInfoType struct addrinfo
#define AdressInfoDelete freeaddrinfo
#elif defined(OSWindows)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#define AdressInfoType ADDRINFOA
#define AdressInfoDelete FreeAddrInfoA
#endif

#include <Async/Thread.h>

#define CSocketID size_t
#define SocketDebug 1
#define SocketIDOffline -1
#define IPv6LengthMax 65

#ifdef __cplusplus
extern "C"
{
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

	typedef enum CSocketType_
	{
		CSocketTypeInvalid,
		CSocketTypeStream,// stream socket */
		CSocketTypeDatagram, // datagram socket */
		CSocketTypeRaw, // raw-protocol interface */
		CSocketTypeRDM, // reliably-delivered message */
		CSocketTypeSeqPacket // sequenced packet stream */
	}
	CSocketType;

	typedef enum ProtocolMode_
	{
		ProtocolModeInvalid,
		ProtocolModeHOPOPTS,  // IPv6 Hop-by-Hop options
		ProtocolModeICMP,
		ProtocolModeIGMP,
		ProtocolModeGGP,
		ProtocolModeIPV4,
		ProtocolModeST,
		ProtocolModeTCP,
		ProtocolModeCBT,
		ProtocolModeEGP,
		ProtocolModeIGP,
		ProtocolModePUP,
		ProtocolModeUDP,
		ProtocolModeIDP,
		ProtocolModeRDP,
		ProtocolModeIPV6, // IPv6 header
		ProtocolModeROUTING, // IPv6 Routing header
		ProtocolModeFRAGMENT, // IPv6 fragmentation header
		ProtocolModeESP, // encapsulating security payload
		ProtocolModeAH, // authentication header
		ProtocolModeICMPV6, // ICMPv6
		ProtocolModeNONE, // IPv6 no next header
		ProtocolModeDSTOPTS, // IPv6 Destination options
		ProtocolModeND,
		ProtocolModeICLFXBM,
		ProtocolModePIM,
		ProtocolModePGM,
		ProtocolModeL2TP,
		ProtocolModeSCTP,
		ProtocolModeRAW,
		ProtocolModeMAX,
		//
		//  These are reserved for CPrivate use by Windows.
		//
		ProtocolModeWindowsRAW,
		ProtocolModeWindowsIPSEC,
		ProtocolModeWindowsIPSECOFFLOAD,
		ProtocolModeWindowsWNV,
		ProtocolModeWindowsMAX
	}
	ProtocolMode;

	typedef enum CSocketState_
	{
		SocketNotInitialised,
		// Client only
		SocketConnecting,
		SocketConnected,
		SocketDataReceiving,
		SocketDataSending,
		SocketFileReceiving,
		SocketFileSending,
		// Server only
		SocketWaitingForConnection, // listenung
		SocketIDLE
	}
	CSocketState;

	typedef struct CSocket_ CSocket;

	typedef void (*SocketCreatingEvent)(const CSocket* cSocket, unsigned char* use);
	typedef void (*SocketCreatedEvent)(const CSocket* cSocket);

	typedef void (*MessageSendEvent)(const CSocket* cSocket, const void* message, const size_t messageSize);
	typedef void (*MessageReceiveEvent)(const CSocket* cSocket, const void* message, const size_t messageSize);

	// Server Only
	typedef void (*ConnectionListeningEvent)(const CSocket* cSocket);
	typedef void (*ConnectionLinkedEvent)(const CSocket* cSocket);

	// Client Only
	typedef void (*ConnectionEstablishedEvent)(const CSocket* cSocket);
	typedef void (*ConnectionTerminatedEvent)(const CSocket* cSocket);


	typedef struct CSocket_
	{
		// Connection Info
		CSocketID ID;
		ProtocolMode Protocol;
		IPAdressFamily Family;
		CSocketType Type;
		char IP[IPv6LengthMax];
		size_t IPSize;
		unsigned short Port;

		//----------------------------

		CSocketState State;

		//---<CPrivate IO>------------
		ThreadID CommunicationThread;
		//----------------------------

		//---<Event CallBack>---------
		SocketCreatingEvent SocketCreatingCallback;
		SocketCreatedEvent SocketCreatedCallback;

		MessageSendEvent MessageSendCallback;
		MessageReceiveEvent MessageReceiveCallback;

		// Server Only
		ConnectionListeningEvent ConnectionListeningCallback;
		ConnectionLinkedEvent ConnectionLinkedCallback;

		// Client Only
		ConnectionEstablishedEvent ConnectionEstablishedCallback;
		ConnectionTerminatedEvent ConnectionTerminatedCallback;
		//----------------------------
	}
	CSocket;


	static ProtocolMode ConvertToProtocolMode(const unsigned int protocolMode);
	static unsigned int ConvertFromProtocolMode(const ProtocolMode protocolMode);

	static CSocketType ConvertToSocketType(const unsigned int socketType);
	static unsigned int ConvertFromSocketType(const CSocketType socketType);

	static IPAdressFamily ConvertToIPAdressFamily(const unsigned int ipMode);
	static unsigned int ConvertFromIPAdressFamily(const IPAdressFamily ipMode);


	CPublic void CSocketConstruct(CSocket* const cSocket);
	CPublic void CSocketDestruct(CSocket* const cSocket);

	CPublic ActionResult CSocketCreate
	(
		CSocket* cSocket,
		const IPAdressFamily adressFamily,
		const CSocketType socketType,
		const ProtocolMode protocolMode
	);

	CPublic ActionResult CSocketConnect(CSocket* cSocket);

	CPublic ActionResult CSocketSetupAdress
	(
		CSocket* cSocketList,
		const size_t cSocketListSizeMax,
		size_t* cSocketListSize,
		const char* const ip, // null for any ipAdress
		unsigned short port, // -1 for no port
		IPAdressFamily ipMode,
		CSocketType socketType,
		ProtocolMode protocolMode
	);

	CPublic unsigned char CSocketIsCurrentlyUsed(CSocket* cSocket);
	CPublic void CSocketClose(CSocket* cSocket);

	CPublic ActionResult CSocketBind(CSocket* cSocket);
	CPublic ActionResult CSocketOptionsSet(CSocket* cSocket);
	CPublic ActionResult CSocketListen(CSocket* cSocket);
	CPublic ActionResult CSocketAccept(CSocket* server, CSocket* client);

	CPublic ActionResult CSocketSend(CSocket* cSocket, const void* inputBuffer, const size_t inputBufferSize, size_t* inputBytesWritten);
	CPublic ActionResult CSocketReceive(CSocket* cSocket, const void* outputBuffer, const size_t outputBufferSize, size_t* outputBytesWritten);

#if defined(OSWindows)
	static ActionResult WindowsSocketAgentStartup();
	static ActionResult WindowsSocketAgentShutdown();
#endif

#ifdef __cplusplus
}
#endif

#endif
