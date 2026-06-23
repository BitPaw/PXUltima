#include "PXChess.h"
#include <PX/Engine/Rule/PXRule.h>

#if 0
const PXRuleDefinition PXRuleDefinitionChess[] =
{
    // --- GAME SETUP ---
    { PXChessCondition_GameStart,                PXChessAction_SetupBoard,                PXGameEventGAME_START },

    // --- TURN START ---
    { PXChessCondition_TurnStart,                PXChessAction_GeneratePseudoMoves,       PXGameEventTURN_START },
    { PXChessCondition_TurnStart,                PXChessAction_FilterLegalMoves,          PXGameEventTURN_START },
    { PXChessCondition_NoLegalMoves,             PXChessAction_CheckmateOrStalemate,      PXGameEventTURN_START },

    // --- PAWN MOVES ---
    { PXChessCondition_PawnForward1,             PXChessAction_AddMove,                   PXGameEventMoveGen },
    { PXChessCondition_PawnForward2,             PXChessAction_AddMove,                   PXGameEventMoveGen },
    { PXChessCondition_PawnCaptureLeft,          PXChessAction_AddMove,                   PXGameEventMoveGen },
    { PXChessCondition_PawnCaptureRight,         PXChessAction_AddMove,                   PXGameEventMoveGen },
    { PXChessCondition_PawnEnPassant,            PXChessAction_AddMove,                   PXGameEventMoveGen },
    { PXChessCondition_PawnPromotion,            PXChessAction_AddPromotionMoves,         PXGameEventMoveGen },

    // --- KNIGHT MOVES ---
    { PXChessCondition_KnightMove,               PXChessAction_AddMove,                   PXGameEventMoveGen },

    // --- BISHOP MOVES ---
    { PXChessCondition_BishopRay,                PXChessAction_AddMove,                   PXGameEventMoveGen },

    // --- ROOK MOVES ---
    { PXChessCondition_RookRay,                  PXChessAction_AddMove,                   PXGameEventMoveGen },

    // --- QUEEN MOVES ---
    { PXChessCondition_QueenRay,                 PXChessAction_AddMove,                   PXGameEventMoveGen },

    // --- KING MOVES ---
    { PXChessCondition_KingMove,                 PXChessAction_AddMove,                   PXGameEventMoveGen },

    // --- CASTLING ---
    { PXChessCondition_CanCastleKingSide,        PXChessAction_AddCastleMove,             PXGameEventMoveGen },
    { PXChessCondition_CanCastleQueenSide,       PXChessAction_AddCastleMove,             PXGameEventMoveGen },

    // --- MOVE LEGALITY FILTER ---
    { PXChessCondition_MoveLeavesKingInCheck,    PXChessAction_DiscardMove,               PXGameEventMoveFilter },
    { PXChessCondition_MoveIsLegal,              PXChessAction_KeepMove,                  PXGameEventMoveFilter },

    // --- MOVE EXECUTION ---
    { PXChessCondition_NormalMove,               PXChessAction_ApplyNormalMove,           PXGameEventMoveApply },
    { PXChessCondition_CaptureMove,              PXChessAction_ApplyCaptureMove,          PXGameEventMoveApply },
    { PXChessCondition_CastleMove,               PXChessAction_ApplyCastleMove,           PXGameEventMoveApply },
    { PXChessCondition_EnPassantMove,            PXChessAction_ApplyEnPassantMove,        PXGameEventMoveApply },
    { PXChessCondition_PromotionMove,            PXChessAction_ApplyPromotionMove,        PXGameEventMoveApply },

    // --- CHECK / CHECKMATE / STALEMATE ---
    { PXChessCondition_KingInCheck,              PXChessAction_SetCheckFlag,              PXGameEventTURN_START },
    { PXChessCondition_NoLegalMoves,             PXChessAction_CheckmateOrStalemate,      PXGameEventTURN_START },

    // --- GAME END RULES ---
    { PXChessCondition_Checkmate,                PXChessAction_PlayerLoses,               PXGameEventTURN_START },
    { PXChessCondition_Stalemate,                PXChessAction_DrawGame,                  PXGameEventTURN_START },
    { PXChessCondition_InsufficientMaterial,     PXChessAction_DrawGame,                  PXGameEventTURN_START },
    { PXChessCondition_ThreefoldRepetition,      PXChessAction_DrawGame,                  PXGameEventTURN_START },
    { PXChessCondition_FiftyMoveRule,            PXChessAction_DrawGame,                  PXGameEventTURN_START }
};
const PXI8U PXRuleDefinitionChessAmount = sizeof(PXRuleDefinitionChess) / sizeof(PXRuleDefinition);
#endif