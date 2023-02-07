#ifndef SBPServerINCLUDE
#define SBPServerINCLUDE

#include <Format/Type.h>

#include "SBPProtocol.h"
#include "SBPDataChunk.h"

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

		SBPDataCache DataCache;

		PXDictionary ChannalEntryLookup; // Look for active channels
	}
	SBPServer;

	PXPrivate void OnSBPServerDataRawSend(const PXSocket* const pxSocket, const void* message, const PXSize messageSize);
	PXPrivate void OnSBPServerDataRawReceive(const PXSocket* const pxSocket, const void* const message, const PXSize messageSize);

	PXPrivate void OnSBPServerDataChunkRecived(SBPServer* const sbpServer, SBPDataCache* const sbpDataCache, const SBPDataChunk* const sbpDataChunk);
	PXPrivate void OnSBPServerChannalCreated(SBPServer* const sbpServer, SBPDataCache* const sbpDataCache, const PXInt8U channalID);

	PXPrivate void OnSBPServerPackageRecived(SBPServer* const sbpServer, const SBPDataPackage* const sbpDataPackage);


	PXPrivate void SBPServerPackageRecivedHandle(SBPServer* const sbpServer, const SBPDataPackage* const sbpDataPackage);

	PXPublic void SBPServerConstruct(SBPServer* const sbpServer);
	PXPublic void SBPServerDestruct(SBPServer* const sbpServer);

	PXPublic PXActionResult SBPServerStart(SBPServer* const sbpServer, const unsigned short port);
	PXPublic PXActionResult SBPServerStop(SBPServer* const sbpServer);

	PXPublic PXActionResult SBPServerSendFileA(SBPServer* const sbpServer, const PXSocketID clientID, const char* text);
	//CPublic void SBPServerSendFile(const PXSocketID clientID, wchar_t* text);

	PXPublic const ResponseID SBPServerGenerateResponseID();
	PXPublic unsigned char SBPServerSendMessageWaitResponse
	(
		const PXSocketID clientID,
		const ResponseID responseID,
		const unsigned char* buffer,
		const PXSize* bufferSize
	);

	PXPublic void SBPServerSendTextToAll(const char* text);
	//CPublic void SBPServerSendTextToAll(const wchar_t* text);
	PXPublic void SBPServerSendTextToPXClient(const unsigned int clientID, const char* text);
	//CPublic void SBPServerSendTextToPXClient(const unsigned int clientID, const wchar_t* text);

#ifdef __cplusplus
}
#endif

#endif
