#include "PXEnginePhysics.h"

void PXAPI PXEngineCollsisionSolve(PXEngine PXREF pxEngine)
{
#if 0

    PXDictionary PXREF hitBoxLookUp = &pxEngine->ResourceManager->HitBoxLookUp;

    for(PXSize indexA = 0; indexA < hitBoxLookUp->List.EntryAmountUsed; ++indexA)
    {
        PXDictionaryEntry pxDictionaryEntryA;
        PXHitBox* pxHitBoxA = PXNull;

        PXDictionaryIndex(hitBoxLookUp, indexA, &pxDictionaryEntryA);

        pxHitBoxA = *(PXHitBox**)pxDictionaryEntryA.Value;

        if(!(pxHitBoxA->Info.Behaviour & PXECSInfoActive))
        {
            continue;
        }

        for(PXSize indexB = 0; indexB < hitBoxLookUp->List.EntryAmountUsed; ++indexB)
        {
            PXDictionaryEntry pxDictionaryEntryB;
            PXHitBox* hitBoxB = PXNull;

            PXDictionaryIndex(hitBoxLookUp, indexB, &pxDictionaryEntryB);

            hitBoxB = *(PXHitBox**)pxDictionaryEntryB.Value;

            if(!(pxHitBoxA->Info.Behaviour & PXECSInfoActive))
            {
                continue;
            }



            /*
            const PXBool isCollising = PXCollisionAABB
            (
                currentOffset.Left,
                currentOffset.Top,
                currentOffset.Right,
                currentOffset.Bottom,
                bfEngine->CollisionCheckInfo.MousePosition[0],
                bfEngine->CollisionCheckInfo.MousePosition[1],
                bfEngine->CollisionCheckInfo.MousePosition[2],
                bfEngine->CollisionCheckInfo.MousePosition[3]
            );*/
        }
    }

#endif
}

void PXAPI PXEngineUpdateCollision(PXEngine PXREF pxEngine)
{

#if 0

#if 0
    PXLogPrint
    (
        PXLoggingInfo,
        "HitBox",
        "Compare",
        "--- START ---"
    );
#endif

    PXDictionary PXREF hitboxLookup = &pxEngine->ResourceManager->HitBoxLookUp;

    PXSize amount = hitboxLookup->List.EntryAmountUsed;


    PXDictionaryEntry pxDictionaryEntry;
    PXHitBox* pxHitBoxA = PXNull;
    PXHitBox* pxHitBoxB = PXNull;
    PXVector3F32* pxHitBoxAVertex = PXNull;
    PXVector3F32* pxHitBoxBVertex = PXNull;
    PXSize pxHitBoxAAmount = 0;
    PXSize pxHitBoxBAmount = 0;



    // Player vs model
    PXCamera PXREF pxCamera = pxEngine->CameraCurrent;


    PXDictionary PXREF modelList = &pxEngine->ResourceManager->ModelLookUp;

    PXVector3F32 position;
    PXMatrix4x4FPositionGet(&pxCamera->MatrixView, &position);

#if 0
    PXConsoleGoToXY(0, 0);
    PXConsoleWrite(7, "Camera POS\n");
    PXConsoleWriteTablePXF32(&position, 3, 3);
#endif


    for(size_t i = 0; i < modelList->List.EntryAmountUsed; i++)
    {
        PXModel* pxModel = PXNull;

        PXDictionaryIndex(modelList, i, &pxDictionaryEntry);

        pxModel = *(PXModel**)pxDictionaryEntry.Value;



        if(!pxModel->Mesh.VertexBufferList)
        {
            continue;
        }

        PXVertexBuffer PXREF pxVertexBuffer = &pxModel->Mesh.VertexBufferList[0];

        PXIndexBuffer PXREF pxIndexBuffer = &pxModel->Mesh.IndexBuffer;

        /*
        for(PXSize i = 0; i < pxIndexBuffer->SegmentListAmount; i++)
        {
            pxIndexBuffer->SegmentList[]
        }*/

        PXVector3F32 forward = { 0,0,-1.0 };

        const PXSize amount = PXMeshTriangleAmount(&pxModel->Mesh);


        pxModel->Range = PXRayTriangleIntersect
        (
            &position,
            &pxCamera->LookAtPosition,
            pxVertexBuffer->VertexData.Data,
            amount
        );

        if(pxModel->Range == -1)
        {
            continue;
        }

#if 1
        PXLogPrint
        (
            PXLoggingInfo,
            "HitBox",
            "Compare",
            "HIT-Range: %f",
            pxModel->Range
        );
#endif
    }






#if 0
    // Hitbox vs model
    for(PXSize i = 0; i < amount; ++i)
    {
        PXDictionaryIndex(hitboxLookup, i, &pxDictionaryEntry);

        pxHitBoxA = *(PXHitBox**)pxDictionaryEntry.Value;
        pxHitBoxAAmount = PXMeshTriangleAmount(&pxHitBoxA->Model->Mesh);

        for(PXSize j = i + 1; j < amount; ++j)
        {
            PXDictionaryIndex(hitboxLookup, j, &pxDictionaryEntry);

            pxHitBoxB = *(PXHitBox**)pxDictionaryEntry.Value;
            pxHitBoxBAmount = PXMeshTriangleAmount(&pxHitBoxB->Model->Mesh);

            for(PXSize k = 0; k < pxHitBoxAAmount; k += 3)
            {
                float* x = pxHitBoxA->Model->Mesh.VertexBufferList[0].VertexData;

                pxHitBoxAVertex = &x[k];

                for(PXSize l = 0; l < pxHitBoxBAmount; l += 3)
                {
                    float* y = pxHitBoxB->Model->Mesh.VertexBufferList[0].VertexData;

                    pxHitBoxBVertex = &y[l];

                    PXF32 intersectionDistance = PXRayTriangleIntersect
                    (
                        pxHitBoxAVertex,
                        PXNull,
                        pxHitBoxBVertex
                    );

                    if(intersectionDistance)
                    {
                        // pxHitBoxA->Model->Info.Setting

#if 1
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "HitBox",
                            "Compare",
                            "HIT"
                        );
#endif
                    }
                }
            }
        }
    }
#endif


    /*

    // Clear
    {

        for(PXSize index = 0; index < hitboxLookup->EntryAmountCurrent; ++index)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXHitBox* pxHitBoxA = PXNull;

            PXDictionaryIndex(hitboxLookup, index, &pxDictionaryEntry);

            pxHitBoxA = *(PXHitBox**)pxDictionaryEntry.Value;

            pxHitBoxA->ColliderChild = PXNull;
            pxHitBoxA->ColliderParent = PXNull;
        }
    }


    for(PXSize index = 0; index < hitboxLookup->EntryAmountCurrent; ++index)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXHitBox* pxHitBoxA = PXNull;

        PXDictionaryIndex(hitboxLookup, index, &pxDictionaryEntry);

        pxHitBoxA = *(PXHitBox**)pxDictionaryEntry.Value;

        if(!(pxHitBoxA->Info.Behaviour & PXECSInfoActive))
        {
            continue;
        }

        for(PXSize indexB = 0; indexB < hitboxLookup->EntryAmountCurrent; ++indexB)
        {
            PXDictionaryEntry pxDictionaryEntry;
            PXHitBox* pxHitBoxB = PXNull;

            PXDictionaryIndex(hitboxLookup, indexB, &pxDictionaryEntry);

            pxHitBoxB = *(PXHitBox**)pxDictionaryEntry.Value;

            if(!(pxHitBoxB->Info.Behaviour & PXECSInfoActive))
            {
                continue;
            }

            if(pxHitBoxA->Info.ID == pxHitBoxB->Info.ID)
            {
                continue;
            }


            PXEngineHitBoxHandleAvsB(pxEngine, pxHitBoxA, pxHitBoxB);

        }

    }*/

#if 0
    PXLogPrint
    (
        PXLoggingInfo,
        "HitBox",
        "Compare",
        "--- DONE ---"
    );
#endif


#endif
}

PXResult PXAPI PXEnginePhysicsUpdate(PXEnginePhysics PXREF pxEnginePhysics, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
    return PXResultOK;
}