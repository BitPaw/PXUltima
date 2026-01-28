#include "PXIconSun.h"
#include <gl/GL.h>


const char PXIconSunName[] = "IconSun";
const PXI8U PXIconSunNameLength = sizeof(PXIconSunName);
const PXECSRegisterInfoStatic PXIconSunRegisterInfoStatic =
{
    {sizeof(PXIconSunName), sizeof(PXIconSunName), PXIconSunName, TextFormatASCII},
    sizeof(PXIconSun),
    __alignof(PXIconSun),
    PXECSTypeEntity,
    PXIconSunCreate,
    PXIconSunRelease,
    PXIconSunDraw
};
PXECSRegisterInfoDynamic PXIconSunRegisterInfoDynamic;

PXResult PXAPI PXIconSunRegisterToECS(void)
{
    PXECSRegister(&PXIconSunRegisterInfoStatic, &PXIconSunRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXIconSunCreate(PXIconSun** pxIconSunREF, PXIconSunCreateInfo PXREF pxIconSunCreateInfo)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXIconSunRelease(PXIconSun PXREF pxIconSun)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXIconSunDraw(PXIconSun PXREF pxIconSun, PXDrawInfo PXREF pxDrawInfo)
{
#if 0
    PXVector3F32 center;
    float size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int circleSegments;
    int rayCount;

    if(circleSegments < 12) circleSegments = 12;
    if(rayCount < 4) rayCount = 4;

    PXVector3F32 camRight, camUp;       

    //get_camera_axes_from_modelview(&camRight, &camUp);

    float R = size * 0.35f;   // circle radius
    float r0 = size * 0.45f;  // ray inner radius
    float r1 = size * 0.65f;  // ray outer radius

    // Worst-case vertices: circleSegments edges + rayCount rays
    // Each edge/ray = 2 vertices
    int maxLines = circleSegments + rayCount;
    LineVtx* v = (LineVtx*)alloca(sizeof(LineVtx) * maxLines * 2);
    int n = 0;

    // Circle
    float step = (float)(2.0 * M_PI / (double)circleSegments);
    for(int i = 0; i < circleSegments; i++) {
        float t0 = step * (float)i;
        float t1 = step * (float)(i + 1);
        float x0 = cosf(t0) * R, y0 = sinf(t0) * R;
        float x1 = cosf(t1) * R, y1 = sinf(t1) * R;

        PXVector3F32 p0 = billboard_point(center, camRight, camUp, x0, y0);
        PXVector3F32 p1 = billboard_point(center, camRight, camUp, x1, y1);
        push_line(v, &n, r, g, b, a, p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
    }

    // Rays
    for(int i = 0; i < rayCount; i++) {
        float t = (float)(2.0 * M_PI * (double)i / (double)rayCount);
        float x0 = cosf(t) * r0, y0 = sinf(t) * r0;
        float x1 = cosf(t) * r1, y1 = sinf(t) * r1;

        PXVector3F32 p0 = billboard_point(center, camRight, camUp, x0, y0);
        PXVector3F32 p1 = billboard_point(center, camRight, camUp, x1, y1);
        push_line(v, &n, r, g, b, a, p0.x, p0.y, p0.z, p1.x, p1.y, p1.z);
    }

    // Render
    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // For "draw on top", choose ONE:
    // glDisable(GL_DEPTH_TEST);
    // or:
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glLineWidth(2.0f);

    glInterleavedArrays(GL_C4UB_V3F, 0, (const GLvoid*)v);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_LINES, 0, n);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDepthMask(GL_TRUE);
    glPopAttrib();
#endif

    return PXResultOK;
}
