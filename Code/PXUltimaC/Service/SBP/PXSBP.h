#ifndef PXSBPINCLUDE
#define PXSBPINCLUDE

#include <OS/Network/PXClient.h>
#include <OS/Network/PXServer.h>

#if PXSocketUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Time/PXTime.h>
#include <Container/Dictionary/PXDictionary.h>

#define PXSBPMessageChunkHeaderSize 6u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef PXInt16U PXMessageID;

	typedef void (*PXSBPReceiverOnMessageInvalid)(PXSocket* const socket, const PXSocketID socketID);

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

		void* MessageData; // Payload data. Context of this data is not interpreted.
		PXSize MessageSize;// Current size of the data chunk, cant be bigger than expected size.
		PXSize MessageSizeCached;

		PXSBPMessageBitSize MessageBitSize;
		PXSBPMessageStorageType StorageType;

		// Delegation info
		PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.
	
		PXInt8U MessageSizeCachedInPercent;

		// Extended delegation
		PXBool HasExtendedDelegation;	
	}
	PXSBPMessage;

	typedef void (*PXSBPOnMessageUpdatedFunction)(const PXSBPMessage* const pxSBPMessage); // If message has updated but not fully recieved
	typedef void (*PXSBPOnMessageReceivedFunction)(const PXSBPMessage* const pxSBPMessage); // if message is fully recieved

	PXPublic void PXSBPMessageConstruct(PXSBPMessage* const pxSBPMessage);
	PXPublic void PXSBPMessageParse(PXSBPMessage* const pxSBPMessage, const void* const data, const PXSize dataSize);
	//-----------------------------------------------------



	//-----------------------------------------------------
	// SBP Chunk Cache
	//-----------------------------------------------------
	typedef struct PXSBPChunkCache_
	{
		void* Message; // Payload data. Context of this data is not interpreted.

		PXInt16U SizeCurrent;// Current size of the data chunk, cant be bigger than expected size.
		PXInt16U SizeExpected;// Expected size of the data. Known from the header info of the first chunk.
		PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.
	}
	PXSBPChunkCache;

	typedef void (*PXSBPOnChunkSegmentUpdatedFunction)(const PXSBPChunkCache* const pxSBPChunkSegment);

	PXPrivate PXBool PXSBPMessageChunkParse(PXSBPChunkCache* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);

	PXPublic PXInt16U PXSBPMessageSizeMissing(PXSBPChunkCache* const pxSBPMessage);
	PXPublic PXBool PXSBPMessageSizeComplete(PXSBPChunkCache* const pxSBPMessage);	

	PXPublic PXBool PXSBPMessageChunkDataIsComplete(const PXSBPChunkCache* const pxSBPMessageChunk);
	PXPublic PXBool PXSBPMessageChunkDataContainsMultible(const PXSBPChunkCache* const pxSBPMessageChunk);

	// Consumes data that will be merged into the SBP-Message. The result is the size consumed by this function.
	PXPublic PXSize PXSBPMessageChunkDataConsume(PXSBPChunkCache* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);

	PXPublic void PXSBPMessageConstructSet(PXSBPChunkCache* const pxSBPMessage);
	PXPublic void PXSBPMessageConstructAllocate(PXSBPChunkCache* const pxSBPMessage);
	//-----------------------------------------------------



	//-----------------------------------------------------
	// SBP Chunk
	//-----------------------------------------------------
	typedef struct PXSBPChunk_
	{
		void* Data;
		PXInt16U DataSize;
		PXMessageID ChannelID;
	}
	PXSBPChunk;

	typedef void (*PXSBPOnChunkReceivedFunction)(const PXSBPChunk* const pxSBPChunk);
	typedef void (*PXSBPOnChunkEmittedFunction)(const PXSBPChunk* const pxSBPChunk);
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

	PXPublic void PXSBPReceiverConstruct(PXSBPReceiver* const pxSBPReceiver);

	PXPrivate void PXSBPReceiverStateChanged(PXSBPReceiver* const pxSBPReceiver, const PXSBPRecieverState pxSBPRecieverState);

	PXPublic void PXSBPReceiverEventListSet(PXSBPReceiver* const pxSBPReceiver, PXSBPReceiverEventList* const pxSBPReceiverEventList);

	PXPrivate void PXSBPOnDataRawReceive(PXSBPReceiver* const pxSBPReceiver, const PXSocketDataReceivedEventData* const pxSocketDataReceivedEventData);
	PXPrivate void PXSBPOnDataChunkReceive(PXSBPReceiver* const pxSBPReceiver, const PXSBPChunk* const pxSBPChunk);
	PXPrivate void PXSBPOnDataMessageReceive(PXSBPReceiver* const pxSBPReceiver, const PXSBPMessage* const pxSBPMessage);
	//-----------------------------------------------------



	//-----------------------------------------------------
	// SBP Emitter
	//-----------------------------------------------------
	typedef struct PXSBPEmitter_
	{
		PXSBPOnChunkEmittedFunction OnChunkEmittedCallBack;

		// Message queue?
		PXInt16U PackageSizeMaximal;
	}
	PXSBPEmitter;

	PXPublic void PXSBPEmitterConstruct(PXSBPEmitter* const pxSBPEmitter);
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

	PXPublic void PXSBPClientConstruct(PXSBPClient* const pxSBPClient);
	PXPublic void PXSBPClientDestruct(PXSBPClient* const pxSBPClient);

	PXPublic void PXSBPClientReceiverEventListSet(PXSBPClient* const pxSBPClient, PXSBPReceiverEventList* const pxSBPReceiverEventList);

	PXPublic PXActionResult PXSBPClientConnectToServer(PXSBPClient* const pxSBPClient, const PXText* const ip, const PXInt16U port);
	PXPublic PXActionResult PXSBPClientDisconnectFromServer(PXSBPClient* const pxSBPClient);

	PXPublic PXActionResult PXSBPClientSendMessage(PXSBPClient* const pxSBPClient, const void* const data, const PXSize dataSize);
	PXPublic PXActionResult PXSBPClientSendFile(PXSBPClient* const pxSBPClient, const PXText* const filePath);
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

	PXPublic void PXSBPServerConstruct(PXSBPServer* const pxSBPServer);
	PXPublic void PXSBPServerDestruct(PXSBPServer* const pxSBPServer);

	PXPublic void PXSBPServerReceiverEventListSet(PXSBPServer* const pxSBPServe, PXSBPReceiverEventList* const pxSBPReceiverEventList);

	PXPublic PXActionResult PXSBPServerStart(PXSBPServer* const pxSBPServer, const PXInt16U port);
	PXPublic PXActionResult PXSBPServerStop(PXSBPServer* const pxSBPServer);
	//-----------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
#endif