#include "SBPClient.h"

#include <Memory/PXMemory.h>
#include <Text/Text.h>
#include <OS/User.h>

#define TimeOutLimit 5000u // 5s

/*

void SBPPXClientConstruct(SBPPXClient* const sbpPXClient)
{
	sbpPXClient->SubConnectionList = 0;
	sbpPXClient->SubConnectionListSize = 0;

	sbpPXClient->_client.EventCallBackSocket = 0;
}

SBPResult SBPPXClientSendAndWaitResponse
(
	SBPPXClient* const sbpPXClient,
	void* inputData,
	const PXSize inputDataSize,
	void* responseData,
	PXSize* responseDataSize,
	const unsigned int sourceID,
	const unsigned int targetID,
	const SBPDataPackage* dataPackage
)
{
	responseData = 0;
	responseDataSize = 0;

	const ResponseID responseID = _responseCache.Register();

	const PXSize writtenBytes = SBPDataPackageSerialize(inputData, inputDataSize, SourceMe, TargetServer, dataPackage, responseID);

	// Send stuff
	{
		const PXActionResult sendResult = _client.Send(inputData, writtenBytes);
		const unsigned char sucessful = sendResult == PXActionSuccessful;

		if(!sucessful)
		{
			_responseCache.Remove(responseID);

			return SBPResultPackageSendingFailure;
		}
	}

	{
		Time timestampStart;
		Time timestampCurrent;

		{
			ResponseCacheEntry responseCacheEntry;
			const ResponseCacheResult responseCacheResult = _responseCache.Find(responseID, responseCacheEntry);
			const unsigned char isRegistered = responseCacheResult != ResponseCacheResultNotRegistered;

			if(!isRegistered)
			{
				return SBPResultPackageSendSucessfulButNoResponder;
			}
		}

		TimeNow(&timestampStart);

		while(1u) // Wait aslong as response is there
		{
			TimeNow(&timestampCurrent);

			const PXSize millisecondsDelta = TimeMillisecondsDelta(&timestampStart, &timestampCurrent);

			// Check if timeout
			{			

				const unsigned char isTimeout = millisecondsDelta > TimeOutLimit;

				if(isTimeout)
				{
#if SocketDebug
					printf("[x][SBP-PXClient] Package timed out!\n");
#endif

					_responseCache.Remove(responseID); // We assume that the message does not come anymore or is not needed

					return SBPResultPackageTimeout;
				}
			}

			// Check if answered
			{
				ResponseCacheEntry responseCacheEntry;
				const ResponseCacheResult responseCacheResult = _responseCache.Find(responseID, responseCacheEntry);
				const unsigned char isAnswered = responseCacheResult == ResponseCacheResult::Answered;

				if(isAnswered)
				{
					responseData = responseCacheEntry.Data;
					responseDataSize = responseCacheEntry.Length;

#if SocketDebug
					printf("[!][SBP-PXClient] Package answered! Took %3zi\n", millisecondsDelta);
#endif

					return SBPResultPackageAnswered;
				}
			}
		}
	}

	return SBPResultInvalid;
}

void SBPPXClientConnectToServer(SBPPXClient* const sbpPXClient, const char* ip, const unsigned short port)
{
	const PXActionResult connectResult = _client.ConnectToServer(ip, port, this, ReciveDataThread);
	const unsigned char sucessful = connectResult == PXActionSuccessful;

	if(!sucessful)
	{
		return;
	}

	char inputBuffer[2048] = { 0 };
	PXSize inputBufferSize = 2024;
	char outputBuffer[2048] = { 0 };
	PXSize outputBufferSize = 2024;

	//StopWatch stopwatch;

	//printf("[Benchmark] Package sending...\n");

	//stopwatch.Start();

	SBPDataPackageIam dataPackageIam;

	dataPackageIam.Fill();

	const SBPResult result = SendAndWaitResponse
	(
		sbpPXClient,
		inputBuffer,
		inputBufferSize,
		outputBuffer,
		outputBufferSize,
		SourceMe,
		TargetServer,
		&dataPackageIam
	);

	//double x = stopwatch.Stop();

	//if(result == SBPResult::PackageAnswered)
	//{
	//	//printf("[Benchmark] Package answered! took %.2lfms", x*1000);
	//}
}

void SBPPXClientConnectToServer(SBPPXClient* const sbpPXClient, const wchar_t* ip, const unsigned short port)
{

}

void SBPPXClientDisconnectFromServer(SBPPXClient* const sbpPXClient)
{
	//_client.Disconnect();
}

void SBPPXClientRegisterMe()
{



}

void SBPPXClientSendText(const char* text)
{
	
}

void SBPPXClientSendFile(const char* filePath)
{
	printf("[SBP-PXClient] Sending file <%s>...\n", filePath);

	/*
	PXClient clientFileSender;

	// Request new connection
	{
		char inputBuffer[2048]{ 0 };
		PXSize inputBufferSize = 2024;
		char outputBuffer[2048]{ 0 };
		PXSize outputBufferSize = 2024;

		const SBPResult result = SendAndWaitResponse
		(
			inputBuffer,
			inputBufferSize,
			outputBuffer,
			outputBufferSize,7
			SourceMe,
			TargetServer,
			SBPData::PackageCreateConnectionRequest
		);
		const bool sucessful = result == SBPResult::PackageAnswered;

		if(!sucessful)
		{
			printf("[SBP-PXClient] File cannot be send, no new connection allowed\n");
			return;
		}

		printf("[SBP-PXClient] File-Send Response parsing...\n");

		// Parse
		{
			SBPData data;

			SBPData::PackageParse(data, outputBuffer, outputBufferSize);

			bool isExpected = data.CommandID.Value == SBPIDResponse;
		}
	}

	// Open new connection
	{
		// Get current connection info
		const char* ip = _client.AdressInfo.IP;
		const unsigned short port = _client.AdressInfo.Port;

		{
			const SocketPXActionResult connectResult = clientFileSender.ConnectToServer(ip, port, &clientFileSender, PXClient::CommunicationFunctionAsync);
			const bool sucessful = connectResult == SocketPXActionResult::Successful;

			if(!sucessful)
			{
				return; // No connection
			}
		}
	}

	// Send file info
	{
		char inputBuffer[2048]{ 0 };
		PXSize inputBufferSize = 2024;
		char outputBuffer[2048]{ 0 };
		PXSize outputBufferSize = 2024;

		const SBPResult result = SendAndWaitResponse
		(
			inputBuffer,
			inputBufferSize,
			outputBuffer,
			outputBufferSize,
			SourceMe,
			TargetServer,
			SBPData::PackageCreateFile
		);
		const bool sucessful = result == SBPResult::PackageAnswered;
	}

	// Map file
	{
		File file;	

		{
			const PXActionResult filePXActionResult = file.MapToVirtualMemory(filePath, MemoryProtectionMode::ReadOnly);
			const bool isLoaded = filePXActionResult == ResultSuccessful;

			if(!isLoaded)
			{
				return; // No mapping
			}
		}

		const PXSize fileBufferSize = 2048u;
		char fileBuffer[fileBufferSize]{ 0 };

		do
		{
			const Byte__* start = file.CursorCurrentAdress();
			const PXSize canRead = file.ReadPossibleSize();
			const PXSize sendSize = canRead < fileBufferSize ? canRead : fileBufferSize;

			clientFileSender.Send(start, sendSize);

			file.CursorAdvance(sendSize);
		}
		while(file.IsAtEnd());
	}

	clientFileSender.Close();
	* /
	return;
}

void SBPPXClientOnSocketCreating(const IPAdressInfo& adressInfo, bool& use)
{
	printf("[?][SBP-PXClient] Should the socket <%zi> %s:%i be created?\n", adressInfo.SocketID, adressInfo.IP, adressInfo.Port);
}

void SBPPXClientOnSocketCreated(const IPAdressInfo& adressInfo, bool& use)
{
	printf("[+][SBP-PXClient] Created <%zi> %s:%i\n", adressInfo.SocketID, adressInfo.IP, adressInfo.Port);
}

void SBPPXClientOnMessageSend(IOSocketMessage socketMessage)
{
#if SocketDebug
	printf("[#][SBP-PXClient] Send %zi Bytes to <%i>\n", socketMessage.MessageSize, socketMessage.SocketID);
#endif
}

void SBPPXClientOnMessageReceive(IOSocketMessage socketMessage)
{
#if SocketDebug 
	printf("[#][SBP-PXClient] Receive %zi Bytes from <%i>\n", socketMessage.MessageSize, socketMessage.SocketID);
#endif
}

void SBPPXClientOnConnectionListening(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-PXClient] OnConnectionListening\n");
#endif
}

void SBPPXClientOnConnectionLinked(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-PXClient] OnConnectionLinked\n");
#endif
}

void SBPPXClientOnConnectionEstablished(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-PXClient] OnConnectionEstablished\n");
#endif
}

void SBPPXClientOnConnectionTerminated(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[-][SBP-PXClient] OnConnectionTerminated\n");
#endif
}

ThreadResult SBPPXClientReciveDataThread(void* sbpPXClientAdress)
{
	SBPPXClient& client = *(SBPPXClient*)sbpPXClientAdress;

	char buffer[1024]{ 0 };
	PXSize bufferSizeMax = 1024;
	PXSize bufferSize = 0;

	while(client._client.IsCurrentlyUsed())
	{
		SBPData data;

		// Get raw bytes
		const auto receiveingResult = client._client.Receive(buffer, bufferSizeMax, bufferSize);

		// Convert raw bytes into data object
		const PXSize parsedBytes = SBPData::PackageParse(data, buffer, bufferSize);

		if(parsedBytes)
		{
#if SocketDebug
			printf("[i][SBP-PXClient] SBP detected! Command:<%c%c%c%c>\n", data.CommandID.A, data.CommandID.B, data.CommandID.C, data.CommandID.D);
#endif
			client._responseCache.Fill(data.ID, buffer, bufferSize);

			//_responseCache.;

			// Handle packaage
			switch(data.CommandID.Value)
			{
				case SBPDataPackageIamID:
				{
					wchar_t* name = (wchar_t*)data.Data;

					//printf("[SBP][I'am] %ls\n", name);

					//InvokeEvent(PackageIAMRecieveCallBack, name);

					break;
				}				
				case SBPDataPackageResponseID:
				{


					break;
				}
				case SBPDataPackageFileID:
				{

					break;
				}

				/*
				case SBPCommand::ConnectionCreate:
				{
					break;
				}
				case SBPCommand::ConnectionInfo:
				{
					break;
				}
				case SBPCommand::ConnectionQuit:
				{
					break;
				}
				case SBPCommand::Text:
				{
					break;
				}
				case SBPCommand::File:
				{
					break;
				}* /
				default:
				{
					//PackageRecieveCallBack(data);
					//return SBPResult::PackageDetectedCustom;
					break;
				}
			}
		}

		//return SBPResult::PackageDetectedRegistered;


		//if(!sucessful)
		//{
			// client.Disconnect();
		//}
	}

	return ThreadFunctionReturnValue;
}

*/

void PXSBPClientConstruct(PXSBPClient* const sbpPXClient)
{
	PXSBPPackageProcessorConstruct(&sbpPXClient->PackageProcessor);

	//---<Server construction>-------------------------------------------------
	{
		PXClient* const client = &sbpPXClient->Client;

		PXClientConstruct(client);

		client->Owner = sbpPXClient;
		client->EventListener.MessageSendCallback = PXSBPPackageProcessorOnDataRawSend;
		client->EventListener.MessageReceiveCallback = PXSBPPackageProcessorOnDataRawReceive;
	}
	//-------------------------------------------------------------------------
}

void PXSBPClientDestruct(PXSBPClient* const sbpPXClient)
{
	PXSBPPackageProcessorDestruct(&sbpPXClient->PackageProcessor);
	PXClientDestruct(&sbpPXClient->Client);
}

PXActionResult SBPPXClientConnectToServer(PXSBPClient* const sbpPXClient, const char* ip, const unsigned short port)
{
	// Connect
	{
		const PXActionResult connectResult = PXClientConnectToServer(&sbpPXClient->Client, ip, port, &sbpPXClient->Client, CommunicationFunctionAsync);

		PXActionExitOnError(connectResult);
	}

	// After connection, who are you?
	{
		SBPDataPackage sbpDataPackage;
		SBPDataPackageIam sbpDataPackageIam;

		char buffer[256];
		PXSize bufferSize = 0;

		sbpDataPackage.Data = buffer;
		sbpDataPackage.DataSizeTotal = 256;

		bufferSize = SBPDataPackageIamSerialize(&sbpDataPackage, &sbpDataPackageIam);

		PXSBPPackageProcessorPackageExport(&sbpPXClient->PackageProcessor, &sbpDataPackage);
	}


	return PXActionSuccessful;
}

PXActionResult SBPPXClientDisconnectFromServer(PXSBPClient* const sbpPXClient)
{
	return PXClientDisconnectFromServer(&sbpPXClient->Client);
}