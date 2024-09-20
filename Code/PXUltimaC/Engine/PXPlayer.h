#ifndef PXPlayerInclude
#define PXPlayerInclude

#include "Media/PXResource.h"

#define PXPlayerRoleAdmin       (1 << 0) // Does user have total control?
#define PXPlayerRoleModerator   (1 << 1) // Does user have power to moderate activity?

// Status-Online of player
#define PXPlayerStatusOnline   (1 << 0) // Is currently connected
#define PXPlayerStatusAFK      (1 << 1) // Is away. From beeing inactive for a longer time
#define PXPlayerStatusStale    (1 << 2) // So long AFK that one might not come back any time soon
#define PXPlayerStatusMIA      (1 << 3) // Lost connection. Expect to connect at any point, waiting for response. 
#define PXPlayerStatusHidden   (1 << 4) // Pretent to be offline
#define PXPlayerStatusIsPlaying(1 << 4) // Cu
#define PXPlayerStatusHidden   (1 << 4)

// Status effects of player
#define PXPlayerStatusBannedJoin     (1 << 0) // Unable to join the server at all, reject connection.
#define PXPlayerStatusBannedInteract (1 << 0) // Able to join but with limited use
#define PXPlayerStatusBannedVoice    (1 << 1) // Banned from speaking?
#define PXPlayerStatusBannedChat     (1 << 2) // Banned from chatting?

// Permission of intent of actions
#define PXPlayerPermissionReadDo            (1 << 0) // Permission to read aynthing without question
#define PXPlayerPermissionReadRequest       (1 << 1) // Permission is requested and need to be aproved
#define PXPlayerPermissionWriteDo           (1 << 2) // ...
#define PXPlayerPermissionWriteRequest      (1 << 3) // ...
#define PXPlayerPermissionExecuteDo         (1 << 4) // ...
#define PXPlayerPermissionExecuteRequest    (1 << 5) // ...
#define PXPlayerPermissionInviteSend        (1 << 5) // ...

// Add the "Seven deadly sins" for reporting purposes
// Flag down, in a silly way, why somone might not be as plesant
#define PXPlayerBehaviourSinPride       (1 << 0) // ...
#define PXPlayerBehaviourSinEnvy        (1 << 1) // ...
#define PXPlayerBehaviourSinWrath       (1 << 2) // ...
#define PXPlayerBehaviourSinSloth       (1 << 3) // ...
#define PXPlayerBehaviourSinGreed       (1 << 4) // ...
#define PXPlayerBehaviourSinGluttony    (1 << 5) // ...
#define PXPlayerBehaviourSinLust        (1 << 6) // ...

typedef struct PXPlayer_
{
    // Profile picture

    PXColorRGBI8 Color;
    PXInt8U Flags;

    // Last seen online
    //
}
PXPlayer;

#endif
