#pragma once

namespace PX
{
	template <class NumberType>
	struct Vector3
	{
		public:
		union
		{
			struct
			{
				NumberType X;
				NumberType Y;
				NumberType Z;
			};

			NumberType Data[3];
		};

		Vector3();
		Vector3(const NumberType x, const NumberType y, const NumberType z);

		void operator+=(const NumberType scalar);
		void operator+=(Vector3<NumberType> vector3);
		Vector3<NumberType> operator+(Vector3<NumberType> vector);
		Vector3<NumberType> operator-(Vector3<NumberType> vector);
		void operator-=(Vector3<NumberType> vector);
		void operator*=(Vector3<NumberType> vector3);
		void operator*=(NumberType scalar);
		Vector3<NumberType> operator*(NumberType scalar);
		Vector3<NumberType> operator*(Vector3<NumberType> vector3);
		Vector3<NumberType> operator/(NumberType scalar);

		void Add(NumberType x, NumberType y, NumberType z);

		void Add(Vector3<NumberType> vector);

		void Multiply(NumberType x, NumberType y, NumberType z);

		void Set(NumberType x, NumberType y, NumberType z);

		void Normalize();

		static Vector3<NumberType> Normalize(Vector3 vector3);

		NumberType Length();
		Vector3<NumberType> CrossProduct(Vector3 vector3);

		Vector3<NumberType> CrossProduct(NumberType x, NumberType y, NumberType z);

		void CrossProduct(Vector3<NumberType> vectorA, Vector3<NumberType> vectorB);

		static void CrossProduct(Vector3<NumberType> vectorA, Vector3<NumberType> vectorB, Vector3<NumberType>& result);

		static NumberType DotProduct(Vector3<NumberType> vectorA, Vector3<NumberType> vectorB);
		void Rotate(Vector3<NumberType> rotation);

		static Vector3<NumberType> Interpolate(Vector3<NumberType> positionCurrent, Vector3<NumberType> positionDesired, NumberType factor);
	};
}