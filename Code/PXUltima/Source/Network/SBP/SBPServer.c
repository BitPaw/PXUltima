#include "SBPServer.h"

#include <stdio.h>

#include <File/PXDataStream.h>
#include <Media/PXText.h>

void SBPServerConstruct(PXSBPServer* const sbpServer)
{
	PXSBPPackageProcessorConstruct(&sbpServer->PackageProcessor);
	//&sbpServer->PackageProcessor.Owner = sbpServer->Server.


	//---<Server construction>-------------------------------------------------
	{
		PXServer* const server = &sbpServer->Server;

		PXServerConstruct(server);

		server->Owner = sbpServer;
		server->EventListener.MessageSendCallback = PXSBPPackageProcessorOnDataRawSend;
		server->EventListener.MessageReceiveCallback = PXSBPPackageProcessorOnDataRawReceive;
	}
	//-------------------------------------------------------------------------
}

void SBPServerDestruct(PXSBPServer* const sbpServer)
{
	PXSBPPackageProcessorDestruct(&sbpServer->PackageProcessor);
	PXServerDestruct(&sbpServer->Server);
}

PXActionResult SBPServerStart(PXSBPServer* const sbpServer, const unsigned short port)
{
	PXServer* server = &sbpServer->Server;

	const PXActionResult result = PXServerStart(server, port, ProtocolModeTCP);

	SBPPackageHeaderCacheStateChange(&sbpServer->PackageProcessor.DataCache, SBPPackageHeaderCacheStateAwaitHeaderStart);

	return result;
}

PXActionResult SBPServerStop(PXSBPServer* const sbpServer)
{
	return PXServerStop(&sbpServer->Server);
}

PXActionResult SBPServerSendFileA(PXSBPServer* const sbpServer, const PXSocketID clientID, const char* text)
{

	/*
	* 	File file;

	// Check if file exists
	{
		const PXActionResult filePXActionResult = file.MapToVirtualMemory(text, MemoryReadOnly);
		const bool sucessful = filePXActionResult == ResultSuccessful;

		if(!sucessful)
		{
			return; // Dont have file
		}
	}

	// Ask for new data connection
	{
		const char id = ConnectionCreateReasonFile;

		SBPPackageHeader data
		(
			SBPIDConnectionCreate,
			SourceMe,
			clientID,
			-1,
			1u,
			&id
		);

		const PXSize bufferSizeMax = 2048;
		PXSize bufferSize = 0;
		Byte__ buffer[bufferSizeMax];*/

		//SBPPackageHeaderGenerateMessage(data, buffer, bufferSize, bufferSizeMax);




		//_server.SendMessageToPXClient(clientID, buffer, bufferSize);
	//}

	// Create "i want to send" package



	//SBPIDConnectionCreate

	// Send pakgage

	// Wait for response


	/*
	for(PXSize i = 0; i < _server.NumberOfConnectedPXClients; i++)
	{
		auto& client = _server.PXClientList[i];

		client.SendFile(text);
	}
	*/


	return PXActionInvalid;
}