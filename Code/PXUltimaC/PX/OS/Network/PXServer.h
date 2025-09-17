/*
#define PXServerIncluded
#ifndef PXServerIncluded
#define PXServerIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Network/PXSocket.h>

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

	PXPublic PXResult PXAPI PXServerStart(PXServer* const server, const PXI16U port, const PXProtocolMode protocolMode);
	PXPublic PXResult PXAPI PXServerStop(PXServer* const server);
	PXPublic PXResult PXAPI PXServerKickClient(PXServer* const server, const PXSocketID socketID);

	PXPublic PXResult PXAPI PXServerSendToAll(PXServer* const server, const void* const data, const PXSize dataSize);

	PXPrivate PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket* const serverSocket);

#ifdef __cplusplus
}
#endif

#endif
*/
