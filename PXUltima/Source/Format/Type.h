#ifndef TypeINCLUDE
#define TypeINCLUDE

#include <stddef.h>

#include <OS/OSVersion.h>

//-- Reivented public / private. The keywords are reserved, so we need other names.
#if OSUnix
#define PXPrivate static 
#define PXPublic extern 
#elif OSWindows
#define PXPrivate static 
#define PXPublic extern __declspec(dllexport) // The visual studio compiler also wants this definition, for microsoft stuff.
#endif
//-----------------------------------------------------------------------------

#ifndef PXAdress
#define PXAdress unsigned char*
#endif

#ifndef PXByte
#define PXByte unsigned char
#endif

#ifndef PXBool
#define PXBool unsigned char
#endif

#ifndef PXYes
#define PXYes 1u
#endif

#ifndef PXNo
#define PXNo 0u
#endif

#ifndef PXTrue
#define PXTrue 1u
#endif

#ifndef PXFalse
#define PXFalse 0u
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif

#endif