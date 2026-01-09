#include "PXModel.h"

#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>

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
    PXECSTypeEntity
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

PXResult PXAPI PXModelDraw(PXModel PXREF pxModel, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    return PXResultOK;
}