#include "PXCollision.h"

#include <Math/PXMath.h>

PXBool PXCollisionAABB(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
    const PXBool xInRage = MathIsInRange(cx, ax, bx) || MathIsInRange(dx, ax, bx);
    const PXBool yInRage = MathIsInRange(cy, ay, by) || MathIsInRange(dy, ay, by);
    const PXBool isFullyColliding = xInRage && yInRage;

    return isFullyColliding;
}