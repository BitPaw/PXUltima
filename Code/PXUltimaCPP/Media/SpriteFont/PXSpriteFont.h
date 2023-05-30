#ifndef PXSpriteFontINCLUDE
#define PXSpriteFontINCLUDE

#define PXSpriteFontFontNameSize 30u
#define PXSpriteFontCharSetNameSize 30u
#define PXSpriteFontPageFileNameSize 128

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>
#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
		PXSpriteFontLineUnkown,
		PXSpriteFontLineInfo,
		PXSpriteFontLineCommon,
		PXSpriteFontLinePage,
		PXSpriteFontLineCharacterCount,
		PXSpriteFontLineCharacterDefinition
	}
	PXSpriteFontLineType;

	typedef struct PXSpriteFontCharacter_
	{
		unsigned int ID;
		float Position[2]; // Position of the character image in the texture.
		float Size[2];	// Size of the character image in the texture.
		float Offset[2];// Offset from the position-center.
		int XAdvance; // How much the current position should be advanced after drawing the character.
		int Page; // The texture page where the character image is found.
		int Chanal;// The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).
	}
	PXSpriteFontCharacter;

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

	typedef struct PXSpriteFontPage_
	{
		int PageID;
		PXSize CharacteListSize;
		PXSpriteFontCharacter* CharacteList;
		PXImage FontTextureMap;
	}
	PXSpriteFontPage;

	typedef struct PXSpriteFont_
	{
		PXSpriteFontInfo Info;
		PXSpriteFontCommonData CommonData;
		PXSize FontPageListSize;
		PXSpriteFontPage* FontPageList;
	}
	PXSpriteFont;

	PXPublic void PXSpriteFontConstruct(PXSpriteFont* const pxSpriteFont);
	PXPublic void PXSpriteFontDestruct(PXSpriteFont* const pxSpriteFont);

	PXPublic PXSpriteFontCharacter* PXSpriteFontGetCharacter(PXSpriteFont* pxSpriteFont, const wchar_t character);

	PXPublic PXActionResult PXSpriteFontParse(PXSpriteFont* const pxSpriteFont, PXFile* const PXFile);

	PXPrivate PXSpriteFontLineType PeekLineType(const void* line, const PXSize fileDataSize);

	PXPublic void PXSpriteFontPrtinf(const PXSpriteFont* pxSpriteFont);

#ifdef __cplusplus
}
#endif

#endif