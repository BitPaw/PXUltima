#include "PXModel.h"

#include <PX/Engine/PXResource.h>
#include <PX/Type/PXMesh.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/PXDictionaryECS.h>
#include <PX/Math/PXTransform.h>

#include <GL/gl.h>

typedef struct PXModel_
{
    PXECSInfo Info;

    PXDictionaryECS MeshLookup; // PXMesh
    PXDictionaryECS MaterialLookup; // PXMaterial

    PXTransform4D Transform;

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

    PXBool Invisible;
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
    PXNull
};
PXECSRegisterInfoDynamic PXModelRegisterInfoDynamic;



void PXAPI PXModelRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXModelRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXModelRegisterInfoDynamic;
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

    PXTransform4DInit(&pxModel->Transform);



    PXDictionaryECSCreate(&pxModel->MeshLookup);
    PXDictionaryECSCreate(&pxModel->MaterialLookup);

    PXModelMeshAmountPrepare(pxModel, pxModelCreateInfo->MeshListAmount);

    if(pxModelCreateInfo->MeshList)
    {
        for(size_t i = 0; i < pxModelCreateInfo->MeshListAmount; i++)
        {
            PXMesh PXREF pxMesh = &pxModelCreateInfo->MeshList[i];

            PXDictionaryECSAdd(&pxModel->MeshLookup, pxMesh);
        }
    }


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
        //PXModelMeshAmountSet(pxModel, pxModelCreateInfo->MeshAmount);
    }

    return PXResultOK;
}

PXResult PXAPI PXModelRelease(PXModel PXREF pxModel)
{
    return PXResultOK;
}

PXResult PXAPI PXModelMeshAmountPrepare(PXModel PXREF pxModel, const PXSize amount)
{
    PXSize amountCurrent = PXModelMeshAmount(pxModel);
    PXBool hasEnough = amountCurrent >= amount;

    if(hasEnough)
    {
        return PXResultOK;
    }

    // Not enough
    //pxModel->MeshLookup = PXMemoryHeapCallocT();

    return PXResultOK;
}

PXSize PXAPI PXModelMeshAmount(const PXModel PXREF pxModel)
{
    return PXDictionaryECSAmount(&pxModel->MeshLookup);
}

PXMesh* PXAPI PXModelMeshGet(const PXModel PXREF pxModel, const PXSize index)
{
    if(!pxModel)
    {
        return PXNull;
    }

    return (PXMesh*)PXDictionaryECSGetViaIndex(&pxModel->MeshLookup, index);
}

PXResult PXAPI PXModelMeshSet(PXModel PXREF pxModel, PXMesh PXREF pxMesh, const PXSize index)
{
    if(!(pxMesh && pxMesh))
    {
        return PXResultRefusedHandleNull;
    }
 
    PXResult pxResult = PXDictionaryECSReplace(&pxModel->MeshLookup, pxMesh, index);

    return pxResult;
}

PXResult PXAPI PXModelMaterialSet(PXModel PXREF pxModel, PXMaterial PXREF pxMaterial, const PXSize meshIndex, const PXSize segmentIndex)
{
    PXMesh PXREF pxMesh = PXModelMeshGet(pxModel, meshIndex);

    if(!pxMesh)
    {
        return PXResultRefusedIndexOutOfBounce;
    }

    PXIndexBuffer PXREF pxIndexBuffer = pxMesh->IndexBuffer;

    if(!pxIndexBuffer)
    {
        return PXResultRefusedHandleNull;
    }

    //PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, segmentIndex);

    PXDictionaryECSAdd(&pxModel->MaterialLookup, pxMaterial);

    return PXResultOK;
}

PXMaterial* PXAPI PXModelMaterialGetViaIndex(const PXModel PXREF pxModel, const PXSize mateialIndex)
{
    return (PXMaterial*)PXDictionaryECSGetViaIndex(&pxModel->MaterialLookup, mateialIndex);
}

PXMaterial* PXAPI PXModelMaterialGetForMeshIndex(const PXModel PXREF pxModel, const PXSize meshIndex, const PXSize segmentIndex)
{
    const PXMesh PXREF pxMesh = PXModelMeshGet(pxModel, meshIndex);

    if(!pxMesh)
    {
        return PXNull;// PXResultRefusedIndexOutOfBounce;
    }

    const PXIndexBuffer PXREF pxIndexBuffer = pxMesh->IndexBuffer;

    if(!pxIndexBuffer)
    {
        return PXNull;//PXResultRefusedHandleNull;
    }

    PXMaterial* pxMaterial = (PXMaterial*)PXDictionaryECSGetViaIndex(&pxModel->MaterialLookup, segmentIndex);

    return pxMaterial;
}

PXSize PXAPI PXModelMaterialAmount(const PXModel PXREF pxModel)
{
    return PXDictionaryECSAmount(&pxModel->MaterialLookup);
}

PXResult PXAPI PXModelShow(PXModel PXREF pxModel)
{
    pxModel->Invisible = PXFalse;

    return PXResultOK;
}

 PXResult PXAPI PXModelHide(PXModel PXREF pxModel)
{
     pxModel->Invisible = PXTrue;

     return PXResultOK;
}

 PXTransform4D* PXAPI PXModelTransformGet(const PXModel PXREF pxModel)
 {
     return &pxModel->Transform;
 }
