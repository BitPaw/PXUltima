#include "PXLZ4.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

const char PXLU4Name[] = "LZ4";

const PXI32U LZ4_MAX_INPUT_SIZE = 0x7E000000; // 2 113 929 216 bytes
const PXI8U PXLZ4ChunksMaxAmount = 127;
const PXI8U PXLZ4TokenLengthMAX = 0x0F;


#define ML_BITS  4
#define ML_MASK  ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)

#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)

typedef enum { decode_full_block = 0, partial_decode = 1 } earlyEnd_directive;
typedef enum { noDict = 0, withPrefix64k, usingExtDict, usingDictCtx } dict_directive;
typedef enum { noDictIssue = 0, dictSmall } dictIssue_directive;

PXResult PXAPI PXLZ4DecompressChunk(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput)
{


    earlyEnd_directive partialDecoding;  /* full, partial */
    dict_directive dict = noDict;                 /* noDict, withPrefix64k, usingExtDict */
    const BYTE PXREF lowPrefix = 0;  /* always <= dst, == dst when no prefix */
    const BYTE PXREF dictStart = 0;  /* only if dict==usingExtDict */
    const size_t dictSize = 0;       /* note : = 0 if noDict */


    // if((src == NULL) || (outputSize < 0)) { return -1; }


    const BYTE PXREF dictEnd = (dictStart == NULL) ? NULL : dictStart + dictSize;
    const int checkOffset = (dictSize < (int)(64 KB));


    /* Set up the "end" pointers for the shortcut. */
   // const BYTE PXREF shortiend = iend - 14 /*maxLL*/ - 2 /*offset*/;
   // const BYTE PXREF shortoend = oend - 14 /*maxLL*/ - 18 /*maxML*/;

    const BYTE* match;



    // Main Loop : decode remaining sequences where output < FASTLOOP_SAFE_DISTANCE
    for(;;)
    {
        if(PXFileIsAtEnd(pxFileInput))
        {
            break;
        }

        PXI16U literalLength = 0;
        PXI16U matchlength = 0;

        {
            PXI8U token = 0;

            // First byte / two nibbles
            // HI-Nibble is length
            // Low-Nibble is token
            PXFileReadI8U(pxFileInput, &token);

            matchlength = token & 0x0F;
            literalLength = token >> 4u;
        }


        // If the length is at a maximum, read next byte to add ontop.
        if(PXLZ4TokenLengthMAX == literalLength)
        {
            for(;;)
            {
                PXI8U size = 0;

                PXFileReadI8U(pxFileInput, &size);

                literalLength += size;

                if(0xFF == size) // if the size of 255, we repreat
                {
                    continue;
                }

                break;
            }
        }

        // Copy literals
        PXFileDataCopy(pxFileInput, pxFileOutput, literalLength);


        // Match copy
        PXI16S offset = 0;

        PXFileReadI16SE(pxFileInput, &offset, PXEndianLittle);

        if(PXLZ4TokenLengthMAX == matchlength)
        {
            matchlength += 4;

            for(;;)
            {
                PXI8U size = 0;

                PXFileReadI8U(pxFileInput, &size);

                matchlength += size;

                if(0xFF == size) // if the size of 255, we repreat
                {
                    continue;
                }

                break;
            }         
        }

        PXFileWriteInternMove(pxFileOutput, offset, matchlength);


      



#if 0
       // if((cpy > oend - MFLIMIT) || (ip + length > iend - (2 + 1 + LASTLITERALS)))
        {
            // We've either hit the input parsing restriction or the output parsing restriction.
            // In the normal scenario, decoding a full block, it must be the last sequence,
            // otherwise it's an error (invalid input or dimensions).
            // In partialDecoding scenario, it's necessary to ensure there is no buffer overflow.

            if(partialDecoding)
            {
                /* Since we are partial decoding we may be in this block because of the output parsing
                 * restriction, which is not valid since the output buffer is allowed to be undersized.
                 */
                 //DEBUGLOG(7, "partialDecoding: copying literals, close to input or output end")
                //DEBUGLOG(7, "partialDecoding: literal length = %u", (unsigned)length);
                //DEBUGLOG(7, "partialDecoding: remaining space in dstBuffer : %i", (int)(oend - op));
                //DEBUGLOG(7, "partialDecoding: remaining space in srcBuffer : %i", (int)(iend - ip));

                // Finishing in the middle of a literals segment, due to lack of input.

                if(ip + length > iend)
                {
                    length = (size_t)(iend - ip);
                    cpy = op + length;
                }

                // Finishing in the middle of a literals segment, due to lack of output space.

                if(cpy > oend) {
                    cpy = oend;
                    assert(op <= oend);
                    length = (size_t)(oend - op);
                }
            }
            else
            {
                /* We must be on the last sequence (or invalid) because of the parsing limitations
                 * so check that we exactly consume the input and don't overrun the output buffer.
                 */
                if((ip + length != iend) || (cpy > oend)) {
                    DEBUGLOG(6, "should have been last run of literals")
                        DEBUGLOG(6, "ip(%p) + length(%i) = %p != iend (%p)", ip, (int)length, ip + length, iend);
                    DEBUGLOG(6, "or cpy(%p) > oend(%p)", cpy, oend);
                    goto _output_error;
                }
            }
            LZ4_memmove(op, ip, length);  /* supports overlapping memory regions, for in-place decompression scenarios */
            ip += length;
            op += length;
            /* Necessarily EOF when !partialDecoding.
             * When partialDecoding, it is EOF if we've either
             * filled the output buffer or
             * can't proceed with reading an offset for following match.
             */
            if(!partialDecoding || (cpy == oend) || (ip >= (iend - 2))) {
                break;
            }
        }
        else {
            LZ4_wildCopy8(op, ip, cpy);   /* can overwrite up to 8 bytes beyond cpy */
            ip += length; op = cpy;
        }

        /* get offset */
        offset = LZ4_readLE16(ip); ip += 2;
        match = op - offset;

        /* get matchlength */
        length = token & ML_MASK;

    _copy_match:
        if(length == ML_MASK) {
            size_t const addl = read_variable_length(&ip, iend - LASTLITERALS + 1, 0);
            if(addl == rvl_error) { goto _output_error; }
            length += addl;
            if(unlikely((uptrval)(op)+length < (uptrval)op)) goto _output_error;   /* overflow detection */
        }
        length += MINMATCH;

#if LZ4_FAST_DEC_LOOP
        safe_match_copy :
#endif
        if((checkOffset) && (unlikely(match + dictSize < lowPrefix))) goto _output_error;   /* Error : offset outside buffers */
        /* match starting within external dictionary */
        if((dict == usingExtDict) && (match < lowPrefix)) {
            assert(dictEnd != NULL);
            if(unlikely(op + length > oend - LASTLITERALS)) {
                if(partialDecoding) length = MIN(length, (size_t)(oend - op));
                else goto _output_error;   /* doesn't respect parsing restriction */
            }

            if(length <= (size_t)(lowPrefix - match)) {
                /* match fits entirely within external dictionary : just copy */
                LZ4_memmove(op, dictEnd - (lowPrefix - match), length);
                op += length;
            }
            else {
                /* match stretches into both external dictionary and current block */
                size_t const copySize = (size_t)(lowPrefix - match);
                size_t const restSize = length - copySize;
                LZ4_memcpy(op, dictEnd - copySize, copySize);
                op += copySize;
                if(restSize > (size_t)(op - lowPrefix)) {  /* overlap copy */
                    BYTE PXREF endOfMatch = op + restSize;
                    const BYTE* copyFrom = lowPrefix;
                    while(op < endOfMatch) *op++ = *copyFrom++;
                }
                else {
                    LZ4_memcpy(op, lowPrefix, restSize);
                    op += restSize;
                }
            }
            continue;
        }
        assert(match >= lowPrefix);

        /* copy match within block */
        cpy = op + length;

        /* partialDecoding : may end anywhere within the block */
        assert(op <= oend);
        if(partialDecoding && (cpy > oend - MATCH_SAFEGUARD_DISTANCE)) {
            size_t const mlen = MIN(length, (size_t)(oend - op));
            const BYTE PXREF matchEnd = match + mlen;
            BYTE PXREF copyEnd = op + mlen;
            if(matchEnd > op) {   /* overlap copy */
                while(op < copyEnd) { *op++ = *match++; }
            }
            else {
                LZ4_memcpy(op, match, mlen);
            }
            op = copyEnd;
            if(op == oend) { break; }
            continue;
        }

        if(unlikely(offset < 8)) {
            LZ4_write32(op, 0);   /* silence msan warning when offset==0 */
            op[0] = match[0];
            op[1] = match[1];
            op[2] = match[2];
            op[3] = match[3];
            match += inc32table[offset];
            LZ4_memcpy(op + 4, match, 4);
            match -= dec64table[offset];
        }
        else {
            LZ4_memcpy(op, match, 8);
            match += 8;
        }
        op += 8;

        if(unlikely(cpy > oend - MATCH_SAFEGUARD_DISTANCE))
        {
            BYTE PXREF oCopyLimit = oend - (WILDCOPYLENGTH - 1);

            if(cpy > oend - LASTLITERALS) { goto _output_error; } /* Error : last LASTLITERALS bytes must be literals (uncompressed) */

            if(op < oCopyLimit) {
                LZ4_wildCopy8(op, match, oCopyLimit);
                match += oCopyLimit - op;
                op = oCopyLimit;
            }
            while(op < cpy) { *op++ = *match++; }
        }
        else
        {
            LZ4_memcpy(op, match, 8);
            if(length > 16) { LZ4_wildCopy8(op + 8, match + 8, cpy); }
        }
        op = cpy;   /* wildcopy correction */

#endif
    }

    return PXResultOK;
}

PXResult PXAPI PXLZ4Compress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXLZ4Decompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput)
{
    PXI8U nChunks = 0;

    if(!(pxFileInput && pxFileOutput))
    {
        return PXResultRefusedParameterNull;
    }

    // Check size
    if(PXFileDataPosition(pxFileInput) <= 1)
    {
        return PXResultRefusedParameterInvalid;
    }

    PXFileReadI8U(pxFileInput, &nChunks);

    if(PXLZ4ChunksMaxAmount <= nChunks)
    {
        return PXResultInvalid; // Too many chunks
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXLU4Name,
        "Decompress",
        "Info\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i",
        "NumberOfChunks", nChunks,
        "SizeCompressed", PXFileDataPosition(pxFileInput),
        "SizeUncompressed", PXFileDataPosition(pxFileOutput)
    );
#endif

    size_t consumedCompressedSize = 1;

    if(PXFileAllocatedSize(pxFileOutput) < LZ4_MAX_INPUT_SIZE)
    {
        // nChunks must be 0 for < LZ4_MAX_INPUT_SIZE
        if(nChunks != 0) 
        {
            return PXResultInvalid; // Corrupted LZ4 compressed data.
        }
    }

    // If true, we can directly decompress one block.
    if(0 == nChunks) 
    {
        const PXResult nDecompressed = PXLZ4DecompressChunk(pxFileInput, pxFileOutput);

        return nDecompressed;
    }

    // We need to decompress more than one block
    for(PXI8U i = 0; i < nChunks; ++i)
    {
        PXI32U chunkSize = 0;

        PXFileReadI32UE(pxFileInput, &chunkSize, PXEndianLittle);

        if(LZ4_MAX_INPUT_SIZE < chunkSize) 
        {
            return PXResultInvalid; // ChunkSize too big
        }

        const PXResult decompessionResult = PXLZ4DecompressChunk(pxFileInput, pxFileOutput);

        if(PXResultOK != decompessionResult)
        {
            return decompessionResult;
        }
    }

    return PXResultOK;
}