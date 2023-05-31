#ifndef PXCPPSocketINCLUDE
#define PXCPPSocketINCLUDE

#include <OS/Network/PXSocket.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
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
		//  These are reserved for private use by Windows.
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

	typedef struct PXSocket_ PXSocket;

	typedef void (*PXSocketCreatingEvent)(const PXSocket* const pxSocket, unsigned char* use);
	typedef void (*PXSocketCreatedEvent)(const PXSocket* const pxSocket);

	typedef struct PXSocketDataMoveEventInfo_
	{
		PXSocket* SocketSending;
		PXSocketID SocketReceiving;
		void* Data;
		PXSize DataSize;
	}
	PXSocketDataMoveEventInfo;

	typedef void (*PXSocketDataSendEvent)(void* owner, const PXSocketDataMoveEventInfo* const pxSocketDataMoveEventInfo);
	typedef void (*PXSocketDataReceiveEvent)(void* owner, const PXSocketDataMoveEventInfo* const pxSocketDataMoveEventInfo);

	// PXServer Only
	typedef void (*PXSocketListeningEvent)(const PXSocket* const pxSocket);
	typedef void (*PXSocketLinkedEvent)(const PXSocket* const pxSocket);

	// PXClient Only
	typedef void (*PXSocketConnectionEstablishedEvent)(const PXSocket* const pxSocket);
	typedef void (*PXSocketConnectionTerminatedEvent)(const PXSocket* const pxSocket);

	typedef void (*PXClientConnectedEvent)(const PXSocket* serverSocket, const PXSocket* clientSocket);
	typedef void (*PXClientDisconnectedEvent)(const PXSocket* serverSocket, const PXSocket* clientSocket);
	typedef void (*PXClientAcceptFailureEvent)(const PXSocket* serverSocket);

	typedef struct PXSocketEventListener_
	{
		//---<Event CallBack>---------
		PXSocketCreatingEvent SocketCreatingCallback;
		PXSocketCreatedEvent SocketCreatedCallback;

		PXSocketDataSendEvent MessageSendCallback;
		PXSocketDataReceiveEvent MessageReceiveCallback;

		// PXServer Only
		PXSocketListeningEvent ConnectionListeningCallback;
		PXSocketLinkedEvent ConnectionLinkedCallback;

		// PXClient Only
		PXSocketConnectionEstablishedEvent ConnectionEstablishedCallback;
		PXSocketConnectionTerminatedEvent ConnectionTerminatedCallback;
		//----------------------------
	}
	PXSocketEventListener;


	typedef struct PXSocketAdressSetupInfo_
	{
		PXText IP; // null for any ipAdress
		PXInt16U Port; // -1 for no port
		IPAdressFamily IPMode;
		PXSocketType SocketType;
		ProtocolMode ProtocolMode;
	}
	PXSocketAdressSetupInfo;

	class Socket : public PXSocket
	{
		public:
		Socket();
		~Socket();

		PX::ActionResult Create
		(
			const IPAdressFamily adressFamily,
			const PXSocketType socketType,
			const ProtocolMode protocolMode
		);

		PX::ActionResult Connect();

		PX::ActionResult SetupAdress
		(
			const PXSize PXSocketListSizeMax,
			PXSize* PXSocketListSize,
			PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo,
			const PXSize pxSocketAdressSetupInfoSize
		);

		PXBool IsCurrentlyUsed(PXSocket* const pxSocket);
		void Close(PXSocket* const pxSocket);


		PX::ActionResult EventPull(void* const buffer, const PXSize bufferSize);

		PX::ActionResult Bind();
		PX::ActionResult OptionsSet();
		PX::ActionResult Listen();
		PX::ActionResult Accept();

		PX::ActionResult SendAsServerToClient(const PXSocketID clientID, const void* inputBuffer, const PXSize inputBufferSize);

		PX::ActionResult Send(const void* inputBuffer, const PXSize inputBufferSize, PXSize* inputBytesWritten);
		PX::ActionResult Receive(const void* outputBuffer, const PXSize outputBufferSize, PXSize* outputBytesWritten);
		PX::ActionResult ReceiveAsServer(const PXSocketID clientID);

		PX::ActionResult ClientRemove(const PXSocketID clientID);
	};
}

#endif