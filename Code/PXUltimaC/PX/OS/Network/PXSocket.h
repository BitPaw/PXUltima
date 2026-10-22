#ifndef PXSocketIncluded
#define PXSocketIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXText.h>
#include <PX/OS/Async/PXLock.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include "PXNetwork.h"

#define SocketDebug 0
#define IPv6LengthMax 65
#define PXSocketBufferSize 1024u
#define PXSocketUnused (PXSocketID)-1

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
typedef void (PXAPI*PXSocketCreatingEvent)(void* owner, const PXSocket PXREF pxSocket, PXBool* use);
typedef void (PXAPI*PXSocketCreatedEvent)(void* owner, const PXSocket PXREF pxSocket);

typedef void (PXAPI*PXSocketClosedEvent)(void* owner, const PXSocket* pxSocket);

typedef void (PXAPI*PXSocketConnectedEvent)(void* owner, const PXSocket* serverSocket, const PXSocket* clientSocket);
typedef void (PXAPI*PXSocketDisconnectedEvent)(void* owner, const PXSocket* serverSocket, const PXSocket* clientSocket);

typedef void (PXAPI*PXSocketStateChangedEvent)(void* owner, const PXSocket* pxSocket, const PXSocketState oldState, const PXSocketState newState);

typedef void (PXAPI*PXSocketDataSendEvent)(void* owner, const PXSocketDataSendEventData PXREF pxSocketDataSendEventData);
typedef void (PXAPI*PXSocketDataReceiveEvent)(void* owner, const PXSocketDataReceivedEventData PXREF pxSocketDataReceivedEventData);

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
    PXECSInfo Info;

    PXSocketID ID;


    PXSocketEventList EventList;

    PXSocketState State;

    // Connection Info

    PXProtocolMode Protocol;
    IPAddressFamily Family;
    PXSocketType Type;
    char IP[IPv6LengthMax];
    char Name[32];  // NI_MAXHOST
    PXSize IPSize;
    PXI16U Port;


    void* Owner;
}
PXSocket;

typedef struct PXSocketCreateInfo_
{
    PXECSCreateInfo Info;

    PXText IP; // null for any ipAddress
    PXI16U Port; // -1 for no port
    IPAddressFamily IPMode;
    PXSocketType SocketType;
    PXProtocolMode ProtocolMode;
}
PXSocketCreateInfo;

typedef struct PXSocketConnectionInfo_
{
    PXSocketID ID;
    PXIPAddress IP;
    PXI16U Port;

    IPAddressFamily AddressFamily;
    PXSocketType Type;
    PXProtocolMode ProtocolMode;
}
PXSocketConnectionInfo;

typedef struct PXSocketConnectInfo_
{
    PXText IP;
    PXI16U Port;

    IPAddressFamily AddressFamily;
    PXSocketType Type;
    PXProtocolMode ProtocolMode;
}
PXSocketConnectInfo;

typedef struct PXSocketListenInfo_
{
    PXI32U ClientInQueueMaximal;
}
PXSocketListenInfo;

typedef struct PXSocketBindInfo_
{
    PXText* IP;
    PXI16U Port;
}
PXSocketBindInfo;

typedef struct PXSocketDataInfo_
{
    PXBuffer Buffer;

    PXSocket* SocketPeer;

    PXSize SegmentSize; // Size of each action of send/read
    PXSize SegmentDelay;

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


PXPublic void PXAPI PXSocketRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSocketCreate(PXSocket** pxSocketREF, PXSocketCreateInfo PXREF pxSocketCreateInfo);
PXPublic PXResult PXAPI PXSocketRelease(PXSocket PXREF pxSocket);

//PXPublic PXResult PXAPI PXSocketDestroyViaID(const PXSocketID pxSocketID);
PXPublic PXResult PXAPI PXSocketConnect(PXSocket PXREF pxSocket, PXSocketConnectInfo PXREF pxSocketConnectInfo);
PXPublic PXResult PXAPI PXSocketListen(PXSocket PXREF pxSocket, PXSocketListenInfo PXREF pxSocketListenInfo);
PXPublic PXResult PXAPI PXSocketAccept(PXSocket PXREF pxSocket, PXSocketAcceptInfo PXREF pxSocketAcceptInfo);
PXPublic PXResult PXAPI PXSocketBind(PXSocket PXREF pxSocket, PXSocketBindInfo PXREF pxSocketBindInfo);
PXPublic PXResult PXAPI PXSocketReceive(PXSocket PXREF pxSocket, PXSocketDataInfo PXREF pxSocketDataInfo);
PXPublic PXResult PXAPI PXSocketSend(PXSocket PXREF pxSocket, PXSocketDataInfo PXREF pxSocketDataInfo);
PXPublic PXResult PXAPI PXSocketPoll(PXSocket PXREF pxSocket);
PXPublic PXResult PXAPI PXSocketClose(PXSocket PXREF pxSocket);
PXPublic PXResult PXAPI PXSocketNameSelfGet(const PXSocket PXREF pxSocket, PXText PXREF pxText);
PXPublic PXResult PXAPI PXSocketNamePeerGet(const PXSocket PXREF pxSocket, PXText PXREF pxText);

PXPublic PXI32U PXAPI PXSocketMTUGet(const PXSocket PXREF pxSocket);
PXPublic PXResult PXAPI PXSocketMTUSet(const PXSocket PXREF pxSocket, PXI32U value);


typedef struct PXSocketStatistics
{
    PXI32U RoundTripTimeCurrentUS; // RTT
    PXI32U RoundTripTimeMinimalUS; // RTT min
    PXI32U TransmissionUnitMaximum; // MTU, OSI-2/3, includes headers, full message in network.
    PXI32U SegmentSizeMaximum; // Mss, OSI-4, excluding headers, our raw data payload.
}
PXSocketStatistics;

PXPublic PXResult PXAPI PXSocketStatisticsGet(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics);

PXPrivate PXResult PXAPI PXSocketStatisticsGet_W10(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics);
PXPrivate PXResult PXAPI PXSocketStatisticsGet_W7(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics);
PXPrivate PXResult PXAPI PXSocketStatisticsGet_NT(const PXSocket PXREF pxSocket, PXSocketStatistics PXREF pxSocketStatistics);

PXPublic PXResult PXAPI PXSocketNameGet(const PXSocket PXREF pxSocket, PXText PXREF pxText);
PXPublic PXResult PXAPI PXSocketInterfaceName(const PXSocket PXREF pxSocket, char* name, const PXSize nameSize);


PXPublic PXBool PXAPI PXSocketIsCurrentlyUsed(PXSocket PXREF pxSocket);

PXPublic void PXAPI PXSocketStateChange(PXSocket PXREF pxSocket, const PXSocketState socketState);


PXPublic PXResult PXAPI PXSocketClientRemove(PXSocket PXREF serverSocket, const PXSocketID clientID);

#if OSWindows
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFetch(void);
PXPrivate PXResult PXAPI PXWindowsSocketAgentErrorFromID(const PXI32S errorID);
#endif

#endif