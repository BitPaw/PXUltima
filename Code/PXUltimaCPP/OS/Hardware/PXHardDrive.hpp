#ifndef PXCPPHardDriveINCLUDE
#define PXCPPHardDriveINCLUDE

#include <OS/Hardware/PXHardDrive.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{

	class HardDrive : public PXHardDrive
	{
		public:
		static PXSize ListSize();

		static PX::ActionResult FetchAll(PXHardDrive* const hardDriveList, const PXSize hardDriveListMaxSize, PXSize* hardDriveListSize);
	};

}

#endif