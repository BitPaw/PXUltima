#ifndef PXCPPDirectXINCLUDE
#define PXCPPDirectXINCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

namespace PX
{
	class DirectX : public PXDirectX
	{
		public:
		DirectX();
		~DirectX();

		void ContextCreate();
	};
}

#endif