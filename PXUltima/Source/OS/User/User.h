#ifndef UserInclude
#define UserInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXSize UserNameGetA(PXTextASCII* name, const PXSize nameSizeMax);
	PXPublic PXSize UserNameGetW(PXTextUNICODE* name, const PXSize nameSizeMax);
	PXPublic PXSize UserNameGetU(PXTextUTF8* name, const PXSize nameSizeMax);

#ifdef __cplusplus
}
#endif

#endif