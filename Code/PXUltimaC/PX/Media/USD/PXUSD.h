#ifndef PXUSDINCLUDE
#define PXUSDINCLUDE

#include "../PXResource.h"


typedef struct PXTOCSectionTokens_
{
    PXInt64U NumberOfTokens;    // 
    PXInt64U SizeUncompressed;  // Must be bigger than 3 and (3 + NumberOfTokens)
    PXInt64U SizeCompressed;    // Compressed with LZ4
    char* TokenListCompressed;  // List of NULL terminated strings
}
PXTOCSectionTokens;

typedef struct PXTOCSectionStrings_
{
    PXInt64U AmountOFElemets;
    PXInt32U* Data;
}
PXTOCSectionStrings;

typedef struct PXTOCSectionFields_
{
    PXInt64U AmountOFFields;
}
PXTOCSectionFields;

typedef struct PXTOCSectionFieldSets_
{
    PXInt64U AmountOFFieldsSets;
}
PXTOCSectionFieldSets;

typedef struct PXTOCSectionSpecs_
{
    PXInt64U AmountOFFieldsSpecs;
}
PXTOCSectionSpecs;

typedef struct PXTOCSectionPaths_
{
    PXInt64U AmountOfPaths;
}
PXTOCSectionPaths;


typedef struct PXTOCSection_
{
    char Name[16];
    PXInt64U OffsetStart;
    PXInt64U BlockSize;

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

    PXInt64U TOCOffset;
    PXInt64U TOCSectionsAmount;
    PXTOCSection* TOCSectionList;
}
PXUSDC;





#define PXUSDEntryXFORM         (1<<0)
#define PXUSDEntryKindAssembly  (1<<1)
#define PXUSDEntryGroup         (1<<2)

typedef struct PXUSDEntry
{
    char* Name;
    char* Include;

    PXF64 Position[3];
    PXF32 Rotation[3];
    PXF32 Scale[3];

    PXInt8U Flags;

    PXInt8U NameSize;
    PXInt8U IncludeSize;
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


PXPrivate PXActionResult PXAPI PXUSDCSectionTokensLoad(PXFile* const pxFile, PXTOCSectionTokens* const pxTOCSectionTokens);
PXPrivate PXActionResult PXAPI PXUSDCSectionStringsLoad(PXFile* const pxFile, PXTOCSectionStrings* const pxTOCSectionStrings);
PXPrivate PXActionResult PXAPI PXUSDCSectionFields(PXFile* const pxFile, PXTOCSectionFields* const pxTOCSectionFields);
PXPrivate PXActionResult PXAPI PXUSDCSectionFieldSets(PXFile* const pxFile, PXTOCSectionFieldSets* const pxTOCSectionFieldSets);
PXPrivate PXActionResult PXAPI PXUSDCSectionSpecs(PXFile* const pxFile, PXTOCSectionSpecs* const pxTOCSectionSpecs);
PXPrivate PXActionResult PXAPI PXUSDCSectionPaths(PXFile* const pxFile, PXTOCSectionPaths* const pxTOCSectionPaths);


PXPrivate void PXAPI PXUSDParsePropertysScene(PXUSDA* const pxUSDA, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseElementDefine(PXUSDA* const pxUSDA, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryName(PXUSDA* const pxUSDA, PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameter(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryProperty(PXUSDA* const pxUSDA, PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);


PXPrivate void PXAPI PXUSDAParseXMLEndTag(PXCompiler* const pxCompiler, char** const name, PXSize* const nameSize);
PXPrivate void PXAPI PXUSDAParseEntryParameterPrepend(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterVarriants(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterAssetInfo(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterPayLoad(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryParameterKind(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);

PXPrivate void PXAPI PXUSDAParseEntryPropertyFloat1(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyFloat3(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyDouble3(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);
PXPrivate void PXAPI PXUSDAParseEntryPropertyUniform(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);


PXPrivate PXActionResult PXAPI PXUSDALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPrivate PXActionResult PXAPI PXUSDCLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPrivate PXActionResult PXAPI PXUSDZLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);

PXPublic PXActionResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif