#ifndef PXTranslatorINCLUDE
#define PXTranslatorINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/DataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif
   
	PXPublic ActionResult PXTranslatorEnglishToGerman(DataStream* const inputStreamEnglish, DataStream* const outputStreamGerman);
	PXPublic ActionResult PXTranslatorGermanToEnglish(DataStream* const inputStreamGerman, DataStream* const outputStreamEnglish);

#ifdef __cplusplus
}
#endif

#endif