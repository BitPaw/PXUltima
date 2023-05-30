#include "InputButton.h"

unsigned char PXInputButtonReset(unsigned char* value)
{
	*value = 0x00;

	return 0;
}

unsigned char PXInputButtonIncrement(unsigned char* value)
{
	if(*value != 0xFF)
	{
		++(*value);
	}

	return 0;
}

unsigned char PXInputButtonIncrementIfAlreadyPressed(unsigned char* value)
{
	if(*value > 0 && *value != 0xFF)
	{
		++(*value);
	}

	return 0;
}

unsigned char PXInputButtonIsShortPressed(const unsigned char value)
{
  return value > 0 && value < 0xA0;
}

unsigned char PXInputButtonIsLongPressed(const unsigned charvalue)
{
    return charvalue >= 0xA0;
}

unsigned char PXInputButtonIsPressed(const unsigned char value)
{
    return value > 0;
}
