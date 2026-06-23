#pragma once

#ifndef PXPlayerLobbyIncluded
#define PXPlayerLobbyIncluded
#include <PX/Engine/Player/PXPlayer.h>
#include <PX/Container/List/PXList.h>

typedef enum PXPlayerLobbyState_ 
{
    PXPlayerLobbyStateInvalid,
    PXPlayerLobbyStateOpen,
    PXPlayerLobbyStateInGame,
    PXPlayerLobbyStateClosed
} 
PXPlayerLobbyState;

// Lobby for players to join into, grouping them
typedef struct PXPlayerLobby_ PXPlayerLobby;

PXPublic PXResult PXAPI PXPlayerLobbyInitialize(PXPlayerLobby PXREF pxPlayerLobby);
PXPublic PXResult PXAPI PXPlayerLobbyPlayerJoin(PXPlayerLobby PXREF pxPlayerLobby, PXPlayer PXREF pxPlayer);
PXPublic PXResult PXAPI PXPlayerLobbyPlayerKick(PXPlayerLobby PXREF pxPlayerLobby, PXPlayer PXREF pxPlayer);

PXPublic PXSize PXAPI PXPlayerLobbyPlayerAmount(const PXPlayerLobby PXREF pxPlayerLobby);
PXPublic PXPlayer* PXAPI PXPlayerLobbyPlayerGetViaIndex(const PXPlayerLobby PXREF pxPlayerLobby, const PXSize index);


#endif