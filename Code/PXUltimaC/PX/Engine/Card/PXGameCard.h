#pragma once

#ifndef PXGameCardIncluded
#define PXGameCardIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/PXGameEvent.h>
#include <PX/Container/List/PXList.h>
#include <PX/Engine/Game/PXGame.h>



#define PXCardFlagHidden        1<<1 // Is card on back?
#define PXCardFlagSideways      1<<2 // Is card laying sideways?
#define PXCardFlagHasEffect     1<<3

typedef struct PXCard
{
    PXI32U FamilyID;   // which game this card belongs to
    PXI32U LocalID;    // index inside that family

    PXI32U StateFlags;
}
PXCard;



typedef struct PXCardHand_
{
    PXListT(PXCard*) CardList;
}
PXCardHand;



// Register the cardset
// Could alternatively be called CardFamily
typedef struct PXCardSet_
{
    PXECSInfo Info;

    PXI32U CardCount;
}
PXCardSet;

typedef struct PXGameCard_
{
    PXGame* Game;
}
PXGameCard;






PXPublic PXResult PXAPI PXGameCardCreate(PXGameCard PXREF pxGameCard, PXGame PXREF pxGame);
PXPublic PXResult PXAPI PXGameCardRelease(PXGameCard PXREF pxGameCard);


PXPublic PXResult PXAPI PXGameCardLoop(PXGameCard PXREF pxGameCard);

PXPublic PXResult PXAPI PXGameCardDraw(PXGameCard PXREF pxGameCard, PXDrawInfo PXREF pxDrawInfo);

// Player

PXPublic PXCardHand* PXAPI PXGameCardPlayerGetHand(const PXGameCard PXREF pxGameCard, PXPlayer PXREF pxPlayer);


PXPublic PXResult PXAPI PXCardDraw(PXCard PXREF pxCard, PXDrawInfo PXREF pxDrawInfo, PXVector3F32 PXREF pxVector3F32);
PXPublic PXResult PXAPI PXCardHandDraw(PXCardHand PXREF pxCardHand, PXDrawInfo PXREF pxDrawInfo, PXVector3F32 PXREF pxVector3F32);

#endif