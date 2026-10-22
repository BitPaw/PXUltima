#ifndef PXServerIncluded
#define PXServerIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Network/PXSocket.h>
#include <PX/Engine/PXDictionaryECS.h>
#include <PX/OS/Network/PXNetwork.h>

typedef struct PXClient_ PXClient;

typedef struct PXServer_
{
    PXECSInfo Info;

    PXSocket* Socketv4;
    PXSocket* Socketv6;

    PXDictionaryECS ClientLookup;

    //PXLock PollingLock;

    PXSocketEventList EventList;
    void* Owner;
}
PXServer;

typedef struct PXServerCreateInfo_
{
    PXECSCreateInfo Info;
}
PXServerCreateInfo;

PXPublic void PXAPI PXServerConstruct(PXServer PXREF server);
PXPublic void PXAPI PXServerDestruct(PXServer PXREF server);

PXPublic PXResult PXAPI PXServerStart(PXServer PXREF server, const PXI16U port, const PXProtocolMode protocolMode);
PXPublic PXResult PXAPI PXServerStop(PXServer PXREF server);
PXPublic PXResult PXAPI PXServerKickClient(PXServer PXREF server, const PXSocketID socketID);
PXPublic PXResult PXAPI PXServerSendToAll(PXServer PXREF server, const void PXREF data, const PXSize dataSize);

PXPrivate PXBool PXAPI PXServerSocketIDIsServer(const PXServer PXREF server, const PXSocketID socketID, PXSocket* PXREF pxSocket);
PXPrivate PXThreadResult PXOSAPI PXServerClientListeningThread(PXSocket PXREF serverSocket);

#endif