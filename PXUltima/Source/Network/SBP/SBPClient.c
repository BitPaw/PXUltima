#include "SBPClient.h"

#include <Memory/Memory.h>
#include <Text/Text.h>
#include <OS/User.h>
#include <Time/Time.h>

#define TimeOutLimit 5000u // 5s

/*

void SBPClientConstruct(SBPClient* const sbpClient)
{
	sbpClient->SubConnectionList = 0;
	sbpClient->SubConnectionListSize = 0;

	sbpClient->_client.EventCallBackSocket = 0;
}

SBPResult SBPClientSendAndWaitResponse
(
	SBPClient* const sbpClient,
	void* inputData,
	const size_t inputDataSize,
	void* responseData,
	size_t* responseDataSize,
	const unsigned int sourceID,
	const unsigned int targetID,
	const SBPDataPackage* dataPackage
)
{
	responseData = 0;
	responseDataSize = 0;

	const ResponseID responseID = _responseCache.Register();

	const size_t writtenBytes = SBPDataPackageSerialize(inputData, inputDataSize, SourceMe, TargetServer, dataPackage, responseID);

	// Send stuff
	{
		const ActionResult sendResult = _client.Send(inputData, writtenBytes);
		const unsigned char sucessful = sendResult == ActionSuccessful;

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

			const size_t millisecondsDelta = TimeMillisecondsDelta(&timestampStart, &timestampCurrent);

			// Check if timeout
			{			

				const unsigned char isTimeout = millisecondsDelta > TimeOutLimit;

				if(isTimeout)
				{
#if SocketDebug
					printf("[x][SBP-Client] Package timed out!\n");
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
					printf("[!][SBP-Client] Package answered! Took %3zi\n", millisecondsDelta);
#endif

					return SBPResultPackageAnswered;
				}
			}
		}
	}

	return SBPResultInvalid;
}

void SBPClientConnectToServer(SBPClient* const sbpClient, const char* ip, const unsigned short port)
{
	const ActionResult connectResult = _client.ConnectToServer(ip, port, this, ReciveDataThread);
	const unsigned char sucessful = connectResult == ActionSuccessful;

	if(!sucessful)
	{
		return;
	}

	char inputBuffer[2048] = { 0 };
	size_t inputBufferSize = 2024;
	char outputBuffer[2048] = { 0 };
	size_t outputBufferSize = 2024;

	//StopWatch stopwatch;

	//printf("[Benchmark] Package sending...\n");

	//stopwatch.Start();

	SBPDataPackageIam dataPackageIam;

	dataPackageIam.Fill();

	const SBPResult result = SendAndWaitResponse
	(
		sbpClient,
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

void SBPClientConnectToServer(SBPClient* const sbpClient, const wchar_t* ip, const unsigned short port)
{

}

void SBPClientDisconnectFromServer(SBPClient* const sbpClient)
{
	//_client.Disconnect();
}

void SBPClientRegisterMe()
{



}

void SBPClientSendText(const char* text)
{
	
}

void SBPClientSendFile(const char* filePath)
{
	printf("[SBP-Client] Sending file <%s>...\n", filePath);

	/*
	Client clientFileSender;

	// Request new connection
	{
		char inputBuffer[2048]{ 0 };
		size_t inputBufferSize = 2024;
		char outputBuffer[2048]{ 0 };
		size_t outputBufferSize = 2024;

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
			printf("[SBP-Client] File cannot be send, no new connection allowed\n");
			return;
		}

		printf("[SBP-Client] File-Send Response parsing...\n");

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
			const SocketActionResult connectResult = clientFileSender.ConnectToServer(ip, port, &clientFileSender, Client::CommunicationFunctionAsync);
			const bool sucessful = connectResult == SocketActionResult::Successful;

			if(!sucessful)
			{
				return; // No connection
			}
		}
	}

	// Send file info
	{
		char inputBuffer[2048]{ 0 };
		size_t inputBufferSize = 2024;
		char outputBuffer[2048]{ 0 };
		size_t outputBufferSize = 2024;

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
			const ActionResult fileActionResult = file.MapToVirtualMemory(filePath, MemoryProtectionMode::ReadOnly);
			const bool isLoaded = fileActionResult == ResultSuccessful;

			if(!isLoaded)
			{
				return; // No mapping
			}
		}

		const size_t fileBufferSize = 2048u;
		char fileBuffer[fileBufferSize]{ 0 };

		do
		{
			const Byte__* start = file.CursorCurrentAdress();
			const size_t canRead = file.ReadPossibleSize();
			const size_t sendSize = canRead < fileBufferSize ? canRead : fileBufferSize;

			clientFileSender.Send(start, sendSize);

			file.CursorAdvance(sendSize);
		}
		while(file.IsAtEnd());
	}

	clientFileSender.Close();
	* /
	return;
}

void SBPClientOnSocketCreating(const IPAdressInfo& adressInfo, bool& use)
{
	printf("[?][SBP-Client] Should the socket <%zi> %s:%i be created?\n", adressInfo.SocketID, adressInfo.IP, adressInfo.Port);
}

void SBPClientOnSocketCreated(const IPAdressInfo& adressInfo, bool& use)
{
	printf("[+][SBP-Client] Created <%zi> %s:%i\n", adressInfo.SocketID, adressInfo.IP, adressInfo.Port);
}

void SBPClientOnMessageSend(IOSocketMessage socketMessage)
{
#if SocketDebug
	printf("[#][SBP-Client] Send %zi Bytes to <%i>\n", socketMessage.MessageSize, socketMessage.SocketID);
#endif
}

void SBPClientOnMessageReceive(IOSocketMessage socketMessage)
{
#if SocketDebug 
	printf("[#][SBP-Client] Receive %zi Bytes from <%i>\n", socketMessage.MessageSize, socketMessage.SocketID);
#endif
}

void SBPClientOnConnectionListening(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-Client] OnConnectionListening\n");
#endif
}

void SBPClientOnConnectionLinked(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-Client] OnConnectionLinked\n");
#endif
}

void SBPClientOnConnectionEstablished(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[i][SBP-Client] OnConnectionEstablished\n");
#endif
}

void SBPClientOnConnectionTerminated(const IPAdressInfo& adressInfo)
{
#if SocketDebug
	printf("[-][SBP-Client] OnConnectionTerminated\n");
#endif
}

ThreadResult SBPClientReciveDataThread(void* sbpClientAdress)
{
	SBPClient& client = *(SBPClient*)sbpClientAdress;

	char buffer[1024]{ 0 };
	size_t bufferSizeMax = 1024;
	size_t bufferSize = 0;

	while(client._client.IsCurrentlyUsed())
	{
		SBPData data;

		// Get raw bytes
		const auto receiveingResult = client._client.Receive(buffer, bufferSizeMax, bufferSize);

		// Convert raw bytes into data object
		const size_t parsedBytes = SBPData::PackageParse(data, buffer, bufferSize);

		if(parsedBytes)
		{
#if SocketDebug
			printf("[i][SBP-Client] SBP detected! Command:<%c%c%c%c>\n", data.CommandID.A, data.CommandID.B, data.CommandID.C, data.CommandID.D);
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