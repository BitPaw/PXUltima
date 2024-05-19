#ifndef PXDocumentINCLUDE
#define PXDocumentINCLUDE

#include <Media/PXResource.h>
#include <OS/File/PXFile.h>

typedef enum PXDocumentElementType_
{
	PXDocumentElementTypeInvalid,
	PXDocumentElementTypeInclude,
	PXDocumentElementTypeNamespace,
	PXDocumentElementTypeStruct,
	PXDocumentElementTypeUnion,
	PXDocumentElementTypeEnum,
	PXDocumentElementTypeClass,
	PXDocumentElementTypeFunction,
	PXDocumentElementTypeClassMember,
	PXDocumentElementTypeClassAttribute,
	PXDocumentElementTypeEnumMember,
	PXDocumentElementTypeFunctionParameter,
}
PXDocumentElementType;

typedef struct PXDocumentElementClass_
{
	PXBool IsTerminateSignal; // Whats this??
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
typedef struct PXCodeDocumentElement_
{
	// General name of the enum, class, struct
	char* NameAdress;
	PXSize NameSize;

	// Used in C for typedefs
	char* AliasAdress;
	PXSize AliasSize;

	char* CommentAdress;
	PXSize CommentSize;

	// Datatype
	PXBool IsArray;
	PXSize ArrayAmount;

	PXBool DataTypeIsAdress;
	PXBool DataTypeIsBuildIn;
	PXSize DataType; // What type is the data representing, for functions this is the return type
	PXSize DataTypeSize;


	// Functions
	// return type


	PXBool IsTypeDefinition; // in C, is typedef used?
	PXBool IsPreDefine; // In C, a typedef is used without a body.



	// managed
	PXSize ID;
	PXSize Depth;

	struct PXCodeDocumentElement_* ElementParent;


	// include
	PXBool IsGlobal; // In C, Global=<xxxx> local="xxxx"



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
PXCodeDocumentElement;

typedef struct PXCodeDocument_
{
	PXFile Data;


	PXSize Depth;

	PXSize ElementListAmount; // How many do we have?
	PXCodeDocumentElement* ElementList;

	PXCodeDocumentElement* ElementRoot;

	//-------------------
	// Current State
	//-------------------
	PXSize LastCounter;
	PXInt32U LastEntryOffset;
	PXInt32U LastEntryDepth;
}
PXCodeDocument;


PXPublic const char* PXAPI PXDocumentElementTypeToString(const PXDocumentElementType pxDocumentElementType);

PXPublic PXActionResult PXAPI PXDocumentElementRoot(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementChildGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementSiblingGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);


PXPublic PXActionResult PXAPI PXDocumentElementAttributeAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);


PXPublic PXSize PXAPI PXDocumentElementIO(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction);

PXPublic PXActionResult PXAPI PXDocumentElementWrite(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementRead(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXDocumentPrint(PXCodeDocument* const pxDocument);

PXPublic PXActionResult PXAPI PXDocumentPrintNode(PXCodeDocumentElement* const pxDocumentElement);

#endif