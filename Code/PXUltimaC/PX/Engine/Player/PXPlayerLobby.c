#include "PXPlayerLobby.h"

typedef struct PXPlayerLobby_
{
    PXECSInfo Info;

    PXListT(PXPlayer*) PlayerList;

    PXSize PlayerMaximal;
}
PXPlayerLobby;

PXResult PXAPI PXPlayerLobbyInitialize(PXPlayerLobby PXREF pxPlayerLobby)
{
    if(!pxPlayerLobby)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXPlayerLobbyPlayerJoin(PXPlayerLobby PXREF pxPlayerLobby, PXPlayer PXREF pxPlayer)
{
    if(!(pxPlayerLobby && pxPlayer))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXPlayerLobbyPlayerKick(PXPlayerLobby PXREF pxPlayerLobby, PXPlayer PXREF pxPlayer)
{
    if(!(pxPlayerLobby && pxPlayer))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXSize PXAPI PXPlayerLobbyPlayerAmount(const PXPlayerLobby PXREF pxPlayerLobby)
{
    if(!pxPlayerLobby)
    {
        return 0;
    }

    return pxPlayerLobby->PlayerList.EntryAmountUsed;
}

PXPlayer* PXAPI PXPlayerLobbyPlayerGetViaIndex(const PXPlayerLobby PXREF pxPlayerLobby, const PXSize index)
{
    if(!pxPlayerLobby)
    {
        return PXNull;
    }

    PXPlayer PXREF pxPlayer = PXListItemAtIndexGetT(PXPlayer, pxPlayerLobby, index);

    return pxPlayer;
}