#ifndef HuffmanTreeInclude
#define HuffmanTreeInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <File/DataStream.h>


/* amount of bits for first huffman table lookup (aka root bits), see HuffmanTree_makeTable and huffmanDecodeSymbol.*/
/* values 8u and 9u work the fastest */
#define FIRSTBITS 9u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum HuffmanCodeType_
	{
		HuffmanCodeInvalid,
		HuffmanCodeLiteral,
		HuffmanCodeLength,
		HuffmanCodeEndOfBlock
	}
	HuffmanCodeType;

	typedef struct HuffmanNumberCode_
	{
		unsigned short NumberOfLiteralCodes;
		unsigned short NumberOfDistanceCodes;
		unsigned short NumberOfLengthCodes;
	}
	HuffmanNumberCode;

	typedef struct HuffmanSymbol_
	{
		unsigned short Code;
		unsigned short Length;
		unsigned short Value;
	}
	HuffmanSymbol;

	typedef enum HuffmanResult_
	{
		HuffmanInvalid,

		HuffmanSucess,



	}HuffmanResult;


	typedef struct HuffmanTree_
	{
		unsigned int* codes; /*the huffman codes (bit patterns representing the symbols)*/
		unsigned int* lengths; /*the lengths of the huffman codes*/
		unsigned int maxbitlen; /*maximum number of bits a single code can get*/
		unsigned int numcodes; /*number of symbols in the alphabet = number of codes*/

		/* for reading only */
		unsigned char* table_len; /*length of symbol from lookup table, or max length if secondary lookup needed*/
		unsigned short* table_value; /*value of symbol from lookup table, or pointer to secondary table if needed*/
	}
	HuffmanTree;


	PXPublic void HuffmanTreeConstruct(HuffmanTree* const huffmanTree);
	PXPublic void HuffmanTreeDestruct(HuffmanTree* const huffmanTree);

	PXPublic HuffmanCodeType HuffmanCodeTypeFromCode(const unsigned short code);


	PXPublic void GenerateFixedLiteralLengthTree(HuffmanTree* huffmanTree);
	PXPublic void GenerateFixedDistanceTree(HuffmanTree* huffmanTree);

	PXPublic int GenerateFromLengths(HuffmanTree* huffmanTree, const unsigned int* bitlen, size_t numcodes, size_t maxbitlen);



	PXPublic unsigned reverseBits(unsigned bits, unsigned num);
	PXPublic unsigned int GenerateDynamicTree(DataStream* const dataStream, HuffmanTree* treeLength, HuffmanTree* treeDistance);

	/* Safely check if a + b > c, even if overflow could happen. */
	PXPublic int lodepng_gtofl(size_t a, size_t b, size_t c);
	PXPublic int lodepng_addofl(size_t a, size_t b, size_t* result);
	PXPublic unsigned int huffmanDecodeSymbol(DataStream* const dataStream, HuffmanTree* codetree);


#ifdef __cplusplus
}
#endif

#endif