#pragma once

#include <OS/System/PXRegistry.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXText.hpp>

namespace PX
{
	enum class RegistrySpace
	{
		Invalid,
		LocalMachine,
		PerformanceData,
		Users
	};

	class Registry : public PXRegistry
	{
		public:
		PXDLLExport Registry();
		PXDLLExport ~Registry();

		PXDLLExport PX::ActionResult ConnectSpace(const PXRegistrySpace registrySpace);
		PXDLLExport PX::ActionResult ConnectRemote(const PX::Text& computerName, const PXRegistrySpace registrySpace);

		PXDLLExport void Close();

		PXDLLExport void KeyCreate();
		PXDLLExport void KeyLoad(PX::Text& file);
		PXDLLExport void KeySave();
		PXDLLExport void KeyDelete();
	};
}