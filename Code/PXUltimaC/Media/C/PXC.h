#ifndef PXCINCLUDE
#define PXCINCLUDE

#include "../PXResource.h"

#include <Compiler/PXCompiler.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MemberFieldFlagIsConstType (1 << 0)
#define MemberFieldFlagIsSigned (1 << 1)

#define MemberFieldFlagIsAdressConst (1 << 2)
#define MemberFieldFlagIsAdress (1 << 3)
#define MemberFieldFlagVolatile (1 << 4)
#define MemberFieldFlagRegister (1 << 5)
#define MemberFieldFlagResticted (1 << 6)
#define MemberFieldFlagIsKnownPrimitive (1 << 7)

	typedef enum CKeyWord_
	{
		CKeyWordInvalid,

		CKeyWordUnkown,
	
		CKeyWordAutomaticType,

		// Preprocessor
		CKeyWordDefine,
		CKeyWordIfNotDefined,
		CKeyWordIfDefined,
		CKeyWordInclude,
		CKeyWordPragma,
		CKeyWordDefinitionEnd,
	

		// Switch
		CKeyWordSwitch,
		CKeyWordCase,
		CKeyWordDefault,
		CKeyWordBreak,

		// Type additives
		CKeyWordSigned,
		CKeyWordUnsigned,
		CKeyWordConst,
		CKeyWordRegister,
		CKeyWordVolitile,
		CKeyWordRestrict,

		// Types
		CKeyWordChar,
		CKeyWordShort,
		CKeyWordInt,
		CKeyWordLong,
		CKeyWordFloat,
		CKeyWordDouble,
		CKeyWordBool,
		CKeyWordTrue,
		CKeyWordFalse,

		// Loops
		CKeyWordWhile,
		CKeyWordDo,
		CKeyWordFor,
		CKeyWordContinue,

		// Branches
		CKeyWordIf,
		CKeyWordElse,
		CKeyWordGoTo,

		// Structure
		CKeyWordTypeDefinition,
		CKeyWordEnum,
		CKeyWordStruct,
		CKeyWordUnion,


		CKeyWordStatic,
		CKeyWordExtern,

		CKeyWordReturn,
		CKeyWordVoid,
		
		CKeyWordTypeSize
	}
	CKeyWord;


	typedef enum PXCStructureType_
	{
		PXCStructureTypeInvalid,

		PXCStructureTypeMakroFlag,
		PXCStructureTypeMakroValue,
		PXCStructureTypeMakroFunction,
		PXCStructureTypeMakroInlcude,

		PXCStructureTypeEnum,
		PXCStructureTypeStruct,
		PXCStructureTypeUnion,
		PXCStructureTypeStructElement,
		PXCStructureTypeFuntion
	}
	PXCStructureType;








	// Member/Field/Variable in a struct or union
	typedef struct PXCStructureVariable_
	{
		char* NameOfType;
		PXInt8U NameOfTypeSizeCurrent;
		PXInt8U NameOfTypeSizeMaximal;

		PXBool IsKnownPrimitve;
		PXBool IsConstantType;
		PXBool IsInRegister;

		// Variable only
		PXBool IsSigned;

		// Pointers only
		PXBool IsAdress;
		PXBool IsAdressConstant;
		PXBool IsAdressVolitile;
		PXBool IsAdressRestricted;

		CKeyWord PrimitiveType; // Only set, if we know it
		PXInt16U SizeOfType;

	
	}
	PXCStructureVariable;


	typedef struct PXCStructure_
	{
		char* NameAlias;
		PXInt8U NameAliasSizeCurrent;
		PXInt8U NameAliasSizeMaximal;

		PXInt16U MemberOffsetCurrent;
		PXInt16U MemberAmount;

		PXInt64U FullSize;
	}
	PXCStructure;

	typedef enum PXCAccessModifier_
	{
		PXCAccessModifierInvalid,
		PXCAccessModifierPublic, // extern
		PXCAccessModifierLocale, // static
	}
	PXCAccessModifier;

	typedef enum PXCLibraryPathType_
	{
		PXCLibraryPathTypeInvalid,
		PXCLibraryPathTypeGlobal, // <xxxx.h>
		PXCLibraryPathTypeLocale, // "xxxx.h"
	}
	PXCLibraryPathType;


	typedef enum PXCCallingConvention_
	{
		PXCCallingConventionInvalid,

		// [cdecl] Default for most C-Compilers
		// Pushed on the stack right-to-left order
		// The caller cleans the stack after the function call returns.
		PXCCallingConventionCDeclaration,

		// [syscall] Same as [cdecl] but mostly just used by OS/2 32-Bit
		PXCCallingConventionSystemCall,

		// [optlink] used by the IBM VisualAge compilers]
		PXCCallingConventionOptlink,

		// []
		PXCCallingConventionPascal,

		// [fastcall]
		PXCCallingConventionFastCall,

		// [register]
		PXCCallingConventionRegister,

		// [stdcall]
		PXCCallingConventionStandardCall,

		// [vectorcall]
		PXCCallingConventionVectorCall,

		// [safecall]
		PXCCallingConventionSafeCall,

		// [thiscall]
		PXCCallingConventionThisCall
	}
	PXCCallingConvention;

	typedef struct PXCFunction_
	{
		PXCCallingConvention CallingConvention;
		PXCAccessModifier AccessModifier;

		PXInt8U ParameterListSize;
	}
	PXCFunction;

	typedef struct PXCMakroInclucde_
	{
		PXCLibraryPathType PathType;
	}
	PXCMakroInclucde;

	typedef struct PXCMakroFunction_
	{
		int dummy;
	}
	PXCMakroFunction;

	typedef struct PXCElement_
	{
		PXCStructureType Type;

		PXBool IsTypeDefinition;

		char* Name;
		PXInt8U NameSizeCurrent;
		PXInt8U NameSizeMaximal;

		union
		{
			PXCStructure ElementStructure;
			PXCStructureVariable ElementVariable;
			PXCFunction ElementFunction;
			PXCMakroInclucde ElementInclucde;
		};
	}
	PXCElement;

	PXPublic PXBool PXCElementHasName(PXCElement* const pxCElement);
	PXPublic void PXCElementClear(PXCElement* const pxCElement);


	PXPublic CKeyWord PXCFileAnalyseElement(const char* name, const PXSize nameSize);


	PXPublic PXBool PXCFileParseTypedef(PXFile* const inputStream, PXFile* const outputStream);
	PXPublic PXBool PXCFileParseStructure(PXFile* const inputStream, PXFile* const outputStream, const CKeyWord structureType, const PXBool isTypeDefitinition);
	PXPublic PXBool PXCFileParseDeclaration(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);
	PXPublic PXBool PXCFileParseFunctionPrototype(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);


	// Parsing functions
	PXPrivate PXActionResult PXAPI PXCParsePreprocessorCondition(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParsePreprocessorDefine(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParsePreprocessorInclude(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParsePreprocessorPragma(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParseTypeDefinition(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPrivate PXActionResult PXAPI PXCParseEndOfCommand(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPrivate PXActionResult PXAPI PXCParseTypeParameterList(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParseFunctionDefinition(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPrivate PXActionResult PXAPI PXCParseTypeDeclarationElement(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParseTypeDeclarationFull(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPrivate PXActionResult PXAPI PXCParseTypeContainer(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPrivate PXActionResult PXAPI PXCParseTypeEnum(PXDocument* const pxDocument, PXFile* const pxFile);

	PXPublic PXActionResult PXAPI PXCLoadFromFile(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXCSaveToFile(PXDocument* const pxDocument, PXFile* const pxFile);


	PXPublic void PXCElementExtract(PXFile* const inputStream, PXCElement* const pxCElement);



#ifdef __cplusplus
}
#endif

#endif