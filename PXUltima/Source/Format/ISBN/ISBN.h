#ifndef ISBNINCLUDE
#define ISBNINCLUDE

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

	extern unsigned char ISBNIsValid(const void* data, const size_t dataSize);

#ifdef __cplusplus
}
#endif

#endif