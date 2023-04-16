#ifndef PXTypeINCLUDE
#define PXTypeINCLUDE

#include <stddef.h>

#include <OS/System/OSVersion.h>

//-- Reivented public / private. The keywords are reserved, so we need other names.
#if OSUnix
#define PXPrivate static
#define PXPublic extern
#elif OSWindows
#define PXDLLExport __declspec(dllexport)
#define PXPrivate static
#define PXPublic extern PXDLLExport // The visual studio compiler also wants this definition, for microsoft stuff.
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

#ifndef PXNull
#define PXNull 0u
#endif



#ifdef __cplusplus
extern "C"
{
#endif


	typedef enum PXDataType_
	{
		PXDataTypeTypeInvalid,

		PXDataTypeInt8S,
		PXDataTypeInt8U,

		PXDataTypeLEInt16S,
		PXDataTypeLEInt16U,
		PXDataTypeLEInt32S,
		PXDataTypeLEInt32U,
		PXDataTypeLEInt64S,
		PXDataTypeLEInt64U,

		PXDataTypeBEInt16S,
		PXDataTypeBEInt16U,
		PXDataTypeBEInt32S,
		PXDataTypeBEInt32U,
		PXDataTypeBEInt64S,
		PXDataTypeBEInt64U,

		PXDataStreamFloat,
		PXDataStreamDouble
	}
	PXDataType;



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

#if OS32Bit
	typedef PXInt32U PXSize;
#elif OS64Bit
	typedef PXInt64U PXSize;
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

#ifdef __cplusplus
}
#endif

#endif