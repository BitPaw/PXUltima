#include "PXCone.h"

#include <PX/Math/PXMath.h>

PXResult PXAPI PXConeCreate(PXModel PXREF pxModel, PXConeCreateInfo PXREF pxConeCreateInfo)
{
    // ---------- top ring ----------
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i) 
    {
        float theta = 2 * PXMathPI * i / pxConeCreateInfo->Segments;
        float x = pxConeCreateInfo->Radius * PXMathCosinusRADF32(theta);
        float y = pxConeCreateInfo->Radius * PXMathSinusRADF32(theta);

#if 0
        p.verts.push_back(makeVertex(x, y, pxConeCreateInfo->Height,
                          0.f, 0.f, 1.f,
                          (theta / (2 * PI)), 1.f));
#endif
    }

    // ---------- bottom ring ----------
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXF32 theta = 2 * PXMathPI * i / pxConeCreateInfo->Segments;
        PXF32 x = pxConeCreateInfo->Radius * PXMathCosinusRADF32(theta);
        PXF32 y = pxConeCreateInfo->Radius * PXMathSinusRADF32(theta);

#if 0
        p.verts.push_back(makeVertex(x, y, 0.f,
                          0.f, 0.f, -1.f,
                          (theta / (2 * PI)), 0.f));
#endif
    }

    // ---------- side vertices (duplicate for correct normals) ----------
    PXSize topStart = 0;
    PXSize bottomStart = pxConeCreateInfo->Segments;
    PXSize sideTopStart = 2 * pxConeCreateInfo->Segments;      // first side vertex (top rim)
    PXSize sideBottomStart = 2 * pxConeCreateInfo->Segments + pxConeCreateInfo->Segments;

    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXF32 theta = 2 * PXMathPI * i / pxConeCreateInfo->Segments;
        PXF32 x = pxConeCreateInfo->Radius * PXMathCosinusRADF32(theta);
        PXF32 y = pxConeCreateInfo->Radius * PXMathSinusRADF32(theta);

#if 0
        // side normal is radial outward
        p.verts.push_back(makeVertex(x, y, pxConeCreateInfo->Height,
                          x / pxConeCreateInfo->Radius, y / pxConeCreateInfo->Radius, 0.f,
                          (theta / (2 * PI)), 1.f));
#endif
    }
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXF32 theta = 2 * PXMathPI * i / pxConeCreateInfo->Segments;
        PXF32 x = pxConeCreateInfo->Radius * PXMathCosinusRADF32(theta);
        PXF32 y = pxConeCreateInfo->Radius * PXMathSinusRADF32(theta);

#if 0
        p.verts.push_back(makeVertex(x, y, 0.f,
                          x / pxConeCreateInfo->Radius, y / pxConeCreateInfo->Radius, 0.f,
                          (theta / (2 * PI)), 0.f));
#endif
    }

    // ---------- indices ----------
    PXSize topRingStart = 0;
    PXSize bottomRingStart = pxConeCreateInfo->Segments;
    sideTopStart = 2 * pxConeCreateInfo->Segments;
    sideBottomStart = 3 * pxConeCreateInfo->Segments;

    // top face
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXSize next = (i + 1) % pxConeCreateInfo->Segments;

#if 0
        p.inds.push_back(topRingStart + i);
        p.inds.push_back(topRingStart + next);
        p.inds.push_back(bottomRingStart + next);   // winding order counter?clockwise when seen from above
#endif
    }

    // bottom face
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXSize next = (i + 1) % pxConeCreateInfo->Segments;

#if 0
        p.inds.push_back(bottomRingStart + i);
        p.inds.push_back(bottomRingStart + next);
        p.inds.push_back(topRingStart + next);     // opposite winding
#endif
    }

    // side surface
    for(PXSize i = 0; i < pxConeCreateInfo->Segments; ++i)
    {
        PXSize next = (i + 1) % pxConeCreateInfo->Segments;

#if 0
        // first triangle of quad
        p.inds.push_back(sideTopStart + i);
        p.inds.push_back(sideBottomStart + i);
        p.inds.push_back(sideBottomStart + next);

        // second triangle
        p.inds.push_back(sideTopStart + i);
        p.inds.push_back(sideBottomStart + next);
        p.inds.push_back(sideTopStart + next);
#endif
    }

    return PXResultOK;
}