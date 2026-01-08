#include "PXRGeometricWorld.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>


const char PXRGeometryText[] = "RGeometricWorld";


const char PXRGeometrySignature[4] = "WGEO";

const PXI32U PXRGeometryHeaderList[] =
{
    PXTypeDatax4, // PXTypeSignatureCheck
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeInt32U,
    PXTypeInt32U
};
const PXI8U PXRGeometryHeaderSize = sizeof(PXRGeometryHeaderList) / sizeof(PXI32U);







const PXI32U PXRGeometryMeshList[] =
{
    PXTypeText(260),
    PXTypeText(64),

    PXTypeF32List(4),
    PXTypeF32List(6),
   
    PXTypeInt32U,
    PXTypeInt32U
};
const PXI8U PXRGeometryMeshListSize = sizeof(PXRGeometryMeshList) / sizeof(PXI32U);



PXResult PXAPI PXRGeometricWorldLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    PXRGeometricWorld pxRGeometricWorld;
    PXClear(PXRGeometricWorld, &pxRGeometricWorld);


    // Check if we have a model to store stuff into
    PXMesh PXREF pxMesh = PXNull;


    const PXSize amount = PXFileBinding
    (
        pxFile,
        &pxRGeometricWorld.Header,
        PXRGeometryHeaderList,
        PXRGeometryHeaderSize,
        PXFileBindingRead
    );


    // Signature ok?
    if(0)
    {

    }

    // version OK?
    if(0)
    {

    }




#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXRGeometryText,
        "Load",
        "Meshes:%i, Face:%i",
        pxRGeometricWorld.Header.ModelCount,
        pxRGeometricWorld.Header.FaceCount
    );
#endif



    // Peek end

    // Allocate
    pxRGeometricWorld.GeometryMeshList = PXMemoryHeapCallocT(PXRGeometricWorldMesh, pxRGeometricWorld.Header.ModelCount);


    for(PXI32U i = 0; i < pxRGeometricWorld.Header.ModelCount; ++i)
    {
        PXRGeometricWorldMesh PXREF pxRGeometryMesh = &pxRGeometricWorld.GeometryMeshList[i];

        const PXSize amount = PXFileBinding
        (
            pxFile,
            pxRGeometryMesh,
            PXRGeometryMeshList,
            PXRGeometryMeshListSize,
            PXFileBindingRead
        );

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXRGeometryText,
            "Load",
            "(%4i/%4i) Vertex:%6i, Index:%6i, Texture:%s",
            i+1,
            pxRGeometricWorld.Header.ModelCount,
            pxRGeometryMesh->VertexCount,
            pxRGeometryMesh->IndexCount,
            pxRGeometryMesh->TexturePath
        );
#endif



        PXSize veretxDataAmount = (2+3) * pxRGeometryMesh->VertexCount; // Expected format is XYZUV
        PXF32* vertexData = PXMemoryHeapCallocT(PXF32, veretxDataAmount);
        


        // if the max index size of smaler then 2^16 only then we use 16-Bit ints
        PXI8U typeSize = 0;

        if(pxRGeometryMesh->IndexCount <= 0xFFFFu)
        {
            typeSize *= sizeof(PXI16U);
        }
        else
        {
            typeSize *= sizeof(PXI32U);
        }

        void* IndexData = PXMemoryHeapCalloc(PXNull, pxRGeometryMesh->IndexCount, typeSize);

        // Read buffers
        {
            PXFileReadB(pxFile, vertexData, sizeof(PXF32) * veretxDataAmount);
            PXFileReadB(pxFile, vertexData, pxRGeometryMesh->IndexCount * typeSize);
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXRGeometricWorldSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}