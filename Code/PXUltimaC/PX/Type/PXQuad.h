#pragma once
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXModel.h>

typedef struct PXQuad_
{
    PXECSInfo Info;

    PXModel* Model;
}
PXQuad;

PXInternal void PXAPI PXQuadRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXQuadCreate(PXQuad** pxQuadREF);
