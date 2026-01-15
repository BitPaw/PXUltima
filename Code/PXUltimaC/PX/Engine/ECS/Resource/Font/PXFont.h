#pragma once

#ifndef PXFontIncluded
#define PXFontIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Media/PXText.h>

#define PXFontAntialiased   (1<<0)
#define PXFontItalic        (1<<1)
#define PXFontBold          (1<<2)
#define PXFontStrikeOut     (1<<3)
#define PXFontUnderline     (1<<4)

typedef struct PXFont_ PXFont;

typedef struct PXFontPageCharacter_
{
    PXI32U ID;
    PXF32 Position[2]; // Position of the character image in the texture.
    PXF32 Size[2];    // Size of the character image in the texture.
    PXF32 Offset[2];// Offset from the position-center.
    int XAdvance; // How much the current position should be advanced after drawing the character.
}
PXFontPageCharacter;

typedef struct PXFontPage_
{
    PXSize CharacteListEntrys;
    PXFontPageCharacter* CharacteList;

    PXTexture* Texture;
}
PXFontPage;


typedef struct PXFontCreateInfo_
{
    PXECSCreateInfo Info;

    PXShaderProgram* ShaderProgramCurrent;

    PXText RegisteredName; // Name of the font, registered by the OS
}
PXFontCreateInfo;

typedef struct PXFont_
{
    PXECSInfo Info;

    HFONT FontHandle;

    PXI32U FontBase;
    struct GLYPHMETRICSFLOAT* GlyphMetricsFloat; // Unicode range

    PXBool IsOK;


    PXSize PageListAmount;
    PXI16U Size;

    union
    {
        PXFontPage PagePrime;
        PXFontPage* PageList;
    };

    PXI32S Height;
    PXI32S Width;
    PXI32S Escapement;
    PXI32S Orientation;
    PXI32S Weight;

    PXI8U CharSet;
    PXI8U OutPrecision;
    PXI8U ClipPrecision;
    PXI8U Quality;
    PXI8U PitchAndFamily;

    char Name[32]; // ToDo: Remove this
}
PXFont;

PXPublic PXResult PXAPI PXFontRegisterToECS();

PXPublic PXFontPage* PXAPI PXFontPageGet(PXFont PXREF pxFont, const PXSize index);
PXPublic PXResult PXAPI PXFontPageCreate(PXFont PXREF pxFont, const PXSize amount);

PXPublic PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage PXREF pxFontPage, const PXI32U characterID);

PXPublic PXResult PXAPI PXFontListFetch();
PXPublic PXResult PXAPI PXFontCreate(PXFont** pxFontREF, PXFontCreateInfo PXREF pxFontCreateInfo);
PXPublic PXResult PXAPI PXFontRelease(PXFont PXREF pxFont);


#endif