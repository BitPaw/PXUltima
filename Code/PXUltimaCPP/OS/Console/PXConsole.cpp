#include "PXConsole.hpp"

void PXAPI PX::Console::Clear()
{
	return PXConsoleClear();
}

void PXAPI PX::Console::GoToXY(const PXInt32U x, const PXInt32U y)
{
	return PXConsoleGoToXY(x, y);
}

void PXAPI PX::Console::Print(const PXLoggingType loggingType, const char* const source, const char* const format, ...)
{
	
}

void PXAPI PX::Console::PrintString(const char* const source, PXSize length)
{

}

void PXAPI PX::Console::PrintStringLine(const char* const source, PXSize length)
{
	
}