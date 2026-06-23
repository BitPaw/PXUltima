#include "PXMauMau.h"
#include <PX/Engine/Rule/PXRule.h>

#include <PX/OS/PXOS.h>
#include "PXGame.h"

const PXSize amountOfCards = 5;
const PXSize amountOfCardsToDraw = 2;

const PXRuleDefinition PXRuleDefinitionMauMauRules[] =
{
    { PXNull,                                   PXMauMauAction_setup_deck_and_hands,        PXGameEventGAME_START },
    { PXNull,                                   PXMauMauAction_allow_player_input,          PXGameEventTURN_START },
    { PXMauMauCondition_match_rank_or_suit,     PXMauMauAction_play_card,                   PXGameEventCardPlay },
    { PXMauMauCondition_match_forced_suit,      PXMauMauAction_play_card,                   PXGameEventCardPlay },
    { PXMauMauCondition_no_playable_card,       PXMauMauAction_force_draw_one,              PXGameEventTURN_START },
    { PXMauMauCondition_rank_7,                 PXMauMauAction_next_player_draw_2,          PXGameEventCardPlay },
    { PXMauMauCondition_rank_8,                 PXMauMauAction_skip_next_player,            PXGameEventCardPlay },
    { PXMauMauCondition_rank_J,                 PXMauMauAction_request_suit_choice,         PXGameEventCardPlay },
    { PXMauMauCondition_suit_chosen,            PXMauMauAction_set_forced_suit,             PXGameEventPlayerChoice },
    { PXMauMauCondition_player_draws,           PXMauMauAction_move_card_to_hand,           PXGameEventCardDraw },
    { PXMauMauCondition_deck_empty,             PXMauMauAction_reshuffle_discard_into_deck, PXGameEventCardDraw },
    { PXMauMauCondition_player_finished_action, PXMauMauAction_end_turn,                    PXGameEventPlayerAction },
    { PXMauMauCondition_hand_empty,             PXMauMauAction_player_wins,                 PXGameEventTURN_END }
};
const PXI8U PXRuleDefinitionMauMauRulesAmount = sizeof(PXRuleDefinitionMauMauRules) / sizeof(PXRuleDefinition);


typedef struct PXCardGameMauMau_
{
    PXGameCard* GameCard;
}
PXCardGameMauMau;

PXResult PXAPI PXMauMauCreate(PXGameCard** pxGameCard)
{
   // PXGameCardInit();

    return PXResultOK;
}

PXResult PXAPI PXMauMauRelease(PXGameCard PXREF pxGameCard)
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_setup_deck_and_hands(PXGameCard PXREF pxGameCard)
{
    // Build deck


    const PXSize playerAmount = PXGamePlayerAmount(pxGameCard);

    PXPlayer* player = PXNull;
    PXGameEvent pxGameEvent;

    // Spawn each player
    for(PXSize i = 0; i < playerAmount; ++i)
    {
        player = PXGamePlayerGet(pxGameCard, i);

        pxGameEvent.EventID = PXGameEventPlayerSpawn;

        PXGameEventEnqueue(pxGameCard, &pxGameEvent);
    }

    // Spawn each player
    for(PXSize cardAmountIndex = 0; cardAmountIndex < amountOfCards; cardAmountIndex++)
    {
        for(PXSize playerINdex = 0; playerINdex < playerAmount; ++playerINdex)
        {
            player = PXGamePlayerGet(pxGameCard, playerINdex);

            pxGameEvent.EventID = PXGameEventCardDraw;

            PXGameEventEnqueue(pxGameCard, &pxGameEvent);
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_allow_player_input(PXGameCard PXREF pxGameCard)
{
    PXPlayer PXREF player = PXGamePlayerCurrentGet(pxGameCard);

    
    PXECSInfoFlagStateAdd(&player->Info, PXECSFlagBehaviourEnabled);    

    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_match_rank_or_suit()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_play_card()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_no_playable_card()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_force_draw_one()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_rank_7(PXGameCard PXREF pxGameCard)
{
    // Is current players card x?


    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_next_player_draw_2(PXGameCard PXREF pxGameCard)
{
    PXPlayer PXREF playerCurrent = PXGamePlayerCurrentGet(pxGameCard);
    PXPlayer PXREF playerNext = PXGamePlayerNextGet(pxGameCard);

    PXGameEvent pxGameEvent;
    PXClear(PXGameEvent, &pxGameEvent);
    pxGameEvent.Source = playerCurrent;
    pxGameEvent.Target = playerNext;
    pxGameEvent.EventID = PXGameEventCardDraw;
    pxGameEvent.Amount = amountOfCardsToDraw;

    

    PXGameEventEnqueue(pxGameCard, &pxGameEvent);

    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_rank_8(PXGameCard PXREF pxGameCard)
{
   // How to check???


    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_skip_next_player(PXGameCard PXREF pxGameCard)
{
    PXPlayer PXREF playerCurrent = PXGamePlayerCurrentGet(pxGameCard);
    PXPlayer PXREF playerNext = PXGamePlayerNextGet(pxGameCard);

    PXGameEvent pxGameEvent;
    PXClear(PXGameEvent, &pxGameEvent);
    pxGameEvent.Source = playerCurrent;
    pxGameEvent.Target = playerNext;
    pxGameEvent.EventID = PXGameEventTurnSupress;
    pxGameEvent.Amount = 1;

    PXGameEventEnqueue(pxGameCard, &pxGameEvent);

    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_rank_J()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_request_suit_choice()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_suit_chosen()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_set_forced_suit()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_player_draws()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_move_card_to_hand()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_deck_empty()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_reshuffle_discard_into_deck()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_player_finished_action()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_end_turn()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_hand_empty()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauAction_player_wins()
{
    return PXResultOK;
}

PXResult PXAPI PXMauMauCondition_match_forced_suit()
{
    return PXResultOK;
}