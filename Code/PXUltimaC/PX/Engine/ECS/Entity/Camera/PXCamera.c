#include "PXCamera.h"
#include <PX/Math/PXVector.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXResource.h>


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

PXResult PXAPI PXCameraRegisterToECS(void)
{
    PXECSRegister(&PXCameraRegisterInfoStatic, &PXCameraRegisterInfoDynamic);

    return PXResultOK;
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
    pxCamera->WalkSpeed = 2;
    pxCamera->ViewSpeed = 1;
    pxCamera->FollowSpeed = 0.98f;
    pxCamera->FieldOfView = 90;
    pxCamera->Height = 1;
    pxCamera->Width = 1;
    pxCamera->Near = 0.1;
    pxCamera->Far = 100000;

    //PXMatrix4x4FIdentity(&camera->MatrixModel);
    //PXMatrix4x4FIdentity(&camera->MatrixView);
    //PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(pxCamera, pxCameraCreateInfo->Perspective);

    const PXVector3F32 position = { 0,0,0 };
    PXCameraRotate(pxCamera, &position);

    return PXResultOK;
}

PXResult PXAPI PXCameraRelease(PXCamera PXREF pxCamera)
{
    return PXActionRefusedNotImplemented;
}




#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <gl/GL.h>

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



PXResult PXAPI PXCameraDraw(PXCamera PXREF pxCamera, PXDrawInfo PXREF pxDrawInfo)
{
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
    float fovY = fovY_deg * (float)(M_PI / 180.0);
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

    return PXActionRefusedNotImplemented;
}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera PXREF camera, const PXSize width, const PXSize height, const PXF32 nearPlane, const PXF32 farPlane)
{
    const PXF32 scaling = 0.005;
    const PXF32 left = -(width / 2.0f) * scaling;
    const PXF32 right = (width / 2.0f) * scaling;
    const PXF32 bottom = -(height / 2.0f) * scaling;
    const PXF32 top = (height / 2.0f) * scaling;

    camera->Perspective = PXCameraPerspective2D;
    camera->Width = width;
    camera->Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;

   // PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
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

    //PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera PXREF camera, const PXSize width, const PXSize height)
{
    if(!camera)
    {
        return;
    }

    camera->Width = width;
    camera->Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

PXF32 PXAPI PXCameraAspectRatio(const PXCamera PXREF camera)
{
    return (PXF32)camera->Width / (PXF32)camera->Height;
}

void PXAPI PXCameraViewChange(PXCamera PXREF camera, const PXCameraPerspective cameraPerspective)
{
    camera->Perspective = cameraPerspective;

    switch(cameraPerspective)
    {
        case PXCameraPerspective2D:
        {
            PXCameraViewChangeToOrthographic(camera, camera->Width, camera->Height, camera->Near, camera->Far);
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
    if(camera->LockView)
    {
        return;
    }

    const PXF32 maxValue = 85.0f;
    const PXF32 minValue = -85.0f;

    camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
    camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

    camera->CurrentRotation.Y = PXMathLimit(camera->CurrentRotation.Y, minValue, maxValue);

    const PXF32 pitchRAD = PXMathDegreeToRadians(camera->CurrentRotation.Y);
    const PXF32 yawRAD = PXMathDegreeToRadians(camera->CurrentRotation.X);
    const PXF32 rx = PXMathCosinusRADF32(pitchRAD) * PXMathCosinusRADF32(yawRAD);
    const PXF32 ry = PXMathSinusRADF32(pitchRAD);
    const PXF32 rz = PXMathCosinusRADF32(pitchRAD) * PXMathSinusRADF32(yawRAD);

    PXVector3F32SetXYZ(&camera->LookAtPosition, rx, ry, rz);
    PXVector3F32Normalize(&camera->LookAtPosition);
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

        PXVector3F32Add(&camera->Position, &targetedMovement);

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
    const PXF32 walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
    const PXF32 viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
    const PXVector3F32 up = { 0,1,0 };
    PXVector3F32 currentPosition;
    PXVector3F32 centerPosition;

   // PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    centerPosition = currentPosition;
   // PXVector3F32Add(&centerPosition, &camera->LookAtPosition);

   // PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up);
}

#include <math.h>

void PXAPI PXCameraForward(const PXCamera PXREF camera, PXVector3F32 PXREF forward)
{
    PXRotor PXREF pxRotor = &camera->Rotation;

    float yaw = PXMathDegreeToRadians(camera->CurrentRotation.X);
    float pitch = PXMathDegreeToRadians(camera->CurrentRotation.Y);

#if 1
    forward->X = cos(pitch) * cos(yaw);
    forward->Y = sin(pitch);
    forward->Z = cos(pitch) * sin(yaw);
#else
    forward->X = sin(yaw) * cos(pitch); 
    forward->Y = sin(pitch);
    forward->Z = cos(yaw) * cos(pitch);
#endif

    PXVector3F32Normalize(forward);
}

void PXAPI PXCameraRight(const PXCamera PXREF camera, PXVector3F32 PXREF right)
{
    PXRotor PXREF pxRotor = &camera->Rotation;
    float yaw = PXMathDegreeToRadians(camera->CurrentRotation.X);

    right->X = -sin(yaw);
    right->Y = 0.0;
    right->Z = cos(yaw);

    PXVector3F32Normalize(right);
}

void PXAPI PXCameraUp(const PXCamera PXREF camera, PXVector3F32 PXREF up)
{
    PXVector3F32 forward;
    PXVector3F32 right;

    PXCameraForward(camera, &forward);
    PXCameraRight(camera, &right);

    PXVector3F32CrossProduct(up, &right, &forward);

    PXVector3F32Normalize(up);
}