#include "PXActionResult.hpp"

#include <OS/Error/PXActionResult.h>

PX::ActionResult PX::ErrorCurrent()
{
	return (PX::ActionResult)PXErrorCurrent();
}