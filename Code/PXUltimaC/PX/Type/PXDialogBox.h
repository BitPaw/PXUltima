#pragma once

#ifndef PXEngineDialogBoxIncluded
#define PXEngineDialogBoxIncluded

#include "PXDialogContainer.h"
#include <PX/Type/PXTimer.h>
#include <PX/Type/PXSound.h>
#include <PX/Engine/PXEngine.h>
#include <PX/Type/PXRectangleNineCell.h>
#include <PX/Type/PXQuad.h>


typedef void (PXAPI* PXEngineDialogEventLineNew)(void* owner, struct PXEngineDialogBox_ PXREF pxEngineDialogBox);
typedef void (PXAPI* PXEngineDialogEventPageNew)(void* owner, struct PXEngineDialogBox_ PXREF pxEngineDialogBox);

typedef enum PXEngineDialogState_
{
    PXEngineDialogStateInvalid,
    PXEngineDialogStateDormant,
    PXEngineDialogStateActive,
    PXEngineDialogStateReadyForNextPage,
    PXEngineDialogStateReadyToClose
}
PXEngineDialogState;

typedef struct PXDialogBox_
{
    PXECSInfo Info;

    PXEngineDialogState State;

    // Elements
    PXRectangleNineCell* RectangleDialogBox; // Base
    PXQuad* RectangleCharacterFace;
    PXQuad* RectanglePageNext;
    PXQuad* RectanglePageFinal;
    

    PXDialogText* DialogBoxText;
    PXTimer* DialogBoxTextTimer;


    // References
    PXSound* DialogTextLetterPopSFX;

    // Internal
    PXBool IsLocked;
    PXBool IsNewWord;
    PXI32U LineNumber;

    // Setting
    PXBool DelayEnable;
    PXF32 DelaySpan;
    PXI32U TextSpeed;


    PXDialogMessagePage* DialogMessagePageList;
    PXSize PageAmountLoaded;
    PXSize PageAmountCurrent;


    PXShaderPixelFragment* ShaderPixelFragment;
    PXShaderVertex* ShaderVertex;
    PXShaderProgram* ShaderProgram;
}
PXDialogBox;

typedef struct PXDialogBoxCreateInfo_
{
    PXECSCreateInfo Info;
}
PXDialogBoxCreateInfo;

PXInternal void PXAPI PXDialogBoxRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXDialogBoxCreate(PXDialogBox** pxDialogBoxREF, PXDialogBoxCreateInfo PXREF pxDialogBoxCreateInfo);


PXPublic void PXAPI PXDialogBoxPageNext(PXDialogBox PXREF pxEngineDialogBox);
PXPublic void PXAPI PXDialogBoxOpen
(
    PXDialogBox PXREF pxEngineDialogBox,
    PXDialogMessagePage PXREF pxDialogMessagePage,
    const PXSize amountOfPages
);
PXPublic void PXAPI PXDialogBoxClose(PXDialogBox PXREF pxEngineDialogBox);
PXPublic PXResult PXAPI PXDialogBoxTimerTrigger(PXTimerEventInfo PXREF pxEngineTimerEventInfo);

#endif