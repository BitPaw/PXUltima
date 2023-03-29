#ifndef PXCSINCLUDE
#define PXCSINCLUDE

#include <Media/Type.h>
#include <OS/Error/PXActionResult.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXCSKeyWord_
	{
		PXCSKeyWordInvalid,

		PXCSKeyWordNameSpace,

		PXCSKeyWordStruct,
		PXCSKeyWordClass
	}
	PXCSKeyWord;

	PXPublic void PXCSSerialize(PXDataStream* const inputSteam, PXDataStream* const outputStream);

	PXPublic void PXCSCreateWrapperFromCSource(PXDataStream* const inputSteam, PXDataStream* const outputStream);

#ifdef __cplusplus
}
#endif

#endif