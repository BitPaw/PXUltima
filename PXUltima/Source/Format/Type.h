#ifndef TypeINCLUDE
#define TypeINCLUDE

#include <stddef.h>

#include <OS/OSVersion.h>

//-- Reivented public / private. The keywords are reserved, so we need other names.
#if defined(OSUnix)
#define CPrivate static 
#define CPublic extern 
#elif defined(OSWindows)
#define CPrivate static 
#define CPublic extern __declspec(dllexport) // The visual studio compiler also wants this definition, for microsoft stuff.
#endif
//-----------------------------------------------------------------------------

#ifndef Byte
#define CByte unsigned char
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif

#endif