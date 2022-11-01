#ifndef PXSocketInclude
#define PXSocketInclude

#include <OS/OSVersion.h>

#if OSUnix
#ifndef _XOPEN_SOURCE
    // Needed for some reason, as without the "netdb.h" does not get included properly
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
#endif // !_WINSOCK_DEPRECATED_NO_WARNINGS

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

#include <Async/PXThread.h>
#include <Format/Type.h>
#include <Error/ActionResult.h>

#define PXSocketID size_t
#define SocketDebug 0
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

	typedef enum PXSocketState_
	{
		SocketNotInitialised,
		// PXClient only
		SocketConnecting,
		SocketConnected,
		SocketDataReceiving,
		SocketDataSending,
		SocketFileReceiving,
		SocketFileSending,
		// PXServer only
		SocketWaitingForConnection, // listenung
		SocketIDLE
	}
	PXSocketState;

	typedef struct PXSocket_ PXSocket;

	typedef void (*SocketCreatingEvent)(const PXSocket* const pxSocket, unsigned char* use);
	typedef void (*SocketCreatedEvent)(const PXSocket* const pxSocket);

	typedef void (*MessageSendEvent)(const PXSocket* const pxSocket, const void* message, const size_t messageSize);
	typedef void (*MessageReceiveEvent)(const PXSocket* const pxSocket, const void* message, const size_t messageSize);

	// PXServer Only
	typedef void (*ConnectionListeningEvent)(const PXSocket* const pxSocket);
	typedef void (*ConnectionLinkedEvent)(const PXSocket* const pxSocket);

	// PXClient Only
	typedef void (*ConnectionEstablishedEvent)(const PXSocket* const pxSocket);
	typedef void (*ConnectionTerminatedEvent)(const PXSocket* const pxSocket);


	typedef struct PXSocket_
	{
		// Connection Info
		PXSocketID ID;
		ProtocolMode Protocol;
		IPAdressFamily Family;
		PXSocketType Type;
		char IP[IPv6LengthMax];
		size_t IPSize;
		unsigned short Port;

		//----------------------------

		PXSocketState State;

		//---<CPrivate IO>------------
		PXThread CommunicationThread;
		//----------------------------

		//---<Event CallBack>---------
		SocketCreatingEvent SocketCreatingCallback;
		SocketCreatedEvent SocketCreatedCallback;

		MessageSendEvent MessageSendCallback;
		MessageReceiveEvent MessageReceiveCallback;

		// PXServer Only
		ConnectionListeningEvent ConnectionListeningCallback;
		ConnectionLinkedEvent ConnectionLinkedCallback;

		// PXClient Only
		ConnectionEstablishedEvent ConnectionEstablishedCallback;
		ConnectionTerminatedEvent ConnectionTerminatedCallback;
		//----------------------------
	}
	PXSocket;


	PXPrivate ProtocolMode ConvertToProtocolMode(const unsigned int protocolMode);
	PXPrivate unsigned int ConvertFromProtocolMode(const ProtocolMode protocolMode);

	PXPrivate PXSocketType ConvertToSocketType(const unsigned int socketType);
	PXPrivate unsigned int ConvertFromSocketType(const PXSocketType socketType);

	PXPrivate IPAdressFamily ConvertToIPAdressFamily(const unsigned int ipMode);
	PXPrivate unsigned int ConvertFromIPAdressFamily(const IPAdressFamily ipMode);


	PXPublic void PXSocketConstruct(PXSocket* const pxSocket);
	PXPublic void PXSocketDestruct(PXSocket* const pxSocket);

	PXPublic ActionResult PXSocketCreate
	(
		PXSocket* const pxSocket,
		const IPAdressFamily adressFamily,
		const PXSocketType socketType,
		const ProtocolMode protocolMode
	);

	PXPublic ActionResult PXSocketConnect(PXSocket* const pxSocket);

	PXPublic ActionResult PXSocketSetupAdress
	(
		PXSocket* const pxSocketList,
		const size_t PXSocketListSizeMax,
		size_t* PXSocketListSize,
		const char* const ip, // null for any ipAdress
		unsigned short port, // -1 for no port
		IPAdressFamily ipMode,
		PXSocketType socketType,
		ProtocolMode protocolMode
	);

	PXPublic unsigned char PXSocketIsCurrentlyUsed(PXSocket* const pxSocket);
	PXPublic void PXSocketClose(PXSocket* const pxSocket);

	PXPublic ActionResult PXSocketBind(PXSocket* const pxSocket);
	PXPublic ActionResult PXSocketOptionsSet(PXSocket* const pxSocket);
	PXPublic ActionResult PXSocketListen(PXSocket* const pxSocket);
	PXPublic ActionResult PXSocketAccept(PXSocket* server, PXSocket* client);

	PXPublic ActionResult PXSocketSend(PXSocket* const pxSocket, const void* inputBuffer, const size_t inputBufferSize, size_t* inputBytesWritten);
	PXPublic ActionResult PXSocketReceive(PXSocket* const pxSocket, const void* outputBuffer, const size_t outputBufferSize, size_t* outputBytesWritten);

#if OSWindows
	PXPrivate ActionResult WindowsSocketAgentStartup();
	PXPrivate ActionResult WindowsSocketAgentShutdown();
#endif

#ifdef __cplusplus
}
#endif

#endif
