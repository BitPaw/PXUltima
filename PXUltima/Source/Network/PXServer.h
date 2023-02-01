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

	//CPublic PXActionResult PXServerSendMessageToAll(PXServer* server, const unsigned char* data, const PXSize dataSize);
//	CPublic PXActionResult PXServerSendMessageToPXClient(PXServer* server, const PXSocketID clientID, const unsigned char* data, const PXSize dataSize);

	//CPublic PXActionResult PXServerSendFileToPXClient(PXServer* server, int clientID, const char* filePath);
	//CPublic PXActionResult PXServerSendFileToPXClient(PXServer* server, int clientID, const wchar_t* filePath);

	//CPublic PXActionResult PXServerBroadcastMessageToPXClients(PXServer* server, char* message, PXSize messageLength);
	//CPublic PXActionResult PXServerBroadcastFileToPXClients(PXServer* server, const char* filePath);

	static PXThreadResult PXServerPXClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif