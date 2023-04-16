#ifndef PXCINCLUDE
#define PXCINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <File/PXDataStream.h>
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
	PXPublic PXBool PXCElementClear(PXCElement* const pxCElement);


	PXPublic CKeyWord PXCFileAnalyseElement(const char* name, const PXSize nameSize);


	PXPublic PXBool PXCFileParseTypedef(PXDataStream* const inputStream, PXDataStream* const outputStream);
	PXPublic PXBool PXCFileParseStructure(PXDataStream* const inputStream, PXDataStream* const outputStream, const CKeyWord structureType, const PXBool isTypeDefitinition);
	PXPublic PXBool PXCFileParseDeclaration(PXDataStream* const inputStream, PXDataStream* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);
	PXPublic PXBool PXCFileParseFunctionPrototype(PXDataStream* const inputStream, PXDataStream* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);

	PXPublic PXActionResult PXCFileLexicalAnalysis(PXDataStream* const inputStream, PXDataStream* const outputStream);
	PXPublic PXActionResult PXCFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream);


	PXPublic void PXCElementExtract(PXDataStream* const inputStream, PXCElement* const pxCElement);



#ifdef __cplusplus
}
#endif

#endif