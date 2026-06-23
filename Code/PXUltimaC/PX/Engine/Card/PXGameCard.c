#include "PXGameCard.h"
#include <gl/GL.h>
#include <PX/Engine/Player/PXPlayerLobby.h>
#include <PX/Engine/Game/PXDeckFrensh.h>


typedef struct PXGameStateMauMau_
{
    PXCardDefitionMauMau CardDefition[32];
}
PXGameStateMauMau;

PXResult PXAPI PXGameCardCreate(PXGameCard PXREF pxGameCard, PXGame PXREF pxGame)
{
    if(!(pxGameCard && pxGame))
    {
        return PXResultRefusedParameterNull;
    }

    pxGameCard->Game = pxGame;

    return PXResultOK;
}

PXResult PXAPI PXGameCardRelease(PXGameCard PXREF pxGameCard)
{
    if(!pxGameCard)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXGameCardLoop(PXGameCard PXREF pxGameCard)
{
    /*
    for(;;) 
    {
        PXGameCardTick(pxGameCard);

        Player* p = &gs->players[gs->current_player];

        Action a = get_action_from_player_or_ai(p, gs, mode);

        if(!mode->validate_action(gs, &a)) {
            // ask again or force a draw
            continue;
        }

        mode->apply_action(gs, &a);

        if(mode->is_round_over(gs)) {
            mode->start_next_round(gs);
        }

        advance_turn(gs, mode);
    }*/

    return PXResultOK;
}

PXResult PXAPI PXGameCardDraw(PXGameCard PXREF pxGameCard, PXDrawInfo PXREF pxDrawInfo)
{
    // Draw stacks?

    // Draw all players hands
    PXGame PXREF pxGame = pxGameCard->Game;
    const PXSize playerAmount = PXGamePlayerAmount(pxGame);

    for(PXSize i = 0; i < playerAmount; i++)
    {
        PXPlayer PXREF pxPlayer = PXGamePlayerGet(pxGame, i);
        PXCardHand PXREF pxCardHand = PXGameCardPlayerGetHand(pxGameCard, pxPlayer);

        // Copy hand position
        PXVector3F32 handPosition = pxPlayer->Transform3D.Position;

        // Add offset
        PXVector3F32 offset = {0, 2, 0};
        PXVector3F32Add(&handPosition, &offset);

        PXCardHandDraw(pxCardHand, pxDrawInfo, &handPosition);
    }

    // Draw spectators
    // ...

    return PXResultOK;
}

PXCardHand* PXAPI PXGameCardPlayerGetHand(const PXGameCard PXREF pxGameCard, PXPlayer PXREF pxPlayer)
{
    return PXNull;
}

PXResult PXAPI PXCardDraw(PXCard PXREF pxCard, PXDrawInfo PXREF pxDrawInfo, PXVector3F32 PXREF pxVector3F32)
{
    if(!(pxCard && pxDrawInfo && pxVector3F32))
    {
        return PXResultRefusedParameterNull;
    }

    float width = 0.5;
    float height = 1.0;

    glColor3f(0.0f, 1.0f, 0.0f); // green lines
    glBegin(GL_LINE_LOOP);
    glVertex3f(pxVector3F32->X, pxVector3F32->Y, pxVector3F32->Z);
    glVertex3f(pxVector3F32->X + width, pxVector3F32->Y, pxVector3F32->Z);
    glVertex3f(pxVector3F32->X + width, pxVector3F32->Y + height, pxVector3F32->Z);
    glVertex3f(pxVector3F32->X, pxVector3F32->Y + height, pxVector3F32->Z);
    glEnd();

    return PXResultOK;
}

PXResult PXAPI PXCardHandDraw(PXCardHand PXREF pxCardHand, PXDrawInfo PXREF pxDrawInfo, PXVector3F32 PXREF positionRoot)
{
    if(!(pxCardHand && pxDrawInfo && positionRoot))
    {
        return PXResultRefusedParameterNull;
    }

    const PXF32 card_w = 0.12f;
    const PXF32 card_h = 0.18f;
    const PXF32 spacing = 0.14f;

    PXVector3F32 positionInsert = *positionRoot;    

    PXSize amountOfCards = pxCardHand->CardList.EntryAmountUsed;

    PXF32 start_x = positionInsert.X - (amountOfCards - 1) * spacing * 0.5f;

    for(PXSize i = 0; i < amountOfCards; ++i)
    {
        PXCard PXREF pxCard = PXListItemAtIndexGetT(PXCard, &pxCardHand->CardList, i);

        positionInsert.X = start_x + i * spacing;

        PXCardDraw(pxCard, pxDrawInfo , &positionInsert);
    }

    return PXResultOK;
}

PXResult PXAPI PXPlayerHandsDraw(PXCardHand PXREF pxCardHand, PXDrawInfo PXREF pxDrawInfo)
{


    return PXResultOK;
}