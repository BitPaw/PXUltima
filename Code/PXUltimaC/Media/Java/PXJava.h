#ifndef PXJavaIncluded
#define PXJavaIncluded

#include "../PXResource.h"

typedef struct PXJava_
{
	int Value;
}
PXJava;


PXPrivate void PXAPI PXJavaComment(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);

PXPrivate void PXAPI PXJavaIncludeWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXJavaContainerWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXJavaFunctionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);

PXPrivate void PXAPI PXJavaDefinitionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXJavaParameterList(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXJavaElementWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);


PXPublic PXActionResult PXAPI PXJavaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXJavaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif