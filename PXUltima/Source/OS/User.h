#ifndef UserInclude
#define UserInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool UserNameGetA(char* name, const PXSize nameSizeMax, PXSize* nameSizeWritten);
	PXPublic PXBool UserNameGetW(wchar_t* name, const PXSize nameSizeMax, PXSize* nameSizeWritten);

#ifdef __cplusplus
}
#endif

#endif