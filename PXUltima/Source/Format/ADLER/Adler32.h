#ifndef A3DSInclude
#define A3DSInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned int Adler32Check(const void* const data, const PXSize length);
	PXPublic unsigned int Adler32Create(const PXSize adler, const unsigned char* data, PXSize length);

#ifdef __cplusplus
}
#endif

#endif