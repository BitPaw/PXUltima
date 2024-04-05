#ifndef PXMouseINCLUDE
#define PXMouseINCLUDE

#define ButtonLeft    0b00000001
#define ButtonMiddle  0b00000010
#define ButtonRight   0b00000100
#define ButtonCustomA 0b00001000
#define ButtonCustomB 0b00010000
#define ButtonCustomC 0b00100000
#define ButtonCustomD 0b01000000
#define ButtonCustomE 0b10000000

#define IsPressedButtonLeft(buttons)	 (buttons & ButtonLeft)
#define IsPressedButtonMiddle(buttons)  ((buttons & ButtonMiddle) >> 1)
#define IsPressedButtonRight(buttons)   ((buttons & ButtonRight) >> 2)
#define IsPressedButtonCustomA(buttons) ((buttons & ButtonCustomA) >> 3)
#define IsPressedButtonCustomB(buttons) ((buttons & ButtonCustomB) >> 4)
#define IsPressedButtonCustomC(buttons) ((buttons & ButtonCustomC) >> 5)
#define IsPressedButtonCustomD(buttons) ((buttons & ButtonCustomD) >> 6)
#define IsPressedButtonCustomE(buttons) ((buttons & ButtonCustomE) >> 7)

#include <Media/PXType.h>

typedef struct PXMouse_
{
	float DeltaNormalisized[2];
	float PositionNormalisized[2];  // Convert Screenspace (width & height) to normal space -1 to +1
	PXInt32S Position[2]; // Window-Position (0/0 to x/y)
	PXInt32S Delta[2]; // Relative Input (-x/-y to 0/0 to x/y)	
	PXInt8U Buttons;
	PXInt8U ButtonsDelta;
}
PXMouse;

PXPublic void PXAPI PXMouseInputReset(PXMouse* const mouse);
PXPublic void PXAPI PXMouseInputPrint(const PXMouse* const mouse);

#endif