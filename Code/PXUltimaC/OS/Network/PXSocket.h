#define PXSocketInclude 
#ifndef PXSocketInclude
#define PXSocketInclude

#include <OS/System/PXOSVersion.h>

#define PXSocketUSE OSDeviceToUse == OSDeviceDestop
#if PXSocketUSE



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





	PXPublic void PXAPI PXSocketConstruct(PXSocket* const pxSocket);
	PXPublic void PXAPI PXSocketDestruct(PXSocket* const pxSocket);




	PXPublic PXActionResult PXAPI PXSocketSetupAdress
	(
		PXSocket* const pxSocketList,
		const PXSize PXSocketListSizeMax,
		PXSize* PXSocketListSize,
		const PXSocketAdressSetupInfo* const pxSocketAdressSetupInfo,
		const PXSize pxSocketAdressSetupInfoSize
	);

	PXPublic PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket* const pxSocket);

	PXPublic void PXAPI PXSocketStateChange(PXSocket* const pxSocket, const PXSocketState socketState);


	PXPublic PXActionResult PXAPI PXSocketClientRemove(PXSocket* const serverSocket, const PXSocketID clientID);

#if OSWindows
	PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFetch(void);
	PXPrivate PXActionResult PXAPI PXWindowsSocketAgentErrorFromID(const PXInt32S errorID);
#endif

#ifdef __cplusplus
}
#endif

#endif
#endif