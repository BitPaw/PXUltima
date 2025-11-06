#pragma once

#ifndef PXDialogIncluded
#define PXDialogIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Media/PXText.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

    PXTexture* CharacterSprite; // Can be NULL, means no texture shall be used
    //PXFont* FontOverride;

    PXText Text;
}
PXDialogMessagePage;


PXPublic PXResult PXAPI PXDialogContainerMessageFetch(PXDialogMessagePage PXREF pxDialogContainer);

#ifdef __cplusplus
}
#endif

#endif
