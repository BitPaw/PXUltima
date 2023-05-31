#ifndef PXCPPMatrixINCLUDE
#define PXCPPMatrixINCLUDE

#include <Media/PXType.h>
#include <Math/PXVector.h>

namespace BF
{
	/*
		+----+----+----+----+
		| XA | YA | ZA | Or |
		+----+----+----+----+
		|  0 |  1 |  2 |  3 |
		|  4 |  5 |  6 |  7 |
		|  8 |  9 | 10 | 11 |
		| 12 | 13 | 14 | 15 |
		+----+----+----+----+
	*/
	template <typename Number>
	class Matrix4x4
	{
		union
		{
			struct
			{
				Number XX;
				Number XY;
				Number XZ;
				Number XW;
				Number YX;
				Number YY;
				Number YZ;
				Number YW;
				Number ZX;
				Number ZY;
				Number ZZ;
				Number ZW;
				Number WX;
				Number WY;
				Number WZ;
				Number WW;
			};

			Number Data[16];

			Number DataXY[4][4];
		};	
	};
}

#endif