#ifndef PXCRC32INCLUDE
#define PXCRC32INCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned int PXCRC32Generate(const unsigned char* const data, const PXSize length);

#ifdef __cplusplus
}
#endif

#endif