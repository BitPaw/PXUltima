#ifndef PXServerINCLUDE
#define PXServerINCLUDE

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <Network/PXSocket.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXClient_ PXClient;

	typedef void (*PXClientConnectedEvent)(const PXSocket* serverSocket, const PXSocket* clientSocket);
	typedef void (*PXClientDisconnectedEvent)(const PXSocket* serverSocket, const PXSocket* clientSocket);
	typedef void (*PXClientAcceptFailureEvent)(const PXSocket* serverSocket);

	typedef struct PXServer_
	{
		PXSocket* ServerSocketList;
		PXSize ServerSocketListSize;

		PXSocket* PXClientSocketList;
		PXSize PXClientSocketListSize;

		PXClientConnectedEvent PXClientConnectedCallback;
		PXClientDisconnectedEvent PXClientDisconnectedCallback;
		PXClientAcceptFailureEvent PXClientAcceptFailureCallback;
	}
	PXServer;

	PXPublic void PXServerConstruct(PXServer* const server);
	PXPublic void PXServerDestruct(PXServer* const server);

	PXPublic ActionResult PXServerStart(PXServer* const server, const unsigned short port, const ProtocolMode protocolMode);
	PXPublic ActionResult PXServerStop(PXServer* const server);
	PXPublic ActionResult PXServerKickPXClient(PXServer* const server, const PXSocketID socketID);
	PXPublic PXSocket* PXServerGetPXClientViaID(PXServer* const server, const PXSocketID socketID);

	PXPublic void PXServerRegisterPXClient(PXServer* const server, PXClient* const client);

	//CPublic ActionResult PXServerSendMessageToAll(PXServer* server, const unsigned char* data, const PXSize dataSize);
//	CPublic ActionResult PXServerSendMessageToPXClient(PXServer* server, const PXSocketID clientID, const unsigned char* data, const PXSize dataSize);

	//CPublic ActionResult PXServerSendFileToPXClient(PXServer* server, int clientID, const char* filePath);
	//CPublic ActionResult PXServerSendFileToPXClient(PXServer* server, int clientID, const wchar_t* filePath);

	//CPublic ActionResult PXServerBroadcastMessageToPXClients(PXServer* server, char* message, PXSize messageLength);
	//CPublic ActionResult PXServerBroadcastFileToPXClients(PXServer* server, const char* filePath);

	static PXThreadResult PXServerPXClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif