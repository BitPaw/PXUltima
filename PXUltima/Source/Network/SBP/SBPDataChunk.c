#include "SBPDataChunk.h"

#include <Container/ClusterValue.h>
#include <Memory/PXMemory.h>

void SBPDataCacheConstruct(SBPDataCache* const sbpDataCache)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	MemoryClear(sbpDataCache, sizeof(SBPDataCache));

	PXDictionaryConstruct(pxDictionary, sizeof(PXInt8U), sizeof(SBPDataChunkIOEvent));
}

void SBPDataCacheDestruct(SBPDataCache* const sbpDataCache)
{
	
}

void SBPDataCacheStateChange(SBPDataCache* const sbpDataCache, const SBPDataCacheState sbpDataCacheState)
{
	sbpDataCache->State = sbpDataCacheState;
}

void SBPDataCacheHandle(SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;
		
	sbpDataCache->DataChunkRecievedCallBack(sbpDataCache->Owmer, sbpDataCache, sbpDataChunk);

#if 0
	// Lookup the callback function for the spesific channal
	SBPDataChunkIOEvent sbpDataChunkIOEvent;

	const PXBool found = PXDictionaryFind(pxDictionary, &sbpDataChunk->Channal, &sbpDataChunkIOEvent);
	
	if (!found)
	{
		// Drop package
		return;
	}

	// call
	sbpDataChunkIOEvent(sbpDataCache->Owmer, sbpDataChunk);
#endif
}

void SBPDataCacheChannalCallBackRegister(SBPDataCache* const sbpDataCache, const PXInt8U channelID, const SBPDataChunkIOEvent sbpDataChunkIOEvent)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	PXDictionaryAdd(pxDictionary, &channelID, &sbpDataChunkIOEvent);
}

void SBPDataCacheChannalCallBackUnregister(SBPDataCache* const sbpDataCache, const PXInt8U channelID)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	PXDictionaryRemove(pxDictionary, &channelID);
}

#define SBPDataCacheHeaderSize 3u

SBPDataChunkResult SBPDataCacheAppend(SBPDataCache* const sbpDataCache, const void* const data, const PXSize dataSize)
{
	switch (sbpDataCache->State) // Check State
	{
		case SBPDataCacheStateAwaitHeaderStart:
		{
			const PXBool isLongEnough = dataSize >= 2u;

			if (!isLongEnough)
			{
				SBPDataCacheStateChange(sbpDataCache, SBPDataCacheStateAwaitHeaderRemainingData);
				return SBPDataChunkCached;
			}

			const PXByte* const header = (const PXByte* const)data;
			const PXSize dataContentSize = dataSize - SBPDataCacheHeaderSize;
			const PXInt16U sizeCurrent = MakeShortLE(header[1], header[2]);
			const PXInt16U sizeTotal = header + SBPDataCacheHeaderSize;
			const PXBool isComplete = sizeCurrent >= sizeTotal; // Is the package fully
			//const PXInt8U percentageWhole = (dataContentSize / (float)size) * 100;
			const SBPDataCacheChunkOrder order = isComplete ? SBPDataCacheChunkOrderFirst : SBPDataCacheChunkOrderFinal;
			const SBPDataChunk sbpDataChunk =
			{
				order, // Info
				header[0], // Channal 
				dataContentSize,
				sizeCurrent, // Size
				sizeTotal // Data
			};

			if (isComplete)
			{
				// Trigger event, that we can consume data imidiently
				SBPDataCacheHandle(sbpDataCache, &sbpDataChunk);

				return SBPDataChunkComsumeImmediately;
			}

			// Data is not yet compleate, cache for later
			{
				PXDataStream* const dataStream = &sbpDataCache->Data;

				const PXDataStreamElementType pxDataStreamElementTypeList[4] =
				{
					PXDataTypeInt8U, &sbpDataChunk.Order,
					PXDataTypeInt8U, &sbpDataChunk.Channal,
					PXDataTypeLEInt16U, &sbpDataChunk.DataSizeCurrent,
					PXDataTypeLEInt16U, &sbpDataChunk.DataSizeTotal
				};
				const PXSize pxDataStreamElementTypeListSize = sizeof(pxDataStreamElementTypeList) / sizeof(PXDataStreamElementType);

				sbpDataCache->DataHeaderStartOffset = dataStream->DataCursor; // BackupPosition for later use

				PXDataStreamWriteMultible(dataStream, pxDataStreamElementTypeList, pxDataStreamElementTypeListSize); // Write header

				PXDataStreamWriteB(dataStream, sbpDataChunk.Data, sbpDataChunk.DataSizeCurrent); // Write Data
			}

			SBPDataCacheStateChange(sbpDataCache, SBPDataCacheStateAwaitData);		

			return SBPDataChunkCached;
		}
		case SBPDataCacheStateAwaitHeaderRemainingData:
		{


			break;
		}
		case SBPDataCacheStateAwaitData:
		{
			PXDataStream* const dataStream = &sbpDataCache->Data;
			SBPDataChunk sbpDataChunk = {0,0,0,0,0};
			const PXDataStreamElementType pxDataStreamElementTypeList[4] =
			{
				PXDataTypeInt8U, &sbpDataChunk.Order,
				PXDataTypeInt8U, &sbpDataChunk.Channal,
				PXDataTypeLEInt16U, &sbpDataChunk.DataSizeCurrent,
				PXDataTypeLEInt16U, &sbpDataChunk.DataSizeTotal
			};
			const PXSize pxDataStreamElementTypeListSize = sizeof(pxDataStreamElementTypeList) / sizeof(PXDataStreamElementType);

			const PXSize headerStart = sbpDataCache->DataHeaderStartOffset;
			const PXSize oldPosition = dataStream->DataCursor;

			PXDataStreamCursorMoveTo(dataStream, headerStart); // Go to header
			PXDataStreamReadMultible(dataStream, pxDataStreamElementTypeList, pxDataStreamElementTypeListSize); // Write header

			// Update chunk data
			sbpDataChunk.DataSizeCurrent += dataSize;

			const PXBool isNowCompleate = sbpDataChunk.DataSizeCurrent >= sbpDataChunk.DataSizeTotal;


			//---<Copy the rest and handle it>-----
			PXDataStreamWriteMultible(dataStream, pxDataStreamElementTypeList, pxDataStreamElementTypeListSize); // Write header

			PXDataStreamCursorMoveTo(dataStream, oldPosition); // Revert position

			PXDataStreamWriteB(dataStream, data, dataSize); // Write Data
			//---------------------------------------


			if (isNowCompleate)
			{
				sbpDataCache->DataHeaderStartOffset = (PXSize)-1; // We dont have a startingpoint naymore

				//-------
				SBPDataCacheStateChange(sbpDataCache, SBPDataCacheStateAwaitHeaderStart);
				//-------

				// handle now
				SBPDataCacheHandle(sbpDataCache, &sbpDataChunk);
				//-------

				return SBPDataChunkComsumeImmediately;
			}

			return SBPDataChunkCached;
		}
		default:
			return SBPDataChunkResultInvalid;
	}

	return SBPDataChunkResultInvalid;
}