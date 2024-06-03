#ifndef PXCINCLUDE
#define PXCINCLUDE

#include "../PXResource.h"

#include <Compiler/PXCompiler.h>

#define MemberFieldFlagIsConstType (1 << 0)
#define MemberFieldFlagIsSigned (1 << 1)

#define MemberFieldFlagIsAdressConst (1 << 2)
#define MemberFieldFlagIsAdress (1 << 3)
#define MemberFieldFlagVolatile (1 << 4)
#define MemberFieldFlagRegister (1 << 5)
#define MemberFieldFlagResticted (1 << 6)
#define MemberFieldFlagIsKnownPrimitive (1 << 7)


// Parsing functions
PXPrivate PXActionResult PXAPI PXCParsePreprocessorCondition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXActionResult PXAPI PXCParsePreprocessorDefine(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXActionResult PXAPI PXCParsePreprocessorInclude(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXActionResult PXAPI PXCParsePreprocessorPragma(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXActionResult PXAPI PXCParseTypeDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

PXPrivate PXActionResult PXAPI PXCParseEndOfCommand(PXCompiler* const pxCompiler);

PXPrivate PXActionResult PXAPI PXCParseTypeParameterList(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXActionResult PXAPI PXCParseFunctionDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

PXPrivate PXActionResult PXAPI PXCParseTypeDeclarationElement(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parent);

// Transform a name with XxxxYyyyZzzzz() into its components like namespace class and function
// This only works if the naming convention of Object-Oriented C is followed.
// Most APIs use this as a given standard but some dont follow this defacto standard
PXPrivate void PXAPI PXCNameCleave(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement);

PXPrivate PXActionResult PXAPI PXCParseEnumMember(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

// Parse struct, unions and enums
PXPrivate PXActionResult PXAPI PXCParseTypeContainer(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement);
PXPrivate PXActionResult PXAPI PXCParseTypeEnum(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElementParent);

PXPublic PXActionResult PXAPI PXCLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXCSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif