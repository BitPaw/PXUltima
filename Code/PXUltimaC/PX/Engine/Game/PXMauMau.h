#pragma once

#ifndef PXMauMauIncluded
#define PXMauMauIncluded
#include <PX/Type/PXType.h>

// Condition -> Action -> EffectQueue -> EffectProcessor -> Actual State Change



PXPublic PXResult PXAPI PXMauMauCreate(PXGameCard** pxGameCard);
PXPublic PXResult PXAPI PXMauMauRelease(PXGameCard PXREF pxGameCard);

// --- GAME SETUP ---
PXPrivate PXResult PXAPI PXMauMauAction_setup_deck_and_hands(PXGameCard PXREF pxGameCard);

// --- TURN START ---
PXPrivate PXResult PXAPI PXMauMauAction_allow_player_input(PXGameCard PXREF pxGameCard);

// --- PLAY VALIDATION ---
PXPrivate PXResult PXAPI PXMauMauCondition_match_rank_or_suit();
PXPrivate PXResult PXAPI PXMauMauAction_play_card();
PXPrivate PXResult PXAPI PXMauMauCondition_match_forced_suit();
PXPrivate PXResult PXAPI PXMauMauAction_play_card();
PXPrivate PXResult PXAPI PXMauMauCondition_no_playable_card();
PXPrivate PXResult PXAPI PXMauMauAction_force_draw_one();

// --- SPECIAL CARDS ---
PXPrivate PXResult PXAPI PXMauMauCondition_rank_7(PXGameCard PXREF pxGameCard);
PXPrivate PXResult PXAPI PXMauMauAction_next_player_draw_2(PXGameCard PXREF pxGameCard);
PXPrivate PXResult PXAPI PXMauMauCondition_rank_8(PXGameCard PXREF pxGameCard);
PXPrivate PXResult PXAPI PXMauMauAction_skip_next_player(PXGameCard PXREF pxGameCard);
PXPrivate PXResult PXAPI PXMauMauCondition_rank_J();
PXPrivate PXResult PXAPI PXMauMauAction_request_suit_choice();
PXPrivate PXResult PXAPI PXMauMauCondition_suit_chosen();
PXPrivate PXResult PXAPI PXMauMauAction_set_forced_suit();

// --- DRAW RULES ---
PXPrivate PXResult PXAPI PXMauMauCondition_player_draws();
PXPrivate PXResult PXAPI PXMauMauAction_move_card_to_hand();
PXPrivate PXResult PXAPI PXMauMauCondition_deck_empty();
PXPrivate PXResult PXAPI PXMauMauAction_reshuffle_discard_into_deck();

// --- TURN END ---
PXPrivate PXResult PXAPI PXMauMauCondition_player_finished_action();
PXPrivate PXResult PXAPI PXMauMauAction_end_turn();

// --- WIN CONDITION ---
PXPrivate PXResult PXAPI PXMauMauCondition_hand_empty();
PXPrivate PXResult PXAPI PXMauMauAction_player_wins();

#endif