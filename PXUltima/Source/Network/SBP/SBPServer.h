#ifndef SBPServerINCLUDE
#define SBPServerINCLUDE

#include <Format/Type.h>

#include "SBPProtocol.h"

#include <Container/Dictionary/PXDictionary.h>
#include <Network/PXServer.h>
#include <Network/PXClient.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct SBPServer_
	{
		PXServer PXServer;
		//SBPQueue _inputQueue;
		//PXDictionary<ResponseID, unsigned char*> _responseLookup;
	}
	SBPServer;

	PXPublic void SBPServerStart(const unsigned short port);
	PXPublic void SBPServerStop();

	PXPublic void SBPServerSendFile(const PXSocketID clientID, const char* text);
	//CPublic void SBPServerSendFile(const PXSocketID clientID, wchar_t* text);

	PXPublic const ResponseID SBPServerGenerateResponseID();
	PXPublic unsigned char SBPServerSendMessageWaitResponse
	(
		const PXSocketID clientID,
		const ResponseID responseID,
		const unsigned char* buffer,
		const size_t* bufferSize
	);

	PXPublic void SBPServerSendTextToAll(const char* text);
	//CPublic void SBPServerSendTextToAll(const wchar_t* text);
	PXPublic void SBPServerSendTextToPXClient(const unsigned int clientID, const char* text);
	//CPublic void SBPServerSendTextToPXClient(const unsigned int clientID, const wchar_t* text);

#ifdef __cplusplus
}
#endif

#endif
