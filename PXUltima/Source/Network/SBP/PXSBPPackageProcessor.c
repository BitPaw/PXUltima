#include "PXSBPPackageProcessor.h"

void PXSBPPackageProcessorOnDataRawSend(const PXSocket* const pxSocket, const void* message, const PXSize messageSize)
{

}

void PXSBPPackageProcessorOnDataRawReceive(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
	const SBPDataChunkResult dataResult = SBPDataCacheAppend(pxSocket->Owner, message, messageSize);

	switch (dataResult)
	{
		default:
			break;
	}
}

void PXSBPPackageProcessorOnChunkRecived(PXSBPPackageProcessor* const pxSBPPackageProcessor, SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk)
{
	// Lookup who the reciever of this chunk data is.
	SBPDataPackage sbpDataPackage;

	PXDictionary* const channalEntryLookup = &pxSBPPackageProcessor->ChannalEntryLookup;

	const PXBool found = PXDictionaryFind(channalEntryLookup, sbpDataChunk->Channal, &sbpDataPackage);

	if (!found)
	{
		// It is probably a new Package. So we try to parse the SBP header

		const PXSize size = PXSBPPackageParse(&sbpDataPackage, sbpDataChunk->Data, sbpDataChunk->DataSizeCurrent);
		const PXSize successful = size > 0;

		if (!successful)
		{
			return; // Drop package, illegal formatting and not expected.
		}

		{
			const PXBool isFullyConsumable = PXSBPPackageIsConsumable(&sbpDataPackage);

			if (isFullyConsumable)
			{
				PXSBPPackageProcessorPackageHandle(pxSBPPackageProcessor, &sbpDataPackage);
				return; // Package is probably smal and caN be consumed at once.
			}
		}

		// Register data, to access later
		PXDictionaryAdd(channalEntryLookup, sbpDataChunk->Channal, &sbpDataPackage);

		// Package is big and not fully recieved at this time. Cache current package and wait for additional chunks.

		// Store()

		// trigger Event progress?

		return; // New package handled
	}
	else
	{
		// Package is already registered

		// Is the packe now compleate?
		const PXBool isConsumable = PXSBPPackageIsConsumable(&sbpDataPackage);

		// Copy anyway, to make it whole.

		// UpdatePackage()

		// Handle Package
		//SBPServerPackageRecivedHandle(sbpServer, &sbpDataPackage);

		// Package was handled. Now we can unregister it
		PXDictionaryRemove(channalEntryLookup, sbpDataChunk->Channal);
	}
}

void PXSBPPackageProcessorOnPackageRecived(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPDataPackage* const sbpDataPackage)
{

}

void PXSBPPackageProcessorPackageHandle(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPDataPackage* const sbpDataPackage)
{
	/*
	switch (sbpDataPackage->CommandID.Value)
	{
		case SBPDataPackageIamID:
		{
			const PXSize bufferSize = 1024u;
			PXSize bufferActural = 0;
			PXByte buffer[1024] = { 0 };

			SBPDataPackageResponse packageResponse;

			packageResponse.Type = SBPDataPackageResponseTypeOK;

			const PXSize written = PXSBPPackageSerialize(buffer, bufferSize, SourceMe, TargetYou, &packageResponse, data.ID);

#if SocketDebug
			printf("[i][SBP-Server] Sending response\n");
#endif

			SBPServerSendTextToPXClient(socketMessage.SocketID, buffer, written);

			break;
		}
		case SBPCommandConnectionCreate:
			break;
		case SBPCommandConnectionInfo:
			break;
		case SBPCommandConnectionQuit:
			break;
		case SBPCommandText:
			break;
		case SBPCommandFile:
			break;

		default: // Custom or unkownPackage
			break;
	}
	*/
}

void PXSBPPackageProcessorPackageExport(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPDataPackage* const sbpDataPackage)
{

}

void PXSBPPackageProcessorConstruct(PXSBPPackageProcessor* const pxSBPPackageProcessor)
{
	//---<Data cache construction>---------------------------------------------
	{
		SBPDataCache* const dataCache = &pxSBPPackageProcessor->DataCache;

		SBPDataCacheConstruct(dataCache);

		dataCache->Owmer = pxSBPPackageProcessor;
		//dataCache->ChannalCreatedCallBack = OnSBPServerChannalCreated;
		dataCache->DataChunkRecievedCallBack = PXSBPPackageProcessorOnChunkRecived;
	}
	//-------------------------------------------------------------------------
}

void PXSBPPackageProcessorDestruct(PXSBPPackageProcessor* const pxSBPPackageProcessor)
{
	PXDataStreamDestruct(&pxSBPPackageProcessor->DataCache);
}
