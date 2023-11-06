#ifndef PXHuffmanTreeINCLUDE
#define PXHuffmanTreeINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>


/* amount of bits for first huffman table lookup (aka root bits), see HuffmanTree_makeTable and huffmanDecodeSymbol.*/
/* values 8u and 9u work the fastest */
#define FIRSTBITS 9u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXHuffmanCodeType_
	{
		HuffmanCodeInvalid,
		HuffmanCodeLiteral,
		HuffmanCodeLength,
		HuffmanCodeEndOfBlock
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

	typedef enum HuffmanResult_
	{
		HuffmanInvalid,

		HuffmanSucess,
	}
	HuffmanResult;

	typedef struct PXHuffmanTree_
	{
		PXInt32U* codes; /*the huffman codes (bit patterns representing the symbols)*/
		PXInt32U* lengths; /*the lengths of the huffman codes*/
		PXInt32U maxbitlen; /*maximum number of bits a single code can get*/
		PXInt32U numcodes; /*number of symbols in the alphabet = number of codes*/

		/* for reading only */
		PXInt8U* table_len; /*length of symbol from lookup table, or max length if secondary lookup needed*/
		PXInt16U* table_value; /*value of symbol from lookup table, or pointer to secondary table if needed*/
	}
	PXHuffmanTree;


	PXPublic void PXHuffmanTreeConstruct(PXHuffmanTree* const huffmanTree);
	PXPublic void PXHuffmanTreeDestruct(PXHuffmanTree* const huffmanTree);

	PXPublic PXHuffmanCodeType PXHuffmanCodeTypeFromCode(const unsigned short code);

	PXPublic void PXHuffmanDistanceTreeGenerateFixedLiteralLengthTree(PXHuffmanTree* huffmanTree);
	PXPublic void PXHuffmanDistanceTreeGenerateFixed(PXHuffmanTree* const huffmanTree);

	PXPublic int PXGenerateFromLengths(PXHuffmanTree* huffmanTree, const unsigned int* bitlen, PXSize numcodes, PXSize maxbitlen);



	PXPublic unsigned reverseBits(unsigned bits, unsigned num);
	PXPublic unsigned int PXHuffmanDistanceTreeGenerateDynamic(PXFile* const pxFile, PXHuffmanTree* treeLength, PXHuffmanTree* treeDistance);

	/* Safely check if a + b > c, even if overflow could happen. */
	PXPublic int lodepng_gtofl(PXSize a, PXSize b, PXSize c);
	PXPublic int lodepng_addofl(PXSize a, PXSize b, PXSize* result);
	PXPublic unsigned int PXHuffmanSymbolDecode(PXFile* const pxFile, const PXHuffmanTree* const codetree);


#ifdef __cplusplus
}
#endif

#endif