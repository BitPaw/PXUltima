#ifndef UserInclude
#define UserInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool UserNameGetA(char* name, const size_t nameSizeMax, size_t* nameSizeWritten);
	PXPublic PXBool UserNameGetW(wchar_t* name, const size_t nameSizeMax, size_t* nameSizeWritten);

#ifdef __cplusplus
}
#endif

#endif