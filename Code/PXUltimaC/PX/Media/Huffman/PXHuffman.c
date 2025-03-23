#include "PXHuffman.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/File/PXFile.h>

#include <stdlib.h>

// a symbol value too big to represent any valid symbol, to indicate reading disallowed huffman bits combination,
// which is possible in case of only 0 or 1 present symbols.
#define INVALIDSYMBOL 65535u
#define NUM_CODE_LENGTH_CODES 19
#define NUM_DEFLATE_CODE_SYMBOLS 288
#define NUM_DISTANCE_SYMBOLS 32


    // the order in which "code length alphabet code lengths" are stored as specified by deflate, out of this the huffman
    // tree of the dynamic huffman tree lengths is generated
const PXInt8U PXHuffmanCodeLengthIndex[NUM_CODE_LENGTH_CODES] =
{
    16, 17, 18, 0, 8, 7,
    9, 6, 10, 5, 11, 4, 12,
    3, 13, 2, 14, 1, 15
};


// numcodes = 288
// maxbitlen = 15
PXActionResult PXAPI PXGenerateFromLengths(PXHuffmanTree* const huffmanTree, const PXInt32U* const bitlen, const PXSize numcodes, const PXSize maxbitlen)
{
    PXClear(PXHuffmanTree, huffmanTree);

    // HuffmanTree_makeFromLengths()
    huffmanTree->LengthsList = PXMemoryHeapCallocT(PXInt32U, numcodes);
    huffmanTree->NumberOfSymbols = NUM_DEFLATE_CODE_SYMBOLS;
    huffmanTree->maxbitlen = 15;
    huffmanTree->CodeSymbols = PXMemoryHeapCallocT(PXInt32U, numcodes);

    PXCopyList(PXInt32U, numcodes, bitlen, huffmanTree->LengthsList);
    //-----------

    //------------------------------------ HuffmanTree_makeFromLengths2()
    const PXInt32U maxBitLenghAA = maxbitlen + 1;

    //PXInt32U* blcount = PXNewList(PXInt32U, maxBitLenghAA);
    //PXInt32U* nextcode = PXNewList(PXInt32U, maxBitLenghAA);

    PXInt32U blcount[16];
    PXInt32U nextcode[16];
    PXClearList(PXInt32U, blcount, 16);
    PXClearList(PXInt32U, nextcode, 16);

    unsigned error = 0;

    if(!huffmanTree->CodeSymbols || !blcount || !nextcode) error = 83; /*alloc fail*/

    if(!error)
    {
        //PXClearList(PXInt32U, blcount, maxbitlen + 1);
        //PXClearList(PXInt32U, nextcode, maxbitlen + 1);

        // step 1: count number of instances of each code length
        for(PXSize bits = 0; bits != numcodes; ++bits)
        {
            ++blcount[huffmanTree->LengthsList[bits]];
        }

        // step 2: generate the nextcode values
        for(PXSize bits = 1; bits <= maxbitlen; ++bits)
        {
            nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1u;
        }
        // step 3: generate all the codes
        for(PXSize n = 0; n != numcodes; ++n)
        {
            if(huffmanTree->LengthsList[n] != 0)
            {
                huffmanTree->CodeSymbols[n] = nextcode[huffmanTree->LengthsList[n]]++;
                // remove superfluous bits from the code
                huffmanTree->CodeSymbols[n] &= ((1u << huffmanTree->LengthsList[n]) - 1u);
            }
        }
    }

    //PXDeleteList(PXInt32U, blcount, maxBitLenghAA);
    //PXDeleteList(PXInt32U, nextcode, maxBitLenghAA);
    //-------------------------------------------------

    // HuffmanTree_makeTable()
    const PXInt16U headsize = 1u << PXHuffmanFirstBits; // size of the first table, 512
    const PXInt16U mask = (1u << PXHuffmanFirstBits) /*headsize*/ - 1u;
    PXSize pointer, size; /*total table size*/

    {
        const PXSize maxlensSize = sizeof(unsigned int) * headsize;
        //unsigned int* maxlens = PXNewList(unsigned int, headsize);

        PXInt32U maxlens[512];
        PXClearList(PXInt32U, maxlens, 512);

        //if (!maxlens) return 83; // alloc fail

        // compute maxlens: max total bit length of symbols sharing prefix in the first table

        for(PXSize i = 0; i < numcodes; i++)
        {
            const PXInt32U symbol = huffmanTree->CodeSymbols[i];
            const PXInt32U l = huffmanTree->LengthsList[i];

            if(l <= PXHuffmanFirstBits)
                continue; // symbols that fit in first table don't increase secondary table size*/

            /*get the FIRSTBITS MSBs, the MSBs of the symbol are encoded first. See later comment about the reversing*/
            const PXInt32U index = reverseBits(symbol >> (l - PXHuffmanFirstBits), PXHuffmanFirstBits);
            maxlens[index] = PXMathMaximum(maxlens[index], l);
        }
        /* compute total table size: size of first table plus all secondary tables for symbols longer than FIRSTBITS */
        size = headsize;

        for(PXSize i = 0; i < headsize; ++i)
        {
            PXSize l = maxlens[i];

            if(l > PXHuffmanFirstBits)
            {
                size += (1u << (l - PXHuffmanFirstBits));
            }
        }

        huffmanTree->TableLength = PXMemoryHeapCallocT(PXInt8U, size); // unsigned char*
        huffmanTree->TableValue = PXMemoryHeapCallocT(PXInt16U, size); // unsigned short

        if(!huffmanTree->TableLength || !huffmanTree->TableValue)
        {
            //PXDeleteList(PXInt32U, maxlens, maxlensSize);
            /* freeing tree->table values is done at a higher scope */
            return PXActionFailedMemoryAllocation; /*alloc fail*/
        }
        /*initialize with an invalid length to indicate unused entries*/
        for(PXSize i = 0; i < size; ++i)
            huffmanTree->TableLength[i] = 16;

        /*fill in the first table for long symbols: max prefix size and pointer to secondary tables*/
        pointer = headsize;
        for(PXInt16U i = 0; i < headsize; ++i)
        {
            const PXInt32U l = maxlens[i];

            if(l <= PXHuffmanFirstBits)
                continue;

            huffmanTree->TableLength[i] = l;
            huffmanTree->TableValue[i] = pointer;

            pointer += (1u << (l - PXHuffmanFirstBits));
        }

        //PXDeleteList(unsigned int, maxlens, maxlensSize);
    }

    /*fill in the first table for short symbols, or secondary table for long symbols*/
    PXSize numpresent = 0;
    for(PXSize i = 0; i < numcodes; ++i)
    {
        const PXInt32U l = huffmanTree->LengthsList[i];
        const PXInt32U symbol = huffmanTree->CodeSymbols[i]; // the huffman bit pattern. i itself is the value.
        // reverse bits, because the huffman bits are given in MSB first order but the bit reader reads LSB first
        const PXInt32U reverse = reverseBits(symbol, l);

        if(l == 0)
            continue;

        numpresent++;

        if(l <= PXHuffmanFirstBits)
        {
            // short symbol, fully in first table, replicated num times if l < FIRSTBITS
            const PXInt16U num = 1u << (PXHuffmanFirstBits - l); // Can only be 0..512 max

            for(PXInt16U j = 0; j < num; ++j)
            {
                // bit reader will read the l bits of symbol first, the remaining FIRSTBITS - l bits go to the MSB's
                PXInt32U index = reverse | (j << l);

                if(huffmanTree->TableLength[index] != 16)
                    return PXActionRefusedParserSymbolNotAsExpected; // invalid tree: long symbol shares prefix with short symbol

                huffmanTree->TableLength[index] = l;
                huffmanTree->TableValue[index] = i;
            }
        }
        else
        {
            /*long symbol, shares prefix with other long symbols in first lookup table, needs second lookup*/
            /*the FIRSTBITS MSBs of the symbol are the first table index*/
            const PXInt32U index = reverse & mask;
            const PXInt8U maxlen = huffmanTree->TableLength[index];
            /*log2 of secondary table length, should be >= l - FIRSTBITS*/
            const PXInt8U tablelen = maxlen - PXHuffmanFirstBits;
            const PXInt16U start = huffmanTree->TableValue[index]; /*starting index in secondary table*/
            const PXInt32U num = 1u << (tablelen - (l - PXHuffmanFirstBits)); /*amount of entries of this symbol in secondary table*/

            if(maxlen < l)
                return PXActionRefusedParserSymbolNotAsExpected; // invalid tree: long symbol shares prefix with short symbol

            for(PXInt32U j = 0; j < num; ++j)
            {
                const PXInt32U reverse2 = reverse >> PXHuffmanFirstBits; /* l - FIRSTBITS bits */
                const PXInt32U index2 = start + (reverse2 | (j << (l - PXHuffmanFirstBits)));
                huffmanTree->TableLength[index2] = l;
                huffmanTree->TableValue[index2] = i;
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
        for(PXSize i = 0; i < size; ++i)
        {
            if(huffmanTree->TableLength[i] == 16)
            {
                /* As length, use a value smaller than FIRSTBITS for the head table,
                and a value larger than FIRSTBITS for the secondary table, to ensure
                valid behavior for advanceBits when reading this symbol. */
                huffmanTree->TableLength[i] = (i < headsize) ? 1 : (PXHuffmanFirstBits + 1);
                huffmanTree->TableValue[i] = INVALIDSYMBOL;
            }
        }
    }
    else
    {
        // A good huffman tree has N * 2 - 1 nodes, of which N - 1 are CPrivate nodes.
        // If that is not the case (due to too long length codes), the table will not
        // have been fully used, and this is an error (not all bit combinations can be
        // decoded): an oversubscribed huffman tree, indicated by error 55.
        for(PXSize i = 0; i < size; ++i)
        {
            if(huffmanTree->TableLength[i] == 16)
                return PXActionRefusedParserSymbolNotAsExpected;
        }
    }
    //----------------------------

    return PXActionSuccessful;
}

PXActionResult PXAPI PXHuffmanDistanceTreeGenerateDynamic(PXFile* const pxFile, PXHuffmanTree* const treeLength, PXHuffmanTree* const treeDistance)
{
    PXClear(PXHuffmanTree, treeLength);

    // make sure that length values that aren't filled in will be 0, or a wrong tree will be generated
    //PXActionResult actionError = PXActionInvalid;
    unsigned n;

    /*see comments in deflateDynamic for explanation of the context and these variables, it is analogous*/
    PXInt32U bitlen_lengh[NUM_DEFLATE_CODE_SYMBOLS]; /*lit,len code lengths*/
    PXInt32U bitlen_distance[NUM_DISTANCE_SYMBOLS]; /*dist code lengths*/
    /*code length code lengths ("clcl"), the bit lengths of the huffman tree used to compress bitlen_ll and bitlen_d*/
    PXInt32U bitlen_codeLength[NUM_CODE_LENGTH_CODES];
    PXHuffmanTree tree_cl; /*the code tree for code length codes (the huffman tree for compressed huffman trees)*/

    //if (!ensureBits17(reader, 14)) return 49; //error: the bit pointer is or will go past the memory BBBB

    PXClear(PXHuffmanTree, &tree_cl);

    PXClearList(PXInt32U, bitlen_lengh, NUM_DEFLATE_CODE_SYMBOLS);
    PXClearList(PXInt32U, bitlen_distance, NUM_DISTANCE_SYMBOLS);
    PXClearList(PXInt32U, bitlen_codeLength, NUM_CODE_LENGTH_CODES);


    PXHuffmanNumberCode huffmanNumberCode;

    // Read huffmancodes
    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&huffmanNumberCode.NumberOfLiteralCodes, PXTypeBit16U(5u)},
            {&huffmanNumberCode.NumberOfDistanceCodes, PXTypeBit16U(5u)},
            {&huffmanNumberCode.NumberOfLengthCodes, PXTypeBit16U(4u)}
        };

        const PXSize bitsRead = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        huffmanNumberCode.NumberOfLiteralCodes += 257u;
        huffmanNumberCode.NumberOfDistanceCodes += 1u;
        huffmanNumberCode.NumberOfLengthCodes += 4u;
    }


    /*number of literal/length codes + 257. Unlike the spec, the value 257 is added to it here already*/
    //huffmanNumberCode.NumberOfLiteralCodes =  readBits(reader, 5) + 257;
    /*number of distance codes. Unlike the spec, the value 1 is added to it here already*/
    //huffmanNumberCode.NumberOfDistanceCodes = readBits(reader, 5) + 1;
    /*number of code length codes. Unlike the spec, the value 4 is added to it here already*/
    //huffmanNumberCode.NumberOfLengthCodes = readBits(reader, 4) + 4;


    //HuffmanTree_init(&tree_cl);

    //while (!actionError)
    //{
    /*read the code length codes out of 3 * (amount of code length codes) bits*/
    /*if (lodepng_gtofl(reader->bp, huffmanNumberCode.NumberOfLengthCodes * 3, reader->bitsize))
    {
        // throw(50); // error: the bit pointer is or will go past the memory* AAAA/
    }*/
    for(PXInt8U index = 0; index != huffmanNumberCode.NumberOfLengthCodes; ++index)
    {
        //ensureBits9(reader, 3); /*out of bounds already checked above */
        bitlen_codeLength[PXHuffmanCodeLengthIndex[index]] = PXFileReadBits(pxFile, 3u);
    }
    for(PXInt8U index = huffmanNumberCode.NumberOfLengthCodes; index != NUM_CODE_LENGTH_CODES; ++index)
    {
        bitlen_codeLength[PXHuffmanCodeLengthIndex[index]] = 0;
    }

    {
        const PXActionResult generateError = PXGenerateFromLengths(&tree_cl, bitlen_codeLength, NUM_CODE_LENGTH_CODES, 7);

        PXActionReturnOnError(generateError);
    }

    // now we can use this tree to read the lengths for the tree that this function will return



    /*i is the current symbol we're reading in the part that contains the code lengths of lit/len and dist codes*/

    for(PXSize i = 0; i < huffmanNumberCode.NumberOfLiteralCodes + huffmanNumberCode.NumberOfDistanceCodes; )
    {
        //ensureBits25(reader, 22); /* up to 15 bits for huffman code, up to 7 extra bits below*/

        const PXInt16U code = PXHuffmanSymbolDecode(pxFile, &tree_cl);
        const PXBool isValid = 19 > code; // valid is 0..18

        if(!isValid) // INVALIDSYMBOL
        {
            return PXActionRefusedParserSymbolNotAsExpected; // Error: Invalid code, tried to read disallowed huffman symbol
        }

        switch(code)
        {
            case 18: // repeat "0" 11-138 times
            {
                PXSize replength = 11; // read in the bits that indicate repeat length
                replength += PXFileReadBits(pxFile, 7u);

                // repeat this value in the next lengths
                for(n = 0; n < replength; ++n)
                {
                    if(i >= huffmanNumberCode.NumberOfLiteralCodes + huffmanNumberCode.NumberOfDistanceCodes)
                        return PXActionRefusedParserSymbolNotAsExpected; // error: i is larger than the amount of codes

                    if(i < huffmanNumberCode.NumberOfLiteralCodes) bitlen_lengh[i] = 0;
                    else bitlen_distance[i - huffmanNumberCode.NumberOfLiteralCodes] = 0;
                    ++i;
                }

                break;
            }
            case 17: // repeat "0" 3-10 times
            {
                PXSize replength = 3; // read in the bits that indicate repeat length
                replength += PXFileReadBits(pxFile, 3u);

                /*repeat this value in the next lengths*/
                for(n = 0; n < replength; ++n)
                {
                    if(i >= huffmanNumberCode.NumberOfLiteralCodes + huffmanNumberCode.NumberOfDistanceCodes)
                        return PXActionRefusedParserSymbolNotAsExpected; // error: i is larger than the amount of codes

                    if(i < huffmanNumberCode.NumberOfLiteralCodes) bitlen_lengh[i] = 0;
                    else bitlen_distance[i - huffmanNumberCode.NumberOfLiteralCodes] = 0;
                    ++i;
                }

                break;
            }
            case 16: // repeat previous
            {
                PXSize replength = 3; /*read in the 2 bits that indicate repeat length (3-6)*/
                PXSize value; /*set value to the previous code*/

                if(i == 0)
                    return PXActionRefusedParserSymbolNotAsExpected; // can't repeat previous if i is 0

                replength += PXFileReadBits(pxFile, 2u);

                if(i < huffmanNumberCode.NumberOfLiteralCodes + 1) value = bitlen_lengh[i - 1];
                else value = bitlen_distance[i - huffmanNumberCode.NumberOfLiteralCodes - 1];
                /*repeat this value in the next lengths*/
                for(n = 0; n < replength; ++n)
                {
                    if(i >= huffmanNumberCode.NumberOfLiteralCodes + huffmanNumberCode.NumberOfDistanceCodes)
                        return PXActionRefusedParserSymbolNotAsExpected; /*error: i is larger than the amount of codes*/

                    if(i < huffmanNumberCode.NumberOfLiteralCodes) bitlen_lengh[i] = value;
                    else bitlen_distance[i - huffmanNumberCode.NumberOfLiteralCodes] = value;
                    ++i;
                }

                break;
            }
            default: // 0..15 : A length code
            {
                if(i < huffmanNumberCode.NumberOfLiteralCodes)
                {
                    bitlen_lengh[i] = code;
                }
                else
                {
                    bitlen_distance[i - huffmanNumberCode.NumberOfLiteralCodes] = code;
                }
                ++i;

                break;
            }
        }

        /*check if any of the ensureBits above went out of bounds*/
        /*if (reader->bp > reader->bitsize)
        {
            /*return error code 10 or 11 depending on the situation that happened in huffmanDecodeSymbol
            (10=no endcode, 11=wrong jump outside of tree)*/
            /* TODO: revise error codes 10,11,50: the above comment is no longer valid * /
            throw(50); /*error, bit pointer jumps past memory* /
            }*/
    }

    //if (actionError) break;

    {
        const PXBool isEndCodeValid = bitlen_lengh[256] > 0; // the length of the end code 256 must be larger than 0

        if(!isEndCodeValid)
            return PXActionRefusedParserSymbolNotAsExpected;

    }

    // now we've finally got huffmanNumberCode.NumberOfLiteralCodes and
    // huffmanNumberCode.NumberOfDistanceCodes, so generate the code trees,
    // and the function is done

    {
        const PXActionResult generateError = PXGenerateFromLengths(treeLength, bitlen_lengh, NUM_DEFLATE_CODE_SYMBOLS, 15); // treeLength

        PXActionReturnOnError(generateError);
    }


    {
        const PXActionResult generateError = PXGenerateFromLengths(treeDistance, bitlen_distance, NUM_DISTANCE_SYMBOLS, 15); // treeDistance

        PXActionReturnOnError(generateError);
    }

    //break; /*end of error-while*/
    //}

    return PXActionSuccessful;
}

PXInt16U PXAPI PXHuffmanSymbolDecode(struct PXFile_* const pxFile, const PXHuffmanTree* const huffmanTree)
{
    PXHuffmanSymbol huffmanSymbol;
    huffmanSymbol.Code = PXFilePeekBits(pxFile, PXHuffmanFirstBits);
    huffmanSymbol.Length = huffmanTree->TableLength[huffmanSymbol.Code];
    huffmanSymbol.Value = huffmanTree->TableValue[huffmanSymbol.Code];

    const PXBool finished = huffmanSymbol.Length <= PXHuffmanFirstBits;

    if(finished)
    {
        PXFileCursorMoveBits(pxFile, huffmanSymbol.Length);

        return huffmanSymbol.Value;
    }
    else
    {
        PXFileCursorMoveBits(pxFile, PXHuffmanFirstBits);

        const PXInt16U extraBitsToRead = huffmanSymbol.Length - PXHuffmanFirstBits;
        const PXInt16U extraBits = PXFilePeekBits(pxFile, extraBitsToRead);
        const PXInt16U index2 = huffmanSymbol.Value + extraBits;
        const PXInt16U moveBits = huffmanTree->TableLength[index2] - PXHuffmanFirstBits;
        const PXInt16U result = huffmanTree->TableValue[index2];

        PXFileCursorMoveBits(pxFile, moveBits);

        return result;
    }
}

PXInt32U PXAPI reverseBits(const PXInt32U bits, const PXInt32U num)
{
    /*TODO: implement faster lookup table based version when needed*/
    PXInt32U result = 0;
    for(PXInt32U i = 0; i < num; i++)
        result |= ((bits >> (num - i - 1u)) & 1u) << i;
    return result;
}

PXActionResult PXAPI PXHuffmanDistanceTreeGenerateFixed(PXHuffmanTree* const treeLength, PXHuffmanTree* const treeDistance)
{
    {
        //------------------------------------------- generateFixedLitLenTree()
        const PXSize maxbitlen = 15;
        const PXSize numcodes = NUM_DEFLATE_CODE_SYMBOLS;
        PXInt32U bitlen[NUM_DEFLATE_CODE_SYMBOLS];

        // 288 possible codes:
        // 0-255=literals, 256=endcode, 257-285=lengthcodes, 286-287=unused*/
        for(PXInt16U i = 0u; i <= 143u; ++i) bitlen[i] = 8u;
        for(PXInt16U i = 144u; i <= 255u; ++i) bitlen[i] = 9u;
        for(PXInt16U i = 256u; i <= 279u; ++i) bitlen[i] = 7u;
        for(PXInt16U i = 280u; i <= 287u; ++i) bitlen[i] = 8u;
        //---------------------------------------------------------------------------

        const PXActionResult result = PXGenerateFromLengths(treeLength, bitlen, numcodes, maxbitlen);
    }

 
    {
        // there are 32 distance codes, but 30-31 are unused

        const PXSize maxbitlen = 15;
        const PXSize numcodes = NUM_DISTANCE_SYMBOLS;
        PXInt32U bitlen[NUM_DISTANCE_SYMBOLS];

        for(PXInt8U i = 0; i < NUM_DISTANCE_SYMBOLS; ++i)
        {
            bitlen[i] = 5u;
        }

        const PXActionResult result = PXGenerateFromLengths(treeDistance, bitlen, numcodes, maxbitlen);

        return result;
    } 
}

void PXAPI PXHuffmanTreeDestruct(PXHuffmanTree* const huffmanTree)
{
    PXMemoryHeapFree(PXNull, huffmanTree->CodeSymbols);
    PXMemoryHeapFree(PXNull, huffmanTree->LengthsList);
    PXMemoryHeapFree(PXNull, huffmanTree->TableLength);
    PXMemoryHeapFree(PXNull, huffmanTree->TableValue);
}

PXHuffmanCodeType PXAPI PXHuffmanCodeTypeFromCode(const PXInt16U code)
{
    if(285u < code) return PXHuffmanCodeInvalid; // if too big => Invalid

    if(256u < code) return PXHuffmanCodeLength; // if 0..255 => Literal

    if(256u == code) return PXHuffmanCodeEndOfBlock; // if exactly 256 => End

    return PXHuffmanCodeLiteral; // if 0..254 => Literal
}