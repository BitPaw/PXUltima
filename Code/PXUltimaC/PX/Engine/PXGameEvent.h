#pragma once

#ifndef PXGameEventIncluded
#define PXGameEventIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/Player/PXPlayer.h>

typedef enum PXGameEventID_
{
    PXGameEventInvalid, // Do not use, detection for uninit state.
    PXGameEventCustom, // Use this when self defined

    // Player - gameplay
    PXGameEventINTERACT,
    PXGameEventCastSpell,
    PXGameEventGAME_START,
    PXGameEventTURN_START,
    PXGameEventTURN_END,
    PXGameEventTurnSupress, // Effect to skip the turn

    PXGameEventPlayerSpawn, // Player (as in entity in game) enters the game


    // Card games
    PXGameEventCardPlay,
    PXGameEventCardDraw,
    PXGameEventCardDiscard,


    // Attack/Damage
    PXGameEventABILITY_USE,
    PXGameEventAttack,
    PXGameEventDAMAGE,

    // Entity
    PXGameEventSUMMON,
    PXGameEventDESTROY,


    // ???
    PXGameEventPlayerAction,
    PXGameEventPlayerChoice
}
PXGameEventID;

typedef struct PXGameEvent_
{
    PXSize PayloadSize;

    union
    {
        void* PayloadData;
        PXI32U Amount;
    };

    PXGameEventID EventID;

    PXPlayer* Source;
    PXPlayer* Target;
}
PXGameEvent;

#endif