#include "PXGraphic.h"

#if PXGraphicUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Window/PXWindow.h>
#include <Media/ADLER/PXAdler32.h>

#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>
#include <OS/Console/PXConsole.h>

//#include <gl/GL.h> // Not found???
#include <GL/gl.h>


#include <Math/PXMath.h>
#include <OS/Window/PXWindow.h>
#include <OS/Hardware/PXMonitor.h>
#include <stdarg.h>

PXActionResult PXAPI PXGraphicSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y)
{
    PXVector2FSetXY(&pxSprite->TextureScalePointOffset, x, y);
}

#define QuadSkybox 0

PXSize PXAPI PXGraphicRenderableListSize(const PXGraphic* const pxGraphic)
{
    return pxGraphic->_renderList.NodeListSizeCurrent;
}

PXBool PXAPI PXGraphicRenderableListGetFromIndex(const PXGraphic* const pxGraphic, PXRenderable** pxRenderable, const PXSize index)
{
#if 1 //
    PXLinkedListNodeFixed currentModel;

    const PXBool successful = PXLinkedListFixedNodeAt(&pxGraphic->_renderList, &currentModel, index);

    if (!successful)
    {
        return PXNo;
    }

    {
        PXRenderable* const renderableCurrent = (PXRenderable*)currentModel.BlockData;

        *pxRenderable = renderableCurrent;

        return PXYes;
    }

#else

    PXLinkedListNodeFixed currentModel;

    PXLinkedListFixedNodeAt(&pxGraphic->_renderList, &currentModel, 0);

    *pxRenderable = 0;

    PXSize counter = 0;

    do
    {
        const PXRenderable* const renderableCurrent = currentModel.BlockData;

        if (index == counter++)
        {
            *pxRenderable = renderableCurrent;
            return PXTrue;
        }

    }
    while (PXLinkedListFixedNodeNext(&pxGraphic->_renderList, &currentModel));

    return PXNo;
#endif
}

PXActionResult PXAPI PXGraphicRenderableCreate(PXGraphic* const pxGraphic, PXRenderable** const pxRenderable)
{
    PXRenderable* const renderable = PXNew(PXRenderable);

    if (!renderable)
    {
        return PXActionFailedMemoryAllocation;
    }

    PXMemoryClear(renderable, sizeof(PXRenderable));

    PXGraphicRenderableRegister(pxGraphic, *pxRenderable);

    *pxRenderable = renderable; // TODO: Is this right?

    return PXActionSuccessful;
}

PXBool PXAPI PXGraphicRenderableRegister(PXGraphic* const pxGraphic, PXRenderable* const pxRenderable)
{
    PXLockEngage(&pxGraphic->_resourceLock);
    PXLinkedListFixedNodeAdd(&pxGraphic->_renderList, pxRenderable);
    PXLockRelease(&pxGraphic->_resourceLock);

    return PXTrue;
}

void PXAPI PXGraphicModelShaderSet(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->ResourceID.OpenGLID;
    }
}

/*
PXActionResult PXGraphicModelLoad(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXText* const filePath)
{
    PXModel* model = 0;

    // Create model
    {
        const PXActionResult createResult = PXGraphicModelCreate(pxGraphic, &model);

        PXActionReturnOnError(createResult);
    }

    // create PXRenderable
    {
        PXLockEngage(&pxGraphic->_resourceLock);
        PXLinkedListFixedNodeAdd(&pxGraphic->_renderList, renderable);
        PXLockRelease(&pxGraphic->_resourceLock);

        /*

        if (renderable) // Owner is caller
        {
            pxRenderable = *renderable;


        }
        else // No model, make one
        {
            const PXActionResult createResult = PXGraphicRenderableCreate(pxGraphic, &pxRenderable);

            PXActionExitOnError(createResult);

            *renderable = pxRenderable; // Write to return back to caller
        }
        * /
    }

    // Load model
    {
        const PXActionResult loadResult = PXModelLoad(model, filePath);

        PXActionReturnOnError(loadResult);
    }

    // Register model into renderable
    {
        const PXActionResult loadResult = PXGraphicModelRegisterFromModel(pxGraphic, renderable, model);

        PXActionReturnOnError(loadResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicModelRegisterFromModel(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXModel* const model)
{
#if PXOpenGLUSE
    PXOpenGL* const openGLContext = &pxGraphic->OpenGLInstance;

    PXRenderableConstruct(renderable);

    PXOpenGLVertexArrayGenerate(openGLContext, 1u, &renderable->VAO);

    PXInt32U vbo = 0;

    // Create VBO Buffers
    PXOpenGLBufferGenerate(openGLContext, 1u, &vbo);
    renderable->VBO = vbo;



    PXOpenGLVertexArrayBind(openGLContext, renderable->VAO);

    PXOpenGLBufferBind(openGLContext, PXOpenGLBufferArray, renderable->VBO);
    PXOpenGLBufferData(openGLContext, PXOpenGLBufferArray, model->DataSize, model->Data, PXOpenGLStoreStaticDraw);

#if 0
    printf
    (
        "| %7s %7s %7s | %7s %7s %7s | %7s %7s |\n",
        "X",
        "Y",
        "Z",
        "Nx",
        "Ny",
        "Nz",
        "Tx",
        "Ty"
    );

    for (PXSize i = 0; i < model->DataIndexSize * stride; i+=8)
    {
        float* data = &(((float*)model->DataVertex)[i]);

        printf
        (
            "| %7.2f %7.2f %7.2f | %7.2f %7.2f %7.2f | %7.2f %7.2f |\n",
            data[0],
            data[1],
            data[2],

            data[3],
            data[4],
            data[5],

            data[6],
            data[7]
        );
    }
#endif

    // TODO
    unsigned int index = 0;
    unsigned int offset = 0;

    if (model->DataVertexWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataVertexWidth, PXDataTypeFloat, 0, model->DataVertexWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataVertexWidth * sizeof(float);
    }

    if (model->DataNormalWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataNormalWidth, PXDataTypeFloat, 0, model->DataNormalWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataNormalWidth * sizeof(float);
    }

    if (model->DataTextureWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataTextureWidth, PXDataTypeFloat, 0, model->DataTextureWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataTextureWidth * sizeof(float);
    }

    // Color?

    PXOpenGLBufferUnbind(openGLContext, PXOpenGLBufferArray);

    if (model->DataIndexWidth)
    {
        PXOpenGLBufferGenerate(openGLContext, 1u, &renderable->IBO);

        PXOpenGLBufferBind(openGLContext, PXOpenGLBufferElementArray, renderable->IBO);

        PXOpenGLBufferData(openGLContext, PXOpenGLBufferElementArray, sizeof(unsigned int) * model->DataIndexSize, model->DataIndexList, PXOpenGLStoreStaticDraw);

        PXOpenGLBufferUnbind(openGLContext, PXOpenGLBufferElementArray);
    }

    PXOpenGLVertexArrayUnbind(openGLContext);
    //-------------------------------------------------------------------------

    //---<Register all textures>-----------------------------------------------
    const PXSize segmentsListSize = 1;// PXModelSegmentsAmount(model);
    const PXSize modelListSize = 1;// PXModelMaterialAmount(model);

    renderable->MeshSegmentListSize = segmentsListSize;
    renderable->MeshSegmentList = PXMemoryAllocateType(PXRenderableMeshSegment, segmentsListSize);

    for (PXSize i = 0; i < segmentsListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];
        MeshSegment meshSegment;

        PXRenderableMeshSegmentConstruct(pxRenderableMeshSegment);
        PXModelSegmentsGet(model, i, &meshSegment);

        pxRenderableMeshSegment->RenderMode = PXGraphicDrawModeTriangle;
        pxRenderableMeshSegment->NumberOfVertices = meshSegment.DrawClusterSize;

        PXMaterial material;
        const PXBool fetchMaterialSuccess = PXModelMaterialGet(model, meshSegment.TextureID, &material);

        if (fetchMaterialSuccess)
        {
            PXTexture2D pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapRepeat;
            pxTexture.WrapWidth = PXGraphicImageWrapRepeat;

            //PXGraphicTextureLoad(pxGraphic, &pxTexture, &material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ResourceID.OpenGLID;
        }

        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;
#endif

    return PXActionSuccessful;
}*/


PXActionResult PXAPI PXGraphicUIRectangleCreate(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
{
    PXMatrix4x4FMoveXY(&renderable->MatrixModel, x, y);
   // PXMatrix4x4FScaleXYZSet(&renderable->MatrixModel, sidth, height, 1);

    // Register rectangle
    {
        //float

        //GraphicModelRegisterFromData(pxGraphic, renderable, , );

    }

    return PXActionSuccessful;
}

void PXAPI PXShaderConstruct(PXShader* const shader)
{
    PXMemoryClear(shader, sizeof(PXShader));

    shader->ResourceID.PXID = PXShaderNotRegisterd;
}

void PXAPI PXShaderDataSet(PXShader* const shader, const PXGraphicShaderType type, const char* data, const PXSize size)
{
    shader->ResourceID.PXID = PXShaderNotRegisterd;
    shader->Type = type;
    shader->ContentSize = size;
    shader->Content = data;
}

void PXAPI PXTextureConstruct(PXTexture2D* const texture)
{
    PXClear(PXTexture2D, texture);

    PXResourceIDMarkAsUnused(&texture->ResourceID);

    texture->Filter = PXGraphicRenderFilterNoFilter;
    texture->LayoutNear = PXGraphicImageLayoutNearest;
    texture->LayoutFar = PXGraphicImageLayoutNearest;
    texture->WrapHeight = PXGraphicImageWrapRepeat;
    texture->WrapWidth = PXGraphicImageWrapRepeat;
}

void PXAPI PXTextureDestruct(PXTexture2D* const texture)
{

}

PXActionResult PXAPI PXGraphicUIElementCreate(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement, const PXSize amount, PXUIElement* const pxUIElementParrent)
{
    //PXClear(PXUIElement, *pxUIElement);

    (*pxUIElement) = PXNew(PXUIElement);

    (*pxUIElement)->ID = ++(pxGraphic->UIElementIDCounter);   


    //*pxUIElement->Type = pxUIElementType;
    //*pxUIElement->TextureReference = PXNull;
    //*pxUIElement->ShaderReference = PXNull;

    //*pxUIElement->NameTextScale = 1;

    PXUIElementColorSet4F(*pxUIElement, 1, 1, 1, 1);

   // PXTextCopyA("[N/A]", 5, pxUIElement->Name, 32);

    if (pxUIElementParrent)
    {
        // Impossible if already has child

        (*pxUIElement)->Parent = pxUIElementParrent; // "It's alway my parent"

        const PXBool alreadyHasAFirstBorn = pxUIElementParrent->Child != PXNull;

        if (alreadyHasAFirstBorn)
        {
            // Add as sibling for firstborn
            PXUIElement* targetSibling = (*pxUIElement)->Parent->Child;

            // Search for last sibling to be
            while (targetSibling->Sibling != PXNull)
            {
                targetSibling = targetSibling->Sibling;
            }

            targetSibling->Sibling = (*pxUIElement); // Add sibling
        }
        else
        {
            // Add as child
            pxUIElementParrent->Child = (*pxUIElement);
        }    
    }
    else
    { 
        if (pxGraphic->UIElementBase.Child) // if Has child, everyone will be sibling from child
        {
            PXUIElement* insertionNode = pxGraphic->UIElementBase.Child;

            while (insertionNode->Sibling != PXNull)
            {
                insertionNode = insertionNode->Sibling;
            }

            insertionNode->Sibling = (*pxUIElement); // I am your sibbling
        }
        else
        {
            (*pxUIElement)->Parent = &pxGraphic->UIElementBase; // You are my parent now
            pxGraphic->UIElementBase.Child = (*pxUIElement); // As I am your child
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementDelete(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementTypeSet(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType)
{
    pxUIElement->Type = pxUIElementType;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementPrintfNode(PXUIElement* const pxUIElement, int depth, void* sender, PXGraphicUIElementTrigger preFound, PXGraphicUIElementTrigger postFound)
{
    PXFunctionInvoke(preFound, sender, pxUIElement);
#if 0
    const char* nothing = "---";
    const char* offset = "    ";

    for (size_t i = 0; i < depth; i++) printf(offset);
    printf("+--------------------+\n");

    for (size_t i = 0; i < depth; i++) printf(offset);

    printf("| Node: <%i> |\n", pxUIElement->ID);  

    for (size_t i = 0; i < depth; i++) printf(offset);

    if (pxUIElement->Parent)
    {
        printf("| %-10s : <%i>\n", "Parent", pxUIElement->Parent->ID);
    }
    else
    {
        printf("| %-10s : %s\n", "Parent", nothing);
    }  

    for (size_t i = 0; i < depth; i++) printf(offset);
#endif

    if (pxUIElement->Child)
    {
#if 0
        printf("| %-10s : <%i>\n", "Child", pxUIElement->Child->ID);
#endif
        PXGraphicUIElementPrintfNode(pxUIElement->Child, depth+1, sender, preFound, postFound);
    }
#if 0
    else
    {
        printf("| %-10s : %s\n", "Child", nothing);
    }

    for (size_t i = 0; i < depth; i++) printf(offset);
#endif

    if (pxUIElement->Sibling)
    {
#if 0
        printf("| %-10s : %s\n", "Siblings", "-->");
        for (size_t i = 0; i < depth; i++) printf(offset);
        printf("+--------------------+\n");
#endif

        PXGraphicUIElementPrintfNode(pxUIElement->Sibling, depth, sender, preFound, postFound);
    }
#if 0
    else
    {
        printf("| %-10s : %s\n", "Siblings", nothing);

       for (size_t i = 0; i < depth; i++) printf(offset);
        printf("+--------------------+\n");
    }
#endif

    PXFunctionInvoke(postFound, sender, pxUIElement);
}

PXActionResult PXAPI PXGraphicUIElementIterator(PXGraphic* const pxGraphic, void* sender, PXGraphicUIElementTrigger preFound, PXGraphicUIElementTrigger postFound)
{
    PXGraphicUIElementPrintfNode(&pxGraphic->UIElementBase, 0, sender, preFound, postFound);
}

PXActionResult PXAPI PXGraphicUIElementPrint(PXGraphic* const pxGraphic)
{
   // printf("+---------------------------------------------------------+\n");

    PXGraphicUIElementIterator(pxGraphic, 0, 0, 0);

   // printf("+---------------------------------------------------------+\n");
}

void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable)
{
    PXMemoryClear(pxRenderable, sizeof(PXRenderable));

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXAPI PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha)
{
    PXColorRGBAF* color = PXNew(PXColorRGBAF);

    color->Red = red;
    color->Green = green;
    color->Blue = blue;
    color->Alpha = alpha;

    pxUIElement->ColorTintReference = color;
}

void PXAPI PXUIElementSizeSet(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXInt32U  pxUIElementPositionMode)
{
    //pxUIElement->X = x;
    //pxUIElement->Y = y;
    // pxUIElement->Width = width;
   // pxUIElement->Height = height;

    pxUIElement->AncerFlagList = pxUIElementPositionMode;

    PXRectangleOffsetSet(&pxUIElement->Margin, x, y, width, height);

   // pxUIElement->PositionMode = pxUIElementPositionMode;
}

void PXAPI PXGraphicPXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText)
{
   // PXText nameBuffer;
   // PXTextConstructFromAdressA(&nameBuffer, pxUIElement->Name, 32);

   // PXTextCopy(pxText, &nameBuffer);
}

void PXAPI PXGraphicPXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text)
{
    PXTextCopyA(text, -1, pxUIElement->TextInfo.Content, 32);

    pxUIElement->TextInfo.Scale = 0.5;
}

void PXAPI PXGraphicPXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...)
{
    va_list args;
    va_start(args, format);

    PXTextPrintA(pxUIElement->TextInfo.Content, 32, format, args);

    va_end(args);
}

void PXAPI PXGraphicPXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont)
{
    //pxUIElement->FontID = pxFont;
}

void PXAPI PXGraphicUIElementFlagSet(PXUIElement* const pxUIElement, const PXInt32U flagList)
{
    pxUIElement->FlagsList = flagList;
}

void PXAPI PXGraphicPXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Parent = pxUIElementParent;

    PXGraphicPXUIElementChildSet(pxUIElementParent, pxUIElement);
}

void PXAPI PXGraphicPXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Child = pxUIElementParent;
}

void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = PXGraphicDrawModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}

PXActionResult PXAPI PXGraphicInstantiate(PXGraphic* const pxGraphic, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    pxGraphic->UIElementIDCounter = 0;

    pxGraphic->AttachedWindow = pxGraphicInitializeInfo->WindowReference;
    pxGraphic->GraphicSystem = pxGraphicInitializeInfo->GraphicSystem;
    pxGraphicInitializeInfo->Graphic = pxGraphic;

#if 0
    // If window is not set, we need to make one!
    if(pxGraphic->AttachedWindow == PXNull)
    {
        // Make window
        PXWindowCreateHidden();
    }
#endif

    
    /*
    for (size_t displayID = 0; displayID < EnumDisplayMonitors(PXNull, ); ++displayID)
    {

    }*/

    // EnumDisplayDevicesA

    // Fetch all graphical devices 
    {
        PXMonitorDeviceAmount(&pxGraphic->DevicePhysicalListSize);
      
        // Allocate space for needed devices
        pxGraphic->DevicePhysicalList = PXNewList(PXGraphicDevicePhysical, pxGraphic->DevicePhysicalListSize);

        PXPhysicalDeviceFetchAll(pxGraphic->DevicePhysicalList, pxGraphic->DevicePhysicalListSize);
    }  
 

#if 0
    // Create GPU affinity
    int selecvtDeviceID = 0;
    HDC dc = CreateDCA(graphicDevicePhysical[selecvtDeviceID].DeviceDisplay, graphicDevicePhysical[selecvtDeviceID].DeviceName, NULL, NULL); // "DISPLAY"

    auto xxxx = GetLastError();

    pxWindow->HandleDeviceContext = dc;
#endif

    PXWindowPixelSystemSet(pxGraphicInitializeInfo->WindowReference);



#if 0

   // void choose_ogl_vendor(const char* vendor_id)
   // {
        int idx;
   
        HDC dc;
        PIXELFORMATDESCRIPTOR pfd;

        dd.cb = sizeof(dd);
        idx = 0;
        while (1)
        {
            if (!EnumDisplayDevicesA(NULL, idx, &dd, 0))
                return; // not found!
            if (strstr(dd.DeviceID, vendor_id))
                break; // there we go
            idx += 1;
        }

        dc = CreateDCA(dd.DeviceName, NULL, NULL, NULL);
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        // those flags are not important, they just need to be valid (and nondemanding, just in case).
        // later you will use whatever flags you wish when you are creating your actual gl context
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
        ChoosePixelFormat(dc, &pfd);
        DeleteDC(dc);
  //  }
#endif

    PXLockCreate(&pxGraphic->_resourceLock, PXLockTypeGlobal);

    PXDictionaryConstruct(&pxGraphic->ResourceImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);


    PXDictionaryConstruct(&pxGraphic->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);

    PXDictionaryConstruct(&pxGraphic->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);

    PXDictionaryConstruct(&pxGraphic->SoundLookup, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->ShaderPXProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);

    //-------------------------------------------------------------------------
    // Setup references
    //-------------------------------------------------------------------------
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            pxGraphic->EventOwner = &pxGraphic->OpenGLInstance;

            pxGraphic->ShaderProgramCreateFromFileVP = PXOpenGLShaderProgramCreateFromFileVF;
            pxGraphic->ShaderProgramCreateFromFileVPA = PXOpenGLShaderProgramCreateFromFileVFA;
            pxGraphic->ShaderProgramCreateFromStringVP = PXOpenGLShaderProgramCreateFromStringVF;
            pxGraphic->ShaderProgramCreateFromStringVPA = PXOpenGLShaderProgramCreateFromStringVFA;

            pxGraphic->ShaderVariableFx1 = PXOpenGLShaderVariableFx1;
            pxGraphic->ShaderVariableFx1xN = PXOpenGLShaderVariableFx1xN;
            pxGraphic->ShaderVariableIx1 = PXOpenGLShaderVariableIx1;
            pxGraphic->ShaderVariableIx1xN = PXOpenGLShaderVariableIx1xN;
            pxGraphic->ShaderVariableFx2 = PXOpenGLShaderVariableFx2;
            pxGraphic->ShaderVariableFx2xN = PXOpenGLShaderVariableFx2xN;
            pxGraphic->ShaderVariableIx2 = PXOpenGLShaderVariableIx2;
            pxGraphic->ShaderVariableIx2xN = PXOpenGLShaderVariableIx2xN;
            pxGraphic->ShaderVariableFx3 = PXOpenGLShaderVariableFx3;
            pxGraphic->ShaderVariableFx3xN = PXOpenGLShaderVariableFx3xN;
            pxGraphic->ShaderVariableIx3 = PXOpenGLShaderVariableIx3;
            pxGraphic->ShaderVariableIx3xN = PXOpenGLShaderVariableIx3xN;
            pxGraphic->ShaderVariableFx4 = PXOpenGLShaderVariableFx4;
            pxGraphic->ShaderVariableFx4xN = PXOpenGLShaderVariableFx4xN;
            pxGraphic->ShaderVariableIx4 = PXOpenGLShaderVariableIx4;
            pxGraphic->ShaderVariableIx4xN = PXOpenGLShaderVariableIx4xN;
            pxGraphic->ShaderVariableMatrix2fv = PXOpenGLShaderVariableMatrix2fv;
            pxGraphic->ShaderVariableMatrix3fv = PXOpenGLShaderVariableMatrix3fv;
            pxGraphic->ShaderVariableMatrix4fv = PXOpenGLShaderVariableMatrix4fv;

            pxGraphic->ScreenBufferRead = PXOpenGLScreenBufferRead;

            pxGraphic->ShaderVariableIDFetch = PXOpenGLShaderVariableIDFetch;
            pxGraphic->DrawModeSet = PXOpenGLDrawMode;

            pxGraphic->DrawColorRGBAF = PXOpenGLDrawColorRGBAF;

            pxGraphic->RectangleDraw = PXOpenGLRectangleDraw;
            pxGraphic->RectangleDrawTx = PXOpenGLRectangleDrawTx;

            pxGraphic->SwapIntervalSet = PXOpenGLSwapIntervalSet;
            pxGraphic->SwapIntervalGet = PXOpenGLSwapIntervalGet;

            pxGraphic->DevicePhysicalListAmount = PXOpenGLDevicePhysicalListAmount;
            pxGraphic->DevicePhysicalListFetch = PXOpenGLDevicePhysicalListFetch;

            pxGraphic->Initialize = PXOpenGLInitialize;
            pxGraphic->Release = PXOpenGLRelease;
            pxGraphic->Select = PXOpenGLSelect;
            pxGraphic->Deselect = PXOpenGLDeselect;
            pxGraphic->Clear = PXOpenGLClear;
            pxGraphic->SceneDeploy = PXOpenGLSceneDeploy;
            pxGraphic->ViewPortGet = PXOpenGLViewPortGet;
            pxGraphic->ViewPortSet = PXOpenGLViewPortSet;
            pxGraphic->ShaderProgramCreate = PXOpenGLShaderProgramCreate;
            pxGraphic->ShaderProgramSelect = PXOpenGLShaderProgramSelect;
            pxGraphic->ShaderProgramDelete = PXOpenGLShaderProgramDelete;
            pxGraphic->SceneBegin = PXNull;
            pxGraphic->SceneEnd = PXNull;
            pxGraphic->DrawScriptCreate = PXOpenGLDrawScriptCreate;
            pxGraphic->DrawScriptBegin = PXOpenGLDrawScriptBegin;
            pxGraphic->DrawScriptEnd = PXOpenGLDrawScriptEnd;
            pxGraphic->DrawScriptDelete = PXOpenGLDrawScriptDelete;
            pxGraphic->DrawScriptExecute = PXOpenGLDrawScriptExecute;
            pxGraphic->TextureCubeRegister = PXOpenGLTextureCubeCreate;
            pxGraphic->TextureCubeRegisterUse = PXNull;
            pxGraphic->TextureCubeRelease = PXNull;
            pxGraphic->ModelRegister = PXOpenGLModelRegister;
            pxGraphic->ModelDraw = PXOpenGLModelDraw;
            pxGraphic->ModelSelect = PXNull;
            pxGraphic->ModelRelease = PXNull;
            pxGraphic->LightSet = PXOpenGLLightSet;
            pxGraphic->LightGet = PXOpenGLLightGet;
            pxGraphic->LightEnableSet = PXNull;
            pxGraphic->LightEnableGet = PXNull;
            pxGraphic->Texture2DRegister = PXOpenGLTexture2DCreate;
            pxGraphic->Texture2DRelease = PXNull;
            pxGraphic->Texture2DSelect = PXOpenGLTexture2DBind;

            break;
        }
#if OSWindows
        case PXGraphicSystemDirectX:
        {
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;

            pxGraphic->ShaderProgramCreateFromFileVP = PXDirectXShaderProgramCreateFromFileVF;
            pxGraphic->ShaderProgramCreateFromFileVPA = PXDirectXShaderProgramCreateFromFileVFA;
            pxGraphic->ShaderProgramCreateFromStringVP = PXDirectXShaderProgramCreateFromStringVF;
            pxGraphic->ShaderProgramCreateFromStringVPA = PXDirectXShaderProgramCreateFromStringVFA;

            pxGraphic->ShaderVariableIDFetch = PXNull;

            pxGraphic->ShaderVariableFx1 = PXNull;
            pxGraphic->ShaderVariableFx1xN = PXNull;
            pxGraphic->ShaderVariableIx1 = PXNull;
            pxGraphic->ShaderVariableIx1xN = PXNull;
            pxGraphic->ShaderVariableFx2 = PXNull;
            pxGraphic->ShaderVariableFx2xN = PXNull;
            pxGraphic->ShaderVariableIx2 = PXNull;
            pxGraphic->ShaderVariableIx2xN = PXNull;
            pxGraphic->ShaderVariableFx3 = PXNull;
            pxGraphic->ShaderVariableFx3xN = PXNull;
            pxGraphic->ShaderVariableIx3 = PXNull;
            pxGraphic->ShaderVariableIx3xN = PXNull;
            pxGraphic->ShaderVariableFx4 = PXNull;
            pxGraphic->ShaderVariableFx4xN = PXNull;
            pxGraphic->ShaderVariableIx4 = PXNull;
            pxGraphic->ShaderVariableIx4xN = PXNull;
            pxGraphic->ShaderVariableMatrix2fv = PXNull;
            pxGraphic->ShaderVariableMatrix3fv = PXNull;
            pxGraphic->ShaderVariableMatrix4fv = PXNull;

            pxGraphic->DrawModeSet = PXNull;
            pxGraphic->DrawColorRGBAF = PXNull;

            pxGraphic->RectangleDraw = PXNull;
            pxGraphic->RectangleDrawTx = PXNull;

            pxGraphic->SwapIntervalSet = PXDirectXSwapIntervalSet;
            pxGraphic->SwapIntervalGet = PXDirectXSwapIntervalGet;

            pxGraphic->DevicePhysicalListAmount = PXNull;
            pxGraphic->DevicePhysicalListFetch = PXNull;

            pxGraphic->Initialize = PXDirectXInitialize; // PXDirectXVersion9, PXDirectXDriverTypeHardwareDevice);
            pxGraphic->Release = PXDirectXRelease;
            pxGraphic->Select = PXNull;
            pxGraphic->Deselect = PXNull;
            pxGraphic->Clear = PXDirectXClear;
            pxGraphic->SceneDeploy = PXDirectXSceneDeploy;
            pxGraphic->ViewPortGet = PXDirectXViewportGet;
            pxGraphic->ViewPortSet = PXDirectXViewportSet;
            pxGraphic->ShaderProgramCreate = PXDirectXShaderProgramCreate;
            pxGraphic->ShaderProgramSelect = PXDirectXShaderProgramSelect;
            pxGraphic->ShaderProgramDelete = PXDirectXShaderProgramDelete;
            pxGraphic->SceneBegin = PXDirectXSceneBegin;
            pxGraphic->SceneEnd = PXDirectXSceneEnd;
            pxGraphic->DrawScriptCreate = PXDirectXDrawScriptCreate;
            pxGraphic->DrawScriptBegin = PXDirectXDrawScriptBegin;
            pxGraphic->DrawScriptEnd = PXDirectXDrawScriptEnd;
            pxGraphic->DrawScriptDelete = PXDirectXDrawScriptDelete;
            pxGraphic->DrawScriptExecute = PXDirectXDrawScriptExecute;
            pxGraphic->TextureCubeRegister = PXDirectXTextureCubeCreate;
            pxGraphic->TextureCubeRegisterUse = PXNull;
            pxGraphic->TextureCubeRelease = PXNull;
            pxGraphic->ModelRegister = PXDirectXModelRegister;
            pxGraphic->ModelDraw = PXDirectXModelDraw;
            pxGraphic->ModelSelect = PXNull;
            pxGraphic->ModelRelease = PXNull;
            pxGraphic->LightSet = PXDirectXLightSet;
            pxGraphic->LightGet = PXDirectXLightGet;
            pxGraphic->LightEnableSet = PXNull;
            pxGraphic->LightEnableGet = PXNull;
            pxGraphic->Texture2DRegister = PXDirectXTexture2DCreate;
            pxGraphic->Texture2DRelease = PXNull;
            pxGraphic->Texture2DSelect = PXNull;           

            break;
        }
#endif
        case PXGraphicSystemVulcan:
        {
            pxGraphic->EventOwner = &pxGraphic->VulcanInstance;

            pxGraphic->Initialize = PXVulcanInitialize;
            pxGraphic->Release = PXVulcanRelease;

            break;
        }

        default:
            break;
    }

    // Graphic initialize
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "Graphic",
            "Init",
            "Invoke spesific API..."
        );

        const PXActionResult pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

        PXActionReturnOnError(pxActionResult);
    }

    //-------------------------------------------------------------------------

#if 0
    for (size_t i = 0; i < pxGraphic->DevicePhysicalListSize; i++)
    {
        PXGraphicDevicePhysical* const pxGraphicDevicePhysical = &pxGraphic->DevicePhysicalList[i];

        char targetBuffer[64];

        if (pxGraphicDevicePhysical->Driver[0] != '\0')
        {
            PXTextPrintA(targetBuffer, 64, "%s, %s (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->Driver, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }
        else
        {
            PXTextPrintA(targetBuffer, 64, "%s, (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }      

        printf
        (
            "+---------------------------------------------------------+\n"
            "| Graphics Card (%2i/%-2i) - Information                     |\n"
            "+---------------------------------------------------------+\n"
            "| Model         : %-39s |\n"
            "| Target        : %-39s |\n"
            "| Vendor        : %-39s |\n"
            "| Renderer      : %-39s |\n"
            "| Shader-OpenGL : %-39s |\n"
            "+---------------------------------------------------------+\n\n",
            i+1,
            pxGraphic->DevicePhysicalListSize,
            pxGraphicDevicePhysical->DeviceName,          
            targetBuffer,
            pxGraphicDevicePhysical->Vendor,
            pxGraphicDevicePhysical->Renderer,
            pxGraphicDevicePhysical->Shader
        );
    }
#endif



#if 0
    PXViewPort pxViewPort;
    pxViewPort.X = 0;
    pxViewPort.Y = 0;
    pxViewPort.Width = pxGraphic->AttachedWindow->Width;
    pxViewPort.Height = pxGraphic->AttachedWindow->Height;
    pxViewPort.ClippingMinimum = 0;
    pxViewPort.ClippingMaximum = 1;

    pxGraphic->ViewPortSet(pxGraphic->EventOwner, &pxViewPort);
#endif

    //PXMatrix4x4FIdentity(&pxGraphic->SpriteScaled.ModelMatrix);
   // PXMatrix4x4FIdentity(&pxGraphic->SpriteUnScaled.ModelMatrix);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicRelease(PXGraphic* const pxGraphic)
{
    return PXActionRefusedNotImplemented;
}

void PXAPI PXGraphicHotSwap(PXGraphic* const pxGraphic, const PXGraphicSystem pxGraphicSystem)
{
    //pxGraphic->Release();
}

void PXAPI PXGraphicResourceRegister(PXGraphic* const pxGraphic, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{

}

PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphic* const pxGraphic, PXSprite* const pxSprite)
{
    PXClear(PXSprite, pxSprite);

    PXModelConstruct(&pxSprite->Model);


    //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
    //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

    PXVector2FSetXY(&pxSprite->TextureScalePositionOffset, 1, 1);

  //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
}

PXActionResult PXAPI PXGraphicSpriteDraw(PXGraphic* const pxGraphic, const PXSprite* const pxSprite, const PXCamera* const pxCamera)
{
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            return PXOpenGLSpriteDraw(&pxGraphic->OpenGLInstance, pxSprite, pxCamera);
        }
        case PXGraphicSystemDirectX:
        {
            return PXActionNotSupportedByLibrary;
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXAPI PXGraphicShaderUpdateMatrix4x4F(PXGraphic* const pxGraphic, const unsigned int locationID, const float* const matrix4x4)
{
#if PXOpenGLUSE
    PXOpenGLShaderVariableMatrix4fv(&pxGraphic->OpenGLInstance, locationID, 1, 0, matrix4x4);
#endif
}
#endif

void PXAPI PXCameraConstruct(PXCamera* const camera)
{
    PXMemoryClear(camera, sizeof(PXCamera));

    camera->WalkSpeed = 0.2;
    camera->ViewSpeed = 0.4;
    camera->FollowSpeed = 0.98f;
    camera->FieldOfView = 90;
    camera->Height = 1;
    camera->Width = 1;
    camera->Near = -0.001;
    camera->Far = 100000;

    PXMatrix4x4FIdentity(&camera->MatrixModel);
    PXMatrix4x4FIdentity(&camera->MatrixView);
    PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(camera, PXCameraPerspective3D);

    const PXVector3F position = {0,0,0};
    PXCameraRotate(camera, &position);
}

void PXAPI PXCameraDestruct(PXCamera* const camera)
{

}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera* const camera, const PXSize width, const PXSize height, const float nearPlane, const float farPlane)
{
    const float scaling = 0.005;
    const float left = -(width / 2.0f) * scaling;
    const float right = (width / 2.0f) * scaling;
    const float bottom = -(height / 2.0f) * scaling;
    const float top = (height / 2.0f) * scaling;

    camera->Perspective = PXCameraPerspective2D;
    camera->Width = width;
    camera->Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera* const camera, const PXSize width, const PXSize height)
{
    camera->Width = width;
    camera->Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

float PXAPI PXCameraAspectRatio(const PXCamera* const camera)
{
    return (float)camera->Width / (float)camera->Height;
}

void PXAPI PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective)
{
    camera->Perspective = cameraPerspective;

    switch (cameraPerspective)
    {
        case PXCameraPerspective2D:
        {
            PXCameraViewChangeToOrthographic(camera, camera->Width, camera->Height, camera->Near, camera->Far);
            break;
        }

        case PXCameraPerspective3D:
        {
            const float aspectRatio = PXCameraAspectRatio(camera);

            PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

            break;
        }
    }
}

void PXAPI PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F)
{
    const float maxValue = 85.0f;
    const float minValue = -85.0f;

    camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
    camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

    camera->CurrentRotation.Y = PXMathLimit(camera->CurrentRotation.Y, minValue, maxValue);

    const float pitchRAD = PXMathDegreeToRadians(camera->CurrentRotation.Y);
    const float yawRAD = PXMathDegreeToRadians(camera->CurrentRotation.X);
    const float rx = PXMathCosinus(pitchRAD) * PXMathCosinus(yawRAD);
    const float ry = PXMathSinus(pitchRAD);
    const float rz = PXMathCosinus(pitchRAD) * PXMathSinus(yawRAD);

    PXVector3FSetXYZ(&camera->LookAtPosition, rx, ry, rz);
    PXVector3FNormalize(&camera->LookAtPosition);
}

void PXAPI PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F)
{
    PXVector3F xAxis = { 0,0,0 };
    const PXVector3F yAxis = { 0, vector3F->Y, 0 };
    PXVector3F zAxis = { 0,0,0 };

    // ...
    {
        const PXVector3F up = { 0, 1, 0 };
        const PXVector3F lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

        PXVector3FCrossProduct(&lookAtPosition, &up, &xAxis);
        PXVector3FNormalize(&xAxis);
        PXVector3FMultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X);

        zAxis = lookAtPosition;

        PXVector3FNormalize(&zAxis);
        PXVector3FMultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z);
    }

    {
        PXVector3F targetedMovement = { 0,0,0 };

        PXVector3FAdd(&targetedMovement, &xAxis);
        PXVector3FAdd(&targetedMovement, &yAxis);
        PXVector3FAdd(&targetedMovement, &zAxis);
        PXVector3FMultiplyS(&targetedMovement, camera->WalkSpeed);

        PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement);
    }
}

void PXAPI PXCameraFollow(PXCamera* const camera, const float deltaTime)
{
    PXVector3F cameraPositionCurrent;
    PXVector3F desiredPosition;

    if (!camera->Target)
    {
        return;
    }

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &cameraPositionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &desiredPosition); // get current target pos

    PXVector3FAdd(&desiredPosition, &camera->Offset); // add offset to target pos

    PXVector3FInterpolate(&cameraPositionCurrent, &desiredPosition, camera->FollowSpeed * deltaTime, &desiredPosition); // calculate delta movement

    PXMatrix4x4FPositionSet(&camera->MatrixModel, &desiredPosition); // Set delte movement
}

void PXAPI PXCameraUpdate(PXCamera* const camera, const float deltaTime)
{
    const float walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
    const float viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
    const PXVector3F up = { 0,1,0 };
    PXVector3F currentPosition;
    PXVector3F centerPosition;

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    centerPosition = currentPosition;
    PXVector3FAdd(&centerPosition, &camera->LookAtPosition);

    PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up);
}
