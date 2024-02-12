#ifndef PXMathINCLUDE
#define PXMathINCLUDE

#include <Media/PXType.h>

//---<Constants>---------------------------------------------------------------
#define PXMathConstantPI 3.1415926535897932384626433832 	// Pi, Archimedes' constant or Ludolph's number
#define PXMathConstantE 2.71828182845904523536028747135266249
#define PXMathConstantP 1.32471795724474602596090885447809734
#define PXMathConstantA 2.50290787509589282228390287321821578
#define PXMathConstantGravity 0.00000000006673

#define PXMathConstantGravityOnVenus 8.87
#define PXMathConstantGravityOnEarth 9.807
#define PXMathConstantGravityOnMoon 1.62
#define PXMathConstantGravityOnMars 3.711
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

	PXPublic int PXMathMinimumI(const int a, const int b);
	PXPublic PXSize PXMathMinimumIU(const PXSize a, const PXSize b);
	PXPublic double PXMathMinimumD(const double a, const double b);

	PXPublic PXSize PXMathMaximumIU(const PXSize a, const PXSize b);

	PXPublic int PXMathFloorI(const int a);
	PXPublic int PXMathFloorD(const double a);

	PXPublic PXSize PXMathAbsoluteI16(const PXInt16S value);
	PXPublic PXSize PXMathAbsoluteI32(const PXInt32S value);
	PXPublic double PXMathAbsoluteD(const double value);


	PXPublic char PXMathLimitC(const char value, const char minimum, const char maximum);
	PXPublic unsigned char PXMathLimitCU(const unsigned char  value, const unsigned char  minimum, const unsigned char maximum);
	PXPublic int PXMathLimitI(const int value, const int minimum, const int maximum);
	PXPublic PXSize PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum);

	PXPublic float PXMathFastInverseSqaureRoot(float number);
	PXPublic double PXMathPower(double base, double exponent);
	PXPublic double PXMathPowerOfTwo(double base);
	PXPublic unsigned int PXMathPowerModulo(unsigned int base, unsigned int exponent, unsigned int modulo);
	PXPublic double PXMathRoot(unsigned int rootNr, double value);
	PXPublic double PXMathSquareRoot(double value);
	PXPublic double PXMathCubicRoot(double value);
	PXPublic double PXMathPythagoras(double a, double b);
	PXPublic double PXMathPythagorasReverse(double c, double a);
	PXPublic double PXMathLogarithmus(int base, double exponent);
	PXPublic double PXMathLogarithmusBase2(double exponent);
	PXPublic double PXMathLogarithmusBase10(double exponent);
	PXPublic double PXMathNewtonGravitation(double massA, double massB, double distance);
	PXPublic int PXMathCeilingF(const float value);
	PXPublic int PXMathCeilingD(const double value);
	PXPublic void PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);
	PXPublic PXInt32U PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);
	PXPublic double PXMathSinus(double value);
	PXPublic double PXMathTangens(double value);
	PXPublic double PXMathCosinus(double value);
	PXPublic unsigned long PXMathFibonacci(unsigned long step);
	PXPublic double PXMathRadiansToDegree(double radians);
	PXPublic double PXMathDegreeToRadians(double degree);

	// Liniar interpolate a value from the range of 'x' min and max to another range 'y'.
	PXPublic float PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue);
	PXPublic float PXMathNormalizeF(const float minimum, const float maximum, const float value);
	PXPublic unsigned int PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value);

#ifdef __cplusplus
}
#endif

#endif
