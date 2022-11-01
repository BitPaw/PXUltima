#ifndef SBPPXClientINCLUDE
#define SBPPXClientINCLUDE

#include <Format/Type.h>

#include "SBPProtocol.h"

#include <Network/PXClient.h>
#include <Async/PXThread.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct SBPPXClient_ SBPPXClient;

	typedef struct SBPPXClient_
	{
		// private
		//ResponseCache _responseCache;
		PXClient _client;

		// public
		wchar_t Name[256];

		SBPPXClient* SubConnectionList;
		size_t SubConnectionListSize;
	}
	SBPPXClient;

	PXThreadResult SBPPXClientReciveDataThread(void* sbpPXClient);

	PXPublic void SBPPXClientConstruct(SBPPXClient* const sbpPXClient);
	PXPublic void SBPPXClientDestruct(SBPPXClient* const sbpPXClient);

	// Sending a message via a socket, await a response.
	PXPublic SBPResult SendAndWaitResponse
	(
		SBPPXClient* const sbpPXClient,
		void* inputData,
		const size_t inputDataSize,
		void* responseData,
		size_t* responseDataSize,

		const unsigned int sourceID,
		const unsigned int targetID,
		const SBPDataPackage* dataPackage
	);



	void SBPPXClientConnectToServer(SBPPXClient* const sbpPXClient, const char* ip, const unsigned short port);
//	void SBPPXClientConnectToServer(SBPPXClient* const sbpPXClient, const wchar_t* ip, const unsigned short port);
	void SBPPXClientDisconnectFromServer(SBPPXClient* const sbpPXClient);

	void SBPPXClientRegisterMe();
	void SBPPXClientSendText(const char* text);
	void SBPPXClientSendFile(const char* filePath);

#ifdef __cplusplus
}
#endif

#endif
