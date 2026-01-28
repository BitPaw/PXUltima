#include "PXResource.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXGUI.h>
#include <PX/Math/PXMath.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Time/PXStopWatch.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/Graphic/PXGraphic.h>


const char PXResourceManagerText[] = "Resource-PX";












#if 0
void PXAPI PXVertexBufferFormatInfoExtract(PXVertexBufferFormatInfo PXREF pxVertexBufferFormatInfo, const PXVertexBufferFormat pxVertexBufferFormat)
{
    pxVertexBufferFormatInfo->VertexAttributesAmount = 0;

    pxVertexBufferFormatInfo->Format = PXVertexBufferFormatExtract(PXVertexBufferFormatFormat, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Position = PXVertexBufferFormatExtract(PXVertexBufferFormatPosition, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->TexturePosition = PXVertexBufferFormatExtract(PXVertexBufferFormatTexturePosition, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Normal = PXVertexBufferFormatExtract(PXVertexBufferFormatNormal, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Color = PXVertexBufferFormatExtract(PXVertexBufferFormatColor, pxVertexBufferFormat);

    char* buffer = pxVertexBufferFormatInfo->AsText;
    PXSize offset = 0;
    PXSize sizeLeft = 64;

    if(pxVertexBufferFormatInfo->Position > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "P%i ", pxVertexBufferFormatInfo->Position);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->Normal > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "N%i ", pxVertexBufferFormatInfo->Normal);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->Color > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "C%i ", pxVertexBufferFormatInfo->Color);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->TexturePosition > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "T%i ", pxVertexBufferFormatInfo->TexturePosition);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    switch(pxVertexBufferFormatInfo->Format)
    {
        case PXVertexBufferFormatI08:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I8");
            pxVertexBufferFormatInfo->VertexElementSize = 1;
            break;

        case PXVertexBufferFormatI16:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I16");
            pxVertexBufferFormatInfo->VertexElementSize = 2;
            break;

        case PXVertexBufferFormatI32:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I32");
            pxVertexBufferFormatInfo->VertexElementSize = 4;
            break;

        case PXVertexBufferFormatF16:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "F16");
            pxVertexBufferFormatInfo->VertexElementSize = 2;
            break;

        case PXVertexBufferFormatF32:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "F32");
            pxVertexBufferFormatInfo->VertexElementSize = 4;
            break;

        default:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "ERROR");
            break;
    }


    // StrideSize
    pxVertexBufferFormatInfo->Stride =
        pxVertexBufferFormatInfo->Position +
        pxVertexBufferFormatInfo->TexturePosition +
        pxVertexBufferFormatInfo->Normal +
        pxVertexBufferFormatInfo->Color;
}
#endif

/*
PXI8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP2I8:
            return 2;
        case PXVertexBufferFormatP3I8:
            return 3;
        case PXVertexBufferFormatT2F_XYZ:
            return 2u + 3u;
        case PXVertexBufferFormatT2F_N3F_XYZ:
            return 2u + 3u + 3u;
        case PXVertexBufferFormatP3F32:
            return 3u;
        case PXVertexBufferFormatN3F_XYZ:
            return 3u + 3u;

        default:
            return 0u;
    }
}

PXI8U PXAPI PXVertexBufferFormatSizePerVertex(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP2I8:
        case PXVertexBufferFormatT2F16:
        case PXVertexBufferFormatT2F32:
            return 2;

        case PXVertexBufferFormatN3F16:
        case PXVertexBufferFormatP3F16:
        case PXVertexBufferFormatN3F32:
        case PXVertexBufferFormatP3F32:
        case PXVertexBufferFormatP3I8:
            return 3;

        default:
        {
            DebugBreak();
            return 0u;
        }
    }
}
*/


PXResult PXAPI PXResourcePropertyIO
(
    PXECSInfo PXREF PXECSInfo,
    PXECSProperty PXREF pxECSProperty, 
    const PXI8U mode,
    const PXBool doWrite
)
{
    switch(mode)
    {
        case PXResourcePropertyName:
        {
            PXText PXREF pxText = &pxECSProperty->Text;

            if(doWrite)
            {
               // PXListDynamicAdd(&_pxResourceManager.NameCache, &PXECSInfo->ID, pxText->A, pxText->SizeUsed);

                PXECSInfo->Behaviour |= PXECSInfoHasSource;
            }
            else
            {
               // PXListDynamicGet(&_pxResourceManager.NameCache, &PXECSInfo->ID, pxText->A, &pxText->SizeUsed);
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXResourceManagerText,
                "Name",
                "Mode:%10s - (%i) %s",
                doWrite ? "Write" : "Read",
                pxText->SizeUsed,
                pxText->A
            
            );
#endif           

            break;
        }
        case PXResourcePropertyPath:
        {
            PXText PXREF pxText = &pxECSProperty->Text;

            if(doWrite)
            {

            }
            else
            {
               // PXListDynamicGet(&_pxResourceManager.SourcePathCache, &PXECSInfo->ID, pxText->A, &pxText->SizeUsed);
            }    

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXResourceManagerText,
                "Path",
                "Mode:%10s - (%i) %s",
                doWrite ? "Write" : "Read",
                pxText->SizeUsed,
                pxText->A

            );
#endif

            break;
        }
        default:
        {
            return PXResultInvalid;
        }
    }

    return PXResultOK;
}



/*
PXResourceManager* PXAPI PXResourceManagerGet(void)
{
    if(PXResourceManagerFlagIsCreated & _pxResourceManager.Flags)
    {
        return &_pxResourceManager;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Start",
        "Initialize..."
    );
#endif

    // Not needed? global mem is alway zero?
    // We do it anyway.. just in case
    PXClear(PXResourceManager, &_pxResourceManager);

    PXLockCreate(&_pxResourceManager.AsyncLock, PXLockTypeProcessOnly);

    PXListDynamicInit(&_pxResourceManager.NameCache, sizeof(PXI32U), PXListDynamicSizeObject2Byte);
    PXListDynamicInit(&_pxResourceManager.SourcePathCache, sizeof(PXI32U), PXListDynamicSizeObject2Byte);

    PXDictionaryConstruct(&_pxResourceManager.SpritelLookUp, sizeof(PXI32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.FontLookUp, sizeof(PXI32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.TextLookUp, sizeof(PXI32U), sizeof(PXDialogText), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.TimerLookUp, sizeof(PXI32U), sizeof(PXTimer), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.SoundLookUp, sizeof(PXI32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.HitBoxLookUp, sizeof(PXI32U), sizeof(PXHitBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.TextureLookUp, sizeof(PXI32U), sizeof(PXTexture), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.ModelLookUp, sizeof(PXI32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.ShaderProgramLookup, sizeof(PXI32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.SkyBoxLookUp, sizeof(PXI32U), sizeof(PXSkyBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.BrushLookUp, sizeof(PXI32U), sizeof(PXWindowBrush), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.GUIElementLookup, sizeof(PXI32U), sizeof(PXWindow), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.SpriteAnimator, sizeof(PXI32U), sizeof(PXSpriteAnimator), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.IconLookUp, sizeof(PXI32U), sizeof(PXIcon), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_pxResourceManager.IconAtlasLookUp, sizeof(PXI32U), sizeof(PXIconAtlas), PXDictionaryValueLocalityExternalReference);


   // PXResourceComponentCreate();

    // Register these with additional lookups

#if 0
    // Resource lookup for directCall
    const PXResourceEntry _GlobalResourceEntryList[] =
    {
        {&_GLOBALResourceManager.SoundLookUp,   PXResourceCreateSound,                  "Sound",PXResourceTypeSound, sizeof(PXSound) },
        {&_GLOBALResourceManager.ModelLookUp,   PXResourceCreateModel,                  "Model",PXResourceTypeModel, sizeof(PXModel) },
        {&_GLOBALResourceManager.FontLookUp,    PXResourceCreateFont,                   "Font",PXResourceTypeFont, sizeof(PXFont) },
        {&_GLOBALResourceManager.MaterialLookUp, PXResourceCreateMaterial,              "Material", PXResourceTypeMaterial, sizeof(PXMaterial) },
        {&_GLOBALResourceManager.BrushLookUp,   PXResourceCreateBrush,                  "Brush",PXResourceTypeBrush, sizeof(PXWindowBrush) },
        {&_GLOBALResourceManager.TextureLookUp, PXResourceCreateTexture,              "Texture",PXResourceTypeTexture2D, sizeof(PXTexture) },
        {&_GLOBALResourceManager.ShaderProgramLookup, PXResourceCreateShaderProgram,    "ShaderProgram",PXResourceTypeShaderProgram, sizeof(PXShaderProgram) },
        {&_GLOBALResourceManager.SkyBoxLookUp,  PXResourceCreateSkybox,                 "SkyBox",PXResourceTypeSkybox, sizeof(PXSkyBox) },
        {&_GLOBALResourceManager.SpritelLookUp, PXResourceCreateSprite,                 "Sprite",PXResourceTypeSprite, sizeof(PXSprite) },
        {&_GLOBALResourceManager.IconLookUp,    PXResourceCreateIcon,                   "Icon",PXResourceTypeIcon, sizeof(PXIcon) },
        {&_GLOBALResourceManager.IconAtlasLookUp, PXResourceCreateIconAtlas,              "IconAtlas",PXResourceTypeIconAtlas, sizeof(PXIconAtlas) },
        {&_GLOBALResourceManager.SpriteAnimator, PXResourceCreateSpriteAnimator,        "SpriteAnimator",PXResourceTypeSpriteAnimator, sizeof(PXSpriteAnimator) },
        {&_GLOBALResourceManager.TimerLookUp,   PXResourceCreateTimer,                  "Timer",PXResourceTypeTimer, sizeof(PXTimer) },
        {&_GLOBALResourceManager.GUIElementLookup, PXResourceCreateWindow,              "Window", PXResourceTypeGUIElement, sizeof(PXWindow) },
        {&_GLOBALResourceManager.HitBoxLookUp,  PXResourceCreateHitBox,                 "HitBox",PXResourceTypeHitBox, sizeof(PXHitBox) },
        {&_GLOBALResourceManager.MaterialLookUp, PXNull,                                "Material", PXResourceTypeMaterialList, PXNull},
        {&_GLOBALResourceManager.SpriteMapAtlasLookUp, PXResourceCreateSpriteMap, "SpriteMap", PXResourceTypeSpriteMap, sizeof(PXSpriteMap) },
    };
    const PXSize amount = sizeof(_GlobalResourceEntryList) / sizeof(_GlobalResourceEntryList[0]);

    for(size_t i = 0; i < amount; i++)
    {

    }
#endif

    _pxResourceManager.Flags |= PXResourceManagerFlagIsCreated;



    return &_pxResourceManager;
}

*/







void PXAPI PXRectangleLTRBI32ToXYWHI32(const PXRectangleLTRBI32 PXREF pxRectangleLTRBI32, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32)
{
    pxRectangleXYWHI32->X = pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Y = pxRectangleLTRBI32->Top;
    pxRectangleXYWHI32->Width = pxRectangleLTRBI32->Right - pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Height = pxRectangleLTRBI32->Bottom - pxRectangleLTRBI32->Top;
}

void PXAPI PXRectangleXYWHI32ToLTRBI32(const PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, PXRectangleLTRBI32 PXREF pxRectangleLTRBI32)
{
    pxRectangleLTRBI32->Left = pxRectangleXYWHI32->X;
    pxRectangleLTRBI32->Top = pxRectangleXYWHI32->Y;
    pxRectangleLTRBI32->Right = pxRectangleXYWHI32->X + pxRectangleXYWHI32->Width;
    pxRectangleLTRBI32->Bottom = pxRectangleXYWHI32->Y + pxRectangleXYWHI32->Height;
}

void PXAPI PXRectangleXYWHI32ToVertex(const PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXVector2F32 PXREF screenSize, PXRectangleVertexF32 PXREF pxRectangleVertexF32)
{
    // Convert width and height to normalized scale
    pxRectangleVertexF32->BX = (float)pxRectangleXYWHI32->Width / screenSize->X * 2.0f;
    pxRectangleVertexF32->BY = (float)pxRectangleXYWHI32->Height / screenSize->Y * 2.0f;

    // Convert x and y to normalized coordinates (OpenGL origin is center)
    pxRectangleVertexF32->AX = ((float)pxRectangleXYWHI32->X / screenSize->X) * 2.0f - 1.0f + pxRectangleVertexF32->BX / 2.0f;
    pxRectangleVertexF32->AY = 1.0f - ((float)pxRectangleXYWHI32->Y / screenSize->Y) * 2.0f - pxRectangleVertexF32->BY / 2.0f;
}

