#ifndef PXTranslatorIncluded
#define PXTranslatorIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXTranslatorEnglishToGerman(PXFile* const inputStreamEnglish, PXFile* const outputStreamGerman);
PXPublic PXResult PXAPI PXTranslatorGermanToEnglish(PXFile* const inputStreamGerman, PXFile* const outputStreamEnglish);

#endif