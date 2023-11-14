#include "PXText.hpp"

PX::Text::Text()
{
	PXTextClear(this);
}

PX::Text::Text(const char* adress, PXSize size)
{
	PXTextConstructFromAdressA(this, ((char*)adress), PXTextUnkownLength, size);
}

template<PXSize stringLength>
PX::Text::Text(const char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdressA(this, stringAdress, PXTextUnkownLength, stringLength);
}

template<PXSize stringLength>
PX::Text::Text(char(&stringAdress)[stringLength])
{
	PXTextConstructFromAdressA(this, stringAdress, PXTextUnkownLength, stringLength);
}