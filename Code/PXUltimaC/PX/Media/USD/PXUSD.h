#ifndef PXUSDINCLUDE
#define PXUSDINCLUDE

#include "../PXResource.h"

typedef struct PXUSDA_
{
    PXF32 Version;
}
PXUSDA;










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
    PXF64 OffsetStart;
    PXF64 BlockSize;

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

    PXF64 TOCOffset;
    PXF64 TOCSectionsAmount;
    PXTOCSection* TOCSectionList;
}
PXUSDC;

typedef struct PXUSD_
{
    union
    {    
        PXUSDA Text;
        PXUSDC Binary;
    };
}
PXUSD;




#define PXUSDEntryXFORM         (1<<0)
#define PXUSDEntryKindAssembly  (1<<1)
#define PXUSDEntryGroup         (1<<2)

typedef struct PXUSDEntry
{
    char Name[32];

    PXVector3F Position;
    PXVector3F Rotation;

    PXInt32U Flags;
}
PXUSDEntry;


PXPrivate PXActionResult PXAPI PXUSDCSectionTokensLoad(PXFile* const pxFile, PXTOCSectionTokens* const pxTOCSectionTokens);
PXPrivate PXActionResult PXAPI PXUSDCSectionStringsLoad(PXFile* const pxFile, PXTOCSectionStrings* const pxTOCSectionStrings);
PXPrivate PXActionResult PXAPI PXUSDCSectionFields(PXFile* const pxFile, PXTOCSectionFields* const pxTOCSectionFields);
PXPrivate PXActionResult PXAPI PXUSDCSectionFieldSets(PXFile* const pxFile, PXTOCSectionFieldSets* const pxTOCSectionFieldSets);
PXPrivate PXActionResult PXAPI PXUSDCSectionSpecs(PXFile* const pxFile, PXTOCSectionSpecs* const pxTOCSectionSpecs);
PXPrivate PXActionResult PXAPI PXUSDCSectionPaths(PXFile* const pxFile, PXTOCSectionPaths* const pxTOCSectionPaths);

PXPrivate PXActionResult PXAPI PXUSDALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPrivate PXActionResult PXAPI PXUSDCLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPrivate PXActionResult PXAPI PXUSDZLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);

PXPublic PXActionResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif