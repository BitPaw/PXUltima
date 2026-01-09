#include "PXDEFLATE.h"

#include <stdlib.h>
#include <string.h>

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Math/PXMath.h>
#include <PX/Media/Huffman/PXHuffman.h>
#include <PX/Media/LZ77/PXLZ77.h>
#include <PX/OS/PXOS.h>


#define FIRST_LENGTH_CODE_INDEX 257
#define LAST_LENGTH_CODE_INDEX 285
// 256 literals, the end code, some length codes, and 2 unused codes
#define NUM_DEFLATE_CODE_SYMBOLS 288
// the distance codes have their own symbols, 30 used, 2 unused
#define NUM_DISTANCE_SYMBOLS 32
// the code length codes. 0-15: code lengths, 16: copy previous 3-6 times, 17: 3-10 zeros, 18: 11-138 zeros
#define NUM_CODE_LENGTH_CODES 19

// version of CERROR_BREAK that assumes the common case where the error variable is named "error"
#define ERROR_BREAK(code) CERROR_BREAK(error, code)





// base lengths represented by codes 257-285
// WARNING! You need to add +3. 
// This list is reduced by 3 to fit into 8-Bit
static const PXI8U PXHuffmanLengthBase[29] =
{
    0,1,2,3,4,5,6,7,8,10,12,14,16,20,
    24,28,32,40,48,56,64,80,96,112,
    128,160,192,224,255
};
#define PXHuffmanLengthBaseGet(index) (((PXI16U)PXHuffmanLengthBase[index]) + 3u)

// extra bits used by codes 257-285 (added to base length)
static const PXI8U PXHuffmanLengthExtra[29] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 5, 5, 5, 5, 0
};

// base backwards distances (the bits of distance codes appear after length codes and use their own huffman tree)
static const PXI16U PXHuffmanDistanceBase[30] =
{
       1,  2,       3,     4,     5,    7,    9,   13,    17, 25,
      33, 49,      65,    97,   129,  193,  257,  385,   513,
     769, 1025,  1537,  2049,  3073, 4097, 6145, 8193, 12289, 16385, 24577
};

// extra bits of backwards distances (added to base)
static const PXI8U PXHuffmanDistanceExtra[30] =
{
    0, 0,  0,  0,  1,  1,  2,  2,  3,  3, 
    4, 4,  5,  5,  6,  6,  7,  7,  8,  8, 
    9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};


// the order in which "code length alphabet code lengths" 
// are stored as specified by deflate, out of this the huffman 
// tree of the dynamic huffman tree lengths is generated
const PXI8U PXHuffmanCLCL_ORDER[NUM_CODE_LENGTH_CODES] =
{ 
    16, 17, 18,  0,  8, 
     7,  9,  6, 10,  5, 
    11,  4, 12,  3, 13,
     2, 14,  1, 15 
};









/*
Often in case of an error a value is assigned to a variable and then it breaks
out of a loop (to go to the cleanup phase of a function). This macro does that.
It makes the error handling code shorter and more readable.

Example: if(!uivector_resize(&lz77_encoded, datasize)) ERROR_BREAK(83);
*/
#define CERROR_BREAK(errorvar, code){\
  errorvar = code;\
  break;\
}



/* amount of bits for first huffman table lookup (aka root bits), see HuffmanTree_makeTable and huffmanDecodeSymbol.*/
/* values 8u and 9u work the fastest */
#define PXHuffmanFirstBits 9u

/* a symbol value too big to represent any valid symbol, to indicate reading disallowed huffman bits combination,
which is possible in case of only 0 or 1 present symbols. */
#define INVALIDSYMBOL 65535u





const PXI32U PXDEFLATEHeader[] =
{
    PXTypeReciverSize08U | PXTypeBit08U(1),
    PXTypeReciverSize08U | PXTypeBit08U(2)
};
const PXI8U PXDEFLATEHeaderSize = sizeof(PXDEFLATEHeader) / sizeof(PXI32U);


const PXI32U PXDEFLATELiteralRawDataList[] =
{
    PXTypeInt16ULE,
    PXTypeInt16ULE
};
const PXI8U PXDEFLATELiteralRawDataListSize = sizeof(PXDEFLATELiteralRawDataList) / sizeof(PXI32U);


typedef struct PXDEFLATELiteralRawData_
{
    PXI16U LengthActual;
    PXI16U LengthRemainder;
}
PXDEFLATELiteralRawData;


PXResult PXAPI PXDEFLATEParse(PXFile PXREF pxInputStream, PXFile PXREF pxOutputStream)
{
    PXHuffmanTree literalAndLengthCodes;
    PXHuffmanTree distanceCodes;
    PXDeflateBlock pxDeflateBlock;
    PXBool foundEndOFBlock = PXFalse;

    do
    {
        foundEndOFBlock = PXFalse;

        const PXSize readBytes = PXFileBinding
        (
            pxInputStream,
            &pxDeflateBlock, 
            PXDEFLATEHeader, 
            PXDEFLATEHeaderSize,
            PXFileBindingRead
        );

        if(!readBytes)
        {
            return PXResultInvalid;
        }

        switch(pxDeflateBlock.EncodingMethod)
        {
            default:
            case PXDeflateEncodingReserverd:
            {
                return PXActionRefusedFormatSettingNotAllowed;
            }
            case PXDeflateEncodingLiteralRaw:
            {
                PXDEFLATELiteralRawData pxDEFLATELiteralRawData;

                PXFileSkipBitsToNextByte(pxInputStream); // Skip remaining Bytes

                const PXSize readBytes = PXFileBinding
                (
                    pxInputStream,
                    &pxDEFLATELiteralRawData,
                    PXDEFLATELiteralRawDataList,
                    PXDEFLATELiteralRawDataListSize,
                    PXFileBindingRead
                );
                const PXBool validLength = 0xFFFFu == (pxDEFLATELiteralRawData.LengthActual + pxDEFLATELiteralRawData.LengthRemainder);

                if(!validLength)
                {
                    return PXActionFailedFormatNotAsExpected;
                }

                const PXSize dataRead = PXFileDataCopy(pxInputStream, pxOutputStream, pxDEFLATELiteralRawData.LengthActual);

                continue;
            }
            case PXDeflateEncodingHuffmanDynamic:
            {
                const PXResult result = PXHuffmanDistanceTreeGenerateDynamic(pxInputStream, &literalAndLengthCodes, &distanceCodes);

                if(PXResultOK != result)
                {
                    return result;
                }
                break;
            }
            case PXDeflateEncodingHuffmanStatic:
            {
                PXHuffmanDistanceTreeGenerateFixed(&literalAndLengthCodes, &distanceCodes);
                break;
            }
        }

        // Start to decode huffman symbols and trees
        while(!foundEndOFBlock)
        {
            const PXI16U resultLengthCode = PXHuffmanSymbolDecode(pxInputStream, &literalAndLengthCodes);
            const PXHuffmanCodeType huffmanCodeType = PXHuffmanCodeTypeFromCode(resultLengthCode);

            switch(huffmanCodeType)
            {
                case PXHuffmanCodeInvalid:
                {
                    // printf("[Symbol] Error: Invalid\n");
                    break; // ERROR
                }
                case PXHuffmanCodeLiteral:
                {
                    // printf("[Symbol] <%2x>(%3i) Literal.\n", resultLengthCode, resultLengthCode);
                    PXFileWriteI8U(pxOutputStream, resultLengthCode);
                    break;
                }
                case PXHuffmanCodeLength:
                {
                    // printf("[Symbol] <%2x>(%3i) Length.\n", resultLengthCode, resultLengthCode);

       

                    // part 1: get length base
                    PXI16U length = PXHuffmanLengthBaseGet(resultLengthCode - FIRST_LENGTH_CODE_INDEX);

                    // part 2: get extra bits and add the value of that to length
                    const PXI8U numextrabits_l = PXHuffmanLengthExtra[resultLengthCode - FIRST_LENGTH_CODE_INDEX];


                    if(numextrabits_l != 0)
                    {
                        /* bits already ensured above */
                        length += PXFileReadBits(pxInputStream, numextrabits_l);
                    }

                    /*part 3: get distance code*/
                    //ensureBits32(reader, 28); /* up to 15 for the huffman symbol, up to 13 for the extra bits */
                    const PXI16U resultDistanceCode = PXHuffmanSymbolDecode(pxInputStream, &distanceCodes);
                    PXBool isUnsupportedCode = resultDistanceCode > 29u;
                    PXBool isIllegalCode = resultDistanceCode > 31u;

                    if(isUnsupportedCode)
                    {
                        if(isIllegalCode)  /* if(code_d == INVALIDSYMBOL) */
                        {
                            return PXActionRefusedParserSymbolNotAsExpected; // error: tried to read disallowed huffman symbol
                        }
                        else
                        {
                            return PXActionRefusedParserSymbolNotAsExpected; // error: invalid distance code (30-31 are never used)
                        }
                    }

                    PXSize distance = PXHuffmanDistanceBase[resultDistanceCode];

                    // part 4: get extra bits from distance
                    PXI8U numextrabits_d = PXHuffmanDistanceExtra[resultDistanceCode]; // extra bits for length and distance
                 
                    if(numextrabits_d != 0)
                    {
                        // bits already ensured above
                        distance += PXFileReadBits(pxInputStream, numextrabits_d);
                    }

                    /*part 5: fill in all the out[n] values based on the length and dist*/
                    PXSize start = PXFileDataPosition(pxOutputStream);//(*outputBufferSizeRead);

                    if(distance > start)
                        return PXActionRefusedParserSymbolNotAsExpected; /*too long backward distance*/

                    PXSize backward = start - distance;

                    /*(*outputBufferSizeRead)*/ 
                    PXFileCursorAdvance(pxOutputStream, length);

                    // if (!ucvector_resize(out, out->size + length)) ERROR_BREAK(83 /*alloc fail*/);


                    PXByte* data = (PXByte*)PXFileDataAtCursor(pxOutputStream);

                    if(distance < length)
                    {
                        start += PXMemoryCopy(data + backward, data + start, distance);

                        for(PXSize forward = distance; forward < length; ++forward)
                        {
                            data[start++] = data[backward++];
                        }
                    }
                    else
                    {
                        PXMemoryCopy(data + backward, data + start, length);
                    }
                    break;
                }
                case PXHuffmanCodeEndOfBlock:
                {
                    //printf("[Symbol] <%2x>(%3i) End of Block.\n", resultLengthCode, resultLengthCode);
                    foundEndOFBlock = PXTrue;

                    break; // FINISHED!
                }
            }
        }
        
    } 
    while(!pxDeflateBlock.IsLastBlock);

    //(PXAdress)pxOutputStream->Data -= 2;

    return PXResultOK;
}













/*dynamic vector of unsigned ints*/
typedef struct uivector
{
    unsigned* data;
    PXSize size; /*size in number of unsigned longs*/
    PXSize allocsize; /*allocated size in bytes*/
} uivector;

static void uivector_cleanup(void* p)
{
    ((uivector*)p)->size = ((uivector*)p)->allocsize = 0;
    PXMemoryHeapFree(PXNull, ((uivector*)p)->data);
    ((uivector*)p)->data = NULL;
}

/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_resize(uivector* p, PXSize size)
{
    PXSize allocsize = size * sizeof(unsigned);
    if(allocsize > p->allocsize)
    {
        PXSize newsize = allocsize + (p->allocsize >> 1u);
        void* data = PXMemoryHeapRealloc(PXNull, p->data, newsize);
        if(data)
        {
            p->allocsize = newsize;
            p->data = (unsigned*)data;
        }
        else return 0; /*error: not enough memory*/
    }
    p->size = size;
    return 1; /*success*/
}

static void uivector_init(uivector* p)
{
    p->data = NULL;
    p->size = p->allocsize = 0;
}

/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_push_back(uivector* p, unsigned c)
{
    if(!uivector_resize(p, p->size + 1)) return 0;
    p->data[p->size - 1] = c;
    return 1;
}






typedef struct LodePNGCompressSettings LodePNGCompressSettings;
struct LodePNGCompressSettings /*deflate = compress*/
{
    /*LZ77 related settings*/
    unsigned btype; /*the block type for LZ (0, 1, 2 or 3, see PXZLIB standard). Should be 2 for proper compression.*/
    unsigned use_lz77; /*whether or not to use LZ77. Should be 1 for proper compression.*/
    unsigned windowsize; /*must be a power of two <= 32768. higher compresses more but is slower. Default value: 2048.*/
    unsigned minmatch; /*minimum lz77 length. 3 is normally best, 6 can be better for some PNGs. Default: 0*/
    unsigned nicematch; /*stop searching if >= this length found. Set to 258 for best compression. Default: 128*/
    unsigned lazymatching; /*use lazy matching: better compression but a bit slower. Default: true*/

    /*use custom PXZLIB encoder instead of built in one (default: null)*/
    unsigned (*custom_PXZLIB)(unsigned char**, PXSize*,
                              const unsigned char*, PXSize,
                              const LodePNGCompressSettings*);
    /*use custom deflate encoder instead of built in one (default: null)
    if custom_PXZLIB is used, custom_deflate is ignored since only the built in
    PXZLIB function will call custom_deflate*/
    unsigned (*custom_deflate)(unsigned char**, PXSize*,
                               const unsigned char*, PXSize,
                               const LodePNGCompressSettings*);

    const void* custom_context; /*optional custom settings for custom functions*/
};

PXPublic const LodePNGCompressSettings lodepng_default_compress_settings;
void lodepng_compress_settings_init(LodePNGCompressSettings* settings);





/*3 bytes of data get encoded into two bytes. The hash cannot use more than 3
bytes as input because 3 is the minimum match length for deflate*/
const unsigned HASH_NUM_VALUES = 65536;
const unsigned HASH_BIT_MASK = 65535; /*HASH_NUM_VALUES - 1, but C90 does not like that as initializer*/
const PXSize MAX_SUPPORTED_DEFLATE_LENGTH = 258;

typedef struct Hash
{
    int* head; /*hash value to head circular pos - can be outdated if went around window*/
    /*circular pos to prev circular pos*/
    PXI16U* chain;
    int* val; /*circular pos to hash value*/

    /*TODO: do this not only for zeros but for any repeated byte. However for PNG
    it's always going to be the zeros that dominate, so not important for PNG*/
    int* headz; /*similar to head, but for chainz*/
    PXI16U* chainz; /*those with same amount of zeros*/
    PXI16U* zeros; /*length of zeros streak, used as a second hash chain*/
} Hash;

PXResult PXAPI hash_init(Hash PXREF hash, const PXSize windowsize)
{
    hash->head = PXMemoryHeapCallocT(int, HASH_NUM_VALUES);
    hash->val = PXMemoryHeapCallocT(int, windowsize);
    hash->chain = PXMemoryHeapCallocT(PXI16U, windowsize);
    hash->zeros = PXMemoryHeapCallocT(PXI16U, windowsize);
    hash->headz = PXMemoryHeapCallocT(int, (MAX_SUPPORTED_DEFLATE_LENGTH + 1));
    hash->chainz = PXMemoryHeapCallocT(PXI16U, windowsize);

    if(!hash->head || !hash->chain || !hash->val || !hash->headz || !hash->chainz || !hash->zeros)
    {
        return PXActionFailedMemoryAllocation;
    }

    // initialize hash table
    for(PXSize i = 0; i != HASH_NUM_VALUES; ++i)
        hash->head[i] = -1;

    for(PXSize i = 0; i != windowsize; ++i) 
        hash->val[i] = -1;

    for(PXSize i = 0; i != windowsize; ++i)
        hash->chain[i] = i; /*same value as index indicates uninitialized*/

    for(PXSize i = 0; i <= MAX_SUPPORTED_DEFLATE_LENGTH; ++i) 
        hash->headz[i] = -1;

    for(PXSize i = 0; i != windowsize; ++i) 
        hash->chainz[i] = i; /*same value as index indicates uninitialized*/

    return PXResultOK;
}

void PXAPI hash_cleanup(Hash PXREF hash)
{
    PXMemoryHeapFree(PXNull, hash->head);
    PXMemoryHeapFree(PXNull, hash->val);
    PXMemoryHeapFree(PXNull, hash->chain);
    PXMemoryHeapFree(PXNull, hash->zeros);
    PXMemoryHeapFree(PXNull, hash->headz);
    PXMemoryHeapFree(PXNull, hash->chainz);
}

PXI32U getHash(const unsigned char* data, const PXSize size, const PXSize pos)
{
    PXI32U result = 0;
    if(pos + 2 < size)
    {
        /*A simple shift and xor hash is used. Since the data of PNGs is dominated
        by zeroes due to the filters, a better hash does not have a significant
        effect on speed in traversing the chain, and causes more time spend on
        calculating the hash.*/
        result ^= ((PXI32U)data[pos + 0] << 0u);
        result ^= ((PXI32U)data[pos + 1] << 4u);
        result ^= ((PXI32U)data[pos + 2] << 8u);
    }
    else
    {
        PXSize amount;
        if(pos >= size) 
            return 0;
        amount = size - pos;

        for(PXSize i = 0; i != amount; ++i) 
            result ^= ((PXI32U)data[pos + i] << (i * 8u));
    }
    return result & HASH_BIT_MASK;
}



/*dynamic vector of unsigned chars*/
typedef struct ucvector
{
    unsigned char* data;
    PXSize size; /*used size*/
    PXSize allocsize; /*allocated size*/
} ucvector;

/*returns 1 if success, 0 if failure ==> nothing done*/
unsigned ucvector_reserve(ucvector* p, PXSize size)
{
    if(size > p->allocsize)
    {
        PXSize newsize = size + (p->allocsize >> 1u);
        void* data = PXMemoryHeapRealloc(PXNull, p->data, newsize);
        if(data)
        {
            p->allocsize = newsize;
            p->data = (unsigned char*)data;
        }
        else return 0; /*error: not enough memory*/
    }
    return 1; /*success*/
}

/*returns 1 if success, 0 if failure ==> nothing done*/
unsigned ucvector_resize(ucvector* p, PXSize size)
{
    p->size = size;
    return ucvector_reserve(p, size);
}

ucvector ucvector_init(unsigned char* buffer, PXSize size)
{
    ucvector v;
    v.data = buffer;
    v.allocsize = v.size = size;
    return v;
}



typedef struct LodePNGBitWriter_
{
    ucvector* data;
    unsigned char bp; /*ok to overflow, indicates bit pos inside byte*/
} LodePNGBitWriter;

void LodePNGBitWriter_init(LodePNGBitWriter* writer, ucvector* data)
{
    writer->data = data;
    writer->bp = 0;
}

/*TODO: this ignores potential out of memory errors*/
#define WRITEBIT(writer, bit){\
  /* append new byte */\
  if(((writer->bp) & 7u) == 0) {\
    if(!ucvector_resize(writer->data, writer->data->size + 1)) return;\
    writer->data->data[writer->data->size - 1] = 0;\
  }\
  (writer->data->data[writer->data->size - 1]) |= (bit << ((writer->bp) & 7u));\
  ++writer->bp;\
}

/* LSB of value is written first, and LSB of bytes is used first */
void PNGwriteBits(LodePNGBitWriter* writer, PXSize value, PXSize nbits)
{
    if(nbits == 1)
    {
        /* compiler should statically compile this case if nbits == 1 */
        WRITEBIT(writer, value);
    }
    else
    {
        /* TODO: increase output size only once here rather than in each WRITEBIT */
        for(PXSize i = 0; i != nbits; ++i)
        {
            WRITEBIT(writer, (unsigned char)((value >> i) & 1));
        }
    }
}

/* This one is to use for adding huffman symbol, the value bits are written MSB first */
void writeBitsReversed(LodePNGBitWriter* writer, PXSize value, PXSize nbits)
{
    for(PXSize i = 0; i != nbits; ++i)
    {
        /* TODO: increase output size only once here rather than in each WRITEBIT */
        WRITEBIT(writer, (unsigned char)((value >> (nbits - 1u - i)) & 1u));
    }
}

#define NUM_CODE_LENGTH_CODES 19




void PXAPI addLengthDistance(uivector* values, const PXSize length, const PXSize distance);
void updateHashChain(Hash* hash, PXSize wpos, unsigned hashval, unsigned short numzeros);
PXSize countZeros(const unsigned char* data, PXSize size, PXSize pos);

void writeLZ77data(LodePNGBitWriter* writer, const uivector* lz77_encoded, const PXHuffmanTree* tree_ll, const PXHuffmanTree* tree_d);
unsigned encodeLZ77(uivector* out, Hash* hash, const unsigned char* in, PXSize inpos, PXSize insize, PXSize windowsize, PXSize minmatch, PXSize nicematch, PXSize lazymatching);





PXResult PXAPI deflateFixed
(
    PXFile PXREF pxFile,
    Hash* hash,
    const void PXREF data,
    PXSize datapos,
    PXSize dataend,
    const LodePNGCompressSettings* settings
)
{
    LodePNGBitWriter writerThing;

    LodePNGBitWriter* writer = &writerThing;
    ucvector aaucvector;

    aaucvector.data = PXFileDataAtCursor(pxFile);
    aaucvector.size = 0;
    aaucvector.allocsize = PXFileSizeToRead(pxFile);

    LodePNGBitWriter_init(writer, &aaucvector);
    writer->bp = PXFileCursorOffsetGetBit(pxFile) + 16u;

    //--------------------------


    PXSize error = 0;

    PXHuffmanTree tree_ll; /*tree for literal values and length codes*/
    PXHuffmanTree tree_d; /*tree for distance codes*/

    PXHuffmanDistanceTreeGenerateFixed(&tree_ll, &tree_d);

    if(!error)
    {
        if(settings->use_lz77) /*LZ77 encoded*/
        {
            uivector lz77_encoded;
            uivector_init(&lz77_encoded);
            error = encodeLZ77(&lz77_encoded, hash, (unsigned char*)data, datapos, dataend, settings->windowsize,
                               settings->minmatch, settings->nicematch, settings->lazymatching);
            if(!error)
                writeLZ77data(writer, &lz77_encoded, &tree_ll, &tree_d);

            uivector_cleanup(&lz77_encoded);

            //PXLZ77ESetting pxLZ77ESetting;
            //pxLZ77ESetting.Windowsize = settings->windowsize;
            //pxLZ77ESetting.Minmatch = settings->minmatch;
            //pxLZ77ESetting.Nicematch = settings->nicematch;
            //pxLZ77ESetting.Lazymatching = settings->lazymatching;

            //error = PXLZ77Encode(writer, hash, data, datapos, dataend, &tree_ll, &tree_d, &pxLZ77ESetting);
        }
        else /*no LZ77, but still will be Huffman compressed*/
        {
            for(PXSize i = datapos; i < dataend; ++i)
            {
                PXI8U codeIndex = ((PXI8U*)data)[i];

                writeBitsReversed(writer, tree_ll.CodeSymbols[codeIndex], tree_ll.LengthsList[codeIndex]);
            }
        }
        /*add END code*/
        if(!error)
            writeBitsReversed(writer, tree_ll.CodeSymbols[256], tree_ll.LengthsList[256]);
    }

    /*cleanup*/
    PXHuffmanTreeDestruct(&tree_ll);
    PXHuffmanTreeDestruct(&tree_d);

    PXFileCursorOffsetSetBit(pxFile, writer->bp);
    PXFileCursorAdvance(pxFile, aaucvector.size);

    //return error;
    return PXResultOK;
}


PXSize countZeros(const unsigned char* data, PXSize size, PXSize pos)
{
    const unsigned char* start = data + pos;
    const unsigned char* end = start + MAX_SUPPORTED_DEFLATE_LENGTH;
    if(end > data + size) end = data + size;
    data = start;
    while(data != end && *data == 0) ++data;
    /*subtracting two addresses returned as 32-bit number (max value is MAX_SUPPORTED_DEFLATE_LENGTH)*/
    return (unsigned)(data - start);
}

/*wpos = pos & (windowsize - 1)*/
void updateHashChain(Hash* hash, PXSize wpos, unsigned hashval, unsigned short numzeros)
{
    hash->val[wpos] = (int)hashval;
    if(hash->head[hashval] != -1) hash->chain[wpos] = hash->head[hashval];
    hash->head[hashval] = (int)wpos;

    hash->zeros[wpos] = numzeros;
    if(hash->headz[numzeros] != -1) hash->chainz[wpos] = hash->headz[numzeros];
    hash->headz[numzeros] = (int)wpos;
}

PXI32U PXAPI searchCodeIndexI8(const PXI8U* array, PXSize array_size, PXSize value)
{
    // binary search (only small gain over linear). TODO: use CPU log2 instruction for getting symbols instead
    PXSize left = 1;
    PXSize right = array_size - 1;

    while(left <= right)
    {
        PXSize mid = (left + right) >> 1;

        PXI16U vvvvak = ((PXI16U)array[mid]) + 3;

        if(vvvvak >= value)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    PXI16U wwww = ((PXI16U)array[left]) + 3;

    if(left >= array_size || wwww > value)
        left--;

    return left;
}

PXI32U PXAPI searchCodeIndexI16(const PXI16U* array, PXSize array_size, PXSize value)
{
    // binary search (only small gain over linear). TODO: use CPU log2 instruction for getting symbols instead
    PXSize left = 1;
    PXSize right = array_size - 1;

    while(left <= right)
    {
        PXSize mid = (left + right) >> 1;
       
        if(array[mid] >= value) 
            right = mid - 1;

        else left = mid + 1;
    }
    
    if(left >= array_size || array[left] > value)
        left--;
   
    return left;
}


void PXAPI addLengthDistance(uivector* values, const PXSize length, const PXSize distance)
{
    /*values in encoded vector are those used by deflate:
    0-255: literal bytes
    256: end
    257-285: length/distance pair (length code, followed by extra length bits, distance code, extra distance bits)
    286-287: invalid*/

    PXI32U length_code = searchCodeIndexI8(PXHuffmanLengthBase, 29, length);
    PXI32U extra_length = (length - PXHuffmanLengthBaseGet(length_code));
    PXI32U dist_code = searchCodeIndexI16(PXHuffmanDistanceBase, 30, distance);
    PXI32U extra_distance = (distance - PXHuffmanDistanceBase[dist_code]);

    PXSize pos = values->size;
    /*TODO: return error when this fails (out of memory)*/
    unsigned ok = uivector_resize(values, values->size + 4);
    if(ok)
    {
        values->data[pos + 0] = length_code + FIRST_LENGTH_CODE_INDEX;
        values->data[pos + 1] = extra_length;
        values->data[pos + 2] = dist_code;
        values->data[pos + 3] = extra_distance;
    }
}










/*BPM: Boundary Package Merge, see "A Fast and Space-Economical Algorithm for Length-Limited Coding",
Jyrki Katajainen, Alistair Moffat, Andrew Turpin, 1995.*/

/*chain node for boundary package merge*/
typedef struct BPMNode
{
    int weight; /*the sum of all weights in this chain*/
    unsigned index; /*index of this leaf node (called "count" in the paper)*/
    struct BPMNode* tail; /*the next nodes in this chain (null if last)*/
    int in_use;
} BPMNode;

/*lists of chains*/
typedef struct BPMLists
{
    /*memory pool*/
    unsigned memsize;
    BPMNode* memory;
    unsigned numfree;
    unsigned nextfree;
    BPMNode** freelist;
    /*two heads of lookahead chains per list*/
    unsigned listsize;
    BPMNode** chains0;
    BPMNode** chains1;
} BPMLists;

/*creates a new chain node with the given parameters, from the memory in the lists */
BPMNode* bpmnode_create(BPMLists* lists, int weight, unsigned index, BPMNode* tail)
{
    unsigned i;
    BPMNode* result;

    /*memory full, so garbage collect*/
    if(lists->nextfree >= lists->numfree)
    {
        /*mark only those that are in use*/
        for(i = 0; i != lists->memsize; ++i) lists->memory[i].in_use = 0;
        for(i = 0; i != lists->listsize; ++i)
        {
            BPMNode* node;
            for(node = lists->chains0[i]; node != 0; node = node->tail) node->in_use = 1;
            for(node = lists->chains1[i]; node != 0; node = node->tail) node->in_use = 1;
        }
        /*collect those that are free*/
        lists->numfree = 0;
        for(i = 0; i != lists->memsize; ++i)
        {
            if(!lists->memory[i].in_use) lists->freelist[lists->numfree++] = &lists->memory[i];
        }
        lists->nextfree = 0;
    }

    result = lists->freelist[lists->nextfree++];
    result->weight = weight;
    result->index = index;
    result->tail = tail;
    return result;
}

// sort the leaves with stable mergesort
void bpmnode_sort(BPMNode PXREF leaves, const PXSize num)
{
    BPMNode* mem = PXMemoryHeapCallocT(BPMNode, num);

    PXSize width, counter = 0;
    for(width = 1; width < num; width *= 2)
    {
        BPMNode* a = (counter & 1) ? mem : leaves;
        BPMNode* b = (counter & 1) ? leaves : mem;

        for(PXSize p = 0; p < num; p += 2 * width)
        {
            PXSize q = (p + width > num) ? num : (p + width);
            PXSize r = (p + 2 * width > num) ? num : (p + 2 * width);
            PXSize i = p, j = q, k;
            for(k = p; k < r; k++)
            {
                if(i < q && (j >= r || a[i].weight <= a[j].weight))
                    b[k] = a[i++];
                else 
                    b[k] = a[j++];
            }
        }
        counter++;
    }
    if(counter & 1)
    {
        const PXSize size = sizeof(*leaves) * num;

        PXMemoryCopy(mem, leaves, size);
    }

    PXMemoryHeapFree(PXNull, &mem);
}

/*Boundary Package Merge step, numpresent is the amount of leaves, and c is the current chain.*/
void boundaryPM(BPMLists* lists, BPMNode* leaves, PXSize numpresent, int c, int num)
{
    unsigned lastindex = lists->chains1[c]->index;

    if(c == 0)
    {
        if(lastindex >= numpresent) return;
        lists->chains0[c] = lists->chains1[c];
        lists->chains1[c] = bpmnode_create(lists, leaves[lastindex].weight, lastindex + 1, 0);
    }
    else
    {
        /*sum of the weights of the head nodes of the previous lookahead chains.*/
        int sum = lists->chains0[c - 1]->weight + lists->chains1[c - 1]->weight;
        lists->chains0[c] = lists->chains1[c];
        if(lastindex < numpresent && sum > leaves[lastindex].weight)
        {
            lists->chains1[c] = bpmnode_create(lists, leaves[lastindex].weight, lastindex + 1, lists->chains1[c]->tail);
            return;
        }
        lists->chains1[c] = bpmnode_create(lists, sum, lastindex, lists->chains1[c - 1]);
        /*in the end we are only interested in the chain of the last list, so no
        need to recurse if we're at the last one (this gives measurable speedup)*/
        if(num + 1 < (int)(2 * numpresent - 2))
        {
            boundaryPM(lists, leaves, numpresent, c - 1, num);
            boundaryPM(lists, leaves, numpresent, c - 1, num);
        }
    }
}


unsigned HuffmanTree_makeTable(PXHuffmanTree* tree)
{
    static const unsigned headsize = 1u << PXHuffmanFirstBits; /*size of the first table*/
    static const unsigned mask = (1u << PXHuffmanFirstBits) /*headsize*/ - 1u;
    PXSize i, numpresent, pointer, size; /*total table size*/
    PXI32U* maxlens = PXMemoryHeapCallocT(PXI32U, headsize);

    if(!maxlens) 
        return 83; /*alloc fail*/

    /* compute maxlens: max total bit length of symbols sharing prefix in the first table*/
    PXMemoryClear(maxlens, headsize * sizeof(*maxlens));
    for(i = 0; i < tree->NumberOfSymbols; i++)
    {
        PXSize symbol = tree->CodeSymbols[i];
        PXSize l = tree->LengthsList[i];
        PXSize index;
        if(l <= PXHuffmanFirstBits) continue; /*symbols that fit in first table don't increase secondary table size*/
        /*get the FIRSTBITS MSBs, the MSBs of the symbol are encoded first. See later comment about the reversing*/
        index = reverseBits(symbol >> (l - PXHuffmanFirstBits), PXHuffmanFirstBits);
        maxlens[index] = PXMathMaximum(maxlens[index], l);
    }
    /* compute total table size: size of first table plus all secondary tables for symbols longer than FIRSTBITS */
    size = headsize;
    for(i = 0; i < headsize; ++i)
    {
        unsigned int l = maxlens[i];

        if(l > PXHuffmanFirstBits)
            size += (1u << (l - PXHuffmanFirstBits));
    }

    tree->TableLength = PXMemoryHeapCallocT(PXI8U, size);
    tree->TableValue = PXMemoryHeapCallocT(PXI16U, size);

    if(!tree->TableLength || !tree->TableValue)
    {
        PXMemoryHeapFree(PXNull, maxlens);
        /* freeing tree->table values is done at a higher scope */
        return 83; /*alloc fail*/
    }
    /*initialize with an invalid length to indicate unused entries*/
    //for(i = 0; i < size; ++i) tree->table_len[i] = 16;
    PXMemorySet(tree->TableLength, 16u, size);

    /*fill in the first table for long symbols: max prefix size and pointer to secondary tables*/
    pointer = headsize;
    for(i = 0; i < headsize; ++i)
    {
        PXSize l = maxlens[i];
        if(l <= PXHuffmanFirstBits) continue;
        tree->TableLength[i] = l;
        tree->TableValue[i] = pointer;
        pointer += (1u << (l - PXHuffmanFirstBits));
    }

    PXMemoryHeapFree(PXNull, maxlens);

    /*fill in the first table for short symbols, or secondary table for long symbols*/
    numpresent = 0;
    for(i = 0; i < tree->NumberOfSymbols; ++i)
    {
        PXSize l = tree->LengthsList[i];
        PXSize symbol, reverse;
        if(l == 0) continue;
        symbol = tree->CodeSymbols[i]; /*the huffman bit pattern. i itself is the value.*/
        /*reverse bits, because the huffman bits are given in MSB first order but the bit reader reads LSB first*/
        reverse = reverseBits(symbol, l);
        numpresent++;

        if(l <= PXHuffmanFirstBits)
        {
            /*short symbol, fully in first table, replicated num times if l < FIRSTBITS*/
            PXSize num = 1u << (PXHuffmanFirstBits - l);
            PXSize j;
            for(j = 0; j < num; ++j)
            {
                /*bit reader will read the l bits of symbol first, the remaining FIRSTBITS - l bits go to the MSB's*/
                PXSize index = reverse | (j << l);
                if(tree->TableLength[index] != 16) return 55; /*invalid tree: long symbol shares prefix with short symbol*/
                tree->TableLength[index] = l;
                tree->TableValue[index] = i;
            }
        }
        else
        {
            /*long symbol, shares prefix with other long symbols in first lookup table, needs second lookup*/
            /*the FIRSTBITS MSBs of the symbol are the first table index*/
            PXSize index = reverse & mask;
            PXSize maxlen = tree->TableLength[index];
            /*log2 of secondary table length, should be >= l - FIRSTBITS*/
            PXSize tablelen = maxlen - PXHuffmanFirstBits;
            PXSize start = tree->TableValue[index]; /*starting index in secondary table*/
            PXSize num = 1u << (tablelen - (l - PXHuffmanFirstBits)); /*amount of entries of this symbol in secondary table*/
           
            if(maxlen < l)
                return 55; /*invalid tree: long symbol shares prefix with short symbol*/
           
            for(PXSize j = 0; j < num; ++j)
            {
                PXSize reverse2 = reverse >> PXHuffmanFirstBits; /* l - FIRSTBITS bits */
                PXSize index2 = start + (reverse2 | (j << (l - PXHuffmanFirstBits)));
                tree->TableLength[index2] = l;
                tree->TableValue[index2] = i;
            }
        }
    }

    if(numpresent < 2)
    {
        /* In case of exactly 1 symbol, in theory the huffman symbol needs 0 bits,
        but deflate uses 1 bit instead. In case of 0 symbols, no symbols can
        appear at all, but such huffman tree could still exist (e.g. if distance
        codes are never used). In both cases, not all symbols of the table will be
        filled in. Fill them in with an invalid symbol value so returning them from
        huffmanDecodeSymbol will cause error. */
        for(i = 0; i < size; ++i)
        {
            if(tree->TableLength[i] == 16)
            {
                /* As length, use a value smaller than FIRSTBITS for the head table,
                and a value larger than FIRSTBITS for the secondary table, to ensure
                valid behavior for advanceBits when reading this symbol. */
                tree->TableLength[i] = (i < headsize) ? 1 : (PXHuffmanFirstBits + 1);
                tree->TableValue[i] = INVALIDSYMBOL;
            }
        }
    }
    else
    {
        /* A good huffman tree has N * 2 - 1 nodes, of which N - 1 are CPrivate nodes.
        If that is not the case (due to too long length codes), the table will not
        have been fully used, and this is an error (not all bit combinations can be
        decoded): an oversubscribed huffman tree, indicated by error 55. */
        for(i = 0; i < size; ++i)
        {
            if(tree->TableLength[i] == 16) 
                return 55;
        }
    }

    return 0;
}

static unsigned HuffmanTree_makeFromLengths2(PXHuffmanTree PXREF tree)
{
    PXSize error = 0;

    tree->CodeSymbols = PXMemoryHeapCallocT(PXI32U, tree->NumberOfSymbols);
    PXI32U* blcount = PXMemoryHeapCallocT(PXI32U, (tree->maxbitlen + 1));
    PXI32U* nextcode = PXMemoryHeapCallocT(PXI32U, (tree->maxbitlen + 1));

    if(!tree->CodeSymbols || !blcount || !nextcode) error = 83; /*alloc fail*/

    if(!error)
    {
        for(PXSize n = 0; n != tree->maxbitlen + 1; n++) blcount[n] = nextcode[n] = 0;
        /*step 1: count number of instances of each code length*/
        for(PXSize bits = 0; bits != tree->NumberOfSymbols; ++bits) ++blcount[tree->LengthsList[bits]];
        /*step 2: generate the nextcode values*/
        for(PXSize bits = 1; bits <= tree->maxbitlen; ++bits)
        {
            nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1u;
        }
        /*step 3: generate all the codes*/
        for(PXSize n = 0; n != tree->NumberOfSymbols; ++n)
        {
            if(tree->LengthsList[n] != 0)
            {
                tree->CodeSymbols[n] = nextcode[tree->LengthsList[n]]++;
                /*remove superfluous bits from the code*/
                tree->CodeSymbols[n] &= ((1u << tree->LengthsList[n]) - 1u);
            }
        }
    }

    PXMemoryHeapFree(PXNull, blcount);
    PXMemoryHeapFree(PXNull, nextcode);

    if(!error)
        error = HuffmanTree_makeTable(tree);

    return error;
}

unsigned lodepng_huffman_code_lengths(unsigned* lengths, const unsigned* frequencies, PXSize numcodes, unsigned maxbitlen)
{
    unsigned error = 0;
    unsigned i;
    PXSize numpresent = 0; /*number of symbols with non-zero frequency*/
    BPMNode* leaves; /*the symbols, only those with > 0 frequency*/

    if(numcodes == 0) 
        return 80; /*error: a tree of 0 symbols is not supposed to be made*/
   
    if((1u << maxbitlen) < (unsigned)numcodes) 
        return 80; /*error: represent all symbols*/

    leaves = PXMemoryHeapCallocT(BPMNode, numcodes);

    if(!leaves) return 83; /*alloc fail*/

    for(i = 0; i != numcodes; ++i)
    {
        if(frequencies[i] > 0)
        {
            leaves[numpresent].weight = (int)frequencies[i];
            leaves[numpresent].index = i;
            ++numpresent;
        }
    }

    PXMemoryClear(lengths, numcodes * sizeof(*lengths));

    /*ensure at least two present symbols. There should be at least one symbol
    according to RFC 1951 section 3.2.7. Some decoders incorrectly require two. To
    make these work as well ensure there are at least two symbols. The
    Package-Merge code below also doesn't work correctly if there's only one
    symbol, it'd give it the theoretical 0 bits but in practice PXZLIB wants 1 bit*/
    if(numpresent == 0)
    {
        lengths[0] = lengths[1] = 1; /*note that for RFC 1951 section 3.2.7, only lengths[0] = 1 is needed*/
    }
    else if(numpresent == 1)
    {
        lengths[leaves[0].index] = 1;
        lengths[leaves[0].index == 0 ? 1 : 0] = 1;
    }
    else
    {
        BPMLists lists;
        BPMNode* node;

        bpmnode_sort(leaves, numpresent);

        lists.listsize = maxbitlen;
        lists.memsize = 2 * maxbitlen * (maxbitlen + 1);
        lists.nextfree = 0;
        lists.numfree = lists.memsize;
        lists.memory = PXMemoryHeapCallocT(BPMNode, lists.memsize);
        lists.freelist = PXMemoryHeapCallocT(BPMNode*, lists.memsize);
        lists.chains0 = PXMemoryHeapCallocT(BPMNode*, lists.listsize);
        lists.chains1 = PXMemoryHeapCallocT(BPMNode*, lists.listsize);
        if(!lists.memory || !lists.freelist || !lists.chains0 || !lists.chains1) error = 83; /*alloc fail*/

        if(!error)
        {
            for(i = 0; i != lists.memsize; ++i) lists.freelist[i] = &lists.memory[i];

            bpmnode_create(&lists, leaves[0].weight, 1, 0);
            bpmnode_create(&lists, leaves[1].weight, 2, 0);

            for(i = 0; i != lists.listsize; ++i)
            {
                lists.chains0[i] = &lists.memory[0];
                lists.chains1[i] = &lists.memory[1];
            }

            /*each boundaryPM call adds one chain to the last list, and we need 2 * numpresent - 2 chains.*/
            for(i = 2; i != 2 * numpresent - 2; ++i) 
                boundaryPM(&lists, leaves, numpresent, (int)maxbitlen - 1, (int)i);

            for(node = lists.chains1[maxbitlen - 1]; node; node = node->tail)
            {
                for(i = 0; i != node->index; ++i) 
                    ++lengths[leaves[i].index];
            }
        }

        PXMemoryHeapFree(PXNull, lists.memory);
        PXMemoryHeapFree(PXNull, lists.freelist);
        PXMemoryHeapFree(PXNull, lists.chains0);
        PXMemoryHeapFree(PXNull, lists.chains1);
    }

    PXMemoryHeapFree(PXNull, leaves);
    return error;
}

unsigned HuffmanTree_makeFromFrequencies(PXHuffmanTree* tree, const unsigned* frequencies, PXSize mincodes, PXSize numcodes, unsigned maxbitlen)
{
    unsigned error = 0;

    while(!frequencies[numcodes - 1] && numcodes > mincodes)
        --numcodes; // trim zeroes

    tree->LengthsList = PXMemoryHeapCallocT(PXI32U, numcodes);

    if(!tree->LengthsList) 
        return 83; // alloc fail
   
    tree->maxbitlen = maxbitlen;
    tree->NumberOfSymbols = numcodes; /*number of symbols*/

    error = lodepng_huffman_code_lengths(tree->LengthsList, frequencies, numcodes, maxbitlen);
   
    if(!error) 
        error = HuffmanTree_makeFromLengths2(tree);

    return error;
}



PXResult PXAPI deflateDynamic
(
    PXFile PXREF pxFile,
    Hash* hash,
    const void PXREF data,
    PXSize datapos,
    PXSize dataend,
    const LodePNGCompressSettings* settings,
    unsigned char BFINAL
)
{

    LodePNGBitWriter writerThing;

    LodePNGBitWriter* writer = &writerThing;
    ucvector aaucvector;

    aaucvector.data = PXFileDataAtCursor(pxFile);
    aaucvector.size = 0;
    aaucvector.allocsize = PXFileSizeToRead(pxFile);

    LodePNGBitWriter_init(writer, &aaucvector);
    writer->bp = PXFileCursorOffsetGetBit(pxFile);// +(2 * 8u);

    //-------------------------




    unsigned error = 0;

    /*
    A block is compressed as follows: The PNG data is lz77 encoded, resulting in
    literal bytes and length/distance pairs. This is then huffman compressed with
    two huffman trees. One huffman tree is used for the lit and len values ("ll"),
    another huffman tree is used for the dist values ("d"). These two trees are
    stored using their code lengths, and to compress even more these code lengths
    are also run-length encoded and huffman compressed. This gives a huffman tree
    of code lengths "cl". The code lengths used to describe this third tree are
    the code length code lengths ("clcl").
    */

    /*The lz77 encoded data, represented with integers since there will also be length and distance codes in it*/
    uivector lz77_encoded;
    uivector_init(&lz77_encoded);
    //PXFile lz77CacheStream;     PXFileConstruct(&lz77CacheStream);


    PXHuffmanTree tree_ll; /*tree for lit,len values*/
    PXHuffmanTree tree_d; /*tree for distance codes*/
    PXHuffmanTree tree_cl; /*tree for encoding the code lengths representing tree_ll and tree_d*/
    PXI32U frequencies_ll[286]; /*frequency of lit,len codes*/
    PXI32U frequencies_d[30]; /*frequency of dist codes*/
    PXI32U frequencies_cl[NUM_CODE_LENGTH_CODES]; /*frequency of code length codes*/
    PXI32U* bitlen_lld = 0; /*lit,len,dist code lengths (int bits), literally (without repeat codes).*/
    PXI32U* bitlen_lld_e = 0; /*bitlen_lld encoded with repeat codes (this is a rudimentary run length compression)*/
    PXSize datasize = dataend - datapos;

    /*
    If we could call "bitlen_cl" the the code length code lengths ("clcl"), that is the bit lengths of codes to represent
    tree_cl in CLCL_ORDER, then due to the huffman compression of huffman tree representations ("two levels"), there are
    some analogies:
    bitlen_lld is to tree_cl what data is to tree_ll and tree_d.
    bitlen_lld_e is to bitlen_lld what lz77_encoded is to data.
    bitlen_cl is to bitlen_lld_e what bitlen_lld is to lz77_encoded.
    */

    PXSize i;
    PXSize numcodes_ll, numcodes_d, numcodes_lld, numcodes_lld_e, numcodes_cl;
    PXSize HLIT, HDIST, HCLEN;


    PXClear(PXHuffmanTree, &tree_ll);
    PXClear(PXHuffmanTree, &tree_d);
    PXClear(PXHuffmanTree, &tree_cl);
    /* could fit on stack, but >1KB is on the larger side so allocate instead */

    /*This while loop never loops due to a break at the end, it is here to
    allow breaking out of it to the cleanup phase on error conditions.*/
    while(!error)
    {
        PXClearList(PXI32U, frequencies_ll, 286);
        PXClearList(PXI32U, frequencies_d, 30);
        PXClearList(PXI32U, frequencies_cl, NUM_CODE_LENGTH_CODES);

        if(settings->use_lz77)
        {
            //PXLZ77ESetting pxLZ77ESetting;
            //pxLZ77ESetting.Windowsize = settings->windowsize;
            //pxLZ77ESetting.Minmatch = settings->minmatch;
            //pxLZ77ESetting.Nicematch = settings->nicematch;
            //pxLZ77ESetting.Lazymatching = settings->lazymatching;

            //error = PXLZ77Encode(&lz77CacheStream, hash, data, datapos, dataend, &tree_ll, &tree_d, &pxLZ77ESetting);

            error = encodeLZ77(&lz77_encoded, hash, (unsigned char*)data, datapos, dataend, settings->windowsize, settings->minmatch, settings->nicematch, settings->lazymatching);
            if(error) break;
        }
        else
        {

            if(!uivector_resize(&lz77_encoded, datasize)) ERROR_BREAK(83 /*alloc fail*/);
            for(i = datapos; i < dataend; ++i) lz77_encoded.data[i - datapos] = ((char*)data)[i]; /*no LZ77, but still will be Huffman compressed*/

            //if(!uivector_resize(&lz77_encoded, datasize)) ERROR_BREAK(83 /*alloc fail*/);
            // for (PXSize index = datapos; index < dataend; ++index) // Data must be atleast "datasize"
            // {
            //     ((PXAdress)lz77CacheStream.Data)[index - datapos] = data[index]; // no LZ77, but still will be Huffman compressed
            // }
        }

        /*Count the frequencies of lit, len and dist codes*/
        /* for (i = 0; i != lz77CacheStream.DataSize; ++i)
         {
             unsigned int symbol = ((PXAdress)lz77CacheStream.Data)[i];
             ++frequencies_ll[symbol];
             if(symbol > 256)
             {
                 unsigned int dist = ((PXAdress)lz77CacheStream.Data)[i + 2];
                 ++frequencies_d[dist];
                 i += 3;
             }
         }*/
        for(i = 0; i != lz77_encoded.size; ++i)
        {
            unsigned symbol = lz77_encoded.data[i];
            ++frequencies_ll[symbol];
            if(symbol > 256)
            {
                unsigned dist = lz77_encoded.data[i + 2];
                ++frequencies_d[dist];
                i += 3;
            }
        }


        frequencies_ll[256] = 1; /*there will be exactly 1 end code, at the end of the block*/

        /*Make both huffman trees, one for the lit and len codes, one for the dist codes*/
        error = HuffmanTree_makeFromFrequencies(&tree_ll, frequencies_ll, 257, 286, 15);
       
        if(error)
            break;

        /*2, not 1, is chosen for mincodes: some buggy PNG decoders require at least 2 symbols in the dist tree*/
        error = HuffmanTree_makeFromFrequencies(&tree_d, frequencies_d, 2, 30, 15);
       
        if(error)
            break;

        numcodes_ll = PXMathMinimumIU(tree_ll.NumberOfSymbols, 286);
        numcodes_d = PXMathMinimumIU(tree_d.NumberOfSymbols, 30);
        // store the code lengths of both generated trees in bitlen_lld
        numcodes_lld = numcodes_ll + numcodes_d;

        bitlen_lld = PXMemoryHeapCallocT(PXI32U, numcodes_lld);
        // numcodes_lld_e never needs more size than bitlen_lld
        bitlen_lld_e = PXMemoryHeapCallocT(PXI32U, numcodes_lld);      
        
        if(!bitlen_lld || !bitlen_lld_e) 
            ERROR_BREAK(83); /*alloc fail*/

        numcodes_lld_e = 0;

        for(i = 0; i != numcodes_ll; ++i) 
            bitlen_lld[i] = tree_ll.LengthsList[i];

        for(i = 0; i != numcodes_d; ++i) 
            bitlen_lld[numcodes_ll + i] = tree_d.LengthsList[i];

        /*run-length compress bitlen_ldd into bitlen_lld_e by using repeat codes 16 (copy length 3-6 times),
        17 (3-10 zeroes), 18 (11-138 zeroes)*/
        for(i = 0; i != numcodes_lld; ++i)
        {
            unsigned j = 0; /*amount of repetitions*/
            while(i + j + 1 < numcodes_lld && bitlen_lld[i + j + 1] == bitlen_lld[i]) ++j;

            if(bitlen_lld[i] == 0 && j >= 2) /*repeat code for zeroes*/
            {
                ++j; /*Included the first zero*/
                if(j <= 10) /*repeat code 17 supports max 10 zeroes*/
                {
                    bitlen_lld_e[numcodes_lld_e++] = 17;
                    bitlen_lld_e[numcodes_lld_e++] = j - 3;
                }
                else /*repeat code 18 supports max 138 zeroes*/
                {
                    if(j > 138) j = 138;
                    bitlen_lld_e[numcodes_lld_e++] = 18;
                    bitlen_lld_e[numcodes_lld_e++] = j - 11;
                }
                i += (j - 1);
            }
            else if(j >= 3) /*repeat code for value other than zero*/
            {
                PXSize k;
                unsigned num = j / 6u, rest = j % 6u;
                bitlen_lld_e[numcodes_lld_e++] = bitlen_lld[i];
                for(k = 0; k < num; ++k)
                {
                    bitlen_lld_e[numcodes_lld_e++] = 16;
                    bitlen_lld_e[numcodes_lld_e++] = 6 - 3;
                }
                if(rest >= 3)
                {
                    bitlen_lld_e[numcodes_lld_e++] = 16;
                    bitlen_lld_e[numcodes_lld_e++] = rest - 3;
                }
                else j -= rest;
                i += j;
            }
            else /*too short to benefit from repeat code*/
            {
                bitlen_lld_e[numcodes_lld_e++] = bitlen_lld[i];
            }
        }

        /*generate tree_cl, the huffmantree of huffmantrees*/
        for(i = 0; i != numcodes_lld_e; ++i)
        {
            ++frequencies_cl[bitlen_lld_e[i]];
            /*after a repeat code come the bits that specify the number of repetitions,
            those don't need to be in the frequencies_cl calculation*/
            if(bitlen_lld_e[i] >= 16) ++i;
        }

        error = HuffmanTree_makeFromFrequencies(&tree_cl, frequencies_cl, NUM_CODE_LENGTH_CODES, NUM_CODE_LENGTH_CODES, 7);
     
        if(error)
            break;

        /*compute amount of code-length-code-lengths to output*/
        numcodes_cl = NUM_CODE_LENGTH_CODES;
        /*trim zeros at the end (using CLCL_ORDER), but minimum size must be 4 (see HCLEN below)*/
        while(numcodes_cl > 4u && tree_cl.LengthsList[PXHuffmanCLCL_ORDER[numcodes_cl - 1u]] == 0)
        {
            numcodes_cl--;
        }

        /*
        Write everything into the output

        After the BFINAL and BTYPE, the dynamic block consists out of the following:
        - 5 bits HLIT, 5 bits HDIST, 4 bits HCLEN
        - (HCLEN+4)*3 bits code lengths of code length alphabet
        - HLIT + 257 code lengths of lit/length alphabet (encoded using the code length
          alphabet, + possible repetition codes 16, 17, 18)
        - HDIST + 1 code lengths of distance alphabet (encoded using the code length
          alphabet, + possible repetition codes 16, 17, 18)
        - compressed data
        - 256 (end code)
        */

        /*Write block type*/
        PNGwriteBits(writer, BFINAL, 1);
        PNGwriteBits(writer, 0, 1); /*first bit of BTYPE "dynamic"*/
        PNGwriteBits(writer, 1, 1); /*second bit of BTYPE "dynamic"*/

        /*write the HLIT, HDIST and HCLEN values*/
        /*all three sizes take trimmed ending zeroes into account, done either by HuffmanTree_makeFromFrequencies
        or in the loop for numcodes_cl above, which saves space. */
        HLIT = (unsigned)(numcodes_ll - 257);
        HDIST = (unsigned)(numcodes_d - 1);
        HCLEN = (unsigned)(numcodes_cl - 4);
        PNGwriteBits(writer, HLIT, 5);
        PNGwriteBits(writer, HDIST, 5);
        PNGwriteBits(writer, HCLEN, 4);

        /*write the code lengths of the code length alphabet ("bitlen_cl")*/
        for(i = 0; i != numcodes_cl; ++i)
            PNGwriteBits(writer, tree_cl.LengthsList[PXHuffmanCLCL_ORDER[i]], 3);

        // write the lengths of the lit/len AND the dist alphabet
        for(i = 0; i != numcodes_lld_e; ++i)
        {
            writeBitsReversed(writer, tree_cl.CodeSymbols[bitlen_lld_e[i]], tree_cl.LengthsList[bitlen_lld_e[i]]);
            // extra bits of repeat codes
            if(bitlen_lld_e[i] == 16)
                PNGwriteBits(writer, bitlen_lld_e[++i], 2);

            else if(bitlen_lld_e[i] == 17) 
                PNGwriteBits(writer, bitlen_lld_e[++i], 3);

            else if(bitlen_lld_e[i] == 18) 
                PNGwriteBits(writer, bitlen_lld_e[++i], 7);
        }

        // write the compressed data symbols
        writeLZ77data(writer, &lz77_encoded, &tree_ll, &tree_d);

        // error: the length of the end code 256 must be larger than 0
        if(tree_ll.LengthsList[256] == 0) 
            ERROR_BREAK(64);

        // write the end code
        writeBitsReversed(writer, tree_ll.CodeSymbols[256], tree_ll.LengthsList[256]);

        break; // end of error-while
    }

    /*cleanup*/
    uivector_cleanup(&lz77_encoded);
    PXHuffmanTreeDestruct(&tree_ll);
    PXHuffmanTreeDestruct(&tree_d);
    PXHuffmanTreeDestruct(&tree_cl);
    PXMemoryHeapFree(PXNull, bitlen_lld);
    PXMemoryHeapFree(PXNull, bitlen_lld_e);

    PXFileCursorOffsetSetBit(pxFile, writer->bp);
    PXFileCursorAdvance(pxFile, aaucvector.size);

  //  return error;
    return PXResultOK;
}

#define DEFAULT_WINDOWSIZE 2048

PXResult PXAPI PXDEFLATESerialize(PXFile PXREF pxInputStream, PXFile PXREF pxOutputStream)
{
    PXSize blocksize;
    Hash hash;

    LodePNGCompressSettings lodePNGCompressSettings;

    lodePNGCompressSettings.btype = 2;
    lodePNGCompressSettings.use_lz77 = 1;
    lodePNGCompressSettings.windowsize = DEFAULT_WINDOWSIZE;
    lodePNGCompressSettings.minmatch = 3;
    lodePNGCompressSettings.nicematch = 128;
    lodePNGCompressSettings.lazymatching = 1;
    lodePNGCompressSettings.custom_PXZLIB = 0;
    lodePNGCompressSettings.custom_deflate = 0;
    lodePNGCompressSettings.custom_context = 0;

    PXI8U deflateEncodingMethod = PXDeflateEncodingHuffmanDynamic;// DeflateEncodingHuffmanDynamic;

    switch(deflateEncodingMethod)
    {
        case PXDeflateEncodingLiteralRaw:
        {
            const PXSize numdeflateblocks = (PXFileSizeToRead(pxInputStream) + 65534u) / 65535u;
            PXSize datapos = 0;

            for(PXSize i = 0; i != numdeflateblocks; ++i)
            {
                const PXBool BFINAL = (i == numdeflateblocks - 1);
                const PXI16U chunkLength = PXMathMinimumIU(65535, PXFileSizeToRead(pxInputStream) - datapos);
                const PXI16U chunkLengthNegated = 65535 - chunkLength;

                const PXByte firstbyte = BFINAL;//(unsigned char)(BFINAL + ((BTYPE & 1u) << 1u) + ((BTYPE & 2u) << 1u));

                PXFileWriteI8U(pxOutputStream, firstbyte);
                PXFileWriteI16U(pxOutputStream, chunkLength);
                PXFileWriteI16U(pxOutputStream, chunkLengthNegated);

                

                PXFileWriteB(pxOutputStream, (PXAdress)PXFileDataPosition(pxOutputStream) + datapos, chunkLength);

                datapos += chunkLength;
            }
        }
        case PXDeflateEncodingHuffmanStatic:
        case PXDeflateEncodingHuffmanDynamic:
        {
            switch(deflateEncodingMethod)
            {
                case PXDeflateEncodingHuffmanStatic:
                {
                    blocksize = PXFileSizeToRead(pxInputStream);
                    break;
                }
                case PXDeflateEncodingHuffmanDynamic:
                {
                    /*on PNGs, deflate blocks of 65-262k seem to give most dense encoding*/
                    blocksize = PXFileSizeToRead(pxInputStream) / 8u + 8u;
                    
                    if(blocksize < 65536) 
                        blocksize = 65536;

                    if(blocksize > 262144) 
                        blocksize = 262144;

                    break;
                }
            }

            PXSize numdeflateblocks = (PXFileSizeToRead(pxInputStream) + blocksize - 1) / blocksize;

            if(numdeflateblocks == 0) 
                numdeflateblocks = 1;

            PXResult error = hash_init(&hash, lodePNGCompressSettings.windowsize);

            for(PXSize i = 0; i != numdeflateblocks && (error == 0); ++i)
            {
                const PXSize indexStart = i * blocksize;
                const PXSize indexEnd = PXMathMinimumIU(indexStart + blocksize, PXFileSizeToRead(pxInputStream));
                const PXBool finalBlock = (i == numdeflateblocks - 1);

                // PXFileWriteBits(pxOutputStream, finalBlock, 1u);

                void* input = PXFileDataAtCursor(pxInputStream);

                switch(deflateEncodingMethod)
                {
                    case PXDeflateEncodingHuffmanStatic:
                    {
                        PXFileWriteBits(pxOutputStream, 1u, 2u);

                        error = deflateFixed(pxOutputStream, &hash, input, indexStart, indexEnd, &lodePNGCompressSettings);
                        break;
                    }
                    case PXDeflateEncodingHuffmanDynamic:
                    {
                        //PXFileWriteBits(pxOutputStream, 2u, 2u);
                        error = deflateDynamic(pxOutputStream, &hash, input, indexStart, indexEnd, &lodePNGCompressSettings, finalBlock);
                        break;
                    }
                }
            }

            hash_cleanup(&hash);

            break;
        }
        case PXDeflateEncodingReserverd:
        default:
            return PXActionRefusedFormatSettingNotAllowed;
    }

    return PXResultOK;
}
























PXActionResult PXLZ77Encode
(
    //PXFile PXREF pxOutputStream,
    PXFile PXREF pxCacheStream,
    Hash* hash,
    const unsigned char* in,
    PXSize inpos,
    PXSize insize,
    const PXHuffmanTree* tree_ll,
    const PXHuffmanTree* tree_d,
    const PXLZ77ESetting PXREF pxLZ77ESetting
)
{
    return PXActionRefusedNotImplemented;
}
PXActionResult PXLZ77Decode()
{
    return PXActionRefusedNotImplemented;
}





/*
write the lz77-encoded data, which has lit, len and dist codes, to compressed stream using huffman trees.
tree_ll: the tree for lit and len codes.
tree_d: the tree for distance codes.
*/
void writeLZ77data
(
    LodePNGBitWriter* writer,
    const uivector* lz77_encoded,
    const PXHuffmanTree* tree_ll,
    const PXHuffmanTree* tree_d
)
{
    PXSize i = 0;
    for(i = 0; i != lz77_encoded->size; ++i)
    {
        PXI32U val = lz77_encoded->data[i];
        writeBitsReversed(writer, tree_ll->CodeSymbols[val], tree_ll->LengthsList[val]);
        if(val > 256) /*for a length code, 3 more things have to be added*/
        {
            const PXI32U length_index = val - FIRST_LENGTH_CODE_INDEX;
            const PXI8U n_length_extra_bits = PXHuffmanLengthExtra[length_index];
            PXSize length_extra_bits = lz77_encoded->data[++i];

            const PXSize distance_code = lz77_encoded->data[++i];
            const PXSize distance_index = distance_code;
            const PXI8U n_distance_extra_bits = PXHuffmanDistanceExtra[distance_index];
            PXSize distance_extra_bits = lz77_encoded->data[++i];

            PNGwriteBits(writer, length_extra_bits, n_length_extra_bits);
            writeBitsReversed(writer, tree_d->CodeSymbols[distance_code], tree_d->LengthsList[distance_code]);
            PNGwriteBits(writer, distance_extra_bits, n_distance_extra_bits);
        }
    }
}

unsigned encodeLZ77
(
    uivector* out,
    Hash* hash,
    const unsigned char* in,
    PXSize inpos,
    PXSize insize,
    PXSize windowsize,
    PXSize minmatch,
    PXSize nicematch,
    PXSize lazymatching
)
{
    PXSize pos;
    PXSize i, error = 0;
    /*for large window lengths, assume the user wants no compression loss. Otherwise, max hash chain length speedup.*/
    PXSize maxchainlength = windowsize >= 8192 ? windowsize : windowsize / 8u;
    PXSize maxlazymatch = windowsize >= 8192 ? MAX_SUPPORTED_DEFLATE_LENGTH : 64;

    PXSize usezeros = 1; /*not sure if setting it to false for windowsize < 8192 is better or worse*/
    PXSize numzeros = 0;

    PXSize offset; /*the offset represents the distance in LZ77 terminology*/
    PXSize length;
    PXSize lazy = 0;
    PXSize lazylength = 0, lazyoffset = 0;
    PXSize hashval;
    PXSize current_offset, current_length;
    PXSize prev_offset;
    const unsigned char* lastptr, * foreptr, * backptr;
    PXSize hashpos;

    if(windowsize == 0 || windowsize > 32768) return 60; /*error: windowsize smaller/larger than allowed*/
    if((windowsize & (windowsize - 1)) != 0) return 90; /*error: must be power of two*/

    if(nicematch > MAX_SUPPORTED_DEFLATE_LENGTH) nicematch = MAX_SUPPORTED_DEFLATE_LENGTH;

    for(pos = inpos; pos < insize; ++pos)
    {
        PXSize wpos = pos & (windowsize - 1); /*position for in 'circular' hash buffers*/
        PXSize chainlength = 0;

        hashval = getHash(in, insize, pos);

        if(usezeros && hashval == 0)
        {
            if(numzeros == 0) numzeros = countZeros(in, insize, pos);
            else if(pos + numzeros > insize || in[pos + numzeros - 1] != 0) --numzeros;
        }
        else
        {
            numzeros = 0;
        }

        updateHashChain(hash, wpos, hashval, numzeros);

        /*the length and offset found for the current position*/
        length = 0;
        offset = 0;

        hashpos = hash->chain[wpos];

        lastptr = &in[insize < pos + MAX_SUPPORTED_DEFLATE_LENGTH ? insize : pos + MAX_SUPPORTED_DEFLATE_LENGTH];

        /*search for the longest string*/
        prev_offset = 0;
        for(;;)
        {
            if(chainlength++ >= maxchainlength) break;
            current_offset = (unsigned)(hashpos <= wpos ? wpos - hashpos : wpos - hashpos + windowsize);

            if(current_offset < prev_offset) break; /*stop when went completely around the circular buffer*/
            prev_offset = current_offset;
            if(current_offset > 0)
            {
                /*test the next characters*/
                foreptr = &in[pos];
                backptr = &in[pos - current_offset];

                /*common case in PNGs is lots of zeros. Quickly skip over them as a speedup*/
                if(numzeros >= 3)
                {
                    unsigned skip = hash->zeros[hashpos];
                    if(skip > numzeros) skip = numzeros;
                    backptr += skip;
                    foreptr += skip;
                }

                while(foreptr != lastptr && *backptr == *foreptr) /*maximum supported length by deflate is max length*/
                {
                    ++backptr;
                    ++foreptr;
                }
                current_length = (unsigned)(foreptr - &in[pos]);

                if(current_length > length)
                {
                    length = current_length; /*the longest length*/
                    offset = current_offset; /*the offset that is related to this longest length*/
                    /*jump out once a length of max length is found (speed gain). This also jumps
                    out if length is MAX_SUPPORTED_DEFLATE_LENGTH*/
                    if(current_length >= nicematch) break;
                }
            }

            if(hashpos == hash->chain[hashpos]) break;

            if(numzeros >= 3 && length > numzeros)
            {
                hashpos = hash->chainz[hashpos];
                if(hash->zeros[hashpos] != numzeros) break;
            }
            else
            {
                hashpos = hash->chain[hashpos];
                /*outdated hash value, happens if particular value was not encountered in whole last window*/
                if(hash->val[hashpos] != (int)hashval) break;
            }
        }

        if(lazymatching)
        {
            if(!lazy && length >= 3 && length <= maxlazymatch && length < MAX_SUPPORTED_DEFLATE_LENGTH)
            {
                lazy = 1;
                lazylength = length;
                lazyoffset = offset;
                continue; /*try the next byte*/
            }
            if(lazy)
            {
                lazy = 0;
                if(pos == 0) ERROR_BREAK(81);
                if(length > lazylength + 1)
                {
                    /*push the previous character as literal*/
                    if(!uivector_push_back(out, in[pos - 1])) ERROR_BREAK(83 /*alloc fail*/);
                }
                else
                {
                    length = lazylength;
                    offset = lazyoffset;
                    hash->head[hashval] = -1; /*the same hashchain update will be done, this ensures no wrong alteration*/
                    hash->headz[numzeros] = -1; /*idem*/
                    --pos;
                }
            }
        }
        if(length >= 3 && offset > windowsize) ERROR_BREAK(86 /*too big (or overflown negative) offset*/);

        /*encode it as length/distance pair or literal value*/
        if(length < 3) /*only lengths of 3 or higher are supported as length/distance pair*/
        {
            if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83 /*alloc fail*/);
        }
        else if(length < minmatch || (length == 3 && offset > 4096))
        {
            /*compensate for the fact that longer offsets have more extra bits, a
            length of only 3 may be not worth it then*/
            if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83 /*alloc fail*/);
        }
        else
        {
            addLengthDistance(out, length, offset);
            for(i = 1; i < length; ++i)
            {
                ++pos;
                wpos = pos & (windowsize - 1);
                hashval = getHash(in, insize, pos);
                if(usezeros && hashval == 0)
                {
                    if(numzeros == 0) numzeros = countZeros(in, insize, pos);
                    else if(pos + numzeros > insize || in[pos + numzeros - 1] != 0) --numzeros;
                }
                else
                {
                    numzeros = 0;
                }
                updateHashChain(hash, wpos, hashval, numzeros);
            }
        }
    } /*end of the loop through each character of input*/

    return error;
}