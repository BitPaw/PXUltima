#include "PXSBPPackageProcessor.h"

void PXSBPPackageProcessorOnDataRawSend(const PXSocket* const pxSocket, const void* message, const PXSize messageSize)
{

}

void PXSBPPackageProcessorOnDataRawReceive(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
	const SBPPackageHeaderChunkResult dataResult = SBPPackageHeaderCacheAppend(pxSocket->Owner, message, messageSize);

	switch (dataResult)
	{
		default:
			break;
	}
}

void PXSBPPackageProcessorOnChunkRecived(PXSBPPackageProcessor* const pxSBPPackageProcessor, SBPPackageHeaderCache* const sbpDataCache, const SBPPackageHeaderChunk* const sbpDataChunk)
{
	// Lookup who the reciever of this chunk data is.
	SBPPackageHeader sbpDataPackage;

	PXDictionary* const channalEntryLookup = &pxSBPPackageProcessor->ChannalEntryLookup;

	const PXBool found = PXDictionaryFind(channalEntryLookup, sbpDataChunk->Channal, &sbpDataPackage);

	if (!found)
	{
		// It is probably a new Package. So we try to parse the SBP header

		const PXSize size = 0;//PXSBPPackageParse(&sbpDataPackage, sbpDataChunk->Data, sbpDataChunk->DataSizeCurrent);
		const PXSize successful = size > 0;

		if (!successful)
		{
			return; // Drop package, illegal formatting and not expected.
		}

		{
			const PXBool isFullyConsumable = 0;// PXSBPPackageIsConsumable(&sbpDataPackage);

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
		const PXBool isConsumable = 0;// PXSBPPackageIsConsumable(&sbpDataPackage);

		// Copy anyway, to make it whole.

		// UpdatePackage()

		// Handle Package
		//SBPServerPackageRecivedHandle(sbpServer, &sbpDataPackage);

		// Package was handled. Now we can unregister it
		PXDictionaryRemove(channalEntryLookup, sbpDataChunk->Channal);
	}
}

void PXSBPPackageProcessorOnPackageRecived(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPPackageHeader* const sbpDataPackage)
{

}

void PXSBPPackageProcessorPackageHandle(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPPackageHeader* const sbpDataPackage)
{
	/*
	switch (sbpDataPackage->CommandID.Value)
	{
		case SBPPackageHeaderPackageIamID:
		{
			const PXSize bufferSize = 1024u;
			PXSize bufferActural = 0;
			PXByte buffer[1024] = { 0 };

			SBPPackageHeaderPackageResponse packageResponse;

			packageResponse.Type = SBPPackageHeaderPackageResponseTypeOK;

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

void PXSBPPackageProcessorPackageExport(PXSBPPackageProcessor* const pxSBPPackageProcessor, const SBPPackageHeader* const sbpDataPackage)
{
	PXSocket* const socket = (PXSocket* const)pxSBPPackageProcessor->Owner;

	char buffer[1024];

	PXSize packageSize = 0;//PXSBPPackageSerialize(sbpDataPackage, buffer, 1024);

	PXSocketSend(socket, buffer, packageSize, 0);
}

void PXSBPPackageProcessorConstruct(PXSBPPackageProcessor* const pxSBPPackageProcessor)
{
	//---<Data cache construction>---------------------------------------------
	{
		SBPPackageHeaderCache* const dataCache = &pxSBPPackageProcessor->DataCache;

		SBPPackageHeaderCacheConstruct(dataCache);

		dataCache->Owmer = pxSBPPackageProcessor;
		//dataCache->ChannalCreatedCallBack = OnSBPServerChannalCreated;
		dataCache->DataChunkRecievedCallBack = PXSBPPackageProcessorOnChunkRecived;
	}
	//-------------------------------------------------------------------------

	PXDictionaryConstruct(&pxSBPPackageProcessor->ChannalEntryLookup, sizeof(PXInt8U), sizeof(SBPPackageHeader));

	pxSBPPackageProcessor->Owner = PXNull;
}

void PXSBPPackageProcessorDestruct(PXSBPPackageProcessor* const pxSBPPackageProcessor)
{
	PXDataStreamDestruct(&pxSBPPackageProcessor->DataCache);
}
