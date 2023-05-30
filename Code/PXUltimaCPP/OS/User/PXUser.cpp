#include "PXUser.hpp"

bool PX::User::NameGet(PX::Text& text)
{
	return PXUserNameGet(&text);
}

bool PX::User::EnviromentFolderGet(PX::Text& text, const UserEnviromentFolder userEnviromentFolder)
{
	return PXUserEnviromentFolderGet(&text, (PXUserEnviromentFolderID)userEnviromentFolder);
}