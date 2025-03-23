#ifndef PXTranslatorINCLUDE
#define PXTranslatorINCLUDE

#include <PX/OS/Error/PXActionResult.h>
#include <PX/Media/PXType.h>
#include <PX/OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

PXPublic PXActionResult PXTranslatorEnglishToGerman(PXFile* const inputStreamEnglish, PXFile* const outputStreamGerman);
PXPublic PXActionResult PXTranslatorGermanToEnglish(PXFile* const inputStreamGerman, PXFile* const outputStreamEnglish);

#ifdef __cplusplus
}
#endif

#endif
