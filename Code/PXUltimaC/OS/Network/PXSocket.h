#ifndef PXSocketInclude
#define PXSocketInclude

#include <OS/System/PXOSVersion.h>

#define PXSocketUSE OSDeviceToUse == OSDeviceDestop
#if PXSocketUSE

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

//-----------------------------------------------

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Async/PXLock.h>
#include <OS/Async/PXThread.h>
#include <OS/Error/PXActionResult.h>
#include <Container/Dictionary/PXDictionary.h>
#include <Container/Buffer/PXBuffer.h>

typedef PXInt32U PXSocketID;

#define SocketDebug 0
#define IPv6LengthMax 65
#define PXSocketBufferSize 1024u
#define PXSocketUnused (PXSocketID)-1


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

	//---<Events-------------------------------------------
	typedef struct PXSocket_ PXSocket;



	typedef struct PXSocketDataReceivedEventData_
	{
		const void* Data;
		PXSize DataSize;
		PXSocket* SocketReceiving;
		PXSocketID SocketSending;
	}
	PXSocketDataReceivedEventData;

	typedef struct PXSocketDataSendEventData_
	{
		const void* Data;
		PXSize DataSize;
		PXSocket* SocketSending;
		PXSocketID SocketReceiving;
	}
	PXSocketDataSendEventData;


	//-----------------------------------------------------
	typedef void (PXAPI*PXSocketCreatingEvent)(void* owner, const PXSocket* const pxSocket, PXBool* use);
	typedef void (PXAPI*PXSocketCreatedEvent)(void* owner, const PXSocket* const pxSocket);

	typedef void (PXAPI*PXSocketClosedEvent)(void* owner, const PXSocket* pxSocket);

	typedef void (PXAPI*PXSocketConnectedEvent)(void* owner, const PXSocket* serverSocket, const PXSocket* clientSocket);
	typedef void (PXAPI*PXSocketDisconnectedEvent)(void* owner, const PXSocket* serverSocket, const PXSocket* clientSocket);

	typedef void (PXAPI*PXSocketStateChangedEvent)(void* owner, const PXSocket* pxSocket, const PXSocketState oldState, const PXSocketState newState);

	typedef void (PXAPI*PXSocketDataSendEvent)(void* owner, const PXSocketDataSendEventData* const pxSocketDataSendEventData);
	typedef void (PXAPI*PXSocketDataReceiveEvent)(void* owner, const PXSocketDataReceivedEventData* const pxSocketDataReceivedEventData);

	typedef struct PXSocketEventList_
	{
		PXSocketCreatingEvent SocketCreatingCallBack;
		PXSocketCreatedEvent SocketCreatedCallBack;

		PXSocketClosedEvent SocketClosedCallBack;

		PXSocketConnectedEvent SocketConnectedCallBack;
		PXSocketDisconnectedEvent SocketDisconnectedCallBack;

		PXSocketStateChangedEvent SocketStateChangedCallBack;

		PXSocketDataSendEvent SocketDataSendCallBack;
		PXSocketDataReceiveEvent SocketDataReceiveCallBack;
	}
	PXSocketEventList;



	typedef struct PXSocket_
	{
		PXSocketEventList EventList;

		PXSocketState State;

		// Connection Info
		PXSocketID ID;
		PXProtocolMode Protocol;
		IPAdressFamily Family;
		PXSocketType Type;
		char IP[IPv6LengthMax];
		PXSize IPSize;
		PXInt16U Port;


		void* Owner;

		PXBuffer BufferInput;
		PXBuffer BufferOutput;

		//----------------------------
		PXDictionary SocketLookup;
		PXLock* PollingLock;
		//----------------------------

		//---<Private IO>------------
		PXThread CommunicationThread;
		//----------------------------
	}
	PXSocket;

	typedef struct PXSocketAdressSetupInfo_
	{
		PXText IP; // null for any ipAdress
		PXInt16U Port; // -1 for no port
		IPAdressFamily IPMode;
		PXSocketType SocketType;
		PXProtocolMode ProtocolMode;
	}
	PXSocketAdressSetupInfo;


	PXPrivate PXProtocolMode PXAPI PXProtocolModeFromID(const PXInt8U protocolMode);
	PXPrivate PXInt8U PXAPI PXProtocolModeToID(const PXProtocolMode protocolMode);

	PXPrivate PXSocketType PXAPI PXSocketTypeFromID(const PXInt8U socketType);
	PXPrivate PXInt8U PXAPI PXSocketTypeToID(const PXSocketType socketType);

	PXPrivate IPAdressFamily PXAPI PXIPAdressFamilyFromID(const PXInt8U ipMode);
	PXPrivate PXInt8U PXAPI PXIPAdressFamilyToID(const IPAdressFamily ipMode);


	PXPublic void PXAPI PXSocketConstruct(PXSocket* const pxSocket);
	PXPublic void PXAPI PXSocketDestruct(PXSocket* const pxSocket);

	PXPublic PXActionResult PXAPI PXSocketCreate
	(
		PXSocket* const pxSocket,
		const IPAdressFamily adressFamily,
		const PXSocketType socketType,
		const PXProtocolMode protocolMode
	);

	PXPublic PXActionResult PXAPI PXSocketConnect(PXSocket* const pxSocket, PXSocket* const pxServer);

	PXPublic PXActionResult PXAPI PXSocketSetupAdress
	(
		PXSocket* const pxSocketList,
		const PXSize PXSocketListSizeMax,
		PXSize* PXSocketListSize,
		const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo,
		const PXSize pxSocketAdressSetupInfoSize
	);

	PXPublic PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket* const pxSocket);
	PXPublic void PXAPI PXSocketClose(PXSocket* const pxSocket);

	PXPublic void PXAPI PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState);

	PXPublic PXActionResult PXAPI PXSocketEventPull(PXSocket* const pxSocket);

	PXPublic PXActionResult PXAPI PXSocketBind(PXSocket* const pxSocket);
	PXPublic PXActionResult PXAPI PXSocketOptionsSet(PXSocket* const pxSocket);
	PXPublic PXActionResult PXAPI PXSocketListen(PXSocket* const pxSocket);
	PXPublic PXActionResult PXAPI PXSocketAccept(PXSocket* const server);

	PXPublic PXActionResult PXAPI PXSocketSend(PXSocket* const pxSocketSender, const PXSocketID pxSocketReceiverID);
	PXPublic PXActionResult PXAPI PXSocketReceive(PXSocket* const pxSocketSender, const PXSocketID pxSocketSenderID);

	PXPublic PXActionResult PXAPI PXSocketClientRemove(PXSocket* const serverSocket, const PXSocketID clientID);

#if OSWindows
	PXPrivate PXActionResult PXAPI WindowsSocketAgentStartup(void);
	PXPrivate PXActionResult PXAPI WindowsSocketAgentShutdown(void);
	PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFetch(void);
	PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFromID(const PXInt32S errorID);
#endif

#ifdef __cplusplus
}
#endif

#endif
#endif