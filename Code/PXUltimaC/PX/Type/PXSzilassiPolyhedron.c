#include "PXSzilassiPolyhedron.h"

#include <PX/Type/PXVertex.h>

const char PXSzilassiPolyhedronName[] = "Tesseract";
const PXI8U PXSzilassiPolyhedronLength = sizeof(PXSzilassiPolyhedronName);
const PXECSRegisterInfoStatic PXSzilassiPolyhedronRegisterInfoStatic =
{
    {sizeof(PXSzilassiPolyhedronName), sizeof(PXSzilassiPolyhedronName), PXSzilassiPolyhedronName, TextFormatASCII},
    sizeof(PXSzilassiPolyhedron),
    __alignof(PXSzilassiPolyhedron),
    PXECSTypeEntity,
    PXSzilassiPolyhedronCreate,
    PXSzilassiPolyhedronRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXSzilassiPolyhedronRegisterInfoDynamic;


const PXVertexP3N3T2F32 PXSzilassiPolyhedronVertexData[42] =
{
    // Format per entry: {pX, pY, pZ,  nX, nY, nZ,  tU, tV}
    // Face 1 (Triangles)
    {-12.0, 0.0, 12.0, 0.0, 1.0, 0.0, 0.1, 0.1},
    {12.0, 0.0, 12.0, 0.0, 1.0, 0.0, 0.9, 0.1},
    {-4.0, -12.0, 4.0, 0.0, 1.0, 0.0, 0.3, 0.8},

    {-4.0, -12.0, 4.0, 0.0, 1.0, 0.0, 0.3, 0.8},
    {12.0, 0.0, 12.0, 0.0, 1.0, 0.0, 0.9, 0.1},
    {4.0, -12.0, 4.0, 0.0, 1.0, 0.0, 0.7, 0.8},

    // Face 2 (Triangles)
    {12.0, 0.0, 12.0, 0.7, 0.0, 0.7, 0.1, 0.2},
    {12.0, 0.0, -12.0, 0.7, 0.0, 0.7, 0.9, 0.2},
    {4.0, -12.0, 4.0, 0.7, 0.0, 0.7, 0.2, 0.9},

    {4.0, -12.0, 4.0, 0.7, 0.0, 0.7, 0.2, 0.9},
    {12.0, 0.0, -12.0, 0.7, 0.0, 0.7, 0.9, 0.2},
    {4.0, -12.0, -4.0, 0.7, 0.0, 0.7, 0.8, 0.9},

    // Face 3 (Triangles)
    {12.0, 0.0, -12.0, 0.0, -1.0, 0.0, 0.1, 0.1},
    {-12.0, 0.0, -12.0, 0.0, -1.0, 0.0, 0.9, 0.1},
    {4.0, -12.0, -4.0, 0.0, -1.0, 0.0, 0.3, 0.9},

    {4.0, -12.0, -4.0, 0.0, -1.0, 0.0, 0.3, 0.9},
    {-12.0, 0.0, -12.0, 0.0, -1.0, 0.0, 0.9, 0.1},
    {-4.0, -12.0, -4.0, 0.0, -1.0, 0.0, 0.7, 0.9},

    // Face 4 (Triangles)
    {-12.0, 0.0, -12.0, -0.7, 0.0, -0.7, 0.1, 0.2},
    {-12.0, 0.0, 12.0, -0.7, 0.0, -0.7, 0.9, 0.2},
    {-4.0, -12.0, -4.0, -0.7, 0.0, -0.7, 0.2, 0.9},

    {-4.0, -12.0, -4.0, -0.7, 0.0, -0.7, 0.2, 0.9},
    {-12.0, 0.0, 12.0, -0.7, 0.0, -0.7, 0.9, 0.2},
    {-4.0, -12.0, 4.0, -0.7, 0.0, -0.7, 0.8, 0.9},

    // Face 5 (Inner Tunnel Triangles)
    {-4.0, 12.0, 4.0, 0.0, 0.0, 1.0, 0.2, 0.2},
    {4.0, 12.0, 4.0, 0.0, 0.0, 1.0, 0.8, 0.2},
    {-4.0, -12.0, 4.0, 0.0, 0.0, 1.0, 0.2, 0.8},

    {-4.0, -12.0, 4.0, 0.0, 0.0, 1.0, 0.2, 0.8},
    {4.0, 12.0, 4.0, 0.0, 0.0, 1.0, 0.8, 0.2},
    {4.0, -12.0, 4.0, 0.0, 0.0, 1.0, 0.8, 0.8},

    // Face 6 (Inner Tunnel Triangles)
    {4.0, 12.0, -4.0, 0.0, 0.0, -1.0, 0.2, 0.2},
    {-4.0, 12.0, -4.0, 0.0, 0.0, -1.0, 0.8, 0.2},
    {4.0, -12.0, -4.0, 0.0, 0.0, -1.0, 0.2, 0.8},

    {4.0, -12.0, -4.0, 0.0, 0.0, -1.0, 0.2, 0.8},
    {-4.0, 12.0, -4.0, 0.0, 0.0, -1.0, 0.8, 0.2},
    {-4.0, -12.0, -4.0, -0.0, 0.0, -1.0, 0.8, 0.8},

    // Face 7 (Bridge Triangles)
    {-4.0, 12.0, -4.0, -1.0, 0.0, 0.0, 0.1, 0.3},
    {-4.0, 12.0, 4.0, -1.0, 0.0, 0.0, 0.9, 0.3},
    {-12.0, 0.0, -12.0, -1.0, 0.0, 0.0, 0.1, 0.7},

    {-12.0, 0.0, -12.0, -1.0, 0.0, 0.0, 0.1, 0.7},
    {-4.0, 12.0, 4.0, -1.0, 0.0, 0.0, 0.9, 0.3},
    {-12.0, 0.0, 12.0, -1.0, 0.0, 0.0, 0.9, 0.7}
};

const PXI8U PXSzilassiPolyhedronIndexData[42] =
{
    0, 1, 2, 3, 4, 5,   // Face 1
    6, 7, 8, 9, 10, 11,   // Face 2
    12, 13, 14, 15, 16, 17,   // Face 3
    18, 19, 20, 21, 22, 23,   // Face 4
    24, 25, 26, 27, 28, 29,   // Face 5
    30, 31, 32, 33, 34, 35,   // Face 6
    36, 37, 38, 39, 40, 41    // Face 7
};


void PXAPI PXSzilassiPolyhedronRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXSzilassiPolyhedronRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXSzilassiPolyhedronRegisterInfoDynamic;
}

PXResult PXAPI PXSzilassiPolyhedronCreate(PXSzilassiPolyhedron** pxSzilassiPolyhedronREF, PXSzilassiPolyhedronCreateInfo PXREF pxSzilassiPolyhedronCreateInfo)
{
    PXSzilassiPolyhedron* pxSzilassiPolyhedron = PXNull;

    if(!(pxSzilassiPolyhedronREF && pxSzilassiPolyhedronCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxSzilassiPolyhedronCreateInfo->Info.Static = &PXSzilassiPolyhedronRegisterInfoStatic;
    pxSzilassiPolyhedronCreateInfo->Info.Dynamic = &PXSzilassiPolyhedronRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxSzilassiPolyhedronREF, pxSzilassiPolyhedronCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxSzilassiPolyhedron = *pxSzilassiPolyhedronREF;

    return PXResultOK;
}

PXResult PXAPI PXSzilassiPolyhedronRelease(PXSzilassiPolyhedron PXREF pxSzilassiPolyhedron)
{
    return PXResultOK;
}