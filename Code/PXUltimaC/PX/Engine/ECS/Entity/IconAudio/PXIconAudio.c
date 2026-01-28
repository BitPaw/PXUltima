#include "PXIconAudio.h"
#include <gl/GL.h>

#include <PX/Math/PXMath.h>
#include <PX/Math/PXVector.h>

const char PXIconAudioName[] = "IconAudio";
const PXI8U PXIconAudioNameLength = sizeof(PXIconAudioName);
const PXECSRegisterInfoStatic PXIconAudioRegisterInfoStatic =
{
    {sizeof(PXIconAudioName), sizeof(PXIconAudioName), PXIconAudioName, TextFormatASCII},
    sizeof(PXIconAudio),
    __alignof(PXIconAudio),
    PXECSTypeEntity,
    PXIconAudioCreate,
    PXIconAudioRelease,
    PXIconAudioDraw
};
PXECSRegisterInfoDynamic PXIconAudioRegisterInfoDynamic;

PXResult PXAPI PXIconAudioRegisterToECS(void)
{
    PXECSRegister(&PXIconAudioRegisterInfoStatic, &PXIconAudioRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXIconAudioCreate(PXIconAudio** pxIconAudioREF, PXIconAudioCreateInfo PXREF pxIconAudioCreateInfo)
{
    PXIconAudio* pxIconAudio = PXNull;

    pxIconAudioCreateInfo->Info.Static = &PXIconAudioRegisterInfoStatic;
    pxIconAudioCreateInfo->Info.Dynamic = &PXIconAudioRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxIconAudioREF, pxIconAudioCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxIconAudio = *pxIconAudioREF;

    return PXResultOK;
}

PXResult PXAPI PXIconAudioRelease(PXIconAudio PXREF pxIconAudio)
{
    return PXActionRefusedNotImplemented;
}


PXVector3F32 v3_add(PXVector3F32 a, PXVector3F32 b) 
{
    PXVector3F32 r = { a.X + b.X,a.Y + b.Y,a.Z + b.Z };
    return r; 
}
PXVector3F32 v3_mul(PXVector3F32 a, float s) 
{
    PXVector3F32 r = { a.X * s,a.Y * s,a.Z * s }; 
    return r;
}

typedef struct {
    unsigned char r, g, b, a;
    float x, y, z;
} LineVtx;

/* Convert 2D billboard coordinates (sx,sy) to world point */
PXVector3F32 billboard_point(PXVector3F32 center, PXVector3F32 right, PXVector3F32 up, float sx, float sy)
{
    return v3_add(center, v3_add(v3_mul(right, sx), v3_mul(up, sy)));
}

void push_line(LineVtx* v, int* n,
                      unsigned char r, unsigned char g, unsigned char b, unsigned char a,
               PXVector3F32 p0, PXVector3F32 p1)
{
    v[*n + 0].r = r; v[*n + 0].g = g; v[*n + 0].b = b; v[*n + 0].a = a;
    v[*n + 0].x = p0.X; v[*n + 0].y = p0.Y; v[*n + 0].z = p0.Z;

    v[*n + 1].r = r; v[*n + 1].g = g; v[*n + 1].b = b; v[*n + 1].a = a;
    v[*n + 1].x = p1.X; v[*n + 1].y = p1.Y; v[*n + 1].z = p1.Z;

    *n += 2;
}

PXResult PXAPI PXIconAudioDraw(PXIconAudio PXREF pxIconAudio, PXDrawInfo PXREF pxDrawInfo)
{
    PXVector3F32 center;
    float size;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int waveSegments;

    if(waveSegments < 8) 
        waveSegments = 8;

    PXVector3F32 camRight, camUp;
   // get_camera_axes_from_modelview(&camRight, &camUp);

    // Local 2D layout on billboard (units are in world space scaled by size)
    float w = size * 0.55f;   // width of speaker body
    float h = size * 0.60f;   // height
    float xL = -size * 0.55f; // left edge
    float xR = xL + w;        // right edge of body
    float yB = -h * 0.5f;
    float yT = +h * 0.5f;

    // Cone points (to the right)
    float coneX = xR + size * 0.25f;
    float coneY0 = -h * 0.30f;
    float coneY1 = +h * 0.30f;

    // Waves radii
    float wx0 = xR + size * 0.30f;
    float waveR1 = size * 0.35f;
    float waveR2 = size * 0.55f;

    // Estimate max lines:
    // body rect 4 + cone 3 + wave arcs (2 arcs * waveSegments)
    int maxLines = 4 + 3 + 2 * waveSegments;
    LineVtx* vtx = (LineVtx*)alloca(sizeof(LineVtx) * maxLines * 2);
    int n = 0;

    // Body rectangle
    PXVector3F32 p0 = billboard_point(center, camRight, camUp, xL, yB);
    PXVector3F32 p1 = billboard_point(center, camRight, camUp, xR, yB);
    PXVector3F32 p2 = billboard_point(center, camRight, camUp, xR, yT);
    PXVector3F32 p3 = billboard_point(center, camRight, camUp, xL, yT);

    push_line(vtx, &n, r, g, b, a, p0, p1);
    push_line(vtx, &n, r, g, b, a, p1, p2);
    push_line(vtx, &n, r, g, b, a, p2, p3);
    push_line(vtx, &n, r, g, b, a, p3, p0);

    // Cone outline (a trapezoid/triangle-ish)
    PXVector3F32 c0 = billboard_point(center, camRight, camUp, xR, coneY0);
    PXVector3F32 c1 = billboard_point(center, camRight, camUp, xR, coneY1);
    PXVector3F32 c2 = billboard_point(center, camRight, camUp, coneX, 0.0f);

    push_line(vtx, &n, r, g, b, a, c0, c2);
    push_line(vtx, &n, r, g, b, a, c2, c1);
    push_line(vtx, &n, r, g, b, a, c1, c0);

    // Sound waves: draw arcs centered roughly at (wx0, 0)
    // Arc from -45° to +45° (like typical speaker icon)
    float ang0 = (float)(-PXMathConstantPI * 0.25);
    float ang1 = (float)(+PXMathConstantPI * 0.25);
    for(int wave = 0; wave < 2; ++wave) {
        float R = (wave == 0) ? waveR1 : waveR2;
        float step = (ang1 - ang0) / (float)waveSegments;

        float t0 = ang0;
        float x0 = wx0 + cosf(t0) * R;
        float y0 = sinf(t0) * R;

        for(int i = 1; i <= waveSegments; i++) {
            float t1 = ang0 + step * (float)i;
            float x1 = wx0 + cosf(t1) * R;
            float y1 = sinf(t1) * R;

            PXVector3F32 a0p = billboard_point(center, camRight, camUp, x0, y0);
            PXVector3F32 a1p = billboard_point(center, camRight, camUp, x1, y1);

            push_line(vtx, &n, r, g, b, a, a0p, a1p);

            x0 = x1; y0 = y1;
        }
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

    glInterleavedArrays(GL_C4UB_V3F, 0, (const GLvoid*)vtx);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_LINES, 0, n);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glDepthMask(GL_TRUE);
    glPopAttrib();

    return PXResultOK;
}