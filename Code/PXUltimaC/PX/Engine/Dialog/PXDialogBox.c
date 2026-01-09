#include "PXDialogBox.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>

void PXAPI PXEngineDialogBoxPageNext(PXEngine PXREF pxEngine, PXEngineDialogBox PXREF pxEngineDialogBox)
{
    switch (pxEngineDialogBox->State)
    {
    case PXEngineDialogStateActive:
    {
        break;
    }
    case PXEngineDialogStateReadyForNextPage:
    {
        pxEngineDialogBox->IsNewWord = PXFalse;
        pxEngineDialogBox->LineNumber = 0;
        pxEngineDialogBox->DialogBoxText->TextRenderAmount = 0;

        ++(pxEngineDialogBox->PageAmountCurrent);

        PXDialogMessagePage PXREF pxDialogMessagePage = &pxEngineDialogBox->DialogMessagePageList[pxEngineDialogBox->PageAmountCurrent];

        PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);

        pxEngineDialogBox->State = PXEngineDialogStateActive;

        PXEngineResourceActionInfo pxEngineResourceActionInfo;
        PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

        pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
        pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
        pxEngineResourceActionInfo.ChangeInfo.Type = PXResourceTypeTimer;
        pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineDialogBox->DialogBoxTextTimer;

        PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);


        break;
    }
    case PXEngineDialogStateReadyToClose:
    {
        PXEngineDialogBoxClose(pxEngine, pxEngineDialogBox);
        break;
    }

    default:
        break;
    }
}

void PXAPI PXEngineDialogBoxOpen
(
    PXEngine PXREF pxEngine,
    PXEngineDialogBox PXREF pxEngineDialogBox,
    PXDialogMessagePage PXREF pxDialogMessagePage,
    const PXSize amountOfPages
)
{
    PXDialogText PXREF dialogBoxText = pxEngineDialogBox->DialogBoxText;

    pxEngineDialogBox->State = PXEngineDialogStateActive;
    pxEngineDialogBox->PageAmountCurrent = 0;
    pxEngineDialogBox->DialogMessagePageList = pxDialogMessagePage;
    pxEngineDialogBox->PageAmountLoaded = amountOfPages;

    pxEngineDialogBox->IsNewWord = PXFalse;
    pxEngineDialogBox->LineNumber = 0;
    dialogBoxText->TextRenderAmount = 0;

    PXEngineResourceActionInfo pxEngineResourceActionInfo[5];
    PXClearList(PXEngineResourceActionInfo, pxEngineResourceActionInfo, 5);

    pxEngineResourceActionInfo[0].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[0].ChangeInfo.Enable = PXTrue;
    pxEngineResourceActionInfo[0].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[0].ChangeInfo.Object = pxEngineDialogBox->SpriteButtonNextPage;

    pxEngineResourceActionInfo[1].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[1].ChangeInfo.Enable = PXTrue;
    pxEngineResourceActionInfo[1].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[1].ChangeInfo.Object = pxEngineDialogBox->DialogBoxSprite;

    pxEngineResourceActionInfo[2].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[2].ChangeInfo.Enable = PXTrue;
    pxEngineResourceActionInfo[2].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[2].ChangeInfo.Object = pxEngineDialogBox->SpriteCharacterFace;

    pxEngineResourceActionInfo[3].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[3].ChangeInfo.Enable = PXTrue;
    pxEngineResourceActionInfo[3].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[3].ChangeInfo.Object = pxEngineDialogBox->DialogBoxText;

    pxEngineResourceActionInfo[4].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[4].ChangeInfo.Enable = PXTrue;
    pxEngineResourceActionInfo[4].ChangeInfo.Type = PXResourceTypeTimer;
    pxEngineResourceActionInfo[4].ChangeInfo.Object = pxEngineDialogBox->DialogBoxTextTimer;

    PXEngineResourceActionBatch(pxEngine, pxEngineResourceActionInfo, 5);


    PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);
    PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);
}

void PXAPI PXEngineDialogBoxClose(PXEngine PXREF pxEngine, PXEngineDialogBox PXREF pxEngineDialogBox)
{
    PXEngineResourceActionInfo pxEngineResourceActionInfo[5];
    PXClearList(PXEngineResourceActionInfo, &pxEngineResourceActionInfo, 5);

    pxEngineResourceActionInfo[0].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[0].ChangeInfo.Enable = PXFalse;
    pxEngineResourceActionInfo[0].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[0].ChangeInfo.Object = pxEngineDialogBox->SpriteButtonNextPage;

    pxEngineResourceActionInfo[1].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[1].ChangeInfo.Enable = PXFalse;
    pxEngineResourceActionInfo[1].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[1].ChangeInfo.Object = pxEngineDialogBox->DialogBoxSprite;

    pxEngineResourceActionInfo[2].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[2].ChangeInfo.Enable = PXFalse;
    pxEngineResourceActionInfo[2].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[2].ChangeInfo.Object = pxEngineDialogBox->SpriteCharacterFace;

    pxEngineResourceActionInfo[3].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[3].ChangeInfo.Enable = PXFalse;
    pxEngineResourceActionInfo[3].ChangeInfo.Type = PXResourceTypeSprite;
    pxEngineResourceActionInfo[3].ChangeInfo.Object = pxEngineDialogBox->DialogBoxText;

    pxEngineResourceActionInfo[4].Type = PXEngineResourceActionTypeStateChange;
    pxEngineResourceActionInfo[4].ChangeInfo.Enable = PXFalse;
    pxEngineResourceActionInfo[4].ChangeInfo.Type = PXResourceTypeTimer;
    pxEngineResourceActionInfo[4].ChangeInfo.Object = pxEngineDialogBox->DialogBoxTextTimer;

    PXEngineResourceActionBatch(pxEngine, pxEngineResourceActionInfo, 5);

    pxEngineDialogBox->State = PXEngineDialogStateDormant;
    pxEngineDialogBox->IsLocked = PXFalse;
}

PXResult PXAPI PXEngineDialogBoxTimerTrigger(PXEngine PXREF pxEngine, PXTimerEventInfo PXREF pxEngineTimerEventInfo, PXEngineDialogBox PXREF pxEngineDialogBox)
{
    if (!(pxEngine && pxEngineTimerEventInfo && pxEngineDialogBox))
    {
        return PXResultRefusedParameterNull;
    }

    // Check state. If we already done, stop the timer from triggering.

    switch (pxEngineDialogBox->State)
    {
    case PXEngineDialogStateDormant:
        return PXActionFailedResourceUnavailableTryAgain;

    case PXEngineDialogStateActive:
    {
        PXDialogText PXREF dialogBoxText = pxEngineDialogBox->DialogBoxText;
        PXDialogMessagePage PXREF pxDialogMessagePage = &pxEngineDialogBox->DialogMessagePageList[pxEngineDialogBox->PageAmountCurrent];
        PXText PXREF pxTextDialog = &pxDialogMessagePage->Text;

        if (!pxTextDialog)
        {
            PXEngineDialogBoxClose(pxEngine, pxEngineDialogBox);

            return PXResultOK;
        }

        dialogBoxText->Text = pxTextDialog;

        const PXBool hasNextLetter = dialogBoxText->TextRenderAmount < pxTextDialog->SizeUsed;

        if (!hasNextLetter) // If we dont have any more text, fetch next page
        {
            const PXBool hasNextPage = (pxEngineDialogBox->PageAmountCurrent +1)< (pxEngineDialogBox->PageAmountLoaded);

            if (hasNextPage) // if we dont have any new page, we are done
            {
                pxEngineDialogBox->State = PXEngineDialogStateReadyForNextPage;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Dialog",
                    "Done with page <%i/%i>. Ready for <%i>",
                    pxEngineDialogBox->PageAmountCurrent + 1,
                    pxEngineDialogBox->PageAmountLoaded,
                    pxEngineDialogBox->PageAmountCurrent + 2
                );
#endif
            }
            else
            {
                pxEngineDialogBox->State = PXEngineDialogStateReadyToClose;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "PX",
                    "Dialog",
                    "Done with all pages <%i>.",
                    pxEngineDialogBox->PageAmountCurrent + 1
                );
#endif
            }

            PXEngineResourceActionInfo pxEngineResourceActionInfo;
            PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

            pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
            pxEngineResourceActionInfo.ChangeInfo.Enable = PXFalse;
            pxEngineResourceActionInfo.ChangeInfo.Type = PXResourceTypeTimer;
            pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineDialogBox->DialogBoxTextTimer;

            PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);

            break;
        }

        ++(dialogBoxText->TextRenderAmount);

        char character = pxTextDialog->A[dialogBoxText->TextRenderAmount];

        const PXI32U rendomDelay = (PXEngineGenerateRandom(pxEngine, 400) - 200) * 1000; // -2000 to 2000
        PXTimer PXREF pxEngineTimer = pxEngineTimerEventInfo->TimerReference;

        pxEngineTimer->TimeDelayShift = 0;

        // pxEngineDialogBox->DialogBoxCharacterImage = pxEngineDialogBox->PageNumber > 0? &pxChracterImageA : &pxChracterImageB;


        //pxChracterImageA.Invinsilbe = pxEngineDialogBox->PageNumber == 0;
        //pxChracterImageB.Invinsilbe = pxEngineDialogBox->PageNumber > 1;

        switch (character)
        {
        case '\n':
        {
           // pxEngine->Audio.DeviceStop(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);

            pxEngineDialogBox->LineNumber++;

            pxEngineTimer->TimeDelayShift = 5000000;

            break;
        }
        case '\0':
        case ' ':
        {
           // pxEngine->Audio.DeviceStop(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);
            pxEngineDialogBox->IsNewWord = PXFalse;
            // Nothing
            break;
        }
        default:
        {
            pxEngineTimer->TimeDelayShift = rendomDelay;

            if (!pxEngineDialogBox->IsNewWord || 1)
            {
               // pxEngine->Audio.DeviceRestart(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);
                pxEngineDialogBox->IsNewWord = PXTrue;
            }

            break;
        }
        }

        break;
    }
    case PXEngineDialogStateReadyToClose:
    {
        //PXEngineDialogBoxClose(pxEngine, pxEngineDialogBox);
        break;
    }
    default:
        return PXActionRefusedStateInvalid;
    }

    return PXResultOK;
}
