#ifndef PXCIncluded
#define PXCIncluded

#include <PX/Engine/PXResource.h>

#include <PX/Compiler/PXCompiler.h>

#define MemberFieldFlagIsConstType (1 << 0)
#define MemberFieldFlagIsSigned (1 << 1)

#define MemberFieldFlagIsAdressConst (1 << 2)
#define MemberFieldFlagIsAdress (1 << 3)
#define MemberFieldFlagVolatile (1 << 4)
#define MemberFieldFlagRegister (1 << 5)
#define MemberFieldFlagResticted (1 << 6)
#define MemberFieldFlagIsKnownPrimitive (1 << 7)


// Parsing functions
PXPrivate PXResult PXAPI  PXCParsePreprocessorCondition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXResult PXAPI  PXCParsePreprocessorDefine(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXResult PXAPI  PXCParsePreprocessorIncluded(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXResult PXAPI  PXCParsePreprocessorPragma(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXResult PXAPI  PXCParseTypeDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

PXPrivate PXResult PXAPI  PXCParseEndOfCommand(PXCompiler* const pxCompiler);

PXPrivate PXResult PXAPI  PXCParseTypeParameterList(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);
PXPrivate PXResult PXAPI  PXCParseFunctionDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

PXPrivate PXResult PXAPI  PXCParseTypeDeclarationElement(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parent);

// Transform a name with XxxxYyyyZzzzz() into its components like namespace class and function
// This only works if the naming convention of Object-Oriented C is followed.
// Most APIs use this as a given standard but some dont follow this defacto standard
PXPrivate void PXAPI PXCNameCleave(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement);

PXPrivate PXResult PXAPI  PXCParseEnumMember(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent);

// Parse struct, unions and enums
PXPrivate PXResult PXAPI  PXCParseTypeContainer(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement);
PXPrivate PXResult PXAPI  PXCParseTypeEnum(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElementParent);

PXPublic PXResult PXAPI PXCLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
