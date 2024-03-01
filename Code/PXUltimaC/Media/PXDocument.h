#ifndef PXDocumentINCLUDE
#define PXDocumentINCLUDE

#include <Media/PXResource.h>
#include <OS/File/PXFile.h>

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
		PXDocumentElementTypeFunction,
		PXDocumentElementTypeClassMember,
		PXDocumentElementTypeClassAttribute
	}
	PXDocumentElementType;

	typedef struct PXDocumentElementClass_
	{
		char* AliasAdress;
		PXSize AliasSize;

		PXBool IsTerminateSignal;
	}
	PXDocumentElementClass;

	typedef struct PXDocumentElementMember_
	{
		char* ValueAdress;
		PXSize ValueSize;
		PXInt32U ValueType;
	}
	PXDocumentElementMember;

	// This containeer needs to hold data for uses like 
	// - reflections, Namespace, Class, MemberName, MemberType, Value 
	typedef struct PXDocumentElement_
	{
		PXSize ID;
		PXSize Depth;
		
		char* NameAdress;
		PXSize NameSize;

		// Attributes
		PXSize AttribtesAmount;

		// Members
		PXSize MemberAmount;

		PXSize ElementChildrenAmount;
		PXSize ElementSiblingsAmount;

		union
		{
			PXDocumentElementMember ElementMember;
			PXDocumentElementClass ElementClass;
		};

		PXSize ParaentDataPosition;

		PXDocumentElementType Type;
	}
	PXDocumentElement;

	typedef struct PXDocument_
	{
		PXFile Data;

		PXSize AmountOfEntrys; // How many do we have?
		PXSize Depth;


		//-------------------
		// Current State
		//-------------------
		PXSize LastCounter;
		PXInt32U LastEntryOffset;
		PXInt32U LastEntryDepth;
	}
	PXDocument;


	PXPublic PXActionResult PXAPI PXDocumentElementRoot(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);
	PXPublic PXActionResult PXAPI PXDocumentElementChildGet(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);
	PXPublic PXActionResult PXAPI PXDocumentElementSiblingGet(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);


	PXPublic PXActionResult PXAPI PXDocumentElementAttributeAdd(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);


	PXPublic PXSize PXAPI PXDocumentElementIO(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction);

	PXPublic PXActionResult PXAPI PXDocumentElementWrite(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);
	PXPublic PXActionResult PXAPI PXDocumentElementRead(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);

	PXPublic PXActionResult PXAPI PXDocumentElementAdd(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement);

	PXPublic PXActionResult PXAPI PXDocumentPrint(PXDocument* const pxDocument);

	PXPublic PXActionResult PXAPI PXDocumentPrintNode(PXDocumentElement* const pxDocumentElement);


#ifdef __cplusplus
}
#endif

#endif