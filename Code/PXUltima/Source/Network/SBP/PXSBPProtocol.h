#ifndef PXSBPProtocolINCLUDE
#define PXSBPProtocolINCLUDE

#include <OS/Network/PXClient.h>
#include <OS/Network/PXServer.h>
#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Time/PXTime.h>
#include <Container/Dictionary/PXDictionary.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

	typedef PXInt16U PXMessageID;



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

	typedef struct PXSBPMessage_
	{
		// Raw data
		void* MessageData; // Payload data. Context of this data is not interpreted.
		PXSize MessageSize;// Current size of the data chunk, cant be bigger than expected size.

		// Delegation info
		PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.
		PXSize MessageSizeCached;
		PXInt8U MessageSizeCachedInPercent;

		// Extended delegation
		PXBool HasExtendedDelegation;

		// Runtime info
		PXTime FirstKnown;
		PXTime LastKnown;
	}
	PXSBPMessage;




	PXPublic void PXSBPMessageConstruct(PXSBPMessage* const pxSBPMessage);
	PXPublic void PXSBPMessageParse(PXSBPMessage* const pxSBPMessage, const void* const data, const PXSize dataSize);


#define PXSBPMessageChunkHeaderSize 6u


	typedef void (*PXSBPReceiverOnMessageInvalid)(PXSocket* const socket, const PXSocketID socketID);

	typedef void (*PXSBPMessageOnMessageReceivedEvent)(PXSBPMessage* const pxSBPMessage);
	typedef void (*PXSBPMessageOnMessageProgressUpdatedEvent)(PXSBPMessage* const pxSBPMessage);

	typedef struct PXSBPChunkSegment_
	{
		void* Message; // Payload data. Context of this data is not interpreted.

		PXInt16U SizeCurrent;// Current size of the data chunk, cant be bigger than expected size.
		PXInt16U SizeExpected;// Expected size of the data. Known from the header info of the first chunk.
		PXMessageID ID; // ID that belongs to message itself. So that chunks can conbine them.
	}
	PXSBPChunkSegment;



	typedef struct PXSBPChunk_
	{
		void* Data;
		PXInt16U DataSize;
		PXMessageID ChannelID;
	}
	PXSBPChunk;








	PXPublic PXBool PXSBPMessageChunkDataIsComplete(const PXSBPChunkSegment* const pxSBPMessageChunk);
	PXPublic PXBool PXSBPMessageChunkDataContainsMultible(const PXSBPChunkSegment* const pxSBPMessageChunk);

	// Consumes data that will be merged into the SBP-Message. The result is the size consumed by this function.
	PXPublic PXSize PXSBPMessageChunkDataConsume(PXSBPChunkSegment* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);


	typedef struct PXSBPReceiver_
	{
		PXSBPRecieverState State;

		PXDictionary MessageStreamLookup;

		PXSBPChunkSegment MessageChunkCurrent;

		char HeaderCacheSize;
		char HeaderCache[PXSBPMessageChunkHeaderSize];

		// External events
		PXSBPReceiverOnMessageInvalid OnMessageInvalidCallBack;


		PXSBPMessageOnMessageReceivedEvent OnMessageReceivedCallBack;
		PXSBPMessageOnMessageProgressUpdatedEvent OnMessageProgressUpdatedCallBack;

	}
	PXSBPReceiver;

	typedef struct PXSBPEmitter_
	{
		int dummy;

		PXSize CacheSizeCurrent;
		PXSize CacheSizeMax;
		char Cache[1024];


		// Message queue?
		PXInt16U PackageSizeMaximal;

	}
	PXSBPEmitter;

	typedef struct PXSBPClient_
	{
		PXClient Client;
		PXSBPReceiver Receiver;
		PXSBPEmitter Emitter;
	}
	PXSBPClient;

	typedef struct PXSBPServer_
	{
		PXServer Server;
		PXSBPReceiver Receiver;
		PXSBPEmitter Emitter;
	}
	PXSBPServer;

	// PXSBPServer

	PXPublic void PXSBPServerConstruct(PXSBPServer* const pxSBPServer);
	PXPublic void PXSBPServerDestruct(PXSBPServer* const pxSBPServer);


	// PXSBPClient

	PXPublic void PXSBPClientConstruct(PXSBPClient* const pxSBPClient);
	PXPublic void PXSBPClientDestruct(PXSBPClient* const pxSBPClient);



	PXPublic void PXSBPClientSendMessage(PXSBPClient* const pxSBPClient, const void* const data, const PXSize dataSize);
	PXPublic void PXSBPClientSendFile(PXSBPClient* const pxSBPClient, const PXText* const filePath);


	// PXSBPReceiver
	PXPublic void PXSBPReceiverConstruct(PXSBPReceiver* const pxSBPReceiver);

	PXPrivate void PXSBPOnDataRawReceive(PXSBPReceiver* const pxSBPReceiver, const PXSocketDataMoveEventInfo* const pxSocketDataMoveEventInfo);
	PXPrivate void PXSBPOnDataChunkReceive(PXSBPReceiver* const pxSBPReceiver, const PXSBPChunk* const pxSBPChunk);

	// PXSBPEmitter

	PXPublic void PXSBPEmitterConstruct(PXSBPEmitter* const pxSBPEmitter);


	PXPublic void PXSBPEmitterBeginAndReset(PXSBPEmitter* const pxSBPEmitter);
	PXPublic void PXSBPEmitterAppend(PXSBPEmitter* const pxSBPEmitter, const void* data, const PXSize dataSize);
	PXPublic void PXSBPEmitterEndAndDeploy(PXSBPEmitter* const pxSBPEmitter);

	PXPrivate void PXSBPEmitterSendChunk(PXSBPEmitter* const pxSBPEmitter, const void* data, const PXSize dataSize);













	PXPrivate PXBool PXSBPMessageChunkParse(PXSBPChunkSegment* const pxSBPMessageChunk, const void* const data, const PXSize dataSize);

	PXPrivate void PXSBPReceiverStateChanged(PXSBPReceiver* const pxSBPReceiver, const PXSBPRecieverState pxSBPRecieverState);





	PXPublic PXInt16U PXSBPMessageSizeMissing(PXSBPChunkSegment* const pxSBPMessage);
		PXPublic PXBool PXSBPMessageSizeComplete(PXSBPChunkSegment* const pxSBPMessage);
      
    PXPublic void PXSBPMessageConstruct(PXSBPChunkSegment* const pxSBPMessage);
    PXPublic void PXSBPMessageConstructSet(PXSBPChunkSegment* const pxSBPMessage);
    PXPublic void PXSBPMessageConstructAllocate(PXSBPChunkSegment* const pxSBPMessage);










#ifdef __cplusplus
}
#endif

#endif
