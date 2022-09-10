#ifndef FileOpenDialogInclude
#define FileOpenDialogInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	CPublic unsigned char FileOpenDialogA(char* filePathOutput);
	CPublic unsigned char FileOpenDialogW(wchar_t* filePathOutput);

#ifdef __cplusplus
}
#endif

#endif