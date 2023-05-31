#ifndef PXCPPMathINCLUDE
#define PXCPPMathINCLUDE

namespace PX
{
	class Math
	{
		public:
		template<typename Number> static Number Minimum(const Number a, const Number b);
		template<typename Number> static Number Maximum(const Number a, const Number b);
		template<typename Number> static Number Floor(const Number a);
		template<typename Number> static Number Absolute(const Number value);
		template<typename Number> static Number Limit(const Number value, const Number minimum, const Number maximum);

		template<typename Number> static Number FastInverseSqaureRoot(const Number number);
		template<typename Number> static Number Power(const Number base, const Number exponent);
		template<typename Number> static Number PowerOfTwo(const Number base);
		template<typename Number> static Number PowerModulo(const Number base, const Number exponent, const Number modulo);
		template<typename Number> static Number Root(const Number rootNr, const Number value);
		template<typename Number> static Number SquareRoot(const Number value);
		template<typename Number> static Number CubicRoot(const Number value);
		template<typename Number> static Number Pythagoras(const Number a, const Number b);
		template<typename Number> static Number PythagorasReverse(const Number c, const Number a);
		template<typename Number> static Number Logarithmus(const Number base, const Number exponent);
		template<typename Number> static Number LogarithmusBase2(const Number exponent);
		template<typename Number> static Number LogarithmusBase10(const Number exponent);
		template<typename Number> static Number NewtonGravitation(const Number massA, const Number massB, const Number distance);
		template<typename Number> static Number Ceiling(const Number value);
		template<typename Number> static Number RandomeNumber();
		template<typename Number> static Number Sinus(const Number value);
		template<typename Number> static Number Tangens(const Number value);
		template<typename Number> static Number Cosinus(const Number value);
		template<typename Number> static Number Fibonacci(const Number step);
		template<typename Number> static Number RadiansToDegree(const Number radians);
		template<typename Number> static Number DegreeToRadians(const Number degree);

		template<typename Number> static Number Liniar(const Number yMinimum, const Number yMaximum, const Number xMinimum, const Number xMaximum, const Number xValue);
		template<typename Number> static Number Normalize(const Number minimum, const Number maximum, const Number value);
		template<typename Number> static Number LiniarClampAsRGBColor(const Number minimum, const Number maximum, const Number value);
	};
}

#endif
