#ifndef PXTypeIncluded
#define PXTypeIncluded

//#include <stddef.h>

#include <PX/OS/System/Version/PXOSVersion.h>

#include <wchar.h>

//-----------------------------------------------------------------------------
#define PXDefaultLibraryEnable 1
//-----------------------------------------------------------------------------


#ifdef NDEBUG
#define PXDEBUG 0
#else
#define PXDEBUG 1
#endif



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
#define PXFASTCALL _fastcall

#endif

#define PXAPI PXFASTCALL

//---------------------------------------------------------
// Change label of functions to C-Style (C++ only)
//---------------------------------------------------------
#ifdef __cplusplus
#define PXFunction extern "C"
#else
#define PXFunction extern
#endif
//---------------------------------------------------------

#define PXPrivate static
#define PXInternal

#if OSUnix
#define PXPublic PXFunction extern
#define PXMSHandle void*
#elif OSWindows
#define PXDLLExport __declspec(dllexport)
#define PXPublic PXFunction PXDLLExport // The visual studio compiler also wants this definition, for microsoft stuff.
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


#ifdef __cplusplus
#define PXYes true
#define PXNo false
#define PXTrue true
#define PXFalse false
#define PXNull nullptr
#else
#define PXYes 1u
#define PXNo 0u
#define PXTrue 1u
#define PXFalse 0u
#define PXNull 0u
#endif




//---------------------------------------------------------
// Flags
//---------------------------------------------------------
#define PXFlagIsSet(value, flagID) (1u && (value & flagID))
#define PXFlagAdd(target, value, flagID) target |= flagID
#define PXFlagRemove(target, value, flagID)  target &= ~flagID



//---------------------------------------------------------
// Type
//---------------------------------------------------------

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

// Float 32-Bit
#define PXTypeF32 PXTypeSize32 | PXTypeBaseDecimal
#define PXTypeF32List(amount) (amount*PXTypeSize32) | PXTypeBaseDecimal

// Float 64-Bit
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

#ifdef __cplusplus
typedef bool PXBool;
#else
typedef unsigned char PXBool;
#endif

typedef unsigned char* PXAdress;

// Integer 8-Bit
typedef char PXI8S;
typedef unsigned char PXI8U;

// Integer 16-Bit, WORD
typedef short PXI16S;
typedef unsigned short PXI16U;

// Integer 32-Bit, DWORD
typedef int PXI32S;
typedef unsigned int PXI32U;

// Integer 64-Bit, QWORD
#if OSUnix
typedef long long PXI64S;
typedef unsigned long long PXI64U;
#elif OSWindows
typedef __int64 PXI64S;
typedef unsigned __int64 PXI64U;
#endif // OSUnix

#if OS32B
typedef PXI32U PXSize;
typedef PXI32S PXOffset;
typedef PXI64U PXAdress64;
#elif OS64B
typedef PXI64U PXSize;
typedef PXI64S PXOffset;
typedef void* PXAdress64;
#else
#error Invalid Bit Version
#endif

// Integer 128-Bit
//typedef char[16] PXI128S;
//typedef char[16] PXI128U;

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


// Internal datatype
typedef PXI32U PXType;




//------------------------------------------------------------------------------
// Text
//------------------------------------------------------------------------------
#if 0
typedef char PXCharASCII;
typedef wchar_t PXCharUNICODE;
typedef PXI32U PXCharUTF8;

typedef char* const PXASCII;
typedef char* const PXTextUTF8;
typedef wchar_t* const PXTextUNICODE;
#endif
//------------------------------------------------------------------------------





typedef struct PXTypeEntry_
{
    void* Adress;
    PXI32U Type;
}
PXTypeEntry;


// Object to bind a runtime member adress of a struct with a name with the type
typedef struct PXTypeBinding_
{
    void* Adress;
    const char* Name;
    PXI32U Type;
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
#define PXI16SplittEndianLittle(number, a, b) \
    a = (number & 0xFF); \
    b = (number & 0xFF00) >> 8u;

#define PXI16SplittEndianBig(number, a, b) \
    a = (number & 0xFF00) >> 8u; \
    b = (number & 0xFF);


#define PXI16MakeEndianLittle(a, b) (\
    ((PXI16U)a) | \
    ((PXI16U)b << 8u))

#define PXI16MakeEndianBig(a, b) (\
    ((PXI16U)a << 8u) | \
    ((PXI16U)b))

#define PXI16FromAdressEndianLittle(list) PXI16MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1])
#define PXI16FromAdressEndianBig(list) PXI16MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1])
//---------------------------------------------------------

//---------------------------------------------------------
// 24-Bit int
//---------------------------------------------------------
#define PXI24MakeEndianLittle(a, b, c) (\
    ((PXI32U)a) | \
    ((PXI32U)b << 8u) | \
    ((PXI32U)c << 16u))

#define PXI24MakeEndianBig(a, b, c) (\
    ((PXI32U)b << 16u) | \
    ((PXI32U)c << 8u) | \
    ((PXI32U)d))


#define PXI24FromAdressEndianLittle(list) PXI24MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2])
#define PXI24FromAdressEndianBig(list) PXI24MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2])
//---------------------------------------------------------

//---------------------------------------------------------
// 32-Bit int
//---------------------------------------------------------
#define PXI32MakeEndianLittle(a, b, c, d) (\
    ((PXI32U)a) | \
    ((PXI32U)b << 8u) | \
    ((PXI32U)c << 16u) | \
    ((PXI32U)d << 24u))

#define PXI32MakeEndianBig(a, b, c, d) (\
    ((PXI32U)a << 24u) | \
    ((PXI32U)b << 16u) | \
    ((PXI32U)c << 8u) | \
    ((PXI32U)d))

#define PXI32FromAdressEndianLittle(list) PXI32MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3])
#define PXI32FromAdressEndianBig(list) PXI32MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3])
//---------------------------------------------------------

//---------------------------------------------------------
// 40-Bit int
//---------------------------------------------------------
#define PXI40MakeEndianLittle(a, b, c, d, e) (\
    ((PXI64U)a | \
    ((PXI64U)b << 8u) | \
    ((PXI64U)c << 16u) | \
    ((PXI64U)d << 24u) | \
    ((PXI64U)e << 32u)))

#define PXI40MakeEndianBig(a, b, c, d, e) (\
    ((PXI64U)a << 32u) | \
    ((PXI64U)b << 24u) | \
    ((PXI64U)c << 16u) | \
    ((PXI64U)d << 8u) | \
    ((PXI64U)e)))

#define PXI40FromAdressEndianLittle(list) PXI40MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4])
#define PXI40FromAdressEndianBig(list) PXI40MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4])
//---------------------------------------------------------

//---------------------------------------------------------
// 48-Bit int
//---------------------------------------------------------
#define PXI48MakeEndianLittle(a, b, c, d, e, f) (\
    ((PXI64U)a | \
    ((PXI64U)b << 8u) | \
    ((PXI64U)c << 16u) | \
    ((PXI64U)d << 24u) | \
    ((PXI64U)e << 32u) | \
    ((PXI64U)f << 40u)))

#define PXI48MakeEndianBig(a, b, c, d, e, f) (\
    ((PXI64U)a << 40u) | \
    ((PXI64U)b << 32u) | \
    ((PXI64U)c << 24u) | \
    ((PXI64U)d << 16u) | \
    ((PXI64U)e << 8u) | \
    ((PXI64U)f)))

#define PXI48FromAdressEndianLittle(list) PXI48MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5])
#define PXI48FromAdressEndianBig(list) PXI48MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5])
//---------------------------------------------------------

//---------------------------------------------------------
// 56-Bit int
//---------------------------------------------------------
#define PXI56MakeEndianLittle(a, b, c, d, e, f, g) (\
    ((PXI64U)a | \
    ((PXI64U)b << 8u) | \
    ((PXI64U)c << 16u) | \
    ((PXI64U)d << 24u) | \
    ((PXI64U)e << 32u) | \
    ((PXI64U)f << 40u) | \
    ((PXI64U)g << 48u)))

#define PXI56MakeEndianBig(a, b, c, d, e, f, g) (\
    ((PXI64U)a << 48u) | \
    ((PXI64U)b << 40u) | \
    ((PXI64U)c << 32u) | \
    ((PXI64U)d << 24u) | \
    ((PXI64U)e << 16u) | \
    ((PXI64U)f << 8u) | \
    ((PXI64U)g)))

#define PXI56FromAdressEndianLittle(list) PXI56MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6])
#define PXI56FromAdressEndianBig(list) PXI56MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6])
//---------------------------------------------------------

//---------------------------------------------------------
// 64-Bit int
//---------------------------------------------------------
#define PXI64MakeEndianLittle(a, b, c, d, e, f, g, h) (\
    ((PXI64U)a | \
    ((PXI64U)b << 8u) | \
    ((PXI64U)c << 16u) | \
    ((PXI64U)d << 24u) | \
    ((PXI64U)e << 32u) | \
    ((PXI64U)f << 40u) | \
    ((PXI64U)g << 48u) | \
    ((PXI64U)h << 56u)))

#define PXI64MakeEndianBig(a, b, c, d, e, f, g, h) (\
    ((PXI64U)a << 56u) | \
    ((PXI64U)b << 48u) | \
    ((PXI64U)c << 40u) | \
    ((PXI64U)d << 32u) | \
    ((PXI64U)e << 24u) | \
    ((PXI64U)f << 16u) | \
    ((PXI64U)g << 8u) | \
    ((PXI64U)h)))

#define PXI64FromAdressEndianLittle(list) PXI64MakeEndianLittle(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6], ((PXAdress)list)[7])
#define PXI64FromAdressEndianBig(list) PXI64MakeEndianBig(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6], ((PXAdress)list)[7])
//---------------------------------------------------------

//---------------------------------------------------------
// Direct int make/create depending on OS
//---------------------------------------------------------
#if OSEngianLittle
#define PXI16Make PXI16MakeEndianLittle
#define PXI24Make PXI24MakeEndianLittle
#define PXI32Make PXI32MakeEndianLittle
#define PXI40Make PXI40MakeEndianLittle
#define PXI48Make PXI48MakeEndianLittle
#define PXI56Make PXI56MakeEndianLittle
#define PXI64Make PXI64MakeEndianLittle

#define PXI16FromAdress PXI16FromAdressEndianLittle
#define PXI24FromAdress PXI24FromAdressEndianLittle
#define PXI32FromAdress PXI32FromAdressEndianLittle
#define PXI40FromAdress PXI40FromAdressEndianLittle
#define PXI48FromAdress PXI48FromAdressEndianLittle
#define PXI56FromAdress PXI56FromAdressEndianLittle
#define PXI64FromAdress PXI64FromAdressEndianLittle
#elif OSEngianBig
#define PXI16Make PXI16MakeEndianBig
#define PXI24Make PXI24MakeEndianBig
#define PXI32Make PXI32MakeEndianBig
#define PXI40Make PXI40MakeEndianBig
#define PXI48Make PXI48MakeEndianBig
#define PXI56Make PXI56MakeEndianBig
#define PXI64Make PXI64MakeEndianBig

#define PXI16FromAdress PXI16FromAdressEndianBig
#define PXI24FromAdress PXI24FromAdressEndianBig
#define PXI32FromAdress PXI32FromAdressEndianBig
#define PXI40FromAdress PXI40FromAdressEndianBig
#define PXI48FromAdress PXI48FromAdressEndianBig
#define PXI56FromAdress PXI56FromAdressEndianBig
#define PXI64FromAdress PXI64FromAdressEndianBig
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

typedef struct PXI16SCluster
{
    union
    {
        struct
        {
            PXByte A;
            PXByte B;
        };

        PXByte Data[2];

        PXI16S Value;
    };
}
PXI16SCluster;

typedef struct PXI16UCluster
{
    union
    {
        struct
        {
            PXByte A;
            PXByte B;
        };

        PXByte Data[2];

        PXI16S Value;
    };
}
PXI16UCluster;

typedef struct PXI32SCluster_
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

        PXI32S Value;
    };
}
PXI32SCluster;

typedef struct PXI32UCluster_
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

        PXI32U Value;
    };
}
PXI32UCluster;

typedef struct PXI64SCluster_
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

        PXI64S Value;
    };
}
PXI64SCluster;

typedef struct PXI64UCluster_
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

        PXI64U Value;
    };
}
PXI64UCluster;


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






//typedef PXI32U PXType;

PXPublic void PXAPI PXTypeToString(const PXI32U dataType, char* buffer);

PXPublic PXI32U PXAPI PXTypeIntFitting(const PXSize expectedSize);

PXPublic void PXAPI PXEndianSwapI32U(PXI32U* const value);
PXPublic void PXAPI PXEndianSwapI16U(PXI16U* const value);


PXPublic void PXAPI PXEndianSwap(void* const data, const PXSize dataSize, const PXEndian endianFrom, const PXEndian endianTo);

PXPublic void PXAPI PXEndianSwapV(void** const data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo);

//-----------------------------------------------------------------------------

#endif
