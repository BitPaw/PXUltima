#include "PXText.h"
#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    class Text : public PXText
    {
        private:

        public:
        Text(void* address, const PXSize sizeUsed, const PXSize sizeAllocated, const PXTextFormat pxTextFormat) { PXTextFromAddress(this, address, sizeUsed, sizeAllocated, pxTextFormat); }
        Text(const char* address, const PXSize sizeUsed, const PXSize sizeAllocated) { PXTextFromAddressA(this, address, sizeUsed, sizeAllocated); }
        Text(const wchar_t* address, const PXSize sizeUsed, const PXSize sizeAllocated) { PXTextFromAddressW(this, address, sizeUsed, sizeAllocated); }

        PX::Result FromAddress(void* address, const PXSize sizeUsed, const PXSize sizeAllocated, const PXTextFormat pxTextFormat) { return (PX::Result)PXTextFromAddress(this, address, sizeUsed, sizeAllocated, pxTextFormat); }
        PX::Result FromAddress(const char* address, const PXSize sizeUsed, const PXSize sizeAllocated) { return (PX::Result)PXTextFromAddressA(this, address, sizeUsed, sizeAllocated); }
        PX::Result FromAddress(const wchar_t* address, const PXSize sizeUsed, const PXSize sizeAllocated) { return (PX::Result)PXTextFromAddressW(this, address, sizeUsed, sizeAllocated); }

    };
}