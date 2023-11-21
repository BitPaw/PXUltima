#ifndef PXDocumentINCLUDE
#define PXDocumentINCLUDE

#include <OS/File/PXFile.h>
#include <Media/PXColor.h>


#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXDocumentElementType_
	{
		PXDocumentElementTypeInvalid,
		PXDocumentElementTypeIncludeGlobal,
		PXDocumentElementTypeIncludePath,
		PXDocumentElementTypeNamespace,
		PXDocumentElementTypeStruct,
		PXDocumentElementTypeClass,
		PXDocumentElementTypeFunction
	}
	PXDocumentElementType;

	typedef struct PXDocumentElement_
	{
		PXDocumentElementType Type;
	}
	PXDocumentElement;

	typedef struct PXDocument_
	{
		int Dummy;
	}
	PXDocument;


#ifdef __cplusplus
}
#endif

#endif