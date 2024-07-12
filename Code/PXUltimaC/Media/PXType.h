#ifndef PXTypeINCLUDE
#define PXTypeINCLUDE

//#include <stddef.h>

#include <OS/System/PXOSVersion.h>

#include <wchar.h>

#ifdef __cplusplus
#define PXLanguageC 0
#define PXLanguageCPP 1
#else
#define PXLanguageC 1
#define PXLanguageCPP 0
#endif

//-----------------------------------------------------------------------------
// Detect C Version
//-----------------------------------------------------------------------------
#define CVersion 201703L

#if (CVersion >= 199711L)
#define CVersionAtleast1998 1
#else
#define CVersionAtleast1998 0
#endif

#if (CVersion >= 201103L)
#define CVersionNewerThen2011 1
#else
#define CVersionNewerThen2011 0
#endif

#if (CVersion >= 201402L)
#define CVersionNewerThen2014 1
#else
#define CVersionNewerThen2014 0
#endif

#if (CVersion >= 201703L)
#define CVersionNewerThen2017 1
#else
#define CVersionNewerThen2017 0
#endif

#if (CVersion >= 202002L)
#define CVersionNewerThen2020 1
#else
#define CVersionNewerThen2020 0
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Detect C++ Version
//-----------------------------------------------------------------------------
#define CPPVersion __cplusplus

// Before 1998, '__cplusplus' is == 1

#if (CPPVersion >= 199711L)
#define CPPVersionAtleast1998 1
#else
#define CPPVersionAtleast1998 0
#endif

#if (CPPVersion >= 201103L)
#define CPPVersionNewerThen2011 1
#else
#define CPPVersionNewerThen2011 0
#endif

#if (CPPVersion >= 201402L)
#define CPPVersionNewerThen2014 1
#else
#define CPPVersionNewerThen2014 0
#endif

#if (CPPVersion >= 201703L)
#define CPPVersionNewerThen2017 1
#else
#define CPPVersionNewerThen2017 0
#endif

#if (CPPVersion >= 202002L)
#define CPPVersionNewerThen2020 1
#else
#define CPPVersionNewerThen2020 0
#endif
//-----------------------------------------------------------------------------





//-- Reivented public / private. The keywords are reserved, so we need other names.
#if OSUnix

#define PXCDECL
#define PXSTDCALL
#define PXSYSCALL
#define PXFASTCALL

#elif OSWindows

#define PXCDECL _cdecl
#define PXSTDCALL _stdcall
#define PXSYSCALL 
#define PXAPIFASTCALL _fastcall

#endif

#define PXAPI PXSTDCALL

//---------------------------------------------------------
// Change label of functions to C-Style (C++ only)
//---------------------------------------------------------
#ifdef __cplusplus
#define PXFunction extern "C"
#else
#define PXFunction
#endif
//---------------------------------------------------------

#define PXPrivate PXFunction static
#define PXInternal 

#if OSUnix
#define PXPublic PXFunction extern
#define PXMSHandle void*
#elif OSWindows
#define PXDLLExport __declspec(dllexport)
#define PXPublic PXFunction extern PXDLLExport // The visual studio compiler also wants this definition, for microsoft stuff.
#define PXMSHandle HANDLE
#endif
//-----------------------------------------------------------------------------

#define PXFunctionInvoke(FunctionPoniter, ...) if(FunctionPoniter) FunctionPoniter(__VA_ARGS__)


#if PXLanguageCPP
#define PXDLLExportMangled PXDLLExport
#define PXDLLExportUnMangled extern "C" PXDLLExport
#endif

//-----------------------------------------------------------------------------
// Restrict keyword
//-----------------------------------------------------------------------------
#if 1
#define PXRestrict __restrict
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

#ifndef PXNull
#define PXNull 0u
#endif


//---------------------------------------------------------
// Flags
//---------------------------------------------------------
#define PXFlagIsSet(value, flagID) (1u && (value & flagID))
#define PXFlagAdd(target, value, flagID) target |= flagID 
#define PXFlagRemove(target, value, flagID)  target &= ~flagID


#define PXDataTypeReadOnlyMask			0b10000000000000000000000000000000
#define PXDataTypeAdressMask			0b01000000000000000000000000000000 // Used if the type is
#define PXDataTypeEndianMask			0b00110000000000000000000000000000 // Little or big endian. If No endian is spesified, we can just copy 1:1
#define PXDataTypeUseFileModeMask		0b00000000000000000000000000000000
#define PXDataTypeSignedMask			0b00001000000000000000000000000000 // Only useful in numeric values
#define PXDataTypeIgnoreIFMask			0b00000110000000000000000000000000
#define PXDataTypeShallDoIO				0b00000001000000000000000000000000 // Set if you want to read or write (To support Padding)
#define PXDataTypeTypeMask				0b00000000110000000000000000000000
#define PXDataTypeBaseNumeric			0b00000000100000000000000000000000
#define PXDataTypeBaseDecimal			0b00000000010000000000000000000000

#define PXDataTypeBitFieldHolderMask	0b00000000000001100000000000000000
#define PXDataTypeBitFieldHolder08U		0b00000000000000000000000000000000 // 1 Bytes
#define PXDataTypeBitFieldHolder16U		0b00000000000000100000000000000000 // 2 Bytes
#define PXDataTypeBitFieldHolder32U		0b00000000000001000000000000000000 // 4 Bytes
#define PXDataTypeBitFieldHolder64U		0b00000000000001100000000000000000 // 8 Bytes

#define PXDataTypeModeMask				0b00000000000000010000000000000000
#define PXDataTypeModeByte				0b00000000000000000000000000000000
#define PXDataTypeModeBit				0b00000000000000010000000000000000

#define PXDataTypeSizeMask				0b00000000000000001111111111111111 // Size in bytes of target data 0x0000FFFF


#define PXDataTypeIgnoreIn32B 0b00000010000000000000000000000000
#define PXDataTypeIgnoreIn64B 0b00000100000000000000000000000000

#define PXDataTypeDirect		(0b00 << 28)
#define PXDataTypeEndianBig		(0b01 << 28)
#define PXDataTypeEndianLittle	(0b10 << 28)

#define PXDataTypeSigned PXDataTypeSignedMask
#define PXDataTypeUnsigned 0

#define PXDataTypeSize00		0
#define PXDataTypeSize08		1
#define PXDataTypeSize16		2
#define PXDataTypeSize32		4
#define PXDataTypeSize64		8
#define PXDataTypeSize128		16

#define PXDataTypeText(size) size

#define PXDataTypeReadOnly PXDataTypeReadOnlyMask

//-------------------------------------------------
// Adress, read as spesified but store it as (void*)
//-------------------------------------------------
#define PXDataTypeAdress08 PXDataTypeAdressMask | PXDataTypeSize08
#define PXDataTypeAdress16 PXDataTypeAdressMask | PXDataTypeSize16
#define PXDataTypeAdress32 PXDataTypeAdressMask | PXDataTypeSize32
#define PXDataTypeAdress64 PXDataTypeAdressMask | PXDataTypeSize64
#define PXDataTypeAdressFlex PXDataTypeAdressMask
#define PXDataTypeAdress PXDataTypeAdressMask | sizeof(void*)
#define PXDataTypeString PXDataTypeAdress

//-------------------------------------------------
// Text - used to parse singatures
//-------------------------------------------------
#define PXDataTypeDatax2 PXDataTypeSize16
#define PXDataTypeDatax4 PXDataTypeSize32
#define PXDataTypeDatax8 PXDataTypeSize64
#define PXDataTypeDatax16 PXDataTypeSize128

//-------------------------------------------------
// Emoty space
//-------------------------------------------------
#define PXDataTypePadding(size) size

//-------------------------------------------------
// Int - Normal
//-------------------------------------------------
#define PXDataTypeIntS PXDataTypeBaseNumeric | PXDataTypeSigned
#define PXDataTypeIntU PXDataTypeBaseNumeric | PXDataTypeUnsigned

#define PXDataTypeInt08S PXDataTypeSize08 | PXDataTypeIntS
#define PXDataTypeInt08U PXDataTypeSize08 | PXDataTypeIntU

#define PXDataTypeBool PXDataTypeInt08U

#define PXDataTypeInt16S PXDataTypeSize16 | PXDataTypeIntS
#define PXDataTypeInt16SLE PXDataTypeSize16 | PXDataTypeIntS | PXDataTypeEndianLittle
#define PXDataTypeInt16SBE PXDataTypeSize16 | PXDataTypeIntS | PXDataTypeEndianBig
#define PXDataTypeInt16U PXDataTypeSize16 | PXDataTypeIntU
#define PXDataTypeInt16ULE PXDataTypeSize16 | PXDataTypeIntU | PXDataTypeEndianLittle
#define PXDataTypeInt16UBE PXDataTypeSize16 | PXDataTypeIntU | PXDataTypeEndianBig

#define PXDataTypeInt32S PXDataTypeSize32 | PXDataTypeIntS
#define PXDataTypeInt32SLE PXDataTypeSize32 | PXDataTypeIntS | PXDataTypeEndianLittle
#define PXDataTypeInt32SBE PXDataTypeSize32 | PXDataTypeIntS | PXDataTypeEndianBig
#define PXDataTypeInt32U PXDataTypeSize32 | PXDataTypeIntU
#define PXDataTypeInt32ULE PXDataTypeSize32 | PXDataTypeIntU | PXDataTypeEndianLittle
#define PXDataTypeInt32UBE PXDataTypeSize32 | PXDataTypeIntU | PXDataTypeEndianBig

#define PXDataTypeInt64U PXDataTypeSize64
#define PXDataTypeInt64ULE PXDataTypeSize64 | PXDataTypeIntU | PXDataTypeEndianLittle
#define PXDataTypeInt64UBE PXDataTypeSize64 | PXDataTypeIntU | PXDataTypeEndianBig
#define PXDataTypeInt64S PXDataTypeSize64
#define PXDataTypeInt64SLE PXDataTypeSize64 | PXDataTypeIntS | PXDataTypeEndianLittle
#define PXDataTypeInt64SBE PXDataTypeSize64 | PXDataTypeIntS | PXDataTypeEndianBig

#define PXDataTypeInt32ULEOnlyIf32B PXDataTypeInt32ULE | PXDataTypeIgnoreIn32B
#define PXDataTypeInt32ULEOnlyIf64B PXDataTypeInt32ULE | PXDataTypeIgnoreIn64B

#define PXDataTypeSize PXDataTypeInt64U

#define PXDataTypeIntFlexLE | PXDataTypeBaseNumeric | PXDataTypeEndianLittle
#define PXDataTypeIntFlexBE | PXDataTypeBaseNumeric | PXDataTypeEndianBig

#define PXDataTypeFloat PXDataTypeSize32 | PXDataTypeBaseDecimal
#define PXDataTypeDouble PXDataTypeSize64 | PXDataTypeBaseDecimal


//-------------------------------------------------
// Int - BitMode
//-------------------------------------------------
#define PXDataTypeIntUFlexBit PXDataTypeIntU | PXDataTypeModeBit
#define PXDataTypeBit08U(bitSize) PXDataTypeBitFieldHolder08U | PXDataTypeIntUFlexBit | bitSize
#define PXDataTypeBit16U(bitSize) PXDataTypeBitFieldHolder16U | PXDataTypeIntUFlexBit | bitSize
#define PXDataTypeBit32U(bitSize) PXDataTypeBitFieldHolder32U | PXDataTypeIntUFlexBit | bitSize
#define PXDataTypeBit64U(bitSize) PXDataTypeBitFieldHolder64U | PXDataTypeIntUFlexBit | bitSize

#define PXDataTypeNibble PXDataTypeBit08U(4)

typedef unsigned char PXByte;
typedef unsigned char PXBool;
typedef unsigned char* PXAdress;

// Integer 8-Bit
typedef char PXInt8S;
typedef unsigned char PXInt8U;

// Integer 16-Bit, WORD
typedef short PXInt16S;
typedef unsigned short PXInt16U;

// Integer 32-Bit, DWORD
typedef int PXInt32S;
typedef unsigned int PXInt32U;

// Integer 64-Bit, QWORD
#if OSUnix
typedef long long PXInt64S;
typedef unsigned long long PXInt64U;
#elif OSWindows
typedef __int64 PXInt64S;
typedef unsigned __int64 PXInt64U;
#endif // OSUnix

#if OS32B
typedef PXInt32U PXSize;
typedef PXInt32S PXOffset;
typedef PXInt64U PXAdress64;
#elif OS64B
typedef PXInt64U PXSize;
typedef PXInt64S PXOffset;
typedef void* PXAdress64;
#else
#error Invalid Bit Version
#endif

// Text
typedef char PXCharASCII;
typedef wchar_t PXCharUNICODE;
typedef PXInt32U PXCharUTF8;

typedef char* const PXTextASCII;
typedef char* const PXTextUTF8;
typedef wchar_t* const PXTextUNICODE;






#define PXHandleNotSet ((void*)-1)





//-----------------------------------------------------------------------------
// Detect current endianess on this system and then use #if
//-----------------------------------------------------------------------------
#if 1 // System is little endian
#define OSEngianLittle 1
#define OSEngianBig 0
#elif 0 // System is big endian
#define OSEngianLittle 0
#define OSEngianBig 1
#else //
#error [PXUltima] System has no defined endian state, neither little or big.
#endif
//-----------------------------------------------------------------------------

//---------------------------------------------------------
// 16-Bit int
//---------------------------------------------------------
#define PXInt16SplittEndianLittle(number, a, b) \
	a = (number & 0xFF); \
	b = (number & 0xFF00) >> 8u;

#define PXInt16SplittEndianBig(number, a, b) \
	a = (number & 0xFF00) >> 8u; \
	b = (number & 0xFF);


#define PXInt16MakeEndianLittle(a, b) (\
	((PXInt16U)a) | \
	((PXInt16U)b << 8u))

#define PXInt16MakeEndianBig(a, b) (\
	((PXInt16U)a << 8u) | \
	((PXInt16U)b))

#define PXInt16FromAdressEndianLittle(list) PXInt16MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1])
#define PXInt16FromAdressEndianBig(list) PXInt16MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1])
//---------------------------------------------------------

//---------------------------------------------------------
// 24-Bit int
//---------------------------------------------------------
#define PXInt24MakeEndianLittle(a, b, c) (\
	((PXInt32U)a) | \
	((PXInt32U)b << 8u) | \
	((PXInt32U)c << 16u))

#define PXInt24MakeEndianBig(a, b, c) (\
	((PXInt32U)b << 16u) | \
	((PXInt32U)c << 8u) | \
	((PXInt32U)d))


#define PXInt24FromAdressEndianLittle(list) PXInt24MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2])
#define PXInt24FromAdressEndianBig(list) PXInt24MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2])
//---------------------------------------------------------

//---------------------------------------------------------
// 32-Bit int
//---------------------------------------------------------
#define PXInt32MakeEndianLittle(a, b, c, d) (\
	((PXInt32U)a) | \
	((PXInt32U)b << 8u) | \
	((PXInt32U)c << 16u) | \
	((PXInt32U)d << 24u))

#define PXInt32MakeEndianBig(a, b, c, d) (\
	((PXInt32U)a << 24u) | \
	((PXInt32U)b << 16u) | \
	((PXInt32U)c << 8u) | \
	((PXInt32U)d))

#define PXInt32FromAdressEndianLittle(list) PXInt32MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3])
#define PXInt32FromAdressEndianBig(list) PXInt32MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3])
//---------------------------------------------------------

//---------------------------------------------------------
// 40-Bit int
//---------------------------------------------------------
#define PXInt40MakeEndianLittle(a, b, c, d, e) (\
	((PXInt64U)a | \
	((PXInt64U)b << 8u) | \
	((PXInt64U)c << 16u) | \
	((PXInt64U)d << 24u) | \
	((PXInt64U)e << 32u)))

#define PXInt40MakeEndianBig(a, b, c, d, e) (\
	((PXInt64U)a << 32u) | \
	((PXInt64U)b << 24u) | \
	((PXInt64U)c << 16u) | \
	((PXInt64U)d << 8u) | \
	((PXInt64U)e)))

#define PXInt40FromAdressEndianLittle(list) PXInt40MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4])
#define PXInt40FromAdressEndianBig(list) PXInt40MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4])
//---------------------------------------------------------

//---------------------------------------------------------
// 48-Bit int
//---------------------------------------------------------
#define PXInt48MakeEndianLittle(a, b, c, d, e, f) (\
	((PXInt64U)a | \
	((PXInt64U)b << 8u) | \
	((PXInt64U)c << 16u) | \
	((PXInt64U)d << 24u) | \
	((PXInt64U)e << 32u) | \
	((PXInt64U)f << 40u)))

#define PXInt48MakeEndianBig(a, b, c, d, e, f) (\
	((PXInt64U)a << 40u) | \
	((PXInt64U)b << 32u) | \
	((PXInt64U)c << 24u) | \
	((PXInt64U)d << 16u) | \
	((PXInt64U)e << 8u) | \
	((PXInt64U)f)))

#define PXInt48FromAdressEndianLittle(list) PXInt48MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5])
#define PXInt48FromAdressEndianBig(list) PXInt48MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5])
//---------------------------------------------------------

//---------------------------------------------------------
// 56-Bit int
//---------------------------------------------------------
#define PXInt56MakeEndianLittle(a, b, c, d, e, f, g) (\
	((PXInt64U)a | \
	((PXInt64U)b << 8u) | \
	((PXInt64U)c << 16u) | \
	((PXInt64U)d << 24u) | \
	((PXInt64U)e << 32u) | \
	((PXInt64U)f << 40u) | \
	((PXInt64U)g << 48u)))

#define PXInt56MakeEndianBig(a, b, c, d, e, f, g) (\
	((PXInt64U)a << 48u) | \
	((PXInt64U)b << 40u) | \
	((PXInt64U)c << 32u) | \
	((PXInt64U)d << 24u) | \
	((PXInt64U)e << 16u) | \
	((PXInt64U)f << 8u) | \
	((PXInt64U)g)))

#define PXInt56FromAdressEndianLittle(list) PXInt56MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6])
#define PXInt56FromAdressEndianBig(list) PXInt56MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6])
//---------------------------------------------------------

//---------------------------------------------------------
// 64-Bit int
//---------------------------------------------------------
#define PXInt64MakeEndianLittle(a, b, c, d, e, f, g, h) (\
	((PXInt64U)a | \
	((PXInt64U)b << 8u) | \
	((PXInt64U)c << 16u) | \
	((PXInt64U)d << 24u) | \
	((PXInt64U)e << 32u) | \
	((PXInt64U)f << 40u) | \
	((PXInt64U)g << 48u) | \
	((PXInt64U)h << 56u)))

#define PXInt64MakeEndianBig(a, b, c, d, e, f, g, h) (\
	((PXInt64U)a << 56u) | \
	((PXInt64U)b << 48u) | \
	((PXInt64U)c << 40u) | \
	((PXInt64U)d << 32u) | \
	((PXInt64U)e << 24u) | \
	((PXInt64U)f << 16u) | \
	((PXInt64U)g << 8u) | \
	((PXInt64U)h)))

#define PXInt64FromAdressEndianLittle(list) PXInt64MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6], ((PXAdress)list)[7])
#define PXInt64FromAdressEndianBig(list) PXInt64MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6], ((PXAdress)list)[7])
//---------------------------------------------------------

//---------------------------------------------------------
// Direct int make/create depending on OS
//---------------------------------------------------------
#if OSEngianLittle
#define PXInt16Make PXInt16MakeEndianLittle
#define PXInt24Make PXInt24MakeEndianLittle
#define PXInt32Make PXInt32MakeEndianLittle
#define PXInt40Make PXInt40MakeEndianLittle
#define PXInt48Make PXInt48MakeEndianLittle
#define PXInt56Make PXInt56MakeEndianLittle
#define PXInt64Make PXInt64MakeEndianLittle

#define PXInt16FromAdress PXInt16FromAdressEndianLittle
#define PXInt24FromAdress PXInt24FromAdressEndianLittle
#define PXInt32FromAdress PXInt32FromAdressEndianLittle
#define PXInt40FromAdress PXInt40FromAdressEndianLittle
#define PXInt48FromAdress PXInt48FromAdressEndianLittle
#define PXInt56FromAdress PXInt56FromAdressEndianLittle
#define PXInt64FromAdress PXInt64FromAdressEndianLittle
#elif OSEngianBig
#define PXInt16Make PXInt16MakeEndianBig
#define PXInt24Make PXInt24MakeEndianBig
#define PXInt32Make PXInt32MakeEndianBig
#define PXInt40Make PXInt40MakeEndianBig
#define PXInt48Make PXInt48MakeEndianBig
#define PXInt56Make PXInt56MakeEndianBig
#define PXInt64Make PXInt64MakeEndianBig

#define PXInt16FromAdress PXInt16FromAdressEndianBig
#define PXInt24FromAdress PXInt24FromAdressEndianBig
#define PXInt32FromAdress PXInt32FromAdressEndianBig
#define PXInt40FromAdress PXInt40FromAdressEndianBig
#define PXInt48FromAdress PXInt48FromAdressEndianBig
#define PXInt56FromAdress PXInt56FromAdressEndianBig
#define PXInt64FromAdress PXInt64FromAdressEndianBig
#else
#endif
//---------------------------------------------------------


#define SplittIntLEA(i) ((i) & 0xFF)
#define SplittIntLEB(i) ((i >>  8u) & 0xFF)
#define SplittIntLEC(i) ((i >> 16u) & 0xFF)
#define SplittIntLED(i) ((i >> 24u) & 0xFF)

#define SplittIntLE(i, a, b, c, d)\
a = SplittIntLEA(i);\
b = SplittIntLEB(i);\
c = SplittIntLEC(i);\
d = SplittIntLED(i);

#define SplittInt(i, a, b, c, d) SplittIntLE(i, a, b, c, d)

typedef struct PXInt16SCluster
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
		};

		PXByte Data[2];

		PXInt16S Value;
	};
}
PXInt16SCluster;

typedef struct PXInt16UCluster
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
		};

		PXByte Data[2];

		PXInt16S Value;
	};
}
PXInt16UCluster;

typedef struct PXInt32SCluster_
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
			PXByte C;
			PXByte D;
		};

		PXByte Data[4];

		PXInt32S Value;
	};
}
PXInt32SCluster;

typedef struct PXInt32UCluster_
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
			PXByte C;
			PXByte D;
		};

		PXByte Data[4];

		PXInt32U Value;
	};
}
PXInt32UCluster;

typedef struct PXInt64SCluster_
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
			PXByte C;
			PXByte D;
			PXByte E;
			PXByte F;
			PXByte G;
			PXByte H;
		};

		PXByte Data[8];

		PXInt64S Value;
	};
}
PXInt64SCluster;

typedef struct PXInt64UCluster_
{
	union
	{
		struct
		{
			PXByte A;
			PXByte B;
			PXByte C;
			PXByte D;
			PXByte E;
			PXByte F;
			PXByte G;
			PXByte H;
		};

		PXByte Data[8];

		PXInt64U Value;
	};
}
PXInt64UCluster;


typedef enum PXBitFormat_
{
	PXBitFormatInvalid,
	PXBitFormat8,
	PXBitFormat16,
	PXBitFormat32,
	PXBitFormat64,
	PXBitFormat128,
	PXBitFormat256,
	PXBitFormat512
}
PXBitFormat;



//-----------------------------------------------------------------------------
// Endianess detection and conversion
//-----------------------------------------------------------------------------

	// Ordering or sequencing of bytes
typedef enum PXEndian_
{
	PXEndianInvalid,

	// Left to Right. Left smallest value.
	PXEndianBig,

	PXEndianMiddle,

	// Right to Left. Right smallest value.
	PXEndianLittle
}
PXEndian;

#if OSEngianLittle
#define EndianCurrentSystem PXEndianLittle
#elif OSEngianBig
#define EndianCurrentSystem PXEndianBig
#endif

//typedef PXInt32U PXDataType;

PXPublic void PXDataTypeToString(const PXInt32U dataType, char* buffer);

PXPublic PXInt32U PXDataTypeIntFitting(const PXSize expectedSize);

PXPublic void PXEndianSwapI32U(PXInt32U* const value);
PXPublic void PXEndianSwapI16U(PXInt16U* const value);


PXPublic void PXEndianSwap(void* const data, const PXSize dataSize, const PXEndian endianFrom, const PXEndian endianTo);

PXPublic void PXEndianSwapV(void** const data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo);

//-----------------------------------------------------------------------------

#endif