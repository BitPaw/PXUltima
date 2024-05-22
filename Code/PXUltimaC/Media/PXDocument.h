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

typedef enum PXDocumentCallingConvention_
{
	PXDocumentCallingConventionInvalid,
	PXDocumentCallingConventionCDeclaration,
	PXDocumentCallingConventionStandardCall,
	PXDocumentCallingConventionFastCall,
	PXDocumentCallingConventionThisCall

}
PXDocumentCallingConvention;

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

	struct PXCodeDocumentElement_* ElementParent; // Firect parent object
	struct PXCodeDocumentElement_* ElementSibling; // Like a linked list, contain the next object
	struct PXCodeDocumentElement_* ElementChildFirstBorn; // contain the first child object

	// include
	PXBool IsGlobal; // In C, Global=<xxxx> local="xxxx"



	// Attributes
	PXSize AttribtesAmount;

	// Members
	PXSize MemberAmount;

	//PXSize ElementChildrenAmount;
	//PXSize ElementSiblingsAmount;

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


	//PXSize Depth;

	PXSize ElementListAllocated;
	PXSize ElementListUsed; // How many do we have?
	PXCodeDocumentElement* ElementList;

	//PXCodeDocumentElement* ElementRoot;

	//-------------------
	// Current State
	//-------------------
	PXSize LastCounter;
	PXInt32U LastEntryOffset;
	PXInt32U LastEntryDepth;
}
PXCodeDocument;





PXPublic const char* PXAPI PXDocumentElementTypeToString(const PXDocumentElementType pxDocumentElementType);

// Add entry to be consumed later.
// if entry is already registered, this will update the value
// Returns the newly generated object
PXPublic PXCodeDocumentElement* PXAPI PXCodeDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXCodeDocumentElementGenerateChild
(
	PXCodeDocument* const pxDocument,
	PXDocumentElementType pxDocumentElementType,
	PXCodeDocumentElement** const pxDocumentElement,
	PXCodeDocumentElement* const pxDocumentElementParent
);




PXPublic PXActionResult PXAPI PXDocumentElementRoot(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementChildGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementSiblingGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);





PXPublic PXSize PXAPI PXDocumentElementIO(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction);

PXPublic PXActionResult PXAPI PXDocumentElementWrite(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic PXActionResult PXAPI PXDocumentElementRead(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXDocumentPrint(PXCodeDocument* const pxDocument);

PXPublic PXActionResult PXAPI PXDocumentPrintNode(PXCodeDocumentElement* const pxDocumentElement);

#endif