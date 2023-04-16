#include "SBPDataChunk.h"

#include <OS/Memory/PXMemory.h>
#include <Network/SBP/SBPProtocol.h>

void SBPPackageHeaderCacheConstruct(SBPPackageHeaderCache* const sbpDataCache)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	MemoryClear(sbpDataCache, sizeof(SBPPackageHeaderCache));

	PXDictionaryConstruct(pxDictionary, sizeof(PXInt8U), sizeof(SBPPackageHeaderChunkIOEvent), PXDictionaryValueLocalityInternalEmbedded);
}

void SBPPackageHeaderCacheDestruct(SBPPackageHeaderCache* const sbpDataCache)
{

}

void SBPPackageHeaderCacheStateChange(SBPPackageHeaderCache* const sbpDataCache, const SBPPackageHeaderCacheState sbpDataCacheState)
{
	sbpDataCache->State = sbpDataCacheState;
}

void SBPPackageHeaderCacheHandle(SBPPackageHeaderCache* const sbpDataCache, const SBPPackageHeaderChunk* const sbpDataChunk)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	sbpDataCache->DataChunkRecievedCallBack(sbpDataCache->Owmer, sbpDataCache, sbpDataChunk);

#if 0
	// Lookup the callback function for the spesific channal
	SBPPackageHeaderChunkIOEvent sbpDataChunkIOEvent;

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

void SBPPackageHeaderCacheChannalCallBackRegister(SBPPackageHeaderCache* const sbpDataCache, const PXInt8U channelID, const SBPPackageHeaderChunkIOEvent sbpDataChunkIOEvent)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	PXDictionaryAdd(pxDictionary, &channelID, &sbpDataChunkIOEvent);
}

void SBPPackageHeaderCacheChannalCallBackUnregister(SBPPackageHeaderCache* const sbpDataCache, const PXInt8U channelID)
{
	PXDictionary* const pxDictionary = &sbpDataCache->DataCallBackLookup;

	PXDictionaryRemove(pxDictionary, &channelID);
}

#define SBPPackageHeaderCacheHeaderSize 3u

SBPPackageHeaderChunkResult SBPPackageHeaderCacheAppend(SBPPackageHeaderCache* const sbpDataCache, const void* const data, const PXSize dataSize)
{
	PXDataStream dataStream;

	PXDataStreamFromExternal(&dataStream, data, dataSize);

	switch (sbpDataCache->State) // Check State
	{
		case SBPPackageHeaderCacheStateAwaitHeaderStart:
		{
			const PXBool isLongEnough = dataSize >= 2u;

			if (!isLongEnough)
			{
				SBPPackageHeaderCacheStateChange(sbpDataCache, SBPPackageHeaderCacheStateAwaitHeaderRemainingData);
				return SBPPackageHeaderChunkCached;
			}




			SBPPackageHeader sbpDataPackage;

			const PXSize writtenBytes = 0;//PXSBPPackageParse(&sbpDataPackage, data, dataSize);
			const PXBool sucessful = writtenBytes > 0;

			if (!sucessful)
			{
				// Illegal Package
			}



			const PXByte* const header = (const PXByte* const)data;
			const PXSize dataContentSize = dataSize - SBPPackageHeaderCacheHeaderSize;
			const PXInt16U sizeCurrent = PXInt16MakeEndianLittle(header[1], header[2]);
			const PXInt16U sizeTotal = header + SBPPackageHeaderCacheHeaderSize;
			const PXBool isComplete = sizeCurrent >= sizeTotal; // Is the package fully
			//const PXInt8U percentageWhole = (dataContentSize / (float)size) * 100;
			const SBPPackageHeaderCacheChunkOrder order = isComplete ? SBPPackageHeaderCacheChunkOrderFirst : SBPPackageHeaderCacheChunkOrderFinal;
			const SBPPackageHeaderChunk sbpDataChunk =
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
				SBPPackageHeaderCacheHandle(sbpDataCache, &sbpDataChunk);

				return SBPPackageHeaderChunkComsumeImmediately;
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

			SBPPackageHeaderCacheStateChange(sbpDataCache, SBPPackageHeaderCacheStateAwaitData);

			return SBPPackageHeaderChunkCached;
		}
		case SBPPackageHeaderCacheStateAwaitHeaderRemainingData:
		{


			break;
		}
		case SBPPackageHeaderCacheStateAwaitData:
		{
			PXDataStream* const dataStream = &sbpDataCache->Data;
			SBPPackageHeaderChunk sbpDataChunk = {0,0,0,0,0};
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
				SBPPackageHeaderCacheStateChange(sbpDataCache, SBPPackageHeaderCacheStateAwaitHeaderStart);
				//-------

				// handle now
				SBPPackageHeaderCacheHandle(sbpDataCache, &sbpDataChunk);
				//-------

				return SBPPackageHeaderChunkComsumeImmediately;
			}

			return SBPPackageHeaderChunkCached;
		}
		default:
			return SBPPackageHeaderChunkResultInvalid;
	}

	return SBPPackageHeaderChunkResultInvalid;
}
