#ifndef MathInclude
#define MathInclude

#include <Format/Type.h>

#define MathConstantPI 3.1415926535897932384626433832 	// Pi, Archimedes' constant or Ludolph's number
#define MathConstantE 2.71828182845904523536028747135266249
#define MathConstantP 1.32471795724474602596090885447809734
#define MathConstantA 2.50290787509589282228390287321821578
#define MathConstantGravity 0.00000000006673

#define GravityOnVenus 8.87
#define GravityOnEarth 9.807
#define GravityOnMoon 1.62
#define GravityOnMars 3.711


#define MathMinimum(a, b) (a <= b ? a : b)
#define MathMaximum(a, b) (a >= b ? a : b)
#define MathFloor(a) ((int)(a))
//#define MathCeiling(a)
#define MathAbsolute(a) (a < 0 ? -a : a)

// Limit v between minimum and maximum
#define MathLimit(v, min, max)  (min*(v <= min) + max*(v >= max) + v *((v > min) && (v < max)));

#ifdef __cplusplus
extern "C"
{
#endif
	CPublic int MathMinimumI(const int a, const int b);
	CPublic size_t MathMinimumIU(const size_t a, const size_t b);
	CPublic double MathMinimumD(const double a, const double b);

	CPublic int MathFloorI(const int a);
	CPublic int MathFloorD(const double a);

	CPublic size_t MathAbsoluteI(const int value);
	CPublic double MathAbsoluteD(const double value);


	CPublic char MathLimitC(const char value, const char minimum, const char maximum);
	CPublic unsigned char MathLimitCU(const unsigned char  value, const unsigned char  minimum, const unsigned char maximum);
	CPublic int MathLimitI(const int value, const int minimum, const int maximum);
	CPublic size_t MathLimitIU(const size_t value, const size_t minimum, const size_t maximum);

	CPublic float MathFastInverseSqaureRoot(float number);
	CPublic double MathPower(double base, double exponent);
	CPublic double MathPowerOfTwo(double base);
	CPublic unsigned int MathPowerModulo(unsigned int base, unsigned int exponent, unsigned int modulo);
	CPublic double MathRoot(unsigned int rootNr, double value);
	CPublic double MathSquareRoot(double value);
	CPublic double MathCubicRoot(double value);
	CPublic double MathPythagoras(double a, double b);
	CPublic double MathPythagorasReverse(double c, double a);
	CPublic double MathLogarithmus(int base, double exponent);
	CPublic double MathLogarithmusBase2(double exponent);
	CPublic double MathLogarithmusBase10(double exponent);
	CPublic double MathNewtonGravitation(double massA, double massB, double distance);
	CPublic int MathCeilingF(const float value);
	CPublic int MathCeilingD(const double value);
	CPublic unsigned int MathRandomeNumber();
	CPublic double MathSinus(double value);
	CPublic double MathTangens(double value);
	CPublic double MathCosinus(double value);
	CPublic unsigned long MathFibonacci(unsigned long step);
	CPublic double MathRadiansToDegree(double radians);
	CPublic double MathDegreeToRadians(double degree);

#ifdef __cplusplus
}
#endif

#endif
