#include "PXAdler32.hpp"

PXSize PX::Adler32::Check(const void* const data, const PXSize length)
{
	return PXAdler32Check(data, length);
}

PXSize PX::Adler32::Create(const PXSize adler, const unsigned char* data, PXSize length)
{
	return PXAdler32Create(adler, data, length);
}