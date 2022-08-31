#ifndef FMTInclude
#define FMTInclude

#include <stddef.h>

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

	extern void FMTConstruct(FMT* const fmt);

	extern ActionResult FMTParse(FMT* const fmt, const void* data, const size_t dataSize, size_t* dataRead, const Endian endian);

#ifdef __cplusplus
}
#endif

#endif