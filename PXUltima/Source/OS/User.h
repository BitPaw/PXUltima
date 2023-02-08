#ifndef UserInclude
#define UserInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXSize UserNameGetA(char* name, const PXSize nameSizeMax);
	PXPublic PXSize UserNameGetW(wchar_t* name, const PXSize nameSizeMax);

#ifdef __cplusplus
}
#endif

#endif