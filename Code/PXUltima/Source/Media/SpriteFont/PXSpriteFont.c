#include "PXSpriteFont.h"

#include <stdio.h>

#include <Media/PXText.h>
#include <OS/File/PXFile.h>
#include <Media/PXImage.h>
#include <OS/Memory/PXMemory.h>

void PXSpriteFontConstruct(PXSpriteFont* const pxPXSpriteFont)
{
	MemoryClear(pxPXSpriteFont, sizeof(PXSpriteFont));
}

void PXSpriteFontDestruct(PXSpriteFont* const pxPXSpriteFont)
{

}

PXSpriteFontCharacter* PXSpriteFontGetCharacter(PXSpriteFont* PXSpriteFont, const wchar_t character)
{
	if(!PXSpriteFont)
	{
		return 0;
	}

	for (PXSize pageIndex = 0; pageIndex < PXSpriteFont->FontPageListSize; ++pageIndex)
	{
		const PXSpriteFontPage* page = &PXSpriteFont->FontPageList[pageIndex];

		for (PXSize i = 0; i < page->CharacteListSize; ++i)
		{
			const PXSpriteFontCharacter* bitMapFontCharacter = &page->CharacteList[i];
			const PXInt32U target = bitMapFontCharacter->ID;
			const PXBool isSameCharacter = target == character;

			if (isSameCharacter)
			{
				return bitMapFontCharacter;
			}
		}
	}

	return 0;
}

PXActionResult PXSpriteFontParse(PXSpriteFont* const PXSpriteFont, PXFile* const PXFile)
{
	PXSpriteFontPage* currentPage = 0;
	PXSize characterIndex = 0;

	PXSpriteFontConstruct(PXSpriteFont);

	while (!PXFileIsAtEnd(PXFile))
	{
		const char* currentPosition = (char*)PXFileCursorPosition(PXFile);
		const PXSize currentReadableBytes = PXFileRemainingSize(PXFile);
		const PXSpriteFontLineType lineType = PeekLineType(currentPosition, currentReadableBytes);

		switch (lineType)
		{
			case PXSpriteFontLineInfo:
			{
				const char parsingData[] = "face=\0size=\0bold=\0italic\0charset=\0unicode=\0stretchH=\0smooth=\0aa=\0padding=\0spacing=\0";
				const char* indexPosition[11];
				const ParsingTokenA parsingTokenList[] =
				{
					{parsingData + 0, &indexPosition[0]},
					{parsingData + 6, &indexPosition[1]},
					{parsingData + 12, &indexPosition[2]},
					{parsingData + 18, &indexPosition[3]},
					{parsingData + 25, &indexPosition[4]},
					{parsingData + 34, &indexPosition[5]},
					{parsingData + 43, &indexPosition[6]},
					{parsingData + 53, &indexPosition[7]},
					{parsingData + 61, &indexPosition[8]},
					{parsingData + 65, &indexPosition[9]},
					{parsingData + 74, &indexPosition[10]}
				};
				const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

				PXFileCursorAdvance(PXFile, 5u);

				PXTextParseFindAllA
				(
					PXFileCursorPosition(PXFile),
					PXFileRemainingSize(PXFile),
					parsingTokenList,
					values
				);

				PXTextCopyA(indexPosition[0] + 1, PXSpriteFontFontNameSize, PXSpriteFont->Info.Name, PXSpriteFontFontNameSize);
				PXTextToIntA(indexPosition[1], 5, &PXSpriteFont->Info.Size);
				PXTextToBoolA(indexPosition[2], 5, &PXSpriteFont->Info.Bold);
				PXTextToBoolA(indexPosition[3], 5, &PXSpriteFont->Info.Italic);
				PXTextCopyA(indexPosition[4] + 1, PXSpriteFontCharSetNameSize, PXSpriteFont->Info.CharSet, PXSpriteFontCharSetNameSize);
				PXTextToBoolA(indexPosition[5], 5, &PXSpriteFont->Info.Unicode);
				PXTextToIntA(indexPosition[6], 5, &PXSpriteFont->Info.StretchH);
				PXTextToBoolA(indexPosition[7], 5, &PXSpriteFont->Info.Smooth);
				PXTextToBoolA(indexPosition[8], 5, &PXSpriteFont->Info.Supersampling);

				PXTextTerminateBeginFromFirstA(PXSpriteFont->Info.Name, PXSpriteFontFontNameSize, '\"');
				PXTextTerminateBeginFromFirstA(PXSpriteFont->Info.CharSet, PXSpriteFontCharSetNameSize, '\"');

				break;
			}
			case PXSpriteFontLineCommon:
			{
				const char parsingData[] = "lineHeight=\0base=\0scaleW=\0scaleH=\0pages=\0packed=\0";
				const char* indexPosition[6];
				const ParsingTokenA parsingTokenList[] =
				{
					{parsingData + 0, &indexPosition[0]},
					{parsingData + 12, &indexPosition[1]},
					{parsingData + 18, &indexPosition[2]},
					{parsingData + 26, &indexPosition[3]},
					{parsingData + 34, &indexPosition[4]},
					{parsingData + 41, &indexPosition[5]}
				};
				const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

				PXFileCursorAdvance(PXFile, 6u);

				PXTextParseFindAllA
				(
					PXFileCursorPosition(PXFile),
					PXFileRemainingSize(PXFile),
					parsingTokenList,
					values
				);

				const PXSize readableSize = PXFileRemainingSize(PXFile);

				PXTextToIntA(indexPosition[0], readableSize, &PXSpriteFont->CommonData.LineHeight);
				PXTextToIntA(indexPosition[1], readableSize, &PXSpriteFont->CommonData.Base);
				PXTextToIntA(indexPosition[2], readableSize, &PXSpriteFont->CommonData.ScaleWidth);
				PXTextToIntA(indexPosition[3], readableSize, &PXSpriteFont->CommonData.ScaleHeight);
				PXTextToIntA(indexPosition[4], readableSize, &PXSpriteFont->CommonData.AmountOfPages);
				PXTextToBoolA(indexPosition[5], readableSize, &PXSpriteFont->CommonData.Packed);

				// Allocate
				{
					const PXSize size = PXSpriteFont->CommonData.AmountOfPages;
					const PXSize sizeInBytes = sizeof(PXSpriteFontPage) * size;
					PXSpriteFontPage* pageList = MemoryAllocateClear(sizeInBytes);

					PXSpriteFont->FontPageListSize = size;
					PXSpriteFont->FontPageList = pageList;

					currentPage = pageList;
				}

				break;
			}
			case PXSpriteFontLinePage:
			{
				const char parsingData[] = "id=\0file=";
				const char* indexPosition[2];
				const ParsingTokenA parsingTokenList[] =
				{
					{parsingData + 0, &indexPosition[0]},
					{parsingData + 4, &indexPosition[1]}
				};
				const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

				PXFileCursorAdvance(PXFile, 5u);

				PXTextParseFindAllA
				(
					PXFileCursorPosition(PXFile),
					PXFileRemainingSize(PXFile),
					parsingTokenList,
					values
				);

				PXTextToIntA(indexPosition[0], 5, &currentPage->PageID);


				// Loading Image
				{
					char fullPath[PathMaxSize];
					char pageFileName[PXSpriteFontPageFileNameSize];

					PXTextCopyA
					(
						indexPosition[1] + 1,
						PXFileRemainingSize(PXFile),
						pageFileName,
						PXSpriteFontPageFileNameSize - 1
					);

					PXTextTerminateBeginFromFirstA(pageFileName, PXSpriteFontPageFileNameSize, '\"');

					//FilePathSwapFileNameA(PXFile->FilePath, fullPath, pageFileName);

					const PXActionResult actionResult = PXImageLoadA(&currentPage->FontTextureMap, fullPath);
				}

				break;
			}
			case PXSpriteFontLineCharacterCount:
			{
				const char countText[] = "count=";

				PXFileCursorAdvance(PXFile, 6u);

				char* count = PXTextFindPositionA
				(
					PXFileCursorPosition(PXFile),
					PXFileRemainingSize(PXFile),
					countText,
					sizeof(countText) - 1
				);

				// Allocate
				{
					int size = 0;

					PXTextToIntA
					(
						count + sizeof(countText) - 1,
						PXFileRemainingSize(PXFile) - sizeof(countText),
						&size
					);

					const PXSize sizeInBytes = sizeof(PXSpriteFontCharacter) * size;

					currentPage->CharacteListSize = size;
					currentPage->CharacteList = MemoryAllocateClear(sizeInBytes);
				}

				characterIndex = 0;

				break;
			}
			case PXSpriteFontLineCharacterDefinition:
			{
				const unsigned char acessCharacterOutofBounce = characterIndex >= currentPage->CharacteListSize;

				if (acessCharacterOutofBounce)
				{
					const PXSize sizeCurrent = currentPage->CharacteListSize * sizeof(PXSpriteFontPage);
					const PXSize sizeNew = currentPage->CharacteListSize * sizeof(PXSpriteFontPage) + 1;

					PXSpriteFontCharacter* characteListR = MemoryHeapReallocateClear(currentPage->CharacteList, sizeCurrent, sizeNew);
					const PXBool adresschanged = characteListR != currentPage->CharacteList;

					if (!characteListR)
					{
						// Error, out of memeory
					}

					currentPage->CharacteListSize++;
					currentPage->CharacteList = characteListR;
				}

				PXSpriteFontCharacter* character = &currentPage->CharacteList[characterIndex++];

				const char parsingData[] = "id=\0x=\0y=\0width=\0height=\0xoffset=\0yoffset=\0xadvance=\0page=\0chnl=";
				const char* indexPosition[10];
				const ParsingTokenA parsingTokenList[] =
				{
					{parsingData + 0, &indexPosition[0]},
					{parsingData + 4, &indexPosition[1]},
					{parsingData + 7, &indexPosition[2]},
					{parsingData + 10, &indexPosition[3]},
					{parsingData + 17, &indexPosition[4]},
					{parsingData + 25, &indexPosition[5]},
					{parsingData + 34, &indexPosition[6]},
					{parsingData + 43, &indexPosition[7]},
					{parsingData + 53, &indexPosition[8]},
					{parsingData + 59, &indexPosition[9]}
				};
				const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

				PXFileCursorAdvance(PXFile, 5u);

				PXTextParseFindAllA
				(
					PXFileCursorPosition(PXFile),
					PXFileRemainingSize(PXFile),
					parsingTokenList,
					values
				);

				PXTextToIntA(indexPosition[0], 5, &character->ID);
				PXTextToFloatA(indexPosition[1], 5, &character->Position[0]);
				PXTextToFloatA(indexPosition[2], 5, &character->Position[1]);
				PXTextToFloatA(indexPosition[3], 5, &character->Size[0]);
				PXTextToFloatA(indexPosition[4], 5, &character->Size[1]);
				PXTextToFloatA(indexPosition[5], 5, &character->Offset[0]);
				PXTextToFloatA(indexPosition[6], 5, &character->Offset[1]);
				PXTextToIntA(indexPosition[7], 5, &character->XAdvance);
				PXTextToIntA(indexPosition[8], 5, &character->Page);
				PXTextToIntA(indexPosition[9], 5, &character->Chanal);

				break;
			}
		}

		PXFileSkipLine(PXFile);
	}

	return PXActionSuccessful;
}

PXSpriteFontLineType PeekLineType(const void* line, const PXSize fileDataSize)
{
	const unsigned char* data = line;
	const unsigned char isCommonLine = data[0] == 'c' && data[1] == 'o' && data[2] == 'm' && data[3] == 'm' && data[4] == 'o' && data[5] == 'n' && data[6] == ' ';
	const unsigned char isCharacterCountLine = data[0] == 'c' && data[1] == 'h' && data[2] == 'a' && data[3] == 'r' && data[4] == 's' && data[5] == ' ';
	const unsigned char isCharacterDeclareLine = data[0] == 'c' && data[1] == 'h' && data[2] == 'a' && data[3] == 'r' && data[4] == ' ';
	const unsigned char isInfoLine = data[0] == 'i' && data[1] == 'n' && data[2] == 'f' && data[3] == 'o' && data[4] == ' ';
	const unsigned char isPageLine = data[0] == 'p' && data[1] == 'a' && data[2] == 'g' && data[3] == 'e' && data[4] == ' ';

	unsigned code =
		isCommonLine * 1 +
		isCharacterCountLine * 2 +
		isCharacterDeclareLine * 3 +
		isInfoLine * 4 +
		isPageLine * 5;

	switch(code)
	{
		case 1:
			return PXSpriteFontLineCommon;

		case 2:
			return PXSpriteFontLineCharacterCount;

		case 3:
			return PXSpriteFontLineCharacterDefinition;

		case 4:
			return PXSpriteFontLineInfo;

		case 5:
			return PXSpriteFontLinePage;

		default:
			return PXSpriteFontLineUnkown;
	}
}

void PXSpriteFontPrtinf(const PXSpriteFont* pxSpriteFont)
{
	printf(" +-------------------------------------------------------------------------+\n");
	printf(" | Font (%s) : %s\n", &pxSpriteFont->Info.CharSet[0], &pxSpriteFont->Info.Name[0]);
	printf(" +-------------------------------------------------------------------------+\n");
	printf(" | Size     : %4u | Smooth  : %3u |\n", pxSpriteFont->Info.Size, pxSpriteFont->Info.Smooth);
	printf(" | Bold     : %4s | AA      : %3u |\n", pxSpriteFont->Info.Bold ? "Yes" : "No", pxSpriteFont->Info.Supersampling);
	printf
	(
		" | Italic   : %4s | Padding : %u,%u,%u,%u |\n",
		pxSpriteFont->Info.Italic ? "Yes" : "No",
		pxSpriteFont->Info.CharacterPadding[0],
		pxSpriteFont->Info.CharacterPadding[1],
		pxSpriteFont->Info.CharacterPadding[2],
		pxSpriteFont->Info.CharacterPadding[3]

	);
	printf(" | unicode  : %4s | Spacing : %u,%u |\n", pxSpriteFont->Info.Unicode ? "Yes" : "No", pxSpriteFont->Info.SpacerOffset[0], pxSpriteFont->Info.SpacerOffset[1]);
	printf(" | stretchH : %4u | Outline : %3u |\n", pxSpriteFont->Info.StretchH, pxSpriteFont->Info.OutlineThickness);


	for(unsigned int pageIndex = 0; pageIndex < pxSpriteFont->FontPageListSize; pageIndex++)
	{
		PXSpriteFontPage* page = &pxSpriteFont->FontPageList[pageIndex];

		//printf(" |          |       |       |       |       |       |       |       |\n");
		printf("\n");
		printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
		printf(" | Page <%zu/%zu> \n", pageIndex + 1, pxSpriteFont->FontPageListSize);
		printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
		printf(" | Letter   | X-Pos | Y-Pos | Width | Height| X-Off | Y-Off | X-Step|\n");
		printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");

		for(unsigned int characterIndex = 0; characterIndex < page->CharacteListSize; characterIndex++)
		{
			PXSpriteFontCharacter* character = &page->CharacteList[characterIndex];

			printf
			(
				" | %3i -> %c | %5.1f | %5.1f | %5.1f | %5.1f | %5.1f | %5.1f | %5i |\n",
				character->ID,
				character->ID,
				character->Position[0],
				character->Position[1],
				character->Size[0],
				character->Size[1],
				character->Offset[0],
				character->Offset[1],
				character->XAdvance
			);
		}

		printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
	}
}
