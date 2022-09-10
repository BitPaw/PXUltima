#ifndef FMTInclude
#define FMTInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <File/Endian.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Format header
	typedef struct FMT_
	{
		unsigned int ChunkSize;
		unsigned short AudioFormat;
		unsigned short NumerOfChannels;
		unsigned int SampleRate;
		unsigned int ByteRate;
		unsigned short BlockAllign;
		unsigned short BitsPerSample;
	}
	FMT;

	CPublic void FMTConstruct(FMT* const fmt);

	CPublic ActionResult FMTParse(FMT* const fmt, const void* data, const size_t dataSize, size_t* dataRead, const Endian endian);

#ifdef __cplusplus
}
#endif

#endif