#include "PXKnowlegeGraph.h"

#include <OS/Graphic/PXGraphic.h>
#include <OS/Window/PXWindow.h>
#include <Math/PXMath.h>

PXActionResult PXAPI PXKnowlegeGraphNodeInsert(PXKnowlegeGraph* const pxKnowlegeGraph, PXKnowlegeGraphNode* const pxKnowlegeGraphNode)
{
    

    return PXActionSuccessful;
}

void PXDrawTriangle()
{
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex2f(-0.5f, -0.5f);
    glColor3f(0, 1, 0); glVertex2f(0.5f, -0.5f);
    glColor3f(0, 0, 1); glVertex2f(0, 0.5f);
    glEnd();
}

typedef struct PXLine_
{
    PXColorRGBAF* ColorFill;
    PXColorRGBAF* ColorBorder;

    float Positon[4];
    float Scale[2];
}
PXLine;

typedef struct PXCircle
{
    PXColorRGBAF* ColorFill;
    PXColorRGBAF* ColorBorder;

    float Positon[2];
    float Scale[2];

    PXInt32U LevelOfDetail;
}
PXCircle;

void PXDrawLine(PXLine* const pxLine)
{
    const PXColorRGBAF* const colorList[2] = {pxLine->ColorBorder, pxLine->ColorFill };
    const int lineSize[2] = { 20, 6 };
    const int drawMode[2] = { GL_LINES, GL_POINTS };

    for (size_t j = 0; j < 2; j++)
    {
        for (size_t i = 0; i < 2; i++)
        {
            const PXColorRGBAF* const color = colorList[i];

            glLineWidth(lineSize[i]);
            glPointSize(lineSize[i]);

            glBegin(drawMode[j]);

            glColor4f(color->Red, color->Green, color->Blue, color->Alpha);

            glVertex2f(pxLine->Positon[0], pxLine->Positon[1]);
            glVertex2f(pxLine->Positon[2], pxLine->Positon[3]);

            glEnd();
        }
    }

   
}

void PXDrawCircle(PXCircle* const pxCircle)
{
    const float piX2 = PXMathConstantPI * 2.0f;

    PXColorRGBAF* colorList[2] = { pxCircle->ColorFill, pxCircle->ColorBorder };
    int modeList[2] = { GL_POLYGON, GL_LINE_LOOP };

    glLineWidth(3);

    for (PXSize i = 0; i < 2; ++i)
    {
        PXColorRGBAF* color = colorList[i];

        glBegin(modeList[i]);

        glColor4f(color->Red, color->Green, color->Blue, color->Alpha);

        for (PXSize j = 0; j < pxCircle->LevelOfDetail; ++j)
        {
            const float angle = piX2 * j / pxCircle->LevelOfDetail;
            const float x = pxCircle->Positon[0] + PXMathCosinus(angle) * pxCircle->Scale[0];
            const float y = pxCircle->Positon[1] + PXMathSinus(angle) * pxCircle->Scale[1];
            glVertex2f(x, y);
        }

        glEnd();
    }
}

void PXDrawCircleList(PXCircle* const pxCircleList, const PXSize pxSize)
{
    PXColorRGBAF fillColor = { 0.6,0.2,0.6,1 };
    PXColorRGBAF borderColor = { 0.8,0.8,0.8,1 };

    PXLine pxLine;
    pxLine.ColorFill = &fillColor;
    pxLine.ColorBorder = &borderColor;

    for (PXSize i = 0; i < pxSize; ++i)
    {
        PXCircle* const pxCircle = &pxCircleList[i];

        pxLine.Positon[2] = pxCircle->Positon[0];
        pxLine.Positon[3] = pxCircle->Positon[1];       

        PXDrawCircle(pxCircle);

        if (i > 0)
        {
            PXDrawLine(&pxLine);
        }

        pxLine.Positon[0] = pxCircle->Positon[0];
        pxLine.Positon[1] = pxCircle->Positon[1];
    }
}



PXActionResult PXAPI PXKnowlegeGraphLoadAndBuild(PXKnowlegeGraph* const pxKnowlegeGraph, PXDocument* const pxDocument, PXImage* const pxImage)
{
    PXSize imageWidth = pxKnowlegeGraph->SizeMaxWidth;
    PXSize imageHeight = pxKnowlegeGraph->SizeMaxHeight;

    imageWidth = 500;
    imageHeight = 500;

    // How many nodes do we have, allocate space for it.

   // PXDocumentPrint(pxDocument);


    // Insert Root at (0,0,0)
    

    // Insert all siblings in a cictular pattern?

    // for each sibling, add their Siblings
    PXWindow pxWindow;
    PXWindowConstruct(&pxWindow);
    //PXWindowCreateA(&pxWindow, 50,50, 1000, 1000, "==", PXTrue);
    PXWindowCreateHidden(&pxWindow, imageWidth, imageHeight, PXTrue);


    // Create windowless rendering instance
    PXGraphic pxGraphic;

    {
        PXClear(PXGraphic, &pxGraphic);

        PXGraphicInitializeInfo pxGraphicInitializeInfo;
        PXClear(PXGraphicInitializeInfo, &pxGraphicInitializeInfo);
        pxGraphicInitializeInfo.WindowReference = &pxWindow;
        pxGraphicInitializeInfo.Width = imageWidth;
        pxGraphicInitializeInfo.Height = imageHeight;
        pxGraphicInitializeInfo.GraphicSystem = PXGraphicSystemOpenGL;
        pxGraphicInitializeInfo.Graphic = &pxGraphic;

        PXGraphicInstantiate(&pxGraphic, &pxGraphicInitializeInfo);
    }

    glViewport(0,0, imageWidth, imageHeight);

    // Render image  
    {
        PXColorRGBAF clearColor = { 0.1,0,0,1 };
        PXColorRGBAF drawColor = { 1,0,0,0 };

        pxGraphic.Select(pxGraphic.EventOwner);

        //glViewport(0,0, pxWindow.Width, pxWindow.Height);

        for (size_t i = 0; i < 4; i++)
        //while (1)
        {
            //pxGraphic.SceneBegin(pxGraphic.EventOwner);

            pxGraphic.Clear(pxGraphic.EventOwner, &clearColor);
      
                       
            PXColorRGBAF fillColor = { 1,0,0,1 };
            PXColorRGBAF borderColor = { 1,1,1,1 };

            PXCircle pxCircleList[5];

            for (size_t i = 0; i < 6; i++)
            {
                PXCircle* const pxCircle = &pxCircleList[i];

                pxCircle->ColorBorder = &borderColor;
                pxCircle->ColorFill = &fillColor;
                pxCircle->Positon[0] = PXMathCosinus(i*0.85) * 0.5;
                pxCircle->Positon[1] = PXMathSinus(i * 0.85) * 0.5;
                pxCircle->Scale[0] = 0.1;
                pxCircle->Scale[1] = 0.1;
                pxCircle->LevelOfDetail = 64;
            }

            PXDrawCircleList(pxCircleList, 5);


           // pxGraphic.DrawColorRGBAF(pxGraphic.EventOwner, 1, 0, 0, 1);
            //pxGraphic.RectangleDraw(pxGraphic.EventOwner, -0.5, 0.2, 0.5, 0.6, 0x01);
           // pxGraphic.SceneEnd(pxGraphic.EventOwner);
            pxGraphic.SceneDeploy(pxGraphic.EventOwner);
        }    
    }

    // Screenshot Scene
    {
        PXClear(PXImage, pxImage);    

        PXImageResize(pxImage, PXColorFormatRGBI8, imageWidth, imageHeight);

        pxGraphic.ScreenBufferRead(pxGraphic.EventOwner, pxImage);

        const PXActionResult pxSaveResult = PXResourceSaveA(pxImage, "_TEST_DATA_INPUT_\\books.bmp", PXFileFormatBitMap);
    }



    PXGraphicRelease(&pxGraphic);
    PXWindowDestruct(&pxWindow);


    return PXActionSuccessful;
}