#ifndef SBPClientINCLUDE
#define SBPClientINCLUDE

#include <stddef.h>

#include "SBPProtocol.h"

#include <Network/Client.h>
#include <Async/Thread.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct SBPClient_ SBPClient;

	typedef struct SBPClient_
	{
		// private
		//ResponseCache _responseCache;
		Client _client;		

		// public
		wchar_t Name[256];

		SBPClient* SubConnectionList;
		size_t SubConnectionListSize;
	}
	SBPClient;

	ThreadResult SBPClientReciveDataThread(void* sbpClient);

	extern void SBPClientConstruct(SBPClient* const sbpClient);
	extern void SBPClientDestruct(SBPClient* const sbpClient);

	// Sending a message via a socket, await a response.
	extern SBPResult SendAndWaitResponse
	(
		SBPClient* const sbpClient,
		void* inputData,
		const size_t inputDataSize,
		void* responseData,
		size_t* responseDataSize,

		const unsigned int sourceID,
		const unsigned int targetID,
		const SBPDataPackage* dataPackage
	);



	void SBPClientConnectToServer(SBPClient* const sbpClient, const char* ip, const unsigned short port);
	void SBPClientConnectToServer(SBPClient* const sbpClient, const wchar_t* ip, const unsigned short port);
	void SBPClientDisconnectFromServer(SBPClient* const sbpClient);

	void SBPClientRegisterMe();
	void SBPClientSendText(const char* text);
	void SBPClientSendFile(const char* filePath);

#ifdef __cplusplus
}
#endif

#endif