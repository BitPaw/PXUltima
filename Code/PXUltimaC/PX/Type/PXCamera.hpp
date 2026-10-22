#include "PXCamera.h"

namespace PX
{
    class Camera
    {
        private:
        PXCamera* _cameraHandle;

        public:
        PXF32 AspectRatio() {return PXCameraAspectRatio(_cameraHandle); }

        void Rotate(const PXVector3F32& vector3F) { PXCameraRotate(_cameraHandle, &vector3F); }
        void Rotate(const PXF32 x, const PXF32 y, const PXF32 z) { PXCameraRotateXYZ(_cameraHandle, x, y, z); }

        void Move(const PXVector3F32& vector3F) { PXCameraMove(_cameraHandle, &vector3F); }
        void Move(const PXF32 x, const PXF32 y, const PXF32 z) { PXCameraMoveXYZ(_cameraHandle, x, y, z); }
    };
}