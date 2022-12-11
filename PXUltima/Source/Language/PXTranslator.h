#ifndef PXTranslatorINCLUDE
#define PXTranslatorINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif
   
	PXPublic ActionResult PXTranslatorEnglishToGerman(PXDataStream* const inputStreamEnglish, PXDataStream* const outputStreamGerman);
	PXPublic ActionResult PXTranslatorGermanToEnglish(PXDataStream* const inputStreamGerman, PXDataStream* const outputStreamEnglish);

#ifdef __cplusplus
}
#endif

#endif