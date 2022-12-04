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

	typedef unsigned char PXByte;
	typedef unsigned char PXBool;
	typedef unsigned char* PXAdress;

	typedef unsigned char PXInt8U;
	typedef unsigned short PXInt16U;
	typedef unsigned int PXInt32U;
	typedef unsigned __int64 PXInt64U;

	typedef char PXCharASCII;
	typedef wchar_t PXCharUNICODE;
	typedef PXInt32U PXCharUTF8;

	typedef char* const PXASCIIChar;
	typedef char* const PXTextUTF8;
	typedef wchar_t* const PXTextUNICODE;

#if OS32Bit
	typedef PXInt32U PXSize;
#elif OS64Bit
	typedef PXInt64U PXSize;
#else
#error Invalid Bit Version
#endif





#ifdef __cplusplus
}
#endif

#endif