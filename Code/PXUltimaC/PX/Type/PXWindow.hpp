#include "PXWindow.h"
#include <PX/OS/Error/PXResult.hpp>

namespace PX
{
    class Window
    {
        private:
        PXWindow* _windowHandle;

        public:
        PX::Result Destroy() { PXWindowDestroy(_windowHandle); }
        PX::Result Draw() { PXWindowDraw(_windowHandle); }
        PX::Result Show() { PXWindowShow(_windowHandle); }
        PX::Result Hide() { PXWindowHide(_windowHandle); }
        PXF32 AspectRatio() { PXWindowHide(_windowHandle); }
        bool IsValid() { PXWindowHide(_windowHandle); }
        bool IsEnabled() { PXWindowHide(_windowHandle); }

        PXSize SiblingAmount() { PXWindowSiblingAmount(_windowHandle); }
        PX::Window* SiblingGet(const PXSize index) { PXWindowSiblingGet(_windowHandle); }

        PXSize ChildAmount() { PXWindowChildAmount(_windowHandle); }
        PX::Window* ChildGet(const PXSize index) { PXWindowChildGet(_windowHandle); }

        PX::Result ChildAdd(PX::Window& pxWindowChild) { PXWindowChildAdd(_windowHandle); }
        PX::Result ChildRemove(PX::Window& pxWindowChild) { PXWindowChildRemove(_windowHandle); }

        PX::Result ParentSet(const PX::Window& pxWindowParent) { PXWindowParentSet(_windowHandle); }
        PX::Window* ParentGet() { PXWindowParentGet(_windowHandle); }
    };
}