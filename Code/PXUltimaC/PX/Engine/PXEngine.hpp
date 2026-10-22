

#include <PX/Engine/PXEngine.h>
#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    class Engine
    {
        private:
        PXEngine* EngineHandle;

        public:
        PX::Result Create(PXEngineCreateInfo PXREF pxEngineCreateInfo);
        PX::Result Release();
        PXWindow* WindowMainGet();
        PX::Result Start();
        void Stop();
        PXWindow* WindowGet();
        void FPSTargetSet(const PXI32U fpsTarget);
    };
}