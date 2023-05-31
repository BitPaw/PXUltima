#ifndef PXGeometricINCLUDE
#define PXGeometricINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXLineF
	{
		float Left;
		float Top;
		float Right;
		float Bottom;
	};

	typedef struct PXRectangleF
	{
		float X;
		float Y;
		float Width;
		float Height;
	};

#ifdef __cplusplus
}
#endif

#endif