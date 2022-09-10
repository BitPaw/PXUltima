#ifndef A3DSInclude
#define A3DSInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	CPublic unsigned int Adler32Check(unsigned char* data, size_t length);
	CPublic unsigned int Adler32Create(size_t adler, const unsigned char* data, size_t length);

#ifdef __cplusplus
}
#endif

#endif