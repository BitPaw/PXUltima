#pragma once

#ifndef PXTranslatorIncluded
#define PXTranslatorIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXTranslatorEnglishToGerman(PXFile PXREF inputStreamEnglish, PXFile PXREF outputStreamGerman);
PXPublic PXResult PXAPI PXTranslatorGermanToEnglish(PXFile PXREF inputStreamGerman, PXFile PXREF outputStreamEnglish);

#endif