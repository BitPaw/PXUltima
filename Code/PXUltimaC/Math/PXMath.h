#ifndef PXMathINCLUDE
#define PXMathINCLUDE

#include <Media/PXType.h>

//---<Constants>---------------------------------------------------------------
#define PXMathConstantPI 3.1415926535897932384626433832 	// Pi, Archimedes' constant or Ludolph's number
#define PXMathConstantE 2.71828182845904523536028747135266249
#define PXMathConstantP 1.32471795724474602596090885447809734
#define PXMathConstantA 2.50290787509589282228390287321821578
#define PXMathConstantGravity 0.00000000006673
//-----------------------------------------------------------------------------

//---<Makro Functions>---------------------------------------------------------
#define PXMathIsInRange(v, minimum, maximum) (minimum <= v && v <= maximum)
#define PXMathMinimum(a, b) ((a < b) * a + (a >= b) * b)
#define PXMathMaximum(a, b) ((a > b) * a + (a <= b) * b)
#define PXMathLimit(x, low, high)  (low*(x <= low) + high*(x >= high) + x *((x > low) && (x < high)))
#define PXMathFloor(a) ((int)(a))
//#define PXMathCeiling(a)


//#define PXMathAbsolute(a) (a * ((a >= 0) - (a < 0)))
#define PXMathAbsolute(x) ((x) < 0 ? -(x) : (x))

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMathRandomGeneratorSeed_
	{
		PXInt32U X;
		PXInt32U Y;
		PXInt32U Z;
	}
	PXMathRandomGeneratorSeed;

	PXPublic int PXAPI PXMathMinimumI(const int a, const int b);
	PXPublic PXSize PXAPI PXMathMinimumIU(const PXSize a, const PXSize b);
	PXPublic double PXAPI PXMathMinimumD(const double a, const double b);

	PXPublic PXSize PXAPI PXMathMaximumIU(const PXSize a, const PXSize b);

	PXPublic int PXAPI PXMathFloorI(const int a);
	PXPublic int PXAPI PXMathFloorD(const double a);

	PXPublic PXSize PXAPI PXMathAbsoluteI16(const PXInt16S value);
	PXPublic PXSize PXAPI PXMathAbsoluteI32(const PXInt32S value);
	PXPublic double PXAPI PXMathAbsoluteD(const double value);


	PXPublic char PXAPI PXMathLimitC(const char value, const char minimum, const char maximum);
	PXPublic unsigned char PXAPI PXMathLimitCU(const unsigned char  value, const unsigned char  minimum, const unsigned char maximum);
	PXPublic int PXAPI PXMathLimitI(const int value, const int minimum, const int maximum);
	PXPublic PXSize PXAPI PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum);

	PXPublic float PXAPI PXMathFastInverseSqaureRoot(float number);
	PXPublic double PXAPI PXMathPower(double base, double exponent);
	PXPublic double PXAPI PXMathPowerOfTwo(double base);
	PXPublic PXInt32U PXAPI PXMathPowerModulo(const PXInt32U base, const PXInt32U exponent, const PXInt32U modulo);
	PXPublic double PXAPI PXMathRoot(unsigned int rootNr, double value);
	PXPublic double PXAPI PXMathSquareRoot(double value);
	PXPublic double PXAPI PXMathCubicRoot(double value);
	PXPublic double PXAPI PXMathPythagoras(double a, double b);
	PXPublic double PXAPI PXMathPythagorasReverse(double c, double a);
	PXPublic double PXAPI PXMathLogarithmus(int base, double exponent);
	PXPublic double PXAPI PXMathLogarithmusBase2(double exponent);
	PXPublic double PXAPI PXMathLogarithmusBase10(double exponent);
	PXPublic double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance);
	PXPublic int PXAPI PXMathCeilingF(const float value);
	PXPublic int PXAPI PXMathCeilingD(const double value);
	PXPublic void PXAPI PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);
	PXPublic PXInt32U PXAPI PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);
	PXPublic double PXAPI PXMathSinus(double value);
	PXPublic double PXAPI PXMathTangens(double value);
	PXPublic double PXAPI PXMathCosinus(double value);
	PXPublic unsigned long PXAPI PXMathFibonacci(unsigned long step);
	PXPublic double PXAPI PXMathRadiansToDegree(double radians);
	PXPublic double PXAPI PXMathDegreeToRadians(double degree);

	// Liniar interpolate a value from the range of 'x' min and max to another range 'y'.
	PXPublic float PXAPI PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue);
	PXPublic float PXAPI PXMathNormalizeF(const float minimum, const float maximum, const float value);
	PXPublic unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value);

#ifdef __cplusplus
}
#endif

#endif
