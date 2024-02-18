#ifndef PXEngineDialogBoxInclude
#define PXEngineDialogBoxInclude

#include <Engine/PXEngine.h>
#include "PXDialogContainer.h"

#ifdef __cplusplus
extern "C"
{
#endif


	typedef void (PXAPI* PXEngineDialogEventLineNew)(void* owner, struct PXEngineDialogBox_* const pxEngineDialogBox);
	typedef void (PXAPI* PXEngineDialogEventPageNew)(void* owner, struct PXEngineDialogBox_* const pxEngineDialogBox);

	typedef enum PXEngineDialogState_
	{
		PXEngineDialogStateInvalid,
		PXEngineDialogStateDormant,
		PXEngineDialogStateActive,
		PXEngineDialogStateReadyForNextPage,
		PXEngineDialogStateReadyToClose
	}
	PXEngineDialogState;

	typedef struct PXEngineDialogBoxCreateInfo_
	{
		char* textureBoxTextureFilePath;
		char* texturePageNextFilePath;
		char* texturePageFinalFilePath;
	}
	PXEngineDialogBoxCreateInfo;

	typedef struct PXEngineDialogBox_
	{
		PXEngineDialogState State;

		PXSprite* DialogBoxSprite;
		
		PXEngineText* DialogBoxText;
		PXEngineTimer* DialogBoxTextTimer;

		PXTexture2D* TexturePageNext;
		PXTexture2D* TexturePageFinal;

		// References
		PXSprite* SpriteCharacterFace;
		PXSprite* SpriteButtonNextPage;
		PXSound* DialogTextLetterPopSFX;

		// Internal
		PXBool IsLocked;
		PXBool IsNewWord;
		PXInt32U LineNumber;

		// Setting		
		PXBool DelayEnable;
		float DelaySpan;
		PXInt32U TextSpeed;


		PXDialogMessagePage* DialogMessagePageList;
		PXSize PageAmountLoaded;
		PXSize PageAmountCurrent;
	}
	PXEngineDialogBox;

	PXPublic void PXAPI PXEngineDialogBoxPageNext(PXEngine* const pxEngine, PXEngineDialogBox* const pxEngineDialogBox);
	PXPublic void PXAPI PXEngineDialogBoxOpen
	(
		PXEngine* const pxEngine, 
		PXEngineDialogBox* const pxEngineDialogBox, 
		PXDialogMessagePage* const pxDialogMessagePage,
		const PXSize amountOfPages
	);
	PXPublic void PXAPI PXEngineDialogBoxClose(PXEngine* const pxEngine, PXEngineDialogBox* const pxEngineDialogBox);
	PXPublic PXActionResult PXAPI PXEngineDialogBoxTimerTrigger(PXEngine* const pxEngine, PXEngineTimerEventInfo* const pxEngineTimerEventInfo, PXEngineDialogBox* const pxEngineDialogBox);

#ifdef __cplusplus
}
#endif

#endif