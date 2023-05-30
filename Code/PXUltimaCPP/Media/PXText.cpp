#include "PXText.hpp"

PX::Text::Text()
{
	PXTextClear(this);
}

PX::Text::Text(const char* adress, PXSize size)
{
	PXTextConstructFromAdress(this, ((char*)adress), size);
}

template<PXSize stringLength>
PX::Text::Text(const char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdress(this, stringAdress, stringLength);
}

template<PXSize stringLength>
PX::Text::Text(char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdress(this, stringAdress, stringLength);
}