#include "PXTranslator.h"

#include <Text/Text.h>

// English <=> German translate

// TH <=> D
// TH <=> TT
// D <=> T
// T <=> S, SS, ß
// T <=> Z
// P <=> F, FF, PF
// K <=> CH
// C <=> K
// Y <=> G
// V <=> B
// GHT <=> CHT

PXActionResult PXTranslatorEnglishToGerman(PXDataStream* const inputStreamEnglish, PXDataStream* const outputStreamGerman)
{
	while (!PXDataStreamIsAtEnd(inputStreamEnglish))
	{
		char* text = PXDataStreamCursorPosition(inputStreamEnglish);
		const PXSize textSize = PXDataStreamSkipBlock(inputStreamEnglish);

		char buffer[1024];

		TextCopyA(text, textSize, buffer, 1024u);		
	}

	return PXActionSuccessful;
}

PXActionResult PXTranslatorGermanToEnglish(PXDataStream* const inputStreamGerman, PXDataStream* const outputStreamEnglish)
{
	return PXActionSuccessful;
}
