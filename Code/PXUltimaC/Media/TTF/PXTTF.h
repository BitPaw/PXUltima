#ifndef PXPXTTFINCLUDE
#define PXPXTTFINCLUDE

#define PXTTFDebug 0

#include "../PXResource.h"

typedef enum PXTTFTableEntryType_
{
    PXTTFTableEntryUnkown,
    PXTTFTableEntryAccentAttachment, // acnt
    PXTTFTableEntryAnchorPoint, // ankr
    PXTTFTableEntryAxisVariation, // avar
    PXTTFTableEntryBitmapData, // bdat
    PXTTFTableEntryBitmapFontHeader, // bhed
    PXTTFTableEntryBitmapLocation, // bloc
    PXTTFTableEntryBaseline, // bsln
    PXTTFTableEntryCharacterCodeMapping, // cmap
    PXTTFTableEntryCVTVariation, // cvar
    PXTTFTableEntryControlValue, // cvt
    PXTTFTableEntryEmbeddedBitmapScalingControl, // EBSC
    PXTTFTableEntryFontDescriptor, // fdsc
    PXTTFTableEntryLayoutFeature, // feat
    PXTTFTableEntryFontMetrics, // fmtx
    PXTTFTableEntryFontFamilyCompatibility, // fond
    PXTTFTableEntryFontProgram, // fpgm
    PXTTFTableEntryFontVariation, // fvar
    PXTTFTableEntryGridFittingAndScanConversionProcedure, // gasp
    PXTTFTableEntryGlyphOutline, // glyf, defines the appearance of the glyphs in the font.
    PXTTFTableEntryGlyphVariation, // gvar
    PXTTFTableEntryHorizontalDeviceMetrics, // hdmx
    PXTTFTableEntryFontHeader, // head
    PXTTFTableEntryHorizontalHeader, // hhea
    PXTTFTableEntryHorizontalMetrics, // hmtx
    PXTTFTableEntryJustification, // just
    PXTTFTableEntryKerning, // kern
    PXTTFTableEntryExtendedKerning, // kerx
    PXTTFTableEntryLigatureCaret, // lcar
    PXTTFTableEntryGlyphLocation, // loca
    PXTTFTableEntryLanguageTag, // ltag
    PXTTFTableEntryMaximumProfile,  // maxp
    PXTTFTableEntryMetadata,  // meta
    PXTTFTableEntryMetamorphosisTabledeprecated,  // mort
    PXTTFTableEntryExtendedMetamorphosis,  // morx
    PXTTFTableEntryName,  // name
    PXTTFTableEntryOpticalBounds, // opbd
    PXTTFTableEntryCompatibility, //  OS/2
    PXTTFTableEntryGlyphNameAndPostScriptCompatibility, // post
    PXTTFTableEntryControlValueProgram, // prep
    PXTTFTableEntryProperties, // prop
    PXTTFTableEntryExtendedBitmaps, // sbix
    PXTTFTableEntryTracking, // trak
    PXTTFTableEntryVerticalHeader, // vhea
    PXTTFTableEntryVerticalMetrics, // vmtx
    PXTTFTableEntryCrossReference, // xref
    PXTTFTableEntryGlyphReference, // Zapf

    // Additional
    PXTTFTableEntryDigitalSignature, // DSIG 
    PXTTFTableEntryLinearThreshold, // LTSH
    PXTTFTableEntryVerticalDeviceMetrics // VDMX
}
PXTTFTableEntryType;


typedef enum PXTTFVersionType_
{
    PXTTFVersionInvalid,
    PXTTFVersion1Dot0,
    PXTTFVersion2Dot0,
    PXTTFVersion2Dot5,
    PXTTFVersion3Dot0
}
PXTTFVersionType;

typedef struct PXTTFVersion_
{
    PXInt16U Major;
    PXInt16U Minor;
    PXTTFVersionType Type;
}
PXTTFVersion;

typedef struct PXTTFLinearThreshold_
{
    unsigned short Version; // Version number(starts at 0).
    unsigned short NumberOfGlyphs; // Number of glyphs(from “numGlyphs” in 'maxp' table).
    unsigned char* PelsHeightList; // The vertical pel height at which the glyph can be assumed to scale linearly.On a per glyph basis.
}
PXTTFLinearThreshold;


typedef struct PXTTFHorizontalHeader_
{
    PXTTFVersion Version;
    short Ascender;
    short Descender;
    short LineGap;
    unsigned short AdvanceWidthMaximum;
    short MinimumLeftSideBearing;
    short MinimumRightSideBearing;
    short MaximalExtendX;
    short CaretSlopeRise;
    short CaretSlopeRun;
    short MetricDataFormat;
    unsigned short NumberOfHorizontalMetrics;
}
PXTTFHorizontalHeader;


typedef struct PXTTFTableEntry_
{
    union
    {
        char TypeRaw[4];
        unsigned int TypeID;
    };

    PXTTFTableEntryType Type;
    unsigned int CheckSum;
    unsigned int Offset;
    unsigned int Length;
}
PXTTFTableEntry;


typedef struct PXTTFVerticalDeviceMetrics_
{
    unsigned short Version; // Version number(0 or 1).
    unsigned short numRecs; // Number of VDMX groups present
    unsigned short numRatios; // Number of aspect ratio groupings
    //	RatioRange 	ratRange[numRatios];// 	Ratio record array.
    //Offset16 	vdmxGroupOffsets[numRatios];// 	Offset from start of this table to the VDMXGroup table for a corresponding RatioRange record.
}
PXTTFVerticalDeviceMetrics;





//-----<CMAP>--------------------------------------------------------------
typedef enum PXEncodingID_
{
    PXEncodingInvalid,

    // (platform ID = 0)

    PXEncodingUnicode10,// semantics—deprecated,
    PXEncodingUnicode11,// semantics—deprecated,
    PXEncodingISOIEC10646,// semantics—deprecated,
    PXEncodingUnicode20BMPOnly,
    PXEncodingUnicode20FullRepertoire,
    PXEncodingUnicodeVariation,// Sequences—for use with subtable format 14,
    PXEncodingUnicodefull,// repertoire—for use with subtable format 13,

    // Macintosh platform (platform ID = 1)

    // (platform ID = 2)

    PXEncodingSevenBitASCII,
    PXEncodingISO10646,
    PXEncodingISO8858,

    // Windows platform (platform ID = 3)
    PXEncodingSymbol,
    PXEncodingUnicodeBMP,
    PXEncodingShiftJIS,
    PXEncodingPRC,
    PXEncodingBig5,
    PXEncodingWansung,
    PXEncodingJohab,
    PXEncodingReserved,
    PXEncodingUnicodeFullRepertoire,

    // Custom platform (platform ID = 4) and OTF Windows NT compatibility mapping
    PXEncodingOTFWindowsNTCompatibilityMapping
}
PXEncodingID;

typedef enum PXPlatformID_
{
    PXPlatformInvalid,
    PXPlatformUnicode,// Various
    PXPlatformMacintosh,// Script manager code
    PXPlatformISO,// ISO encoding[deprecated]
    PXPlatformReserved,
    PXPlatformWindows,// Windows encoding
    PXPlatformCustom
}
PXPlatformID;

typedef struct PXEncodingRecord_
{
    PXPlatformID Platform;
    PXEncodingID Encoding; // Platform specific.
    PXInt32U SubtableOffset;// 	Byte__ offset from beginning of table to the subtable for this encoding.
}
PXEncodingRecord;

typedef struct PXTTFCharacterMapping_
{
    PXInt16U Version; // Table version number(0).
    PXInt16U NumberOfTables; // Number of encoding tables that follow.
    PXSize EncodingRecordListSize;
    PXEncodingRecord* EncodingRecordList;
}
PXTTFCharacterMapping;
//-------------------------------------------------------------------------

//-----<DSIG>--------------------------------------------------------------
typedef struct PXTTFDigitalSignatureBlock_
{
    unsigned short Reserved1; // Reserved for future use; set to zero.
    unsigned short Reserved2; // Reserved for future use; set to zero.
    unsigned int SignatureLength; // Length(in bytes) of the PKCS#7 packet in the signature field.
    char* Signature; // PKCS#7 packet
}
PXTTFDigitalSignatureBlock;

typedef struct PXTTFDigitalSignatureRecord_
{
    unsigned int Format; // Format of the signature
    unsigned int Length; // Length of signature in bytes
    unsigned int SignatureBlockOffset; // Offset to the signature block from the beginning of the table
}
PXTTFDigitalSignatureRecord;

typedef struct PXTTFDigitalSignature_
{
    unsigned int Version;
    unsigned short NumberOfSignatures;
    unsigned short Flags;
    PXTTFDigitalSignatureRecord* SignatureRecordList;
    PXTTFDigitalSignatureBlock* SignatureBlockList;
}
PXTTFDigitalSignature;
//-------------------------------------------------------------------------

//------------------
typedef struct PXTTFKerningPair_
{
    unsigned short Left;// 	The glyph index for the left - hand glyph in the kerning pair.
    unsigned short Right;// 	The glyph index for the right - hand glyph in the kerning pair.
    short Value;// 	The kerning value for the above pair, in FUnits.If this value is greater than zero, the characters will be moved apart.If this value is less than zero, the character will be moved closer together.
}
PXTTFKerningPair;

typedef struct PXTTFSubtableFormat0_
{
    unsigned short NumberOfPairs; // This gives the number of kerning pairs in the table.
    unsigned short SearchRange; // The largest power of two less than or equal to the value of nPairs, multiplied by the size in bytes of an entry in the table.
    unsigned short EntrySelector; // This is calculated as log2 of the largest power of two less than or equal to the value of nPairs.This value indicates how many iterations of the search loop will have to be made. (For example, in a list of eight items, there would have to be three iterations of the loop).
    unsigned short RangeShift; // The value of nPairs minus the largest power of two less than or equal to nPairs, and then multiplied by the size in bytes of an entry in the table.

    PXTTFKerningPair* KerningPairList;
}
PXTTFSubtableFormat0;

typedef struct PXTTFSubtableFormat2_
{
    unsigned short RowWidth;// 	The width, in bytes, of a row in the table.
    unsigned short LeftClassOffset;// 	Offset from beginning of this subtable to left - hand class table.
    unsigned short RightClassOffset;// 	Offset from beginning of this subtable to right - hand class table.
    unsigned short KerningArrayOffset;// 	Offset from beginning of this subtable to the start of the kerning array.
}
PXTTFSubtableFormat2;

typedef struct PXTTFKerningSubtable_
{
    unsigned short Version;// 	Kern subtable version number
    unsigned short Length;// 	Length of the subtable, in bytes(including this header).
    //unsigned short Coverage;// 	What type of information is contained in this table.

    unsigned char Horizontal;// 	0 	1 	1 if table has horizontal data, 0 if vertical.
    unsigned char Minimum;// 	1 	1 	If this bit is set to 1, the table has minimum values.If set to 0, the table has kerning values.
    unsigned char Cross;// - stream 	2 	1 	If set to 1, kerning is perpendicular to the flow of the text.
    unsigned char Override;//	3 	1 	If this bit is set to 1 the value in this table should replace the value currently being accumulated.
    unsigned char Reserved;// 	4 - 7 	4 	Reserved.This should be set to zero.
    unsigned char Format;// 	8 - 15 	8 	Format of the subtable.Only formats 0 and 2 have been defined.Formats 1 and 3 through 255 are reserved for future use.

    PXTTFSubtableFormat0 SubtableFormat0;
    PXTTFSubtableFormat2 SubtableFormat2;
}
PXTTFKerningSubtable;

typedef struct PXTTFKerning_
{
    unsigned short Version;// Table version number(0)
    unsigned short NumberOfSubtables;// Number of subtables in the kerning table.
    PXTTFKerningSubtable* KerningSubtableList;
}
PXTTFKerning;

PXPublic void PXTTFKerningSubtableCoverageValueParse(unsigned short coverage);
//----------------


// OS2
typedef enum PXTTFArmStyle_
{
    PXTTFArmStyleInvalid,
    PXTTFArmStyleAny,
    PXTTFArmStyleNoFit,
    PXTTFArmStyleStraightArmsHorizontal,
    PXTTFArmStyleStraightArmsWedge,
    PXTTFArmStyleStraightArmsVertical,
    PXTTFArmStyleStraightArmsSingleSerif,
    PXTTFArmStyleStraightArmsDoubleSerif,
    PXTTFArmStyleNonStraightArmsHorizontal,
    PXTTFArmStyleNonStraightArmsWedge,
    PXTTFArmStyleNonStraightArmsVertical,
    PXTTFArmStyleNonStraightArmsSingleSerif,
    PXTTFArmStyleNonStraightArmsDoubleSerif
}
PXTTFArmStyle;

typedef enum PXTTFContrast_
{
    PXTTFContrastInvalid,
    PXTTFContrastAny,
    PXTTFContrastNoFit,
    PXTTFContrastNoneContrast,
    PXTTFContrastVeryLow,
    PXTTFContrastLow,
    PXTTFContrastMediumLow,
    PXTTFContrastMedium,
    PXTTFContrastMediumHigh,
    PXTTFContrastHigh,
    PXTTFContrastVeryHigh
}
PXTTFContrast;

typedef enum PXTTFFamilyType_
{
    PXTTFFamilyInvalid,
    PXTTFFamilyAny,
    PXTTFFamilyNoFit,
    PXTTFFamilyTextAndDisplay,
    PXTTFFamilyScript,
    PXTTFFamilyDecorative,
    PXTTFFamilyPictorial
}
PXTTFFamilyType;

typedef enum PXTTFHeightX_
{
    PXTTFHeightInvalid,
    PXTTFHeightAny,
    PXTTFHeightNoFit,
    PXTTFHeightConstantSmall,
    PXTTFHeightConstantStandard,
    PXTTFHeightConstantLarge,
    PXTTFHeightDuckingSmall,
    PXTTFHeightDuckingStandard,
    PXTTFHeightDuckingLarge
}
PXTTFHeightX;

typedef enum PXTTFLetterform_
{
    PXTTFLetterformInvalid,
    PXTTFLetterformAny,
    PXTTFLetterformNoFit,
    PXTTFLetterformNormalContact,
    PXTTFLetterformNormalWeighted,
    PXTTFLetterformNormalBoxed,
    PXTTFLetterformNormalFlattened,
    PXTTFLetterformNormalRounded,
    PXTTFLetterformNormalOffCenter,
    PXTTFLetterformNormalSquare,
    PXTTFLetterformObliqueContact,
    PXTTFLetterformObliqueWeighted,
    PXTTFLetterformObliqueBoxed,
    PXTTFLetterformObliqueFlattened,
    PXTTFLetterformObliqueRounded,
    PXTTFLetterformObliqueOffCenter,
    PXTTFLetterformObliqueSquare
}
PXTTFLetterform;

typedef enum PXTTFMidline_
{
    PXTTFMidlineInvalid,
    PXTTFMidlineAny,
    PXTTFMidlineNoFit,
    PXTTFMidlineStandardTrimmed,
    PXTTFMidlineStandardPointed,
    PXTTFMidlineStandardSerifed,
    PXTTFMidlineHighTrimmed,
    PXTTFMidlineHighPointed,
    PXTTFMidlineHighSerifed,
    PXTTFMidlineConstantTrimmed,
    PXTTFMidlineConstantPointed,
    PXTTFMidlineConstantSerifed,
    PXTTFMidlineLowTrimmed,
    PXTTFMidlineLowPointed,
    PXTTFMidlineLowSerifed
}
PXTTFMidline;

typedef enum PXTTFProportion_
{
    PXTTFProportionInvalid,
    PXTTFProportionAny,
    PXTTFProportionNoFit,
    PXTTFProportionOldStyle,
    PXTTFProportionModern,
    PXTTFProportionEvenWidth,
    PXTTFProportionExpanded,
    PXTTFProportionCondensed,
    PXTTFProportionVeryExpanded,
    PXTTFProportionVeryCondensed,
    PXTTFProportionMonospaced
}
PXTTFProportion;

typedef enum PXTTFSerifStyle_
{
    PXTTFSerifStyleInvalid,
    PXTTFSerifStyleAny,
    PXTTFSerifStyleNoFit,
    PXTTFSerifStyleCove,
    PXTTFSerifStyleObtuseCove,
    PXTTFSerifStyleSquareCove,
    PXTTFSerifStyleObtuseSquareCove,
    PXTTFSerifStyleSquare,
    PXTTFSerifStyleThin,
    PXTTFSerifStyleBone,
    PXTTFSerifStyleExaggerated,
    PXTTFSerifStyleTriangle,
    PXTTFSerifStyleNormalSans,
    PXTTFSerifStyleObtuseSans,
    PXTTFSerifStylePerpSans,
    PXTTFSerifStyleFlared,
    PXTTFSerifStyleRounded
}
PXTTFSerifStyle;

typedef enum PXTTFStrokeVariatoon_
{
    PXTTFStrokeVariatoonInvalid,
    PXTTFStrokeVariatoonAny,
    PXTTFStrokeVariatoonNoFit,
    PXTTFStrokeVariatoonGradualDiagonal,
    PXTTFStrokeVariatoonGradualTransitional,
    PXTTFStrokeVariatoonGradualVertical,
    PXTTFStrokeVariatoonGradualHorizontal,
    PXTTFStrokeVariatoonRapidVertical,
    PXTTFStrokeVariatoonRapidHorizontal,
    PXTTFStrokeVariatoonInstantVertical
}
PXTTFStrokeVariatoon;

typedef enum PXTTFWeight_
{
    PXTTFWeightInvalid,
    PXTTFWeightAny,
    PXTTFWeightNoFit,
    PXTTFWeightVeryLight,
    PXTTFWeightLight,
    PXTTFWeightThin,
    PXTTFWeightBook,
    PXTTFWeightMedium,
    PXTTFWeightDemi,
    PXTTFWeightBold,
    PXTTFWeightHeavy,
    PXTTFWeightBlack,
    PXTTFWeightNord
}
PXTTFWeight;

typedef struct PXTTFPanose_
{
    PXTTFFamilyType FamilyType;
    PXTTFSerifStyle SerifStyle;
    PXTTFWeight Weight;
    PXTTFProportion Proportion;
    PXTTFContrast Contrast;
    PXTTFStrokeVariatoon StrokeVariation;
    PXTTFArmStyle ArmStyle;
    PXTTFLetterform Letterform;
    PXTTFMidline Midline;
    PXTTFHeightX HeightX;
}
PXTTFPanose;

typedef struct PXTTFCompatibility_
{
    // Base
    unsigned short Version;// table version number(set to 0)
    short xAvgCharWidth;// average weighted advance width of lower case lettersand space
    unsigned short usWeightClass;// visual weight(degree of blackness or thickness) of stroke in glyphs
    unsigned short usWidthClass;//relative change from the normal aspect ratio(width to height ratio) as specified by a font designer for the glyphs in the font
    short fsType;// characteristicsand properties of this font(set undefined bits to zero)
    short ySubscriptXSize;// recommended horizontal size in pixels for subscripts
    short ySubscriptYSize;// recommended vertical size in pixels for subscripts
    short ySubscriptXOffset;// recommended horizontal offset for subscripts
    short ySubscriptYOffset;//recommended vertical offset form the baseline for subscripts
    short ySuperscriptXSize;//recommended horizontal size in pixels for superscripts
    short ySuperscriptYSize;// recommended vertical size in pixels for superscripts
    short ySuperscriptXOffset;//recommended horizontal offset for superscripts
    short ySuperscriptYOffset;//recommended vertical offset from the baseline for superscripts
    short yStrikeoutSize;// width of the strikeout stroke
    short yStrikeoutPosition;// position of the strikeout stroke relative to the baseline
    short sFamilyClass;//classification of font - family design.
    PXTTFPanose Panose;//10 byte series of number used to describe the visual characteristics of a given typeface
    unsigned int ulUnicodeRange[4];//Field is split into two bit fields of 96 and 36 bits each.The low 96 bits are used to specify the Unicode blocks encompassed by the font file.The high 32 bits are used to specify the character or script sets covered by the font file.Bit assignments are pending.Set to 0
    char achVendID[4];//four character identifier for the font vendor
    unsigned short fsSelection;//2 - byte bit field containing information concerning the nature of the font patterns
    unsigned short fsFirstCharIndex;//The minimum Unicode index in this font.
    unsigned short fsLastCharIndex;//The maximum Unicode index in this font.

    // Additional Fields

    short sTypoAscender;//The typographic ascender for this font.This is not necessarily the same as the ascender value in the 'hhea' table.
    short sTypoDescender;// The typographic descender for this font.This is not necessarily the same as the descender value in the 'hhea' table.
    short sTypoLineGap;//The typographic line gap for this font.This is not necessarily the same as the line gap value in the 'hhea' table.
    unsigned short usWinAscent;//The ascender metric for Windows.usWinAscent is computed as the yMax for all characters in the Windows ANSI character set.
    unsigned short usWinDescent;//The descender metric for Windows.usWinDescent is computed as the - yMin for all characters in the Windows ANSI character set.
    unsigned int ulCodePageRange1;//Bits 0 - 31
    unsigned int ulCodePageRange2;//Bits 32 - 63
    short sxHeight;//The distance between the baseline and the approximate height of non - ascending lowercase letters measured in FUnits.
    short sCapHeight;//The distance between the baseline and the approximate height of uppercase letters measured in FUnits.
    unsigned short usDefaultChar;// The default character displayed by Windows to represent an unsupported character. (Typically this should be 0.)
    unsigned short usBreakChar;//The break character used by Windows.
    unsigned short usMaxContext;//The maximum length of a target glyph OpenType context for any feature in this font.
    unsigned short usLowerPointSize;//Proposed for version 5 The lowest size(in twentieths of a typographic point), at which the font starts to be used.This is an inclusive value.
    unsigned short usUpperPointSize;//Proposed for version 5 The highest size(in twentieths of a typographic point), at which the font starts to be used.This is an exclusive value.Use 0xFFFFU to indicate no upper limit.
}
PXTTFCompatibility;
//-------------------


typedef struct PXTTFHeader_
{
    PXTTFVersion Version;
    PXTTFVersion Revision;

    unsigned int CheckSumAdjustment;
    unsigned int MagicNumber;
    unsigned short Flags;
    unsigned short UnitsPerEM;
    unsigned long long Created;
    unsigned long long Modified;
    short Minimum[2];
    short Maximum[2];
    unsigned short MacStyle;
    unsigned short LowestRecPpem;
    short FontDirectionHint;
    short IndexToLocFormat;
    short GlyphDataFormat;
}
PXTTFHeader;







typedef struct PXTTFPostScript_
{
    // 0x00010000 for version 1.0
    // 0x00020000 for version 2.0
    // 0x00025000 for version 2.5 (deprecated)
    // 0x00030000 for version 3.0
    PXTTFVersion Version;

    //Fixed 	italicAngle;// 	Italic angle in counter - clockwise degrees from the vertical.Zero for upright text, negative for text that leans to the right(forward).
    //FWord 	underlinePosition;// 	This is the suggested distance of the top of the underline from the baseline(negative values indicate below baseline).
    // The PostScript definition of this FontInfo dictionary key(the y coordinate of the center of the stroke) is not used for historical reasons.The value of the PostScript key may be calculated by subtracting half the underlineThickness from the value of this field.
    //FWord 	underlineThickness;// 	Suggested values for the underline thickness.In general, the underline thickness should match the thickness of the underscore character(U + 005F LOW LINE), and should also match the strikeout thickness, which is specified in the OS / 2 table.
    unsigned int  isFixedPitch;// 	Set to 0 if the font is proportionally spaced, non - zero if the font is not proportionally spaced(i.e.monospaced).
    unsigned int  minMemType42;// 	Minimum memory usage when an OpenType font is downloaded.
    unsigned int  maxMemType42;// 	Maximum memory usage when an OpenType font is downloaded.
    unsigned int  minMemType1;// 	Minimum memory usage when an OpenType font is downloaded as a Type 1 font.
    unsigned int maxMemType1;// 	Maximum memory usage when an OpenType font is downloaded as a Type 1 font.

    // Version 1.0

    // Version 2.0
    unsigned short NumberOfGlyphs; // Number of glyphs(this should be the same as numGlyphs in 'maxp' table).
    unsigned short* GlyphNameIndex; // (NumberOfGlyphs) Array of indices into the string data.See below for details.
    char* StringData; // (variable) Storage for the string data.

    // Version 2.5
    // uint16 	numGlyphs 	Number of glyphs
    char* Offset; // (numGlyphs) Difference between graphic indexand standard order of glyph

    // Version 3.0
}
PXTTFPostScript;


typedef struct PXTTFOffsetTable_
{
    PXTTFVersion Version;
    PXInt16U NumberOfTables;
    PXInt16U SearchRange;
    PXInt16U EntrySelctor;
    PXInt16U RangeShift;
}
PXTTFOffsetTable;


typedef struct PXTTFMaximumProfile_
{
    PXTTFVersion Version;
    PXInt16S NumberOfGlyphs; // the number of glyphs in the font
    PXInt16S PointsMaximal; // points in non - compound glyph
    PXInt16S ContoursMaximal; // contours in non - compound glyph
    PXInt16S ComponentPointsMaximal; //points in compound glyph
    PXInt16S ComponentContoursMaximal; //	contours in compound glyph
    PXInt16S ZonesMaximal; // set to 2
    PXInt16S TwilightPointsMaximal;//points used in Twilight Zone(Z0)
    PXInt16S StorageMaximal; // number of Storage Area locations
    PXInt16S FunctionDefsMaximal; // number of FDEFs
    PXInt16S InstructionDefsMaximal; // number of IDEFs
    PXInt16S StackElementsMaximal; // maximum stack depth
    PXInt16S SizeOfInstructionsMaximal; // byte count for glyph instructions
    PXInt16S ComponentElementsMaximal; // number of glyphs referenced at top level
    PXInt16S ComponentDepthMaximal; // levels of recursion, set to 0 if font has only simple glyphs
}
PXTTFMaximumProfile;


#define PXOutlineFlagOnCurve        0b100000000
#define PXOutlineFlagShortVectorX   0b010000000
#define PXOutlineFlagShortVectorY   0b001000000
#define PXOutlineFlagRepeat         0b000100000
#define PXOutlineFlagShortVectorXB  0b000010000
#define PXOutlineFlagShortVectorYB  0b000001000
#define PXOutlineFlagReserved       0b000000111

typedef struct PXTableEntryGlyphOutlineEntry_
{
    PXInt16U* ContourList;
    PXInt16S ContourListSize;
    PXInt16U Minimum[2];
    PXInt16U Maximum[2];


    // Simple glyphs
    PXInt8U FlagList;

    PXInt8U* InstructionList;
    PXInt16U InstructionListSize;


    // Compound glyphs
    PXInt16U ComponentFlagList;
    PXInt16U GlyphIndex;
}
PXTableEntryGlyphOutlineEntry;


// Tag Image File Format
typedef struct PXTTF_
{
    PXTTFHeader Header;
    PXTTFHorizontalHeader HorizontalHeader;
    PXTTFMaximumProfile MaximumProfile;
    PXTTFCompatibility Compatibility;

    PXTTFCharacterMapping CharacterMapping;
    PXTTFPostScript PostScript;
    PXTTFKerning Kerning;
    //---------------------------------------------------------------------

    //---<Windows Standard>------------------------------------------------
    PXTTFDigitalSignature DigitalSignature;
    PXTTFLinearThreshold LinearThreshold;
    PXTTFVerticalDeviceMetrics VerticalDeviceMetrics;
    //---------------------------------------------------------------------
}
PXTTF;

PXPrivate PXPlatformID PXAPI PXTTFPlatformFromID(const PXInt16U platformID);
PXPrivate PXEncodingID PXAPI PXTTFEncodingFromID(const PXPlatformID platformID, const PXInt16U encodingID);

PXPrivate PXTTFVersionType PXAPI PXTTFVersionTypeFromID(unsigned short major, unsigned short minor);
//PXPublic void PXTTFVersionTypeToID(unsigned short* major, unsigned short* minor, const PXTTFVersionType versionType);

PXPrivate PXTTFTableEntryType PXAPI PXTTFTableEntryTypeFromID(const PXInt32U tableEntryType);
//PXPublic PXInt32U PXTTFTableEntryTypeFromID(const PXTTFTableEntryType tableEntryType);

PXPublic void PXAPI PXTTFDestruct(PXTTF* const ttf);

PXPublic PXActionResult PXAPI PXTTFLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXTTFSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif