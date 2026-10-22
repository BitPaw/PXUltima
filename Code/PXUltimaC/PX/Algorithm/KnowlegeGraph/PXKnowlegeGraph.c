#include "PXKnowlegeGraph.h"


#include <PX/Math/PXMath.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>

PXResult PXAPI PXKnowlegeGraphNodeInsert(PXKnowlegeGraph PXREF pxKnowlegeGraph, PXKnowlegeGraphNode PXREF pxKnowlegeGraphNode)
{


    return PXResultOK;
}

void PXDrawTriangle(PXOpenGL* pxOpenGL)
{
    pxOpenGL->Binding.Begin(GL_TRIANGLES);
    pxOpenGL->Binding.Color3f(1, 0, 0);
    pxOpenGL->Binding.Vertex2f(-0.5f, -0.5f);
    pxOpenGL->Binding.Color3f(0, 1, 0);
    pxOpenGL->Binding.Vertex2f(0.5f, -0.5f);
    pxOpenGL->Binding.Color3f(0, 0, 1);
    pxOpenGL->Binding.Vertex2f(0, 0.5f);
    pxOpenGL->Binding.End();
}

typedef struct PXLine_
{
    PXColorRGBA_F32* ColorFill;
    PXColorRGBA_F32* ColorBorder;

    PXF32 Positon[4];
    PXF32 Scale[2];
}
PXLine;

typedef struct PXCircle
{
    PXColorRGBA_F32* ColorFill;
    PXColorRGBA_F32* ColorBorder;

    PXF32 Positon[2];
    PXF32 Scale[2];

    PXI32U LevelOfDetail;
}
PXCircle;

void PXDrawLine(PXOpenGL PXREF pxOpenGL, PXLine PXREF pxLine)
{
    const PXColorRGBA_F32 PXREF colorList[2] = {pxLine->ColorBorder, pxLine->ColorFill };
    const int lineSize[2] = { 20, 6 };
    const int drawMode[2] = { GL_LINES, GL_POINTS };

    for (size_t j = 0; j < 2; j++)
    {
        for (size_t i = 0; i < 2; i++)
        {
            const PXColorRGBA_F32 PXREF color = colorList[i];

            pxOpenGL->Binding.LineWidth(lineSize[i]);
            pxOpenGL->Binding.PointSize(lineSize[i]);

            pxOpenGL->Binding.Begin(drawMode[j]);

            pxOpenGL->Binding.Color4f(color->Red, color->Green, color->Blue, color->Alpha);

            pxOpenGL->Binding.Vertex2f(pxLine->Positon[0], pxLine->Positon[1]);
            pxOpenGL->Binding.Vertex2f(pxLine->Positon[2], pxLine->Positon[3]);

            pxOpenGL->Binding.End();
        }
    }


}

void PXDrawCircle(PXOpenGL PXREF pxOpenGL, PXCircle PXREF pxCircle)
{
    const PXF32 piX2 = PXMathPI * 2.0f;

    PXColorRGBA_F32* colorList[2] = { pxCircle->ColorFill, pxCircle->ColorBorder };
    int modeList[2] = { GL_POLYGON, GL_LINE_LOOP };

    pxOpenGL->Binding.LineWidth(3);

    for (PXSize i = 0; i < 2; ++i)
    {
        PXColorRGBA_F32* color = colorList[i];

        pxOpenGL->Binding.Begin(modeList[i]);

        pxOpenGL->Binding.Color4f(color->Red, color->Green, color->Blue, color->Alpha);

        for (PXSize j = 0; j < pxCircle->LevelOfDetail; ++j)
        {
            const PXF32 angle = piX2 * j / pxCircle->LevelOfDetail;
            const PXF32 x = pxCircle->Positon[0] + PXMathCosinusRADF32(angle) * pxCircle->Scale[0];
            const PXF32 y = pxCircle->Positon[1] + PXMathSinusRADF32(angle) * pxCircle->Scale[1];
            pxOpenGL->Binding.Vertex2f(x, y);
        }

        pxOpenGL->Binding.End();
    }
}

void PXDrawCircleList(PXOpenGL PXREF pxOpenGL, PXCircle PXREF pxCircleList, const PXSize pxSize)
{
    PXColorRGBA_F32 fillColor = { 0.6,0.2,0.6,1 };
    PXColorRGBA_F32 borderColor = { 0.8,0.8,0.8,1 };

    PXLine pxLine;
    pxLine.ColorFill = &fillColor;
    pxLine.ColorBorder = &borderColor;

    for (PXSize i = 0; i < pxSize; ++i)
    {
        PXCircle PXREF pxCircle = &pxCircleList[i];

        pxLine.Positon[2] = pxCircle->Positon[0];
        pxLine.Positon[3] = pxCircle->Positon[1];

        PXDrawCircle(pxOpenGL, pxCircle);

        if (i > 0)
        {
            PXDrawLine(pxOpenGL, &pxLine);
        }

        pxLine.Positon[0] = pxCircle->Positon[0];
        pxLine.Positon[1] = pxCircle->Positon[1];
    }
}

PXResult PXAPI PXKnowlegeGraphLoadAndBuild
(
    PXKnowlegeGraph PXREF pxKnowlegeGraph, 
    PXCodeDocument PXREF pxDocument
)
{
    PXSize imageWidth = pxKnowlegeGraph->SizeMaxWidth;
    PXSize imageHeight = pxKnowlegeGraph->SizeMaxHeight;

    imageWidth = 500;
    imageHeight = 500;

#if 0

    // How many nodes do we have, allocate space for it.

    // PXDocumentPrint(pxDocument);


    // Insert Root at (0,0,0)


    // Insert all siblings in a cictular pattern?

    // for each sibling, add their Siblings
    PXWindow* pxWindow = 0;
    //PXWindowConstruct(&pxWindow);
    //PXWindowCreateA(&pxWindow, 50,50, 1000, 1000, "==", PXTrue);
    //PXWindowCreateHidden(&pxWindow, imageWidth, imageHeight, PXTrue);

    /*
    // Create windowless rendering instance
    PXGraphic pxGraphic;

    {
        PXClear(PXGraphic, &pxGraphic);

        PXGraphicInitializeInfo pxGraphicInitializeInfo;
        PXClear(PXGraphicInitializeInfo, &pxGraphicInitializeInfo);
        pxGraphicInitializeInfo.WindowReference = pxWindow;
        pxGraphicInitializeInfo.Width = imageWidth;
        pxGraphicInitializeInfo.Height = imageHeight;
        pxGraphicInitializeInfo.GraphicSystem = PXGraphicAPIOpenGL;
        pxGraphicInitializeInfo.Graphic = &pxGraphic;

        PXGraphicInstantiate(&pxGraphic, &pxGraphicInitializeInfo);
    }
    */

    pxGraphic.APIOpenGL.Binding.Viewport(0,0, imageWidth, imageHeight);

    // Render image
    {
        PXColorRGBA_F32 clearColor = { 0.1,0,0,1 };
        PXColorRGBA_F32 drawColor = { 1,0,0,0 };

        pxGraphic.Select(pxGraphic.EventOwner, pxWindow);

        //glViewport(0,0, pxWindow.Width, pxWindow.Height);

        for (size_t i = 0; i < 4; i++)
            //while (1)
        {
            //pxGraphic.SceneBegin(pxGraphic.EventOwner);

            pxGraphic.Clear(pxGraphic.EventOwner, &clearColor);


            PXColorRGBA_F32 fillColor = { 1,0,0,1 };
            PXColorRGBA_F32 borderColor = { 1,1,1,1 };

            PXCircle pxCircleList[5];

            for (size_t i = 0; i < 6; i++)
            {
                PXCircle PXREF pxCircle = &pxCircleList[i];

                pxCircle->ColorBorder = &borderColor;
                pxCircle->ColorFill = &fillColor;
                pxCircle->Positon[0] = PXMathCosinusRADF32(i*0.85) * 0.5;
                pxCircle->Positon[1] = PXMathSinusRADF32(i * 0.85) * 0.5;
                pxCircle->Scale[0] = 0.1;
                pxCircle->Scale[1] = 0.1;
                pxCircle->LevelOfDetail = 64;
            }

            PXDrawCircleList(&pxGraphic.APIOpenGL, pxCircleList, 5);


            // pxGraphic.DrawColorRGBAF(pxGraphic.EventOwner, 1, 0, 0, 1);
            //pxGraphic.RectangleDraw(pxGraphic.EventOwner, -0.5, 0.2, 0.5, 0.6, 0x01);
            // pxGraphic.SceneEnd(pxGraphic.EventOwner);
            pxGraphic.SceneDeploy(pxGraphic.EventOwner);
        }
    }

    // Screenshot Scene
    {
        PXClear(PXTexture, PXTexture);

        PXTextureResize
        (
            PXTexture,
            PXColorFormatRGBI8, 
            imageWidth, 
            imageHeight,
            PXTextureNoMinmaps
        );

        pxGraphic.ScreenBufferRead(pxGraphic.EventOwner, PXTexture);

        //const PXResult pxSaveResult = PXResourceSaveA(PXTexture, "_TEST_DATA_INPUT_\\books.bmp", PXFileFormatBitMap);
    }



    PXGraphicRelease(&pxGraphic);
    //  PXWindowDestruct(&pxWindow);
#endif

    return PXResultOK;
}
