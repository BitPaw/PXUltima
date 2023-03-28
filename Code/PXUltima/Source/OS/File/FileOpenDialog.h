#ifndef FileOpenDialogInclude
#define FileOpenDialogInclude

#include <Media/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool FileOpenDialogA(char* filePathOutput);
	PXPublic PXBool FileOpenDialogW(wchar_t* filePathOutput);

#ifdef __cplusplus
}
#endif

#endif