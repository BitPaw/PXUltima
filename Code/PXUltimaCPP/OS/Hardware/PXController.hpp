#ifndef PXCPPControllerINCLUDE
#define PXCPPControllerINCLUDE

#include <OS/Hardware/PXController.h>

namespace PX
{
	class Controller : public PXController
	{
		public:
		static bool ScanDevices(NewControllerDetectedCallback callback);
		bool DataGet();

#if PXWindowUSE
		bool AttachToWindow(const PXWindowID PXWindowID);
		bool DetachToWindow();
#endif
	};
}

#endif