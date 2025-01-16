#ifndef PXMathINCLUDE
#define PXMathINCLUDE

#include <Media/PXType.h>

//---<Constants>---------------------------------------------------------------
#define PXMathConstantPI 3.1415926535897932384626433832     // Pi, Archimedes' constant or Ludolph's number
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
#define PXMathAbsolute(x) (((x) > 0) ? (x) : -(x))

//-----------------------------------------------------------------------------

typedef struct PXMathRandomGeneratorSeed_
{
    PXInt32U X;
    PXInt32U Y;
    PXInt32U Z;
}
PXMathRandomGeneratorSeed;

//---------------------------------------------------------
// Minimum - Smaler of two numbers
//---------------------------------------------------------
PXPublic int PXAPI PXMathMinimumI(const int a, const int b);
PXPublic PXSize PXAPI PXMathMinimumIU(const PXSize a, const PXSize b);
PXPublic double PXAPI PXMathMinimumD(const double a, const double b);
//---------------------------------------------------------


//---------------------------------------------------------
// Maximum - Bigger of two numbers
//---------------------------------------------------------
PXPublic PXSize PXAPI PXMathMaximumIU(const PXSize a, const PXSize b);
//---------------------------------------------------------


//---------------------------------------------------------
// Floor - Round down to the next whole number
//---------------------------------------------------------
PXPublic int PXAPI PXMathFloorI(const int a);
PXPublic int PXAPI PXMathFloorD(const double a);
//---------------------------------------------------------


//---------------------------------------------------------
// Absolute - Convert to positive
//---------------------------------------------------------
PXPublic PXInt16U PXAPI PXMathAbsoluteI16(const PXInt16S value);
PXPublic PXInt32U PXAPI PXMathAbsoluteI32(const PXInt32S value);
PXPublic double PXAPI PXMathAbsoluteD(const double value);
//---------------------------------------------------------


//---------------------------------------------------------
// Ceiling - Round to the next whole number
//---------------------------------------------------------
PXPublic int PXAPI PXMathCeilingF(const float value);
PXPublic int PXAPI PXMathCeilingD(const double value);
//---------------------------------------------------------


//---------------------------------------------------------
PXPublic char PXAPI PXMathLimitC(const char value, const char minimum, const char maximum);
PXPublic unsigned char PXAPI PXMathLimitCU(const unsigned char  value, const unsigned char  minimum, const unsigned char maximum);
PXPublic int PXAPI PXMathLimitI(const int value, const int minimum, const int maximum);
PXPublic PXSize PXAPI PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum);
//---------------------------------------------------------











//---------------------------------------------------------
// Logarithmus
//---------------------------------------------------------
PXPublic double PXAPI PXMathLogarithmus(int base, double exponent);
PXPublic double PXAPI PXMathLogarithmusBase2(double exponent);
PXPublic double PXAPI PXMathLogarithmusBase10(double exponent);
//---------------------------------------------------------



//---------------------------------------------------------
// Power
//---------------------------------------------------------
PXPublic double PXAPI PXMathPower(double base, double exponent);
PXPublic double PXAPI PXMathPowerOfTwo(double base);
PXPublic PXInt32U PXAPI PXMathPowerModulo(const PXInt32U base, const PXInt32U exponent, const PXInt32U modulo);
//---------------------------------------------------------


//---------------------------------------------------------
// Root
//---------------------------------------------------------
PXPublic double PXAPI PXMathRoot(unsigned int rootNr, double value);
PXPublic double PXAPI PXMathRootSquare(double value);
PXPublic double PXAPI PXMathRootCubic(double value);
//---------------------------------------------------------



//---------------------------------------------------------
// Geometric
//---------------------------------------------------------
PXPublic float PXAPI PXMathSinusF(const float x);
PXPublic double PXAPI PXMathSinusD(const double x);

PXPublic double PXAPI PXMathTangensF(const float x);
PXPublic double PXAPI PXMathTangensD(const double x);

PXPublic float PXAPI PXMathCosinusF(const float x);
PXPublic double PXAPI PXMathCosinusD(const double x);


PXPublic double PXAPI PXMathPythagoras(double a, double b);
PXPublic double PXAPI PXMathPythagorasReverse(double c, double a);
//---------------------------------------------------------


//---------------------------------------------------------
// Arcus
//---------------------------------------------------------
PXPublic double PXAPI PXMathArcusSinus(const double x);
PXPublic double PXAPI PXMathArcusTangens(const double x);
//---------------------------------------------------------



//---------------------------------------------------------
// Hyperbolic
//---------------------------------------------------------
PXPublic double PXAPI PXMathHyperbolicSinus(const double x);
PXPublic double PXAPI PXMathHyperbolicCosinus(const double x);

// tanh(x)= (e^x + e^(−x)) / (e^x − e^(−x))
PXPublic double PXAPI PXMathHyperbolicTangens(const double x);
//---------------------------------------------------------


//---------------------------------------------------------
// DEG <=> RAD
//---------------------------------------------------------
PXPublic double PXAPI PXMathRadiansToDegree(double radians);
PXPublic double PXAPI PXMathDegreeToRadians(double degree);
//---------------------------------------------------------



// Tangens inverse that can handle the values from all the 4 different quadrants
//PXPublic double PXAPI PXMathTangensArc2(double x, double y);
PXPublic double PXAPI PXMathArcusTangens2(const double x, const double y);



// exp()
PXPublic double PXAPI PXExponential(const double x);


PXPublic unsigned long PXAPI PXMathFibonacci(unsigned long step);

PXPublic float PXAPI PXMathFastInverseSqaureRoot(float number);

PXPublic double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance);

PXPublic void PXAPI PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);
PXPublic PXInt32U PXAPI PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed);

// Liniar interpolate a value from the range of 'x' min and max to another range 'y'.
PXPublic float PXAPI PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue);
PXPublic float PXAPI PXMathNormalizeF(const float minimum, const float maximum, const float value);
PXPublic unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value);

// x = (-b +/- b^2 - 4ac) / 2a
PXPublic void PXAPI PXMathFormulaQuadratic(const float a, const float b, const float c, float* const resultA, float* const resultB);

// x = -(p/2) +/- sqrt((p/2)^2 - q)
PXPublic void PXAPI PXMathFormulaPQ(const float p, const float q, float* const resultA, float* const resultB);

PXPublic double PXAPI PXMathHornerD(double* const coefficientList, const PXInt32U degree, const double x);

PXPublic void PXAPI PXFastFourierTransform();

#endif