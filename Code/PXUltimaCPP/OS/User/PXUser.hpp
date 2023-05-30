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
		static bool NameGet(PX::Text& text);
		static bool EnviromentFolderGet(PX::Text& text, const UserEnviromentFolder userEnviromentFolder);
	};
}

#endif