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
    PXECSCreate(pxModelREF, pxModelCreateInfo);

    pxModel = *pxModelREF;

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);

    return PXResultOK;
}

PXResult PXAPI PXModelRelease(PXModel PXREF pxModel)
{
    return PXResultOK;
}

PXResult PXAPI PXModelDraw(PXModel PXREF pxModel, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXModelDrawGL(pxModel, pxWindowDrawInfo);

    return PXResultOK;
}

PXResult PXAPI PXModelDrawGL(PXModel PXREF pxModel, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(VertexPNT), &model->verts[0].px);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, sizeof(VertexPNT), &model->verts[0].nx);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for(int i = 0; i < model->segment_count; ++i) {
        PXMeshSegment* seg = &model->segments[i];
        PXMaterial* mat = seg->material;

        // Apply material properties
        GLfloat diffuse[4] = { mat->Kd[0], mat->Kd[1], mat->Kd[2], mat->d };
        GLfloat ambient[4] = { mat->Ka[0], mat->Ka[1], mat->Ka[2], mat->d };
        GLfloat specular[4] = { mat->Ks[0], mat->Ks[1], mat->Ks[2], mat->d };

        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->Ns);

        // Bind texture if available
        if(mat->texture_id) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mat->texture_id);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }

        // Draw this segment
        glTexCoordPointer(2, GL_FLOAT, sizeof(VertexPNT), &model->verts[0].tu);
        glDrawElements(GL_TRIANGLES, seg->index_count, GL_UNSIGNED_SHORT, seg->indices);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    */
    return PXResultOK;
}
