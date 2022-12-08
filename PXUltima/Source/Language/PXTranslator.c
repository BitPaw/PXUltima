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

ActionResult PXTranslatorEnglishToGerman(DataStream* const inputStreamEnglish, DataStream* const outputStreamGerman)
{
	while (!DataStreamIsAtEnd(inputStreamEnglish))
	{
		char* text = DataStreamCursorPosition(inputStreamEnglish);
		const PXSize textSize = DataStreamSkipBlock(inputStreamEnglish);

		char buffer[1024];

		TextCopyA(text, textSize, buffer, 1024u);		
	}

	return ActionSuccessful;
}

ActionResult PXTranslatorGermanToEnglish(DataStream* const inputStreamGerman, DataStream* const outputStreamEnglish)
{
	return ActionSuccessful;
}
