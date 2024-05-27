#ifndef PXCSINCLUDE
#define PXCSINCLUDE

#include "../PXResource.h"

#include <Media/C/PXC.h>

typedef enum PXCSKeyWord_
{
	PXCSKeyWordInvalid,

	PXCSKeyWordNameSpace,

	PXCSKeyWordStruct,
	PXCSKeyWordClass,

	PXCSKeyWordVolatile,

	PXCSKeyWordEE
}
PXCSKeyWord;

typedef struct PXCTranslateStruct_
{
	PXCElement Element;

	PXBool AreInUnion;

	PXInt16U StructureOffsetTotal;

	PXInt16U UnionStartOffset;
	PXInt16U UnionWidthOffset;
}
PXCTranslateStruct;

PXPublic PXCSKeyWord PXCSFetchNext(PXFile* const inputSteam);

PXPublic void PXCSSerialize(PXFile* const inputSteam, PXFile* const outputStream, PXCTranslateStruct* const pxCTranslateStruct);

PXPublic void PXCSCreateWrapperFromCSource(PXFile* const inputSteam, PXFile* const outputStream);


PXPrivate void PXAPI PXCSharpComment(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);

PXPrivate void PXAPI PXCSharpIncludeWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXCSharpContainerWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXCSharpFunctionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);

PXPrivate void PXAPI PXCSharpDefinitionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXCSharpParameterList(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);
PXPrivate void PXAPI PXCSharpElementWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile);


PXPublic PXActionResult PXAPI PXCSLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXCSSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif