#ifndef PXCPPDirectXINCLUDE
#define PXCPPDirectXINCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

namespace PX
{
	class DirectX : public PXDirectX
	{
		public:
		PXDLLExport DirectX();
		PXDLLExport ~DirectX();

		PXDLLExport void ContextCreate();
	};
}

#endif