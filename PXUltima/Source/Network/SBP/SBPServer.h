#ifndef SBPServerINCLUDE
#define SBPServerINCLUDE

#include <stddef.h>

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

	extern void SBPServerStart(const unsigned short port);
	extern void SBPServerStop();

	extern void SBPServerSendFile(const CSocketID clientID, const char* text);
	extern void SBPServerSendFile(const CSocketID clientID, wchar_t* text);

	extern const ResponseID SBPServerGenerateResponseID();
	extern unsigned char SBPServerSendMessageWaitResponse
	(
		const CSocketID clientID,
		const ResponseID responseID,
		const unsigned char* buffer,
		const size_t* bufferSize
	);

	extern void SBPServerSendTextToAll(const char* text);
	extern void SBPServerSendTextToAll(const wchar_t* text);
	extern void SBPServerSendTextToClient(const unsigned int clientID, const char* text);
	extern void SBPServerSendTextToClient(const unsigned int clientID, const wchar_t* text);

#ifdef __cplusplus
}
#endif

#endif