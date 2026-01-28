#pragma once

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
PXPrivate PXResult PXAPI PXCParsePreprocessorCondition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);
PXPrivate PXResult PXAPI PXCParsePreprocessorDefine(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);
PXPrivate PXResult PXAPI PXCParsePreprocessorIncluded(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);
PXPrivate PXResult PXAPI PXCParsePreprocessorPragma(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);
PXPrivate PXResult PXAPI PXCParseTypeDefinition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);

PXPrivate PXResult PXAPI PXCParseEndOfCommand(PXCompiler PXREF pxCompiler);

PXPrivate PXResult PXAPI PXCParseTypeParameterList(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);
PXPrivate PXResult PXAPI PXCParseFunctionDefinition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);

PXPrivate PXResult PXAPI PXCParseTypeDeclarationElement(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parent);

// Transform a name with XxxxYyyyZzzzz() into its components like namespace class and function
// This only works if the naming convention of Object-Oriented C is followed.
// Most APIs use this as a given standard but some dont follow this defacto standard
PXPrivate void PXAPI PXCNameCleave(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElement);

PXPrivate PXResult PXAPI PXCParseEnumMember(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent);

// Parse struct, unions and enums
PXPrivate PXResult PXAPI PXCParseTypeContainer(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElement);
PXPrivate PXResult PXAPI PXCParseTypeEnum(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElementParent);

PXPublic PXResult PXAPI PXCLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
