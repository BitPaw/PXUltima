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

#define PXInt8FromAdress(list) PXInt8Make(list[0])
#define PXInt16FromAdress(list) PXInt16Make(list[0], list[1])
#define PXInt24FromAdress(list) PXInt24Make(list[0], list[1], list[2])
#define PXInt32FromAdress(list) PXInt32Make(list[0], list[1], list[2], list[3])

#define PXInt40FromAdress(list) PXInt40Make(list[0], list[1], list[2], list[3], list[4])
#define PXInt48FromAdress(list) PXInt48Make(list[0], list[1], list[2], list[3], list[4], list[5])
#define PXInt56FromAdress(list) PXInt56Make(list[0], list[1], list[2], list[3], list[4], list[5], list[6])
#define PXInt64FromAdress(list) PXInt64Make(list[0], list[1], list[2], list[3], list[4], list[5], list[6], list[7])

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
(((unsigned long long)a << 56LL) | \
((unsigned long long)b << 48LL) | \
((unsigned long long)c << 40LL) | \
((unsigned long long)d << 32LL) | \
((unsigned long long)e << 24LL) | \
((unsigned long long)f << 16LL) | \
((unsigned long long)g << 8LL) | \
((unsigned long long)h))

#define MakeLongLongLE(a, b, c, d, e, f, g, h) \
(((unsigned long long)a) | \
((unsigned long long)b << 8LL) | \
((unsigned long long)c << 16LL) | \
((unsigned long long)d << 24LL) | \
((unsigned long long)e << 32LL) | \
((unsigned long long)f << 40LL) | \
((unsigned long long)g << 48LL) | \
((unsigned long long)h << 56LL))



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