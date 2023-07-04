#ifndef PXCPPUserINCLUDE
#define PXCPPUserINCLUDE

#include <OS/User/PXUser.h>
#include <Media/PXText.hpp>

namespace PX
{
	enum class UserEnviromentFolder
	{
		Invalid,
		AppData,
		Download,
		Documents,
		Pictures,
		Screenshots,
		Video,
		Destop,
		Fonts,
		Music
	};

	class User
	{
		public:
		PXDLLExport static bool NameGet(PX::Text& text);
		PXDLLExport static bool EnviromentFolderGet(PX::Text& text, const UserEnviromentFolder userEnviromentFolder);
	};
}

#endif