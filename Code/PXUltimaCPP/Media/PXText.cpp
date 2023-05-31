#include "PXText.hpp"

PX::Text::Text()
{
	PXTextClear(this);
}

PX::Text::Text(const char* adress, PXSize size)
{
	PXTextConstructFromAdressA(this, ((char*)adress), size);
}

template<PXSize stringLength>
PX::Text::Text(const char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdressA(this, stringAdress, stringLength);
}

template<PXSize stringLength>
PX::Text::Text(char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdressA(this, stringAdress, stringLength);
}