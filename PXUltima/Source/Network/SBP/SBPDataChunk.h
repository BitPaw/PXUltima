#ifndef SBPDataChunkINCLUDE
#define SBPDataChunkINCLUDE

#include <Format/Type.h>
#include <File/PXDataStream.h>
#include <Container/Dictionary/PXDictionary.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum SBPDataChunkResult_
	{
		SBPDataChunkResultInvalid,

		// If a chunk header is expected and does not follow the format.
		SBPDataChunkIllegal,

		// Data is not fully complete and is stored until the additional data has been serverd
		SBPDataChunkCached,

		// Data is now complete and ready to be consumed
		SBPDataChunkComplete,

		// Data has not been cached. It can be handled fully at once.
		SBPDataChunkComsumeImmediately,
	}
	SBPDataChunkResult;

	typedef enum SBPDataCacheState_
	{
		SBPDataCacheStateInvalid,
		SBPDataCacheStateAwaitHeaderStart,
		SBPDataCacheStateAwaitHeaderRemainingData,
		SBPDataCacheStateAwaitData
	}
	SBPDataCacheState;

	typedef enum SBPDataCacheChunkOrder_
	{
		SBPDataCacheChunkOrderInvalid,
		SBPDataCacheChunkOrderClose, // 0b00
		SBPDataCacheChunkOrderFirst, // 0b01
		SBPDataCacheChunkOrderMiddle, // 0b10
		SBPDataCacheChunkOrderFinal, // 0b11
	}
	SBPDataCacheChunkOrder;

	typedef struct SBPDataChunk_
	{
		// Info
		SBPDataCacheChunkOrder Order;
		PXInt8U Channal;
		//----------------

		//--<Spesific temp data>---
		PXInt16U DataSizeCurrent;
		PXInt16U DataSizeTotal;	
		//-----------------------

		// Volitile data
		PXByte* Data;
		
	}
	SBPDataChunk;


	typedef struct SBPDataCache_ SBPDataCache;

	typedef void (*SBPDataChunkIOEvent)(void* const owner, SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk);
	typedef void (*SBPDataChunkChannalCreated)(void* const owner, SBPDataCache* const sbpDataCache, const PXInt8U channalID);

	typedef struct SBPDataCache_
	{
		SBPDataCacheState State;

		// Data holding recieving data
		PXSize DataHeaderStartOffset;

		PXDataStream Data;

		// Callbacks for each channel
		PXDictionary DataCallBackLookup;

		// general CallBacks

		void* Owmer;

		SBPDataChunkChannalCreated ChannalCreatedCallBack;
		SBPDataChunkIOEvent DataChunkRecievedCallBack;
	}
	SBPDataCache;




	PXPublic void SBPDataCacheConstruct(SBPDataCache* const sbpDataCache);
	PXPublic void SBPDataCacheDestruct(SBPDataCache* const sbpDataCache);

	PXPublic void SBPDataCacheStateChange(SBPDataCache* const sbpDataCache, const SBPDataCacheState sbpDataCacheState);

	PXPublic void SBPDataCacheHandle(SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk);

	PXPublic void SBPDataCacheChannalCallBackRegister(SBPDataCache* const sbpDataCache, const PXInt8U channelID, const SBPDataChunkIOEvent sbpDataChunkIOEvent);
	PXPublic void SBPDataCacheChannalCallBackUnregister(SBPDataCache* const sbpDataCache, const PXInt8U channelID);

	// Cache data if needed and trigger handling event of possible.
	PXPublic SBPDataChunkResult SBPDataCacheAppend(SBPDataCache* const sbpDataCache, const void* const data, const PXSize dataSize);
	
#ifdef __cplusplus
}
#endif

#endif