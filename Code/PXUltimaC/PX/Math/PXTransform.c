#include "PXTransform.h"

#include <PX/OS/PXOS.h>

void PXAPI PXTransform3DInit(PXTransform3D PXREF pxTransform3D)
{
    if(!pxTransform3D)
    {
        return;
    }

    PXRotor3DF32Identity(&pxTransform3D->Rotor);
    PXVector3F32SetXYZ(&pxTransform3D->Position, 0, 0, 0);
    PXVector3F32SetXYZ(&pxTransform3D->Scale, 1, 1, 1);
}

void PXAPI PXTransform4DInit(PXTransform4D PXREF pxTransform4D)
{
    if(!pxTransform4D)
    {
        return;
    }

    PXRotor4DF32Identity(&pxTransform4D->Rotor);
    PXVector4F32SetXYZW(&pxTransform4D->Position, 0, 0, 0, 0);
    PXVector4F32SetXYZW(&pxTransform4D->Scale, 1, 1, 1, 1);
}

void PXAPI PXTransform4DToMatrix(const PXTransform4D PXREF pxTransform4D, PXMatrix4x4F PXREF pxMatrix4x4F)
{
    PXMatrix4x4FIdentity(pxMatrix4x4F);

    // Rotation
    PXMatrix4x4F rotationMatrix;
    //PXRotor4DF32ToMatrix4x4F32(&pxTransform4D->Rotor, &rotationMatrix);
    //PXMatrix4x4FMultiply(pxMatrix4x4F, &rotationMatrix);
    PXMatrix4x4FScale4D(pxMatrix4x4F, &pxTransform4D->Scale);
    PXMatrix4x4FMove4F(pxMatrix4x4F, &pxTransform4D->Position);
}