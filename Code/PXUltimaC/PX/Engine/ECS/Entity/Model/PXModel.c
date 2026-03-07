#include "PXModel.h"

#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <PX/OS/PXOS.h>
#include <gl/GL.h>

typedef struct PXModel_
{
    PXECSInfo Info;

    PXSize MeshAmount;
    PXMesh** MeshList;

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

        //PXMeshCreate(&pxModel->Mesh, &pxMeshCreateInfo);
    }
    else
    {
        // No path specifies, are we loading something predefined?
        PXModelMeshAmountSet(pxModel, pxModelCreateInfo->MeshAmount);
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

PXResult PXAPI PXModelDrawGL(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo)
{
    if(!(pxModel && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXSize meshAmount = pxModel->MeshAmount;    

    for(PXSize meshIndex = 0; meshIndex < meshAmount; ++meshIndex)
    {
        PXMesh PXREF pxMesh = pxModel->MeshList[meshIndex];

        PXMeshDraw(pxMesh, pxDrawInfo);
    }    
    
    return PXResultOK;
}

PXResult PXAPI PXModelMeshAmountSet(PXModel PXREF pxModel, const PXSize amount)
{
    pxModel->MeshAmount = amount;
    pxModel->MeshList = PXMemoryHeapCallocT(PXMesh*, amount);

    return PXResultOK;
}

PXSize PXAPI PXModelMeshAmountGet(const PXModel PXREF pxModel)
{
    return pxModel->MeshAmount;
}

PXMesh** PXAPI PXModelMeshGet(const PXModel PXREF pxModel, const PXSize index)
{
    if(pxModel->MeshAmount < index)
    {
        return PXNull;
    }

    return &pxModel->MeshList[index];
}