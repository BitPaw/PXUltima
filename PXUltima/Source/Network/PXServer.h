#ifndef PXServerINCLUDE
#define PXServerINCLUDE

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <Network/PXSocket.h>

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

		PXSocketEventListener SocketEventListener;
	}
	PXServer;

	PXPublic void PXServerConstruct(PXServer* const server);
	PXPublic void PXServerDestruct(PXServer* const server);

	PXPrivate PXBool PXServerSocketIDIsServer(const PXServer* const server, const PXSocketID socketID, PXSocket** const pxSocket);

	PXPublic PXActionResult PXServerStart(PXServer* const server, const unsigned short port, const ProtocolMode protocolMode);
	PXPublic PXActionResult PXServerStop(PXServer* const server);
	PXPublic PXActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID);

	static PXThreadResult PXServerPXClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif