#include "SBPServer.h"

#include <stdio.h>

#include <File/DataStream.h>
#include <Text/Text.h>

/*

void SBPServerStart(const unsigned short port)
{
	_server.EventCallBackSocket = this;

	_server.Start(port);
}

void SBPServerStop()
{
	_server.Stop();
}

void SBPServerSendFile(const PXClientID clientID, const char* text)
{
	File file;
	/*

	// Check if file exists
	{
		const ActionResult fileActionResult = file.MapToVirtualMemory(text, MemoryReadOnly);
		const bool sucessful = fileActionResult == ResultSuccessful;

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