#include "PXCollision.h"

#include <Math/PXMath.h>

PXBool PXAPI PXCollisionAABB(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
    const PXBool xInRage = PXMathIsInRange(cx, ax, bx) || PXMathIsInRange(dx, ax, bx);
    const PXBool yInRage = PXMathIsInRange(cy, ay, by) || PXMathIsInRange(dy, ay, by);
    const PXBool isFullyColliding = xInRage && yInRage;

    return isFullyColliding;
}
