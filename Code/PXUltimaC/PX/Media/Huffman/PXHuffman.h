#ifndef PXHuffmanTreeIncluded
#define PXHuffmanTreeIncluded

#include <PX/Engine/PXResource.h>

// amount of bits for first huffman table lookup (aka root bits),
// see HuffmanTree_makeTable and huffmanDecodeSymbol.
// values 8u and 9u work the fastest
#define PXHuffmanFirstBits 9u

typedef enum PXHuffmanCodeType_
{
    PXHuffmanCodeInvalid,
    PXHuffmanCodeLiteral,
    PXHuffmanCodeLength,
    PXHuffmanCodeEndOfBlock
}
PXHuffmanCodeType;

typedef struct PXHuffmanNumberCode_
{
    PXI16U NumberOfLiteralCodes; // 9-Bit
    PXI16U NumberOfDistanceCodes; // 5-Bit
    PXI16U NumberOfLengthCodes; // 4-Bit
}
PXHuffmanNumberCode;

typedef struct PXHuffmanSymbol_
{
    PXI16U Code;
    PXI16U Length;
    PXI16U Value;
}
PXHuffmanSymbol;

typedef struct PXHuffmanTree_
{
    PXI32U* CodeSymbols; // the huffman codes (bit patterns representing the symbols)
    PXI32U* LengthsList; // the lengths of the huffman codes
    PXI32U maxbitlen; // maximum number of bits a single code can get
    PXI32U NumberOfSymbols; // number of symbols in the alphabet = number of codes

    // for reading only
    PXI8U* TableLength; // length of symbol from lookup table, or max length if secondary lookup needed
    PXI16U* TableValue; // value of symbol from lookup table, or pointer to secondary table if needed
}
PXHuffmanTree;

PXPublic void PXAPI PXHuffmanTreeDestruct(PXHuffmanTree* const huffmanTree);

// Literal : 0 <= x < 256
// End of Block : x = 256
// Length : 256 < x < 285
// Invalid : x > 285
PXPublic PXHuffmanCodeType PXAPI PXHuffmanCodeTypeFromCode(const PXI16U code);

PXPublic PXResult PXAPI PXHuffmanDistanceTreeGenerateFixed(PXHuffmanTree* const treeLength, PXHuffmanTree* const treeDistance);
PXPublic PXResult PXAPI PXHuffmanDistanceTreeGenerateDynamic(PXFile* const pxFile, PXHuffmanTree* const treeLength, PXHuffmanTree* const treeDistance);

PXPublic PXResult PXAPI PXGenerateFromLengths(PXHuffmanTree* const huffmanTree, const PXI32U* const bitlen, const PXSize numcodes, const PXSize maxbitlen);

PXPublic PXI16U PXAPI PXHuffmanSymbolDecode(struct PXFile_* const pxFile, const PXHuffmanTree* const codetree);

PXPublic PXI32U PXAPI reverseBits(const PXI32U bits, const PXI32U num);

#endif
