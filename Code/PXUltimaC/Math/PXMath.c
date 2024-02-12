#include "PXMath.h"

#include <math.h>

int PXMathMinimumI(const int a, const int b)
{
	return PXMathMinimum(a, b);
}

PXSize PXMathMinimumIU(const PXSize a, const PXSize b)
{
	return PXMathMinimum(a, b);
}

double PXMathMinimumD(const double a, const double b)
{
	return PXMathMinimum(a, b);
}

PXSize PXMathMaximumIU(const PXSize a, const PXSize b)
{
	return PXMathMaximum(a, b);
}

int PXMathFloorI(const int a)
{
	return PXMathFloor(a);
}

int PXMathFloorD(const double a)
{
	return PXMathFloor(a);
}

PXSize PXMathAbsoluteI16(const PXInt16S value)
{
	return PXMathAbsolute(value);
}

PXSize PXMathAbsoluteI32(const PXInt32S value)
{
	return PXMathAbsolute(value);
}

double PXMathAbsoluteD(const double a)
{
	return PXMathAbsolute(a);
}

char PXMathLimitC(const char value, const char minimum, const char maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

unsigned char PXMathLimitCU(const unsigned char value, const unsigned char minimum, const unsigned char maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

int PXMathLimitI(const int value, const int minimum, const int maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

PXSize PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

float PXMathFastInverseSqaureRoot(float number)
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

double PXMathPower(double base, double exponent)
{
	return pow(base, exponent);
}

double PXMathPowerOfTwo(double base)
{
	return PXMathPower(base, 2);
}

unsigned int PXMathPowerModulo(unsigned int base, unsigned int exponent, unsigned int modulo)
{
	unsigned int result = 1;
	for(int i = 0; i < exponent; i++)
	{
		result *= base;
		result %= modulo;
	}
	return result;
}

double PXMathRoot(unsigned int rootNr, double value)
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
			return cbrt(value);

		default:
			return 0;
	}
}

double PXMathSquareRoot(double value)
{
	return PXMathRoot(2, value);
}

double PXMathCubicRoot(double value)
{
	return PXMathRoot(3, value);
}

double PXMathPythagoras(double a, double b)
{
	return PXMathSquareRoot(PXMathPowerOfTwo(a) + PXMathPowerOfTwo(b));
}

double PXMathPythagorasReverse(double c, double a)
{
	return 0.0;
}

double PXMathLogarithmus(int base, double exponent)
{
#if OSUnix
	return 0; // TODO: not implemented?
#elif OSWindows
	return _dlog(exponent, base);
#endif	
}

double PXMathLogarithmusBase2(double exponent)
{
	return log2(exponent);
}

double PXMathLogarithmusBase10(double exponent)
{
	return log10(exponent);
}

double PXMathNewtonGravitation(double massA, double massB, double distance)
{
	return PXMathConstantGravity * ((massA * massB) / (PXMathPower(distance, 2)));
}

int PXMathCeilingF(const float value)
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

int PXMathCeilingD(const double value)
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

void PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
{
	pxMathRandomGeneratorSeed->X = 123456789;
	pxMathRandomGeneratorSeed->Y = 362436069;
	pxMathRandomGeneratorSeed->Z = 521288629;
}

PXInt32U PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
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

double PXMathSinus(double value)
{
	return sin(value);
}

double PXMathTangens(double value)
{
	return tan(value);
}

double PXMathCosinus(double value)
{
	return cos(value);
}

unsigned long PXMathFibonacci(unsigned long step)
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

double PXMathRadiansToDegree(double radians)
{
	return radians * (180.0 / PXMathConstantPI);
}

double PXMathDegreeToRadians(double degree)
{
	return degree * (PXMathConstantPI / 180.0);
}

float PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue)
{
	// Cap Value
	float xCap = xValue > xMaximum ? xMaximum : xValue;
	xCap = xValue < xMinimum ? xMinimum : xValue;

	// ((ydelta * xValueDelta) / xdelta) + yMinimum;

	return (((yMaximum - yMinimum) * (xValue - xMinimum)) / (xMaximum - xMinimum)) + yMinimum;
}

float PXMathNormalizeF(const float minimum, const float maximum, const float value)
{
	return PXMathLiniarF(0, 1, minimum, maximum, value);
}

unsigned int PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value)
{
	const float result = PXMathLiniarF(0, 255, minimum, maximum, value);
	const unsigned int convertedResult = (unsigned int)(result);

	return convertedResult;
}