#ifndef PXServerINCLUDE
#define PXServerINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Network/PXSocket.h>

#if PXSocketUSE

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

	PXPublic void PXServerConstruct(PXServer* const server);
	PXPublic void PXServerDestruct(PXServer* const server);

	PXPrivate PXBool PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket);

	PXPublic PXActionResult PXServerStart(PXServer* const server, const PXInt16U port, const ProtocolMode protocolMode);
	PXPublic PXActionResult PXServerStop(PXServer* const server);
	PXPublic PXActionResult PXServerKickClient(PXServer* const server, const PXSocketID socketID);

	PXPrivate PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket* const serverSocket);

#ifdef __cplusplus
}
#endif

#endif
#endif