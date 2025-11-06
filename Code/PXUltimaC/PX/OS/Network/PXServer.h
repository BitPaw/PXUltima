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

	PXPublic void PXAPI PXServerConstruct(PXServer PXREF server);
	PXPublic void PXAPI PXServerDestruct(PXServer PXREF server);

	PXPrivate PXBool PXAPI PXServerSocketIDIsServer(const PXServer PXREF server, const PXSocketID socketID, PXSocket* PXREF pxSocket);

	PXPublic PXResult PXAPI PXServerStart(PXServer PXREF server, const PXI16U port, const PXProtocolMode protocolMode);
	PXPublic PXResult PXAPI PXServerStop(PXServer PXREF server);
	PXPublic PXResult PXAPI PXServerKickClient(PXServer PXREF server, const PXSocketID socketID);

	PXPublic PXResult PXAPI PXServerSendToAll(PXServer PXREF server, const void PXREF data, const PXSize dataSize);

	PXPrivate PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket PXREF serverSocket);

#ifdef __cplusplus
}
#endif

#endif
*/
