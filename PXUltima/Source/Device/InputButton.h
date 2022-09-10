#ifndef InputButtonInclude
#define InputButtonInclude

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	CPublic unsigned char InputButtonReset(unsigned char* value);
	CPublic unsigned char InputButtonIncrement(unsigned char* value);
	CPublic unsigned char InputButtonIncrementIfAlreadyPressed(unsigned char* value);
	CPublic unsigned char InputButtonIsShortPressed(const unsigned char value);
	CPublic unsigned char InputButtonIsLongPressed(const unsigned charvalue);
	CPublic unsigned char InputButtonIsPressed(const unsigned char value);

#ifdef __cplusplus
}
#endif

#endif