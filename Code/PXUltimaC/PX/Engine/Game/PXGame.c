#include "PXGame.h"
#include <PX/Engine/Player/PXPlayerLobby.h>


typedef struct PXGame_
{
    PXText Name;

    PXPlayerLobby* Lobby;

    // Ringbuffer to store all actions to a queue
    PXListT(PXGameEvent) EventQueue;

    // List for modules
    PXListT(PXGameModule) ModuleList;


    PXSize RoundNumber; // Current Turn
    PXSize GameNumber;

    // Playerorder

    // Card family
    //PXCardSet family;

    // Card lookup
    //const void* (*get_card_def)(uint16_t local_id);
    PXBool StartNewRound;

    // Rules
    PXGameCallBinding Binding;
}
PXGame;





const char PXGameText[] = "Game-PX";
const PXI8U PXGameTextLength = sizeof(PXGameText);
const PXECSRegisterInfoStatic PXGameInfoStatic =
{
    {sizeof(PXGameText), sizeof(PXGameText), PXGameText, TextFormatASCII},
    sizeof(PXGame),
    __alignof(PXGame),
    PXECSTypeSystem,
    PXGameCreate,
    PXGameRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXGameInfoDynamic;


void PXAPI PXGameRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXGameInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXGameInfoDynamic;
}

PXResult PXAPI PXGameCreate(PXGame** pxGameREF, PXGameCreateInfo PXREF pxGameCreateInfo)
{
    PXGame* pxGame = PXNull;

    if(!(pxGameREF && pxGameCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxGameCreateInfo->Info.Static = &PXGameInfoStatic;
    pxGameCreateInfo->Info.Dynamic = &PXGameInfoDynamic;
    PXResult pxResult = PXECSCreate(pxGameREF, pxGameCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxGame = *pxGameREF;
    pxGame->Binding = pxGameCreateInfo->Binding;

    PXListCreate(&pxGame->EventQueue, sizeof(PXGameEvent), 128);
    PXListCreate(&pxGame->ModuleList, sizeof(PXGameModule), 128);

    return PXResultOK;
}

PXResult PXAPI PXGameRelease(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return PXResultRefusedParameterNull;
    }

    PXListRelease(&pxGame->EventQueue);
    PXListRelease(&pxGame->ModuleList);

    return PXResultOK;
}

PXPlayer* PXAPI PXGamePlayerCurrentGet(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return PXNull;
    }

    return PXNull;
}

PXPlayer* PXAPI PXGamePlayerNextGet(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return PXNull;
    }

    return PXNull;
}

PXResult PXAPI PXGameTick(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return PXResultRefusedParameterNull;
    }

    PXBool isGameOver = pxGame->Binding.IsGameOver(pxGame);

    if(isGameOver)
    {
        return PXResultOK;
    }

    // Process actions
    for(PXSize actionIndex = 0; ; ++actionIndex)
    {
        PXPlayerAction* pxPlayerAction;

        // Get action

        pxGame->Binding.ActionApply(pxGame, pxPlayerAction);
    }

    PXBool isRoundOver = pxGame->Binding.IsRoundOver(pxGame);

    if(isRoundOver && pxGame->StartNewRound)
    {
        // Start next round
    }

    isGameOver = pxGame->Binding.IsGameOver(pxGame);

    if(isGameOver)
    {
        // Kill game
    }

    return PXResultOK;
}

PXResult PXAPI PXGameModuleRegister(PXGame PXREF pxGame, PXGameModule PXREF pxGameModule)
{
    if(!(pxGame && pxGameModule))
    {
        return PXResultRefusedParameterNull;
    }

    PXListAdd(&pxGame->ModuleList, pxGameModule);

    return PXResultOK;
}

PXResult PXAPI PXGameEventEnqueue(PXGame PXREF pxGame, PXGameEvent PXREF pxGameEvent)
{
    if(!(pxGame && pxGameEvent))
    {
        return PXResultRefusedParameterNull;
    }

    PXListEnqueue(&pxGame->ModuleList, pxGameEvent);

    return PXResultOK;
}

PXGameEvent* PXAPI PXGameEventDequeue(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return PXNull;
    }

    PXGameEvent PXREF pxGameEvent = (PXGameEvent*)PXListDequeue(&pxGame->ModuleList);

    return pxGameEvent;
}

PXSize PXAPI PXGamePlayerAmount(PXGame PXREF pxGame)
{
    if(!pxGame)
    {
        return 0;
    }

    return PXNull;
}

PXPlayer* PXAPI PXGamePlayerGet(PXGame PXREF pxGame, const PXSize playerIndex)
{
    if(!pxGame)
    {
        return PXNull;
    }

    return PXNull;
}

PXBool PXAPI PXGameConditionGameStart(PXGame PXREF pxGame)
{
    return pxGame->RoundNumber == 0;
}