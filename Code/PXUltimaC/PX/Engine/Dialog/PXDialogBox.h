#ifndef PXEngineDialogBoxIncluded
#define PXEngineDialogBoxIncluded

#include <PX/Engine/PXEngine.h>
#include "PXDialogContainer.h"

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

    PXDialogText* DialogBoxText;
    PXTimer* DialogBoxTextTimer;

    PXTexture* TexturePageNext;
    PXTexture* TexturePageFinal;

    // References
    PXSprite* SpriteCharacterFace;
    PXSprite* SpriteButtonNextPage;
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
PXPublic PXResult PXAPI PXEngineDialogBoxTimerTrigger(PXEngine* const pxEngine, PXTimerEventInfo* const pxEngineTimerEventInfo, PXEngineDialogBox* const pxEngineDialogBox);

#endif