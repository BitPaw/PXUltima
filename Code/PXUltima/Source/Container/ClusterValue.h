#ifndef ClusterValueDEFINED
#define ClusterValueDEFINED


// Detect current endianess on this system and then use #if
#define MakeShortFromAdress(list) MakeShortLE(list[0], list[1])
#define MakeShort(a, b) MakeShortLE(a, b)

#define MakeShortBE(a, b) (((unsigned short)a << 8) | ((unsigned short)b))
#define MakeShortLE(a, b) (((unsigned short)a) | ((unsigned short)b << 8))


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

#define PXInt8FromAdress(list) PXInt8Make(((PXAdress)list)[0])
#define PXInt16FromAdress(list) PXInt16Make(((PXAdress)list)[0], ((PXAdress)list)[1])
#define PXInt24FromAdress(list) PXInt24Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2])
#define PXInt32FromAdress(list) PXInt32Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3])

#define PXInt40FromAdress(list) PXInt40Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4])
#define PXInt48FromAdress(list) PXInt48Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5])
#define PXInt56FromAdress(list) PXInt56Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6])
#define PXInt64FromAdress(list) PXInt64Make(((PXAdress)list)[0], ((PXAdress)list)[1], ((PXAdress)list)[2], ((PXAdress)list)[3], ((PXAdress)list)[4], ((PXAdress)list)[5], ((PXAdress)list)[6], ((PXAdress)list)[7])

#define PXInt8Make(a) MakeInt(a, 0, 0, 0)
#define PXInt16Make(a, b) MakeInt(a, b, 0, 0)
#define PXInt24Make(a, b, c) MakeInt(a, b, c, 0)
#define PXInt32Make(a, b, c, d) MakeInt(a, b, c, d)

#define PXInt40Make(a, b, c, d, e) MakeLongLong(a, b, c, d, e, 0,0,0)
#define PXInt48Make(a, b, c, d, e, f) MakeLongLong(a, b, c, d, e, f, 0, 0)
#define PXInt56Make(a, b, c, d, e, f, g) MakeLongLong(a, b, c, d, e, f, g, 0)
#define PXInt64Make(a, b, c, d, e, f, g, h) MakeLongLong(a, b, c, d, e, f, g, h)


#define MakeInt(a, b, c, d) MakeIntLE(a, b, c, d)
#define MakeIntBE(a, b, c, d) (((unsigned int)a << 24) | ((unsigned int)b << 16) | ((unsigned int)c << 8) | ((unsigned int)d))
#define MakeIntLE(a, b, c, d) (((unsigned int)a) | ((unsigned int)b << 8) | ((unsigned int)c << 16) | ((unsigned int)d << 24))

#define MakeLongLong(a, b, c, d, e, f, g, h) MakeLongLongLE(a, b, c, d, e, f, g, h)

#define MakeLongLongBE(a, b, c, d, e, f, g, h) \
(((PXInt64U)a << 56LL) | \
((PXInt64U)b << 48LL) | \
((PXInt64U)c << 40LL) | \
((PXInt64U)d << 32LL) | \
((PXInt64U)e << 24LL) | \
((PXInt64U)f << 16LL) | \
((PXInt64U)g << 8LL) | \
((PXInt64U)h))

#define MakeLongLongLE(a, b, c, d, e, f, g, h) \
(((PXInt64U)a | \
((PXInt64U)b << 8LL) | \
((PXInt64U)c << 16LL) | \
((PXInt64U)d << 24LL) | \
((PXInt64U)e << 32LL) | \
((PXInt64U)f << 40LL) | \
((PXInt64U)g << 48LL) | \
((PXInt64U)h << 56LL)))



#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct ClusterShort_
	{
		union
		{
			struct
			{
				unsigned char A;
				unsigned char B;
			};

			unsigned char Data[2];

			unsigned short Value;
		};
	}
	ClusterShort;

	typedef struct ClusterInt_
	{
		union
		{
			struct
			{
				unsigned char A;
				unsigned char B;
				unsigned char C;
				unsigned char D;
			};

			unsigned char Data[4];

			unsigned int Value;
		};
	}
	ClusterInt;

	typedef struct ClusterLongLong_
	{
		union
		{
			struct
			{
				unsigned char A;
				unsigned char B;
				unsigned char C;
				unsigned char D;
				unsigned char E;
				unsigned char F;
				unsigned char G;
				unsigned char H;
			};

			unsigned char Data[8];

			unsigned long long Value;
		};
	}
	ClusterLongLong;	

#ifdef __cplusplus
}
#endif

#endif