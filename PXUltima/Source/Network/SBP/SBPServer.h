#ifndef SBPServerINCLUDE
#define SBPServerINCLUDE

#include <Format/Type.h>

#include "SBPProtocol.h"

#include <Container/Dictionary/Dictionary.h>
#include <Network/Server.h>
#include <Network/Client.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct SBPServer_
	{
		CServer Server;
		//SBPQueue _inputQueue;
		//Dictionary<ResponseID, unsigned char*> _responseLookup;
	}
	SBPServer;

	CPublic void SBPServerStart(const unsigned short port);
	CPublic void SBPServerStop();

	CPublic void SBPServerSendFile(const CSocketID clientID, const char* text);
	CPublic void SBPServerSendFile(const CSocketID clientID, wchar_t* text);

	CPublic const ResponseID SBPServerGenerateResponseID();
	CPublic unsigned char SBPServerSendMessageWaitResponse
	(
		const CSocketID clientID,
		const ResponseID responseID,
		const unsigned char* buffer,
		const size_t* bufferSize
	);

	CPublic void SBPServerSendTextToAll(const char* text);
	CPublic void SBPServerSendTextToAll(const wchar_t* text);
	CPublic void SBPServerSendTextToClient(const unsigned int clientID, const char* text);
	CPublic void SBPServerSendTextToClient(const unsigned int clientID, const wchar_t* text);

#ifdef __cplusplus
}
#endif

#endif