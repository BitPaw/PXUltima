#include "PXType.h"
#include <PX/Type/PXViewPort.h>
#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    class ViewPort : public PXViewPort
    {
        public:
        PX::Result SetWH
        (
            const PXI32S width,
            const PXI32S height
        )
        {
            PXViewPortSetWH(this, width, height);
        }


        PX::Result SetXYWH
        (
            const PXI32S x,
            const PXI32S y,
            const PXI32S width,
            const PXI32S height
        )
        {
            PXViewPortSetXYWH(this, x, y, width, height);
        }
    };
}