#pragma once

#ifndef PXUSDIncluded
#define PXUSDIncluded

#include <PX/Engine/PXResource.h>


typedef struct PXTOCSectionTokens_
{
    PXI64U NumberOfTokens;    // 
    PXI64U SizeUncompressed;  // Must be bigger than 3 and (3 + NumberOfTokens)
    PXI64U SizeCompressed;    // Compressed with LZ4
    char* TokenListCompressed;  // List of NULL terminated strings
}
PXTOCSectionTokens;

typedef struct PXTOCSectionStrings_
{
    PXI64U AmountOFElemets;
    PXI32U* Data;
}
PXTOCSectionStrings;

typedef struct PXTOCSectionFields_
{
    PXI64U AmountOFFields;

    PXI32U* FieldList;
}
PXTOCSectionFields;

typedef struct PXTOCSectionFieldSets_
{
    PXI64U AmountOFFieldsSets;
}
PXTOCSectionFieldSets;

typedef struct PXTOCSectionSpecs_
{
    PXI64U AmountOFFieldsSpecs;
}
PXTOCSectionSpecs;

typedef struct PXTOCSectionPaths_
{
    PXI64U AmountOfPaths;
}
PXTOCSectionPaths;


typedef struct PXTOCSection_
{
    char Name[16];
    PXI64U OffsetStart;
    PXI64U BlockSize;

    union
    {
        char Data[1];
        PXTOCSectionTokens Tokens;
        PXTOCSectionStrings Strings;
        PXTOCSectionFields Fields;
        PXTOCSectionFieldSets FieldSets;
        PXTOCSectionSpecs Specs;
        PXTOCSectionPaths Paths;
    };
}
PXTOCSection;


#define PXUSDSectionIDUnkown    (1<<0)
#define PXUSDSectionIDTokens    (1<<1)
#define PXUSDSectionIDStrings   (1<<2)
#define PXUSDSectionIDFields    (1<<3)
#define PXUSDSectionIDFieldSets (1<<4)
#define PXUSDSectionIDSpecs     (1<<5)
#define PXUSDSectionIDPaths     (1<<6)

typedef struct PXUSDC_
{
    PXF32 Version;

    // TOC
    PXSize TOCSectionCounterTokens;
    PXSize TOCSectionCounterPaths;
    PXSize TOCSectionCounterStrings;
    PXSize TOCSectionCounterFields;
    PXSize TOCSectionCounterFieldsets;
    PXSize TOCSectionCounterSpecs;

    PXI64U TOCOffset;
    PXI64U TOCSectionsAmount;
    PXTOCSection* TOCSectionList;
}
PXUSDC;





#define PXUSDEntryXFORM         (1<<0)
#define PXUSDEntryKindAssembly  (1<<1)
#define PXUSDEntryGroup         (1<<2)

typedef struct PXUSDEntry
{
    char* Name;
    char* Included;

    PXF64 Position[3];
    PXF32 Rotation[3];
    PXF32 Scale[3];

    PXI8U Flags;

    PXI8U NameSize;
    PXI8U IncludedSize;
}
PXUSDEntry;

typedef struct PXUSDA_
{
    PXF32 Version;

    PXSize EntryAmount;
    PXUSDEntry* EntryList;
}
PXUSDA;

typedef struct PXUSD_
{
    union
    {    
        PXUSDA Text;
        PXUSDC Binary;
    };
}
PXUSD;


PXPrivate PXResult PXAPI PXUSDCSectionTokensLoad(PXFile PXREF pxFile, PXTOCSectionTokens PXREF pxTOCSectionTokens);
PXPrivate PXResult PXAPI PXUSDCSectionStringsLoad(PXFile PXREF pxFile, PXTOCSectionStrings PXREF pxTOCSectionStrings);
PXPrivate PXResult PXAPI PXUSDCSectionFields(PXFile PXREF pxFile, PXTOCSectionFields PXREF pxTOCSectionFields);
PXPrivate PXResult PXAPI PXUSDCSectionFieldSets(PXFile PXREF pxFile, PXTOCSectionFieldSets PXREF pxTOCSectionFieldSets);
PXPrivate PXResult PXAPI PXUSDCSectionSpecs(PXFile PXREF pxFile, PXTOCSectionSpecs PXREF pxTOCSectionSpecs);
PXPrivate PXResult PXAPI PXUSDCSectionPaths(PXFile PXREF pxFile, PXTOCSectionPaths PXREF pxTOCSectionPaths);


PXPrivate void PXAPI PXUSDParsePropertysScene(PXUSDA PXREF pxUSDA, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseElementDefine(PXUSDA PXREF pxUSDA, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryName(PXUSDA PXREF pxUSDA, PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameter(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryProperty(PXUSDA PXREF pxUSDA, PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);


PXPrivate void PXAPI PXUSDAParseXMLEndTag(PXCompiler PXREF pxCompiler, char* PXREF name, PXSize PXREF nameSize);
PXPrivate void PXAPI PXUSDAParseEntryParameterPrepend(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterVarriants(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterAssetInfo(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterPayLoad(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterKind(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);

PXPrivate void PXAPI PXUSDAParseEntryPropertyFloat1(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyFloat3(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyDouble3(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyUniform(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);


PXPrivate PXResult PXAPI PXUSDALoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPrivate PXResult PXAPI PXUSDCLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPrivate PXResult PXAPI PXUSDZLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);

PXPublic PXResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif