#include "PXInputDevice.h"

const char* PXInputDeviceTypeToString(const PXInputDeviceType pxInputDeviceType)
{
	switch(pxInputDeviceType)
	{
        default:
        case PXInputDeviceTypeInvalid:              return "Invalid";
        case PXInputDeviceTypeMouse:                return "Mouse";
        case PXInputDeviceTypeKeyBoard:             return "KeyBoard";
        case PXInputDeviceTypeJoystick:             return "Joystick";
        case PXInputDeviceTypeGamepad:              return "Gamepad";
        case PXInputDeviceTypeMultiAxisController:  return "MultiAxisController";
        case PXInputDeviceTypeSteeringWheel:        return "SteeringWheel";
        case PXInputDeviceTypePedals:               return "Pedals";
        case PXInputDeviceTypeFlightStick:          return "FlightStick";
        case PXInputDeviceTypeRudderPedals:         return "RudderPedals";

        case PXInputDeviceTypeDRIVING:              return "Drivving";
        case PXInputDeviceTypeFLIGHT:               return "Flight";
        case PXInputDeviceType1STPERSON:            return "FPS";
        case PXInputDeviceTypeDEVICECTRL:           return "DevCTRL";
        case PXInputDeviceTypeSCREENPOINTER:        return "Pointer";
        case PXInputDeviceTypeREMOTE:               return "Remote";
        case PXInputDeviceTypeSUPPLEMENTAL:         return "Supplemental";

        case PXInputDeviceTypeUnkown:               return "Unkown";
	}
}