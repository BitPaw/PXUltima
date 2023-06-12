#ifndef PXCPPRegistryINCLUDE
#define PXCPPRegistryINCLUDE

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
		Registry();
		~Registry();

		PX::ActionResult ConnectSpace(const PXRegistrySpace registrySpace);
		PX::ActionResult ConnectRemote(const PX::Text& computerName, const PXRegistrySpace registrySpace);

		void Close();

		void KeyCreate();
		void KeyLoad(PX::Text& file);
		void KeySave();
		void KeyDelete();
	};
}

#endif