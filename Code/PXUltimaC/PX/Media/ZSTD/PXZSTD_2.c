#include "PXZSTD.h"

#include <PX/Type/PXType.h>
#include <PX/OS/File/PXFile.h>

/* *******************************************************
*  Constants
*********************************************************/
#define HASH_LOG (ZSTD_MEMORY_USAGE - 2)
#define HASH_TABLESIZE (1 << HASH_LOG)
#define HASH_MASK (HASH_TABLESIZE - 1)

#define KNUTH 2654435761

#define BIT7 128
#define BIT6  64
#define BIT5  32
#define BIT4  16
#define BIT1   2
#define BIT0   1

#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)

#define BLOCKSIZE (128 KB)                 /* define, for static allocation */
#define MIN_SEQUENCES_SIZE (2 /*seqNb*/ + 2 /*dumps*/ + 3 /*seqTables*/ + 1 /*bitStream*/)
#define MIN_CBLOCK_SIZE (3 /*litCSize*/ + MIN_SEQUENCES_SIZE)
#define IS_RAW BIT0
#define IS_RLE BIT1

#define WORKPLACESIZE (BLOCKSIZE*3)
#define MINMATCH 4
#define MLbits   7
#define LLbits   6
#define Offbits  5
#define MaxML  ((1<<MLbits )-1)
#define MaxLL  ((1<<LLbits )-1)
#define MaxOff   31
#define LitFSELog  11
#define MLFSELog   10
#define LLFSELog   10
#define OffFSELog   9
#define MAX(a,b) ((a)<(b)?(b):(a))
#define MaxSeq MAX(MaxLL, MaxML)

#define LITERAL_NOENTROPY 63
#define COMMAND_NOENTROPY 7   /* to remove */

#define ZSTD_CONTENTSIZE_ERROR   (0ULL - 2)

static const size_t ZSTD_blockHeaderSize = 3;
static const size_t ZSTD_frameHeaderSize = 4;


typedef enum { bt_compressed, bt_raw, bt_rle, bt_end } blockType_t;

typedef enum {
    BIT_DStream_unfinished = 0,
    BIT_DStream_endOfBuffer = 1,
    BIT_DStream_completed = 2,
    BIT_DStream_overflow = 3
} BIT_DStream_status;  /* result of BIT_reloadDStream() */
/* 1,2,4,8 would be better for bitmap combinations, but slows down performance a bit ... :( */


typedef struct
{
    size_t   bitContainer;
    unsigned bitsConsumed;
    const char* ptr;
    const char* start;
} BIT_DStream_t;

typedef struct
{
    size_t      state;
    const void* table;   /* precise table may vary, depending on U16 */
}
FSE_DState_t;

typedef struct 
{
    size_t litLength;
    size_t offset;
    size_t matchLength;
}
seq_t;

typedef struct 
{
    BIT_DStream_t DStream;
    FSE_DState_t stateLL;
    FSE_DState_t stateOffb;
    FSE_DState_t stateML;
    size_t prevOffset;
    const BYTE* dumps;
    const BYTE* dumpsEnd;
} 
seqState_t;


typedef struct {
    PXInt16U tableLog;
    PXInt16U fastMode;
} FSE_DTableHeader;   /* sizeof U32 */

typedef struct
{
    unsigned short newState;
    unsigned char  symbol;
    unsigned char  nbBits;
} FSE_decode_t;   /* size == U32 */



#define HUF_DECODE_SYMBOLX4_0(ptr, DStreamPtr) \
    ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

#define HUF_DECODE_SYMBOLX4_1(ptr, DStreamPtr) \
    if (MEM_64bits() || (HUF_MAX_TABLELOG<=12)) \
        ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

#define HUF_DECODE_SYMBOLX4_2(ptr, DStreamPtr) \
    if (MEM_64bits()) \
        ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

typedef struct { BYTE byte; BYTE nbBits; } HUF_DEltX2;   /* single-symbol decoding */

typedef struct { PXInt16U sequence; BYTE nbBits; BYTE length; } HUF_DEltX4;  /* double-symbols decoding */

typedef struct { BYTE symbol; BYTE weight; } sortedSymbol_t;



#define FSE_CTABLE_SIZE_U32(maxTableLog, maxSymbolValue)   (1 + (1<<(maxTableLog-1)) + ((maxSymbolValue+1)*2))
#define FSE_DTABLE_SIZE_U32(maxTableLog)                   (1 + (1<<maxTableLog))


#define FSE_MAX_MEMORY_USAGE 14
#define FSE_DEFAULT_MEMORY_USAGE 13

#define FSE_MAX_TABLELOG  (FSE_MAX_MEMORY_USAGE-2)
#define FSE_MAX_TABLESIZE (1U<<FSE_MAX_TABLELOG)
#define FSE_MAXTABLESIZE_MASK (FSE_MAX_TABLESIZE-1)
#define FSE_DEFAULT_TABLELOG (FSE_DEFAULT_MEMORY_USAGE-2)
#define FSE_MIN_TABLELOG 5

#define FSE_TABLELOG_ABSOLUTE_MAX 15
#if FSE_MAX_TABLELOG > FSE_TABLELOG_ABSOLUTE_MAX
#error "FSE_MAX_TABLELOG > FSE_TABLELOG_ABSOLUTE_MAX is not supported"
#endif


struct ZSTDv03_Dctx_s
{
    PXInt32U LLTable[FSE_DTABLE_SIZE_U32(LLFSELog)];
    PXInt32U OffTable[FSE_DTABLE_SIZE_U32(OffFSELog)];
    PXInt32U MLTable[FSE_DTABLE_SIZE_U32(MLFSELog)];
    void* previousDstEnd;
    void* base;
    size_t expected;
    blockType_t bType;
    PXInt32U phase;
    const BYTE* litPtr;
    size_t litSize;
    BYTE litBuffer[BLOCKSIZE + 8 /* margin for wildcopy */];
};   /* typedef'd to ZSTD_Dctx within "zstd_static.h" */

typedef struct ZSTDv03_Dctx_s ZSTD_DCtx;

typedef struct
{
    blockType_t blockType;
    PXInt32U origSize;
} blockProperties_t;


typedef unsigned FSE_CTable;   /* don't allocate that. It's just a way to be more restrictive than void* */
typedef unsigned FSE_DTable;   /* don't allocate that. It's just a way to be more restrictive than void* */




#define ZSTD_magicNumber 0xFD2FB523   /* v0.3 */

PXActionResult PXAPI PXZSTDCompress(PXFile* const pxFileInput, PXFile* const pxFileOutput, const PXInt8U compressionLevel)
{
    return PXActionRefusedNotImplemented;
}



typedef enum {
    ZSTD_error_no_error = 0,
    ZSTD_error_GENERIC = 1,
    ZSTD_error_prefix_unknown = 10,
    ZSTD_error_version_unsupported = 12,
    ZSTD_error_frameParameter_unsupported = 14,
    ZSTD_error_frameParameter_windowTooLarge = 16,
    ZSTD_error_corruption_detected = 20,
    ZSTD_error_checksum_wrong = 22,
    ZSTD_error_literals_headerWrong = 24,
    ZSTD_error_dictionary_corrupted = 30,
    ZSTD_error_dictionary_wrong = 32,
    ZSTD_error_dictionaryCreation_failed = 34,
    ZSTD_error_parameter_unsupported = 40,
    ZSTD_error_parameter_combination_unsupported = 41,
    ZSTD_error_parameter_outOfBound = 42,
    ZSTD_error_tableLog_tooLarge = 44,
    ZSTD_error_maxSymbolValue_tooLarge = 46,
    ZSTD_error_maxSymbolValue_tooSmall = 48,
    ZSTD_error_cannotProduce_uncompressedBlock = 49,
    ZSTD_error_stabilityCondition_notRespected = 50,
    ZSTD_error_stage_wrong = 60,
    ZSTD_error_init_missing = 62,
    ZSTD_error_memory_allocation = 64,
    ZSTD_error_workSpace_tooSmall = 66,
    ZSTD_error_dstSize_tooSmall = 70,
    ZSTD_error_srcSize_wrong = 72,
    ZSTD_error_dstBuffer_null = 74,
    ZSTD_error_noForwardProgress_destFull = 80,
    ZSTD_error_noForwardProgress_inputEmpty = 82,
    /* following error codes are __NOT STABLE__, they can be removed or changed in future versions */
    ZSTD_error_frameIndex_tooLarge = 100,
    ZSTD_error_seekableIO = 102,
    ZSTD_error_dstBuffer_wrong = 104,
    ZSTD_error_srcBuffer_wrong = 105,
    ZSTD_error_sequenceProducer_failed = 106,
    ZSTD_error_externalSequences_invalid = 107,
    ZSTD_error_maxCode = 120  /* never EVER use this value directly, it can change in future versions! Use ZSTD_isError() instead */
} ZSTD_ErrorCode;
typedef ZSTD_ErrorCode ERR_enum;


#define ERROR(name) ZSTD_ERROR(name)
#define ZSTD_ERROR(name) ((size_t)-PREFIX(name))

unsigned ERR_isError(size_t code) { return (code > ERROR(ZSTD_error_maxCode)); }

ERR_enum ERR_getErrorCode(size_t code) { if(!ERR_isError(code)) return (ERR_enum)0; return (ERR_enum)(0 - code); }

/* check and forward error code */
#define CHECK_V_F(e, f)     \
    size_t const e = f;     \
    do {                    \
        if (ERR_isError(e)) \
            return e;       \
    } while (0)
#define CHECK_F(f)   do { CHECK_V_F(_var_err__, f); } while (0)


/*! ZSTD_isError
*   tells if a return value is an error code */
static unsigned ZSTD_isError(size_t code) 
{
    return ERR_isError(code); 
}



#define HUF_ABSOLUTEMAX_TABLELOG  16   /* absolute limit of HUF_MAX_TABLELOG. Beyond that value, code does not work */
#define HUF_MAX_TABLELOG  12           /* max configured tableLog (for static allocation); can be modified up to HUF_ABSOLUTEMAX_TABLELOG */
#define HUF_DEFAULT_TABLELOG  HUF_MAX_TABLELOG   /* tableLog by default, when not specified */
#define HUF_MAX_SYMBOL_VALUE 255
#if (HUF_MAX_TABLELOG > HUF_ABSOLUTEMAX_TABLELOG)
#  error "HUF_MAX_TABLELOG is too large !"
#endif


typedef PXInt32U rankVal_t[HUF_ABSOLUTEMAX_TABLELOG][HUF_ABSOLUTEMAX_TABLELOG + 1];



static short FSE_abs(short a)
{
    return a < 0 ? -a : a;
}


static size_t BIT_readBits(BIT_DStream_t* bitD, PXInt32U nbBits);
static void ZSTD_copy4(void* dst, const void* src);
static void ZSTD_copy8(void* dst, const void* src); 
static size_t ZSTD_decompressLiterals(void* dst, size_t* maxDstSizePtr, const void* src, size_t srcSize);
static size_t FSE_buildDTable_rle(FSE_DTable* dt, BYTE symbolValue);
static size_t HUF_decompress4X2(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /* single-symbol decoder */
static size_t HUF_decompress4X4(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /* double-symbols decoder */
static PXInt64U MEM_readLE64(const void* memPtr);
static size_t BIT_initDStream(BIT_DStream_t* bitD, const void* srcBuffer, size_t srcSize);
static size_t BIT_lookBitsFast(BIT_DStream_t* bitD, PXInt32U nbBits);
static void BIT_skipBits(BIT_DStream_t* bitD, PXInt32U nbBits);
static size_t FSE_readNCount(short* normalizedCounter, unsigned* maxSVPtr, unsigned* tableLogPtr, const void* headerBuffer, size_t hbSize);


static PXInt16U MEM_readLE16(const void* memPtr);
static PXInt16U MEM_read16(const void* memPtr);
static PXInt32U MEM_read32(const void* memPtr);
static PXInt64U MEM_read64(const void* memPtr);
static void HUF_fillDTableX4Level2
(
    HUF_DEltX4* DTable,
    PXInt32U sizeLog,
    const PXInt32U consumed,
    const PXInt32U* rankValOrigin,
    const int minWeight,
    const sortedSymbol_t* sortedSymbols,
    const PXInt32U sortedListSize,
    PXInt32U nbBitsBaseline,
    PXInt16U baseSeq
);
static void FSE_initDState(FSE_DState_t* DStatePtr, BIT_DStream_t* bitD, const FSE_DTable* dt);

static size_t ZSTD_decodeSeqHeaders
(
    int* nbSeq,
    const BYTE** dumpsPtr,
    size_t* dumpsLengthPtr,
    FSE_DTable* DTableLL,
    FSE_DTable* DTableML,
    FSE_DTable* DTableOffb,
    const void* src,
    size_t srcSize
);
static size_t ZSTD_decompressLiterals(void* dst, size_t* maxDstSizePtr, const void* src, size_t srcSize);
static size_t FSE_decompress_usingDTable_generic
(
    void* dst,
    size_t maxDstSize,
    const void* cSrc,
    size_t cSrcSize,
    const FSE_DTable* dt,
    const unsigned fast
);










static size_t ZSTD_getcBlockSize(const void* src, size_t srcSize, blockProperties_t* bpPtr)
{
    const BYTE* const in = (const BYTE* const)src;
    BYTE headerFlags;
    PXInt32U cSize;

    if(srcSize < 3)
        return 0;// ERROR(srcSize_wrong);

    headerFlags = *in;
    cSize = in[2] + (in[1] << 8) + ((in[0] & 7) << 16);

    bpPtr->blockType = (blockType_t)(headerFlags >> 6);
    bpPtr->origSize = (bpPtr->blockType == bt_rle) ? cSize : 0;

    if(bpPtr->blockType == bt_end) 
        return 0;

    if(bpPtr->blockType == bt_rle) 
        return 1;
    
    return cSize;
}




/** ZSTD_decodeLiteralsBlock
    @return : nb of bytes read from src (< srcSize )*/
static size_t ZSTD_decodeLiteralsBlock(void* ctx, const void* src, size_t srcSize)
{
    ZSTD_DCtx* dctx = (ZSTD_DCtx*)ctx;
    const BYTE* const istart = (const BYTE* const)src;

    /* any compressed block with literals segment must be at least this size */
    if(srcSize < MIN_CBLOCK_SIZE)
        return 0;// ERROR(corruption_detected);

    switch(*istart & 3)
    {
        default:
        case 0:
        {
            size_t litSize = BLOCKSIZE;
            const size_t readSize = ZSTD_decompressLiterals(dctx->litBuffer, &litSize, src, srcSize);
            dctx->litPtr = dctx->litBuffer;
            dctx->litSize = litSize;
            memset(dctx->litBuffer + dctx->litSize, 0, 8);
            return readSize;   /* works if it's an error too */
        }
        case IS_RAW:
        {
            const size_t litSize = (MEM_readLE32(istart) & 0xFFFFFF) >> 2;   /* no buffer issue : srcSize >= MIN_CBLOCK_SIZE */
            if(litSize > srcSize - 11)   /* risk of reading too far with wildcopy */
            {
                if(litSize > BLOCKSIZE)
                    return 0;// ERROR(corruption_detected);

                if(litSize > srcSize - 3)
                    return 0;// ERROR(corruption_detected);

                memcpy(dctx->litBuffer, istart, litSize);
                dctx->litPtr = dctx->litBuffer;
                dctx->litSize = litSize;
                memset(dctx->litBuffer + dctx->litSize, 0, 8);
                return litSize + 3;
            }
            /* direct reference into compressed stream */
            dctx->litPtr = istart + 3;
            dctx->litSize = litSize;
            return litSize + 3;
        }
        case IS_RLE:
        {
            const size_t litSize = (MEM_readLE32(istart) & 0xFFFFFF) >> 2;   /* no buffer issue : srcSize >= MIN_CBLOCK_SIZE */
           
            if(litSize > BLOCKSIZE)
                return 0;// ERROR(corruption_detected);
           
            memset(dctx->litBuffer, istart[3], litSize + 8);
            dctx->litPtr = dctx->litBuffer;
            dctx->litSize = litSize;
            return 4;
        }
    }
}


/****************************************************************
*  Error Management
****************************************************************/
#define HUF_STATIC_ASSERT(c) { enum { HUF_static_assert = 1/(int)(!!(c)) }; }   /* use only *after* variable declarations */


/******************************************
*  Helper functions
******************************************/
static unsigned HUF_isError(size_t code) { return ERR_isError(code); }

#define HUF_ABSOLUTEMAX_TABLELOG  16   /* absolute limit of HUF_MAX_TABLELOG. Beyond that value, code does not work */
#define HUF_MAX_TABLELOG  12           /* max configured tableLog (for static allocation); can be modified up to HUF_ABSOLUTEMAX_TABLELOG */
#define HUF_DEFAULT_TABLELOG  HUF_MAX_TABLELOG   /* tableLog by default, when not specified */
#define HUF_MAX_SYMBOL_VALUE 255
#if (HUF_MAX_TABLELOG > HUF_ABSOLUTEMAX_TABLELOG)
#  error "HUF_MAX_TABLELOG is too large !"
#endif






#define HUF_CTABLEBOUND 129
#define HUF_BLOCKBOUND(size) (size + (size>>8) + 8)   /* only true if incompressible pre-filtered with fast heuristic */
#define HUF_COMPRESSBOUND(size) (HUF_CTABLEBOUND + HUF_BLOCKBOUND(size))   /* Macro version, useful for static allocation */

/* static allocation of Huff0's DTable */
#define HUF_DTABLE_SIZE(maxTableLog)   (1 + (1<<maxTableLog))  /* nb Cells; use unsigned short for X2, unsigned int for X4 */
#define HUF_CREATE_STATIC_DTABLEX2(DTable, maxTableLog) \
        unsigned short DTable[HUF_DTABLE_SIZE(maxTableLog)] = { maxTableLog }
#define HUF_CREATE_STATIC_DTABLEX4(DTable, maxTableLog) \
        unsigned int DTable[HUF_DTABLE_SIZE(maxTableLog)] = { maxTableLog }
#define HUF_CREATE_STATIC_DTABLEX6(DTable, maxTableLog) \
        unsigned int DTable[HUF_DTABLE_SIZE(maxTableLog) * 3 / 2] = { maxTableLog }















typedef PXInt32U DTable_max_t[FSE_DTABLE_SIZE_U32(FSE_MAX_TABLELOG)];
static unsigned FSE_isError(size_t code) { return ERR_isError(code); }


/* FSE_MAX_SYMBOL_VALUE :
*  Maximum symbol value authorized.
*  Required for proper stack allocation */
#define FSE_MAX_SYMBOL_VALUE 255

static size_t FSE_decompress(void* dst, size_t maxDstSize, const void* cSrc, size_t cSrcSize)
{
    const BYTE* const istart = (const BYTE*)cSrc;
    const BYTE* ip = istart;
    short counting[FSE_MAX_SYMBOL_VALUE + 1];
    DTable_max_t dt;   /* Static analyzer seems unable to understand this table will be properly initialized later */
    unsigned tableLog;
    unsigned maxSymbolValue = FSE_MAX_SYMBOL_VALUE;
    size_t errorCode;

    if(cSrcSize < 2)
        return 0;// ERROR(srcSize_wrong);   /* too small input size */

    /* normal FSE decoding mode */
    errorCode = FSE_readNCount(counting, &maxSymbolValue, &tableLog, istart, cSrcSize);
    if(FSE_isError(errorCode)) 
        return errorCode;

    if(errorCode >= cSrcSize) 
        return;//ERROR(srcSize_wrong);   /* too small input size */

    ip += errorCode;
    cSrcSize -= errorCode;

    errorCode = FSE_buildDTable(dt, counting, maxSymbolValue, tableLog);

    if(FSE_isError(errorCode)) 
        return errorCode;

    /* always return, even if it is an error code */
    return FSE_decompress_usingDTable(dst, maxDstSize, ip, cSrcSize, dt);
}



/*! HUF_readStats
    Read compact Huffman tree, saved by HUF_writeCTable
    @huffWeight : destination buffer
    @return : size read from `src`
*/
static size_t HUF_readStats(BYTE* huffWeight, size_t hwSize, PXInt32U* rankStats,
                            PXInt32U* nbSymbolsPtr, PXInt32U* tableLogPtr,
                            const void* src, size_t srcSize)
{
    PXInt32U weightTotal;
    PXInt32U tableLog;
    const BYTE* ip = (const BYTE*)src;
    size_t iSize;
    size_t oSize;
    PXInt32U n;

    if(!srcSize) return 0;// ERROR(srcSize_wrong);
    iSize = ip[0];
    //memset(huffWeight, 0, hwSize);   /* is not necessary, even though some analyzer complain ... */

    if(iSize >= 128)  /* special header */
    {
        if(iSize >= (242))   /* RLE */
        {
            static int l[14] = { 1, 2, 3, 4, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128 };
            oSize = l[iSize - 242];
            memset(huffWeight, 1, hwSize);
            iSize = 0;
        }
        else   /* Incompressible */
        {
            oSize = iSize - 127;
            iSize = ((oSize + 1) / 2);
            if(iSize + 1 > srcSize) return 0;//ERROR(srcSize_wrong);
            if(oSize >= hwSize) return 0;// ERROR(corruption_detected);
            ip += 1;
            for(n = 0; n < oSize; n += 2)
            {
                huffWeight[n] = ip[n / 2] >> 4;
                huffWeight[n + 1] = ip[n / 2] & 15;
            }
        }
    }
    else  /* header compressed with FSE (normal case) */
    {
        if(iSize + 1 > srcSize)
            return 0;//ERROR(srcSize_wrong);

        oSize = FSE_decompress(huffWeight, hwSize - 1, ip + 1, iSize);   /* max (hwSize-1) values decoded, as last one is implied */
       
        if(FSE_isError(oSize)) 
            return oSize;
    }

    /* collect weight stats */
    memset(rankStats, 0, (HUF_ABSOLUTEMAX_TABLELOG + 1) * sizeof(PXInt32U));
    weightTotal = 0;

    for(n = 0; n < oSize; n++)
    {
        if(huffWeight[n] >= HUF_ABSOLUTEMAX_TABLELOG)
            return 0;//ERROR(corruption_detected);

        rankStats[huffWeight[n]]++;
        weightTotal += (1 << huffWeight[n]) >> 1;
    }

    if(weightTotal == 0)
        return 0;//ERROR(corruption_detected);

    /* get last non-null symbol weight (implied, total must be 2^n) */
    tableLog = BIT_highbit32(weightTotal) + 1;
    if(tableLog > HUF_ABSOLUTEMAX_TABLELOG) 
        return 0;//ERROR(corruption_detected);
    {
        PXInt32U total = 1 << tableLog;
        PXInt32U rest = total - weightTotal;
        PXInt32U verif = 1 << BIT_highbit32(rest);
        PXInt32U lastWeight = BIT_highbit32(rest) + 1;
        
        if(verif != rest)
            return 0;// ERROR(corruption_detected);    /* last value must be a clean power of 2 */

        huffWeight[oSize] = (BYTE)lastWeight;
        rankStats[lastWeight]++;
    }

    /* check tree construction validity */
    if((rankStats[1] < 2) || (rankStats[1] & 1))
        return 0;// ERROR(corruption_detected);   /* by construction : at least 2 elts of rank 1, must be even */

    /* results */
    *nbSymbolsPtr = (PXInt32U)(oSize + 1);
    *tableLogPtr = tableLog;
    return iSize + 1;
}



static void HUF_fillDTableX4Level2
(
    HUF_DEltX4* DTable,
    PXInt32U sizeLog,
    const PXInt32U consumed,
    const PXInt32U* rankValOrigin,
    const int minWeight,
    const sortedSymbol_t* sortedSymbols,
    const PXInt32U sortedListSize,
    PXInt32U nbBitsBaseline,
    PXInt16U baseSeq
)
{
    HUF_DEltX4 DElt;
    PXInt32U rankVal[HUF_ABSOLUTEMAX_TABLELOG + 1];
    PXInt32U s;

    /* get pre-calculated rankVal */
    memcpy(rankVal, rankValOrigin, sizeof(rankVal));

    /* fill skipped values */
    if(minWeight > 1)
    {
        PXInt32U i, skipSize = rankVal[minWeight];
        MEM_writeLE16(&(DElt.sequence), baseSeq);
        DElt.nbBits = (BYTE)(consumed);
        DElt.length = 1;
        for(i = 0; i < skipSize; i++)
            DTable[i] = DElt;
    }

    /* fill DTable */
    for(s = 0; s < sortedListSize; s++)   /* note : sortedSymbols already skipped */
    {
        const PXInt32U symbol = sortedSymbols[s].symbol;
        const PXInt32U weight = sortedSymbols[s].weight;
        const PXInt32U nbBits = nbBitsBaseline - weight;
        const PXInt32U length = 1 << (sizeLog - nbBits);
        const PXInt32U start = rankVal[weight];
        PXInt32U i = start;
        const PXInt32U end = start + length;

        MEM_writeLE16(&(DElt.sequence), (PXInt16U)(baseSeq + (symbol << 8)));
        DElt.nbBits = (BYTE)(nbBits + consumed);
        DElt.length = 2;
        do { DTable[i++] = DElt; } while(i < end);   /* since length >= 1 */

        rankVal[weight] += length;
    }
}


static void HUF_fillDTableX4
(
    HUF_DEltX4* DTable,
    const PXInt32U targetLog,
    const sortedSymbol_t* sortedList,
    const PXInt32U sortedListSize,
    const PXInt32U* rankStart,
    rankVal_t rankValOrigin,
    const PXInt32U maxWeight,
    const PXInt32U nbBitsBaseline
)
{
    PXInt32U rankVal[HUF_ABSOLUTEMAX_TABLELOG + 1];
    const int scaleLog = nbBitsBaseline - targetLog;   /* note : targetLog >= srcLog, hence scaleLog <= 1 */
    const PXInt32U minBits = nbBitsBaseline - maxWeight;
    PXInt32U s;

    memcpy(rankVal, rankValOrigin, sizeof(rankVal));

    /* fill DTable */
    for(s = 0; s < sortedListSize; s++)
    {
        const PXInt16U symbol = sortedList[s].symbol;
        const PXInt32U weight = sortedList[s].weight;
        const PXInt32U nbBits = nbBitsBaseline - weight;
        const PXInt32U start = rankVal[weight];
        const PXInt32U length = 1 << (targetLog - nbBits);

        if(targetLog - nbBits >= minBits)   /* enough room for a second symbol */
        {
            PXInt32U sortedRank;
            int minWeight = nbBits + scaleLog;
            if(minWeight < 1) minWeight = 1;
            sortedRank = rankStart[minWeight];
            HUF_fillDTableX4Level2(DTable + start, targetLog - nbBits, nbBits,
                                   rankValOrigin[nbBits], minWeight,
                                   sortedList + sortedRank, sortedListSize - sortedRank,
                                   nbBitsBaseline, symbol);
        }
        else
        {
            PXInt32U i;
            const PXInt32U end = start + length;
            HUF_DEltX4 DElt;

            MEM_writeLE16(&(DElt.sequence), symbol);
            DElt.nbBits = (BYTE)(nbBits);
            DElt.length = 1;
            for(i = start; i < end; i++)
                DTable[i] = DElt;
        }
        rankVal[weight] += length;
    }
}

static size_t HUF_readDTableX4(PXInt32U* DTable, const void* src, size_t srcSize)
{
    BYTE weightList[HUF_MAX_SYMBOL_VALUE + 1];
    sortedSymbol_t sortedSymbol[HUF_MAX_SYMBOL_VALUE + 1];
    PXInt32U rankStats[HUF_ABSOLUTEMAX_TABLELOG + 1] = { 0 };
    PXInt32U rankStart0[HUF_ABSOLUTEMAX_TABLELOG + 2] = { 0 };
    PXInt32U* const rankStart = rankStart0 + 1;
    rankVal_t rankVal;
    PXInt32U tableLog, maxW, sizeOfSort, nbSymbols;
    const PXInt32U memLog = DTable[0];
    const BYTE* ip = (const BYTE*)src;
    size_t iSize = ip[0];
    void* ptr = DTable;
    HUF_DEltX4* const dt = ((HUF_DEltX4*)ptr) + 1;

    HUF_STATIC_ASSERT(sizeof(HUF_DEltX4) == sizeof(PXInt32U));   /* if compilation fails here, assertion is false */
    if(memLog > HUF_ABSOLUTEMAX_TABLELOG) return 0;// ERROR(tableLog_tooLarge);
    //memset(weightList, 0, sizeof(weightList));   /* is not necessary, even though some analyzer complain ... */

    iSize = HUF_readStats(weightList, HUF_MAX_SYMBOL_VALUE + 1, rankStats, &nbSymbols, &tableLog, src, srcSize);
    if(HUF_isError(iSize)) return iSize;

    /* check result */
    if(tableLog > memLog) return 0;// ERROR(tableLog_tooLarge);   /* DTable can't fit code depth */

    /* find maxWeight */
    for(maxW = tableLog; rankStats[maxW] == 0; maxW--)
    {
        if(!maxW) return 0;// ERROR(GENERIC);
    }  /* necessarily finds a solution before maxW==0 */

/* Get start index of each weight */
    {
        PXInt32U w, nextRankStart = 0;
        for(w = 1; w <= maxW; w++)
        {
            PXInt32U current = nextRankStart;
            nextRankStart += rankStats[w];
            rankStart[w] = current;
        }
        rankStart[0] = nextRankStart;   /* put all 0w symbols at the end of sorted list*/
        sizeOfSort = nextRankStart;
    }

    /* sort symbols by weight */
    {
        PXInt32U s;
        for(s = 0; s < nbSymbols; s++)
        {
            PXInt32U w = weightList[s];
            PXInt32U r = rankStart[w]++;
            sortedSymbol[r].symbol = (BYTE)s;
            sortedSymbol[r].weight = (BYTE)w;
        }
        rankStart[0] = 0;   /* forget 0w symbols; this is beginning of weight(1) */
    }

    /* Build rankVal */
    {
        const PXInt32U minBits = tableLog + 1 - maxW;
        PXInt32U nextRankVal = 0;
        PXInt32U w, consumed;
        const int rescale = (memLog - tableLog) - 1;   /* tableLog <= memLog */
        PXInt32U* rankVal0 = rankVal[0];
        for(w = 1; w <= maxW; w++)
        {
            PXInt32U current = nextRankVal;
            nextRankVal += rankStats[w] << (w + rescale);
            rankVal0[w] = current;
        }
        for(consumed = minBits; consumed <= memLog - minBits; consumed++)
        {
            PXInt32U* rankValPtr = rankVal[consumed];
            for(w = 1; w <= maxW; w++)
            {
                rankValPtr[w] = rankVal0[w] >> consumed;
            }
        }
    }

    HUF_fillDTableX4
    (
        dt,
        memLog,
        sortedSymbol,
        sizeOfSort,
        rankStart0,
        rankVal,
        maxW,
        tableLog + 1
    );

    return iSize;
}
















size_t FSE_decompress_usingDTable_generic
(
    void* dst,
    size_t maxDstSize,
    const void* cSrc,
    size_t cSrcSize,
    const FSE_DTable* dt,
    const unsigned fast
)
{
    BYTE* const ostart = (BYTE*)dst;
    BYTE* op = ostart;
    BYTE* const omax = op + maxDstSize;
    BYTE* const olimit = omax - 3;

    BIT_DStream_t bitD;
    FSE_DState_t state1;
    FSE_DState_t state2;
    size_t errorCode;

    /* Init */
    errorCode = BIT_initDStream(&bitD, cSrc, cSrcSize);   /* replaced last arg by maxCompressed Size */
    if(FSE_isError(errorCode)) return errorCode;

    FSE_initDState(&state1, &bitD, dt);
    FSE_initDState(&state2, &bitD, dt);

#define FSE_GETSYMBOL(statePtr) fast ? FSE_decodeSymbolFast(statePtr, &bitD) : FSE_decodeSymbol(statePtr, &bitD)

    /* 4 symbols per loop */
    for(; (BIT_reloadDStream(&bitD) == BIT_DStream_unfinished) && (op < olimit); op += 4)
    {
        op[0] = FSE_GETSYMBOL(&state1);

        if(FSE_MAX_TABLELOG * 2 + 7 > sizeof(bitD.bitContainer) * 8)    /* This test must be static */
            BIT_reloadDStream(&bitD);

        op[1] = FSE_GETSYMBOL(&state2);

        if(FSE_MAX_TABLELOG * 4 + 7 > sizeof(bitD.bitContainer) * 8)    /* This test must be static */
        {
            if(BIT_reloadDStream(&bitD) > BIT_DStream_unfinished) { op += 2; break; }
        }

        op[2] = FSE_GETSYMBOL(&state1);

        if(FSE_MAX_TABLELOG * 2 + 7 > sizeof(bitD.bitContainer) * 8)    /* This test must be static */
            BIT_reloadDStream(&bitD);

        op[3] = FSE_GETSYMBOL(&state2);
    }

    /* tail */
    /* note : BIT_reloadDStream(&bitD) >= FSE_DStream_partiallyFilled; Ends at exactly BIT_DStream_completed */
    while(1)
    {
        if((BIT_reloadDStream(&bitD) > BIT_DStream_completed) || (op == omax) || (BIT_endOfDStream(&bitD) && (fast || FSE_endOfDState(&state1))))
            break;

        *op++ = FSE_GETSYMBOL(&state1);

        if((BIT_reloadDStream(&bitD) > BIT_DStream_completed) || (op == omax) || (BIT_endOfDStream(&bitD) && (fast || FSE_endOfDState(&state2))))
            break;

        *op++ = FSE_GETSYMBOL(&state2);
    }

    /* end ? */
    if(BIT_endOfDStream(&bitD) && FSE_endOfDState(&state1) && FSE_endOfDState(&state2))
        return op - ostart;

    if(op == omax) 
        return 0;//ERROR(dstSize_tooSmall);   /* dst buffer is full, but cSrc unfinished */

    return 0;// ERROR(corruption_detected);
}








static size_t HUF_readDTableX2(PXInt16U* DTable, const void* src, size_t srcSize)
{
    BYTE huffWeight[HUF_MAX_SYMBOL_VALUE + 1];
    PXInt32U rankVal[HUF_ABSOLUTEMAX_TABLELOG + 1];   /* large enough for values from 0 to 16 */
    PXInt32U tableLog = 0;
    const BYTE* ip = (const BYTE*)src;
    size_t iSize = ip[0];
    PXInt32U nbSymbols = 0;
    PXInt32U n;
    PXInt32U nextRankStart;
    void* ptr = DTable + 1;
    HUF_DEltX2* const dt = (HUF_DEltX2*)(ptr);

   // HUF_STATIC_ASSERT(sizeof(HUF_DEltX2) == sizeof(U16));   /* if compilation fails here, assertion is false */
    //memset(huffWeight, 0, sizeof(huffWeight));   /* is not necessary, even though some analyzer complain ... */

    iSize = HUF_readStats(huffWeight, HUF_MAX_SYMBOL_VALUE + 1, rankVal, &nbSymbols, &tableLog, src, srcSize);
    if(HUF_isError(iSize)) return iSize;

    /* check result */
    if(tableLog > DTable[0]) return 0;//ERROR(tableLog_tooLarge);   /* DTable is too small */
    DTable[0] = (PXInt16U)tableLog;   /* maybe should separate sizeof DTable, as allocated, from used size of DTable, in case of DTable re-use */

    /* Prepare ranks */
    nextRankStart = 0;
    for(n = 1; n <= tableLog; n++)
    {
        PXInt32U current = nextRankStart;
        nextRankStart += (rankVal[n] << (n - 1));
        rankVal[n] = current;
    }

    /* fill DTable */
    for(n = 0; n < nbSymbols; n++)
    {
        const PXInt32U w = huffWeight[n];
        const PXInt32U length = (1 << w) >> 1;
        PXInt32U i;
        HUF_DEltX2 D;

        D.byte = (BYTE)n; 
        D.nbBits = (BYTE)(tableLog + 1 - w);

        for(i = rankVal[w]; i < rankVal[w] + length; i++)
            dt[i] = D;

        rankVal[w] += length;
    }

    return iSize;
}

#define HUF_DECODE_SYMBOLX2_0(ptr, DStreamPtr) \
    *ptr++ = HUF_decodeSymbolX2(DStreamPtr, dt, dtLog)

#define HUF_DECODE_SYMBOLX2_1(ptr, DStreamPtr) \
    if (MEM_64bits() || (HUF_MAX_TABLELOG<=12)) \
        HUF_DECODE_SYMBOLX2_0(ptr, DStreamPtr)

#define HUF_DECODE_SYMBOLX2_2(ptr, DStreamPtr) \
    if (MEM_64bits()) \
        HUF_DECODE_SYMBOLX2_0(ptr, DStreamPtr)














typedef struct { PXInt32U tableTime; PXInt32U decode256Time; } algo_time_t;
static const algo_time_t algoTime[16 /* Quantization */][3 /* single, double, quad */] =
{
    /* single, double, quad */
    {{0,0}, {1,1}, {2,2}},  /* Q==0 : impossible */
    {{0,0}, {1,1}, {2,2}},  /* Q==1 : impossible */
    {{  38,130}, {1313, 74}, {2151, 38}},   /* Q == 2 : 12-18% */
    {{ 448,128}, {1353, 74}, {2238, 41}},   /* Q == 3 : 18-25% */
    {{ 556,128}, {1353, 74}, {2238, 47}},   /* Q == 4 : 25-32% */
    {{ 714,128}, {1418, 74}, {2436, 53}},   /* Q == 5 : 32-38% */
    {{ 883,128}, {1437, 74}, {2464, 61}},   /* Q == 6 : 38-44% */
    {{ 897,128}, {1515, 75}, {2622, 68}},   /* Q == 7 : 44-50% */
    {{ 926,128}, {1613, 75}, {2730, 75}},   /* Q == 8 : 50-56% */
    {{ 947,128}, {1729, 77}, {3359, 77}},   /* Q == 9 : 56-62% */
    {{1107,128}, {2083, 81}, {4006, 84}},   /* Q ==10 : 62-69% */
    {{1177,128}, {2379, 87}, {4785, 88}},   /* Q ==11 : 69-75% */
    {{1242,128}, {2415, 93}, {5155, 84}},   /* Q ==12 : 75-81% */
    {{1349,128}, {2644,106}, {5260,106}},   /* Q ==13 : 81-87% */
    {{1455,128}, {2422,124}, {4174,124}},   /* Q ==14 : 87-93% */
    {{ 722,128}, {1891,145}, {1936,146}},   /* Q ==15 : 93-99% */
};

typedef size_t(*decompressionAlgo)(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);

static size_t HUF_decompress(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    static const decompressionAlgo decompress[3] = { HUF_decompress4X2, HUF_decompress4X4, NULL };
    /* estimate decompression time */
    PXInt32U Q;
    const PXInt32U D256 = (PXInt32U)(dstSize >> 8);
    PXInt32U Dtime[3];
    PXInt32U algoNb = 0;
    int n;

    /* validation checks */
    if(dstSize == 0) return 0;// ERROR(dstSize_tooSmall);
    if(cSrcSize > dstSize) return 0;//ERROR(corruption_detected);   /* invalid */
    if(cSrcSize == dstSize) { memcpy(dst, cSrc, dstSize); return dstSize; }   /* not compressed */
    if(cSrcSize == 1) { memset(dst, *(const BYTE*)cSrc, dstSize); return dstSize; }   /* RLE */

    /* decoder timing evaluation */
    Q = (PXInt32U)(cSrcSize * 16 / dstSize);   /* Q < 16 since dstSize > cSrcSize */
    for(n = 0; n < 3; n++)
        Dtime[n] = algoTime[Q][n].tableTime + (algoTime[Q][n].decode256Time * D256);

    Dtime[1] += Dtime[1] >> 4; Dtime[2] += Dtime[2] >> 3; /* advantage to algorithms using less memory, for cache eviction */

    if(Dtime[1] < Dtime[0]) algoNb = 1;

    return decompress[algoNb](dst, dstSize, cSrc, cSrcSize);

    //return HUF_decompress4X2(dst, dstSize, cSrc, cSrcSize);   /* multi-streams single-symbol decoding */
    //return HUF_decompress4X4(dst, dstSize, cSrc, cSrcSize);   /* multi-streams double-symbols decoding */
    //return HUF_decompress4X6(dst, dstSize, cSrc, cSrcSize);   /* multi-streams quad-symbols decoding */
}



















static size_t HUF_decompress4X2_usingDTable(
    void* dst, size_t dstSize,
    const void* cSrc, size_t cSrcSize,
    const PXInt16U* DTable)
{
    if(cSrcSize < 10)
        return 0;// ERROR(corruption_detected);   /* strict minimum : jump table + 1 byte per stream */

    {
        const BYTE* const istart = (const BYTE*)cSrc;
        BYTE* const ostart = (BYTE*)dst;
        BYTE* const oend = ostart + dstSize;

        const void* ptr = DTable;
        const HUF_DEltX2* const dt = ((const HUF_DEltX2*)ptr) + 1;
        const PXInt32U dtLog = DTable[0];
        size_t errorCode;

        /* Init */
        BIT_DStream_t bitD1;
        BIT_DStream_t bitD2;
        BIT_DStream_t bitD3;
        BIT_DStream_t bitD4;
        const size_t length1 = MEM_readLE16(istart);
        const size_t length2 = MEM_readLE16(istart + 2);
        const size_t length3 = MEM_readLE16(istart + 4);
        size_t length4;
        const BYTE* const istart1 = istart + 6;  /* jumpTable */
        const BYTE* const istart2 = istart1 + length1;
        const BYTE* const istart3 = istart2 + length2;
        const BYTE* const istart4 = istart3 + length3;
        const size_t segmentSize = (dstSize + 3) / 4;
        BYTE* const opStart2 = ostart + segmentSize;
        BYTE* const opStart3 = opStart2 + segmentSize;
        BYTE* const opStart4 = opStart3 + segmentSize;
        BYTE* op1 = ostart;
        BYTE* op2 = opStart2;
        BYTE* op3 = opStart3;
        BYTE* op4 = opStart4;
        PXInt32U endSignal;

        length4 = cSrcSize - (length1 + length2 + length3 + 6);
        if(length4 > cSrcSize)
            return 0;// ERROR(corruption_detected);   /* overflow */

        errorCode = BIT_initDStream(&bitD1, istart1, length1);
        if(HUF_isError(errorCode))
            return errorCode;

        errorCode = BIT_initDStream(&bitD2, istart2, length2);
        if(HUF_isError(errorCode)) 
            return errorCode;

        errorCode = BIT_initDStream(&bitD3, istart3, length3);
        if(HUF_isError(errorCode)) 
            return errorCode;

        errorCode = BIT_initDStream(&bitD4, istart4, length4);
        if(HUF_isError(errorCode)) 
            return errorCode;

        /* 16-32 symbols per loop (4-8 symbols per stream) */
        endSignal = BIT_reloadDStream(&bitD1) | BIT_reloadDStream(&bitD2) | BIT_reloadDStream(&bitD3) | BIT_reloadDStream(&bitD4);
        for(; (endSignal == BIT_DStream_unfinished) && (op4 < (oend - 7)); )
        {
            HUF_DECODE_SYMBOLX2_2(op1, &bitD1);
            HUF_DECODE_SYMBOLX2_2(op2, &bitD2);
            HUF_DECODE_SYMBOLX2_2(op3, &bitD3);
            HUF_DECODE_SYMBOLX2_2(op4, &bitD4);
            HUF_DECODE_SYMBOLX2_1(op1, &bitD1);
            HUF_DECODE_SYMBOLX2_1(op2, &bitD2);
            HUF_DECODE_SYMBOLX2_1(op3, &bitD3);
            HUF_DECODE_SYMBOLX2_1(op4, &bitD4);
            HUF_DECODE_SYMBOLX2_2(op1, &bitD1);
            HUF_DECODE_SYMBOLX2_2(op2, &bitD2);
            HUF_DECODE_SYMBOLX2_2(op3, &bitD3);
            HUF_DECODE_SYMBOLX2_2(op4, &bitD4);
            HUF_DECODE_SYMBOLX2_0(op1, &bitD1);
            HUF_DECODE_SYMBOLX2_0(op2, &bitD2);
            HUF_DECODE_SYMBOLX2_0(op3, &bitD3);
            HUF_DECODE_SYMBOLX2_0(op4, &bitD4);

            endSignal = BIT_reloadDStream(&bitD1) | BIT_reloadDStream(&bitD2) | BIT_reloadDStream(&bitD3) | BIT_reloadDStream(&bitD4);
        }

        /* check corruption */
        if(op1 > opStart2) return 0;//ERROR(corruption_detected);
        if(op2 > opStart3) return 0;//ERROR(corruption_detected);
        if(op3 > opStart4) return 0;// ERROR(corruption_detected);
        /* note : op4 supposed already verified within main loop */

        /* finish bitStreams one by one */
        HUF_decodeStreamX2(op1, &bitD1, opStart2, dt, dtLog);
        HUF_decodeStreamX2(op2, &bitD2, opStart3, dt, dtLog);
        HUF_decodeStreamX2(op3, &bitD3, opStart4, dt, dtLog);
        HUF_decodeStreamX2(op4, &bitD4, oend, dt, dtLog);

        /* check */
        endSignal = BIT_endOfDStream(&bitD1) & BIT_endOfDStream(&bitD2) & BIT_endOfDStream(&bitD3) & BIT_endOfDStream(&bitD4);
        if(!endSignal) return 0;//ERROR(corruption_detected);

        /* decoded size */
        return dstSize;
    }
}


size_t HUF_decompress4X2(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX2(DTable, HUF_MAX_TABLELOG);
    const BYTE* ip = (const BYTE*)cSrc;
    size_t errorCode;

    errorCode = HUF_readDTableX2(DTable, cSrc, cSrcSize);
    if(HUF_isError(errorCode)) 
        return errorCode;

    if(errorCode >= cSrcSize)
        return 0;// ERROR(srcSize_wrong);

    ip += errorCode;
    cSrcSize -= errorCode;

    return HUF_decompress4X2_usingDTable(dst, dstSize, ip, cSrcSize, DTable);
}






static PXInt32U HUF_decodeLastSymbolX4(void* op, BIT_DStream_t* DStream, const HUF_DEltX4* dt, const PXInt32U dtLog)
{
    const size_t val = BIT_lookBitsFast(DStream, dtLog);   /* note : dtLog >= 1 */
    memcpy(op, dt + val, 1);
    if(dt[val].length == 1) BIT_skipBits(DStream, dt[val].nbBits);
    else
    {
        if(DStream->bitsConsumed < (sizeof(DStream->bitContainer) * 8))
        {
            BIT_skipBits(DStream, dt[val].nbBits);
            if(DStream->bitsConsumed > (sizeof(DStream->bitContainer) * 8))
                DStream->bitsConsumed = (sizeof(DStream->bitContainer) * 8);   /* ugly hack; works only because it's the last symbol. Note : can't easily extract nbBits from just this symbol */
        }
    }
    return 1;
}




#define HUF_DECODE_SYMBOLX4_0(ptr, DStreamPtr) \
    ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

#define HUF_DECODE_SYMBOLX4_1(ptr, DStreamPtr) \
    if (MEM_64bits() || (HUF_MAX_TABLELOG<=12)) \
        ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

#define HUF_DECODE_SYMBOLX4_2(ptr, DStreamPtr) \
    if (MEM_64bits()) \
        ptr += HUF_decodeSymbolX4(ptr, DStreamPtr, dt, dtLog)

static inline size_t HUF_decodeStreamX4(BYTE* p, BIT_DStream_t* bitDPtr, BYTE* const pEnd, const HUF_DEltX4* const dt, const PXInt32U dtLog)
{
    BYTE* const pStart = p;

    /* up to 8 symbols at a time */
    while((BIT_reloadDStream(bitDPtr) == BIT_DStream_unfinished) && (p < pEnd - 7))
    {
        HUF_DECODE_SYMBOLX4_2(p, bitDPtr);
        HUF_DECODE_SYMBOLX4_1(p, bitDPtr);
        HUF_DECODE_SYMBOLX4_2(p, bitDPtr);
        HUF_DECODE_SYMBOLX4_0(p, bitDPtr);
    }

    /* closer to the end */
    while((BIT_reloadDStream(bitDPtr) == BIT_DStream_unfinished) && (p <= pEnd - 2))
        HUF_DECODE_SYMBOLX4_0(p, bitDPtr);

    while(p <= pEnd - 2)
        HUF_DECODE_SYMBOLX4_0(p, bitDPtr);   /* no need to reload : reached the end of DStream */

    if(p < pEnd)
        p += HUF_decodeLastSymbolX4(p, bitDPtr, dt, dtLog);

    return p - pStart;
}






static size_t HUF_decompress4X4_usingDTable(
    void* dst, size_t dstSize,
    const void* cSrc, size_t cSrcSize,
    const PXInt32U* DTable)
{
    if(cSrcSize < 10)
        return 0;// ERROR(corruption_detected);   /* strict minimum : jump table + 1 byte per stream */

    {
        const BYTE* const istart = (const BYTE*)cSrc;
        BYTE* const ostart = (BYTE*)dst;
        BYTE* const oend = ostart + dstSize;

        const void* ptr = DTable;
        const HUF_DEltX4* const dt = ((const HUF_DEltX4*)ptr) + 1;
        const PXInt32U dtLog = DTable[0];
        size_t errorCode;

        /* Init */
        BIT_DStream_t bitD1;
        BIT_DStream_t bitD2;
        BIT_DStream_t bitD3;
        BIT_DStream_t bitD4;
        const size_t length1 = MEM_readLE16(istart);
        const size_t length2 = MEM_readLE16(istart + 2);
        const size_t length3 = MEM_readLE16(istart + 4);
        size_t length4;
        const BYTE* const istart1 = istart + 6;  /* jumpTable */
        const BYTE* const istart2 = istart1 + length1;
        const BYTE* const istart3 = istart2 + length2;
        const BYTE* const istart4 = istart3 + length3;
        const size_t segmentSize = (dstSize + 3) / 4;
        BYTE* const opStart2 = ostart + segmentSize;
        BYTE* const opStart3 = opStart2 + segmentSize;
        BYTE* const opStart4 = opStart3 + segmentSize;
        BYTE* op1 = ostart;
        BYTE* op2 = opStart2;
        BYTE* op3 = opStart3;
        BYTE* op4 = opStart4;
        PXInt32U endSignal;

        length4 = cSrcSize - (length1 + length2 + length3 + 6);
        if(length4 > cSrcSize)
            return 0;// ERROR(corruption_detected);   /* overflow */

        errorCode = BIT_initDStream(&bitD1, istart1, length1);

        if(HUF_isError(errorCode)) 
            return errorCode;

        errorCode = BIT_initDStream(&bitD2, istart2, length2);

        if(HUF_isError(errorCode)) 
            return errorCode;

        errorCode = BIT_initDStream(&bitD3, istart3, length3);

        if(HUF_isError(errorCode))
            return errorCode;

        errorCode = BIT_initDStream(&bitD4, istart4, length4);

        if(HUF_isError(errorCode)) 
            return errorCode;

        /* 16-32 symbols per loop (4-8 symbols per stream) */
        endSignal = BIT_reloadDStream(&bitD1) | BIT_reloadDStream(&bitD2) | BIT_reloadDStream(&bitD3) | BIT_reloadDStream(&bitD4);
        for(; (endSignal == BIT_DStream_unfinished) && (op4 < (oend - 7)); )
        {
            HUF_DECODE_SYMBOLX4_2(op1, &bitD1);
            HUF_DECODE_SYMBOLX4_2(op2, &bitD2);
            HUF_DECODE_SYMBOLX4_2(op3, &bitD3);
            HUF_DECODE_SYMBOLX4_2(op4, &bitD4);
            HUF_DECODE_SYMBOLX4_1(op1, &bitD1);
            HUF_DECODE_SYMBOLX4_1(op2, &bitD2);
            HUF_DECODE_SYMBOLX4_1(op3, &bitD3);
            HUF_DECODE_SYMBOLX4_1(op4, &bitD4);
            HUF_DECODE_SYMBOLX4_2(op1, &bitD1);
            HUF_DECODE_SYMBOLX4_2(op2, &bitD2);
            HUF_DECODE_SYMBOLX4_2(op3, &bitD3);
            HUF_DECODE_SYMBOLX4_2(op4, &bitD4);
            HUF_DECODE_SYMBOLX4_0(op1, &bitD1);
            HUF_DECODE_SYMBOLX4_0(op2, &bitD2);
            HUF_DECODE_SYMBOLX4_0(op3, &bitD3);
            HUF_DECODE_SYMBOLX4_0(op4, &bitD4);

            endSignal = BIT_reloadDStream(&bitD1) | BIT_reloadDStream(&bitD2) | BIT_reloadDStream(&bitD3) | BIT_reloadDStream(&bitD4);
        }

        /* check corruption */
        if(op1 > opStart2) 
            return 0;//ERROR(corruption_detected);

        if(op2 > opStart3) 
            return 0;//ERROR(corruption_detected);

        if(op3 > opStart4) 
            return 0;// ERROR(corruption_detected);

        /* note : op4 supposed already verified within main loop */

        /* finish bitStreams one by one */
        HUF_decodeStreamX4(op1, &bitD1, opStart2, dt, dtLog);
        HUF_decodeStreamX4(op2, &bitD2, opStart3, dt, dtLog);
        HUF_decodeStreamX4(op3, &bitD3, opStart4, dt, dtLog);
        HUF_decodeStreamX4(op4, &bitD4, oend, dt, dtLog);

        /* check */
        endSignal = BIT_endOfDStream(&bitD1) & BIT_endOfDStream(&bitD2) & BIT_endOfDStream(&bitD3) & BIT_endOfDStream(&bitD4);
      
        if(!endSignal)
            return 0;// ERROR(corruption_detected);

        /* decoded size */
        return dstSize;
    }
}


size_t HUF_decompress4X4(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX4(DTable, HUF_MAX_TABLELOG);
    const BYTE* ip = (const BYTE*)cSrc;

    size_t hSize = HUF_readDTableX4(DTable, cSrc, cSrcSize);
    if(HUF_isError(hSize))
        return hSize;

    if(hSize >= cSrcSize)
        return 0;// ERROR(srcSize_wrong);

    ip += hSize;
    cSrcSize -= hSize;

    return HUF_decompress4X4_usingDTable(dst, dstSize, ip, cSrcSize, DTable);
}















static size_t FSE_readNCount(short* normalizedCounter, unsigned* maxSVPtr, unsigned* tableLogPtr, const void* headerBuffer, size_t hbSize)
{
    const BYTE* const istart = (const BYTE*)headerBuffer;
    const BYTE* const iend = istart + hbSize;
    const BYTE* ip = istart;
    int nbBits;
    int remaining;
    int threshold;
    PXInt32U bitStream;
    int bitCount;
    unsigned charnum = 0;
    int previous0 = 0;

    if(hbSize < 4)
        return 0;//ERROR(srcSize_wrong);

    bitStream = MEM_readLE32(ip);
    nbBits = (bitStream & 0xF) + FSE_MIN_TABLELOG;   /* extract tableLog */

    if(nbBits > FSE_TABLELOG_ABSOLUTE_MAX) 
        return 0;// ERROR(tableLog_tooLarge);

    bitStream >>= 4;
    bitCount = 4;
    *tableLogPtr = nbBits;
    remaining = (1 << nbBits) + 1;
    threshold = 1 << nbBits;
    nbBits++;

    while((remaining > 1) && (charnum <= *maxSVPtr))
    {
        if(previous0)
        {
            unsigned n0 = charnum;
            while((bitStream & 0xFFFF) == 0xFFFF)
            {
                n0 += 24;
                if(ip < iend - 5)
                {
                    ip += 2;
                    bitStream = MEM_readLE32(ip) >> bitCount;
                }
                else
                {
                    bitStream >>= 16;
                    bitCount += 16;
                }
            }
            while((bitStream & 3) == 3)
            {
                n0 += 3;
                bitStream >>= 2;
                bitCount += 2;
            }
            n0 += bitStream & 3;
            bitCount += 2;

            if(n0 > *maxSVPtr) 
                return 0;//ERROR(maxSymbolValue_tooSmall);

            while(charnum < n0) normalizedCounter[charnum++] = 0;
            if((ip <= iend - 7) || (ip + (bitCount >> 3) <= iend - 4))
            {
                ip += bitCount >> 3;
                bitCount &= 7;
                bitStream = MEM_readLE32(ip) >> bitCount;
            }
            else
                bitStream >>= 2;
        }
        {
            const short max = (short)((2 * threshold - 1) - remaining);
            short count;

            if((bitStream & (threshold - 1)) < (PXInt32U)max)
            {
                count = (short)(bitStream & (threshold - 1));
                bitCount += nbBits - 1;
            }
            else
            {
                count = (short)(bitStream & (2 * threshold - 1));
                if(count >= threshold) count -= max;
                bitCount += nbBits;
            }

            count--;   /* extra accuracy */
            remaining -= FSE_abs(count);
            normalizedCounter[charnum++] = count;
            previous0 = !count;
            while(remaining < threshold)
            {
                nbBits--;
                threshold >>= 1;
            }

            {
                if((ip <= iend - 7) || (ip + (bitCount >> 3) <= iend - 4))
                {
                    ip += bitCount >> 3;
                    bitCount &= 7;
                }
                else
                {
                    bitCount -= (int)(8 * (iend - 4 - ip));
                    ip = iend - 4;
                }
                bitStream = MEM_readLE32(ip) >> (bitCount & 31);
            }
        }
    }
    if(remaining != 1) 
        return 0;// ERROR(GENERIC);
    *maxSVPtr = charnum - 1;

    ip += (bitCount + 7) >> 3;
   
    if((size_t)(ip - istart) > hbSize)
        return 0;//ERROR(srcSize_wrong);

    return ip - istart;
}



static size_t FSE_buildDTable_raw(FSE_DTable* dt, unsigned nbBits)
{
    void* ptr = dt;
    FSE_DTableHeader* const DTableH = (FSE_DTableHeader*)ptr;
    FSE_decode_t* const dinfo = (FSE_decode_t*)(ptr)+1;
    const unsigned tableSize = 1 << nbBits;
    const unsigned tableMask = tableSize - 1;
    const unsigned maxSymbolValue = tableMask;
    unsigned s;

    /* Sanity checks */
    if(nbBits < 1)
        return 0;// ERROR(GENERIC);         /* min size */

    /* Build Decoding Table */
    DTableH->tableLog = (PXInt16U)nbBits;
    DTableH->fastMode = 1;

    for(s = 0; s <= maxSymbolValue; s++)
    {
        dinfo[s].newState = 0;
        dinfo[s].symbol = (BYTE)s;
        dinfo[s].nbBits = (BYTE)nbBits;
    }

    return 0;
}

size_t FSE_buildDTable_rle(FSE_DTable* dt, BYTE symbolValue)
{
    void* ptr = dt;
    FSE_DTableHeader* const DTableH = (FSE_DTableHeader*)ptr;
    FSE_decode_t* const cell = (FSE_decode_t*)(ptr)+1;

    DTableH->tableLog = 0;
    DTableH->fastMode = 0;

    cell->newState = 0;
    cell->symbol = symbolValue;
    cell->nbBits = 0;

    return 0;
}








PXInt64U MEM_readLE64(const void* memPtr)
{
    if(MEM_isLittleEndian())
        return MEM_read64(memPtr);
    else
    {
        const BYTE* p = (const BYTE*)memPtr;
        return (PXInt64U)((PXInt64U)p[0] + ((PXInt64U)p[1] << 8) + ((PXInt64U)p[2] << 16) + ((PXInt64U)p[3] << 24)
                     + ((PXInt64U)p[4] << 32) + ((PXInt64U)p[5] << 40) + ((PXInt64U)p[6] << 48) + ((PXInt64U)p[7] << 56));
    }
}










unsigned MEM_32bits(void) { return sizeof(void*) == 4; }
unsigned MEM_64bits(void) { return sizeof(void*) == 8; }

unsigned MEM_isLittleEndian(void)
{
    const union { PXInt32U u; BYTE c[4]; } one = { 1 };   /* don't use static : performance detrimental  */
    return one.c[0];
}

PXInt16U MEM_read16(const void* memPtr)
{
    PXInt16U val; memcpy(&val, memPtr, sizeof(val)); return val;
}

PXInt32U MEM_read32(const void* memPtr)
{
    PXInt32U val; memcpy(&val, memPtr, sizeof(val)); return val;
}

PXInt64U MEM_read64(const void* memPtr)
{
    PXInt64U val; memcpy(&val, memPtr, sizeof(val)); return val;
}


PXInt16U MEM_readLE16(const void* memPtr)
{
    if(MEM_isLittleEndian())
        return MEM_read16(memPtr);
    else
    {
        const BYTE* p = (const BYTE*)memPtr;
        return (PXInt16U)(p[0] + (p[1] << 8));
    }
}

PXInt32U MEM_readLE32(const void* memPtr)
{
    if(MEM_isLittleEndian())
        return MEM_read32(memPtr);
    else
    {
        const BYTE* p = (const BYTE*)memPtr;
        return (PXInt32U)((PXInt32U)p[0] + ((PXInt32U)p[1] << 8) + ((PXInt32U)p[2] << 16) + ((PXInt32U)p[3] << 24));
    }
}

size_t MEM_readLEST(const void* memPtr)
{
    if(MEM_32bits())
        return (size_t)MEM_readLE32(memPtr);
    else
        return (size_t)MEM_readLE64(memPtr);
}

#define COPY8(d,s) { ZSTD_copy8(d,s); d+=8; s+=8; }


/*! ZSTD_wildcopy : custom version of memcpy(), can copy up to 7-8 bytes too many */
static void ZSTD_wildcopy(void* dst, const void* src, ptrdiff_t length)
{
    const BYTE* ip = (const BYTE*)src;
    BYTE* op = (BYTE*)dst;
    BYTE* const oend = op + length;
    do COPY8(op, ip) while(op < oend);
}


PXInt32U MEM_readLE24(const void* memPtr)
{
    return MEM_readLE16(memPtr) + (((const BYTE*)memPtr)[2] << 16);
}



static void ZSTD_decodeSequence(seq_t* seq, seqState_t* seqState)
{
    size_t litLength;
    size_t prevOffset;
    size_t offset;
    size_t matchLength;
    const BYTE* dumps = seqState->dumps;
    const BYTE* const de = seqState->dumpsEnd;

    /* Literal length */
    litLength = FSE_decodeSymbol(&(seqState->stateLL), &(seqState->DStream));
    prevOffset = litLength ? seq->offset : seqState->prevOffset;
    seqState->prevOffset = seq->offset;
    if(litLength == MaxLL)
    {
        const PXInt32U add = dumps < de ? *dumps++ : 0;
        if(add < 255) litLength += add;
        else if(dumps + 3 <= de)
        {
            litLength = MEM_readLE24(dumps);
            dumps += 3;
        }
        if(dumps >= de) dumps = de - 1;   /* late correction, to avoid read overflow (data is now corrupted anyway) */
    }

    /* Offset */
    {
        static const size_t offsetPrefix[MaxOff + 1] = {  /* note : size_t faster than U32 */
                1 /*fake*/, 1, 2, 4, 8, 16, 32, 64, 128, 256,
                512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144,
                524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, /*fake*/ 1, 1, 1, 1, 1 };
        PXInt32U offsetCode, nbBits;
        offsetCode = FSE_decodeSymbol(&(seqState->stateOffb), &(seqState->DStream));   /* <= maxOff, by table construction */
        if(MEM_32bits()) BIT_reloadDStream(&(seqState->DStream));
        nbBits = offsetCode - 1;
        if(offsetCode == 0) nbBits = 0;   /* cmove */
        offset = offsetPrefix[offsetCode] + BIT_readBits(&(seqState->DStream), nbBits);
        if(MEM_32bits()) BIT_reloadDStream(&(seqState->DStream));
        if(offsetCode == 0) offset = prevOffset;   /* cmove */
    }

    /* MatchLength */
    matchLength = FSE_decodeSymbol(&(seqState->stateML), &(seqState->DStream));
    if(matchLength == MaxML)
    {
        const PXInt32U add = dumps < de ? *dumps++ : 0;
        if(add < 255) matchLength += add;
        else if(dumps + 3 <= de)
        {
            matchLength = MEM_readLE24(dumps);
            dumps += 3;
        }
        if(dumps >= de) dumps = de - 1;   /* late correction, to avoid read overflow (data is now corrupted anyway) */
    }
    matchLength += MINMATCH;

    /* save result */
    seq->litLength = litLength;
    seq->offset = offset;
    seq->matchLength = matchLength;
    seqState->dumps = dumps;
}



/** ZSTD_decompressLiterals
    @return : nb of bytes read from src, or an error code*/
size_t ZSTD_decompressLiterals(void* dst, size_t* maxDstSizePtr, const void* src, size_t srcSize)
{
    const BYTE* ip = (const BYTE*)src;

    const size_t litSize = (MEM_readLE32(src) & 0x1FFFFF) >> 2;   /* no buffer issue : srcSize >= MIN_CBLOCK_SIZE */
    const size_t litCSize = (MEM_readLE32(ip + 2) & 0xFFFFFF) >> 5;   /* no buffer issue : srcSize >= MIN_CBLOCK_SIZE */

    if(litSize > *maxDstSizePtr)
        return 0;// ERROR(corruption_detected);

    if(litCSize + 5 > srcSize) 
        return 0;//ERROR(corruption_detected);

    if(HUF_isError(HUF_decompress(dst, litSize, ip + 5, litCSize)))
        return 0;// ERROR(corruption_detected);

    *maxDstSizePtr = litSize;
    return litCSize + 5;
}





void   ZSTD_copy4(void* dst, const void* src) 
{ memcpy(dst, src, 4); }

void   ZSTD_copy8(void* dst, const void* src) 
{ memcpy(dst, src, 8); }




static size_t ZSTD_execSequence
(
    BYTE* op,
    seq_t sequence,
    const BYTE** litPtr,
    const BYTE* const litLimit,
    BYTE* const base,
    BYTE* const oend
)
{
    static const int dec32table[] = { 0, 1, 2, 1, 4, 4, 4, 4 };   /* added */
    static const int dec64table[] = { 8, 8, 8, 7, 8, 9,10,11 };   /* subtracted */
    const BYTE* const ostart = op;
    BYTE* const oLitEnd = op + sequence.litLength;
    BYTE* const oMatchEnd = op + sequence.litLength + sequence.matchLength;   /* risk : address space overflow (32-bits) */
    BYTE* const oend_8 = oend - 8;
    const BYTE* const litEnd = *litPtr + sequence.litLength;

    /* checks */
    size_t const seqLength = sequence.litLength + sequence.matchLength;

    if(seqLength > (size_t)(oend - op))
        return 0;// ERROR(dstSize_tooSmall);

    if(sequence.litLength > (size_t)(litLimit - *litPtr))
        return 0;//ERROR(corruption_detected);

    /* Now we know there are no overflow in literal nor match lengths, can use pointer checks */
    if(oLitEnd > oend_8)
        return 0;//ERROR(dstSize_tooSmall);

    if(sequence.offset > (PXInt32U)(oLitEnd - base)) 
        return 0;//ERROR(corruption_detected);

    if(oMatchEnd > oend)
        return 0;//ERROR(dstSize_tooSmall);   /* overwrite beyond dst buffer */

    if(litEnd > litLimit) 
        return 0;//ERROR(corruption_detected);   /* overRead beyond lit buffer */

    /* copy Literals */
    ZSTD_wildcopy(op, *litPtr, (ptrdiff_t)sequence.litLength);   /* note : oLitEnd <= oend-8 : no risk of overwrite beyond oend */
    op = oLitEnd;
    *litPtr = litEnd;   /* update for next sequence */

    /* copy Match */
    {
        const BYTE* match = op - sequence.offset;

        /* check */
        if(sequence.offset > (size_t)op)
            return 0;//ERROR(corruption_detected);   /* address space overflow test (this test seems kept by clang optimizer) */

        //if (match > op) return ERROR(corruption_detected);   /* address space overflow test (is clang optimizer removing this test ?) */
        if(match < base) 
            return 0;//ERROR(corruption_detected);

        /* close range match, overlap */
        if(sequence.offset < 8)
        {
            const int dec64 = dec64table[sequence.offset];
            op[0] = match[0];
            op[1] = match[1];
            op[2] = match[2];
            op[3] = match[3];
            match += dec32table[sequence.offset];
            ZSTD_copy4(op + 4, match);
            match -= dec64;
        }
        else
        {
            ZSTD_copy8(op, match);
        }
        op += 8; match += 8;

        if(oMatchEnd > oend - (16 - MINMATCH))
        {
            if(op < oend_8)
            {
                ZSTD_wildcopy(op, match, oend_8 - op);
                match += oend_8 - op;
                op = oend_8;
            }
            while(op < oMatchEnd) *op++ = *match++;
        }
        else
        {
            ZSTD_wildcopy(op, match, (ptrdiff_t)sequence.matchLength - 8);   /* works even if matchLength < 8 */
        }
    }

    return oMatchEnd - ostart;
}


size_t BIT_lookBits(BIT_DStream_t* bitD, PXInt32U nbBits)
{
    const PXInt32U bitMask = sizeof(bitD->bitContainer) * 8 - 1;
    return ((bitD->bitContainer << (bitD->bitsConsumed & bitMask)) >> 1) >> ((bitMask - nbBits) & bitMask);
}




BIT_DStream_status BIT_reloadDStream(BIT_DStream_t* bitD)
{
    if(bitD->bitsConsumed > (sizeof(bitD->bitContainer) * 8))  /* should never happen */
        return BIT_DStream_overflow;

    if(bitD->ptr >= bitD->start + sizeof(bitD->bitContainer))
    {
        bitD->ptr -= bitD->bitsConsumed >> 3;
        bitD->bitsConsumed &= 7;
        bitD->bitContainer = MEM_readLEST(bitD->ptr);
        return BIT_DStream_unfinished;
    }
    if(bitD->ptr == bitD->start)
    {
        if(bitD->bitsConsumed < sizeof(bitD->bitContainer) * 8) return BIT_DStream_endOfBuffer;
        return BIT_DStream_completed;
    }
    {
        PXInt32U nbBytes = bitD->bitsConsumed >> 3;
        BIT_DStream_status result = BIT_DStream_unfinished;
        if(bitD->ptr - nbBytes < bitD->start)
        {
            nbBytes = (PXInt32U)(bitD->ptr - bitD->start);  /* ptr > start */
            result = BIT_DStream_endOfBuffer;
        }
        bitD->ptr -= nbBytes;
        bitD->bitsConsumed -= nbBytes * 8;
        bitD->bitContainer = MEM_readLEST(bitD->ptr);   /* reminder : srcSize > sizeof(bitD) */
        return result;
    }
}



unsigned BIT_highbit32(PXInt32U val)
{
#   if defined(_MSC_VER)   /* Visual */
    unsigned long r;
    return _BitScanReverse(&r, val) ? (unsigned)r : 0;
#   elif defined(__GNUC__) && (__GNUC__ >= 3)   /* Use GCC Intrinsic */
    return __builtin_clz(val) ^ 31;
#   else   /* Software version */
    static const unsigned DeBruijnClz[32] = { 0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30, 8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31 };
    U32 v = val;
    unsigned r;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    r = DeBruijnClz[(U32)(v * 0x07C4ACDDU) >> 27];
    return r;
#   endif
}







/*! BIT_lookBitsFast :
*   unsafe version; only works if nbBits >= 1 */
size_t BIT_lookBitsFast(BIT_DStream_t* bitD, PXInt32U nbBits)
{
    const PXInt32U bitMask = sizeof(bitD->bitContainer) * 8 - 1;
    return (bitD->bitContainer << (bitD->bitsConsumed & bitMask)) >> (((bitMask + 1) - nbBits) & bitMask);
}


void BIT_skipBits(BIT_DStream_t* bitD, PXInt32U nbBits)
{
    bitD->bitsConsumed += nbBits;
}


/*!BIT_readBitsFast :
*  unsafe version; only works if nbBits >= 1 */
size_t BIT_readBitsFast(BIT_DStream_t* bitD, PXInt32U nbBits)
{
    size_t value = BIT_lookBitsFast(bitD, nbBits);
    BIT_skipBits(bitD, nbBits);
    return value;
}


size_t BIT_readBits(BIT_DStream_t* bitD, PXInt32U nbBits)
{
    size_t value = BIT_lookBits(bitD, nbBits);
    BIT_skipBits(bitD, nbBits);
    return value;
}


static size_t ZSTD_decodeSeqHeaders
(
    int* nbSeq,
    const BYTE** dumpsPtr,
    size_t* dumpsLengthPtr,
    FSE_DTable* DTableLL,
    FSE_DTable* DTableML,
    FSE_DTable* DTableOffb,
    const void* src,
    size_t srcSize
)
{
    const BYTE* const istart = (const BYTE* const)src;
    const BYTE* ip = istart;
    const BYTE* const iend = istart + srcSize;
    PXInt32U LLtype, Offtype, MLtype;
    PXInt32U LLlog, Offlog, MLlog;
    size_t dumpsLength;

    /* check */
    if(srcSize < 5) 
        return 0;//ERROR(srcSize_wrong);

    /* SeqHead */
    *nbSeq = MEM_readLE16(ip); ip += 2;
    LLtype = *ip >> 6;
    Offtype = (*ip >> 4) & 3;
    MLtype = (*ip >> 2) & 3;
    if(*ip & 2)
    {
        dumpsLength = ip[2];
        dumpsLength += ip[1] << 8;
        ip += 3;
    }
    else
    {
        dumpsLength = ip[1];
        dumpsLength += (ip[0] & 1) << 8;
        ip += 2;
    }
    *dumpsPtr = ip;
    ip += dumpsLength;
    *dumpsLengthPtr = dumpsLength;

    /* check */
    if(ip > iend - 3)
        return 0;//ERROR(srcSize_wrong); /* min : all 3 are "raw", hence no header, but at least xxLog bits per type */

    /* sequences */
    {
        PXInt16S norm[MaxML + 1];    /* assumption : MaxML >= MaxLL and MaxOff */
        size_t headerSize;

        /* Build DTables */
        switch(LLtype)
        {
            case bt_rle:
                LLlog = 0;
                FSE_buildDTable_rle(DTableLL, *ip++); break;
            case bt_raw:
                LLlog = LLbits;
                FSE_buildDTable_raw(DTableLL, LLbits); break;
            default:
            {
                PXInt32U max = MaxLL;
                headerSize = FSE_readNCount(norm, &max, &LLlog, ip, iend - ip);
                
                if(FSE_isError(headerSize)) 
                    return 0;//ERROR(GENERIC);
               
                if(LLlog > LLFSELog) 
                    return 0;//ERROR(corruption_detected);

                ip += headerSize;

                FSE_buildDTable(DTableLL, norm, max, LLlog);
            }
        }

        switch(Offtype)
        {
            case bt_rle:
                Offlog = 0;
                
                if(ip > iend - 2) 
                    return 0;//ERROR(srcSize_wrong);   /* min : "raw", hence no header, but at least xxLog bits */

                FSE_buildDTable_rle(DTableOffb, *ip++ & MaxOff); /* if *ip > MaxOff, data is corrupted */
                break;
            case bt_raw:
                Offlog = Offbits;
                FSE_buildDTable_raw(DTableOffb, Offbits); break;
            default:
            {
                PXInt32U max = MaxOff;
                headerSize = FSE_readNCount(norm, &max, &Offlog, ip, iend - ip);
                
                if(FSE_isError(headerSize)) 
                    return 0;//ERROR(GENERIC);
                
                if(Offlog > OffFSELog) 
                    return 0;//ERROR(corruption_detected);

                ip += headerSize;
                FSE_buildDTable(DTableOffb, norm, max, Offlog);
            }
        }

        switch(MLtype)
        {
            case bt_rle:
                MLlog = 0;
                if(ip > iend - 2)
                    return 0;// ERROR(srcSize_wrong); /* min : "raw", hence no header, but at least xxLog bits */
                
                FSE_buildDTable_rle(DTableML, *ip++); 
                break;
            case bt_raw:
                MLlog = MLbits;
                FSE_buildDTable_raw(DTableML, MLbits); 
                break;
            default:
            {
                PXInt32U max = MaxML;
                headerSize = FSE_readNCount(norm, &max, &MLlog, ip, iend - ip);

                if(FSE_isError(headerSize)) 
                    return 0;//ERROR(GENERIC);

                if(MLlog > MLFSELog) 
                    return 0;//ERROR(corruption_detected);

                ip += headerSize;
                FSE_buildDTable(DTableML, norm, max, MLlog);
            }
        }
    }

    return ip - istart;
}

unsigned BIT_endOfDStream(const BIT_DStream_t* DStream)
{
    return ((DStream->ptr == DStream->start) && (DStream->bitsConsumed == sizeof(DStream->bitContainer) * 8));
}

void FSE_initDState(FSE_DState_t* DStatePtr, BIT_DStream_t* bitD, const FSE_DTable* dt)
{
    FSE_DTableHeader DTableH;
    memcpy(&DTableH, dt, sizeof(DTableH));
    DStatePtr->state = BIT_readBits(bitD, DTableH.tableLog);
    BIT_reloadDStream(bitD);
    DStatePtr->table = dt + 1;
}




size_t BIT_initDStream(BIT_DStream_t* bitD, const void* srcBuffer, size_t srcSize)
{
    if(srcSize < 1) 
    {
        memset(bitD, 0, sizeof(*bitD));
        return 0;//ERROR(srcSize_wrong); 
    }

    if(srcSize >= sizeof(size_t))   /* normal case */
    {
        PXInt32U contain32;
        bitD->start = (const char*)srcBuffer;
        bitD->ptr = (const char*)srcBuffer + srcSize - sizeof(size_t);
        bitD->bitContainer = MEM_readLEST(bitD->ptr);
        contain32 = ((const BYTE*)srcBuffer)[srcSize - 1];
        if(contain32 == 0) return 0;// ERROR(GENERIC);   /* endMark not present */
        bitD->bitsConsumed = 8 - BIT_highbit32(contain32);
    }
    else
    {
        PXInt32U contain32;
        bitD->start = (const char*)srcBuffer;
        bitD->ptr = bitD->start;
        bitD->bitContainer = *(const BYTE*)(bitD->start);
        switch(srcSize)
        {
            case 7: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[6]) << (sizeof(size_t) * 8 - 16);
                /* fallthrough */
            case 6: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[5]) << (sizeof(size_t) * 8 - 24);
                /* fallthrough */
            case 5: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[4]) << (sizeof(size_t) * 8 - 32);
                /* fallthrough */
            case 4: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[3]) << 24;
                /* fallthrough */
            case 3: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[2]) << 16;
                /* fallthrough */
            case 2: bitD->bitContainer += (size_t)(((const BYTE*)(bitD->start))[1]) << 8;
                /* fallthrough */
            default:;
        }
        contain32 = ((const BYTE*)srcBuffer)[srcSize - 1];
      
        if(contain32 == 0)
            return 0;// ERROR(GENERIC);   /* endMark not present */

        bitD->bitsConsumed = 8 - BIT_highbit32(contain32);
        bitD->bitsConsumed += (PXInt32U)(sizeof(size_t) - srcSize) * 8;
    }

    return srcSize;
}







static size_t ZSTD_decompressSequences
(
    void* ctx,
    void* dst, 
    size_t maxDstSize,
    const void* seqStart, 
    size_t seqSize
)
{
    ZSTD_DCtx* dctx = (ZSTD_DCtx*)ctx;
    const BYTE* ip = (const BYTE*)seqStart;
    const BYTE* const iend = ip + seqSize;
    BYTE* const ostart = (BYTE* const)dst;
    BYTE* op = ostart;
    BYTE* const oend = ostart + maxDstSize;
    size_t errorCode, dumpsLength;
    const BYTE* litPtr = dctx->litPtr;
    const BYTE* const litEnd = litPtr + dctx->litSize;
    int nbSeq;
    const BYTE* dumps;
    PXInt32U* DTableLL = dctx->LLTable;
    PXInt32U* DTableML = dctx->MLTable;
    PXInt32U* DTableOffb = dctx->OffTable;
    BYTE* const base = (BYTE*)(dctx->base);

    /* Build Decoding Tables */
    errorCode = ZSTD_decodeSeqHeaders
    (
        &nbSeq,
        &dumps,
        &dumpsLength,
        DTableLL,
        DTableML, 
        DTableOffb,
        ip,
        iend - ip
    );


    if(ZSTD_isError(errorCode)) 
        return errorCode;

    ip += errorCode;

    /* Regen sequences */
    {
        seq_t sequence;
        seqState_t seqState;

        memset(&sequence, 0, sizeof(sequence));
        seqState.dumps = dumps;
        seqState.dumpsEnd = dumps + dumpsLength;
        seqState.prevOffset = sequence.offset = 4;
        errorCode = BIT_initDStream(&(seqState.DStream), ip, iend - ip);
       
        if(ERR_isError(errorCode))
            return 0;// ERROR(corruption_detected);
       
        FSE_initDState(&(seqState.stateLL), &(seqState.DStream), DTableLL);
        FSE_initDState(&(seqState.stateOffb), &(seqState.DStream), DTableOffb);
        FSE_initDState(&(seqState.stateML), &(seqState.DStream), DTableML);

        for(; (BIT_reloadDStream(&(seqState.DStream)) <= BIT_DStream_completed) && (nbSeq > 0); )
        {
            size_t oneSeqSize;
            nbSeq--;
            ZSTD_decodeSequence(&sequence, &seqState);
            oneSeqSize = ZSTD_execSequence(op, sequence, &litPtr, litEnd, base, oend);
            if(ZSTD_isError(oneSeqSize)) return oneSeqSize;
            op += oneSeqSize;
        }

        /* check if reached exact end */
        if(!BIT_endOfDStream(&(seqState.DStream))) 
            return 0;//ERROR(corruption_detected);   /* requested too much : data is corrupted */
       
        if(nbSeq < 0) 
            return 0;//ERROR(corruption_detected);   /* requested too many sequences : data is corrupted */

        /* last literal segment */
        {
            size_t lastLLSize = litEnd - litPtr;

            if(litPtr > litEnd)
                return 0;//ERROR(corruption_detected);
           
            if(op + lastLLSize > oend)
                return 0;//ERROR(dstSize_tooSmall);

            if(lastLLSize > 0) 
            {
                if(op != litPtr) 
                    memmove(op, litPtr, lastLLSize);

                op += lastLLSize;
            }
        }
    }

    return op - ostart;
}




static size_t ZSTD_decompressBlock
(
    void* ctx,
    void* dst, size_t maxDstSize,
    const void* src, size_t srcSize
)
{
    /* blockType == blockCompressed */
    const BYTE* ip = (const BYTE*)src;

    /* Decode literals sub-block */
    size_t litCSize = ZSTD_decodeLiteralsBlock(ctx, src, srcSize);

   
    if(ZSTD_isError(litCSize))
        return litCSize;

    ip += litCSize;
    srcSize -= litCSize;

    return ZSTD_decompressSequences(ctx, dst, maxDstSize, ip, srcSize);
}


static size_t ZSTD_copyUncompressedBlock(void* dst, size_t maxDstSize, const void* src, size_t srcSize)
{
    if(srcSize > maxDstSize)
        return 0;// ERROR(dstSize_tooSmall);

    if(srcSize > 0) 
    {
        memcpy(dst, src, srcSize);
    }

    return srcSize;
}



PXActionResult PXAPI PXZSTDDecompress(PXFile* const pxFileInput, PXFile* const pxFileOutput)
{
    ZSTD_DCtx ctx;
    ctx.base = pxFileOutput->Data;

    const BYTE* ip = (const BYTE*)pxFileInput->Data;
    const BYTE* iend = ip + pxFileInput->DataUsed;
    BYTE* const ostart = (BYTE* const)pxFileOutput->Data;
    BYTE* op = ostart;
    BYTE* const oend = ostart + pxFileOutput->DataUsed;
    size_t remainingSize = pxFileInput->DataUsed;
    PXInt32U magicNumber;
    blockProperties_t blockProperties;

    /* Frame Header */
    if(pxFileInput->DataUsed < ZSTD_frameHeaderSize + ZSTD_blockHeaderSize)
        return 0;// ERROR(srcSize_wrong);

    PXFileReadI32U(pxFileInput, &magicNumber);  

    if(magicNumber != ZSTD_magicNumber)
        return 0;// ERROR(prefix_unknown);

    ip += ZSTD_frameHeaderSize; remainingSize -= ZSTD_frameHeaderSize;

    // Loop on each block
    for(;;)
    {
        size_t decodedSize = 0;
        size_t cBlockSize = ZSTD_getcBlockSize(ip, iend - ip, &blockProperties);
       
        if(ZSTD_isError(cBlockSize))
            return cBlockSize;

        ip += ZSTD_blockHeaderSize;
        remainingSize -= ZSTD_blockHeaderSize;

        if(cBlockSize > remainingSize)
            return 0;// ERROR(srcSize_wrong);

        switch(blockProperties.blockType)
        {
            case bt_compressed:
                decodedSize = ZSTD_decompressBlock(&ctx, op, oend - op, ip, cBlockSize);
                break;
            case bt_raw:
                decodedSize = ZSTD_copyUncompressedBlock(op, oend - op, ip, cBlockSize);
                break;
            case bt_rle:
                return 0;//ERROR(GENERIC);   /* not yet supported */
                break;
            case bt_end:
                /* end of frame */
                if(remainingSize) 
                    return 0;//ERROR(srcSize_wrong);
                break;
            default:
                return 0;//ERROR(GENERIC);   /* impossible */
        }

        if(cBlockSize == 0)
            break;   /* bt_end */

        if(ZSTD_isError(decodedSize)) 
            return decodedSize;

        op += decodedSize;
        ip += cBlockSize;
        remainingSize -= cBlockSize;
    }

   // return op - ostart;


    return PXActionSuccessful;
}