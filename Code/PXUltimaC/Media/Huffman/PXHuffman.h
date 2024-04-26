#ifndef PXHuffmanTreeINCLUDE
#define PXHuffmanTreeINCLUDE

#include "../PXResource.h"

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
	PXInt16U NumberOfLiteralCodes;
	PXInt16U NumberOfDistanceCodes;
	PXInt16U NumberOfLengthCodes;
}
PXHuffmanNumberCode;

typedef struct PXHuffmanSymbol_
{
	PXInt16U Code;
	PXInt16U Length;
	PXInt16U Value;
}
PXHuffmanSymbol;

typedef struct PXHuffmanTree_
{
	PXInt32U* CodeSymbols; // the huffman codes (bit patterns representing the symbols)
	PXInt32U* LengthsList; // the lengths of the huffman codes
	PXInt32U maxbitlen; // maximum number of bits a single code can get
	PXInt32U NumberOfSymbols; // number of symbols in the alphabet = number of codes

	// for reading only
	PXInt8U* TableLength; // length of symbol from lookup table, or max length if secondary lookup needed
	PXInt16U* TableValue; // value of symbol from lookup table, or pointer to secondary table if needed
}
PXHuffmanTree;

PXPublic void PXAPI PXHuffmanTreeDestruct(PXHuffmanTree* const huffmanTree);

// Literal : 0 <= x < 256
// End of Block : x = 256
// Length : 256 < x < 285
// Invalid : x > 285
PXPublic PXHuffmanCodeType PXAPI PXHuffmanCodeTypeFromCode(const PXInt16U code);

PXPublic PXActionResult PXAPI PXHuffmanDistanceTreeGenerateFixedLiteralLengthTree(PXHuffmanTree* const huffmanTree);
PXPublic PXActionResult PXAPI PXHuffmanDistanceTreeGenerateFixed(PXHuffmanTree* const huffmanTree);

PXPublic PXActionResult PXAPI PXHuffmanDistanceTreeGenerateDynamic(struct PXFile_* const pxFile, PXHuffmanTree* treeLength, PXHuffmanTree* treeDistance);

PXPublic PXActionResult PXAPI PXGenerateFromLengths(PXHuffmanTree* const huffmanTree, const PXInt32U* const bitlen, const PXSize numcodes, const PXSize maxbitlen);

PXPublic PXInt16U PXAPI PXHuffmanSymbolDecode(struct PXFile_* const pxFile, const PXHuffmanTree* const codetree);

PXPublic PXInt32U PXAPI reverseBits(const PXInt32U bits, const PXInt32U num);

#endif