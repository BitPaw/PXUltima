#pragma once

#ifndef PXPlayerActionIncluded
#define PXPlayerActionIncluded
#include <PX/Engine/Player/PXPlayer.h>

// When a player takes any action, this is the key.
typedef struct PXPlayerAction_
{
    // Who?
    PXID PlayerID;
    PXPlayer* PlayerSource; // who performed the action
    
    // What?
    PXID ActionID;        // defined by the GameMode
    
    // How?
    void* Payload;    // game-specific data
}
PXPlayerAction;

#endif