#include "DEFLATE.h"

#include <stdlib.h>
#include <string.h>

#include <Memory/PXMemory.h>
#include <Format/HUFFMAN/HuffmanTree.h>
#include <File/PXDataStream.h>
#include <Math/PXMath.h>

#define DeflateEncodingInvalidID -1
#define DeflateEncodingLiteralRawID 0b00
#define DeflateEncodingHuffmanStaticID 0b01
#define DeflateEncodingHuffmanDynamicID 0b10
#define DeflateEncodingReserverdID 0b11


#define FIRST_LENGTH_CODE_INDEX 257
#define LAST_LENGTH_CODE_INDEX 285
/*256 literals, the end code, some length codes, and 2 unused codes*/
#define NUM_DEFLATE_CODE_SYMBOLS 288
/*the distance codes have their own symbols, 30 used, 2 unused*/
#define NUM_DISTANCE_SYMBOLS 32
/*the code length codes. 0-15: code lengths, 16: copy previous 3-6 times, 17: 3-10 zeros, 18: 11-138 zeros*/
#define NUM_CODE_LENGTH_CODES 19

/*version of CERROR_BREAK that assumes the common case where the error variable is named "error"*/
#define ERROR_BREAK(code) CERROR_BREAK(error, code)


/*the base lengths represented by codes 257-285*/
static const unsigned LENGTHBASE[29]
= { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59,
   67, 83, 99, 115, 131, 163, 195, 227, 258 };

/*the extra bits used by codes 257-285 (added to base length)*/
static const unsigned LENGTHEXTRA[29]
= { 0, 0, 0, 0, 0, 0, 0,  0,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,
    4,  4,  4,   4,   5,   5,   5,   5,   0 };

/*the base backwards distances (the bits of distance codes appear after length codes and use their own huffman tree)*/
static const unsigned DISTANCEBASE[30]
= { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513,
   769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };

/*the extra bits of backwards distances (added to base)*/
static const unsigned DISTANCEEXTRA[30]
= { 0, 0, 0, 0, 1, 1, 2,  2,  3,  3,  4,  4,  5,  5,   6,   6,   7,   7,   8,
     8,    9,    9,   10,   10,   11,   11,   12,    12,    13,    13 };


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
#define FIRSTBITS 9u

/* a symbol value too big to represent any valid symbol, to indicate reading disallowed huffman bits combination,
which is possible in case of only 0 or 1 present symbols. */
#define INVALIDSYMBOL 65535u


DeflateEncodingMethod ConvertToDeflateEncodingMethod(const unsigned char deflateEncodingMethod)
{
    switch(deflateEncodingMethod)
    {
        case DeflateEncodingLiteralRawID:
            return DeflateEncodingLiteralRaw;

        case DeflateEncodingHuffmanStaticID:
            return DeflateEncodingHuffmanStatic;

        case DeflateEncodingHuffmanDynamicID:
            return DeflateEncodingHuffmanDynamic;

        case DeflateEncodingReserverdID:
            return DeflateEncodingReserverd;

        default:
            return DeflateEncodingInvalid;
    }
}

unsigned char ConvertFromDeflateEncodingMethod(const DeflateEncodingMethod deflateEncodingMethod)
{
    switch(deflateEncodingMethod)
    {
        case DeflateEncodingLiteralRaw:
            return DeflateEncodingLiteralRawID;

        case DeflateEncodingHuffmanStatic:
            return DeflateEncodingHuffmanStaticID;

        case DeflateEncodingHuffmanDynamic:
            return DeflateEncodingHuffmanDynamicID;

        default:
        case DeflateEncodingReserverd:
            return DeflateEncodingReserverdID;
    }
}

int DEFLATEParse(const void* const inputBuffer, const PXSize inputBufferSize, void* const outputBuffer, const PXSize outputBufferSize, PXSize* const outputBufferSizeRead)
{
    PXDataStream dataStream;
    DeflateBlock deflateBlock;

    PXDataStreamConstruct(&dataStream);
    PXDataStreamFromExternal(&dataStream, inputBuffer, inputBufferSize);

    do
    {
        deflateBlock.IsLastBlock = PXDataStreamReadBits(&dataStream, 1u);

        {
            const unsigned char encodingMethodValue = PXDataStreamReadBits(&dataStream, 2u);

            deflateBlock.EncodingMethod = ConvertToDeflateEncodingMethod(encodingMethodValue);
        }

        switch(deflateBlock.EncodingMethod)
        {
            default:
            case DeflateEncodingReserverd:
            case DeflateEncodingInvalid:
            {
                return -1; // Invalid Mode
            }
            case DeflateEncodingLiteralRaw:
            {
                PXDataStreamSkipBitsToNextByte(&dataStream); // Skip remaining Bytes

                const unsigned short length = PXDataStreamReadBits(&dataStream, 16u);
                const unsigned short lengthInverse = PXDataStreamReadBits(&dataStream, 16u);
                const void* sourceAdress = PXDataStreamCursorPosition(&dataStream);
                const unsigned char validLength = (length + lengthInverse) == 65535;
                //const PXSize bitsToJump = (PXSize)length * 8;

                //assert(validLength);

                MemoryCopy(sourceAdress, PXDataStreamRemainingSize(&dataStream), (unsigned char*)outputBuffer + *outputBufferSizeRead, length);

                *outputBufferSizeRead += length;

                PXDataStreamCursorAdvance(&dataStream, length);

                break;
            }
            case DeflateEncodingHuffmanDynamic:
            case DeflateEncodingHuffmanStatic:
            {
                HuffmanTree literalAndLengthCodes;
                HuffmanTree distanceCodes;
                unsigned char foundEndOFBlock = 0;

                HuffmanTreeConstruct(&literalAndLengthCodes);
                HuffmanTreeConstruct(&distanceCodes);

                switch(deflateBlock.EncodingMethod)
                {
                    case DeflateEncodingHuffmanDynamic:
                    {
                        const unsigned int result = GenerateDynamicTree(&dataStream, &literalAndLengthCodes, &distanceCodes);

                        if(result != 0)
                        {
                            printf("EE");
                        }

                        break;
                    }
                    case DeflateEncodingHuffmanStatic:
                    {
                        GenerateFixedLiteralLengthTree(&literalAndLengthCodes);
                        GenerateFixedDistanceTree(&distanceCodes);
                        break;
                    }
                }

                while(!foundEndOFBlock)
                {
                    unsigned int resultLengthCode = huffmanDecodeSymbol(&dataStream, &literalAndLengthCodes);
                    HuffmanCodeType huffmanCodeType = HuffmanCodeTypeFromCode(resultLengthCode);

                    switch(huffmanCodeType)
                    {
                        case HuffmanCodeInvalid:
                        {
                            // printf("[Symbol] Error: Invalid\n");
                            break; // ERROR
                        }
                        case HuffmanCodeLiteral:
                        {
                            // printf("[Symbol] <%2x>(%3i) Literal.\n", resultLengthCode, resultLengthCode);
                            ((unsigned char*)outputBuffer)[(*outputBufferSizeRead)++] = resultLengthCode;
                            break;
                        }
                        case HuffmanCodeLength:
                        {
                            // printf("[Symbol] <%2x>(%3i) Length.\n", resultLengthCode, resultLengthCode);

                            PXSize distance = 0;
                            PXSize numextrabits_l = 0;
                            PXSize numextrabits_d = 0; /*extra bits for length and distance*/
                            PXSize start, backward, length;

                            /*the base lengths represented by codes 257-285*/
                            const unsigned int LENGTHBASE[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };

                            /*the extra bits used by codes 257-285 (added to base length)*/
                            const unsigned int LENGTHEXTRA[29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };

                            /*the base backwards distances (the bits of distance codes appear after length codes and use their own huffman tree)*/
                            static const unsigned DISTANCEBASE[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513,                                           769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };

                            /*the extra bits of backwards distances (added to base)*/
                            static const unsigned DISTANCEEXTRA[30] = { 0, 0, 0, 0, 1, 1, 2,  2,  3,  3,  4,  4,  5,  5,   6,   6,   7,   7,   8, 8,    9,    9,   10,   10,   11,   11,   12,    12,    13,    13 };


                            {
                                //const unsigned int FIRST_LENGTH_CODE_INDEX = 257u;
                                //const unsigned int LAST_LENGTH_CODE_INDEX = 285u;

                                /*part 1: get length base*/
                                length = LENGTHBASE[resultLengthCode - FIRST_LENGTH_CODE_INDEX];

                                /*part 2: get extra bits and add the value of that to length*/
                                numextrabits_l = LENGTHEXTRA[resultLengthCode - FIRST_LENGTH_CODE_INDEX];
                            }


                            if(numextrabits_l != 0)
                            {
                                /* bits already ensured above */
                                length += PXDataStreamReadBits(&dataStream, numextrabits_l);
                            }

                            /*part 3: get distance code*/
                            //ensureBits32(reader, 28); /* up to 15 for the huffman symbol, up to 13 for the extra bits */
                            unsigned int resultDistanceCode = huffmanDecodeSymbol(&dataStream, &distanceCodes);
                            unsigned char isUnsupportedCode = resultDistanceCode > 29;
                            unsigned char isIllegalCode = resultDistanceCode > 31;

                            if(isUnsupportedCode)
                            {
                                if(isIllegalCode)  /* if(code_d == INVALIDSYMBOL) */
                                {
                                    return(16); /*error: tried to read disallowed huffman symbol*/
                                }
                                else
                                {
                                    return(18); /*error: invalid distance code (30-31 are never used)*/
                                }
                            }

                            distance = DISTANCEBASE[resultDistanceCode];

                            /*part 4: get extra bits from distance*/
                            numextrabits_d = DISTANCEEXTRA[resultDistanceCode];
                            if(numextrabits_d != 0)
                            {
                                /* bits already ensured above */
                                distance += PXDataStreamReadBits(&dataStream, numextrabits_d);
                            }

                            /*part 5: fill in all the out[n] values based on the length and dist*/
                            start = (*outputBufferSizeRead);
                            if(distance > start) return (52); /*too long backward distance*/
                            backward = start - distance;

                            (*outputBufferSizeRead) += length;

                            // if (!ucvector_resize(out, out->size + length)) ERROR_BREAK(83 /*alloc fail*/);


                            if(distance < length)
                            {
                                MemoryCopy((unsigned char*)outputBuffer + backward, distance, (unsigned char*)outputBuffer + start, distance);

                                start += distance;

                                for(PXSize forward = distance; forward < length; ++forward)
                                {
                                    ((unsigned char*)outputBuffer)[start++] = ((unsigned char*)outputBuffer)[backward++];
                                }
                            }
                            else
                            {
                                MemoryCopy((unsigned char*)outputBuffer + backward, length, (unsigned char*)outputBuffer + start, length);
                            }
                            break;
                        }
                        case HuffmanCodeEndOfBlock:
                        {
                            //printf("[Symbol] <%2x>(%3i) End of Block.\n", resultLengthCode, resultLengthCode);
                            foundEndOFBlock = 1u;

                            break; // FINISHED!
                        }
                    }
                }

                break;
            }
        }
    }
    while(!deflateBlock.IsLastBlock);

    //*outputBufferSizeRead = dataStream.DataCursor;

    return 0;
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
    free(((uivector*)p)->data);
    ((uivector*)p)->data = NULL;
}

/*returns 1 if success, 0 if failure ==> nothing done*/
static unsigned uivector_resize(uivector* p, PXSize size)
{
    PXSize allocsize = size * sizeof(unsigned);
    if(allocsize > p->allocsize)
    {
        PXSize newsize = allocsize + (p->allocsize >> 1u);
        void* data = realloc(p->data, newsize);
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
    unsigned btype; /*the block type for LZ (0, 1, 2 or 3, see zlib standard). Should be 2 for proper compression.*/
    unsigned use_lz77; /*whether or not to use LZ77. Should be 1 for proper compression.*/
    unsigned windowsize; /*must be a power of two <= 32768. higher compresses more but is slower. Default value: 2048.*/
    unsigned minmatch; /*minimum lz77 length. 3 is normally best, 6 can be better for some PNGs. Default: 0*/
    unsigned nicematch; /*stop searching if >= this length found. Set to 258 for best compression. Default: 128*/
    unsigned lazymatching; /*use lazy matching: better compression but a bit slower. Default: true*/

    /*use custom zlib encoder instead of built in one (default: null)*/
    unsigned (*custom_zlib)(unsigned char**, PXSize*,
                            const unsigned char*, PXSize,
                            const LodePNGCompressSettings*);
    /*use custom deflate encoder instead of built in one (default: null)
    if custom_zlib is used, custom_deflate is ignored since only the built in
    zlib function will call custom_deflate*/
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
    unsigned short* chain;
    int* val; /*circular pos to hash value*/

    /*TODO: do this not only for zeros but for any repeated byte. However for PNG
    it's always going to be the zeros that dominate, so not important for PNG*/
    int* headz; /*similar to head, but for chainz*/
    unsigned short* chainz; /*those with same amount of zeros*/
    unsigned short* zeros; /*length of zeros streak, used as a second hash chain*/
} Hash;

unsigned hash_init(Hash* hash, unsigned windowsize)
{
    PXSize i;
    hash->head = (int*)MemoryAllocate(sizeof(int) * HASH_NUM_VALUES);
    hash->val = (int*)MemoryAllocate(sizeof(int) * windowsize);
    hash->chain = (unsigned short*)MemoryAllocate(sizeof(unsigned short) * windowsize);

    hash->zeros = (unsigned short*)MemoryAllocate(sizeof(unsigned short) * windowsize);
    hash->headz = (int*)MemoryAllocate(sizeof(int) * (MAX_SUPPORTED_DEFLATE_LENGTH + 1));
    hash->chainz = (unsigned short*)MemoryAllocate(sizeof(unsigned short) * windowsize);

    if(!hash->head || !hash->chain || !hash->val || !hash->headz || !hash->chainz || !hash->zeros)
    {
        return 83; /*alloc fail*/
    }

    /*initialize hash table*/
    for(i = 0; i != HASH_NUM_VALUES; ++i) hash->head[i] = -1;
    for(i = 0; i != windowsize; ++i) hash->val[i] = -1;
    for(i = 0; i != windowsize; ++i) hash->chain[i] = i; /*same value as index indicates uninitialized*/

    for(i = 0; i <= MAX_SUPPORTED_DEFLATE_LENGTH; ++i) hash->headz[i] = -1;
    for(i = 0; i != windowsize; ++i) hash->chainz[i] = i; /*same value as index indicates uninitialized*/

    return 0;
}

void hash_cleanup(Hash* hash)
{
    free(hash->head);
    free(hash->val);
    free(hash->chain);

    free(hash->zeros);
    free(hash->headz);
    free(hash->chainz);
}



unsigned getHash(const unsigned char* data, PXSize size, PXSize pos)
{
    unsigned result = 0;
    if(pos + 2 < size)
    {
        /*A simple shift and xor hash is used. Since the data of PNGs is dominated
        by zeroes due to the filters, a better hash does not have a significant
        effect on speed in traversing the chain, and causes more time spend on
        calculating the hash.*/
        result ^= ((unsigned)data[pos + 0] << 0u);
        result ^= ((unsigned)data[pos + 1] << 4u);
        result ^= ((unsigned)data[pos + 2] << 8u);
    }
    else
    {
        PXSize amount, i;
        if(pos >= size) return 0;
        amount = size - pos;
        for(i = 0; i != amount; ++i) result ^= ((unsigned)data[pos + i] << (i * 8u));
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
        void* data = realloc(p->data, newsize);
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



typedef struct
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
void PNGwriteBits(LodePNGBitWriter* writer, unsigned value, PXSize nbits)
{
    if(nbits == 1)
    { /* compiler should statically compile this case if nbits == 1 */
        WRITEBIT(writer, value);
    }
    else
    {
        /* TODO: increase output size only once here rather than in each WRITEBIT */
        PXSize i;
        for(i = 0; i != nbits; ++i)
        {
            WRITEBIT(writer, (unsigned char)((value >> i) & 1));
        }
    }
}

/* This one is to use for adding huffman symbol, the value bits are written MSB first */
void writeBitsReversed(LodePNGBitWriter* writer, unsigned value, PXSize nbits)
{
    PXSize i;
    for(i = 0; i != nbits; ++i)
    {
        /* TODO: increase output size only once here rather than in each WRITEBIT */
        WRITEBIT(writer, (unsigned char)((value >> (nbits - 1u - i)) & 1u));
    }
}

#define NUM_CODE_LENGTH_CODES 19




void addLengthDistance(uivector* values, PXSize length, PXSize distance);
void updateHashChain(Hash* hash, PXSize wpos, unsigned hashval, unsigned short numzeros);
PXSize countZeros(const unsigned char* data, PXSize size, PXSize pos);







/*
write the lz77-encoded data, which has lit, len and dist codes, to compressed stream using huffman trees.
tree_ll: the tree for lit and len codes.
tree_d: the tree for distance codes.
*/
void writeLZ77data
(
    LodePNGBitWriter* writer,
    const uivector* lz77_encoded,
    const HuffmanTree* tree_ll,
    const HuffmanTree* tree_d
)
{
    PXSize i = 0;
    for(i = 0; i != lz77_encoded->size; ++i)
    {
        PXSize val = lz77_encoded->data[i];
        writeBitsReversed(writer, tree_ll->codes[val], tree_ll->lengths[val]);
        if(val > 256) /*for a length code, 3 more things have to be added*/
        {
            PXSize length_index = val - FIRST_LENGTH_CODE_INDEX;
            PXSize n_length_extra_bits = LENGTHEXTRA[length_index];
            PXSize length_extra_bits = lz77_encoded->data[++i];

            PXSize distance_code = lz77_encoded->data[++i];

            PXSize distance_index = distance_code;
            PXSize n_distance_extra_bits = DISTANCEEXTRA[distance_index];
            PXSize distance_extra_bits = lz77_encoded->data[++i];

            PNGwriteBits(writer, length_extra_bits, n_length_extra_bits);
            writeBitsReversed(writer, tree_d->codes[distance_code], tree_d->lengths[distance_code]);
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






unsigned deflateNoCompression(ucvector* out, const unsigned char* data, PXSize datasize)
{
    /*non compressed deflate block data: 1 bit BFINAL,2 bits BTYPE,(5 bits): it jumps to start of next byte,
  2 bytes LEN, 2 bytes NLEN, LEN bytes literal DATA*/

    PXSize i, numdeflateblocks = (datasize + 65534u) / 65535u;
    unsigned datapos = 0;
    for(i = 0; i != numdeflateblocks; ++i)
    {
        unsigned BFINAL, BTYPE, LEN, NLEN;
        unsigned char firstbyte;
        PXSize pos = out->size;

        BFINAL = (i == numdeflateblocks - 1);
        BTYPE = 0;

        LEN = 65535;
        if(datasize - datapos < 65535u) LEN = (unsigned)datasize - datapos;
        NLEN = 65535 - LEN;

        if(!ucvector_resize(out, out->size + LEN + 5)) return 83; /*alloc fail*/

        firstbyte = (unsigned char)(BFINAL + ((BTYPE & 1u) << 1u) + ((BTYPE & 2u) << 1u));
        out->data[pos + 0] = firstbyte;
        out->data[pos + 1] = (unsigned char)(LEN & 255);
        out->data[pos + 2] = (unsigned char)(LEN >> 8u);
        out->data[pos + 3] = (unsigned char)(NLEN & 255);
        out->data[pos + 4] = (unsigned char)(NLEN >> 8u);
        memcpy(out->data + pos + 5, data + datapos, LEN);
        datapos += LEN;
    }

    return 0;
}

unsigned deflateFixed
(
    LodePNGBitWriter* writer,
    Hash* hash,
    const unsigned char* data,
    PXSize datapos, PXSize dataend,
    const LodePNGCompressSettings* settings,
    PXSize final
)
{
    HuffmanTree tree_ll; /*tree for literal values and length codes*/
    HuffmanTree tree_d; /*tree for distance codes*/

    PXSize BFINAL = final;
    PXSize error = 1;
    PXSize i;

    HuffmanTreeConstruct(&tree_ll);
    HuffmanTreeConstruct(&tree_d);

    GenerateFixedLiteralLengthTree(&tree_ll);
    GenerateFixedDistanceTree(&tree_d);

    if(!error)
    {
        PNGwriteBits(writer, BFINAL, 1);
        PNGwriteBits(writer, 1, 1); /*first bit of BTYPE*/
        PNGwriteBits(writer, 0, 1); /*second bit of BTYPE*/

        if(settings->use_lz77) /*LZ77 encoded*/
        {
            uivector lz77_encoded;
            uivector_init(&lz77_encoded);
            error = encodeLZ77(&lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
                               settings->minmatch, settings->nicematch, settings->lazymatching);
            if(!error) writeLZ77data(writer, &lz77_encoded, &tree_ll, &tree_d);
            uivector_cleanup(&lz77_encoded);
        }
        else /*no LZ77, but still will be Huffman compressed*/
        {
            for(i = datapos; i < dataend; ++i)
            {
                writeBitsReversed(writer, tree_ll.codes[data[i]], tree_ll.lengths[data[i]]);
            }
        }
        /*add END code*/
        if(!error) writeBitsReversed(writer, tree_ll.codes[256], tree_ll.lengths[256]);
    }

    /*cleanup*/
    HuffmanTreeDestruct(&tree_ll);
    HuffmanTreeDestruct(&tree_d);

    return error;
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

PXSize searchCodeIndex(const unsigned* array, PXSize array_size, PXSize value)
{
    /*binary search (only small gain over linear). TODO: use CPU log2 instruction for getting symbols instead*/
    PXSize left = 1;
    PXSize right = array_size - 1;

    while(left <= right)
    {
        PXSize mid = (left + right) >> 1;
        if(array[mid] >= value) right = mid - 1;
        else left = mid + 1;
    }
    if(left >= array_size || array[left] > value) left--;
    return left;
}


void addLengthDistance(uivector* values, PXSize length, PXSize distance)
{
    /*values in encoded vector are those used by deflate:
 0-255: literal bytes
 256: end
 257-285: length/distance pair (length code, followed by extra length bits, distance code, extra distance bits)
 286-287: invalid*/

    unsigned length_code = (unsigned)searchCodeIndex(LENGTHBASE, 29, length);
    unsigned extra_length = (unsigned)(length - LENGTHBASE[length_code]);
    unsigned dist_code = (unsigned)searchCodeIndex(DISTANCEBASE, 30, distance);
    unsigned extra_distance = (unsigned)(distance - DISTANCEBASE[dist_code]);

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





/*the order in which "code length alphabet code lengths" are stored as specified by deflate, out of this the huffman
tree of the dynamic huffman tree lengths is generated*/
const unsigned CLCL_ORDER[NUM_CODE_LENGTH_CODES]
= { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };




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

/*sort the leaves with stable mergesort*/
void bpmnode_sort(BPMNode* leaves, PXSize num)
{
    BPMNode* mem = (BPMNode*)MemoryAllocate(sizeof(*leaves) * num);
    PXSize width, counter = 0;
    for(width = 1; width < num; width *= 2)
    {
        BPMNode* a = (counter & 1) ? mem : leaves;
        BPMNode* b = (counter & 1) ? leaves : mem;
        PXSize p;
        for(p = 0; p < num; p += 2 * width)
        {
            PXSize q = (p + width > num) ? num : (p + width);
            PXSize r = (p + 2 * width > num) ? num : (p + 2 * width);
            PXSize i = p, j = q, k;
            for(k = p; k < r; k++)
            {
                if(i < q && (j >= r || a[i].weight <= a[j].weight)) b[k] = a[i++];
                else b[k] = a[j++];
            }
        }
        counter++;
    }
    if(counter & 1) memcpy(leaves, mem, sizeof(*leaves) * num);

    free(mem);
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


unsigned HuffmanTree_makeTable(HuffmanTree* tree)
{
    static const unsigned headsize = 1u << FIRSTBITS; /*size of the first table*/
    static const unsigned mask = (1u << FIRSTBITS) /*headsize*/ - 1u;
    PXSize i, numpresent, pointer, size; /*total table size*/
    unsigned* maxlens = (unsigned*)MemoryAllocate(headsize * sizeof(unsigned));
    if(!maxlens) return 83; /*alloc fail*/

    /* compute maxlens: max total bit length of symbols sharing prefix in the first table*/
    memset(maxlens, 0, headsize * sizeof(*maxlens));
    for(i = 0; i < tree->numcodes; i++)
    {
        unsigned symbol = tree->codes[i];
        unsigned l = tree->lengths[i];
        unsigned index;
        if(l <= FIRSTBITS) continue; /*symbols that fit in first table don't increase secondary table size*/
        /*get the FIRSTBITS MSBs, the MSBs of the symbol are encoded first. See later comment about the reversing*/
        index = reverseBits(symbol >> (l - FIRSTBITS), FIRSTBITS);
        maxlens[index] = MathMaximum(maxlens[index], l);
    }
    /* compute total table size: size of first table plus all secondary tables for symbols longer than FIRSTBITS */
    size = headsize;
    for(i = 0; i < headsize; ++i)
    {
        unsigned l = maxlens[i];
        if(l > FIRSTBITS) size += (1u << (l - FIRSTBITS));
    }
    tree->table_len = (unsigned char*)MemoryAllocate(size * sizeof(*tree->table_len));
    tree->table_value = (unsigned short*)MemoryAllocate(size * sizeof(*tree->table_value));
    if(!tree->table_len || !tree->table_value)
    {
        free(maxlens);
        /* freeing tree->table values is done at a higher scope */
        return 83; /*alloc fail*/
    }
    /*initialize with an invalid length to indicate unused entries*/
    for(i = 0; i < size; ++i) tree->table_len[i] = 16;

    /*fill in the first table for long symbols: max prefix size and pointer to secondary tables*/
    pointer = headsize;
    for(i = 0; i < headsize; ++i)
    {
        unsigned l = maxlens[i];
        if(l <= FIRSTBITS) continue;
        tree->table_len[i] = l;
        tree->table_value[i] = pointer;
        pointer += (1u << (l - FIRSTBITS));
    }

    free(maxlens);

    /*fill in the first table for short symbols, or secondary table for long symbols*/
    numpresent = 0;
    for(i = 0; i < tree->numcodes; ++i)
    {
        unsigned l = tree->lengths[i];
        unsigned symbol, reverse;
        if(l == 0) continue;
        symbol = tree->codes[i]; /*the huffman bit pattern. i itself is the value.*/
        /*reverse bits, because the huffman bits are given in MSB first order but the bit reader reads LSB first*/
        reverse = reverseBits(symbol, l);
        numpresent++;

        if(l <= FIRSTBITS)
        {
            /*short symbol, fully in first table, replicated num times if l < FIRSTBITS*/
            unsigned num = 1u << (FIRSTBITS - l);
            unsigned j;
            for(j = 0; j < num; ++j)
            {
                /*bit reader will read the l bits of symbol first, the remaining FIRSTBITS - l bits go to the MSB's*/
                unsigned index = reverse | (j << l);
                if(tree->table_len[index] != 16) return 55; /*invalid tree: long symbol shares prefix with short symbol*/
                tree->table_len[index] = l;
                tree->table_value[index] = i;
            }
        }
        else
        {
            /*long symbol, shares prefix with other long symbols in first lookup table, needs second lookup*/
            /*the FIRSTBITS MSBs of the symbol are the first table index*/
            unsigned index = reverse & mask;
            unsigned maxlen = tree->table_len[index];
            /*log2 of secondary table length, should be >= l - FIRSTBITS*/
            unsigned tablelen = maxlen - FIRSTBITS;
            unsigned start = tree->table_value[index]; /*starting index in secondary table*/
            unsigned num = 1u << (tablelen - (l - FIRSTBITS)); /*amount of entries of this symbol in secondary table*/
            unsigned j;
            if(maxlen < l) return 55; /*invalid tree: long symbol shares prefix with short symbol*/
            for(j = 0; j < num; ++j)
            {
                unsigned reverse2 = reverse >> FIRSTBITS; /* l - FIRSTBITS bits */
                unsigned index2 = start + (reverse2 | (j << (l - FIRSTBITS)));
                tree->table_len[index2] = l;
                tree->table_value[index2] = i;
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
            if(tree->table_len[i] == 16)
            {
                /* As length, use a value smaller than FIRSTBITS for the head table,
                and a value larger than FIRSTBITS for the secondary table, to ensure
                valid behavior for advanceBits when reading this symbol. */
                tree->table_len[i] = (i < headsize) ? 1 : (FIRSTBITS + 1);
                tree->table_value[i] = INVALIDSYMBOL;
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
            if(tree->table_len[i] == 16) return 55;
        }
    }

    return 0;
}

static unsigned HuffmanTree_makeFromLengths2(HuffmanTree* tree)
{
    unsigned* blcount;
    unsigned* nextcode;
    unsigned error = 0;
    unsigned bits, n;

    tree->codes = (unsigned*)MemoryAllocate(tree->numcodes * sizeof(unsigned));
    blcount = (unsigned*)MemoryAllocate((tree->maxbitlen + 1) * sizeof(unsigned));
    nextcode = (unsigned*)MemoryAllocate((tree->maxbitlen + 1) * sizeof(unsigned));
    if(!tree->codes || !blcount || !nextcode) error = 83; /*alloc fail*/

    if(!error)
    {
        for(n = 0; n != tree->maxbitlen + 1; n++) blcount[n] = nextcode[n] = 0;
        /*step 1: count number of instances of each code length*/
        for(bits = 0; bits != tree->numcodes; ++bits) ++blcount[tree->lengths[bits]];
        /*step 2: generate the nextcode values*/
        for(bits = 1; bits <= tree->maxbitlen; ++bits)
        {
            nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1u;
        }
        /*step 3: generate all the codes*/
        for(n = 0; n != tree->numcodes; ++n)
        {
            if(tree->lengths[n] != 0)
            {
                tree->codes[n] = nextcode[tree->lengths[n]]++;
                /*remove superfluous bits from the code*/
                tree->codes[n] &= ((1u << tree->lengths[n]) - 1u);
            }
        }
    }

    free(blcount);
    free(nextcode);

    if(!error) error = HuffmanTree_makeTable(tree);
    return error;
}

unsigned lodepng_huffman_code_lengths(unsigned* lengths, const unsigned* frequencies,
                                      PXSize numcodes, unsigned maxbitlen)
{
    unsigned error = 0;
    unsigned i;
    PXSize numpresent = 0; /*number of symbols with non-zero frequency*/
    BPMNode* leaves; /*the symbols, only those with > 0 frequency*/

    if(numcodes == 0) return 80; /*error: a tree of 0 symbols is not supposed to be made*/
    if((1u << maxbitlen) < (unsigned)numcodes) return 80; /*error: represent all symbols*/

    leaves = (BPMNode*)MemoryAllocate(numcodes * sizeof(*leaves));
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

    memset(lengths, 0, numcodes * sizeof(*lengths));

    /*ensure at least two present symbols. There should be at least one symbol
    according to RFC 1951 section 3.2.7. Some decoders incorrectly require two. To
    make these work as well ensure there are at least two symbols. The
    Package-Merge code below also doesn't work correctly if there's only one
    symbol, it'd give it the theoretical 0 bits but in practice zlib wants 1 bit*/
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
        lists.memory = (BPMNode*)MemoryAllocate(lists.memsize * sizeof(*lists.memory));
        lists.freelist = (BPMNode**)MemoryAllocate(lists.memsize * sizeof(BPMNode*));
        lists.chains0 = (BPMNode**)MemoryAllocate(lists.listsize * sizeof(BPMNode*));
        lists.chains1 = (BPMNode**)MemoryAllocate(lists.listsize * sizeof(BPMNode*));
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
            for(i = 2; i != 2 * numpresent - 2; ++i) boundaryPM(&lists, leaves, numpresent, (int)maxbitlen - 1, (int)i);

            for(node = lists.chains1[maxbitlen - 1]; node; node = node->tail)
            {
                for(i = 0; i != node->index; ++i) ++lengths[leaves[i].index];
            }
        }

        free(lists.memory);
        free(lists.freelist);
        free(lists.chains0);
        free(lists.chains1);
    }

    free(leaves);
    return error;
}

unsigned HuffmanTree_makeFromFrequencies(HuffmanTree* tree, const unsigned* frequencies,
                                                PXSize mincodes, PXSize numcodes, unsigned maxbitlen)
{
    unsigned error = 0;
    while(!frequencies[numcodes - 1] && numcodes > mincodes) --numcodes; /*trim zeroes*/
    tree->lengths = (unsigned*)MemoryAllocate(numcodes * sizeof(unsigned));
    if(!tree->lengths) return 83; /*alloc fail*/
    tree->maxbitlen = maxbitlen;
    tree->numcodes = (unsigned)numcodes; /*number of symbols*/

    error = lodepng_huffman_code_lengths(tree->lengths, frequencies, numcodes, maxbitlen);
    if(!error) error = HuffmanTree_makeFromLengths2(tree);
    return error;
}



unsigned deflateDynamic
(
    LodePNGBitWriter* writer,
    Hash* hash,
    const unsigned char* data,
    PXSize datapos,
    PXSize dataend,
    const LodePNGCompressSettings* settings,
    unsigned final
)
{
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
    HuffmanTree tree_ll; /*tree for lit,len values*/
    HuffmanTree tree_d; /*tree for distance codes*/
    HuffmanTree tree_cl; /*tree for encoding the code lengths representing tree_ll and tree_d*/
    unsigned* frequencies_ll = 0; /*frequency of lit,len codes*/
    unsigned* frequencies_d = 0; /*frequency of dist codes*/
    unsigned* frequencies_cl = 0; /*frequency of code length codes*/
    unsigned* bitlen_lld = 0; /*lit,len,dist code lengths (int bits), literally (without repeat codes).*/
    unsigned* bitlen_lld_e = 0; /*bitlen_lld encoded with repeat codes (this is a rudimentary run length compression)*/
    PXSize datasize = dataend - datapos;

    /*
    If we could call "bitlen_cl" the the code length code lengths ("clcl"), that is the bit lengths of codes to represent
    tree_cl in CLCL_ORDER, then due to the huffman compression of huffman tree representations ("two levels"), there are
    some analogies:
    bitlen_lld is to tree_cl what data is to tree_ll and tree_d.
    bitlen_lld_e is to bitlen_lld what lz77_encoded is to data.
    bitlen_cl is to bitlen_lld_e what bitlen_lld is to lz77_encoded.
    */

    unsigned BFINAL = final;
    PXSize i;
    PXSize numcodes_ll, numcodes_d, numcodes_lld, numcodes_lld_e, numcodes_cl;
    unsigned HLIT, HDIST, HCLEN;

    uivector_init(&lz77_encoded);
    HuffmanTreeConstruct(&tree_ll);
    HuffmanTreeConstruct(&tree_d);
    HuffmanTreeConstruct(&tree_cl);
    /* could fit on stack, but >1KB is on the larger side so allocate instead */
    frequencies_ll = (unsigned*)MemoryAllocate(286 * sizeof(*frequencies_ll));
    frequencies_d = (unsigned*)MemoryAllocate(30 * sizeof(*frequencies_d));
    frequencies_cl = (unsigned*)MemoryAllocate(NUM_CODE_LENGTH_CODES * sizeof(*frequencies_cl));

    if(!frequencies_ll || !frequencies_d || !frequencies_cl) error = 83; /*alloc fail*/

    /*This while loop never loops due to a break at the end, it is here to
    allow breaking out of it to the cleanup phase on error conditions.*/
    while(!error)
    {
        memset(frequencies_ll, 0, 286 * sizeof(*frequencies_ll));
        memset(frequencies_d, 0, 30 * sizeof(*frequencies_d));
        memset(frequencies_cl, 0, NUM_CODE_LENGTH_CODES * sizeof(*frequencies_cl));

        if(settings->use_lz77)
        {
            error = encodeLZ77(&lz77_encoded, hash, data, datapos, dataend, settings->windowsize,
                               settings->minmatch, settings->nicematch, settings->lazymatching);
            if(error) break;
        }
        else
        {
            if(!uivector_resize(&lz77_encoded, datasize)) ERROR_BREAK(83 /*alloc fail*/);
            for(i = datapos; i < dataend; ++i) lz77_encoded.data[i - datapos] = data[i]; /*no LZ77, but still will be Huffman compressed*/
        }

        /*Count the frequencies of lit, len and dist codes*/
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
        if(error) break;
        /*2, not 1, is chosen for mincodes: some buggy PNG decoders require at least 2 symbols in the dist tree*/
        error = HuffmanTree_makeFromFrequencies(&tree_d, frequencies_d, 2, 30, 15);
        if(error) break;

        numcodes_ll = MathMinimum(tree_ll.numcodes, 286);
        numcodes_d = MathMinimum(tree_d.numcodes, 30);
        /*store the code lengths of both generated trees in bitlen_lld*/
        numcodes_lld = numcodes_ll + numcodes_d;
        bitlen_lld = (unsigned*)MemoryAllocate(numcodes_lld * sizeof(*bitlen_lld));
        /*numcodes_lld_e never needs more size than bitlen_lld*/
        bitlen_lld_e = (unsigned*)MemoryAllocate(numcodes_lld * sizeof(*bitlen_lld_e));
        if(!bitlen_lld || !bitlen_lld_e) ERROR_BREAK(83); /*alloc fail*/
        numcodes_lld_e = 0;

        for(i = 0; i != numcodes_ll; ++i) bitlen_lld[i] = tree_ll.lengths[i];
        for(i = 0; i != numcodes_d; ++i) bitlen_lld[numcodes_ll + i] = tree_d.lengths[i];

        /*run-length compress bitlen_ldd into bitlen_lld_e by using repeat codes 16 (copy length 3-6 times),
        17 (3-10 zeroes), 18 (11-138 zeroes)*/
        for(i = 0; i != numcodes_lld; ++i)
        {
            unsigned j = 0; /*amount of repetitions*/
            while(i + j + 1 < numcodes_lld && bitlen_lld[i + j + 1] == bitlen_lld[i]) ++j;

            if(bitlen_lld[i] == 0 && j >= 2) /*repeat code for zeroes*/
            {
                ++j; /*include the first zero*/
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

        error = HuffmanTree_makeFromFrequencies(&tree_cl, frequencies_cl,
                                                NUM_CODE_LENGTH_CODES, NUM_CODE_LENGTH_CODES, 7);
        if(error) break;

        /*compute amount of code-length-code-lengths to output*/
        numcodes_cl = NUM_CODE_LENGTH_CODES;
        /*trim zeros at the end (using CLCL_ORDER), but minimum size must be 4 (see HCLEN below)*/
        while(numcodes_cl > 4u && tree_cl.lengths[CLCL_ORDER[numcodes_cl - 1u]] == 0)
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
        for(i = 0; i != numcodes_cl; ++i) PNGwriteBits(writer, tree_cl.lengths[CLCL_ORDER[i]], 3);

        /*write the lengths of the lit/len AND the dist alphabet*/
        for(i = 0; i != numcodes_lld_e; ++i)
        {
            writeBitsReversed(writer, tree_cl.codes[bitlen_lld_e[i]], tree_cl.lengths[bitlen_lld_e[i]]);
            /*extra bits of repeat codes*/
            if(bitlen_lld_e[i] == 16) PNGwriteBits(writer, bitlen_lld_e[++i], 2);
            else if(bitlen_lld_e[i] == 17) PNGwriteBits(writer, bitlen_lld_e[++i], 3);
            else if(bitlen_lld_e[i] == 18) PNGwriteBits(writer, bitlen_lld_e[++i], 7);
        }

        /*write the compressed data symbols*/
        writeLZ77data(writer, &lz77_encoded, &tree_ll, &tree_d);
        /*error: the length of the end code 256 must be larger than 0*/
        if(tree_ll.lengths[256] == 0) ERROR_BREAK(64);

        /*write the end code*/
        writeBitsReversed(writer, tree_ll.codes[256], tree_ll.lengths[256]);

        break; /*end of error-while*/
    }

    /*cleanup*/
    uivector_cleanup(&lz77_encoded);
    HuffmanTreeDestruct(&tree_ll);
    HuffmanTreeDestruct(&tree_d);
    HuffmanTreeDestruct(&tree_cl);
    free(frequencies_ll);
    free(frequencies_d);
    free(frequencies_cl);
    free(bitlen_lld);
    free(bitlen_lld_e);

    return error;
}

#define DEFAULT_WINDOWSIZE 2048

int DEFLATESerialize(const void* const inputBuffer, const PXSize inputBufferSize, void* const outputBuffer, const PXSize outputBufferSize, PXSize* const outputBufferSizeWritten)
{
    unsigned error = 0;
    PXSize i, blocksize, numdeflateblocks;
    Hash hash;
    LodePNGBitWriter writer;

    ucvector aaucvector;

    aaucvector.data = outputBuffer;
    aaucvector.size = 0;
    aaucvector.allocsize = outputBufferSize;

    LodePNGBitWriter_init(&writer, &aaucvector);

    LodePNGCompressSettings lodePNGCompressSettings;

    lodePNGCompressSettings.btype = 2;
    lodePNGCompressSettings.use_lz77 = 1;
    lodePNGCompressSettings.windowsize = DEFAULT_WINDOWSIZE;
    lodePNGCompressSettings.minmatch = 3;
    lodePNGCompressSettings.nicematch = 128;
    lodePNGCompressSettings.lazymatching = 1;
    lodePNGCompressSettings.custom_zlib = 0;
    lodePNGCompressSettings.custom_deflate = 0;
    lodePNGCompressSettings.custom_context = 0;


    if(lodePNGCompressSettings.btype > 2) return 61;
    else if(lodePNGCompressSettings.btype == 0) return deflateNoCompression(outputBuffer, inputBuffer, inputBufferSize);
    else if(lodePNGCompressSettings.btype == 1) blocksize = inputBufferSize;
    else /*if(settings->btype == 2)*/
    {
        /*on PNGs, deflate blocks of 65-262k seem to give most dense encoding*/
        blocksize = inputBufferSize / 8u + 8;
        if(blocksize < 65536) blocksize = 65536;
        if(blocksize > 262144) blocksize = 262144;
    }

    numdeflateblocks = (inputBufferSize + blocksize - 1) / blocksize;
    if(numdeflateblocks == 0) numdeflateblocks = 1;

    error = hash_init(&hash, lodePNGCompressSettings.windowsize);

    if(!error)
    {
        for(i = 0; i != numdeflateblocks && !error; ++i)
        {
            PXSize final = (i == numdeflateblocks - 1);
            PXSize start = i * blocksize;
            PXSize end = start + blocksize;
            if(end > inputBufferSize) end = inputBufferSize;

            if(lodePNGCompressSettings.btype == 1) error = deflateFixed(&writer, &hash, inputBuffer, start, end, &lodePNGCompressSettings, final);
            else if(lodePNGCompressSettings.btype == 2) error = deflateDynamic(&writer, &hash, inputBuffer, start, end, &lodePNGCompressSettings, final);
        }
    }

    hash_cleanup(&hash);

    *outputBufferSizeWritten = writer.data->size;

    return error;
}
