#ifndef PXCPPVectorINCLUDE
#define PXCPPVectorINCLUDE

namespace PX
{
	template<typename Number>
	class Vector2
	{
		union
		{
			struct
			{
				Number X;
				Number Y;
			};

			Number Data[2];
		};
	};

	template<typename Number>
	class Vector3
	{
		union
		{
			struct
			{
				Number X;
				Number Y;
				Number Z;
			};

			Number Data[3];
		};
	};

	template<typename Number>
	class Vector4
	{
		union
		{
			struct
			{
				Number X;
				Number Y;
				Number Z;
				Number W;
			};

			Number Data[4];
		};
	};
}

#endif