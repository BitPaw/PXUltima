#include "PXModel.h"

#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <gl/GL.h>

typedef struct PXModel_
{
    PXECSInfo Info;

    PXMesh* Mesh;

    //-----------------------------
    // Render info
    //-----------------------------
    PXMatrix4x4F ModelMatrix;
    PXShaderProgram* ShaderProgramReference;
    //-----------------------------

   // PXModel* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
    //PXModel* StructureParent; // Structural parent of structure
    //XModel* StructureSibling; // Stuctual sibling, works like a linked list.
    //PXModel* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
    //-----------------------------

    //-----------------------------
    // Settings
    //-----------------------------
    PXBool IgnoreViewPosition; // Removes positiondata from the view matrix
    PXBool IgnoreViewRotation; // remove rotationdata from the view matrix
    PXBool RenderBothSides;
    //  Margin ??
    //-----------------------------
    struct PXHitBox_* HitBox;

    PXF32 Range;
}
PXModel;


const char PXModelName[] = "Model";
const PXI8U PXModelNameLength = sizeof(PXModelName);
const PXECSRegisterInfoStatic PXModelRegisterInfoStatic =
{
    {sizeof(PXModelName), sizeof(PXModelName), PXModelName, TextFormatASCII},
    sizeof(PXModel),
    __alignof(PXModel),
    PXECSTypeEntity,
    PXModelCreate,
    PXModelRelease,
    PXModelDraw
};
PXECSRegisterInfoDynamic PXModelRegisterInfoDynamic;



PXResult PXAPI PXModelRegisterToECS()
{
    PXECSRegister(&PXModelRegisterInfoStatic, &PXModelRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXModelCreate(PXModel** pxModelREF, PXModelCreateInfo PXREF pxModelCreateInfo)
{
    PXModel* pxModel = PXNull;

    pxModelCreateInfo->Info.Static = &PXModelRegisterInfoStatic;
    pxModelCreateInfo->Info.Dynamic = &PXModelRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxModelREF, pxModelCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxModel = *pxModelREF;

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);

    // We want to load the mesh from a file or embedded
    PXText* filePath = &pxModelCreateInfo->Info.FilePath;

    if(filePath->Data)
    {
        // Path defined, lets load it!
        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        pxMeshCreateInfo.Info = pxModelCreateInfo->Info;

        PXMeshCreate(&pxModel->Mesh, &pxMeshCreateInfo);
    }
    else
    {
        // No path specifies, are we loading something predefined?
    }

    return PXResultOK;
}

PXResult PXAPI PXModelRelease(PXModel PXREF pxModel)
{
    return PXResultOK;
}

PXResult PXAPI PXModelDraw(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo)
{
    PXModelDrawGL(pxModel, pxDrawInfo);

    return PXResultOK;
}

#include <PX/OS/Console/PXConsole.h>

PXResult PXAPI PXModelDrawGL(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo)
{
    PXMesh PXREF pxMesh = pxModel->Mesh;
    PXMeshGeometry PXREF pxMeshGeometry = pxMesh->Geometry;






    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer
    (
        3,
        GL_FLOAT,
        0,
        pxMeshGeometry->VertexBufferPrime[0]->VertexData.Data
    );
    glScalef(3,3,3);
    glTranslatef(-20, 0, 0);

    float* ddat = pxMeshGeometry->VertexBufferPrime[0]->VertexData.Data;
    PXSize amountff = pxMeshGeometry->VertexBufferPrime[0]->VertexData.CursorOffsetByte / sizeof(float);

#if 0
    for(size_t i = 0; i < amountff; i+=3)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Model",
            "Color-Set",
            "%6.2f %6.2f %6.2f",
            ddat[i + 0],
            ddat[i + 1],
            ddat[i + 2]
        );
#endif 
    }
#endif


    if(1)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer
        (
            GL_FLOAT,
            0,
            pxMeshGeometry->VertexBufferPrime[1]->VertexData.Data
        );

    }

    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    PXSize amount = pxMeshGeometry->IndexBuffer->Segment.SegmentListAmount;
    PXIndexSegmentEntry* pxIndexSegmentEntryList = PXIndexSegmentGET(&pxMeshGeometry->IndexBuffer->Segment);

    PXSize offset = 0;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXIndexSegmentEntry* pxIndexSegmentEntry = &pxIndexSegmentEntryList[i];
       // PXMaterial* mat = pxIndexSegmentEntry->material;

        // Apply material properties
      //  GLfloat diffuse[4] = { mat->Kd[0], mat->Kd[1], mat->Kd[2], mat->d };
       // GLfloat ambient[4] = { mat->Ka[0], mat->Ka[1], mat->Ka[2], mat->d };
       // GLfloat specular[4] = { mat->Ks[0], mat->Ks[1], mat->Ks[2], mat->d };

      //  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
       // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
       // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
       // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->Ns);

        // Bind texture if available
        if(0) {
            //glEnable(GL_TEXTURE_2D);
           // glBindTexture(GL_TEXTURE_2D, mat->texture_id);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }

        PXSize typeSize = PXTypeSizeGet(pxMeshGeometry->IndexBuffer->DataType);

        // Draw this segment
       
        //glTexCoordPointer(2, GL_FLOAT, sizeof(VertexPNT), &model->verts[0].tu);
       





        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        GLfloat lightPos[] = { 1.0f, 10.0f, 1.0f, 100.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        glColor3f(1.0, 1.0, 1.0);
        glDrawArrays
        (
            GL_TRIANGLES,
            offset,
            pxIndexSegmentEntry->DataRange
        );

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_NORMALIZE);


        glPointSize(8);
        glColor3f(0.8, 0.2, 0.2);
        glDrawArrays
        (
            GL_POINTS,
            offset,
            pxIndexSegmentEntry->DataRange
        );

        glLineWidth(1);
        glColor3f(0.6, 1.0, 0.6);
        glDrawArrays
        (
            GL_LINES,
            offset,
            pxIndexSegmentEntry->DataRange
        );

      



       /*
        glDrawElements
        (
            GL_POINTS,
            pxIndexSegmentEntry->DataRange, 
            GL_UNSIGNED_SHORT,
            pxMesh->IndexBuffer->Data.Data + offset
        );*/

        offset += pxIndexSegmentEntry->DataRange * typeSize;
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    return PXResultOK;
}
