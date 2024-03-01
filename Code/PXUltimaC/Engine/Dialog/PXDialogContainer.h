#ifndef PXDialogInclude
#define PXDialogInclude

#include <Media/PXResource.h>
#include <Media/PXText.h>

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
		PXInt8U CharacterMoodCounter;

		PXTexture2D* CharacterSprite; // Can be NULL, means no texture shall be used
		//PXFont* FontOverride;

		PXText Text;
	}
	PXDialogMessagePage;


	PXPublic PXActionResult PXAPI PXDialogContainerMessageFetch(PXDialogMessagePage* const pxDialogContainer);

#ifdef __cplusplus
}
#endif

#endif