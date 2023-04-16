#ifndef ISBNINCLUDE
#define ISBNINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned char ISBNIsValid(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif