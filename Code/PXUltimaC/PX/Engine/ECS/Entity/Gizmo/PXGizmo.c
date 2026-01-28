#include "PXGizmo.h"
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

const char PXGizmoName[] = "Gizmo";
const PXI8U PXGizmoNameLength = sizeof(PXGizmoName);
const PXECSRegisterInfoStatic PXGizmoRegisterInfoStatic =
{
    {sizeof(PXGizmoName), sizeof(PXGizmoName), PXGizmoName, TextFormatASCII},
    sizeof(PXGizmo),
    __alignof(PXGizmo),
    PXECSTypeEntity,
    PXGizmoCreate,
    PXGizmoRelease,
    PXGizmoDraw
};
PXECSRegisterInfoDynamic PXGizmoRegisterInfoDynamic;

PXResult PXAPI PXGizmoRegisterToECS(void)
{
    PXECSRegister(&PXGizmoRegisterInfoStatic, &PXGizmoRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXGizmoCreate(PXGizmo** pxGizmoREF, PXGizmoCreateInfo PXREF pxGizmoCreateInfo)
{
    PXGizmo* pxGizmo = PXNull;

    pxGizmoCreateInfo->Info.Static = &PXGizmoRegisterInfoStatic;
    pxGizmoCreateInfo->Info.Dynamic = &PXGizmoRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxGizmoREF, pxGizmoCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxGizmo = *pxGizmoREF;

    return PXResultOK;
}

PXResult PXAPI PXGizmoRelease(PXGizmo PXREF pxGizmo)
{
    return PXActionRefusedNotImplemented;
}



















/* gizmo_render.c – OpenGL 1.1 gizmo rendering (translation + scale + rotation + optional 4D) */

#include <math.h>
#include <GL/gl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    GLfloat x, y, z;
    GLfloat r, g, b;
} GizmoVertex;

/* ---------- CONFIG ---------- */

static const GLfloat GIZMO_AXIS_LEN = 1.0f;   /* from 0 to +1, but we draw -1..+1 */
static const GLfloat GIZMO_HALF_AXIS_LEN = 1.0f;
static const GLfloat GIZMO_ARROW_SIZE = 0.08f;
static const GLfloat GIZMO_BOX_SIZE = 0.10f;
static const GLfloat GIZMO_CIRCLE_R = 0.8f;
static const GLfloat GIZMO_CIRCLE_R_4D = 0.85f;
static const int     GIZMO_CIRCLE_SEG = 64;

/* Axis colors: X=red, Y=green, Z=blue, W=purple */
static const GLfloat AXIS_COLOR_X[3] = { 1.0f, 0.0f, 0.0f };
static const GLfloat AXIS_COLOR_Y[3] = { 0.0f, 1.0f, 0.0f };
static const GLfloat AXIS_COLOR_Z[3] = { 0.0f, 0.0f, 1.0f };
static const GLfloat AXIS_COLOR_W[3] = { 0.7f, 0.0f, 0.7f };

/* Plane colors for scaling rectangles (slightly dimmer) */
static const GLfloat PLANE_COLOR_XY[3] = { 0.7f, 0.7f, 0.0f };
static const GLfloat PLANE_COLOR_XZ[3] = { 0.7f, 0.0f, 0.7f };
static const GLfloat PLANE_COLOR_YZ[3] = { 0.0f, 0.7f, 0.7f };
static const GLfloat PLANE_COLOR_XW[3] = { 0.7f, 0.4f, 0.7f };
static const GLfloat PLANE_COLOR_YW[3] = { 0.4f, 0.7f, 0.7f };
static const GLfloat PLANE_COLOR_ZW[3] = { 0.7f, 0.7f, 1.0f };

/* ---------- INTERNAL HELPERS (CLIENT ARRAYS) ---------- */

static void gizmo_enable_arrays(const GizmoVertex* verts) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(GizmoVertex), &verts[0].x);
    glColorPointer(3, GL_FLOAT, sizeof(GizmoVertex), &verts[0].r);
}

static void gizmo_disable_arrays(void) {
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

/* Build a simple line (two vertices) */
static void gizmo_build_line(GizmoVertex* v,
                             GLfloat x0, GLfloat y0, GLfloat z0,
                             GLfloat x1, GLfloat y1, GLfloat z1,
                             const GLfloat color[3]) {
    v[0].x = x0; v[0].y = y0; v[0].z = z0;
    v[0].r = color[0]; v[0].g = color[1]; v[0].b = color[2];

    v[1].x = x1; v[1].y = y1; v[1].z = z1;
    v[1].r = color[0]; v[1].g = color[1]; v[1].b = color[2];
}

/* Build a box centered at (cx,cy,cz) with half-size s */
static int gizmo_build_box(GizmoVertex* v,
                           GLfloat cx, GLfloat cy, GLfloat cz,
                           GLfloat s,
                           const GLfloat color[3]) {
    /* 8 vertices, 12 edges (we draw as GL_LINES) */
    GLfloat x0 = cx - s, x1 = cx + s;
    GLfloat y0 = cy - s, y1 = cy + s;
    GLfloat z0 = cz - s, z1 = cz + s;

    GizmoVertex tmp[8];
    int i = 0;

    tmp[0].x = x0; tmp[0].y = y0; tmp[0].z = z0;
    tmp[1].x = x1; tmp[1].y = y0; tmp[1].z = z0;
    tmp[2].x = x1; tmp[2].y = y1; tmp[2].z = z0;
    tmp[3].x = x0; tmp[3].y = y1; tmp[3].z = z0;
    tmp[4].x = x0; tmp[4].y = y0; tmp[4].z = z1;
    tmp[5].x = x1; tmp[5].y = y0; tmp[5].z = z1;
    tmp[6].x = x1; tmp[6].y = y1; tmp[6].z = z1;
    tmp[7].x = x0; tmp[7].y = y1; tmp[7].z = z1;

    for(i = 0; i < 8; ++i) {
        tmp[i].r = color[0];
        tmp[i].g = color[1];
        tmp[i].b = color[2];
    }

    /* edges indices */
    static const int e[24] = {
        0,1, 1,2, 2,3, 3,0,
        4,5, 5,6, 6,7, 7,4,
        0,4, 1,5, 2,6, 3,7
    };

    for(i = 0; i < 24; ++i) {
        v[i] = tmp[e[i]];
    }

    return 24; /* number of vertices */
}

/* Build a circle in a given plane, centered at origin, radius r */
static int gizmo_build_circle(GizmoVertex* v,
                              GLfloat r,
                              const GLfloat color[3],
                              char plane) {
    /* plane: 'X','Y','Z' meaning:
       'X' -> circle in YZ plane
       'Y' -> circle in XZ plane
       'Z' -> circle in XY plane
    */
    int i;
    for(i = 0; i < GIZMO_CIRCLE_SEG; ++i) {
        float t0 = (float)i * (2.0f * (float)M_PI / (float)GIZMO_CIRCLE_SEG);
        float t1 = (float)(i + 1) * (2.0f * (float)M_PI / (float)GIZMO_CIRCLE_SEG);

        float c0 = cosf(t0), s0 = sinf(t0);
        float c1 = cosf(t1), s1 = sinf(t1);

        GizmoVertex v0, v1;

        if(plane == 'X') { /* YZ */
            v0.x = 0.0f; v0.y = r * c0; v0.z = r * s0;
            v1.x = 0.0f; v1.y = r * c1; v1.z = r * s1;
        }
        else if(plane == 'Y') { /* XZ */
            v0.x = r * c0; v0.y = 0.0f; v0.z = r * s0;
            v1.x = r * c1; v1.y = 0.0f; v1.z = r * s1;
        }
        else { /* 'Z' -> XY */
            v0.x = r * c0; v0.y = r * s0; v0.z = 0.0f;
            v1.x = r * c1; v1.y = r * s1; v1.z = 0.0f;
        }

        v0.r = color[0]; v0.g = color[1]; v0.b = color[2];
        v1.r = color[0]; v1.g = color[1]; v1.b = color[2];

        v[2 * i + 0] = v0;
        v[2 * i + 1] = v1;
    }
    return 2 * GIZMO_CIRCLE_SEG;
}

/* Build a plane scaling rectangle inside a circle (simple quad) */
static int gizmo_build_plane_rect(GizmoVertex* v,
                                  GLfloat size,
                                  const GLfloat color[3],
                                  const char* plane) {
    /* plane: "XY", "XZ", "YZ" etc. size is half-extent */
    GLfloat x0 = -size, x1 = size;
    GLfloat y0 = -size, y1 = size;

    GizmoVertex tmp[4];

    if(plane[0] == 'X' && plane[1] == 'Y') {
        tmp[0].x = x0; tmp[0].y = y0; tmp[0].z = 0.0f;
        tmp[1].x = x1; tmp[1].y = y0; tmp[1].z = 0.0f;
        tmp[2].x = x1; tmp[2].y = y1; tmp[2].z = 0.0f;
        tmp[3].x = x0; tmp[3].y = y1; tmp[3].z = 0.0f;
    }
    else if(plane[0] == 'X' && plane[1] == 'Z') {
        tmp[0].x = x0; tmp[0].y = 0.0f; tmp[0].z = y0;
        tmp[1].x = x1; tmp[1].y = 0.0f; tmp[1].z = y0;
        tmp[2].x = x1; tmp[2].y = 0.0f; tmp[2].z = y1;
        tmp[3].x = x0; tmp[3].y = 0.0f; tmp[3].z = y1;
    }
    else if(plane[0] == 'Y' && plane[1] == 'Z') {
        tmp[0].x = 0.0f; tmp[0].y = x0; tmp[0].z = y0;
        tmp[1].x = 0.0f; tmp[1].y = x1; tmp[1].z = y0;
        tmp[2].x = 0.0f; tmp[2].y = x1; tmp[2].z = y1;
        tmp[3].x = 0.0f; tmp[3].y = x0; tmp[3].z = y1;
    }
    else {
        /* For 4D planes (XW, YW, ZW) we just reuse XY/XZ/YZ visually.
           You can adapt this to your 4D projection scheme. */
        tmp[0].x = x0; tmp[0].y = y0; tmp[0].z = 0.0f;
        tmp[1].x = x1; tmp[1].y = y0; tmp[1].z = 0.0f;
        tmp[2].x = x1; tmp[2].y = y1; tmp[2].z = 0.0f;
        tmp[3].x = x0; tmp[3].y = y1; tmp[3].z = 0.0f;
    }

    for(int i = 0; i < 4; ++i) {
        tmp[i].r = color[0];
        tmp[i].g = color[1];
        tmp[i].b = color[2];
    }

    /* draw as GL_LINE_LOOP (4 vertices) */
    v[0] = tmp[0];
    v[1] = tmp[1];
    v[2] = tmp[2];
    v[3] = tmp[3];

    return 4;
}













static int gizmo_build_diamond_shaft(GizmoVertex* v,
                                     float length,
                                     float radius,
                                     const float color[3])
{
    /* Diamond shaft along +X axis:
       Center line from 0 ? length
       Cross-section is a diamond (4 points)
    */

    float x0 = 0.0f;
    float x1 = length;

    /* 4 cross-section points around the axis */
    float y = radius;
    float z = radius;

    /* 8 vertices: 4 at x0, 4 at x1 */
    GizmoVertex p[8] = {
        {x0,  y, 0,  color[0],color[1],color[2]},
        {x0, 0,  z,  color[0],color[1],color[2]},
        {x0, -y,0,  color[0],color[1],color[2]},
        {x0, 0, -z, color[0],color[1],color[2]},

        {x1,  y, 0,  color[0],color[1],color[2]},
        {x1, 0,  z,  color[0],color[1],color[2]},
        {x1, -y,0,  color[0],color[1],color[2]},
        {x1, 0, -z, color[0],color[1],color[2]},
    };

    /* 4 quads ? 8 triangles */
    int idx[24] = {
        0,1,4,  1,5,4,
        1,2,5,  2,6,5,
        2,3,6,  3,7,6,
        3,0,7,  0,4,7
    };

    for(int i = 0; i < 24; ++i)
        v[i] = p[idx[i]];

    return 24; /* number of vertices */
}


static int gizmo_build_pyramid_tip(GizmoVertex* v,
                                   float baseX,
                                   float size,
                                   float length,
                                   const float color[3])
{
    /* Square base centered at baseX, pyramid tip at baseX + length */

    float x0 = baseX;
    float x1 = baseX + length;

    float s = size;

    GizmoVertex p[5] = {
        {x0, -s, -s, color[0],color[1],color[2]}, /* base 0 */
        {x0,  s, -s, color[0],color[1],color[2]}, /* base 1 */
        {x0,  s,  s, color[0],color[1],color[2]}, /* base 2 */
        {x0, -s,  s, color[0],color[1],color[2]}, /* base 3 */
        {x1,  0,  0, color[0],color[1],color[2]}, /* tip */
    };

    int idx[12] = {
        0,1,4,
        1,2,4,
        2,3,4,
        3,0,4
    };

    for(int i = 0; i < 12; ++i)
        v[i] = p[idx[i]];

    return 12;
}



static void gizmo_draw_solid_arrow(const float origin[3],
                                   const float color[3],
                                   int enableBox)
{
    GizmoVertex verts[256];
    int count;

    glPushMatrix();
    glTranslatef(origin[0], origin[1], origin[2]);

    /* Shaft: diamond shape */
    count = gizmo_build_diamond_shaft(verts,
                                      0.8f,   /* shaft length */
                                      0.05f,  /* radius */
                                      color);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_TRIANGLES, 0, count);
    gizmo_disable_arrays();

    /* Tip: square pyramid */
    count = gizmo_build_pyramid_tip(verts,
                                    0.8f,   /* base X */
                                    0.08f,  /* base half-size */
                                    0.2f,   /* tip length */
                                    color);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_TRIANGLES, 0, count);
    gizmo_disable_arrays();

    /* Box at -1 end (your existing code) */
    if(enableBox) {
        count = gizmo_build_box(verts,
                                -1.0f, 0.0f, 0.0f,
                                0.10f,
                                color);
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();
    }

    glPopMatrix();
}














/* ---------- PUBLIC RENDER FUNCTION ---------- */

/* origin: world-space position of gizmo
   enable4D: if non-zero, draw W axis + extra 4D rotation planes
*/
void Gizmo_Render(const float origin[3], int enable4D) 
{
    GizmoVertex verts[2048]; /* enough for all parts */
    int count;

    


    glPushMatrix();
    glTranslatef(origin[0], origin[1], origin[2]);

    /* --- TRANSLATION AXES (LINES + ARROW BOXES) --- */

    /* X axis line */
    count = 30;

#if 0
    //gizmo_build_line(verts, -GIZMO_HALF_AXIS_LEN, 0.0f, 0.0f, GIZMO_HALF_AXIS_LEN, 0.0f, 0.0f, AXIS_COLOR_X);
  

    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();
#else
    gizmo_draw_solid_arrow((float[3]) { 0, 0, 0 }, AXIS_COLOR_X, 1);
#endif


 




    /* Y axis line */
#if 0
    gizmo_build_line(verts, 0.0f, -GIZMO_HALF_AXIS_LEN, 0.0f, 0.0f, GIZMO_HALF_AXIS_LEN, 0.0f, AXIS_COLOR_Y);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();
#else
    glPushMatrix();
    glRotatef(90, 0, 0, 1);  /* X?Y */
    gizmo_draw_solid_arrow((float[3]) { 0, 0, 0 }, AXIS_COLOR_Y, 1);
    glPopMatrix();
#endif

    /* Z axis line */
#if 0
    gizmo_build_line(verts, 0.0f, 0.0f, -GIZMO_HALF_AXIS_LEN, 0.0f, 0.0f, GIZMO_HALF_AXIS_LEN, AXIS_COLOR_Z);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();
#else
    glPushMatrix();
    glRotatef(-90, 0, 1, 0); /* X?Z */
    gizmo_draw_solid_arrow((float[3]) { 0, 0, 0 }, AXIS_COLOR_Z, 1);
    glPopMatrix();
#endif

    /* W axis (visualized as another line in XY plane, purple) */
    if(enable4D) 
    {
#if 0
        gizmo_build_line(verts,                         -GIZMO_HALF_AXIS_LEN, -GIZMO_HALF_AXIS_LEN, 0.0f,                         GIZMO_HALF_AXIS_LEN, GIZMO_HALF_AXIS_LEN, 0.0f,                         AXIS_COLOR_W);
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();
#else
        glPushMatrix();
        glRotatef(-90, 1, 1, 1); /* X?Z */
        gizmo_draw_solid_arrow((float[3]) { 0, 0, 0 }, AXIS_COLOR_W, 1);
        glPopMatrix();
#endif
    }

    /* --- SCALING BOXES AT +1 END OF EACH AXIS --- */

    /* X box at (1,0,0) */
    count = gizmo_build_box(verts,
                            GIZMO_AXIS_LEN, 0.0f, 0.0f,
                            GIZMO_BOX_SIZE,
                            AXIS_COLOR_X);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* Y box at (0,1,0) */
    count = gizmo_build_box(verts,
                            0.0f, GIZMO_AXIS_LEN, 0.0f,
                            GIZMO_BOX_SIZE,
                            AXIS_COLOR_Y);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* Z box at (0,0,1) */
    count = gizmo_build_box(verts,
                            0.0f, 0.0f, GIZMO_AXIS_LEN,
                            GIZMO_BOX_SIZE,
                            AXIS_COLOR_Z);
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* W box – visually placed at (0.7,0.7,0) for distinction */
    if(enable4D) {
        count = gizmo_build_box(verts,
                                0.7f, 0.7f, 0.0f,
                                GIZMO_BOX_SIZE,
                                AXIS_COLOR_W);
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();
    }

    /* --- ROTATION CIRCLES (3D PLANES) --- */

    /* XY plane (around Z) */
    count = gizmo_build_circle(verts, GIZMO_CIRCLE_R, AXIS_COLOR_Z, 'Z');
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* XZ plane (around Y) */
    count = gizmo_build_circle(verts, GIZMO_CIRCLE_R, AXIS_COLOR_Y, 'Y');
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* YZ plane (around X) */
    count = gizmo_build_circle(verts, GIZMO_CIRCLE_R, AXIS_COLOR_X, 'X');
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINES, 0, count);
    gizmo_disable_arrays();

    /* --- 4D ROTATION PLANES (XW, YW, ZW) AS LARGER CIRCLES --- */

    if(enable4D) {
        /* XW -> visually another circle in XY plane, purple, slightly larger */
        count = gizmo_build_circle(verts, GIZMO_CIRCLE_R_4D, AXIS_COLOR_W, 'Z');
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();

        /* YW -> visually another circle in XZ plane, purple */
        count = gizmo_build_circle(verts, GIZMO_CIRCLE_R_4D, AXIS_COLOR_W, 'Y');
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();

        /* ZW -> visually another circle in YZ plane, purple */
        count = gizmo_build_circle(verts, GIZMO_CIRCLE_R_4D, AXIS_COLOR_W, 'X');
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINES, 0, count);
        gizmo_disable_arrays();
    }

    /* --- PLANE SCALING RECTANGLES INSIDE CIRCLES --- */

    /* XY */
    count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.5f, PLANE_COLOR_XY, "XY");
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINE_LOOP, 0, count);
    gizmo_disable_arrays();

    /* XZ */
    count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.5f, PLANE_COLOR_XZ, "XZ");
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINE_LOOP, 0, count);
    gizmo_disable_arrays();

    /* YZ */
    count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.5f, PLANE_COLOR_YZ, "YZ");
    gizmo_enable_arrays(verts);
    glDrawArrays(GL_LINE_LOOP, 0, count);
    gizmo_disable_arrays();

    if(enable4D) {
        /* XW */
        count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.6f, PLANE_COLOR_XW, "XW");
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINE_LOOP, 0, count);
        gizmo_disable_arrays();

        /* YW */
        count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.6f, PLANE_COLOR_YW, "YW");
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINE_LOOP, 0, count);
        gizmo_disable_arrays();

        /* ZW */
        count = gizmo_build_plane_rect(verts, GIZMO_CIRCLE_R * 0.6f, PLANE_COLOR_ZW, "ZW");
        gizmo_enable_arrays(verts);
        glDrawArrays(GL_LINE_LOOP, 0, count);
        gizmo_disable_arrays();
    }

    glPopMatrix();
}

float poss[3] = { 00,5,00 };

#include <gl/GLU.h>
PXResult PXAPI PXGizmoDraw(PXGizmo PXREF pxGizmo, PXDrawInfo PXREF pxDrawInfo)
{

    glScalef(5,5,5);
    glLineWidth(2);
    Gizmo_Render(poss, 1);

    return PXResultOK;
}
