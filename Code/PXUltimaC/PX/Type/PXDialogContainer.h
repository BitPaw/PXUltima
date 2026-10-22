#pragma once

#ifndef PXDialogIncluded
#define PXDialogIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Type/PXText.h>

typedef enum PXDialogCharacterMood_
{
    PXDialogCharacterMoodInvalid,
    PXDialogCharacterMoodCustom,
    PXDialogCharacterMoodNeutral,
    PXDialogCharacterMoodShame,
    PXDialogCharacterMoodHeh,
    PXDialogCharacterMoodHuh,
    PXDialogCharacterMoodSmile,
    PXDialogCharacterMoodHappy,
    PXDialogCharacterMoodSigh,
    PXDialogCharacterMoodConfused,
    PXDialogCharacterMoodShock,
    PXDialogCharacterMoodSad,
    PXDialogCharacterMoodEcstatic,
    PXDialogCharacterMoodDistressed,
    PXDialogCharacterMoodWhat,
    PXDialogCharacterMoodThinking,
    PXDialogCharacterMoodAngry,
    PXDialogCharacterMoodCry,
    PXDialogCharacterMoodYawn
}
PXDialogCharacterMood;

typedef struct PXDialogMessagePage_
{
    PXDialogCharacterMood CharacterMood;
    PXI8U CharacterMoodCounter;

    PXTexture2D* CharacterSprite; // Can be NULL, means no texture shall be used
    //PXFont* FontOverride;

    PXText Text;
}
PXDialogMessagePage;

PXPublic PXResult PXAPI PXDialogContainerMessageFetch(PXDialogMessagePage PXREF pxDialogContainer);

#endif