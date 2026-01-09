#include "PXMesh.h"
#include <PX/Engine/PXResource.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>
#include "PXIndexBuffer.h"

const char PXIndexBufferText[] = "IndexBuffer";
const char PXVertexBufferText[] = "VertexBuffer";
const char PXMeshText[] = "Mesh";
const PXI8U PXMeshTextLength = sizeof(PXMeshText);
const PXECSRegisterInfoStatic PXMeshRegisterInfoStatic =
{
    {sizeof(PXMeshTextLength), sizeof(PXMeshTextLength), PXMeshText, TextFormatASCII},
    sizeof(PXMesh),
    __alignof(PXMesh),
    PXECSTypeResource
};
PXECSRegisterInfoDynamic PXMeshRegisterInfoDynamic;


const PXF32 PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
        1.0f, -1.0f,
        0.5f,  1.0f
};

const PXI8S PXIndexDataTriangle[] =
{
    0,1,2
};

// This only works if we dont use any texturepositions
const PXF32 PXVertexDataRectangle[] =
{
    -1, -1,
     1, -1,
     1,  1,
    -1,  1
};

const PXF32 PXVertexDataRectangleTX[] =
{
    -1, -1, 1, 1, // Bottom-left
     1, -1, 0, 1, // Bottom-right
     1,  1, 0, 0,  // Top-right
    -1,  1, 1, 0   // Top-left
};

const PXI8U PXIndexDataRectangle[] =
{
    0,1,2,
    2,3,0
};

const PXF32 PXVertexDataCube[] = // PXI8S
{
    -1, -1, -1,
        1, -1, -1,
        -1,  1, -1,
        1,  1, -1,
        -1, -1,  1,
        1, -1,  1,
        -1,  1,  1,
        1,  1,  1,
};

const PXI8U PXIndexDataCube[] =
{
#if 0
    0,1,2,3, // Left
    0,2,4,6, // Back
    2,3,7,6, // Top
    4,5,6,7, // Right
    0,1,4,5, // Bottom
    1,3,5,7  // Font
#else

    // Left
    0,1,2,
    2,3,1,

    // Right
    4,5,6,
    6,7,5,

    // Back
    0,2,4,
    4,6,2,

    // Font
    1,3,5,
    5,7,3,

    // Bottom
    0,1,4,
    4,5,1,

    // Top
    2,3,7,
    7,6,2

#endif
};


PXResult PXAPI PXMeshRegisterToECS()
{
    PXECSRegister(&PXMeshRegisterInfoStatic, &PXMeshRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXMeshCreate(PXMesh** pxMeshREF, PXMeshCreateInfo PXREF pxMeshCreateInfo)
{
    PXMesh* pxMesh = PXNull;

    PXECSCreate(pxMeshREF, pxMeshCreateInfo);

    pxMesh = *pxMeshREF;

    pxMesh->VertexBufferListAmount = 1;
    PXVertexBuffer PXREF pxVertexBuffer = PXMeshVertexBufferListGET(pxMesh);
    PXIndexBuffer PXREF pxIndexBuffer = &pxMesh->IndexBuffer;

    const PXBool hasFilePath = PXNull != 0;// pxResourceCreateInfo->FilePath.A;

    if(hasFilePath)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXMeshText,
            "Create",
            "Model ID:%i <%s>.",
            pxMesh->Info.ID,
            pxMeshCreateInfo->Info.FilePath.A
        );
#endif

        // Load model
        {
            PXResourceMoveInfo pxResourceLoadInfo;
            PXClear(PXResourceMoveInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.ResourceTarget = pxMesh;
            pxResourceLoadInfo.ResourceType = PXResourceTypeModel;

            const PXResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxMeshCreateInfo->Info.FilePath);
            const PXBool success = PXResultOK == loadResult;

            if(!success)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Load",
                    "ID:%i Failed",
                    pxMesh->Info.ID
                );
#endif
                return loadResult;
            }
        }
    }
    else
    {
        // As this element is internal, we need to create an ID
        pxMesh->Info.ID = PXIDGenerate();

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXMeshText,
            "Create",
            "ID:%i internal",
            pxMesh->Info.ID
        );
#endif


        switch(0) // pxModelCreateInfo->Form
        {
            case PXModelFormCustom:
            {
                // Can the data be used form a const source? If so, we dont need to copy
                const PXBool isDataConst =
                    PXECSInfoPermissionREAD & pxMesh->Info.Behaviour &&
                    PXECSInfoStorageMemory & pxMesh->Info.Behaviour;
                /*
                if(!isDataConst)
                {
                    PXCopy(PXVertexBuffer, &pxModelCreateInfo->VertexBuffer, pxVertexBuffer);
                    PXCopy(PXIndexBuffer, &pxModelCreateInfo->IndexBuffer, pxIndexBuffer);

                    // Allocate memory and copy
                    pxIndexBuffer->DataIndexPosition = 0;
                    pxIndexBuffer->DataIndexSizeSegment = 0;
                    pxVertexBuffer->VertexData.Size = pxModelCreateInfo->VertexBuffer.VertexData.Size;
                    pxVertexBuffer->VertexData.Data = PXMemoryHeapCallocT(PXByte, pxModelCreateInfo->VertexBuffer.VertexData.Size, &pxModel->Mesh.VertexBuffer.VertexData, &pxModel->Mesh.VertexBuffer.VertexDataSize);

                    PXBufferAllocate(&pxModel->Mesh.IndexBuffer.Data, );

                    pxModel->Mesh.IndexBuffer.DataIndexSizeSegment = pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment;
                    pxModel->Mesh.IndexBuffer.DataIndexPosition = PXMemoryHeapCallocT
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment,
                        &pxModel->Mesh.IndexBuffer.DataIndexPosition,
                        &pxModel->Mesh.IndexBuffer.DataIndexSizeSegment
                    );

                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->VertexBuffer.VertexData.Size,
                        pxModelCreateInfo->VertexBuffer.VertexData.Data,
                        pxVertexBuffer->VertexData.Data
                    );
                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment,
                        pxModelCreateInfo->IndexBuffer.DataIndexPosition,
                        pxModel->Mesh.IndexBuffer.DataIndexPosition
                    );
                }
                */

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: Custom"
                );
#endif

                break;
            }
            case PXModelFormTriangle:
            {
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;

                PXBufferSet(&pxVertexBuffer->VertexData, PXVertexDataTriangle, sizeof(PXVertexDataTriangle));

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;

                PXBufferSet(&pxIndexBuffer->Data, PXIndexDataTriangle, sizeof(PXIndexDataTriangle));

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;


#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: Triangle"
                );
#endif

                break;
            }
            case PXModelFormRectangle:
            {
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

                PXBufferSet(&pxVertexBuffer->VertexData, PXVertexDataRectangle, sizeof(PXVertexDataRectangle));

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

                PXBufferSet(&pxIndexBuffer->Data, PXIndexDataRectangle, sizeof(PXIndexDataRectangle));

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: Rectangle"
                );
#endif

                break;
            }
            case PXModelFormRectangleTX:
            {
                pxVertexBuffer->LayoutAmount = 2;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                pxVertexBuffer->LayoutPrime[1].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[1].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[1].Type = PXVertexBufferLayoutTypeTexturePos;

                PXBufferSet(&pxVertexBuffer->VertexData, PXVertexDataRectangleTX, sizeof(PXVertexDataRectangleTX));

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

                PXBufferSet(&pxIndexBuffer->Data, PXIndexDataRectangle, sizeof(PXIndexDataRectangle));

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: RectangleTX"
                );
#endif

                break;
            }
            case PXModelFormCircle:
            {
                PXF32 cx = 0;
                PXF32 cy = 0;
                PXF32 radius = 1;
                int segmentAmount = 16;

                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeInt32S;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

                PXBufferAllocate(&pxVertexBuffer->VertexData, sizeof(PXF32) * segmentAmount * 2);

                PXF32* vertexData = (PXF32*)pxVertexBuffer->VertexData.Data;

                for(PXSize i = 0; i < segmentAmount; ++i)
                {
                    const PXF32 theta = 2.0f * 3.14f * i / (PXF32)segmentAmount;//get the current angle
                    const PXF32 x = radius * PXMathCosinusRADF32(theta);//calculate the x component
                    const PXF32 y = radius * PXMathSinusRADF32(theta);//calculate the y component

                    vertexData[i++] = x + cx;
                    vertexData[i++] = x + cy; //output vertex
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: Circle"
                );
#endif

                break;
            }
            case PXModelFormCube:
            {
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 3;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32; // PXTypeInt08S
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

                PXBufferSet(&pxVertexBuffer->VertexData, PXVertexDataCube, sizeof(PXVertexDataCube));

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

                PXBufferSet(&pxIndexBuffer->Data, PXIndexDataCube, sizeof(PXIndexDataCube));

                pxIndexBuffer->LayoutListAmount = 1;
                pxIndexBuffer->LayoutPrime.AmountOfElements = 1;
                //pxIndexBuffer->LayoutPrime. = 1;
                pxIndexBuffer->LayoutPrime.AmountOfElements = 1;

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;




#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXMeshText,
                    "Create",
                    "From: Cube"
                );
#endif

                /*

                PXF32* input = 0;

                PXF32* output = 0;
                PXSize outINdex = 0;

                // QUAD to TRIANGLE
                for(size_t i = 0; i < indexLength; i+=4)
                {
                    output[outINdex++] = input[i + 0];
                    output[outINdex++] = input[i + 1];
                    output[outINdex++] = input[i + 2];
                    output[outINdex++] = input[i + 2];
                    output[outINdex++] = input[i + 3];
                    output[outINdex++] = input[i + 1];
                }
                */

                break;
            }
            default:
                break;
        }


        // if we dont have an index array, create one
        // TODO: ???
    }

    // Setup
    // PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);


    // After loading, we need to transmute it 
    // into a format we can actually use
    //PXMeshVertexLayoutTransmute(&pxModel->Mesh);


    //pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

    //pxModel->Info.Behaviour |= PXECSInfoRender;

    return PXResultOK;
}

PXResult PXAPI PXMeshVertexLayoutPrint(PXMesh PXREF pxMesh)
{
    PXVertexBuffer* pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    char bufferLayout[64];

    for(size_t i = 0; i < pxMesh->VertexBufferListAmount; i++)
    {
        PXVertexBuffer* pxVertexBuffer = &pxVertexBufferList[i];

        PXBufferLayout PXREF pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXVertexBufferText,
            "Layout",
            "PXID:%i, GLID_VBO:%i, (%i/%i), Amount:%i",
            pxVertexBuffer->Info.ID,
            pxVertexBuffer->VBO,
            i + 1,
            pxMesh->VertexBufferListAmount,
            pxVertexBuffer->LayoutAmount
        );
#endif

        for(size_t w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXVertexBufferText,
                "Layout",
                "- (%i/%i) Amount:%i, TypeSize:%i, Type:%i",
                w + 1,
                pxVertexBuffer->LayoutAmount,
                amountOfElement,
                sizeOfSingleElement,
                pxVertexBufferLayout->Type
            );
#endif
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXMeshVertexLayout(PXMesh PXREF pxMesh, const PXSize index, PXBufferLayout PXREF pxVertexBufferLayoutList, const PXSize amount)
{
    PXVertexBuffer* pxVertexBufferList = PXNull;

    pxMesh->VertexBufferListAmount = amount;

    //-----------------------------------------------------
    // Calc total use
    PXSize totalVertexDataSize = 0;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXBufferLayout PXREF pxVertexBufferLayout = &pxVertexBufferLayoutList[i];

        PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
        PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

        totalVertexDataSize += sizeOfSingleElement * amountOfElement;
    }
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Alloc, VBO
    pxVertexBufferList = PXMeshVertexBufferListSET(pxMesh, amount);

    //-----------------------------------------------------

    // Distructbute

    PXVertexBuffer* pxVertexBuffer = &pxVertexBufferList[index];

    pxVertexBuffer->LayoutAmount = amount;
    PXCopyList(PXBufferLayout, amount, pxVertexBufferLayoutList, pxVertexBuffer->LayoutPrime);

    PXBufferAllocate(&pxVertexBuffer->VertexData, totalVertexDataSize);

    // How many vertex arrays?
}

PXResult PXAPI PXMeshIndexLayout(PXMesh PXREF pxMesh, const PXSize primitveAmount, const PXSize segmentAmount)
{
    PXIndexBufferPrepare(&pxMesh->IndexBuffer, primitveAmount, segmentAmount);

    pxMesh->IndexBuffer->DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;

    pxMesh->MaterialContaierListAmount = segmentAmount;
    pxMesh->MaterialContaierList = PXMemoryHeapCallocT(PXMaterialContainer, segmentAmount);

    return PXResultOK;
}

PXBufferLayout* PXAPI PXMeshVertexBufferGET(PXMesh PXREF pxMesh, const PXI8U type)
{
    PXVertexBuffer PXREF pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer PXREF pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout PXREF pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            // if layout matches..
            if(pxVertexBufferLayout->Type == type)
            {
                return pxVertexBufferLayout;
            }
        }
    }

    return PXNull;
}

void* PXAPI PXMeshVertexInsert(PXMesh PXREF pxMesh, const PXI8U type)
{
    PXVertexBuffer PXREF pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer PXREF pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout PXREF pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize offset = 0;

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

            offset += sizeOfSingleElement * amountOfElement;

            // if layout matches..
            if(pxVertexBufferLayout->Type == type)
            {
                void* adress = (PXByte*)pxVertexBuffer->VertexData.Data + offset;

                PXAssert
                (
                    offset <=
                    pxVertexBuffer->VertexData.SizeAllocated,
                    "Out of bounce"
                );

                return adress;
            }
        }
    }

    return PXNull;
}

void* PXAPI PXMeshIndexInsert(PXMesh PXREF pxMesh, const PXI8U type)
{
    PXByte* data = (PXByte*)pxMesh->IndexBuffer->Data.Data;
    const PXSize sizeOfElement = PXTypeSizeGet(pxMesh->IndexBuffer->DataType);

    PXBufferLayout* pxBufferLayoutList = PXIndexLayoutListGET(&pxMesh->IndexBuffer);

    for(PXSize i = 0; i < pxMesh->IndexBuffer->LayoutListAmount; ++i)
    {
        PXBufferLayout* pxBufferLayout = &pxBufferLayoutList[i];

        if(type == pxBufferLayout->Type)
        {
            return data + sizeOfElement * i;
        }
    }

    return PXNull;
}

PXSize PXAPI PXMeshVertexStrideGET(PXMesh PXREF pxMesh)
{
    return 0;
}

PXVertexBuffer* PXAPI PXMeshVertexBufferListGET(PXMesh PXREF pxMesh)
{
    if(pxMesh->VertexBufferListAmount <= 4)
    {
        return pxMesh->VertexBufferPrime;
    }
    else
    {
        return pxMesh->VertexBufferList;
    }
}

PXVertexBuffer* PXAPI PXMeshVertexBufferListSET(PXMesh PXREF pxMesh, const PXSize amount)
{
    if(amount <= 4)
    {
        return pxMesh->VertexBufferPrime;
    }
    else
    {
        pxMesh->VertexBufferList = PXMemoryHeapCallocT(PXVertexBuffer, pxMesh->VertexBufferListAmount);

        return pxMesh->VertexBufferList;
    }
}

PXSize PXAPI PXMeshIndexBufferLengthGET(PXMesh PXREF pxMesh)
{
    PXBufferLayout PXREF pxBufferLayoutList = PXIndexLayoutListGET(&pxMesh->IndexBuffer);

    PXI8U typeSize = 0;

    for(size_t i = 0; i < pxMesh->VertexBufferListAmount; i++)
    {
        PXBufferLayout PXREF pxBufferLayout = &pxBufferLayoutList[i];

        typeSize = PXTypeSizeGet(pxBufferLayout->Type);
    }

    if(typeSize == 0)
    {
        typeSize = 1;
    }

    PXSize res = (pxMesh->IndexBuffer->Data.SizeAllocated / typeSize) / pxMesh->VertexBufferListAmount;

    return res;
}

PXResult PXAPI PXMeshVertexLayoutTransmute(PXMesh PXREF pxMesh)
{
#if 0

    // If we have POS; NORM; TEX. Convert it into an interleaved array
    // As the GPU is to stupit to use seperate arrays.
    PXIndexBuffer* pxIndexBuffer = &pxMesh->IndexBuffer;

    PXVertexBuffer PXREF pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);
    PXBufferLayout PXREF pxIndexBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);


    const PXSize layoutListAmount = pxIndexBuffer->LayoutListAmount;
    const PXSize vertexBufferAmount = pxMesh->VertexBufferListAmount;

    const PXBool hasMultiIndexArray = layoutListAmount > 1;
    const PXSize indexAmount = PXMeshIndexBufferLengthGET(pxIndexBuffer);


    // If the format is not already compatible, we need to translate it.
    // Allocate new vertex memory for this
    const PXSize stride = PXMeshVertexStrideGET(pxMesh);
    const PXSize fullData = sizeof(float) * stride * indexAmount;

    float* memBlock = PXMemoryVirtualAllocate(fullData, PXNull, PXAccessModeReadAndWrite);


    PXSize offset = 0;
    float* vertexDataNew[8];

    for(size_t i = 0; i < vertexBufferAmount; i++)
    {
        PXVertexBuffer PXREF pxVertexBuffer = &pxVertexBufferList[i];


        // Width of the whole buffer
        PXBufferLayout PXREF pxBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize width = 0;

        for(size_t i = 0; i < pxVertexBuffer->LayoutAmount; i++)
        {
            PXBufferLayout PXREF pxBufferLayout = &pxBufferLayoutList[i];

            width += PXTypeSizeGet(pxBufferLayout->Format);
        }

        vertexDataNew[i] = &((float*)memBlock)[offset];

        offset += indexAmount * width;
    }


    if(hasMultiIndexArray)
    {
        // Loop over all the arrays 
        for(PXSize x = 0; x < layoutListAmount; ++x)
        {
            PXBufferLayout PXREF pxBufferLayout = &pxIndexBufferLayoutList[x];

            // Lookup reference vertex array
            PXBufferLayout* vertexBufferLayout = PXMeshVertexBufferGET(pxMesh, pxBufferLayout->Type);

            void* source = PXMeshVertexInsert(pxMesh, pxBufferLayout->Type);
            float* target = vertexDataNew[x];


            const PXI8U vertexTypeSize = PXTypeSizeGet(vertexBufferLayout->Type);
            const PXI8U indexTypeSize = PXTypeSizeGet(pxBufferLayout->Type);


            void* indexData = pxIndexBuffer->Data.Data;



            for(size_t y = 0; y < indexAmount; ++y)
            {
                PXSize refIndex = x + y * layoutListAmount;
                PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                void* vertexDataANY = (PXByte*)source * vertexTypeSize * index;
                float* vertexDataF = &target[index];

                PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
            }

            switch(indexTypeSize)
            {
                case 1:
                {

                    break;
                }
                case 2:
                {
                    for(size_t y = 0; y < indexAmount; ++y)
                    {
                        PXSize refIndex = x + y * layoutListAmount;
                        PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                        void* vertexDataANY = index * (PXByte*)source * vertexTypeSize;
                        float* vertexDataF = &target[index];

                        PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
                    }

                    break;
                }
                case 4:
                {
                    for(size_t y = 0; y < indexAmount; ++y)
                    {
                        PXSize refIndex = x + y * layoutListAmount;
                        PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                        void* vertexDataANY = index * (PXByte*)source * vertexTypeSize;
                        float* vertexDataF = &target[index];

                        PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    else
    {
        // We prob. only need to promote vertex data
        // As the index data is only one type, there is no need to do anything

        for(PXSize i = 0; i < vertexBufferAmount; i++)
        {
            PXVertexBuffer PXREF pxVertexBuffer = &pxVertexBufferList[i];


        }



    }






#endif



#if 0


    PXSize vertexDataCacheOffset = 0;

    PXSize globalOffset = 0;
    PXSize indexOffset = 0;

    PXSize typeSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    PXF32 cacheLine[3 + 3 + 2];


    float* inputPosition = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
    float* inputNormal = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
    float* inputTexture = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

    void* inputIndexPosition = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
    void* inputIndexNormal = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
    void* inputIndexTexture = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

    // Problem: Opengl does not work with multible index data 
    // things, so we need to change them
    const PXBool hasMultibleIndexData = pxMesh->IndexBuffer.LayoutListAmount > 1;




    const PXSize pxIndexSegmentListAmount = pxIndexBuffer->SegmentListAmount;
    PXIndexSegment* pxIndexSegmentList = PXIndexBufferSegmentListGET(pxIndexBuffer);

    for(PXSize segmentIndex = 0; segmentIndex < pxIndexSegmentListAmount; ++segmentIndex)
    {
        PXIndexSegment PXREF pxIndexSegment = &pxIndexSegmentList[segmentIndex];

        const PXSize range = pxIndexSegment->DataRange;

        for(PXSize i = 0; i < range; ++i)
        {
            PXI16U dataIndexVertex = *(PXI16U*)((PXByte*)inputIndexPosition + (typeSize * i));
            PXI16U dataIndexNormal = *(PXI16U*)((PXByte*)inputIndexNormal + (typeSize * i));
            PXI16U dataIndexTexture = *(PXI16U*)((PXByte*)inputIndexTexture + (typeSize * i));

            float* inVertex = &inputPosition[dataIndexVertex];
            float* inNormal = &inputNormal[dataIndexNormal];
            float* inTexture = &inputTexture[dataIndexTexture];

            float* outVertex = &cacheLine[0];
            float* outNormal = &cacheLine[3];
            float* outTexture = &cacheLine[3 + 3];

            vertexDataCacheOffset += PXMemoryCopy(inVertex, sizeof(float) * 3, outVertex, 100);
            vertexDataCacheOffset += PXMemoryCopy(inNormal, sizeof(float) * 3, outNormal, 100);
            vertexDataCacheOffset += PXMemoryCopy(inTexture, sizeof(float) * 2, outTexture, 100);

            pxOpenGL->Binding.BufferDataSub
            (
                GL_ARRAY_BUFFER,
                globalOffset,
                (GLsizeiptr)vertexDataCacheOffset,
                cacheLine
            );

            PXOpenGLErrorCurrent(pxOpenGL, 1);

            globalOffset += vertexDataCacheOffset;
            vertexDataCacheOffset = 0;
            ++indexOffset;

            // Mark as usable!                      
        }

#if PXLogEnable
        int progress = -1;// (globalOffset / (float)vertexDataInterleavedSize) * 100;

        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            PXOpenGLModelName,
            "Segment:<%2i/%-2i> ready! %7i/%7i (%3i)",
            segmentIndex + 1,
            pxIndexBuffer->SegmentListAmount,
            globalOffset,
            -1,
            progress
        );
#endif
    }
    //#endif


























    PXMeshVertexLayoutPrint(pxMesh);

    PXVertexBuffer PXREF pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer PXREF pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout PXREF pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize offset = 0;

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);

            if(sizeOfSingleElement == 1)
            {
                // We are using chars, we promite
            }

            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

            offset += sizeOfSingleElement * amountOfElement;


        }
    }



    //#if 0
        //-----------------------------------------------------
        // Vertex
        //-----------------------------------------------------

    PXMesh* pxMesh = &pxModel->Mesh;
    const PXVertexBufferFormat oldFormat = pxMesh->VertexBufferPrime.Format;


    if(1 == pxMesh->VertexBufferListAmount)
    {
        PXVertexBuffer PXREF pxVertexBuffer = &pxMesh->VertexBufferPrime;

        PXVertexBufferFormatInfo pxVertexBufferFormatInfoOLD;
        PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoOLD, pxVertexBuffer->Format);

        PXVertexBufferFormat formatNEW = PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 0, 0);
        PXVertexBufferFormatInfo pxVertexBufferFormatInfoNEW;
        PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoNEW, formatNEW);


        const PXBool isP2I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 2;
        const PXBool isP3I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 3;
        const PXBool isT2P2I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 2 && pxVertexBufferFormatInfoOLD.TexturePosition == 2;

        PXSize amountCurrent = pxVertexBufferFormatInfoOLD.Stride;
        PXSize amountFuture = pxVertexBufferFormatInfoNEW.Stride;

        if(isT2P2I8)
        {
            PXSize sizeBefore = pxVertexBuffer->VertexDataSize;
            PXSize sizeCurrent = (pxVertexBuffer->VertexDataSize / 4) * amountFuture;

            // Store old data
            PXI8S* dataOld = (PXI8S*)pxVertexBuffer->VertexData;

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, sizeCurrent);;
            pxVertexBuffer->VertexDataSize = sizeof(PXF32) * sizeCurrent;

            PXF32* dataNew = (PXF32*)pxVertexBuffer->VertexData;

            PXSize newOffset = 0;

            for(PXSize i = 0; i < sizeBefore; i += 4)
            {
                dataNew[newOffset++] = (PXF32)dataOld[i + 0];
                dataNew[newOffset++] = (PXF32)dataOld[i + 1];
                dataNew[newOffset++] = 0.0f;
                dataNew[newOffset++] = (PXF32)dataOld[i + 2];
                dataNew[newOffset++] = (PXF32)dataOld[i + 3];
            }

            //PXConsoleWriteTablePXF32(dataNew, sizeCurrent, 5);
        }
        else if(isP2I8)
        {
            PXSize sizeBefore = pxVertexBuffer->VertexDataSize;
            PXSize sizeCurrent = (pxVertexBuffer->VertexDataSize / 2) * amountFuture;

            // Store old data
            PXI8S* dataOld = (PXI8S*)pxVertexBuffer->VertexData;

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, sizeCurrent);;
            pxVertexBuffer->VertexDataSize = sizeof(PXF32) * sizeCurrent;

            PXF32* dataNew = (PXF32*)pxVertexBuffer->VertexData;

            PXSize newOffset = 0;

            const char texCoords[] =
            {
                0, 1,   // Top-left
                1, 1,   // Top-right
                1, 0,   // Bottom-right
                0, 0    // Bottom-left
            };

            for(PXSize i = 0; i < sizeBefore; i += 2)
            {
                dataNew[newOffset++] = (PXF32)dataOld[i + 0];
                dataNew[newOffset++] = (PXF32)dataOld[i + 1];
                dataNew[newOffset++] = 0.0f;

                dataNew[newOffset++] = (PXF32)texCoords[(i + 0) % 8];
                dataNew[newOffset++] = (PXF32)texCoords[(i + 1) % 8];

                //dataNew[newOffset++] = (dataOld[i + 0] + 1) / 2.0f;
                //dataNew[newOffset++] = -(dataOld[i + 1] + 1) / 2.0f;
            }

            //PXConsoleWriteTablePXF32(dataNew, sizeCurrent, 5);

            // Memory leak? dataOld needs to be deleted? But what if its read only?         
        }
        else if(isP3I8)
        {
            formatNEW = PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 0, 0, 0);
            PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoNEW, formatNEW);
            amountFuture = pxVertexBufferFormatInfoNEW.Stride;


            PXSize sizeCurrent = pxVertexBuffer->VertexDataSize / 1;

            const PXSize newVertexArraySize = sizeof(PXF32) * sizeCurrent;
            PXF32* newVertexArray = PXMemoryHeapCallocT(PXF32, sizeCurrent);

            PXI8S* dataSource = (PXI8S*)pxVertexBuffer->VertexData;

            for(size_t i = 0; i < sizeCurrent; i++)
            {
                newVertexArray[i] = dataSource[i];
            }

            // Memory leak?

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = newVertexArray;
            pxVertexBuffer->VertexDataSize = newVertexArraySize;
        }

        PXVertexBufferFormat newFormat = pxVertexBuffer->Format;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Model-Format",
            "Transmute <%s> to <%s>",
            pxVertexBufferFormatInfoOLD.AsText,
            pxVertexBufferFormatInfoNEW.AsText
        );
#endif

    }

#endif
}

PXResult PXAPI PXMeshNormalDataGenerate(PXMesh PXREF pxMesh)
{
    // ...

    return PXResultOK;
}

PXResult PXAPI PXMeshVertexArrayAdd
(
    PXMesh PXREF pxMesh,
    void* data,
    const PXSize dataLength,
    PXBufferLayout PXREF pxVertexBufferLayoutList,
    const PXSize pxVertexBufferLayoutListAmount
)
{
    PXVertexBuffer* pxVertexBufferTarget = PXNull;

    // Do we need to allocate? Buffer
    const PXSize expectedIndex = pxMesh->VertexBufferListAmount;
    const PXBool createNew = PXEmbeddedArraySize < (expectedIndex + 1);

    if(createNew)
    {
        // Do then
        pxVertexBufferTarget = &pxMesh->VertexBufferList[expectedIndex];
    }
    else
    {
        pxVertexBufferTarget = &pxMesh->VertexBufferPrime[expectedIndex];
    }

    PXAssert(pxVertexBufferTarget, "Can't be NULL");

    pxVertexBufferTarget->LayoutAmount = pxVertexBufferLayoutListAmount;
    PXCopyList(PXBufferLayout, pxVertexBufferLayoutListAmount, pxVertexBufferLayoutList, pxVertexBufferTarget->LayoutPrime);

    // COPY??
    PXBufferSet(&pxVertexBufferTarget->VertexData, data, dataLength);


    //---------------------
   // Offset buffer
   //---------------------

   // GetGraphicsMode();
#if 0

   // Request API

    PXGraphic PXREF pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL PXREF opengl = &pxGraphic->OpenGLInstance;

    PXVertexBuffer indexedVertexBuffer;

    PXSize attributeIndex = 0;

    // Create
    PXOpenGLBufferGenerate(opengl, 1, &indexedVertexBuffer.Info.Handle.OpenGLID);

    // Bind
    PXOpenGLBufferBind(opengl, PXOpenGLBufferArray, indexedVertexBuffer.Info.Handle.OpenGLID);

    // Upload
    PXOpenGLBufferData(opengl, PXOpenGLBufferArray, dataLength, data, PXOpenGLStoreStaticDraw);

    // Define
    PXOpenGLVertexArrayAttributeDefine(opengl, attributeIndex, 2, PXTypeF32, PXFalse, dataLength, (void*)0);
    PXOpenGLVertexArrayEnable(opengl, attributeIndex);

#endif



    // Attach to model...

    return PXResultOK;
}

PXSize PXAPI PXMeshTriangleAmount(PXMesh PXREF pxMesh)
{
    /*
    const PXI8U stride = PXVertexBufferFormatStrideSize(pxMesh->VertexBuffer.Format);
    PXSize amount = (pxMesh->VertexBuffer.VertexDataSize / stride) / 3;

    return amount;
    */

    return 0;// (pxMesh->IndexBuffer.DataIndexSizeTotal[0];
}

PXF32* PXAPI PXMeshTriangleIndex(PXMesh PXREF pxMesh, const PXSize index)
{
    //PXVertexBufferInsertionPoint();

    return 0;// PXPublic PXF32* PXAPI();
}













PXBufferLayout* PXAPI PXVertexBufferLayoutGET(PXVertexBuffer PXREF pxVertexBuffer)
{
    if(PXEmbeddedArraySize >= pxVertexBuffer->LayoutAmount)
    {
        return pxVertexBuffer->LayoutPrime;
    }
    else
    {
        return pxVertexBuffer->LayoutList;
    }
}

/*
void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer PXREF pxVertexBuffer, const PXVertexBufferFormat pxVertexBufferFormat, const PXSize index)
{
   // PXVertexBufferFormatInfo pxVertexBufferFormatInfo;
   // PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfo, pxVertexBuffer->Format);

    const PXI8U stride = 0;// pxVertexBufferFormatInfo.Stride;
    const PXSize rowEntiry = stride * index;

    return 0;

#if 0
    switch(pxVertexBuffer->Format)
    {
        case PXVertexBufferFormatT2F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                case PXVertexBufferDataTypeTexture:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatT2F_N3F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2 + 3];

                case PXVertexBufferDataTypeTexture:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                case PXVertexBufferDataTypeNormal:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatXYZPXF32:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatN3F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 3];

                case PXVertexBufferDataTypeNormal:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }

        default:
            return PXNull;
    }
#endif
}
*/

void PXAPI PXIndexBufferPrepare(PXIndexBuffer PXREF pxIndexBuffer, const PXSize amountVertex, const PXSize amountMaterials)
{
    // Index array data Type    
    if(amountVertex <= 0xFFu)
    {
        // 8-Bit
        pxIndexBuffer->DataType = PXTypeInt08U;
    }
    else if(amountVertex <= 0xFFFFu)
    {
        // 16-Bit
        pxIndexBuffer->DataType = PXTypeInt16U;
    }
    else if(amountVertex <= 0xFFFFFFFFu)
    {
        // 32-Bit
        pxIndexBuffer->DataType = PXTypeInt32U;
    }
    else
    {
        // 64-Bit
        pxIndexBuffer->DataType = PXTypeInt64U;
    }

    const PXSize dataSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    PXBufferAllocate(&pxIndexBuffer->Data, amountVertex * 3 * dataSize);

    PXSize indexAmount = 3;

    pxIndexBuffer->LayoutListAmount = 3;
    pxIndexBuffer->LayoutList = PXMemoryHeapCallocT(PXBufferLayout, pxIndexBuffer->LayoutListAmount);

    pxIndexBuffer->LayoutList[0].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[0].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[0].Type = PXVertexBufferLayoutTypePosition;

    pxIndexBuffer->LayoutList[1].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[1].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[1].Type = PXVertexBufferLayoutTypeNormal;

    pxIndexBuffer->LayoutList[2].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[2].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[2].Type = PXVertexBufferLayoutTypeTexturePos;


    pxIndexBuffer->SegmentListAmount = amountMaterials;
    pxIndexBuffer->SegmentList = PXMemoryHeapCallocT(PXIndexSegment, amountMaterials);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIndexBufferText,
        "prepare",
        "PXID:%i, TypeSize:%i, IndexAmount:%i, VertexAmount:%i, Materials:%i",
        pxIndexBuffer->IBO,
        dataSize,
        indexAmount,
        amountVertex,
        amountMaterials
    );
#endif
}

PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer PXREF pxIndexBuffer)
{
    return pxIndexBuffer->IBO != 0;
}

PXIndexSegment* PXAPI PXIndexBufferSegmentListGET(PXIndexBuffer PXREF pxIndexBuffer)
{
    PXAssert(pxIndexBuffer->SegmentListAmount != 0, "This cant be 0");

    if(pxIndexBuffer->SegmentListAmount == 1)
    {
        return &pxIndexBuffer->SegmentPrime;
    }
    else
    {
        return pxIndexBuffer->SegmentList;
    }
}

PXBufferLayout* PXAPI PXIndexLayoutListGET(PXIndexBuffer PXREF pxIndexBuffer)
{
    PXBufferLayout* pxBufferLayout = PXNull;

    PXAssert(pxIndexBuffer->LayoutListAmount != 0, "This cant be 0");

    if(pxIndexBuffer->LayoutListAmount == 1)
    {
        pxBufferLayout = &pxIndexBuffer->LayoutPrime;
    }
    else
    {
        pxBufferLayout = pxIndexBuffer->LayoutList;
    }

    return pxBufferLayout;
}

PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer PXREF pxIndexBuffer, const PXI8U type)
{
#if 1



#else


    PXBufferLayout PXREF pxBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);

    for(size_t i = 0; i < pxIndexBuffer->LayoutListAmount; i++)
    {
        PXBufferLayout PXREF pxBufferLayout = &pxBufferLayoutList[i];

        if(type != pxBufferLayout->Type)
        {
            continue;
        }

        pxBufferLayout->
    }

    return 0;
#endif
}
