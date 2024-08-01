#ifndef PXPNGINCLUDE
#define PXPNGINCLUDE

#include "../PXResource.h"

// 0x0001 = palette used
// 0x0010 = color used
// 0x0100 = alpha channel used
typedef enum PXPNGColorType_
{
    PXPNGColorInvalid,
    PXPNGColorGrayscale, // ColorType = 0
    PXPNGColorRGB,  // ColorType = 2
    PXPNGColorPalette,  // ColorType = 3
    PXPNGColorGrayscaleAlpha,  // ColorType = 4
    PXPNGColorRGBA  // ColorType = 6
}
PXPNGColorType;

typedef enum PXPNGInterlaceMethod_
{
    PXPNGInterlaceInvalid,
    PXPNGInterlaceNone,
    PXPNGInterlaceADAM7
}
PXPNGInterlaceMethod;

typedef enum PXPNGUnitSpecifier_
{
    PXPNGUnitSpecifierInvalid,
    PXPNGUnitSpecifierUnkown,
    PXPNGUnitSpecifierMeter
}
PXPNGUnitSpecifier;

typedef struct PXPNGPhysicalPixelDimension_
{
    PXInt32U PixelsPerUnit[2];
    PXPNGUnitSpecifier UnitSpecifier;
}
PXPNGPhysicalPixelDimension;

typedef struct PXPNGPrimaryChromatics_
{
    PXInt32U WhiteX;
    PXInt32U WhiteY;
    PXInt32U RedX;
    PXInt32U RedY;
    PXInt32U GreenX;
    PXInt32U GreenY;
    PXInt32U BlueX;
    PXInt32U BlueY;
}
PXPNGPrimaryChromatics;

typedef struct PXPNGBackgroundColor_
{
    PXInt16U GreyScale;
    PXInt16U Red;
    PXInt16U Green;
    PXInt16U Blue;
    PXInt8U PaletteIndex;
}
PXPNGBackgroundColor;

typedef struct PXPNGLastModificationTime_
{
    PXInt16U Year;
    PXInt8U Month;
    PXInt8U Day;
    PXInt8U Hour;
    PXInt8U Minute;
    PXInt8U Second;
}
PXPNGLastModificationTime;

typedef struct PXPNGPaletteHistogram_
{
    PXSize ColorFrequencyListSize;
    PXInt16U* ColorFrequencyList;
}
PXPNGPaletteHistogram;

// Chunk Specifications
typedef enum PXPNGChunkType_
{
    PXPNGChunkInvalid,

    //--------------------------------------------------------------------------
    // [Critical chunks]
    //--------------------------------------------------------------------------

    PXPNGChunkImageHeader               = PXInt32Make('I', 'H', 'D', 'R'), // [IHDR] The chunk will appear first.
    PXPNGChunkPalette                   = PXInt32Make('P', 'L', 'T', 'E'), // [PLTE] Palette
    PXPNGChunkImageData                 = PXInt32Make('I', 'D', 'A', 'T'), // [IDAT] Image data
    PXPNGChunkImageEnd                  = PXInt32Make('I', 'E', 'N', 'D'), // [IEND] The chunk appears LAST. It marks the end of the PNG datastream.

    //--------------------------------------------------------------------------
    // [Ancillary chunks]
    //--------------------------------------------------------------------------
    // Transparency information
    //--------------------------------------------------------------------------

    PXPNGChunkTransparency              = PXInt32Make('i', 'T', 'X', 't'), // [tRNS] Transparency

    //--------------------------------------------------------------------------
    // Color space information
    //--------------------------------------------------------------------------

    PXPNGChunkImageGamma                = PXInt32Make('g', 'A', 'M', 'A'), // [gAMA] Image gamma
    PXPNGChunkPrimaryChromaticities     = PXInt32Make('c', 'H', 'R', 'M'), // [cHRM] -
    PXPNGChunkStandardRGBColorSpace     = PXInt32Make('s', 'R', 'G', 'B'), // [sRGB] -
    PXPNGChunkEmbeddedICCProfile        = PXInt32Make('i', 'C', 'C', 'P'), // [iCCP] -

    //--------------------------------------------------------------------------
    // Textual, information
    //--------------------------------------------------------------------------

    PXPNGChunkTextualData               = PXInt32Make('t', 'E', 'X', 't'), // [tEXt] -
    PXPNGChunkCompressedTextualData     = PXInt32Make('z', 'T', 'X', 't'), // [zTXt] -
    PXPNGChunkInternationalTextualData  = PXInt32Make('i', 'T', 'X', 't'), // [iTXt] -

    //--------------------------------------------------------------------------
    // Miscellaneous information
    //--------------------------------------------------------------------------

    PXPNGChunkBackgroundColor           = PXInt32Make('b', 'K', 'G', 'D'), // [bKGD] -
    PXPNGChunkPhysicalPixelDimensions   = PXInt32Make('p', 'H', 'Y', 's'), // [pHYs] -
    PXPNGChunkSignificantBits           = PXInt32Make('s', 'B', 'I', 'T'), // [sBIT] -
    PXPNGChunkSuggestedPalette          = PXInt32Make('s', 'P', 'L', 'T'), // [sPLT] -
    PXPNGChunkPaletteHistogram          = PXInt32Make('h', 'I', 'S', 'T'), // [hIST] -
    PXPNGChunkLastModificationTime      = PXInt32Make('t', 'I', 'M', 'E'), // [tIME] -

    //--------------------------------------------------------------------------
    // Additional chunk types
    //--------------------------------------------------------------------------

    // Unkown type, placegolder for future types or a midified PNG standard
    PXPNGChunkCustom
}
PXPNGChunkType;

typedef    struct PXPNGChunk_
{
    // The data bytes appropriate to the chunk type, if any. This field can be of zero length.
    //void* ChunkData;

    // [4-byte] Giving the number of bytes in the chunk's data field. The length counts only the data field, not itself, the chunk type code, or the CRC. Zero is a valid length. Although encoders and decoders should treat the length as unsigned, its value must not exceed 231 bytes.
    PXInt32U Lengh;

    PXInt32UCluster ChunkID;

    PXBool IsEssential; // Ancillary Bit - Is this chunk not replaceable?.
    PXBool IsRegisteredStandard; // Private Bit - Is this chunk in the offically registered in any way?
    PXBool IsSafeToCopy;  // Can this cunk be modifyed anyhow or does it have a depecdency on the imagedata?

    // [4-byte] uppercase and lowercase ASCII letters (A-Z and a-z, or 65-90 and 97-122 decimal).
    PXPNGChunkType ChunkType;

    // A 4-byte CRC (Cyclic Redundancy Check) calculated on the preceding bytes in the chunk, including the chunk type code and chunk data fields, but not including the length field. The CRC is always present, even for chunks containing no data. See CRC algorithm.

    PXBool CRCOK;
    PXInt32U CRC;
}
PXPNGChunk;

typedef struct PXPNGImageHeader_
{
    PXInt32U Width;
    PXInt32U Height;

    // Color type is a single-byte integer that describes the interpretation of the image data.
    // Color type codes represent sums of the following values: 1 (palette used), 2 (color used),
    // and 4 (alpha channel used). Valid values are 0, 2, 3, 4, and 6.
    PXPNGColorType ColorType;
    PXPNGInterlaceMethod InterlaceMethod;

    // Bit depth is a single-byte integer giving the number of bits per sample or per palette index (not per pixel).
    // Valid values are 1, 2, 4, 8, and 16, although not all values are allowed for all color types.
    PXInt8U BitDepth;
    PXInt8U CompressionMethod;
    PXInt8U FilterMethod;
}
PXPNGImageHeader;

typedef struct PXPNG_
{
    //---[Important Data]--------------------------------------------------------
    PXPNGImageHeader ImageHeader;
    PXInt16U PaletteSize;
    PXInt8U Palette[1024];
    PXPNGPrimaryChromatics PrimaryChromatics;
    //---------------------------------------------------------------------------

    //---[Optional Data]---------------------------------------------------------
    PXInt32U SignificantBits; // sBIT
    PXInt32U Gamma; // gAMA
    PXPNGBackgroundColor BackgroundColor; // bKGD
    PXPNGPaletteHistogram PaletteHistogram; // hIST
    // XXXXXXXXXXXXXX XXXXXXXXXXXXXX // cHRM
    // XXXXXXXXXXXXXX XXXXXXXXXXXXXX // iCCP
    // XXXXXXXXXXXXXX XXXXXXXXXXXXXX // cHRM
    unsigned char RenderingIntent; // sRGB
    //PNGTransparency Transparency; // tRNS
    PXPNGPhysicalPixelDimension PhysicalPixelDimension; // pHYs
    //PNGSuggestedPalette SuggestedPalette; // sPLT
    PXPNGLastModificationTime LastModificationTime; // tIME
    // XXXXXXXXXXXXXX XXXXXXXXXXXXXX // iTXt
    //
    //---[ oFFs - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ sCAL - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ tEXt - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ zTXt - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ fRAc - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ gIFg - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ gIFt - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---[ gIFx - xxxxxxxxxxxxxxxxxxxxxxx ]--------------------------------------
    //---------------------------------------------------------------------------

    //---[ IDAT - Image Data (Compressed)]---------------------------------------
    //PXSize PXZLIBHeaderListSize;
    //PXZLIBHeader* PXZLIBHeaderList;
    //---------------------------------------------------------------------------

    PXSize PixelDataSize;
    void* PixelData;
    //---------------------------------------------------------------------------
}
PXPNG;

typedef enum LodePNGColorType
{
    LCT_GREY = 0, /*grayscale: 1,2,4,8,16 bit*/
    LCT_RGB = 2, /*RGB: 8,16 bit*/
    LCT_PALETTE = 3, /*palette: 1,2,4,8 bit*/
    LCT_GREY_ALPHA = 4, /*grayscale with alpha: 8,16 bit*/
    LCT_RGBA = 6, /*RGB with alpha: 8,16 bit*/
    /*LCT_MAX_OCTET_VALUE lets the compiler allow this enum to represent any invalid
    byte value from 0 to 255 that could be present in an invalid PNG file header. Do
    not use, compare with or set the name LCT_MAX_OCTET_VALUE, instead either use
    the valid color type names above, or numeric values like 1 or 7 when checking for
    particular disallowed color type byte values, or cast to integer to print it.*/
    LCT_MAX_OCTET_VALUE = 255
} LodePNGColorType;

/*
Color mode of an image. Contains all information required to decode the pixel
bits to RGBA colors. This information is the same as used in the PNG file
format, and is used both for PNG and raw image data in LodePNG.
*/
typedef struct LodePNGColorMode
{
    /*header (IHDR)*/
    LodePNGColorType colortype; /*color type, see PNG standard or documentation further in this header file*/
    unsigned bitdepth;  /*bits per sample, see PNG standard or documentation further in this header file*/

    /*
    palette (PLTE and tRNS)

    Dynamically allocated with the colors of the palette, including alpha.
    This field may not be allocated directly, use lodepng_color_mode_init first,
    then lodepng_palette_add per color to correctly initialize it (to ensure size
    of exactly 1024 bytes).

    The alpha channels must be set as well, set them to 255 for opaque images.

    When decoding, by default you can ignore this palette, since LodePNG already
    fills the palette colors in the pixels of the raw RGBA output.

    The palette is only supported for color type 3.
    */
    const PXInt8U* palette; /*palette in RGBARGBA... order. Must be either 0, or when allocated must have 1024 bytes*/
    PXSize palettesize; /*palette size in number of colors (amount of used bytes is 4 * palettesize)*/

    /*
    transparent color key (tRNS)

    This color uses the same bit depth as the bitdepth value in this struct, which can be 1-bit to 16-bit.
    For grayscale PNGs, r, g and b will all 3 be set to the same.

    When decoding, by default you can ignore this information, since LodePNG sets
    pixels with this key to transparent already in the raw RGBA output.

    The color key is only supported for color types 0 and 2.
    */
    unsigned key_defined; /*is a transparent color key given? 0 = false, 1 = true*/
    unsigned key_r;       /*red/grayscale component of color key*/
    unsigned key_g;       /*green component of color key*/
    unsigned key_b;       /*blue component of color key*/
} LodePNGColorMode;



// One node of a color tree
// This is the data structure used to count the number of unique colors and to get a palette
// index for a color. It's like an octree, but because the alpha channel is used too, each
// node has 16 instead of 8 children.

typedef struct PNGColorTree_
{
    // Up to 8
    struct PNGColorTree_* children[16]; // up to 16 pointers to ColorTree of next level
    int index;                            // the payload. Only has a meaningful value if this is in the last level
}
PNGColorTree;





PXPublic PXActionResult PXAPI PXPNGImageDataDecompress(const PXPNG* const png, const void* pixelDataIn, void* pixelDataOut, unsigned char bitDepth, PXPNGColorType colorType);







static unsigned int color_tree_add(PNGColorTree* tree, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned index);



static unsigned getNumColorChannels(LodePNGColorType colortype);

static PXSize lodepng_get_bpp_lct(LodePNGColorType colortype, PXSize bitdepth);

static int lodepng_color_mode_equal(const LodePNGColorMode* a, const LodePNGColorMode* b);

/*Get RGBA16 color of pixel with index i (y * width + x) from the raw image with
given color type, but the given color type must be 16-bit itself.*/
static void getPixelColorRGBA16(unsigned short* r, unsigned short* g, unsigned short* b, unsigned short* a,                                const unsigned char* in, PXSize i, const LodePNGColorMode* mode);


/*put a pixel, given its RGBA16 color, into image of any color 16-bitdepth type*/
static void rgba16ToPixel(unsigned char* out, PXSize i, const LodePNGColorMode* mode, unsigned short r, unsigned short g, unsigned short b, unsigned short a);


/*Similar to getPixelColorRGBA8, but with all the for loops inside of the color
mode test cases, optimized to convert the colors much faster, when converting
to the common case of RGBA with 8 bit per channel. buffer must be RGBA with
enough memory.*/
static void getPixelColorsRGBA8(unsigned char* buffer, PXSize numpixels, const unsigned char* in, const LodePNGColorMode* mode);


/*Similar to getPixelColorsRGBA8, but with 3-channel RGB output.*/
static void getPixelColorsRGB8(unsigned char* buffer, PXSize numpixels, const unsigned char* in, const LodePNGColorMode* mode);

/*Get RGBA8 color of pixel with index i (y * width + x) from the raw image with given color type.*/
static void getPixelColorRGBA8(PXColorRGBAI8* const color, const unsigned char* in, PXSize i, const LodePNGColorMode* mode);


/*put a pixel, given its RGBA color, into image of any color type*/
PXPrivate PXActionResult PXAPI rgba8ToPixel
(
    unsigned char* out,
    PXSize i,
    const LodePNGColorMode* mode,
    PNGColorTree* tree /*for palette*/,
    const PXColorRGBAI8* const color
);


PXPublic PXSize lodepng_get_raw_size_lct(PXSize w, PXSize h, LodePNGColorType colortype, PXSize bitdepth);
PXPublic PXSize lodepng_get_raw_size(PXSize w, PXSize h, const LodePNGColorMode* color);

/*returns -1 if color not present, its index otherwise*/
PXPublic int color_tree_get(PNGColorTree* tree, const PXColorRGBAI8* const color);

/*index: bitgroup index, bits: bitgroup size(1, 2 or 4), in: bitgroup value, out: octet array to add bits to*/
PXPublic void addColorBits(unsigned char* out, PXSize index, unsigned int bits, unsigned int in);

PXPublic unsigned char readBitFromReversedStream(PXSize* bitpointer, const char* bitstream);
PXPublic unsigned readBitsFromReversedStream(PXSize* bitpointer, const unsigned char* bitstream, PXSize nbits);



//----------------------------------------------------
PXPrivate inline PXPNGColorType PXAPI PXPNGColorTypeFromID(const PXInt8U colorType);
PXPrivate inline PXInt8U PXAPI PXPNGColorTypeToID(const PXPNGColorType colorType);

PXPrivate inline PXPNGInterlaceMethod PXAPI PXPNGInterlaceMethodFromID(const PXInt8U interlaceMethod);
PXPrivate inline PXInt8U PXAPI PXPNGInterlaceMethodToID(const PXPNGInterlaceMethod interlaceMethod);

PXPrivate inline PXInt8U PXAPI PXPNGColorTypeNumberOfChannels(const PXPNGColorType pngColorType);

PXPublic void PXAPI PXPNGDestruct(PXPNG* const png);

PXPublic PXInt8U PXAPI PXPNGBitsPerPixel(const PXPNG* const png);

PXPublic PXSize PXAPI PXPNGFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

PXPublic PXActionResult PXAPI PXPNGLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPNGSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif