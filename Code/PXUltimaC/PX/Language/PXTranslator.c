#include "PXTranslator.h"

#include <PX/Media/PXText.h>
#include <PX/OS/File/PXFile.h>

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

PXResult PXAPI PXTranslatorEnglishToGerman(PXFile PXREF inputStreamEnglish, PXFile PXREF outputStreamGerman)
{
    while (!PXFileIsAtEnd(inputStreamEnglish))
    {
        char* text = PXFileDataAtCursor(inputStreamEnglish);
        const PXSize textSize = PXFileSkipBlock(inputStreamEnglish);

        char buffer[1024];

        PXTextCopyA(text, textSize, buffer, 1024u);
    }

    return PXResultOK;
}

PXResult PXAPI PXTranslatorGermanToEnglish(PXFile PXREF inputStreamGerman, PXFile PXREF outputStreamEnglish)
{
    return PXResultOK;
}