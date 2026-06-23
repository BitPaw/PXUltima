#pragma once

#ifndef PXCardFrenshIncluded
#define PXCardFrenshIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/Card/PXGameCard.h>

typedef enum PXCardSuit_
{
    PXCardSuitCLUBS,
    PXCardSuitSPADES,
    PXCardSuitHEARTS,
    PXCardSuitDIAMONDS
}
PXCardSuit;

typedef enum PXCardRank_
{
    PXCardRank1,
    PXCardRank2,
    PXCardRank3,
    PXCardRank4,
    PXCardRank5,
    PXCardRank6,
    PXCardRank7,
    PXCardRank8,
    PXCardRank9,
    PXCardRank10,
    PXCardRankJack,
    PXCardRankQueen,
    PXCardRankKing,
    PXCardRankAce,
    PXCardRankJoker,
}
PXCardRank;

typedef struct PXCardDefitionMauMau_
{
    //PXCard Card;

    PXCardSuit Suit;
    PXCardRank Rank;
}
PXCardDefitionMauMau;

PXPublic void PXAPI PXDeckFrenshInit(PXCardSet PXREF pxCardSet);

#endif