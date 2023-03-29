#ifndef PXCINCLUDE
#define PXCINCLUDE

#include <Media/Type.h>
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


	typedef enum PXCToken_ PXCToken;

	typedef enum PXCToken_
	{
		PXCTokenInvalid,

		PXCTokenMakroDefintion,
		PXCTokenMakroFunction,

		PXCTokenEnumTypeDefinition,
		PXCTokenEnumVariableDefinition,

		PXCTokenStructTypeDefinition,
		PXCTokenStructVariableDefinition,

		PXCTokenUnionTypeDefinition,
		PXCTokenUnionVariableDefinition,
	}
	PXCToken;


	PXPublic CKeyWord PXCFileAnalyseElement(const char* name, const PXSize nameSize);


	PXPublic PXBool PXCFileParseTypedef(PXDataStream* const inputStream, PXDataStream* const outputStream);
	PXPublic PXBool PXCFileParseStructure(PXDataStream* const inputStream, PXDataStream* const outputStream, const CKeyWord structureType);
	PXPublic PXBool PXCFileParseDeclaration(PXDataStream* const inputStream, PXDataStream* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);
	PXPublic PXBool PXCFileParseFunctionPrototype(PXDataStream* const inputStream, PXDataStream* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry);

	PXPublic PXActionResult PXCFileLexicalAnalysis(PXDataStream* const inputStream, PXDataStream* const outputStream);
	PXPublic PXActionResult PXCFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream);


#ifdef __cplusplus
}
#endif

#endif