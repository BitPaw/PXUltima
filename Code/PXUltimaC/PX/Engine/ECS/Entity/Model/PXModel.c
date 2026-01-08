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


PXResult PXAPI PXModelRegisterToECS()
{
    return PXActionSuccessful;
}

void PXAPI PXModelConstruct(PXModel PXREF pxModel)
{
    PXClear(PXModel, pxModel);

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);

    // PXRectangleOffsetSet(&pxModel->Margin, 1, 1, 1, 1);
}
