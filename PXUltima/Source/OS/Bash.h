#ifndef BashInclude
#define BashInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic ActionResult BashExecuteA(const char* programPath, const unsigned char syncronous);
	PXPublic ActionResult BashExecuteW(const wchar_t* programPath, const unsigned char syncronous);

#ifdef __cplusplus
}
#endif

#endif
