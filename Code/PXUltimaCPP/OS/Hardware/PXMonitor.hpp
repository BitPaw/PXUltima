#ifndef PXCPPMonitorINCLUDE
#define PXCPPMonitorINCLUDE

#include <OS/Hardware/PXMonitor.h>

namespace PX
{
	class Monitor : public PXMonitor
	{
		public:
		static void FetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize);
		static void GetSize(PXInt32U& width, PXInt32U& height);
	};
}

#endif