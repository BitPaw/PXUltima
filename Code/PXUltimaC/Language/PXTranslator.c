#include "PXTranslator.h"

#include <Media/PXText.h>

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

PXActionResult PXTranslatorEnglishToGerman(PXFile* const inputStreamEnglish, PXFile* const outputStreamGerman)
{
    while (!PXFileIsAtEnd(inputStreamEnglish))
    {
        char* text = PXFileCursorPosition(inputStreamEnglish);
        const PXSize textSize = PXFileSkipBlock(inputStreamEnglish);

        char buffer[1024];

        PXTextCopyA(text, textSize, buffer, 1024u);
    }

    return PXActionSuccessful;
}

PXActionResult PXTranslatorGermanToEnglish(PXFile* const inputStreamGerman, PXFile* const outputStreamEnglish)
{
    return PXActionSuccessful;
}
