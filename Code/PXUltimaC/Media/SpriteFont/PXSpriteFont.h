#ifndef PXSpriteFontINCLUDE
#define PXSpriteFontINCLUDE

#define PXSpriteFontFontNameSize 30u
#define PXSpriteFontCharSetNameSize 30u
#define PXSpriteFontPageFileNameSize 128

#include "../PXResource.h"

typedef enum PXSpriteFontChanalMode_
{
	PXSpriteFontChanalInvalid,
	PXSpriteFontChanalGlyphData,		// 0 if the channel holds the glyph data
	PXSpriteFontChanalOutline, 		// 1 if it holds the outline		
	PXSpriteFontChanalGlyphAndOutline,// 2 if it holds the glyphand the outline		
	PXSpriteFontChanalSetToZero,		// 3 if its set to zero		
	PXSpriteFontChanalSetToOne		// 4 if its set to one.
}
PXSpriteFontChanalMode;

typedef enum PXSpriteFontLineType_
{
	PXSpriteFontInvalid,

	PXSpriteFontLineUnkown,

	PXSpriteFontSymbolX,
	PXSpriteFontSymbolY,

	PXSpriteFontSymbolID,
	PXSpriteFontSymbolAntilising,


	PXSpriteFontSymbolCharacterDefinition,
	PXSpriteFontSymbolBase,
	PXSpriteFontSymbolInfo,
	PXSpriteFontSymbolFontName,
	PXSpriteFontSymbolFontSize,
	PXSpriteFontSymbolBold,
	PXSpriteFontSymbolPage,
	PXSpriteFontSymbolFilePath,
	PXSpriteFontSymbolChannel,


	PXSpriteFontSymbolCharacterList,
	PXSpriteFontSymbolCharacterAmount,
	PXSpriteFontSymbolPageList,

	PXSpriteFontSymbolItalic,
	PXSpriteFontSymbolScaleWidth,
	PXSpriteFontSymbolScaleHeight,
	PXSpriteFontSymbolPacked,
	PXSpriteFontSymbolHeight,
	PXSpriteFontSymbolCommon,
	PXSpriteFontSymbolSmooth,

	PXSpriteFontSymbolCharSet,
	PXSpriteFontSymbolUnicode,
	PXSpriteFontSymbolPadding,
	PXSpriteFontSymbolSpacing,
	PXSpriteFontSymbolXOffset,
	PXSpriteFontSymbolYOffset,

	PXSpriteFontSymbolXAdvance,
	PXSpriteFontSymbolStretchH,
	PXSpriteFontSymbolLineHeight

}
PXSpriteFontLineType;

typedef struct PXSpriteFontInfo_
{
	char Name[PXSpriteFontFontNameSize]; // Name of this font. (synonym: face)
	int Size;// Size of the Letters int Pixel as height.
	PXBool Bold;// Is the text fat?
	PXBool Italic;// Is the text cursive?
	char CharSet[PXSpriteFontCharSetNameSize];// The name of the OEM charset used (when not unicode). Like 'ANSI'.
	PXBool Unicode;// Is the charset in the unicode format?
	int StretchH;
	PXBool Smooth;// Set to 1 if smoothing was turned on.
	PXBool Supersampling; // Supersampling level used. 1 means no supersampling was used. (AA)
	unsigned int CharacterPadding[4]; // Padding for each character.
	unsigned int SpacerOffset[2];
	unsigned int OutlineThickness;// How thick is the line arount the Letter?
}
PXSpriteFontInfo;

typedef struct PXSpriteFontCommonData_
{
	int LineHeight;	// This is the distance in pixels between each line of text.		
	int Base;// The number of pixels from the absolute top of the line to the base of the characters.		
	int ScaleWidth; // The width of the texture, normally used to scale the x pos of the character image.	
	int ScaleHeight;	// The height of the texture, normally used to scale the y pos of the character image.	
	int AmountOfPages;	// The number of texture pages included in the font.	
	PXBool Packed; // Set to 1 if the monochrome characters have been packed into each of the texture channels.In this case alphaChnl describes what is stored in each channel.

	PXSpriteFontChanalMode AlphaChanal;
	PXSpriteFontChanalMode RedChanal;
	PXSpriteFontChanalMode GreenChanal;
	PXSpriteFontChanalMode BlueChanal;
}
PXSpriteFontCommonData;

typedef struct PXSpriteFont_
{
	PXSpriteFontInfo Info;
	PXSpriteFontCommonData CommonData;
	PXSize FontPageListSize;
}
PXSpriteFont;

PXPublic PXActionResult PXAPI PXSpriteFontLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSpriteFontSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

PXPrivate PXSpriteFontLineType PXAPI PeekSymbol(const char* const line, const PXSize fileDataSize);

PXPublic void PXAPI PXSpriteFontPrtinf(const PXSpriteFont* pxSpriteFont);

#endif