#ifndef PXTypeINCLUDE
#define PXTypeINCLUDE

//#include <stddef.h>

#include <PX/OS/System/Version/PXOSVersion.h>

#include <wchar.h>

//-----------------------------------------------------------------------------
#define PXDefaultLibraryEnable 1
//-----------------------------------------------------------------------------





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
#define PXCVersion 201703L

#if (PXCVersion >= 199711L)
#define CVersionAtleast1998 1
#else
#define CVersionAtleast1998 0
#endif

#if (PXCVersion >= 201103L)
#define CVersionNewerThen2011 1
#else
#define CVersionNewerThen2011 0
#endif

#if (PXCVersion >= 201402L)
#define CVersionNewerThen2014 1
#else
#define CVersionNewerThen2014 0
#endif

#if (PXCVersion >= 201703L)
#define CVersionNewerThen2017 1
#else
#define CVersionNewerThen2017 0
#endif

#if (PXCVersion >= 202002L)
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

#define PXYes 1u
#define PXNo 0u
#define PXTrue 1u
#define PXFalse 0u
#define PXNull 0u



//---------------------------------------------------------
// Flags
//---------------------------------------------------------
#define PXFlagIsSet(value, flagID) (1u && (value & flagID))
#define PXFlagAdd(target, value, flagID) target |= flagID
#define PXFlagRemove(target, value, flagID)  target &= ~flagID




#define PXTypeReadOnlyMask          0b10000000000000000000000000000000
#define PXTypeAdressMask            0b01000000000000000000000000000000 // Used if the type is
#define PXTypeEndianMask            0b00110000000000000000000000000000 // Little or big endian. If No endian is spesified, we can just copy 1:1
#define PXTypeUseFileModeMask       0b00000000000000000000000000000000
#define PXTypeSignedMask            0b00001000000000000000000000000000 // Only useful in numeric values
#define PXTypeIgnoreIFMask          0b00000110000000000000000000000000
#define PXTypeAvoidTransphere       0b00000001000000000000000000000000 // Set if you want to read or write (To support Padding)

#define PXTypeTypeMask              0b00000000110000000000000000000000 // Define type
#define PXTypeBaseText              0b00000000000000000000000000000000 // Bytes are "as is"
#define PXTypeBaseNumeric           0b00000000010000000000000000000000 // int
#define PXTypeBaseDecimal           0b00000000100000000000000000000000 // PXF32 or double
#define PXTypeBaseBit               0b00000000110000000000000000000000 // Bits are used instead of bytes


// Array 
#define PXTypeIsArray               0b00000000001000000000000000000000 // Is this an array of given type?
#define PXTypeArraySize             0b00000000000000001111111100000000 // Is this an array of given type?


#define PXTypeVarriantMask          0b00000000000010000000000000000000

// When we want to parse, we often need to check against a fixed symbol 
// like a signature, so we dnt actually care about reading it, just 
// check and store it as an 1-Byte bool
#define PXTypeSignatureCheck        0b00000000000010000000000000000000

// The size of the recieving data
// This needs to be known if you want to load 5 Bytes into a 64Bit Integer
#define PXTypeReciverSizeMask       0b00000000000001100000000000000000
#define PXTypeReciverSize08U        0b00000000000000000000000000000000 // 1 Bytes
#define PXTypeReciverSize16U        0b00000000000000100000000000000000 // 2 Bytes
#define PXTypeReciverSize32U        0b00000000000001000000000000000000 // 4 Bytes
#define PXTypeReciverSize64U        0b00000000000001100000000000000000 // 8 Bytes
#define PXTypeReciverSizeGet(x)     ((PXTypeReciverSizeMask & x) >> 16)

#define PXTypeSizeMask              0b00000000000000001111111111111111 // Size in bytes of target data 0x0000FFFF
#define PXTypeSizeGet(x)            (PXTypeSizeMask & x) 

#define PXTypeIgnoreIn32B           0b00000010000000000000000000000000
#define PXTypeIgnoreIn64B           0b00000100000000000000000000000000

#define PXTypeDirect                0b00000000000000000000000000000000
#define PXTypeEndianBig             0b00010000000000000000000000000000
#define PXTypeEndianLittle          0b00100000000000000000000000000000
#define PXTypeEndianGet(x)          ((PXTypeEndianMask & x) >> 28) 

#define PXTypeSigned    PXTypeSignedMask
#define PXTypeUnsigned  0

#define PXTypeSize00    0
#define PXTypeSize08    1
#define PXTypeSize16    2
#define PXTypeSize32    4
#define PXTypeSize64    8
#define PXTypeSize128   16

#define PXTypeText(size) size | PXTypeBaseText

#define PXTypeReadOnly PXTypeReadOnlyMask

//-------------------------------------------------
// Adress, read as spesified but store it as (void*)
//-------------------------------------------------
#define PXTypeAdress08      PXTypeAdressMask | PXTypeSize08
#define PXTypeAdress16      PXTypeAdressMask | PXTypeSize16
#define PXTypeAdress32      PXTypeAdressMask | PXTypeSize32
#define PXTypeAdress64      PXTypeAdressMask | PXTypeSize64
#define PXTypeAdressFlex    PXTypeAdressMask
#define PXTypeAdress        PXTypeAdressMask | sizeof(void*)
#define PXTypeString        PXTypeAdress

//-------------------------------------------------
// Text - used to parse singatures
//-------------------------------------------------
#define PXTypeDatax2 PXTypeSize16
#define PXTypeDatax4 PXTypeSize32
#define PXTypeDatax8 PXTypeSize64
#define PXTypeDatax16 PXTypeSize128

//-------------------------------------------------
// Empty space
//-------------------------------------------------
#define PXTypePadding(size) size | PXTypeAvoidTransphere


//-------------------------------------------------
// Int - Normal
//-------------------------------------------------
#define PXTypeIntS      PXTypeBaseNumeric | PXTypeSigned
#define PXTypeIntU      PXTypeBaseNumeric | PXTypeUnsigned

#define PXTypeIntSLE    PXTypeIntS | PXTypeEndianLittle
#define PXTypeIntSBE    PXTypeIntS | PXTypeEndianBig

#define PXTypeIntULE    PXTypeIntU | PXTypeEndianLittle
#define PXTypeIntUBE    PXTypeIntU | PXTypeEndianBig

#define PXTypeBoolAsText 

// Integer 8-Bit
#define PXTypeInt08S    PXTypeSize08 | PXTypeIntS
#define PXTypeInt08U    PXTypeSize08 | PXTypeIntU
#define PXTypeBool PXTypeInt08U

// Integer 16-Bit
#define PXTypeInt16S    PXTypeSize16 | PXTypeIntS
#define PXTypeInt16SLE  PXTypeSize16 | PXTypeIntSLE
#define PXTypeInt16SBE  PXTypeSize16 | PXTypeIntSBE
#define PXTypeInt16U    PXTypeSize16 | PXTypeIntU
#define PXTypeInt16ULE  PXTypeSize16 | PXTypeIntULE
#define PXTypeInt16UBE  PXTypeSize16 | PXTypeIntUBE

// Integer 32-Bit
#define PXTypeInt32S    PXTypeSize32 | PXTypeIntS
#define PXTypeInt32SLE  PXTypeSize32 | PXTypeIntSLE
#define PXTypeInt32SBE  PXTypeSize32 | PXTypeIntSBE
#define PXTypeInt32U    PXTypeSize32 | PXTypeIntU
#define PXTypeInt32ULE  PXTypeSize32 | PXTypeIntULE
#define PXTypeInt32UBE  PXTypeSize32 | PXTypeIntUBE

// Integer 64-Bit
#define PXTypeInt64U    PXTypeSize64 | PXTypeIntS
#define PXTypeInt64ULE  PXTypeSize64 | PXTypeIntULE
#define PXTypeInt64UBE  PXTypeSize64 | PXTypeIntUBE
#define PXTypeInt64S    PXTypeSize64 | PXTypeIntU
#define PXTypeInt64SLE  PXTypeSize64 | PXTypeIntULE
#define PXTypeInt64SBE  PXTypeSize64 | PXTypeIntUBE

// Integer 128-Bit
#define PXTypeInt128U    PXTypeSize128 | PXTypeIntS
#define PXTypeInt128ULE  PXTypeSize128 | PXTypeIntULE
#define PXTypeInt128UBE  PXTypeSize128 | PXTypeIntUBE
#define PXTypeInt128S    PXTypeSize128 | PXTypeIntU
#define PXTypeInt128SLE  PXTypeSize128 | PXTypeIntULE
#define PXTypeInt128SBE  PXTypeSize128 | PXTypeIntUBE

// Float 16-Bit
#define PXTypeF16 PXTypeSize16 | PXTypeBaseDecimal

// PXF32 32-Bit
#define PXTypeF32 PXTypeSize32 | PXTypeBaseDecimal

// PXF32 64-Bit
#define PXTypeF64 PXTypeSize64 | PXTypeBaseDecimal

// System depended
#define PXTypeSize PXTypeInt64U

// Conditional
#define PXTypeInt32ULEOnlyIf32B PXTypeInt32ULE | PXTypeIgnoreIn32B
#define PXTypeInt32ULEOnlyIf64B PXTypeInt32ULE | PXTypeIgnoreIn64B

#define PXTypeIntFlexLE | PXTypeBaseNumeric | PXTypeEndianLittle
#define PXTypeIntFlexBE | PXTypeBaseNumeric | PXTypeEndianBig





//------------------------------------------------------------------------------
// Int - BitMode
//------------------------------------------------------------------------------
#define PXTypeBit08U(bitSize) PXTypeReciverSize08U | PXTypeBaseBit | bitSize
#define PXTypeBit16U(bitSize) PXTypeReciverSize16U | PXTypeBaseBit | bitSize
#define PXTypeBit32U(bitSize) PXTypeReciverSize32U | PXTypeBaseBit | bitSize
#define PXTypeBit64U(bitSize) PXTypeReciverSize64U | PXTypeBaseBit | bitSize

#define PXTypeNibble PXTypeBit08U(4)


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
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// PXF32ing point units
// Calculated by : 
// ((-1) * sign) * 2^(exponent-bias) * (1 + mantissa)
// Bias is calulated: 2^(exponent-bit-size - 1) - 1
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IEEE 754 - Half-precision PXF32ing-point
typedef unsigned short PXF16; 
// Most compilers do not have symbols for this type
#define PXF16MaskSign       0b1000000000000000u //  1-Bit
#define PXF16MaskExponent   0b0111110000000000u //  5-Bit
#define PXF16MaskFraction   0b0000001111111111u // 10-Bit
#define PXF16Bias           15 // = 2^(5-1)-1
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IEEE 754 - Single-precision PXF32ing-point
typedef float PXF32;
#define PXF32MaskSign       0b10000000000000000000000000000000u //  1-Bit
#define PXF32MaskExponent   0b01111111100000000000000000000000u //  8-Bit
#define PXF32MaskFraction   0b00000000011111111111111111111111u // 23-Bit
#define PXF32Bias           127 // = 2^(8-1)-1
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IEEE 754 - Double-precision PXF32ing-point
typedef double PXF64;
#define PXF64MaskSign       0b1000000000000000000000000000000000000000000000000000000000000000u //  1-Bit
#define PXF64MaskExponent   0b0111111111110000000000000000000000000000000000000000000000000000u // 11-Bit
#define PXF64MaskFraction   0b0000000000001111111111111111111111111111111111111111111111111111u // 52-Bit
#define PXF64Bias           1023 // = 2^(11-1)-1
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IEEE 754 - Quadruple-precision PXF32ing-point
//typedef _PXF32128 F128;// long double sometimes used for this but can also mean 80-Bit
#define PXF128MaskSign      0b0 //   1-Bit
#define PXF128MaskExponent  0b0 //  15-Bit
#define PXF128MaskFraction  0b0 // 112-Bit
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// IEEE 754 - Octuple-precision PXF32ing-point
typedef int PXF256;
#define PXF256MaskSign      0b0 //   1-Bit
#define PXF256MaskExponent  0b0 //  19-Bit
#define PXF256MaskFraction  0b0 // 236-Bit
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Text
//------------------------------------------------------------------------------
typedef char PXCharASCII;
typedef wchar_t PXCharUNICODE;
typedef PXInt32U PXCharUTF8;

typedef char* const PXTextASCII;
typedef char* const PXTextUTF8;
typedef wchar_t* const PXTextUNICODE;
//------------------------------------------------------------------------------





typedef struct PXTypeEntry_
{
    void* Adress;
    PXInt32U Type;
}
PXTypeEntry;


// Object to bind a runtime member adress of a struct with a name with the type
typedef struct PXTypeBinding_
{
    void* Adress;
    const char* Name;
    PXInt32U Type;
}
PXTypeBinding;





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








// Helper object to calculate how many steps we 
// need to crunsh through numbers
typedef struct PXWorkSetCounter_
{
    int WorkToDo; // Amount of work that needs to be processed
    int BatchSize; // Amount of work that can be done in one rotation

    int AmountBatchFull; // Amount of batches that can be compleated fully
    int AmountBatchRest; // rest amount of work that cannot be batched on its own and needs padding.
}
PXWorkSetCounter;

PXPublic void PXAPI PXWorkSetCounterCalc(PXWorkSetCounter* const pxWorkSetCounter);
PXPublic PXSize PXAPI PXWorkSetCounterPull(PXWorkSetCounter* const pxWorkSetCounter, const PXSize index);






//typedef PXInt32U PXType;

PXPublic void PXAPI PXTypeToString(const PXInt32U dataType, char* buffer);

PXPublic PXInt32U PXAPI PXTypeIntFitting(const PXSize expectedSize);

PXPublic void PXAPI PXEndianSwapI32U(PXInt32U* const value);
PXPublic void PXAPI PXEndianSwapI16U(PXInt16U* const value);


PXPublic void PXAPI PXEndianSwap(void* const data, const PXSize dataSize, const PXEndian endianFrom, const PXEndian endianTo);

PXPublic void PXAPI PXEndianSwapV(void** const data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo);

//-----------------------------------------------------------------------------

#endif
