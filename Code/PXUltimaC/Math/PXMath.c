#include "PXMath.h"

#include <math.h>
#include <intrin.h>
#include <immintrin.h>
#include <wmmintrin.h>
#include <mmintrin.h>

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

PXInt16U PXAPI PXMathAbsoluteI16(const PXInt16S value)
{
    return PXMathAbsolute(value);
}

PXInt32U PXAPI PXMathAbsoluteI32(const PXInt32S value)
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
            return sqrt(value);  // TODO: Dependeny problem 

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
    return log10(exponent);  // TODO: Dependeny problem 
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

//extern _cdecl PXIntrinsicFSIN(float* val);
//extern _cdecl PXIntrinsicFSQRT(float* val);

float PXAPI PXMathSinusF(const float value)
{
    float x = value;

#if 0
    double dx = value;

    PXIntrinsicFSIN(&dx); // sin(value); // TODO: Dependeny problem 

    return dx;

#else
    x = sin(value);
#endif

    return x;
}

float PXAPI PXMathRootSquareF(const float value)
{
    float x = value;

#if 0
    PXIntrinsicFSQRT(&x); // sin(value); // TODO: Dependeny problem 
#else
    x = sqrt(value);
#endif

    return x;
}

double PXAPI PXMathSinus(double value)
{
    float x = 0;// value;

    x = sin(value); // sin(value); // TODO: Dependeny problem 

    return x;
}

double PXAPI PXMathTangens(double value)
{
    return tan(value); // TODO: Dependeny problem 
}

double PXAPI PXMathCosinus(double value)
{
    return cos(value); // TODO: Dependeny problem 
}

double PXAPI PXMathSinusArc(double value)
{
    return sinh(value);
}

double PXAPI PXMathSinusA(double value)
{
#if 1 // Support asin()
    return asin(value);
#else
    return PXMathSinusArc(value);
#endif

}

double PXAPI PXMathTangensArc(double value)
{
    return tanh(value);
}

double PXAPI PXMathTangensArc2(double x, double y)
{
    return atan2(y, x);
}

double PXAPI PXMathCosinusArc(double value)
{
    return cosh(value);
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

void PXAPI PXMathFormulaQuadratic(const float a, const float b, const float c, float* const resultA, float* const resultB)
{
    const float bNegativ = -b;
    const float dividor = 2.0f * a;
    const float rootData = b * b - 4.0f * a * c;
    const float rootResult = PXMathSquareRoot(rootData);

    *resultA = (bNegativ + rootResult) / dividor;
    *resultB = (bNegativ - rootResult) / dividor;
}

void PXAPI PXMathFormulaPQ(const float p, const float q, float* const resultA, float* const resultB)
{
    const float pHalf = p / 2.0f;
    const float pHalfNeg = -pHalf;
    const float rootData = pHalf * pHalf - q;
    const float rootResult = PXMathSquareRoot(rootData);

    *resultA = pHalfNeg + rootResult;
    *resultB = pHalfNeg - rootResult;
}