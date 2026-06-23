#include "PXDeckFrensh.h"

void PXAPI PXDeckFrenshInit(PXCardSet PXREF pxCardSet)
{
    for(PXI8U suits = 0; suits < 4; suits++)
    {
        for(PXI8U rank = 0; rank < 8; rank++)
        {
            /*
            PXCardDefitionMauMau PXREF pxCardDefition = &pxGameStateMauMau->CardDefition[suits * 8 + rank];

            pxCardDefition->Suit = (PXCardSuit)MAUMAU_SUITS[suits];
            pxCardDefition->Rank = (PXCardRank)MAUMAU_RANKS[rank];
            */
        }
    }
}