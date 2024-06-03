/*
#define PXServerINCLUDE
#ifndef PXServerINCLUDE 
#define PXServerINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Network/PXSocket.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXClient_ PXClient;

	// 160 Bytes
	typedef struct PXServer_
	{
		PXSocketEventList EventList;

		void* Owner;

		PXSocketState State;

		PXSocket* ServerSocketList;
		PXSize ServerSocketListSize;
		PXSize ServerSocketListSizeAllocated;

		PXLock PollingLock;
	}
	PXServer;

	PXPublic void PXAPI PXServerConstruct(PXServer* const server);
	PXPublic void PXAPI PXServerDestruct(PXServer* const server);

	PXPrivate PXBool PXAPI PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket);

	PXPublic PXActionResult PXAPI PXServerStart(PXServer* const server, const PXInt16U port, const PXProtocolMode protocolMode);
	PXPublic PXActionResult PXAPI PXServerStop(PXServer* const server);
	PXPublic PXActionResult PXAPI PXServerKickClient(PXServer* const server, const PXSocketID socketID);

	PXPublic PXActionResult PXAPI PXServerSendToAll(PXServer* const server, const void* const data, const PXSize dataSize);

	PXPrivate PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket* const serverSocket);

#ifdef __cplusplus
}
#endif

#endif
*/