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

	typedef struct PXServer_
	{
		PXSocketState State;

		PXSocket* ServerSocketList;
		PXSize ServerSocketListSize;
		PXSize ServerSocketListSizeAllocated;

		void* Owner;

		PXLock PollingLock;

		PXSocketEventListener EventListener;
	}
	PXServer;

	PXPublic void PXServerConstruct(PXServer* const server);
	PXPublic void PXServerDestruct(PXServer* const server);

	PXPrivate PXBool PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket);

	PXPublic PXActionResult PXServerStart(PXServer* const server, const PXInt16U port, const ProtocolMode protocolMode);
	PXPublic PXActionResult PXServerStop(PXServer* const server);
	PXPublic PXActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID);

	static PXThreadResult PXServerPXClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif