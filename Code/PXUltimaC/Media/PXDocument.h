#ifndef PXDocumentINCLUDE
#define PXDocumentINCLUDE

#include <Media/PXResource.h>
#include <OS/File/PXFile.h>

typedef enum PXDocumentElementType_
{
	PXDocumentElementTypeInvalid,

	PXDocumentElementTypeUnkown,

	PXDocumentElementTypeFile, // Context of a compiled file, used as a global root
	PXDocumentElementTypeNamespace, // namespace
	PXDocumentElementTypeTypeDefinition, // typedef 
	PXDocumentElementTypeStruct, // struct
	PXDocumentElementTypeUnion, // union
	PXDocumentElementTypeEnum, // enum
	PXDocumentElementTypeClass, // class
	PXDocumentElementTypeFunction, // xxx Function();
	PXDocumentElementTypeFunctionPointer, // xxx (*XXXX)();
	PXDocumentElementTypeClassMember,
	PXDocumentElementTypeClassAttribute,
	PXDocumentElementTypeEnumMember,
	PXDocumentElementTypeFunctionParameter,



	PXDocumentElementTypeAutomaticType,

	// Preprocessor
	PXDocumentElementTypePreprocessorDefine,
	PXDocumentElementTypePreprocessorIfNotDefined,
	PXDocumentElementTypePreprocessorIfDefined,
	PXDocumentElementTypePreprocessorInclude, // includes/dependecny on another file
	PXDocumentElementTypePreprocessorPragma,
	PXDocumentElementTypePreprocessorDefinitionEnd,
	PXDocumentElementTypePreprocessorValue,
	PXDocumentElementTypePreprocessorFunction,

	// Switch
	PXDocumentElementTypeSwitch,
	PXDocumentElementTypeCase,
	PXDocumentElementTypeDefault,
	PXDocumentElementTypeBreak,

	// Type additives
	PXDocumentElementTypeFinal,
	PXDocumentElementTypeSigned,
	PXDocumentElementTypeUnsigned,
	PXDocumentElementTypeConst,
	PXDocumentElementTypeRegister,
	PXDocumentElementTypeVolitile,
	PXDocumentElementTypeRestrict,

	// Types
	PXDocumentElementTypeChar,
	PXDocumentElementTypeShort,
	PXDocumentElementTypeInt,
	PXDocumentElementTypeLong,
	PXDocumentElementTypeFloat,
	PXDocumentElementTypeDouble,
	PXDocumentElementTypeBool,
	PXDocumentElementTypeTrue,
	PXDocumentElementTypeFalse,

	// Loops
	PXDocumentElementTypeWhile,
	PXDocumentElementTypeDo,
	PXDocumentElementTypeFor,
	PXDocumentElementTypeContinue,

	// Branches
	PXDocumentElementTypeIf,
	PXDocumentElementTypeElse,
	PXDocumentElementTypeGoTo,


	PXDocumentElementTypeStatic,
	PXDocumentElementTypeExtern,

	PXDocumentElementTypeReturn,
	PXDocumentElementTypeVoid,

	PXDocumentElementTypeSizeOfType // sizeof()
}
PXDocumentElementType;

typedef enum PXDocumentCallingConvention_
{
	PXDocumentCallingConventionInvalid,

	// [cdecl] Default for most C-Compilers
	// Pushed on the stack right-to-left order
	// The caller cleans the stack after the function call returns.
	PXDocumentCallingConventionCDeclaration,

	// [syscall] Same as [cdecl] but mostly just used by OS/2 32-Bit
	PXCCallingConventionSystemCall,

	// [optlink] used by the IBM VisualAge compilers]
	PXCCallingConventionOptlink,

	// []
	PXCCallingConventionPascal,

	// [fastcall]
	PXDocumentCallingConventionFastCall,

	// [register]
	PXCCallingConventionRegister,

	// [stdcall]
	PXDocumentCallingConventionStandardCall,

	// [vectorcall]
	PXCCallingConventionVectorCall,

	// [safecall]
	PXCCallingConventionSafeCall,

	// [thiscall]
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


	char* NameSpaceAdress;
	PXSize NameSpaceSize;
	char* NameClassAdress;
	PXSize NameClassSize;
	char* NameShortAdress;
	PXSize NameShortSize;


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

	// only used if type is not build in
	char* TypeNameAdress;
	PXSize TypeNameSize;


	// Functions
	// return type


	PXBool IsTypeDefinition; // in C, is typedef used?
	PXBool IsPreDefine; // In C, a typedef is used without a body.


	PXDocumentCallingConvention FunctionCallingConvention;


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


PXDocumentElementType PXAPI PXDocumentElementAnalyseElement(const char* name, const PXSize nameSize);


PXPublic const char* PXAPI PXDocumentElementTypeToString(const PXDocumentElementType pxDocumentElementType);

// Add entry to be consumed later.
// if entry is already registered, this will update the value
// Returns the newly generated object
PXPublic PXCodeDocumentElement* PXAPI PXCodeDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);

PXPublic PXActionResult PXAPI PXCodeDocumentElementGenerateChild
(
	PXCodeDocument* const pxDocument,
	PXDocumentElementType pxDocumentElementType,
	const PXSize depth,
	PXCodeDocumentElement** const pxDocumentElement,
	PXCodeDocumentElement* const pxDocumentElementParent
);


PXPublic void PXAPI PXCodeDocumentElementPrintSingle(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement);
PXPublic void PXAPI PXCodeDocumentElementPrintAll(PXCodeDocument* const pxDocument);




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