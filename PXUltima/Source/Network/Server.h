#ifndef ServerINCLUDE
#define ServerINCLUDE

#include <stddef.h>

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

	extern void ServerConstruct(CServer* const server);
	extern void ServerDestruct(CServer* const server);

	extern ActionResult ServerStart(CServer* const server, const unsigned short port, const ProtocolMode protocolMode);
	extern ActionResult ServerStop(CServer* const server);
	extern ActionResult ServerKickClient(CServer* server, const CSocketID socketID);
	extern CSocket* ServerGetClientViaID(CServer* server, const CSocketID socketID);

	extern void ServerRegisterClient(CServer* server, Client* client);

	//extern ActionResult ServerSendMessageToAll(CServer* server, const unsigned char* data, const size_t dataSize);
//	extern ActionResult ServerSendMessageToClient(CServer* server, const CSocketID clientID, const unsigned char* data, const size_t dataSize);

	//extern ActionResult ServerSendFileToClient(CServer* server, int clientID, const char* filePath);
	//extern ActionResult ServerSendFileToClient(CServer* server, int clientID, const wchar_t* filePath);

	//extern ActionResult ServerBroadcastMessageToClients(CServer* server, char* message, size_t messageLength);
	//extern ActionResult ServerBroadcastFileToClients(CServer* server, const char* filePath);

	static ThreadResult ServerClientListeningThread(void* server);

#ifdef __cplusplus
}
#endif

#endif