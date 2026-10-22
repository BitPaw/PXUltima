#include "PXCamera.h"
#include <PX/Math/PXVector.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXResource.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

const char PXCameraName[] = "Camera";
const PXI8U PXCameraNameLength = sizeof(PXCameraName);
const PXECSRegisterInfoStatic PXCameraRegisterInfoStatic =
{
    {sizeof(PXCameraName), sizeof(PXCameraName), PXCameraName, TextFormatASCII},
    sizeof(PXCamera),
    __alignof(PXCamera),
    PXECSTypeEntity,
    PXCameraCreate,
    PXCameraRelease,
    PXCameraDraw
};
PXECSRegisterInfoDynamic PXCameraRegisterInfoDynamic;

void PXAPI PXCameraRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXCameraRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXCameraRegisterInfoDynamic;
}

PXResult PXAPI PXCameraCreate(PXCamera** pxCameraREF, PXCameraCreateInfo PXREF pxCameraCreateInfo)
{
    PXCamera* pxCamera = PXNull;

    pxCameraCreateInfo->Info.Static = &PXCameraRegisterInfoStatic;
    pxCameraCreateInfo->Info.Dynamic = &PXCameraRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxCameraREF, pxCameraCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxCamera = *pxCameraREF;
    pxCamera->WalkSpeed = 30;// 500;
    pxCamera->ViewSpeed = 0.02f;
    pxCamera->FollowSpeed = 0.98f;
    pxCamera->FieldOfView = 90;
    pxCamera->Rectangle.Height = 1;
    pxCamera->Rectangle.Width = 1;
    pxCamera->Near = 0.1;
    pxCamera->Far = 100000000000.f;

    PXTransform4DInit(&pxCamera->Transform);

    PXCameraViewChange(pxCamera, pxCameraCreateInfo->Perspective);

    const PXVector3F32 position = { 0,0,0 };
    //PXCameraRotate(pxCamera, &position);

    return PXResultOK;
}

PXResult PXAPI PXCameraRelease(PXCamera PXREF pxCamera)
{
    return PXResultRefusedNotImplemented;
}

typedef struct GizLineVtx {
    unsigned char r, g, b, a;
    float x, y, z;
} GizLineVtx;

static void pushLine
(
    GizLineVtx* v,
    int* n,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a,
    float x0,
    float y0,
    float z0,
    float x1,
    float y1,
    float z1
)
{
    GizLineVtx* p = &v[*n];
    p[0].r = r; p[0].g = g; p[0].b = b; p[0].a = a; p[0].x = x0; p[0].y = y0; p[0].z = z0;
    p[1].r = r; p[1].g = g; p[1].b = b; p[1].a = a; p[1].x = x1; p[1].y = y1; p[1].z = z1;
    *n += 2;
}

static void addWireBox(GizLineVtx* v, int* n,
                       unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                       float cx, float cy, float cz,
                       float hx, float hy, float hz)
{
    /* 8 corners */
    float x0 = cx - hx, x1 = cx + hx;
    float y0 = cy - hy, y1 = cy + hy;
    float z0 = cz - hz, z1 = cz + hz;

    /* 12 edges */
    pushLine(v, n, r, g, b, a, x0, y0, z0, x1, y0, z0);
    pushLine(v, n, r, g, b, a, x1, y0, z0, x1, y1, z0);
    pushLine(v, n, r, g, b, a, x1, y1, z0, x0, y1, z0);
    pushLine(v, n, r, g, b, a, x0, y1, z0, x0, y0, z0);

    pushLine(v, n, r, g, b, a, x0, y0, z1, x1, y0, z1);
    pushLine(v, n, r, g, b, a, x1, y0, z1, x1, y1, z1);
    pushLine(v, n, r, g, b, a, x1, y1, z1, x0, y1, z1);
    pushLine(v, n, r, g, b, a, x0, y1, z1, x0, y0, z1);

    pushLine(v, n, r, g, b, a, x0, y0, z0, x0, y0, z1);
    pushLine(v, n, r, g, b, a, x1, y0, z0, x1, y0, z1);
    pushLine(v, n, r, g, b, a, x1, y1, z0, x1, y1, z1);
    pushLine(v, n, r, g, b, a, x0, y1, z0, x0, y1, z1);
}

static void addFrustumLines(GizLineVtx* v, int* n,
                            unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                            float nw, float nh, float nearZ,
                            float fw, float fh, float farZ)
{
    /* near corners (counter-clockwise when looking from camera outward) */
    float n0x = -nw, n0y = -nh, n0z = nearZ; /* bottom-left */
    float n1x = +nw, n1y = -nh, n1z = nearZ; /* bottom-right */
    float n2x = +nw, n2y = +nh, n2z = nearZ; /* top-right */
    float n3x = -nw, n3y = +nh, n3z = nearZ; /* top-left */

    /* far corners */
    float f0x = -fw, f0y = -fh, f0z = farZ;
    float f1x = +fw, f1y = -fh, f1z = farZ;
    float f2x = +fw, f2y = +fh, f2z = farZ;
    float f3x = -fw, f3y = +fh, f3z = farZ;

    /* near rectangle */
    pushLine(v, n, r, g, b, a, n0x, n0y, n0z, n1x, n1y, n1z);
    pushLine(v, n, r, g, b, a, n1x, n1y, n1z, n2x, n2y, n2z);
    pushLine(v, n, r, g, b, a, n2x, n2y, n2z, n3x, n3y, n3z);
    pushLine(v, n, r, g, b, a, n3x, n3y, n3z, n0x, n0y, n0z);

    /* far rectangle */
    pushLine(v, n, r, g, b, a, f0x, f0y, f0z, f1x, f1y, f1z);
    pushLine(v, n, r, g, b, a, f1x, f1y, f1z, f2x, f2y, f2z);
    pushLine(v, n, r, g, b, a, f2x, f2y, f2z, f3x, f3y, f3z);
    pushLine(v, n, r, g, b, a, f3x, f3y, f3z, f0x, f0y, f0z);

    /* connecting edges (pyramid stump sides) */
    pushLine(v, n, r, g, b, a, n0x, n0y, n0z, f0x, f0y, f0z);
    pushLine(v, n, r, g, b, a, n1x, n1y, n1z, f1x, f1y, f1z);
    pushLine(v, n, r, g, b, a, n2x, n2y, n2z, f2x, f2y, f2z);
    pushLine(v, n, r, g, b, a, n3x, n3y, n3z, f3x, f3y, f3z);
}

static void addFrustumHandles
(
    GizLineVtx* v,
    int* n,
    unsigned char r,
    unsigned char g,
    unsigned char b,
    unsigned char a,
    float nw,
    float nh,
    float nearZ,
    float fw,
    float fh,
    float farZ,
    float hs,
    unsigned int mask
)
{
    float nC[4][3] = {
        {-nw,-nh,nearZ}, {+nw,-nh,nearZ}, {+nw,+nh,nearZ}, {-nw,+nh,nearZ}
    };
    float fC[4][3] = {
        {-fw,-fh,farZ}, {+fw,-fh,farZ}, {+fw,+fh,farZ}, {-fw,+fh,farZ}
    };

    /* bit0: side edges midpoints */
    if(mask & 1u) {
        for(int i = 0; i < 4; i++) {
            float mx = 0.5f * (nC[i][0] + fC[i][0]);
            float my = 0.5f * (nC[i][1] + fC[i][1]);
            float mz = 0.5f * (nC[i][2] + fC[i][2]);
            addWireBox(v, n, r, g, b, a, mx, my, mz, hs, hs, hs);
        }
    }

    /* bit1: near edges midpoints */
    if(mask & 2u) {
        for(int i = 0; i < 4; i++) {
            int j = (i + 1) & 3;
            float mx = 0.5f * (nC[i][0] + nC[j][0]);
            float my = 0.5f * (nC[i][1] + nC[j][1]);
            float mz = nearZ;
            addWireBox(v, n, r, g, b, a, mx, my, mz, hs, hs, hs);
        }
    }

    /* bit2: far edges midpoints */
    if(mask & 4u) {
        for(int i = 0; i < 4; i++) {
            int j = (i + 1) & 3;
            float mx = 0.5f * (fC[i][0] + fC[j][0]);
            float my = 0.5f * (fC[i][1] + fC[j][1]);
            float mz = farZ;
            addWireBox(v, n, r, g, b, a, mx, my, mz, hs, hs, hs);
        }
    }

    /* bit3: near corners */
    if(mask & 8u) {
        for(int i = 0; i < 4; i++) {
            addWireBox(v, n, r, g, b, a, nC[i][0], nC[i][1], nC[i][2], hs, hs, hs);
        }
    }

    /* bit4: far corners */
    if(mask & 16u) {
        for(int i = 0; i < 4; i++) {
            addWireBox(v, n, r, g, b, a, fC[i][0], fC[i][1], fC[i][2], hs, hs, hs);
        }
    }
}



PXResult PXAPI PXCameraDraw(PXCamera PXREF pxCamera, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    if(!(pxCamera && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    float fovY_deg = pxCamera->FieldOfView;
    float aspect = PXCameraAspectRatio(pxCamera);
    float nearD = pxCamera->Near;
    float farD = pxCamera->Far;
    float bodyLen = 1;
    float bodyHalf = 1;
    float handleSize = 3;
    unsigned int handleMask;

    if(nearD <= 0.0f)
        nearD = 0.1f;

    if(farD <= nearD)
        farD = nearD * 10.0f;

    if(aspect <= 0.0f)
        aspect = 1.0f;

    /* Prepare trig for frustum */
    float fovY = fovY_deg * (float)(PXMathPI / 180.0);
    float t = tanf(0.5f * fovY);
    float nh = t * nearD;
    float nw = nh * aspect;
    float fh = t * farD;
    float fw = fh * aspect;

    float nearZ = -nearD;
    float farZ = -farD;

    /* Estimate worst-case vertices:
       - body box: 12 edges => 24 vertices
       - frustum: 12 edges => 24 vertices
       - handles: up to (4+4+4+4+4)=20 boxes, each 12 edges => 240 edges => 480 vertices
       Total < 600 vertices.
    */
    GizLineVtx verts[800];
    int vcount = 0;

    /* Colors */
    const unsigned char A = 255;
    /* body: yellow-ish */
    const unsigned char br = 240, bg = 200, bb = 60;
    /* frustum: light gray */
    const unsigned char fr = 220, fg = 220, fb = 220;
    /* handles: orange */
    const unsigned char hr = 255, hg = 140, hb = 40;

    /* Camera body: wire box behind camera (along +Z) */
    {
        float cx = 0.0f;
        float cy = 0.0f;
        float cz = +0.5f * bodyLen; /* box centered behind origin */
        addWireBox(verts, &vcount, br, bg, bb, A, cx, cy, cz, bodyHalf, bodyHalf, 0.5f * bodyLen);

        /* Optional: a small "lens ring" rectangle on the front face (near origin) */
        float ringZ = +0.02f;
        float r0 = bodyHalf * 0.55f;
        pushLine(verts, &vcount, br, bg, bb, A, -r0, -r0, ringZ, +r0, -r0, ringZ);
        pushLine(verts, &vcount, br, bg, bb, A, +r0, -r0, ringZ, +r0, +r0, ringZ);
        pushLine(verts, &vcount, br, bg, bb, A, +r0, +r0, ringZ, -r0, +r0, ringZ);
        pushLine(verts, &vcount, br, bg, bb, A, -r0, +r0, ringZ, -r0, -r0, ringZ);
    }

    /* Frustum lines */
    addFrustumLines(verts, &vcount, fr, fg, fb, A, nw, nh, nearZ, fw, fh, farZ);

    /* Handles (“notches/boxes” in the middle of each line) */
    if(handleSize > 0.0f && handleMask != 0u) {
        addFrustumHandles(verts, &vcount, hr, hg, hb, A, nw, nh, nearZ, fw, fh, farZ, handleSize, handleMask);
    }

    /* Render */
    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glLineWidth(1.5f);

    glInterleavedArrays(GL_C4UB_V3F, 0, (const GLvoid*)verts);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_LINES, 0, vcount);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopAttrib();

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXCameraGLFPP(const PXCamera PXREF pxCamera, const PXCameraPerspective cameraPerspective)
{
    if(!pxCamera)
    {
        return PXResultOK;
    }

    const PXRectangleXYWHI32 PXREF pxRectangle = &pxCamera->Rectangle;

    switch(cameraPerspective)
    {
        case PXCameraPerspective2D:
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho
            (
                pxRectangle->X, // Left
                pxRectangle->X + pxRectangle->Width, // Right
                pxRectangle->Y + pxRectangle->Height, // Top
                pxRectangle->Y, // Bottom
                -1.0f,
                +1.0f
            );

            // Why did we add this???
            //glScalef(1.0f, -1.0f, 1.0f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            break;
        }
        case PXCameraPerspective3D:
        {
            const PXF32 aspectRatio = pxRectangle->Width / pxRectangle->Height;

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(pxCamera->FieldOfView, aspectRatio, pxCamera->Near, pxCamera->Far);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            //glRotatef(-PXMathRadiansToDegree(pxCamera->CurrentRotation.X), 1.0f, 0.0f, 0.0f);
            //glRotatef(-PXMathRadiansToDegree(pxCamera->CurrentRotation.Y), 0.0f, 1.0f, 0.0f);
            //glRotatef(-PXMathRadiansToDegree(pxCamera->CurrentRotation.Z), 0.0f, 0.0f, 1.0f);
            glTranslatef
            (
                -pxCamera->Transform.Position.X, 
                -pxCamera->Transform.Position.Y, 
                -pxCamera->Transform.Position.Z
            );

            break;
        }
    }

    return PXResultOK;
}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera PXREF camera, const PXSize width, const PXSize height, const PXF32 nearPlane, const PXF32 farPlane)
{
    if(!camera)
    {
        return;
    }

    camera->Perspective = PXCameraPerspective2D;
    camera->Rectangle.X = 0;
    camera->Rectangle.Y = 0;
    camera->Rectangle.Width = width;
    camera->Rectangle.Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera PXREF camera, const PXF32 fieldOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane)
{
    if(!camera)
    {
        return;
    }

    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;
}

void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height)
{
    if(!camera)
    {
        return;
    }

    camera->Rectangle.Width = width;
    camera->Rectangle.Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

PXF32 PXAPI PXCameraAspectRatio(const PXCamera PXREF camera)
{
    PXF32 aspectRatio = 1.0f;

    if(!camera)
    {
        return aspectRatio;
    }

    if(!(camera->Rectangle.Width && camera->Rectangle.Height))
    {
        return aspectRatio;
    }

    aspectRatio = (PXF32)camera->Rectangle.Width / (PXF32)camera->Rectangle.Height;

    return aspectRatio;
}

void PXAPI PXCameraViewChange(PXCamera PXREF camera, const PXCameraPerspective cameraPerspective)
{
    camera->Perspective = cameraPerspective;

    switch(cameraPerspective)
    {
        case PXCameraPerspective2D:
        {
            PXCameraViewChangeToOrthographic(camera, camera->Rectangle.Width, camera->Rectangle.Height, camera->Near, camera->Far);
            break;
        }

        case PXCameraPerspective3D:
        {
            const PXF32 aspectRatio = PXCameraAspectRatio(camera);

            PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

            break;
        }
    }
}

void PXAPI PXCameraRotate(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F)
{
    if(!(camera && vector3F))
    {
        return;
    }

    if(camera->LockView)
    {
        return;
    }

    camera->AccumulatorRAD.Yaw += -vector3F->X * camera->ViewSpeed;
    camera->AccumulatorRAD.Pitch += -vector3F->Y * camera->ViewSpeed;

    PXRotor4DF32 pxRotorFinal = { 0 };
    PXRotor4DF32 pitchRotor = { 0 };
    PXRotor4DF32 yawRotor = { 0 };

#if 1

    const float MAX_PITCH = 89.0f * (PXMathPI / 180.0f); // 89 degrees in radians

#if 1
    // Limit pitch
    if(camera->AccumulatorRAD.Pitch > MAX_PITCH)
        camera->AccumulatorRAD.Pitch = MAX_PITCH;

    if(camera->AccumulatorRAD.Pitch < -MAX_PITCH)
        camera->AccumulatorRAD.Pitch = -MAX_PITCH;
#endif

    // Limit yaw
#if 1
    if(camera->AccumulatorRAD.Yaw > PXMathPI) 
    {
        camera->AccumulatorRAD.Yaw -= 2.0f * PXMathPI;
    }
    else if(camera->AccumulatorRAD.Yaw < -PXMathPI) 
    {
        camera->AccumulatorRAD.Yaw += 2.0f * PXMathPI;
    }
#endif

    PXRotor4DFromAngleAndPlane(&yawRotor, camera->AccumulatorRAD.Yaw, 0.0f, 0.0f, 1.0f); // ZX plane
    PXRotor4DFromAngleAndPlane(&pitchRotor, camera->AccumulatorRAD.Pitch, 0.0f, 1.0f, 0.0f); // YZ plane

#if 0
    PXRotor4DF32Set(&pxRotorFinal, &pitchRotor);
    PXRotor4DF32Multiply(&pxRotorFinal, &yawRotor);
#else
    PXRotor4DF32Set(&pxRotorFinal, &yawRotor);
    PXRotor4DF32Multiply(&pxRotorFinal, &pitchRotor);
#endif

    PXRotor4DF32Normalize(&pxRotorFinal);

    //PXRotor4DF32Set(&camera->Transform.Rotor, &pxRotorFinal);

#else
    const float maxPitch = 89.0f;
    const float minPitch = -89.0f;

    if(camera->AccumulatorRAD.Pitch > maxPitch) 
        camera->AccumulatorRAD.Pitch = maxPitch;

    if(camera->AccumulatorRAD.Pitch < minPitch) 
        camera->AccumulatorRAD.Pitch = minPitch;

    // 3. Keep Yaw within a friendly 0 to 360 circle to prevent float overflow over time
    if(camera->AccumulatorRAD.Yaw > 360.0f)  
        camera->AccumulatorRAD.Yaw -= 360.0f;

    if(camera->AccumulatorRAD.Yaw < -360.0f) 
        camera->AccumulatorRAD.Yaw += 360.0f;



    // 4. Compute half-angles for the current total absolute rotation
    float halfYaw = camera->AccumulatorRAD.Yaw * 0.5f;
    float halfPitch = camera->AccumulatorRAD.Pitch * 0.5f;

    // Convert degrees to radians if your cos/sin functions expect radians!
    float cosY = PXMathCosinusDEGF32(halfYaw);
    float sinY = PXMathSinusDEGF32(halfYaw);
    float cosP = PXMathCosinusDEGF32(halfPitch);
    float sinP = PXMathSinusDEGF32(halfPitch);    

    

    // 5. Build clean, independent absolute rotors for this frame
    pitchRotor.Scalar = cosP;
    pitchRotor.PlaneAngles[PXRotorAxisYZ] = sinP; // Local Pitch (YZ Plane)

    yawRotor.Scalar = cosY;
    yawRotor.PlaneAngles[PXRotorAxisZX] = sinY; // World Yaw (ZX Plane)

    // 6. Rebuild the final camera rotor fresh (World Yaw * Local Pitch)
    // We load yawRotor into our temporary final holder, then right-multiply by pitchRotor
    PXRotor4DF32 finalOrientation;
    PXRotor4DF32Set(&finalOrientation, &yawRotor);
    PXRotor4DF32Multiply(&finalOrientation, &pitchRotor); // finalOrientation = yaw * pitch

    // 7. Overwrite the camera transform with the fresh, stable calculation
    PXRotor4DF32Set(&camera->Transform.Rotor, &finalOrientation);

#endif
}

void PXAPI PXCameraRotateXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z)
{
    if(!camera)
    {
        return;
    }

    const PXVector3F32 vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera PXREF camera, const PXF32 x, const PXF32 y, const PXF32 z)
{
    if(!camera)
    {
        return;
    }

    const PXVector3F32 vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera PXREF camera, const PXVector3F32 PXREF vector3F)
{
    if(!camera)
    {
        return;
    }

    if(camera->LockMovement)
    {
        return;
    }

    PXVector3F32 xAxis = { 0,0,0 };
    const PXVector3F32 yAxis = { 0, vector3F->Y, 0 };
    PXVector3F32 zAxis = { 0,0,0 };

    // ...
    {
        const PXVector3F32 up = { 0, 1, 0 };
        const PXVector3F32 lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

        PXVector3F32CrossProduct(&xAxis, &lookAtPosition, &up);
        PXVector3F32Normalize(&xAxis);
        PXVector3F32MultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X);

        zAxis = lookAtPosition;

        PXVector3F32Normalize(&zAxis);
        PXVector3F32MultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z);
    }

    {
        PXVector3F32 targetedMovement = { 0,0,0 };

        PXVector3F32Add(&targetedMovement, &xAxis);
        PXVector3F32Add(&targetedMovement, &yAxis);
        PXVector3F32Add(&targetedMovement, &zAxis);
        PXVector3F32MultiplyS(&targetedMovement, camera->WalkSpeed);

        PXVector3F32Add(&camera->Transform.Position.V3, &targetedMovement);

        //PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement);
    }
}

void PXAPI PXCameraFollow(PXCamera PXREF camera, const PXF32 deltaTime)
{
    PXVector3F32 positionCurrent;
    PXVector3F32 positionDesired;

    PXVector3F32 rotationCurrent;
    PXVector3F32 rotationDesired;

    PXVector3F32 positionDelta;
    PXVector3F32 rotationDelta;

    if(!camera->Target)
    {
        return;
    }

   // PXMatrix4x4FPositionGet(&camera->MatrixModel, &positionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &positionDesired); // get current target pos



   // PXMatrix4x4FRotationGet(&camera->MatrixView, &rotationCurrent); // Get current camera pos
    PXMatrix4x4FRotationGet(camera->Target, &rotationDesired); // get current target pos

    camera->FollowSpeed = 12.3;

    //PXVector3F32Add(&positionCurrent, &camera->Offset); // add offset to target pos
    PXVector3F32Add(&positionDesired, &camera->Offset); // add offset to target pos


    PXVector3F32 eye = { 0,0,0 };
    PXVector3F32 center = { 0,0,0 };
    PXVector3F32 up = { 0,1,0 };

    // PXMatrix4x4FLookAt(&camera->MatrixModel, &eye, &desiredPosition, &up);

    PXVector3F32Interpolate(&positionDesired, &positionCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement
    PXVector3F32Interpolate(&rotationDesired, &rotationCurrent, camera->FollowSpeed * deltaTime); // calculate delta movement

#if 0
    // Not how i want it.
    // Problem: it snaps to strong if you get in range, then no movement until were too far away again.
    // We need a deadzone- then a softstart, then a rampup the further away we are

    PXVector3F32Set(&positionDelta, &positionCurrent);
    PXVector3F32Substract(&positionDelta, &positionDesired);
    PXVector3F32Absolute(&positionDelta);

    PXVector3F32Set(&rotationDelta, &rotationCurrent);
    PXVector3F32Substract(&rotationDelta, &rotationDesired);
    PXVector3F32Absolute(&positionDelta);

    if(positionDelta.X < camera->DeadZone.X)
    {
        // Cancel X movement
        positionDesired.X = positionCurrent.X;
    }

    if(positionDelta.Y < camera->DeadZone.Y)
    {
        // Cancel Y movement
        positionDesired.Y = positionCurrent.Y;
    }

    if(positionDelta.Z < camera->DeadZone.Z)
    {
        // Cancel Z movement
        positionDesired.Z = positionCurrent.Z;
    }
#endif



   // PXMatrix4x4FPositionSet(&camera->MatrixModel, &positionDesired); // Set delte movement
   // PXMatrix4x4FRotationSet(&camera->MatrixView, &rotationDesired);


    PXVector3F32 rotation;

    //PXMatrix4x4FRotationGet(&camera->MatrixView, &rotation);

#if 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Camera",
        "Follow",
        "x:%6.2f y:%6.2f z:%6.2f, Yaw:%6.2f Pitch:%6.2f Roll:%6.2f",
        camera->Target->Data[TransformX],
        camera->Target->Data[TransformY],
        camera->Target->Data[TransformZ],
        PXMathRadiansToDegree(rotation.X),
        PXMathRadiansToDegree(rotation.Y),
        PXMathRadiansToDegree(rotation.Z)
    );
#endif
}

void PXAPI PXCameraUpdate(PXCamera PXREF camera, const PXF32 deltaTime)
{
    if(!camera)
    {
        return;
    }

    PXRotor4DF32Normalize(&camera->Transform.Rotor);
}

void PXAPI PXCameraForward(const PXCamera PXREF camera, PXVector3F32 PXREF forward)
{
    if(!(camera && forward)) 
        return;

    // 1. Define standard OpenGL 4D unit forward vector (points down -Z)
    PXVector4F32 unitForward = { 0.0f, 0.0f, -1.0f, 0.0f };
    PXVector4F32 forward4;

    // 2. Rotate the 4D vector using your 4D transform rotor
    PXRotor4DF32RotateVector(&camera->Transform.Rotor, &unitForward, &forward4);

    forward->X = forward4.X;
    forward->Y = forward4.Y;
    forward->Z = forward4.Z;

    // Optional but safe: ensure precision loss from consecutive rotations doesn't shrink the vector
    PXVector3F32Normalize(forward);
}

void PXAPI PXCameraRight(const PXCamera PXREF camera, PXVector3F32 PXREF right)
{
    if(!(camera && right))
    {
        return;
    }

    // Standard OpenGL 4D unit right vector (points down +X)
    PXVector4F32 unitRight = { 1.0f, 0.0f, 0.0f, 0.0f };
    PXVector4F32 realRight;

    PXRotor4DF32RotateVector(&camera->Transform.Rotor, &unitRight, &realRight);

    right->X = realRight.X;
    right->Y = realRight.Y;
    right->Z = realRight.Z;

    PXVector3F32Normalize(right);
}

void PXAPI PXCameraUp(const PXCamera PXREF camera, PXVector3F32 PXREF up)
{
    if(!(camera && up))
    {
        return;
    }

    // Standard OpenGL 4D unit up vector (points down +Y)
    PXVector4F32 unitUp = { 0.0f, 1.0f, 0.0f, 0.0f };
    PXVector4F32 realUp;

    PXRotor4DF32RotateVector(&camera->Transform.Rotor, &unitUp, &realUp);

    up->X = realUp.X;
    up->Y = realUp.Y;
    up->Z = realUp.Z;

    PXVector3F32Normalize(up);
}

void PXAPI PXCameraOrthonormalBasisGet
(
    const PXCamera PXREF camera,
    PXVector3F32 PXREF out_right,
    PXVector3F32 PXREF out_up,
    PXVector3F32 PXREF out_forward
)
{
    // 1. Define the World's True Up Vector (Y-axis assumed to be world up)
    const PXVector3F32 WORLD_UP = { 0.0f, 1.0f, 0.0f };

    // Step 1: Get the initial best estimate for Forward (Z axis).
    // We reuse your original logic to get the direction vector.
    // NOTE: You might need to embed or call your existing PXCameraForward function here.
    // For simplicity, we assume a helper function that calculates Z based on rotation is available.
    PXCameraForward(camera, out_forward); // Assume this populates 'forward'

    // Step 2: Calculate Right Vector (X axis) using Cross Product.
    // X = Normalize(WorldUp x Forward). This guarantees 'right' is perpendicular to the plane defined by WorldUp and Forward.

    // Calculate WorldUp x Forward -> This vector is the Right axis (X)
    PXVector3F32CrossProduct
    (
        out_right,
        out_forward,
        &WORLD_UP
    );
    PXVector3F32Normalize(out_right);


    // Step 3: Calculate True Up Vector (Y axis) using Cross Product.
    // Y = Forward x Right. This cross product guarantees that Y is perpendicular to both the calculated Z and X, 
    // thus forcing orthogonality and solving the "roll" issue.

    // Calculate Forward x Right -> This vector is the True Up axis (Y)
    PXVector3F32CrossProduct(
        out_up,
        out_right,        // X-axis (Right)
        out_forward    // Z-axis (Forward)
    );
    PXVector3F32Normalize(out_up);
}


void PXAPI PXCameraOrthonormalBasisGetEuler
(
    const PXCamera PXREF camera,
    PXVector3F32 PXREF out_right,
    PXVector3F32 PXREF out_up,
    PXVector3F32 PXREF out_forward
)
{   
    const PXEulerAngle3F32 PXREF pxEulerAngle = &camera->AccumulatorRAD;

    /* 1. Convert angles from degrees to radians */
    float yaw_rad = -pxEulerAngle->X;
    float pitch_rad = pxEulerAngle->Y;

    /* 2. Calculate trigs using our Taylor series functions */
    float cos_yaw = PXMathCosinusRADF32(yaw_rad);
    float sin_yaw = PXMathSinusRADF32(yaw_rad);
    float cos_pitch = PXMathCosinusRADF32(pitch_rad);
    float sin_pitch = PXMathSinusRADF32(pitch_rad);

    /* 3. Compute Forward Vector from Euler coordinates */
    out_forward->X = cos_yaw * cos_pitch;
    out_forward->Y = sin_pitch;
    out_forward->Z = sin_yaw * cos_pitch;

    /* OpenGL looks towards negative Z, invert target direction for the matrix axis */
    out_forward->X = -out_forward->X;
    out_forward->Y = -out_forward->Y;
    out_forward->Z = -out_forward->Z;
    PXVector3F32Normalize(out_forward);

    /* 4. Cross product with absolute World Up to get Right vector */
    PXVector3F32 world_up = { 0.0f, 1.0f, 0.0f };

    PXVector3F32CrossProduct(out_right, &world_up, out_forward);
    PXVector3F32Normalize(out_right);

    /* 5. Get localized Up vector */
    PXVector3F32CrossProduct(out_up, out_forward, out_right);
}

void PXAPI PXCameraMakeMatrix
(
    PXCamera PXREF camera,
    const PXTransform4D PXREF modelTransform,
    PXMatrixModelViewProjection PXREF pxMatrixModelViewProjection
)
{
    PXVector4F32 PXREF cameraPosition = &camera->Transform.Position;

    PXVector3F32 cameraRight;
    PXVector3F32 cameraUp;
    PXVector3F32 cameraForward;


    PXMatrix4x4F PXREF model = &pxMatrixModelViewProjection->MatrixModel;
    PXMatrix4x4F PXREF view = &pxMatrixModelViewProjection->MatrixView;
    PXMatrix4x4F PXREF projection = &pxMatrixModelViewProjection->MatrixProjection;

    PXCameraOrthonormalBasisGetEuler(camera, &cameraRight, &cameraUp, &cameraForward);

    //-----------------------------------------------------
    // Model
    // Note! DO NOT put camera transform here, dumbass.
    PXTransform4DToMatrix(modelTransform, model);
    //-----------------------------------------------------

    //-----------------------------------------------------
    // View
#if 1
   // PXEulerAngle3F32ToMatrix4x4(&camera->AccumulatorRAD, &pxMatrixModelViewProjection->MatrixView);
    //PXMatrix4x4FMove3F(&pxMatrixModelViewProjection->MatrixView, &camera->Transform.Position);



    view->Data[0] = cameraRight.X;
    view->Data[1] = cameraUp.X;
    view->Data[2] = cameraForward.X;
    view->Data[3] = 0.0f;

    /* Column 1 */
    view->Data[4] = cameraRight.Y;
    view->Data[5] = cameraUp.Y;
    view->Data[6] = cameraForward.Y;
    view->Data[7] = 0.0f;

    /* Column 2 */
    view->Data[8] = cameraRight.Z;
    view->Data[9] = cameraUp.Z;
    view->Data[10] = cameraForward.Z;
    view->Data[11] = 0.0f;

    /* Column 3 (Eye position translation mapping) */
    view->Data[12] = -PXVector3F32DotProduct(&cameraRight, cameraPosition);
    view->Data[13] = -PXVector3F32DotProduct(&cameraUp, cameraPosition);
    view->Data[14] = -PXVector3F32DotProduct(&cameraForward, cameraPosition);
    view->Data[15] = 1.0f;



#else
    PXVector4F32 currentPosition = camera->Transform.Position;

    // --- STEP 2: Calculate Target Position (Position + Direction) ---
    // The target is simply the current position moved forward by a defined look-ahead distance.
    const float LOOK_AHEAD_DISTANCE = 1.0f; // How far ahead to "look"
    PXVector3F32 targetPosition;

    targetPosition.X = currentPosition.X + (cameraForward.X * LOOK_AHEAD_DISTANCE);
    targetPosition.Y = currentPosition.Y + (cameraForward.Y * LOOK_AHEAD_DISTANCE); // Y remains constant for basic ground movement
    targetPosition.Z = currentPosition.Z + (cameraForward.Z * LOOK_AHEAD_DISTANCE);


    // --- STEP 3: Construct the View Matrix using LookAt ---
    //PXVector3F32 worldUp = { 0.0f, 1.0f, 0.0f };

    // Now we pass three *correctly calculated* vectors to LookAt:
    // The actual current position, the calculated target (forward), and world up.
    PXMatrix4x4FLookAt
    (
        &pxMatrixModelViewProjection->MatrixView,
        &currentPosition,       // Anchor Point (P)
        &targetPosition,       // Target Point (T = P + ForwardVector * Distance)
        &cameraUp                // Up Vector (U)
    );
#endif

    //PXMatrix4x4FInverse(&pxMatrixModelViewProjection->MatrixView);
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Perspective
    const PXF32 aspectRatio = PXCameraAspectRatio(camera);

    switch(camera->Perspective)
    {
        case PXCameraPerspective3D:
        {
            PXMatrix4x4FPerspective
            (
                projection,
                camera->FieldOfView,
                aspectRatio,
                camera->Near,
                camera->Far
            );

            break;
        }
        case PXCameraPerspective2D:
        {
            const PXF32 scaling = 0.005;
            const PXF32 left = -(camera->Rectangle.Width / 2.0f) * scaling;
            const PXF32 right = (camera->Rectangle.Width / 2.0f) * scaling;
            const PXF32 bottom = -(camera->Rectangle.Height / 2.0f) * scaling;
            const PXF32 top = (camera->Rectangle.Height / 2.0f) * scaling;

            PXMatrix4x4FOrthographic
            (
                projection,
                left, 
                right, 
                bottom, 
                top,
                camera->Near,
                camera->Far
            );
            break;
        }
        default:
        {
            break;
        }
    }
}
/*
void PXAPI PXCameraMakeMatrix2
(
    const PXCamera PXREF camera, 
    const PXTransform4D PXREF modelTransform,
    PXMatrixModelViewProjectionSquashed PXREF pxMatrixModelViewProjectionSquashed
)
{
    PXMatrixModelViewProjection pxMatrixModelViewProjection;

    PXCameraMakeMatrix(camera, modelTransform, &pxMatrixModelViewProjection);

    PXMatrix4x4FCopy(&pxMatrixModelViewProjectionSquashed->MatrixModelView, &pxMatrixModelViewProjection.MatrixModel);
    PXMatrix4x4FMultiply(&pxMatrixModelViewProjectionSquashed->MatrixModelView, &pxMatrixModelViewProjection.MatrixView);

    pxMatrixModelViewProjectionSquashed->MatrixProjection = pxMatrixModelViewProjection.MatrixProjection;
}*/