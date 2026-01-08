#pragma once

#ifndef PXPlayerIncluded
#define PXPlayerIncluded

//#include "Media/PXResource.h"

//---------------------------------------------------------
// Online status of player
//---------------------------------------------------------
#define PXPlayerStatusOnline        (1 << 0) // Is currently connected
#define PXPlayerStatusAFK           (1 << 1) // Is away. From beeing inactive for a longer time
#define PXPlayerStatusStale         (1 << 2) // So long AFK that one might not come back any time soon
#define PXPlayerStatusMIA           (1 << 3) // Lost connection. Expect to connect at any point, waiting for response. 
#define PXPlayerStatusHidden        (1 << 4) // Pretent to be offline
#define PXPlayerStatusBusy          (1 << 5) // Will not recieve alertable notfications
#define PXPlayerStatusIsPlaying     (1 << 6) // Is currently playing a game
#define PXPlayerStatusInMatch       (1 << 7) // Is currently in a PvsP match, can't pause
//---------------------------------------------------------

//---------------------------------------------------------
// Permission of intent of actions
//---------------------------------------------------------
#define PXPlayerRoleAdmin                   (1 << 0+8) // Does user have total control?
#define PXPlayerRoleModerator               (1 << 1+8) // Does user have power to moderate activity?
#define PXPlayerPermissionReadDo            (1 << 2+8) // Permission to read aynthing without question
#define PXPlayerPermissionReadRequest       (1 << 3+8) // Permission is requested and need to be aproved
#define PXPlayerPermissionWriteDo           (1 << 4+8) // ...
#define PXPlayerPermissionWriteRequest      (1 << 5+8) // ...
#define PXPlayerPermissionExecuteDo         (1 << 6+8) // ...
#define PXPlayerPermissionExecuteRequest    (1 << 7+8) // ...
//---------------------------------------------------------

//---------------------------------------------------------
// Add the "Seven deadly sins" for reporting purposes
// Flag down, in a silly way, why somone might not be as plesant
//---------------------------------------------------------
#define PXPlayerBehaviourSinPride       (1 << 0+16) // ...
#define PXPlayerBehaviourSinEnvy        (1 << 1+16) // ...
#define PXPlayerBehaviourSinWrath       (1 << 2+16) // ...
#define PXPlayerBehaviourSinSloth       (1 << 3+16) // ...
#define PXPlayerBehaviourSinGreed       (1 << 4+16) // ...
#define PXPlayerBehaviourSinGluttony    (1 << 5+16) // ...
#define PXPlayerBehaviourSinLust        (1 << 6+16) // ...
//---------------------------------------------------------

//---------------------------------------------------------
// Status effects of player
//---------------------------------------------------------
#define PXPlayerStatusBannedJoin        (1 << 0+24) // Unable to join the server at all, reject connection.
#define PXPlayerStatusBannedInteract    (1 << 1+24) // Able to join but with limited use
#define PXPlayerStatusBannedVoice       (1 << 2+24) // Banned from speaking?
#define PXPlayerStatusBannedChat        (1 << 3+24) // Banned from chatting?
#define PXPlayerPermissionInviteSend    (1 << 4+24) // ...
//---------------------------------------------------------

typedef struct PXPlayer_
{
    // "PXPlayer.." flags are stored in the "Info->Flags" field
   // PXECSInfo Info;

    // Profile picture

  //  PXColorRGBI8 Color;
    
    // PXTimeStamp TimeSeenOnlineFirst;
    // PXTimeStamp TimeSeenOnlineLast;
    int x;
}
PXPlayer;

#endif
