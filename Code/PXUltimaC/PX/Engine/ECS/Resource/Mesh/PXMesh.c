#include "PXMesh.h"
#include <PX/Engine/PXResource.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

#include <gl/GL.h>

#include "PXIndexBuffer.h"
#include <PX/Math/PXMath.h>

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

    pxMeshCreateInfo->Info.Static = &PXMeshRegisterInfoStatic;
    pxMeshCreateInfo->Info.Dynamic = &PXMeshRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxMeshREF, pxMeshCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxMesh = *pxMeshREF;


    pxMesh->Positions = PXMemoryHeapCallocT(PXVector3F32, pxMeshCreateInfo->VertexAmount);
    pxMesh->Normals = PXMemoryHeapCallocT(PXVector3F32, pxMeshCreateInfo->VertexAmount);
    pxMesh->UVs = PXMemoryHeapCallocT(PXVector2F32, pxMeshCreateInfo->VertexAmount);

    pxMeshCreateInfo->Positions = pxMesh->Positions;
    pxMeshCreateInfo->Normals = pxMesh->Normals;
    pxMeshCreateInfo->UVs = pxMesh->UVs;


    const PXBool hasFilePath = pxMeshCreateInfo->Info.FilePath.Data > 0;

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
            const PXResult loadResult = PXECSLoad(pxMesh, &pxMeshCreateInfo->Info);
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
       // pxMesh->VertexBufferListAmount = 1;
       // PXVertexBuffer** pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);
       // PXIndexBuffer PXREF pxIndexBuffer = pxMesh->IndexBuffer;

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
                //pxVertexBuffer->LayoutAmount = 1;
                //pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                //pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;

                //PXBufferSet(&pxVertexBufferList->VertexData, PXVertexDataTriangle, sizeof(PXVertexDataTriangle));

                //pxIndexBuffer->DataType = PXTypeI08U;
               // pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;

              //  PXBufferSet(&pxIndexBuffer->Data, PXIndexDataTriangle, sizeof(PXIndexDataTriangle));

                //pxIndexBuffer->SegmentListAmount = 1;
                //pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;


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
                //pxVertexBuffer->LayoutAmount = 1;
                //pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                //pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                //pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

                //PXBufferSet(&pxVertexBufferList->VertexData, PXVertexDataRectangle, sizeof(PXVertexDataRectangle));

                //pxIndexBuffer->DataType = PXTypeI08U;
               // pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

               // PXBufferSet(&pxIndexBuffer->Data, PXIndexDataRectangle, sizeof(PXIndexDataRectangle));

                //pxIndexBuffer->SegmentListAmount = 1;
                //pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;

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
                //pxVertexBuffer->LayoutAmount = 2;
                //pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                //pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                //pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                //pxVertexBuffer->LayoutPrime[1].AmountOfElements = 2;
                //pxVertexBuffer->LayoutPrime[1].Format = PXTypeF32;
                //pxVertexBuffer->LayoutPrime[1].Type = PXVertexBufferLayoutTypeTexturePos;

              //  PXBufferSet(&pxVertexBufferList->VertexData, PXVertexDataRectangleTX, sizeof(PXVertexDataRectangleTX));

               // pxIndexBuffer->DataType = PXTypeI08U;
               // pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

               // PXBufferSet(&pxIndexBuffer->Data, PXIndexDataRectangle, sizeof(PXIndexDataRectangle));

                //pxIndexBuffer->SegmentListAmount = 1;
                //pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;

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

                //pxVertexBuffer->LayoutAmount = 1;
                //pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                //pxVertexBuffer->LayoutPrime[0].Format = PXTypeI32S;
                //pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

              //  PXBufferAllocate(&pxVertexBufferList->VertexData, sizeof(PXF32) * segmentAmount * 2);

                PXF32* vertexData = 0;// (PXF32*)pxVertexBufferList->VertexData.Data;

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
                //pxVertexBuffer->LayoutAmount = 1;
                //pxVertexBuffer->LayoutPrime[0].AmountOfElements = 3;
                //pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32; // PXTypeI08S
                //pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;

              //  PXBufferSet(&pxVertexBufferList->VertexData, PXVertexDataCube, sizeof(PXVertexDataCube));

               // pxIndexBuffer->DataType = PXTypeI08U;
               // pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;

               // PXBufferSet(&pxIndexBuffer->Data, PXIndexDataCube, sizeof(PXIndexDataCube));

                //pxIndexBuffer->LayoutListAmount = 1;
                //pxIndexBuffer->LayoutPrime.AmountOfElements = 1;
                //pxIndexBuffer->LayoutPrime. = 1;
                //pxIndexBuffer->LayoutPrime.AmountOfElements = 1;

                //pxIndexBuffer->SegmentListAmount = 1;
                //pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.SizeAllocated;




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
    //PXMeshVertexLayoutTransmute(pxMesh);


    //pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

    //pxModel->Info.Behaviour |= PXECSInfoRender;

    return PXResultOK;
}

PXResult PXAPI PXMeshRelease(PXMesh PXREF pxMesh)
{
    if(!pxMesh)
    {
        return PXResultRefusedParameterNull;
    }

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMeshDraw(PXMesh PXREF pxMesh, PXDrawInfo PXREF pxDrawInfo)
{
    if(!(pxMesh && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXMeshGeometry PXREF pxMeshGeometry = pxMesh->Geometry;


#if 1
    GLenum modfs[3] = { GL_FILL, GL_LINE , GL_POINT };
    GLenum wwwdee[3] = { GL_TRIANGLE_STRIP, GL_LINES , GL_POINTS };
    PXVector4F32 colorList[3] =
    {
        {0.3f, 0.7f, 1.0f, 0.3f},
        {0.2f, 0.2f, 0.2f, 1.0f},
        {1.0f, 0.2f, 0.0f, 1.0f},
    };

    PXTexture PXREF pxTexture = pxMesh->Texture;

    if(pxTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, pxTexture->OpenGLID);
    }
    else
    {

    }



    glFrontFace(GL_CW);

    glPointSize(5);
    glLineWidth(5);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float scale = 0.0005f;

    glPushMatrix();
    glTranslatef(pxMesh->Position.X* scale, pxMesh->Position.Y * scale, pxMesh->Position.Z * scale);

    glScalef(scale, scale, scale);

    // Enable client states
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, pxMesh->Positions);

    if(pxMesh->Normals)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, pxMesh->Normals);
    }  

    if(pxMesh->UVs)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, pxMesh->UVs);
    }    

    for(size_t i = 0; i < 3; i++)
    {
        PXVector4F32* color = &colorList[i];

        glColor4f(color->X, color->Y, color->Z, color->W);

        // Draw triangles
        glDrawElements(wwwdee[i], pxMesh->IndexAmount, GL_UNSIGNED_SHORT, pxMesh->Index);
    }

    // Disable client states
    glDisableClientState(GL_VERTEX_ARRAY);

    if(pxMesh->Normals)
        glDisableClientState(GL_NORMAL_ARRAY);

    if(pxMesh->UVs)
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);


    if(pxTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
#else





    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer
    (
        3,
        GL_FLOAT,
        0,
        pxMeshGeometry->VertexBufferPrime[0]->VertexData.Data
    );
    glScalef(3, 3, 3);
    glTranslatef(-20, 0, 0);

    float* ddat = pxMeshGeometry->VertexBufferPrime[0]->VertexData.Data;
    PXSize amountff = pxMeshGeometry->VertexBufferPrime[0]->VertexData.CursorOffsetByte / sizeof(float);

#if 0
    for(size_t i = 0; i < amountff; i += 3)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Model",
            "Color-Set",
            "%6.2f %6.2f %6.2f",
            ddat[i + 0],
            ddat[i + 1],
            ddat[i + 2]
        );
#endif 
    }
#endif


    if(1)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer
        (
            GL_FLOAT,
            0,
            pxMeshGeometry->VertexBufferPrime[1]->VertexData.Data
        );

    }

    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    PXSize amount = pxMeshGeometry->IndexBuffer->Segment.SegmentListAmount;
    PXIndexSegmentEntry* pxIndexSegmentEntryList = PXIndexSegmentGET(&pxMeshGeometry->IndexBuffer->Segment);

    PXSize offset = 0;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXIndexSegmentEntry* pxIndexSegmentEntry = &pxIndexSegmentEntryList[i];
        // PXMaterial* mat = pxIndexSegmentEntry->material;

         // Apply material properties
       //  GLfloat diffuse[4] = { mat->Kd[0], mat->Kd[1], mat->Kd[2], mat->d };
        // GLfloat ambient[4] = { mat->Ka[0], mat->Ka[1], mat->Ka[2], mat->d };
        // GLfloat specular[4] = { mat->Ks[0], mat->Ks[1], mat->Ks[2], mat->d };

       //  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat->Ns);

         // Bind texture if available
        if(0) {
            //glEnable(GL_TEXTURE_2D);
           // glBindTexture(GL_TEXTURE_2D, mat->texture_id);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }

        PXSize typeSize = PXTypeSizeGet(pxMeshGeometry->IndexBuffer->DataType);

        // Draw this segment

        //glTexCoordPointer(2, GL_FLOAT, sizeof(VertexPNT), &model->verts[0].tu);






        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);

        GLfloat lightPos[] = { 1.0f, 10.0f, 1.0f, 100.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        glColor3f(1.0, 1.0, 1.0);
        glDrawArrays
        (
            GL_TRIANGLES,
            offset,
            pxIndexSegmentEntry->DataRange
        );

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_NORMALIZE);


        glPointSize(8);
        glColor3f(0.8, 0.2, 0.2);
        glDrawArrays
        (
            GL_POINTS,
            offset,
            pxIndexSegmentEntry->DataRange
        );

        glLineWidth(1);
        glColor3f(0.6, 1.0, 0.6);
        glDrawArrays
        (
            GL_LINES,
            offset,
            pxIndexSegmentEntry->DataRange
        );





        /*
         glDrawElements
         (
             GL_POINTS,
             pxIndexSegmentEntry->DataRange,
             GL_UNSIGNED_SHORT,
             pxMesh->IndexBuffer->Data.Data + offset
         );*/

        offset += pxIndexSegmentEntry->DataRange * typeSize;
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMeshVertexLayoutTransmute(PXMesh PXREF pxMesh)
{
#if 0

    // If we have POS; NORM; TEX. Convert it into an interleaved array
    // As the GPU is to stupit to use seperate arrays.
    PXIndexBuffer* pxIndexBuffer = &pxMesh->IndexBuffer;

    PXVertexBuffer PXREF pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);
    PXBufferLayout PXREF pxIndexBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);


    const PXSize layoutListAmount = pxIndexBuffer->Layout.LayoutAmount;
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

        for(size_t i = 0; i < pxVertexBuffer->Layout.LayoutAmount; i++)
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
    PXBufferLayoutEntry PXREF pxVertexBufferLayoutList,
    const PXSize pxVertexBufferLayoutListAmount
)
{
    PXVertexBuffer* pxVertexBufferTarget = PXNull;

    // Do we need to allocate? Buffer
    const PXSize expectedIndex = pxMesh->Geometry->VertexBufferListAmount;
    const PXBool createNew = PXEmbeddedArraySize < (expectedIndex + 1);

    if(createNew)
    {
        // Do then
        pxVertexBufferTarget = &pxMesh->Geometry->VertexBufferList[expectedIndex];
    }
    else
    {
        pxVertexBufferTarget = &pxMesh->Geometry->VertexBufferPrime[expectedIndex];
    }

    PXAssert(pxVertexBufferTarget, "Can't be NULL");

    //pxVertexBufferTarget->LayoutAmount = pxVertexBufferLayoutListAmount;
    //PXCopyList(PXBufferLayoutEntry, pxVertexBufferLayoutListAmount, pxVertexBufferLayoutList, pxVertexBufferTarget->LayoutPrime);

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