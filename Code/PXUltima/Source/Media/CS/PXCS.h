#ifndef PXCSINCLUDE
#define PXCSINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <File/PXDataStream.h>
#include <Media/C/PXC.h>

#ifdef __cplusplus
extern "C"
{
#endif

	

	typedef enum PXCSKeyWord_
	{
		PXCSKeyWordInvalid,

		PXCSKeyWordNameSpace,

		PXCSKeyWordStruct,
		PXCSKeyWordClass,

		PXCSKeyWordVolatile,

		PXCSKeyWordEE
	}
	PXCSKeyWord;


	typedef struct PXCTranslateStruct_
	{
		PXCElement Element;

		PXBool AreInUnion;

		PXInt16U StructureOffsetTotal;

		PXInt16U UnionStartOffset;
		PXInt16U UnionWidthOffset;
	}
	PXCTranslateStruct;

	PXPublic PXCSKeyWord PXCSFetchNext(PXDataStream* const inputSteam);

	PXPublic void PXCSSerialize(PXDataStream* const inputSteam, PXDataStream* const outputStream, PXCTranslateStruct* const pxCTranslateStruct);

	PXPublic void PXCSCreateWrapperFromCSource(PXDataStream* const inputSteam, PXDataStream* const outputStream);

#ifdef __cplusplus
}
#endif

#endif