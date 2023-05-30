#ifndef PXInputButtonINCLUDE
#define PXInputButtonINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic unsigned char PXInputButtonReset(unsigned char* value);
	PXPublic unsigned char PXInputButtonIncrement(unsigned char* value);
	PXPublic unsigned char PXInputButtonIncrementIfAlreadyPressed(unsigned char* value);
	PXPublic unsigned char PXInputButtonIsShortPressed(const unsigned char value);
	PXPublic unsigned char PXInputButtonIsLongPressed(const unsigned charvalue);
	PXPublic unsigned char PXInputButtonIsPressed(const unsigned char value);

#ifdef __cplusplus
}
#endif

#endif