#ifndef PXDialogInclude
#define PXDialogInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXDialogBoxState_
	{
		PXDialogBoxStateInvalid,
		PXDialogBoxStateClosed,
		PXDialogBoxStateOpen
	}
	PXDialogBoxState;

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

	typedef struct PXDialogContainerMesssage_
	{
		PXDialogCharacterMood CharacterMood;
		PXInt8U CharacterMoodCounter;

		PXTexture2D* CharacterSprite;
		PXText Text;
		PXDialogBoxState State;
	}
	PXDialogContainerMesssage;

	typedef struct PXDialogContainer_
	{
		int __dummy__;
	}
	PXDialogContainer;

	PXPublic PXActionResult PXAPI PXDialogContainerMessageFetch(PXDialogContainer* const pxDialogContainer);

#ifdef __cplusplus
}
#endif

#endif