#ifndef ServerINCLUDE
#define ServerINCLUDE

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <Network/CSocket.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct Client_ Client;

	typedef void (*ClientConnectedEvent)(const CSocket* serverSocket, const CSocket* clientSocket);
	typedef void (*ClientDisconnectedEvent)(const CSocket* serverSocket, const CSocket* clientSocket);
	typedef void (*ClientAcceptFailureEvent)(const CSocket* serverSocket);

	typedef struct CServer_
	{
		CSocket* ServerSocketList;
		size_t ServerSocketListSize;

		CSocket* ClientSocketList;
		size_t ClientSocketListSize;

		ClientConnectedEvent ClientConnectedCallback;
		ClientDisconnectedEvent ClientDisconnectedCallback;
		ClientAcceptFailureEvent ClientAcceptFailureCallback;
	}
	CServer;

	CPublic void ServerConstruct(CServer* const server);
	CPublic void ServerDestruct(CServer* const server);

	CPublic ActionResult ServerStart(CServer* const server, const unsigned short port, const ProtocolMode protocolMode);
	CPublic ActionResult ServerStop(CServer* const server);
	CPublic ActionResult ServerKickClient(CServer* server, const CSocketID socketID);
	CPublic CSocket* ServerGetClientViaID(CServer* server, const CSocketID socketID);

	CPublic void ServerRegisterClient(CServer* server, Client* client);

	//CPublic ActionResult ServerSendMessageToAll(CServer* server, const unsigned char* data, const size_t dataSize);
//	CPublic ActionResult ServerSendMessageToClient(CServer* server, const CSocketID clientID, const unsigned char* data, const size_t dataSize);

	//CPublic ActionResult ServerSendFileToClient(CServer* server, int clientID, const char* filePath);
	//CPublic ActionResult ServerSendFileToClient(CServer* server, int clientID, const wchar_t* filePath);

	//CPublic ActionResult ServerBroadcastMessageToClients(CServer* server, char* message, size_t messageLength);
	//CPublic ActionResult ServerBroadcastFileToClients(CServer* server, const char* filePath);

	static ThreadResult ServerClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif