#include "PXDialogBox.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>

const char PXDialogBoxName[] = "DialogBox";
const PXI8U PXDialogBoxLength = sizeof(PXDialogBoxName);
const PXECSRegisterInfoStatic PXDialogBoxRegisterInfoStatic =
{
    {sizeof(PXDialogBoxName), sizeof(PXDialogBoxName), PXDialogBoxName, TextFormatASCII},
    sizeof(PXDialogBox),
    __alignof(PXDialogBox),
    PXECSTypeContainer,
    PXDialogBoxCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXDialogBoxRegisterInfoDynamic;

void PXAPI PXDialogBoxRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXDialogBoxRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXDialogBoxRegisterInfoDynamic;
}

PXResult PXAPI PXDialogBoxCreate(PXDialogBox** pxDialogBoxREF, PXDialogBoxCreateInfo PXREF pxDialogBoxCreateInfo)
{
    PXDialogBox* pxDialogBox = PXNull;

    pxDialogBoxCreateInfo->Info.Static = &PXDialogBoxRegisterInfoStatic;
    pxDialogBoxCreateInfo->Info.Dynamic = &PXDialogBoxRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxDialogBoxREF, pxDialogBoxCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxDialogBox = *pxDialogBoxREF;
    

    {
        PXShaderPixelFragmentCreateInfo pxShaderPixelFragmentCreateInfo;
        PXClear(PXShaderPixelFragmentCreateInfo, &pxShaderPixelFragmentCreateInfo);
        PXTextFromAddressA(&pxShaderPixelFragmentCreateInfo.Shader.FilePath, "Shader/UI_Pixel.glsl", 21, 21);
       // PXTextFromAddressA(&pxShaderPixelFragmentCreateInfo.Shader.FilePath, "Shader/Object_Fragment.glsl", 21, 21);
        PXShaderPixelFragmentCreate(&pxDialogBox->ShaderPixelFragment, &pxShaderPixelFragmentCreateInfo);
    }

    {
        PXShaderVertexCreateInfo pxShaderVertexCreateInfo;
        PXClear(PXShaderVertexCreateInfo, &pxShaderVertexCreateInfo);
        PXTextFromAddressA(&pxShaderVertexCreateInfo.Shader.FilePath, "Shader/UI_Vertex.glsl", 22, 22);
      //  PXTextFromAddressA(&pxShaderVertexCreateInfo.Shader.FilePath, "Shader/Object_Vertex.glsl", 22, 22);
        PXShaderVertexCreate(&pxDialogBox->ShaderVertex, &pxShaderVertexCreateInfo);
    }

    {
        PXShaderProgramCreateInfo pxShaderPrograCmreateInfo;
        PXClear(PXShaderProgramCreateInfo, &pxShaderPrograCmreateInfo);
        PXTextFromAddressA(&pxShaderPrograCmreateInfo.Info.Name, "Shader_UI", 10, 10);
        pxShaderPrograCmreateInfo.PixelFragment = pxDialogBox->ShaderPixelFragment;
        pxShaderPrograCmreateInfo.Vertex = pxDialogBox->ShaderVertex;
        PXShaderProgramCreate(&pxDialogBox->ShaderProgram, &pxShaderPrograCmreateInfo);
    }

    PXTexture2D* baseTexture = 0;
    PXMaterial* pxMaterial;

    {     
        PXTexture2DCreateInfo pxTexture2DCreateInfo;
        PXClear(PXTexture2DCreateInfo, &pxTexture2DCreateInfo);
        PXTextFromAddressA(&pxTexture2DCreateInfo.Info.Name, "DialogBox_Base_Texture", 23, 23);
        PXTextFromAddressA(&pxTexture2DCreateInfo.Info.FilePath, "Texture/DialogBox2.bmp", 22, 22);
        
        PXTexture2DCreate(&baseTexture, &pxTexture2DCreateInfo);


        PXMaterialCreateInfo pxMaterialCreateInfo;
        PXClear(PXMaterialCreateInfo, &pxMaterialCreateInfo);
        PXTextFromAddressA(&pxMaterialCreateInfo.Info.Name, "DialogBox_Base_Material", 24, 24);
        pxMaterialCreateInfo.Data.ShaderProgram = pxDialogBox->ShaderProgram;
        pxMaterialCreateInfo.Data.TextureBase = baseTexture;

        PXMaterialCreate(&pxMaterial, &pxMaterialCreateInfo);
    }


    PXRectangleNineCellCreateInfo pxRectangleNineCellCreateInfo;
    PXClear(PXRectangleNineCellCreateInfo, &pxRectangleNineCellCreateInfo);
    pxRectangleNineCellCreateInfo.Texture = baseTexture;
    PXRectangleNineCellCreate(&pxDialogBox->RectangleDialogBox, &pxRectangleNineCellCreateInfo);


    PXModelMaterialSet(pxDialogBox->RectangleDialogBox->Model, pxMaterial, 0, 0);



    // PXQuadCreate(&pxDialogBox->RectangleCharacterFace);
    //  PXQuadCreate(&pxDialogBox->RectanglePageNext);
  //  PXQuadCreate(&pxDialogBox->RectanglePageFinal);


    



   // PXModelMaterialSet(pxDialogBox->RectangleCharacterFace->Model);

    //PXModelMaterialSet(pxDialogBox->RectanglePageNext->Model);
    //PXModelMaterialSet(pxDialogBox->RectanglePageFinal->Model, );


    return PXResultOK;
}

void PXAPI PXDialogBoxPageNext(PXDialogBox PXREF pxDialogBox)
{
    switch(pxDialogBox->State)
    {
        case PXEngineDialogStateActive:
        {
            break;
        }
        case PXEngineDialogStateReadyForNextPage:
        {
            pxDialogBox->IsNewWord = PXFalse;
            pxDialogBox->LineNumber = 0;
            pxDialogBox->DialogBoxText->TextRenderAmount = 0;

            ++(pxDialogBox->PageAmountCurrent);

            PXDialogMessagePage PXREF pxDialogMessagePage = &pxDialogBox->DialogMessagePageList[pxDialogBox->PageAmountCurrent];

            //PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);

            pxDialogBox->State = PXEngineDialogStateActive;
  
            PXTimerRun(pxDialogBox->DialogBoxTextTimer);            

            break;
        }
        case PXEngineDialogStateReadyToClose:
        {
            PXDialogBoxClose(pxDialogBox);
            break;
        }

        default:
            break;
    }
}

void PXAPI PXDialogBoxOpen
(
    PXEngine PXREF pxEngine,
    PXDialogBox PXREF pxEngineDialogBox,
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

    
    PXModelShow(pxEngineDialogBox->RectangleDialogBox);
    PXModelShow(pxEngineDialogBox->RectangleCharacterFace);
   // PXModelShow(pxEngineDialogBox->SpriteCharacterFace);
    // pxEngineDialogBox->DialogBoxText
    PXTimerRun(pxEngineDialogBox->DialogBoxTextTimer);


    //PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);
    //PXEngineSpriteTextureSet(pxEngine, pxEngineDialogBox->SpriteCharacterFace, pxDialogMessagePage->CharacterSprite);
}

void PXAPI PXDialogBoxClose(PXDialogBox PXREF pxEngineDialogBox)
{
    if(!pxEngineDialogBox)
    {
        return;
    }

    PXModelHide(pxEngineDialogBox->RectangleDialogBox);
    PXModelHide(pxEngineDialogBox->RectangleCharacterFace);
    //PXModelHide(pxEngineDialogBox->SpriteCharacterFace);
    // pxEngineDialogBox->DialogBoxText
    PXTimerStop(pxEngineDialogBox->DialogBoxTextTimer);


    pxEngineDialogBox->State = PXEngineDialogStateDormant;
    pxEngineDialogBox->IsLocked = PXFalse;
}

PXResult PXAPI PXDialogBoxTimerTrigger(PXTimerEventInfo PXREF pxEngineTimerEventInfo)
{
    if(!pxEngineTimerEventInfo)
    {
        return PXResultRefusedParameterNull;
    }

    PXDialogBox PXREF pxDialogBox = (PXDialogBox*)pxEngineTimerEventInfo->CallBackOwner;

    // Check state. If we already done, stop the timer from triggering.

    switch(pxDialogBox->State)
    {
        case PXEngineDialogStateDormant:
            return PXResultFailedResourceUnavailableTryAgain;

        case PXEngineDialogStateActive:
        {
            PXDialogText PXREF dialogBoxText = pxDialogBox->DialogBoxText;
            PXDialogMessagePage PXREF pxDialogMessagePage = &pxDialogBox->DialogMessagePageList[pxDialogBox->PageAmountCurrent];
            PXText PXREF pxTextDialog = &pxDialogMessagePage->Text;

            if(!pxTextDialog)
            {
                PXDialogBoxClose(pxDialogBox);

                return PXResultOK;
            }

            dialogBoxText->Text = pxTextDialog;

            const PXBool hasNextLetter = dialogBoxText->TextRenderAmount < pxTextDialog->SizeUsed;

            if(!hasNextLetter) // If we dont have any more text, fetch next page
            {
                const PXBool hasNextPage = (pxDialogBox->PageAmountCurrent + 1) < (pxDialogBox->PageAmountLoaded);

                if(hasNextPage) // if we dont have any new page, we are done
                {
                    pxDialogBox->State = PXEngineDialogStateReadyForNextPage;

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXDialogBoxName,
                        "Dialog",
                        "Done with page <%i/%i>. Ready for <%i>",
                        pxDialogBox->PageAmountCurrent + 1,
                        pxDialogBox->PageAmountLoaded,
                        pxDialogBox->PageAmountCurrent + 2
                    );
#endif
                }
                else
                {
                    pxDialogBox->State = PXEngineDialogStateReadyToClose;

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXDialogBoxName,
                        "Dialog",
                        "Done with all pages <%i>.",
                        pxDialogBox->PageAmountCurrent + 1
                    );
#endif
                }

                PXTimerStop(pxDialogBox->DialogBoxTextTimer);

                break;
            }

            ++(dialogBoxText->TextRenderAmount);

            char character = pxTextDialog->A[dialogBoxText->TextRenderAmount];

            const PXI32U rendomDelay = (PXEngineGenerateRandom(0, 400) - 200) * 1000; // -2000 to 2000
            PXTimer PXREF pxEngineTimer = pxEngineTimerEventInfo->TimerReference;

            pxEngineTimer->TimeDelayShift = 0;

            // pxEngineDialogBox->DialogBoxCharacterImage = pxEngineDialogBox->PageNumber > 0? &pxChracterImageA : &pxChracterImageB;


            //pxChracterImageA.Invinsilbe = pxEngineDialogBox->PageNumber == 0;
            //pxChracterImageB.Invinsilbe = pxEngineDialogBox->PageNumber > 1;

            switch(character)
            {
                case '\n':
                {
                    // pxEngine->Audio.DeviceStop(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);

                    pxDialogBox->LineNumber++;

                    pxEngineTimer->TimeDelayShift = 5000000;

                    break;
                }
                case '\0':
                case ' ':
                {
                    // pxEngine->Audio.DeviceStop(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);
                    pxDialogBox->IsNewWord = PXFalse;
                    // Nothing
                    break;
                }
                default:
                {
                    pxEngineTimer->TimeDelayShift = rendomDelay;

                    if(!pxDialogBox->IsNewWord || 1)
                    {
                        // pxEngine->Audio.DeviceRestart(&pxEngine->Audio, &pxEngine->AudioStandardOutDevice);
                        pxDialogBox->IsNewWord = PXTrue;
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
            return PXResultRefusedStateInvalid;
    }

    return PXResultOK;
}