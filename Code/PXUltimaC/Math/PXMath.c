#include "PXMath.h"

#include <math.h>

int PXAPI PXMathMinimumI(const int a, const int b)
{
	return PXMathMinimum(a, b);
}

PXSize PXAPI PXMathMinimumIU(const PXSize a, const PXSize b)
{
	return PXMathMinimum(a, b);
}

double PXAPI PXMathMinimumD(const double a, const double b)
{
	return PXMathMinimum(a, b);
}

PXSize PXAPI PXMathMaximumIU(const PXSize a, const PXSize b)
{
	return PXMathMaximum(a, b);
}

int PXAPI PXMathFloorI(const int a)
{
	return PXMathFloor(a);
}

int PXAPI PXMathFloorD(const double a)
{
	return PXMathFloor(a);
}

PXSize PXAPI PXMathAbsoluteI16(const PXInt16S value)
{
	return PXMathAbsolute(value);
}

PXSize PXAPI PXMathAbsoluteI32(const PXInt32S value)
{
	return PXMathAbsolute(value);
}

double PXAPI PXMathAbsoluteD(const double a)
{
	return PXMathAbsolute(a);
}

char PXAPI PXMathLimitC(const char value, const char minimum, const char maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

unsigned char PXAPI PXMathLimitCU(const unsigned char value, const unsigned char minimum, const unsigned char maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

int PXAPI PXMathLimitI(const int value, const int minimum, const int maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

PXSize PXAPI PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

float PXAPI PXMathFastInverseSqaureRoot(float number)
{
	const float threehalfs = 1.5f;
	float x2 = number * 0.5f;
	float y = number;
	long i;

	i = *(long*)&y; // Convert the Binary Value from a float to an long. No conversion! A 1:1 Bitcopy. IEEE 574 standard
	i = 0x5f3759df - (i >> 1); // .The bit shift Halfs i. AProximation of (1/root(y))
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// last line can be repeatet for more acuratecy.

	return y;
}

double PXAPI PXMathPower(double base, double exponent)
{
	return pow(base, exponent);
}

double PXAPI PXMathPowerOfTwo(double base)
{
	return PXMathPower(base, 2);
}

PXInt32U PXAPI PXMathPowerModulo(const PXInt32U base, const PXInt32U exponent, const PXInt32U modulo)
{
	PXInt32U result = 1;

	for(PXInt32U i = 0; i < exponent; ++i)
	{
		result *= base;
		result %= modulo;
	}

	return result;
}

double PXAPI PXMathRoot(unsigned int rootNr, double value)
{
	switch(rootNr)
	{
		case 0:
			return 0;

		case 1:
			return 0;

		case 2:
			return sqrt(value);

		case 3:
			return 0;// cbrt(value); // TODO:! !!!

		default:
			return 0;
	}
}

double PXAPI PXMathSquareRoot(double value)
{
	return PXMathRoot(2, value);
}

double PXAPI PXMathCubicRoot(double value)
{
	return PXMathRoot(3, value);
}

double PXAPI PXMathPythagoras(double a, double b)
{
	return PXMathSquareRoot(PXMathPowerOfTwo(a) + PXMathPowerOfTwo(b));
}

double PXAPI PXMathPythagorasReverse(double c, double a)
{
	return 0.0;
}

double PXAPI PXMathLogarithmus(int base, double exponent)
{
#if OSUnix
	return 0; // TODO: not implemented?
#elif OSWindows
	return 0;// _dlog(exponent, base); // TODO: !!!
#endif	
}

double PXAPI PXMathLogarithmusBase2(double exponent)
{
	return 0;// log2(exponent); // TODO: !!!
}

double PXAPI PXMathLogarithmusBase10(double exponent)
{
	return log10(exponent);
}

double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance)
{
	return PXMathConstantGravity * ((massA * massB) / (PXMathPower(distance, 2)));
}

int PXAPI PXMathCeilingF(const float value)
{
	const int fullNumberBlockInt = (int)value;
	const float fullNumberBlock = fullNumberBlockInt;
	const float remaining = value - fullNumberBlock;

	if (remaining)
	{
		return fullNumberBlockInt + 1;
	}

	return fullNumberBlockInt;
}

int PXAPI PXMathCeilingD(const double value)
{
	const int fullNumberBlockInt = (int)value;
	const double fullNumberBlock = fullNumberBlockInt;
	const double remaining = value - fullNumberBlock;

	if (remaining)
	{
		return fullNumberBlockInt + 1;
	}

	return fullNumberBlockInt;
}

void PXAPI PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
{
	pxMathRandomGeneratorSeed->X = 123456789;
	pxMathRandomGeneratorSeed->Y = 362436069;
	pxMathRandomGeneratorSeed->Z = 521288629;
}

PXInt32U PXAPI PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
{
	PXInt32U t;

	pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X << 16;
	pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X >> 5;
	pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X << 1;

	t = pxMathRandomGeneratorSeed->X;
	pxMathRandomGeneratorSeed->X = pxMathRandomGeneratorSeed->Y;
	pxMathRandomGeneratorSeed->Y = pxMathRandomGeneratorSeed->Z;
	pxMathRandomGeneratorSeed->Z = t ^ pxMathRandomGeneratorSeed->X ^ pxMathRandomGeneratorSeed->Y;

	return pxMathRandomGeneratorSeed->Z;
}

double PXAPI PXMathSinus(double value)
{
	return sin(value);
}

double PXAPI PXMathTangens(double value)
{
	return tan(value);
}

double PXAPI PXMathCosinus(double value)
{
	return cos(value);
}

unsigned long PXAPI PXMathFibonacci(unsigned long step)
{
	double rootOfFive = PXMathSquareRoot(5);

	if(step < 2)
	{
		return step;
	}
	else
	{
		// (1 + W/5)^n - (1 - W/5)^n 
		// -------------------------
		//         2^n * W/5

		double a = PXMathPower(1 + rootOfFive, step);
		double b = PXMathPower(1 - rootOfFive, step);
		double c = PXMathPower(2, step) * rootOfFive;

		return ((a - b) / c);

		// return FibonacciRetardVersion(step -1) + FibonacciRetardVersion(step -2); Recusive version
	}
}

double PXAPI PXMathRadiansToDegree(double radians)
{
	return radians * (180.0 / PXMathConstantPI);
}

double PXAPI PXMathDegreeToRadians(double degree)
{
	return degree * (PXMathConstantPI / 180.0);
}

float PXAPI PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue)
{
	// Cap Value
	float xCap = xValue > xMaximum ? xMaximum : xValue;
	xCap = xValue < xMinimum ? xMinimum : xValue;

	// ((ydelta * xValueDelta) / xdelta) + yMinimum;

	return (((yMaximum - yMinimum) * (xValue - xMinimum)) / (xMaximum - xMinimum)) + yMinimum;
}

float PXAPI PXMathNormalizeF(const float minimum, const float maximum, const float value)
{
	return PXMathLiniarF(0, 1, minimum, maximum, value);
}

unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value)
{
	const float result = PXMathLiniarF(0, 255, minimum, maximum, value);
	const unsigned int convertedResult = (unsigned int)(result);

	return convertedResult;
}