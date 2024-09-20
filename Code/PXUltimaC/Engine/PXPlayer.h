#ifndef PXPlayerInclude
#define PXPlayerInclude

#include "Media/PXResource.h"

#define PXPlayerRoleAdmin       (1 << 0) // Does user have total control?
#define PXPlayerRoleModerator   (1 << 1) // Does user have power to moderate activity?

// Status of player
#define PXPlayerStatusBanned        (1 << 0) // Is user banned?
#define PXPlayerStatusMutedVoice    (1 << 1) // Is user banned from speaking?
#define PXPlayerStatusMutedChat     (1 << 2) // Is user banned from chatting?

// Permission of intent of actions
#define PXPlayerPermissionReadDo            (1 << 1) // ...
#define PXPlayerPermissionReadRequest       (1 << 2) // ...
#define PXPlayerPermissionWriteDo           (1 << 3) // ...
#define PXPlayerPermissionWriteRequest      (1 << 4) // ...
#define PXPlayerPermissionExecuteDo         (1 << 5) // ...
#define PXPlayerPermissionExecuteRequest    (1 << 6) // ...

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
    PXColorRGBI8 Color;
    PXInt8U Flags;
}
PXPlayer;

#endif