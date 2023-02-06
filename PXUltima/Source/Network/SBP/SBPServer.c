#include "SBPServer.h"

#include <stdio.h>

#include <File/PXDataStream.h>
#include <Text/Text.h>

void OnSBPServerDataRawSend(const PXSocket* const pxSocket, const void* message, const PXSize messageSize)
{
	
}

void OnSBPServerDataRawReceive(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize)
{
	SBPServer* const server = (SBPServer* const)pxSocket->Owner;

	const SBPDataChunkResult dataResult = SBPDataCacheAppend(&server->DataCache, message, messageSize);

	switch (dataResult)
	{
		default:
			break;
	}
}

void OnSBPServerDataChunkRecived(SBPServer* const sbpServer, SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk)
{
	// Lookup who the reciever of this chunk data is.
	const PXBool found = PXDictionaryFind(&sbpServer->ChannalEntryLookup, sbpDataChunk->Channal, 0);

	if (!found)
	{
		// cant handle package, there is no reviever channel
		return;
	}

	switch (sbpDataChunk->Order)
	{
		case SBPDataCacheChunkOrderClose:
		case SBPDataCacheChunkOrderFirst:
		case SBPDataCacheChunkOrderMiddle:
		case SBPDataCacheChunkOrderFinal:
		{
			break;
		}
		default:
			break;
	}



	// if we dont have one, ignore package and drop it.

	// 
}

void OnSBPServerChannalCreated(SBPServer* const sbpServer, SBPDataCache* const sbpDataCache, const PXInt8U channalID)
{
	
}

void SBPServerConstruct(SBPServer* const sbpServer)
{
	//---<Server construction>-------------------------------------------------
	{
		PXServer* const server = &sbpServer->PXServer;

		PXServerConstruct(server);

		server->Owner = sbpServer;
		server->SocketEventListener.MessageSendCallback = OnSBPServerDataRawSend;
		server->SocketEventListener.MessageReceiveCallback = OnSBPServerDataRawReceive;	
	}
	//-------------------------------------------------------------------------

	//---<Data cache construction>---------------------------------------------
	{
		SBPDataCache* const dataCache = &sbpServer->DataCache;

		SBPDataCacheConstruct(dataCache);

		dataCache->Owmer = sbpServer;
		dataCache->ChannalCreatedCallBack = OnSBPServerChannalCreated;
		dataCache->DataChunkRecievedCallBack = OnSBPServerDataChunkRecived;
	}
	//-------------------------------------------------------------------------
}

void SBPServerDestruct(SBPServer* const sbpServer)
{

}

PXActionResult SBPServerStart(SBPServer* const sbpServer, const unsigned short port)
{
	PXServer* server = &sbpServer->PXServer;

	const PXActionResult result = PXServerStart(server, port, ProtocolModeTCP);

	return result;
}

PXActionResult SBPServerStop(SBPServer* const sbpServer)
{
	return PXServerStop(&sbpServer->PXServer);
}

PXActionResult SBPServerSendFileA(SBPServer* const sbpServer, const PXSocketID clientID, const char* text)
{
	return PXActionInvalid;
}


/*
void SBPServerSendFile(const PXClientID clientID, const char* text)
{
	File file;
	/*

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

		SBPData data
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

		//SBPDataGenerateMessage(data, buffer, bufferSize, bufferSizeMax);




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
	}	* /
}

PXSize randomIDGenerator = 420;

const ResponseID SBPServerGenerateResponseID()
{
	return randomIDGenerator++;
}

bool SBPServerSendMessageWaitResponse(const PXClientID clientID, const ResponseID responseID, const Byte__* buffer, const PXSize& bufferSize)
{
	_responseLookup.Add(responseID, nullptr);



	//_server.SendMessageToPXClient(clientID, buffer, bufferSize);

	/*
	// wait for message
	while(true)
	{
		_responseLookup.
	}* /


	return false;
}

void SBPServerSendTextToAll(const char* text)
{
	const PXSize bufferSize = 2048;
	PXSize size = 0;
	Byte__ buffer[bufferSize]{ 0 };

	//CreateText(text, buffer, size, bufferSize);

	//_server.SendMessageToAll(buffer, size);
}

void SBPServerSendTextToAll(const wchar_t* text)
{
	const PXSize bufferSize = 2048;
	PXSize size = 0;
	Byte__ buffer[bufferSize]{ 0 };
	
	//CreateText(text, buffer, size, bufferSize);

	//_server.SendMessageToAll(buffer, size);
}

void SBPServerSendTextToPXClient(const unsigned int clientID, const char* text)
{


	//CreateText(text, buffer, size, bufferSize);

	//_server.SendMessageToPXClient(clientID, buffer, size);
}

void SBPServerSendTextToPXClient(const unsigned int clientID, const wchar_t* text)
{
	const PXSize bufferSize = 2048;
	PXSize size = 0;
	Byte__ buffer[bufferSize]{ 0 };

	//CreateText(text, buffer, size, bufferSize);

	//_server.SendMessageToPXClient(clientID, buffer, size);
}

void SBPServerOnSocketCreating(const IPAdressInfo& adressInfo, bool& use)
{
#if SocketDebug
	printf("[i][SBP-Server] OnSocketCreating\n");
#endif
}

void SBPServerOnSocketCreated(const IPAdressInfo& adressInfo, bool& use)
{
#if SocketDebug
	printf("[i][SBP-Server] OnSocketCreated\n");
#endif
}

void SBPServerOnMessageSend(IOSocketMessage socketMessage)
{
#if SocketDebug
	printf("[i][SBP-Server] OnMessageSend\n");
#endif
}

void SBPServerOnMessageReceive(IOSocketMessage socketMessage)
{
#if SocketDebug
	printf("[i][SBP-Server] Message from <%i> %zi Bytes\n",socketMessage.SocketID, socketMessage.MessageSize);
#endif

	SBPData data;
	const PXSize read = SBPDataPackageParse(data, socketMessage.Message, socketMessage.MessageSize);

	if(read)
	{
#if SocketDebug
		printf("[i][SBP-Server] SBP Pachage detected Command:%c%c%c%c\n", data.CommandID.A, data.CommandID.B, data.CommandID.C, data.CommandID.D);
#endif
		//data.Print();

		switch(data.CommandID.Value)
		{	
			case SBPDataPackageIamID:
			{
				const PXSize bufferSize = 1024u;
				PXSize bufferActural = 0;
				Byte__ buffer[bufferSize]{0};			

				SBPDataPackageResponse packageResponse;

				packageResponse.Type = SBPDataPackageResponseTypeOK;

				const PXSize written = SBPDataPackageSerialize(buffer, bufferSize, SourceMe, TargetYou, &packageResponse, data.ID);

#if SocketDebug
				printf("[i][SBP-Server] Sending response\n");
#endif
				_server.SendMessageToPXClient(socketMessage.SocketID, buffer, written);			

				break;
			}				
			/*case SBPCommandConnectionCreate:
				break;
			case SBPCommandConnectionInfo:
				break;
			case SBPCommandConnectionQuit:
				break;
			case SBPCommandText:
				break;
			case SBPCommandFile:
				break;* /

			default: // Custom or unkownPackage
				break;
		}
	}
}

void SBPServerOnConnectionListening(const IPAdressInfo& adressInfo)
{
	printf("[i][SBP-Server][%zi] Listening on IP:%s Port:%i\n", adressInfo.SocketID, adressInfo.IP, adressInfo.Port);
}

void SBPServerOnConnectionLinked(const IPAdressInfo& adressInfo)
{
	printf("[i][SBP-Server] Linked \n");
}

void SBPServerOnConnectionEstablished(const IPAdressInfo& adressInfo)
{
	printf("[i][SBP-Server]Established <%zi>\n", adressInfo.SocketID);
}

void SBPServerOnConnectionTerminated(const IPAdressInfo& adressInfo)
{
	printf("[-][SBP-Server] Terminated <%zi>\n", adressInfo.SocketID);
}*/