#include "PXCollision.h"

#include <Math/PXMath.h>

PXBool PXAPI PXCollisionAABB(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
    const PXBool xInRage = PXMathIsInRange(cx, ax, bx) || PXMathIsInRange(dx, ax, bx);
    const PXBool yInRage = PXMathIsInRange(cy, ay, by) || PXMathIsInRange(dy, ay, by);
    const PXBool isFullyColliding = xInRage && yInRage;

    return isFullyColliding;
}


#define PXRayNoHit -1.0f
const float EPSILON = 0.0000001f;


float PXAPI PXRayTriangleIntersect
(
    PXVector3F* const rayOriginPositon,
    PXVector3F* const rayOriginDirection,
    PXVector3F* const triangleVertexList
)
{
    // Möller–Trumbore intersection

    const PXVector3F* const vertex0 = &triangleVertexList[0];
    const PXVector3F* const vertex1 = &triangleVertexList[1];
    const PXVector3F* const vertex2 = &triangleVertexList[2];
    PXVector3F edge1; 
    PXVector3F edge2;
    PXVector3F h;
    PXVector3F s;
    PXVector3F q;
    float allignment;
    float allignmentInverse; 
    float u; 
    float v;

    PXVector3FSet(&edge1, vertex1);
    PXVector3FSubstract(&edge1, vertex0);

    PXVector3FSet(&edge2, vertex2);
    PXVector3FSubstract(&edge2, vertex0);

    PXVector3FCrossProduct(&h, rayOriginDirection, &edge2);
    allignment = PXVector3FDotProduct(&h, &edge1);
   
    const PXBool isRayParralel = allignment > -EPSILON && allignment < EPSILON; // This ray is parallel to this triangle.

    if(isRayParralel)
        return PXRayNoHit; 

    allignmentInverse = 1.0f / allignment;

    PXVector3FSet(&s, rayOriginPositon);
    PXVector3FSubstract(&s, vertex0);

    u = allignmentInverse * PXVector3FDotProduct(&s, &h);

    const PXBool isSomething = u < 0.0 || u > 1.0; // Check this

    if(isSomething)
        return PXRayNoHit;


    PXVector3FCrossProduct(&q, &s, &edge1);
    v = allignmentInverse * PXVector3FDotProduct(&q, rayOriginDirection);


    const PXBool isSomethingB = v < 0.0 || u + v > 1.0; // Check this

    if(isSomethingB)
        return PXRayNoHit;

    // At this stage we can compute t to find out where the intersection point is on the line.
    const float t = allignmentInverse * PXVector3FDotProduct(&q, &edge2);
    const PXBool rayHit = t > EPSILON; // ray intersection

    if(!rayHit) 
    { 
        // This means that there is a line intersection but not a ray intersection.
        return PXRayNoHit;
    }

    return t;
}