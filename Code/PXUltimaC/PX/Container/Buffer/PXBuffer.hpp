#include "PXBuffer.h"
#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    class Buffer : public PXBuffer
    {
        public:
        bool IsInBounce() { return PXBufferIsInBounce(this); }
        bool IsInBounceWithOffset(const PXSize offset) { return PXBufferIsInBounceWithOffset(this, offset); }
        PXByte* Data() { return PXBufferData(this); }
        PXByte* DataWithOffset(const PXSize offset) { return PXBufferDataWithOffset(this, offset); }
        PXSize Remaining() { return PXBufferRemaining(this); }
        PX::Result Allocate(const PXSize size) { return (PX::Result)PXBufferAllocate(this, size); }
        PX::Result EnsureTotal(const PXSize sizeTotal) { return (PX::Result)PXBufferEnsureTotal(this, sizeTotal); }
        PX::Result EnsureAdditional(const PXSize sizeAdditionalData) { return (PX::Result)PXBufferEnsureAdditional(this, sizeAdditionalData); }
        PX::Result Resize(const PXSize size) { return (PX::Result)PXBufferResize(this, size); }
        PX::Result Release() { return (PX::Result)PXBufferRelease(this); }
        PX::Result Set(void* data, const PXSize size) { return (PX::Result)PXBufferSet(this, data, size); }
        PX::Result Copy(void* data, const PXSize size) { return (PX::Result)PXBufferCopy(this, data, size); }
        PX::Result Append(void* data, const PXSize size, void** out) { return (PX::Result)PXBufferAppend(this, data, size, out); }
        PX::Result CursorMove(const PXI64S move) { return (PX::Result)PXBufferCursorMove(this, move); }
        PX::Result Store(const void* data, const PXSize sizeTotal, const PXBool isDataConst) { return (PX::Result)PXBufferStore(this, data, sizeTotal, isDataConst); }
    };
}
