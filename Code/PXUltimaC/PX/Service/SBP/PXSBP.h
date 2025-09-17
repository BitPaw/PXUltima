#ifndef PXSBPIncluded
#define PXSBPIncluded

#include <PX/OS/Network/PXClient.h>
#include <PX/OS/Network/PXServer.h>

#if PXSocketUSE

#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Time/PXTime.h>
#include <PX/Container/Dictionary/PXDictionary.h>

#define PXSBPMessageChunkHeaderSize 6u

#ifdef __cplusplus
extern "C"
{
#endif

typedef PXI16U PXMessageID;

typedef void (PXAPI*PXSBPReceiverOnMessageInvalid)(PXSocket* const socket, const PXSocketID socketID);

typedef enum PXSBPRecieverState_
{
    PXSBPRecieverStateInvalid,
    PXSBPRecieverStateAwaitingHeaderBegin,
    PXSBPRecieverStateAwaitingHeaderEnd,
    PXSBPRecieverStateAwaitingDataStart,
    PXSBPRecieverStateAwaitingDataEnd,
    PXSBPRecieverStateProcessingData
}
PXSBPRecieverState;

// Header Info Byte
// 2 Bits Version
// 1 Bit has folowing Extended source/Target header
// 5 Bits currently unused
typedef enum PXSBPMessageBitSize_
{
    PXSBPMessageBitSizeInvalid,
    PXSBPMessageBitSize8Bit,
    PXSBPMessageBitSize16Bit,
    PXSBPMessageBitSize32Bit,
    PXSBPMessageBitSize64Bit
}
PXSBPMessageBitSize;

typedef enum PXSBPMessageStorageType_
{
    PXSBPMessageStorageTypeInvalid,
    PXSBPMessageStorageTypeDirect,
    PXSBPMessageStorageTypeCacheCollected,
}
PXSBPMessageStorageType;


//-----------------------------------------------------
// SBP Message, 64 Bytes
//-----------------------------------------------------
typedef struct PXSBPMessage_
{
    // Runtime info
    PXTime FirstKnown;
    PXTime LastKnown;

    // Raw data
    void* Owner;

    void* Data; // Payload data. Context of this data is not interpreted.
    PXSize DataSizeCurrent;// Current size of the data chunk, cant be bigger than expected size.
    PXSize DataSizeExpected;

    //PXSBPMessageBitSize MessageBitSize;
    PXSBPMessageStorageType StorageType;

    // Delegation info
    PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.

    PXI8U MessageSizeCachedInPercent;

    // Extended delegation
    PXBool HasExtendedDelegation;
}
PXSBPMessage;

typedef void (PXAPI*PXSBPOnMessageUpdatedFunction)(const PXSBPMessage* const pxSBPMessage); // If message has updated but not fully recieved
typedef void (PXAPI*PXSBPOnMessageReceivedFunction)(const PXSBPMessage* const pxSBPMessage); // if message is fully recieved

PXPublic void PXAPI PXSBPMessageConstruct(PXSBPMessage* const pxSBPMessage);
PXPublic void PXAPI PXSBPMessageConstructFlat(PXSBPMessage* const pxSBPMessage, const void* const data, const PXSize dataSize, const void* const owner);
PXPublic void PXAPI PXSBPMessageParse(PXSBPMessage* const pxSBPMessage, const void* const data, const PXSize dataSize);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Chunk Cache
//-----------------------------------------------------
typedef struct PXSBPChunkCache_
{
    void* Message; // Payload data. Context of this data is not interpreted.

    PXI16U SizeCurrent;// Current size of the data chunk, cant be bigger than expected size.
    PXI16U SizeExpected;// Expected size of the data. Known from the header info of the first chunk.
    PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.
}
PXSBPChunkCache;

typedef void (PXAPI*PXSBPOnChunkSegmentUpdatedFunction)(const PXSBPChunkCache* const pxSBPChunkSegment);

PXPrivate PXBool PXAPI PXSBPMessageChunkParse(PXSBPChunkCache* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);

PXPublic PXI16U PXAPI PXSBPMessageSizeMissing(PXSBPChunkCache* const pxSBPMessage);
PXPublic PXBool PXAPI PXSBPMessageSizeComplete(PXSBPChunkCache* const pxSBPMessage);

PXPublic PXBool PXAPI PXSBPMessageChunkDataIsComplete(const PXSBPChunkCache* const pxSBPMessageChunk);
PXPublic PXBool PXAPI PXSBPMessageChunkDataContainsMultible(const PXSBPChunkCache* const pxSBPMessageChunk);

// Consumes data that will be merged into the SBP-Message. The result is the size consumed by this function.
PXPublic PXSize PXAPI PXSBPMessageChunkDataConsume(PXSBPChunkCache* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);

PXPublic void PXAPI PXSBPMessageConstructSet(PXSBPChunkCache* const pxSBPMessage);
PXPublic void PXAPI PXSBPMessageConstructAllocate(PXSBPChunkCache* const pxSBPMessage);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Chunk
//-----------------------------------------------------
typedef struct PXSBPChunk_
{
    void* Data;
    PXI16U DataSize;
    PXMessageID ChannelID;
}
PXSBPChunk;

typedef void (PXAPI*PXSBPOnChunkReceivedFunction)(const PXSBPChunk* const pxSBPChunk);
typedef void (PXAPI*PXSBPOnChunkEmittedFunction)(const PXSBPChunk* const pxSBPChunk);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Receiver
//-----------------------------------------------------
typedef struct PXSBPReceiverEventList_
{
    void* Owner;

    PXSBPOnMessageUpdatedFunction OnMessageUpdatedCallBack;
    PXSBPOnMessageReceivedFunction OnMessageReceivedCallBack;

    PXSBPOnChunkSegmentUpdatedFunction OnChunkSegmentUpdatedCallBack;
    PXSBPOnChunkReceivedFunction OnChunkReceivedCallBack;

    PXSBPReceiverOnMessageInvalid OnMessageInvalidCallBack;
}
PXSBPReceiverEventList;

typedef struct PXSBPReceiver_
{
    PXSBPReceiverEventList EventList;

    PXSBPRecieverState State;

    PXDictionary MessageStreamLookup;

    PXSBPChunkCache MessageChunkCurrent;

    char HeaderCacheSize;
    char HeaderCache[PXSBPMessageChunkHeaderSize];

    PXBool EnableSBP;
}
PXSBPReceiver;

PXPublic void PXAPI PXSBPReceiverConstruct(PXSBPReceiver* const pxSBPReceiver);

PXPrivate void PXAPI PXSBPReceiverStateChanged(PXSBPReceiver* const pxSBPReceiver, const PXSBPRecieverState pxSBPRecieverState);

PXPublic void PXAPI PXSBPReceiverEventListSet(PXSBPReceiver* const pxSBPReceiver, PXSBPReceiverEventList* const pxSBPReceiverEventList);

PXPrivate void PXAPI PXSBPOnDataRawReceive(PXSBPReceiver* const pxSBPReceiver, const PXSocketDataReceivedEventData* const pxSocketDataReceivedEventData);
PXPrivate void PXAPI PXSBPOnDataChunkReceive(PXSBPReceiver* const pxSBPReceiver, const PXSBPChunk* const pxSBPChunk);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Emitter
//-----------------------------------------------------
typedef struct PXSBPEmitter_
{
    PXSBPOnChunkEmittedFunction OnChunkEmittedCallBack;

    // Message queue?
    PXI16U PackageSizeMaximal;

    PXI16U MessageID;

    PXSocket* SocketSender;
    PXSocketID SocketReceiverID;
}
PXSBPEmitter;

PXPublic void PXAPI PXSBPEmitterConstruct(PXSBPEmitter* const pxSBPEmitter);
PXPublic PXResult PXAPI PXSBPEmitterDeploy(PXSBPEmitter* const pxSBPEmitter, const void* const message, const PXSize messageSize);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Client
//-----------------------------------------------------
typedef struct PXSBPClient_
{
    PXClient Client;
    PXSBPReceiver Receiver;
    PXSBPEmitter Emitter;

    PXBool EnableSBP;
}
PXSBPClient;

PXPublic void PXAPI PXSBPClientConstruct(PXSBPClient* const pxSBPClient);
PXPublic void PXAPI PXSBPClientDestruct(PXSBPClient* const pxSBPClient);

PXPublic void PXAPI PXSBPClientReceiverEventListSet(PXSBPClient* const pxSBPClient, PXSBPReceiverEventList* const pxSBPReceiverEventList);

PXPublic PXResult PXAPI PXSBPClientConnectToServer(PXSBPClient* const pxSBPClient, const PXText* const ip, const PXI16U port);
PXPublic PXResult PXAPI PXSBPClientDisconnectFromServer(PXSBPClient* const pxSBPClient);

PXPublic PXResult PXAPI PXSBPClientSendMessage(PXSBPClient* const pxSBPClient, const void* const data, const PXSize dataSize);
PXPublic PXResult PXAPI PXSBPClientSendFile(PXSBPClient* const pxSBPClient, const PXText* const filePath);
//-----------------------------------------------------



//-----------------------------------------------------
// SBP Server
//-----------------------------------------------------
typedef struct PXSBPServer_
{
    PXServer Server;
    PXSBPReceiver Receiver;
    PXSBPEmitter Emitter;
}
PXSBPServer;

PXPublic void PXAPI PXSBPServerConstruct(PXSBPServer* const pxSBPServer);
PXPublic void PXAPI PXSBPServerDestruct(PXSBPServer* const pxSBPServer);

PXPublic void PXAPI PXSBPServerReceiverEventListSet(PXSBPServer* const pxSBPServe, PXSBPReceiverEventList* const pxSBPReceiverEventList);

PXPublic PXResult PXAPI PXSBPServerStart(PXSBPServer* const pxSBPServer, const PXI16U port);
PXPublic PXResult PXAPI PXSBPServerStop(PXSBPServer* const pxSBPServer);

PXPublic PXResult PXAPI PXSBPServerSendToAll(PXSBPServer* const pxSBPServer, const void* const data, const PXSize dataSize);
//-----------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
#endif
