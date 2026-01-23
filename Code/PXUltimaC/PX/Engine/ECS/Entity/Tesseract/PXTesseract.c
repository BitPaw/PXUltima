#include "PXTesseract.h"

#include <PX/Math/PXMatrix.h>
#include <PX/Math/PXMath.h>
#include <math.h>
#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <gl/GL.h>

const char PXTesseractName[] = "Tesseract";
const PXI8U PXTesseractNameLength = sizeof(PXTesseractName);
const PXECSRegisterInfoStatic PXTesseractRegisterInfoStatic =
{
    {sizeof(PXTesseractName), sizeof(PXTesseractName), PXTesseractName, TextFormatASCII},
    sizeof(PXTesseract),
    __alignof(PXTesseract),
    PXECSTypeEntity,
    PXTesseractCreate,
    PXTesseractRelease,
    PXTesseractDraw
};
PXECSRegisterInfoDynamic PXTesseractRegisterInfoDynamic;




#define NUM_VERTICES 16
const PXVector4F32 PXTesseractVertices[] =
{
    {-1,-1,-1,-1},
    {1,-1,-1,-1},
    {-1,1,-1,-1},
    {1,1,-1,-1},
    {-1,-1,1,-1},
    {1,-1,1,-1},
    {-1,1,1,-1},
    {1,1,1,-1},
    {-1,-1,-1,1},
    {1,-1,-1,1},
    {-1,1,-1,1},
    {1,1,-1,1},
    {-1,-1,1,1},
    {1,-1,1,1},
    {-1,1,1,1},
    {1,1,1,1}
};

#define NUM_TRIANGLES 48
const PXI8U PXTesseractIndexTriangleFaces[NUM_TRIANGLES][3] =
{
    // Cube 1 faces (indices 0–7)
    {0,1,2},{1,3,2}, {4,5,6},{5,7,6},
    {0,1,4},{1,5,4}, {2,3,6},{3,7,6},
    {0,2,4},{2,6,4}, {1,3,5},{3,7,5},

    // Cube 2 faces (indices 8–15)
    {8,9,10},{9,11,10}, {12,13,14},{13,15,14},
    {8,9,12},{9,13,12}, {10,11,14},{11,15,14},
    {8,10,12},{10,14,12}, {9,11,13},{11,15,13},

    // Connections between cubes
    {0,8,1},{1,9,8}, {2,10,3},{3,11,10},
    {4,12,5},{5,13,12}, {6,14,7},{7,15,14},
    {0,8,2},{2,10,8}, {1,9,3},{3,11,9},
    {4,12,6},{6,14,12}, {5,13,7},{7,15,13}
};

float distance = 3.0f;


PXResult PXAPI PXTesseractRegisterToECS()
{
    PXECSRegister(&PXTesseractRegisterInfoStatic, &PXTesseractRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXTesseractCreate(PXTesseract** pxTesseractREF, PXTesseractCreateInfo PXREF pxTesseractCreateInfo)
{
    if(!(pxTesseractREF && pxTesseractCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXTesseract* pxTesseract = PXNull;

    pxTesseractCreateInfo->Info.Static = &PXTesseractRegisterInfoStatic;
    pxTesseractCreateInfo->Info.Dynamic = &PXTesseractRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxTesseractREF, pxTesseractCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTesseract = *pxTesseractREF;

    return PXResultOK;
}

PXResult PXAPI PXTesseractRelease(PXTesseract PXREF pxTesseract)
{
    return PXActionRefusedNotImplemented;
}

float angle = 0;
float dmax = 0.5;

PXResult PXAPI PXTesseractDraw(PXTesseract PXREF pxTesseract, PXDrawInfo PXREF pxDrawInfo)
{
    PXCamera PXREF pxCamera = pxDrawInfo->Camera;

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_DITHER);

    glScalef(2,2,2);
    glTranslatef(5.0f, 2.0f, -0.5);

    //pxRenderEntity->Draw();

   // distance -= angle;

    angle += 0.005f;

    // Create rotation matrices
    PXMatrix4x4F rotXY;
    PXMatrix4x4F rotXW;
    PXMatrix4x4F rotYZ;

    //y22 += angle*0.02;    

    PXMatrix4x4FIdentity(&rotXY);
    PXMatrix4x4FIdentity(&rotXW);
    PXMatrix4x4FIdentity(&rotYZ);
    //  Matrix4x4RotationSet4D(&rotXW, 0, 2, angle);
    // Matrix4x4RotationSet4D(&rotXW, 0, 3, angle);    
     // Matrix4x4RotationSet4D(&rotXY, 0, 1, angle);

    PXMatrix4x4FRotationAxisSet(&rotXW, 1, 3, angle);
    PXMatrix4x4FRotationAxisSet(&rotYZ, 0, 2, angle);



    PXMatrix4x4F combined;

    PXMatrix4x4FIdentity(&combined);
    PXMatrix4x4FMultiply(&combined, &rotXY);
    PXMatrix4x4FMultiply(&combined, &rotYZ);
    PXMatrix4x4FMultiply(&combined, &rotXW);

    PXVector4F32 lineColor = { 0.15f,0.15f,0.15f,0 };
    PXVector4F32 dotColor = { 1.0f,0,0,0 };


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); // or GL_FRONT



    GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f }; // Positional light
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);


    int trianglesToRender = NUM_TRIANGLES; // PXD20_Triangles

    // Draw triangle faces
#if 1
    glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    int oneCube = NUM_TRIANGLES / 3;

    for(int i = 0; i < trianglesToRender; ++i)
    {
        PXVector4F32 vA = PXTesseractVertices[PXTesseractIndexTriangleFaces[i][0]];
        PXVector4F32 vB = PXTesseractVertices[PXTesseractIndexTriangleFaces[i][1]];
        PXVector4F32 vC = PXTesseractVertices[PXTesseractIndexTriangleFaces[i][2]];

        PXVector3F32 n;

        PXVector4FNormal(&n, &vA, &vB, &vC, distance);

        glNormal3f(n.X, n.Y, n.Z);

        for(int j = 0; j < 3; ++j)
        {

            PXVector4F32 vOrg = PXTesseractVertices[PXTesseractIndexTriangleFaces[i][j]];

            PXVector4F32 v = vOrg;
            PXVector3F32 p;


            PXMatrix4x4FMultiplyV4F(&combined, &v);
            PXVector4Demote(&v, &p, distance);

            // Color based on w (depth cue)
            float color = (v.W + 1.0f) / 2.0f;
            

       
        

            PXVector4F32 ccolor = (vOrg);
            ccolor.X = 1 - (ccolor.X + color) / 2.0f;
            ccolor.Y = 1 - (ccolor.Y + color) / 2.0f;
            ccolor.Z = 1 - (ccolor.Z + color) / 2.0f;

#if 0
            float d = abs(vOrg.W - pxCamera->Position.W);
            float t = PXMathClampF(d / dmax, 0.0, 1.0);
            ccolor.W = 1.0 - (t * t * (3.0 - 2.0 * t));  // smoothstep fade
#else
            ccolor.W = 1.0;
#endif



            glColor4f(ccolor.X, ccolor.Y, ccolor.Z, ccolor.W);

            // glVertex4f();

            glVertex3f(p.X, p.Y, p.Z);
        }
    }
    glEnd();

    glDepthMask(GL_TRUE);
#endif






    // Draw triangle faces
#if 0
   // glDisable(GL_BLEND);
    //glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_LINES);

    for(int i = 0; i < trianglesToRender; ++i)
    {
#if usehypercube
        Vec4 vA = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][0]]);
        Vec4 vB = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][1]]);
        Vec4 vC = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][2]]);
#else
        Vec4 vA = PXD20_vertices[PXD20_indices[i * 3 + 0]];
        Vec4 vB = PXD20_vertices[PXD20_indices[i * 3 + 1]];
        Vec4 vC = PXD20_vertices[PXD20_indices[i * 3 + 2]];
#endif

        Vec3 vProj3DA = project4Dto3D(multiplyMatrixVec4(combined, vA), distance);
        Vec3 vProj3DB = project4Dto3D(multiplyMatrixVec4(combined, vB), distance);
        Vec3 vProj3DC = project4Dto3D(multiplyMatrixVec4(combined, vC), distance);

        Vec3 normalStart = computeNormal
        (
            vProj3DA,
            vProj3DB,
            vProj3DC
        );

        float scale = 0.05f; // Adjust length of the normal line


        Vec3 surfacePosition = Vec3TriangleCenter(&vProj3DA, &vProj3DB, &vProj3DC);


        Vec3 normalDirection;
        normalDirection.x = surfacePosition.x + normalStart.x * scale;
        normalDirection.y = surfacePosition.y + normalStart.y * scale;
        normalDirection.z = surfacePosition.z + normalStart.z * scale;

        //glColor4f(1.0, 0, 1.0, 1.0);
        //glVertex3f(vProj3DA.x, vProj3DA.y, vProj3DA.z);
        //glVertex3f(vProj3DB.x, vProj3DB.y, vProj3DB.z);
        //glVertex3f(vProj3DC.x, vProj3DC.y, vProj3DC.z);

        glColor4f(0.0, 1.0, 0, 1.0);
        glVertex3f(surfacePosition.x, surfacePosition.y, surfacePosition.z);
        glColor4f(1.0, 1.0, 0, 1.0);
        glVertex3f(normalDirection.x, normalDirection.y, normalDirection.z);
    }
    glEnd();
#endif




    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2);
    glPointSize(10.0f);
    glBegin(GL_TRIANGLES);

    glColor4f(lineColor.X, lineColor.Y, lineColor.Z, 1.0f);
    //glColor4f(dotColor.x, dotColor.y, dotColor.z, 1.0f);

    for(int i = 0; i < trianglesToRender; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            PXVector4F32 vOrg = PXTesseractVertices[PXTesseractIndexTriangleFaces[i][j]];

            PXVector4F32 v;
            PXVector3F32 p;

            v = vOrg;

            PXMatrix4x4FMultiplyV4F(&combined, &v);
            PXVector4Demote(&v, &p, distance);

            glVertex3f(p.X, p.Y, p.Z);
        }
    }
    glEnd();

    return PXResultOK;
}