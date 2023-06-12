#include "PXCRC32.hpp"

PXInt32U PX::CRC32::Generate(const PXByte* const data, const PXSize length)
{
	return PXCRC32Generate(data, length);
}
