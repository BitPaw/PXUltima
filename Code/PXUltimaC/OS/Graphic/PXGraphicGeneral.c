#include "PXGraphicGeneral.h"

PXInt8U PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
{
	switch (pxVertexBufferFormat)
	{
		case PXVertexBufferFormatT2F_XYZ: return 2u + 3u;
		case PXVertexBufferFormatT2F_N3F_XYZ: return 2u + 3u + 3u;
		case PXVertexBufferFormatXYZ: return 3u;

		default:
			return 0u;
	}
}

PXFontPageCharacter* PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID)
{
	PXFontPageCharacter* lastMatch = PXNull;
	PXBool match = PXFalse;

	if (!pxFontPage)
	{
		return PXNull;
	}

	for (PXSize i = 0; i < pxFontPage->CharacteListSize && !match; ++i)
	{
		const PXFontPageCharacter* const currentCharacter = &pxFontPage->CharacteList[i];

		match = currentCharacter->ID == characterID;

		lastMatch = (PXFontPageCharacter*)((PXSize)match * (PXSize)currentCharacter);
	}

    return lastMatch;
}