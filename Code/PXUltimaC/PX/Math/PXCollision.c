#include "PXCollision.h"

#include <PX/Math/PXMath.h>
#include "PXTriangle.h"

PXBool PXAPI PXCollisionAABB(PXF32 ax, PXF32 ay, PXF32 bx, PXF32 by, PXF32 cx, PXF32 cy, PXF32 dx, PXF32 dy)
{
    const PXBool xInRage = PXMathIsInRange(cx, ax, bx) || PXMathIsInRange(dx, ax, bx);
    const PXBool yInRage = PXMathIsInRange(cy, ay, by) || PXMathIsInRange(dy, ay, by);
    const PXBool isFullyColliding = xInRage && yInRage;

    return isFullyColliding;
}


#define PXRayNoHit -1.0f
const PXF32 EPSILON = 0.0000001f;


PXF32 PXAPI PXRayTriangleIntersect
(
    PXVector3F32 PXREF rayOriginPositon,
    PXVector3F32* rayOriginDirection,
    const PXTriangleF32 PXREF triangleVertexList,
    const PXSize amountTriangles
)
{
    // Möller–Trumbore intersection
    for(PXSize i = 0; i < amountTriangles; ++i)
    {
        const PXVector3F32 PXREF vertex0 = &triangleVertexList[i].A;
        const PXVector3F32 PXREF vertex1 = &triangleVertexList[i].B;
        const PXVector3F32 PXREF vertex2 = &triangleVertexList[i].C;
        PXVector3F32 edge1;
        PXVector3F32 edge2;
        PXVector3F32 h;
        PXVector3F32 s;
        PXVector3F32 q;
        PXF32 allignment;
        PXF32 allignmentInverse;
        PXF32 u;
        PXF32 v;


        // Failsafe origin
        PXVector3F32 failsafeOrigin;

        if(!rayOriginDirection)
        {
            rayOriginDirection = &failsafeOrigin;

            PXVector3F32Set(rayOriginDirection, rayOriginPositon);
            PXVector3F32Substract(rayOriginDirection, vertex0);
        }

        PXVector3F32Set(&edge1, vertex1);
        PXVector3F32Substract(&edge1, vertex0);

        PXVector3F32Set(&edge2, vertex2);
        PXVector3F32Substract(&edge2, vertex0);

        PXVector3F32CrossProduct(&h, rayOriginDirection, &edge2);
        allignment = PXVector3F32DotProduct(&h, &edge1);

        const PXBool isRayParralel = allignment > -EPSILON && allignment < EPSILON; // This ray is parallel to this triangle.

        if(isRayParralel)
            return PXRayNoHit;

        allignmentInverse = 1.0f / allignment;

        PXVector3F32Set(&s, rayOriginPositon);
        PXVector3F32Substract(&s, vertex0);

        u = allignmentInverse * PXVector3F32DotProduct(&s, &h);

        const PXBool isSomething = u < 0.0 || u > 1.0; // Check this

        if(isSomething)
            return PXRayNoHit;


        PXVector3F32CrossProduct(&q, &s, &edge1);
        v = allignmentInverse * PXVector3F32DotProduct(&q, rayOriginDirection);


        const PXBool isSomethingB = v < 0.0 || u + v > 1.0; // Check this

        if(isSomethingB)
            return PXRayNoHit;

        // At this stage we can compute t to find out where the intersection point is on the line.
        const PXF32 t = allignmentInverse * PXVector3F32DotProduct(&q, &edge2);
        const PXBool rayHit = t > EPSILON; // ray intersection

        if(!rayHit)
        {
            // This means that there is a line intersection but not a ray intersection.
            return PXRayNoHit;
        }

        return t;
    }
}