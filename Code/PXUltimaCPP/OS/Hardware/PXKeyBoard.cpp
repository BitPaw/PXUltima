#include "PXKeyBoard.hpp"

void PX::KeyBoard::InputReset()
{
	PXKeyBoardInputReset(this);
}

unsigned char PX::KeyBoard::KeyPressedGet(const VirtualKey virtualKey)
{
	return PXKeyBoardKeyPressedGet(this, (PXVirtualKey)virtualKey);
}

unsigned char PX::KeyBoard::KeyPressedSet(const VirtualKey virtualKey, const unsigned char isPressed)
{
	return PXKeyBoardKeyPressedSet(this, (PXVirtualKey)virtualKey, isPressed);
}

void PX::KeyBoard::InputPrint()
{
	
}
