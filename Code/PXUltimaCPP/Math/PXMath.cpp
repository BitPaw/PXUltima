#include "PXMath.hpp"

#include <Math/PXMath.h>

template<typename Number>
Number PX::Math::Minimum(const Number a, const Number b)
{
	return PXMathMinimum(a, b);
}
template<typename Number>
Number PX::Math::Maximum(const Number a, const Number b)
{
	return PXMathMaximum(a, b);
}

template<typename Number>
Number PX::Math::Floor(const Number a)
{
	return PXMathFloor(a);
}

template<typename Number>
Number PX::Math::Absolute(const Number value)
{
	return PXMathAbsolute(value);
}

template<typename Number>
Number PX::Math::Limit(const Number value, const Number minimum, const Number maximum)
{
	return PXMathLimit(value, minimum, maximum);
}

template<typename Number>
Number PX::Math::FastInverseSqaureRoot(const Number number)
{
	return PXMathFastInverseSqaureRoot(number);
}

template<typename Number>
Number PX::Math::Power(const Number base, const Number exponent)
{
	return PXMathPower(base, exponent);
}

template<typename Number>
Number PX::Math::PowerOfTwo(const Number base)
{
	return PXMathPowerOfTwo(base);
}

template<typename Number>
Number PX::Math::PowerModulo(const Number base, const Number exponent, const Number modulo)
{
	return PXMathPowerModulo(base, exponent, modulo);
}

template<typename Number>
Number PX::Math::Root(const Number rootNr, const Number value)
{
	return PXMathRoot(rootNr, value);
}

template<typename Number>
Number PX::Math::SquareRoot(const Number value)
{
	return PXMathSquareRoot(value);
}

template<typename Number>
Number PX::Math::CubicRoot(const Number value)
{
	return PXMathCubicRoot(value);
}

template<typename Number>
Number PX::Math::Pythagoras(const Number a, const Number b)
{
	return PXMathPythagoras(a, b);
}

template<typename Number>
Number PX::Math::PythagorasReverse(const Number c, const Number a)
{
	return PXMathPythagorasReverse(c, a);
}

template<typename Number>
Number PX::Math::Logarithmus(const Number base, const Number exponent)
{
	PXMathLogarithmus(base, exponent);
}

template<typename Number>
Number PX::Math::LogarithmusBase2(const Number exponent)
{
	return PXMathLogarithmusBase2(exponent);
}

template<typename Number>
Number PX::Math::LogarithmusBase10(const Number exponent)
{
	return PXMathLogarithmusBase10(exponent);
}

template<typename Number>
Number PX::Math::NewtonGravitation(const Number massA, const Number massB, const Number distance)
{
	return PXMathNewtonGravitation(massA, massB, distance);
}

template<typename Number>
Number PX::Math::Ceiling(const Number value)
{
	return PXMathCeilingD(value);
}

template<typename Number>
Number PX::Math::RandomeNumber()
{
	return PXMathRandomeNumber();
}

template<typename Number>
Number PX::Math::Sinus(const Number value)
{
	return PXMathSinus(value);
}

template<typename Number>
Number PX::Math::Tangens(const Number value)
{
	return PXMathTangens(value);
}

template<typename Number>
Number PX::Math::Cosinus(const Number value)
{
	return PXMathCosinus(value);
}

template<typename Number>
Number PX::Math::Fibonacci(const Number step)
{
	return PXMathFibonacci(step);
}

template<typename Number>
Number PX::Math::RadiansToDegree(const Number radians)
{
	return PXMathRadiansToDegree(radians);
}

template<typename Number>
Number PX::Math::DegreeToRadians(const Number degree)
{
	return PXMathDegreeToRadians(degree);
}

template<typename Number>
Number PX::Math::Liniar(const Number yMinimum, const Number yMaximum, const Number xMinimum, const Number xMaximum, const Number xValue)
{
	return PXMathLiniarF(yMinimum, yMaximum, xMinimum, xMaximum, xValue);
}

template<typename Number>
Number PX::Math::Normalize(const Number minimum, const Number maximum, const Number value)
{
	return PXMathNormalizeF(minimum, maximum, value);
}

template<typename Number>
Number PX::Math::LiniarClampAsRGBColor(const Number minimum, const Number maximum, const Number value)
{
	return PXMathLiniarClampAsRGBColorF(minimum, maximum, value);
}