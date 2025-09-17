#include <PX/OS/Console/PXConsole.h>

#define PXLDAPTest 0
#define PXKeyBoardVirtualTest 0

int _fltused = 0;

#if 1

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Comctl32.lib")
#endif


#pragma comment(lib, "opengl32.lib")



#if 0
#include "OS/TestSystemInfo.h"
#include "Graphic/TestWindow.h"
#include "Graphic/TestOpenGL.h"
#include "Graphic/TestFont.h"
#include "Compiler/TestPXCompiler.h"
#include "Graphic/TestImage.h"
#include "Compiler/TestYAML.h"
#include "Sound/TestSound.h"
#include "OS/TestSocket.h"
#include "Service/TestFTP.h"
#include "Service/TestSBP.h"
#endif

#include "Network/PXNetworkTest.h"
#include "Window/PXWindowTest.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/FastFile/PXFastFile.h>
#include <PX/Media/C/PXC.h>
#include <PX/Media/PXDocument.h>

/*

void OnFileElementDetected(PXFileElementInfo* pxFileElementInfo)
{
    switch (pxFileElementInfo->Type)
    {
        case PXFileElementInfoTypeInvalid:
        case PXFileElementInfoTypeFile:
        {
            printf("| %-4s | %-20ls | %i |\n", "File", pxFileElementInfo->Name, pxFileElementInfo->Size);        

            break;
        }
        case PXFileElementInfoTypeDictionary:
        {
            printf("| %-4s | %-20ls |\n", "DIR", pxFileElementInfo->Name);
            break;
        }

        default:
            break;
    }


}



void PXTextMatchTest()
{
    const PXBool a = PXTextMatchW(L"MyFile.h", 8, L"*.h", 3);
    printf("");

    const PXBool b = PXTextMatchW(L"MyFile.c", 8, L"*.h", 3);
    printf("");

    const PXBool c = PXTextMatchW(L"MyFile.*", 8, L"*.h", 3);
    printf("");
}*/

#include "Compiler/PXTestCompiler.h"
#include "Debug/PXTestDebug.h"
#include "Media/PXMediaTest.h"
#include "Window/PXWindowTest.h"

#include <PX/OS/Hardware/PXHardware.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Time/PXTime.h>
#include <PX/OS/Async/PXProcess.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/BinaryWindows/PXBinaryWindows.h>



#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <PX/OS/Memory/PXMemory.h>

#pragma comment(lib, "pdh.lib")


#include <wbemidl.h>
#include <oleauto.h>
#pragma comment(lib, "wbemuuid.lib")






#include <PX/OS/File/PXDirectory.h>








void PXAPI Trace_File()
{
    PXText pxText;

    PXDirectoryIterator pxDirectoryIterator;

  //  PXConsoleWriteF(0, "File: %s\n", pxTextSubDir.A);
}

void PXAPI Trace_Folder(PXDirectoryIterator* parentDir, PXText* pxText)
{
    
}


void PXAPI Trace_FolderFiles(PXText* pxText)
{
    Trace_Folder(PXNull, pxText);
}


#include <PX/Engine/PXGUI.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/System/Driver/PXDriver.h>
#include <PX/Algorithm/CollatzConjecture/PXCollatzConjecture.h>
#include <PX/OS/Library/PXLibrary.h>
#include <PX/Math/PXMath.h>
#include <PX/Media/PXType.h>
#include <PX/Engine/PXResource.h>


#define VXSize 1024*100

PXF32 calcNumbersIN[VXSize];
PXF32 calcNumbersOUT[VXSize];


#include <PX/OS/PXOS.h>
#include <PX/OS/Network/PXNetwork.h>
#include <PX/Media/PXSound.h>
#include <PX/OS/Audio/Windows/PXDirectSound.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Stream/PXStream.h>

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <math.h>


#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <PX/Media/Bitmap/PXBitmap.h>



int SaveBMP(const char* filename, BYTE* pData, int width, int height, int bbp) 
{
    BITMAPFILEHEADER fileHeader = { 0 };
    BITMAPINFOHEADER infoHeader = { 0 };

    int bytesPerPixel = bbp / 8;

    int rowSize = ((width * 3 + 3) & ~3); // Pad row to multiple of 4 bytes
    int dataSize = rowSize * height;


    int stride = width * bytesPerPixel;
    int padding = (4 - (stride % 4)) % 4;
    int imageSize = (stride + padding) * height;


    fileHeader.bfType = 0x4D42; // "BM"
    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dataSize;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = bbp;
    infoHeader.biCompression = BI_RGB;
    infoHeader.biSizeImage = dataSize;


    

    FILE* fp = fopen(filename, "wb");

    if(!fp) 
        return 0;

    fwrite(&fileHeader, sizeof(fileHeader), 1, fp);
    fwrite(&infoHeader, sizeof(infoHeader), 1, fp);

    if(bbp == 24)
    {
        // BMP stores bottom-up: flip vertically
        for(int y = height - 1; y >= 0; y--) 
        {
            fwrite(pData + y * width * 3, 1, rowSize, fp);
        }
    }
    else
    {
        PXColorRGBAI8* dataUB = (PXColorRGBAI8*)pData;

        for(size_t i = 0; i < width*height; i++)
        {
            PXColorRGBAI8 pixel = dataUB[i];

            fwrite(&pixel.Red, 1, 1, fp);
            fwrite(&pixel.Green, 1, 1, fp);
            fwrite(&pixel.Blue, 1, 1, fp);         
        }

        /*
         if(padding) {
                BYTE pad[3] = { 0, 0, 0 };
                fwrite(pad, padding, 1, fp);
            }
        */
    }

  

    fclose(fp);
    return 1;
}



int i = 0;

PXResult PXAPI  PXStreamOnFrame(PXStream* const pxStream, const PXStreamOnFrameInfo* const pxStreamOnFrameInfo)
{
    // Save raw RGB32 data to file (for testing)
    // FILE* f = fopen("webcam_frame.raw", "wb");
    // fwrite(dataAdress, 1, dataSize, f);
    // fclose(f);

   // void

    char fileBuffer[64];

    PXTextPrintA
    (
        fileBuffer, 
        64,
        "P:\\_IN\\%2.2i___%4.4ix%4.4i___X%i.bmp",
        i,
        pxStreamOnFrameInfo->Width,
        pxStreamOnFrameInfo->Height,
        pxStreamOnFrameInfo->BitPerPixel
    );


    PXTexture PXTexture;
    PXTexture.PixelData = pxStreamOnFrameInfo->DataAdress;
    PXTexture.PixelDataSize = pxStreamOnFrameInfo->DataSize;
    PXTexture.Width = pxStreamOnFrameInfo->Width;
    PXTexture.Height = pxStreamOnFrameInfo->Height;
    PXTexture.Depth = 1;

    if(pxStreamOnFrameInfo->BitPerPixel == 24)
    {
        PXTexture.Format = PXColorFormatRGBI8;
    }
    else
    {
        PXTexture.Format = PXColorFormatRGBAI8;
    }

    PXResourceTransphereInfo pxResourceTransphereInfo;
    PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
    pxResourceTransphereInfo.ResourceType =PXResourceTypeTexture;
    pxResourceTransphereInfo.ResourceTarget = &PXTexture;

#if 1
    PXResourceSaveA(&pxResourceTransphereInfo, fileBuffer);
#else
    SaveBMP
    (
        fileBuffer,
        pxStreamOnFrameInfo->DataAdress,
        pxStreamOnFrameInfo->Width,
        pxStreamOnFrameInfo->Height,
        pxStreamOnFrameInfo->BitPerPixel
    );
#endif
    return PXActionSuccessful;
}










#define NUM_VERTICES 16
const PXVector4F32 PXTesseractVertices[] =
{
    {-1,-1,-1,-1},
    {1,-1,-1,-1},
    {-1,1,-1,-1},
    {1,1,-1,-1},
    {-1,-1,1,-1},
    {1,-1,1,-1},
    {-1,1,1,-1},
    {1,1,1,-1},
    {-1,-1,-1,1},
    {1,-1,-1,1},
    {-1,1,-1,1},
    {1,1,-1,1},
    {-1,-1,1,1},
    {1,-1,1,1},
    {-1,1,1,1},
    {1,1,1,1}
};

#define NUM_TRIANGLES 48
int triangleFaces[NUM_TRIANGLES][3] =
{
    // Cube 1 faces (indices 0–7)
    {0,1,2},{1,3,2}, {4,5,6},{5,7,6},
    {0,1,4},{1,5,4}, {2,3,6},{3,7,6},
    {0,2,4},{2,6,4}, {1,3,5},{3,7,5},

    // Cube 2 faces (indices 8–15)
    {8,9,10},{9,11,10}, {12,13,14},{13,15,14},
    {8,9,12},{9,13,12}, {10,11,14},{11,15,14},
    {8,10,12},{10,14,12}, {9,11,13},{11,15,13},

    // Connections between cubes
    {0,8,1},{1,9,8}, {2,10,3},{3,11,10},
    {4,12,5},{5,13,12}, {6,14,7},{7,15,14},
    {0,8,2},{2,10,8}, {1,9,3},{3,11,9},
    {4,12,6},{6,14,12}, {5,13,7},{7,15,13}
};








// D20 (Icosahedron)
const PXVector4F32 PXD20_vertices[] = {
    -1.000000, 1.618034, 0.000000, 0,
    1.000000, 1.618034, 0.000000, 0,
    -1.000000, -1.618034, 0.000000, 0,
    1.000000, -1.618034, 0.000000, 0,
    0.000000, -1.000000, 1.618034, 0,
    0.000000, 1.000000, 1.618034, 0,
    0.000000, -1.000000, -1.618034, 0,
    0.000000, 1.000000, -1.618034, 0,
    1.618034, 0.000000, -1.000000, 0,
    1.618034, 0.000000, 1.000000, 0,
    -1.618034, 0.000000, -1.000000, 0,
    -1.618034, 0.000000, 1.000000, 0
};
const unsigned char PXD20_indices[] =
{
    0, 11, 5,
    0, 5, 1,
    0, 1, 7,
    0, 7, 10,
    0, 10, 11,
    1, 5, 9,
    5, 11, 4,
    11, 10, 2,
    10, 7, 6,
    7, 1, 8,
    3, 9, 4,
    3, 4, 2,
    3, 2, 6,
    3, 6, 8,
    3, 8, 9,
    4, 9, 5,
    2, 4, 11,
    6, 2, 10,
    8, 6, 7,
    9, 8, 1,
};
const int PXD20_Triangles = sizeof(PXD20_indices) / (3 * sizeof(char));











LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if(msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void setupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 
        32, 
        0,0,0,0,0,0,0,0,0,0,0,0,0,
        16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };
    int format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);
}



float distance = 3.0f;
float y22 = 0;
//PXTesseract pxTesseract;

#define usehypercube 1

void drawHypercube(PXRenderEntity* pxRenderEntity, float angle)
{

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_DITHER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -0.5);

    //pxRenderEntity->Draw();

   // distance -= angle;


    // Create rotation matrices
    PXMatrix4x4F rotXY;
    PXMatrix4x4F rotXW;
    PXMatrix4x4F rotYZ;

    //y22 += angle*0.02;    

    PXMatrix4x4FIdentity(&rotXY);
    PXMatrix4x4FIdentity(&rotXW);
    PXMatrix4x4FIdentity(&rotYZ);
    //  Matrix4x4RotationSet4D(&rotXW, 0, 2, angle);
    // Matrix4x4RotationSet4D(&rotXW, 0, 3, angle);    
     // Matrix4x4RotationSet4D(&rotXY, 0, 1, angle);
#if usehypercube
    PXMatrix4x4FRotationAxisSet(&rotXW, 1, 3, angle);
    PXMatrix4x4FRotationAxisSet(&rotYZ, 0, 2, angle);
#else
    //Matrix4x4RotationSet4D(&rotXW, 0, 3, angle);
    Matrix4x4RotationSet4D(&rotYZ, 1, 2, -angle);
#endif


    PXMatrix4x4F combined;

    PXMatrix4x4FIdentity(&combined);
    PXMatrix4x4FMultiply(&combined, &rotXY);
    PXMatrix4x4FMultiply(&combined, &rotYZ);
    PXMatrix4x4FMultiply(&combined, &rotXW);

    PXVector4F32 lineColor = { 0.15f,0.15f,0.15f,0 };
    PXVector4F32 dotColor = { 1.0f,0,0,0 };


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); // or GL_FRONT



    GLfloat lightPos[] = { 0.0f, 0.0f, 5.0f, 1.0f }; // Positional light
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);


#if usehypercube
    int trianglesToRender = NUM_TRIANGLES; // PXD20_Triangles
#else
    int trianglesToRender = PXD20_Triangles; // PXD20_Triangles
#endif


    // Draw triangle faces
#if 1
    glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_TRIANGLES);
    int oneCube = NUM_TRIANGLES / 3;

    for(int i = 0; i < trianglesToRender; ++i)
    {
#if usehypercube
        PXVector4F32 vA = PXTesseractVertices[triangleFaces[i][0]];
        PXVector4F32 vB = PXTesseractVertices[triangleFaces[i][1]];
        PXVector4F32 vC = PXTesseractVertices[triangleFaces[i][2]];
#else
        Vec4 vA = PXD20_vertices[PXD20_indices[i * 3 + 0]];
        Vec4 vB = PXD20_vertices[PXD20_indices[i * 3 + 1]];
        Vec4 vC = PXD20_vertices[PXD20_indices[i * 3 + 2]];
#endif

        PXVector3F32 n;

        PXVector4FNormal(&n, &vA, &vB, &vC, distance);

        glNormal3f(n.X, n.Y, n.Z);

        for(int j = 0; j < 3; ++j)
        {
#if usehypercube
            PXVector4F32 vOrg = PXTesseractVertices[triangleFaces[i][j]];
#else
            Vec4 vOrg = PXD20_vertices[PXD20_indices[i * 3 + j]];
#endif


            PXVector4F32 v = vOrg;
            PXVector3F32 p;


            PXMatrix4x4FMultiplyV4F(&combined, &v);
            PXVector4Demote(&v, &p, distance);

            // Color based on w (depth cue)
            float color = (v.W + 1.0f) / 2.0f;


            PXVector4F32 ccolor = (vOrg);
            ccolor.X = 1 - (ccolor.X + color) / 2.0f;
            ccolor.Y = 1 - (ccolor.Y + color) / 2.0f;
            ccolor.Z = 1 - (ccolor.Z + color) / 2.0f;
            ccolor.W = 0.6;

            glColor4f(ccolor.X, ccolor.Y, ccolor.Z, ccolor.W);

            // glVertex4f();

            glVertex3f(p.X, p.Y, p.Z);
        }
    }
    glEnd();

    glDepthMask(GL_TRUE);
#endif






    // Draw triangle faces
#if 0
   // glDisable(GL_BLEND);
    //glDepthMask(GL_FALSE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_LINES);

    for(int i = 0; i < trianglesToRender; ++i)
    {
#if usehypercube
        Vec4 vA = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][0]]);
        Vec4 vB = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][1]]);
        Vec4 vC = Vec4bTo4f(PXTesseractVertices[triangleFaces[i][2]]);
#else
        Vec4 vA = PXD20_vertices[PXD20_indices[i * 3 + 0]];
        Vec4 vB = PXD20_vertices[PXD20_indices[i * 3 + 1]];
        Vec4 vC = PXD20_vertices[PXD20_indices[i * 3 + 2]];
#endif

        Vec3 vProj3DA = project4Dto3D(multiplyMatrixVec4(combined, vA), distance);
        Vec3 vProj3DB = project4Dto3D(multiplyMatrixVec4(combined, vB), distance);
        Vec3 vProj3DC = project4Dto3D(multiplyMatrixVec4(combined, vC), distance);

        Vec3 normalStart = computeNormal
        (
            vProj3DA,
            vProj3DB,
            vProj3DC
        );

        float scale = 0.05f; // Adjust length of the normal line


        Vec3 surfacePosition = Vec3TriangleCenter(&vProj3DA, &vProj3DB, &vProj3DC);


        Vec3 normalDirection;
        normalDirection.x = surfacePosition.x + normalStart.x * scale;
        normalDirection.y = surfacePosition.y + normalStart.y * scale;
        normalDirection.z = surfacePosition.z + normalStart.z * scale;

        //glColor4f(1.0, 0, 1.0, 1.0);
        //glVertex3f(vProj3DA.x, vProj3DA.y, vProj3DA.z);
        //glVertex3f(vProj3DB.x, vProj3DB.y, vProj3DB.z);
        //glVertex3f(vProj3DC.x, vProj3DC.y, vProj3DC.z);

        glColor4f(0.0, 1.0, 0, 1.0);
        glVertex3f(surfacePosition.x, surfacePosition.y, surfacePosition.z);
        glColor4f(1.0, 1.0, 0, 1.0);
        glVertex3f(normalDirection.x, normalDirection.y, normalDirection.z);
    }
    glEnd();
#endif













#if 1
    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2);
    glPointSize(10.0f);
    glBegin(GL_TRIANGLES);

    glColor4f(lineColor.X, lineColor.Y, lineColor.Z, 1.0f);
    //glColor4f(dotColor.x, dotColor.y, dotColor.z, 1.0f);

    for(int i = 0; i < trianglesToRender; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {

            PXVector4F32 vOrg =
#if usehypercube
                PXTesseractVertices[triangleFaces[i][j]];
#else
                PXD20_vertices[PXD20_indices[i * 3 + j]];
#endif

            PXVector4F32 v;
            PXVector3F32 p;

            v = vOrg;
            
            PXMatrix4x4FMultiplyV4F(&combined, &v);
            PXVector4Demote(&v, &p, distance);

            glVertex3f(p.X, p.Y, p.Z);
        }
    }
    glEnd();

#else
    // glBegin(GL_LINES);
    for(int i = 0; i < NUM_EDGES; ++i) {
        // Vec4 v1 = vertices[edges[i][0]];
         //Vec4 v2 = vertices[edges[i][1]];

         //float color = i / (float)NUM_EDGES;

        Vec4 v1 = multiplyMatrixVec4(combined, Vec4bTo4f(PXTesseractVertices[edges[i][0]]));
        Vec4 v2 = multiplyMatrixVec4(combined, Vec4bTo4f(PXTesseractVertices[edges[i][1]]));

        Vec3 p1 = project4Dto3D(v1, distance);
        Vec3 p2 = project4Dto3D(v2, distance);


#if 0
        rotate4D(&v1, angle, 0, 3); // X-W
        rotate4D(&v1, angle, 1, 2); // Y-Z
        rotate4D(&v2, angle, 0, 3);
        rotate4D(&v2, angle, 1, 2);

        Vec3 p1 = project4Dto3D(v1, 4.0f);
        Vec3 p2 = project4Dto3D(v2, 4.0f);
#endif

        float color = (v1.w + 1.0f) / 2.0f; // Normalize w from [-1,1] to [0,1]
        // glColor3f(color, 0.5f, 1.0f - color);
         //glColor3f(0.2, 0.2f, 0.2f);
        glColor3f(lineColor.x, lineColor.y, lineColor.z);

        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
    }

    glEnd();
#endif


#if 0
    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i < NUM_VERTICES; ++i)
    {

        Vec4 v = multiplyMatrixVec4(combined, Vec4bTo4f(PXTesseractVertices[i]));
        Vec3 p = project4Dto3D(v, distance);

        glColor3f(dotColor.x, dotColor.y, dotColor.z);

        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
#endif
}


void TEST_OPENGL_1x1()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);


    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a window that's 90% of the screen size
    int windowWidth = (int)(screenWidth * 0.9);
    int windowHeight = (int)(screenHeight * 0.9);

    // Center the window
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;




    WNDCLASSA wc = { CS_OWNDC, WndProc, 0, 0, hInstance, NULL, NULL, NULL, NULL, "4DWindow" };
    RegisterClassA(&wc);
    HWND hWnd = CreateWindowA("4DWindow", "4D Hypercube", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                              windowX, windowY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    HDC hdc = GetDC(hWnd);
    setupPixelFormat(hdc);
    HGLRC hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    PXMatrix4x4F perspecive;
    PXMatrix4x4FPerspective(&perspecive, 60.0, (float)windowWidth / (float)windowHeight, 0.1, 100.0);

    glLoadMatrixf(perspecive.Data);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //initHypercube();

    MSG msg;
    float angle = 0.0f;
    while(1) {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) goto exit;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        angle += 0.01f;
        drawHypercube(0, angle);
        SwapBuffers(hdc);
        Sleep(16);
    }

exit:
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hWnd, hdc);
}


int main()
{ 

















    TEST_OPENGL_1x1();





    PXGPUList(0, 0);


    PXStream pxStream;

    for(i = 0; i < 27; i++)
    {
        PXStreamOpenInfo pxStreamOpenInfo;
        pxStreamOpenInfo.Mode = PXStreamModeWindow;
        pxStreamOpenInfo.DeviceIndex = i;
        pxStreamOpenInfo.OnFrameCallBack = PXStreamOnFrame;

        PXStreamOpen(&pxStream, &pxStreamOpenInfo);

       // while(0)
        {
            pxStream.Update(&pxStream);
            Sleep(250);
        }

    }


    Sleep(0);








#define SAMPLE_RATE 44100
#define DURATION_SECONDS 4
#define BUFFER_SIZE (SAMPLE_RATE * DURATION_SECONDS)

  

   // void InitDirectSound(HWND hwnd) 

  //  GetWind

    PXAudioDevice pxAudioDevice;

    PXDirectSoundInitialize(PXNull, PXNull);

    PXDirectSoundDeviceOpen(&pxAudioDevice, PXAudioDeviceTypeOutput, 0);
   

    PXSound pxSound;
    PXClear(PXSound, &pxSound);
    pxSound.SampleRate = SAMPLE_RATE;
    pxSound.ByteRate = SAMPLE_RATE * 2;  
    pxSound.NumerOfChannels = 1;
    pxSound.BlockAllign = 2;
    pxSound.BitsPerSample = 16;
    pxSound.DataSize;
    pxSound.ChunkSize;
    pxSound.AudioFormat;

    // gen sound

    pxSound.DataSize = BUFFER_SIZE * 2;
    pxSound.Data = PXMemoryHeapCalloc(PXNull, pxSound.DataSize, 1);


   // PXAudioWaveGenerate();
    //FillBufferWithWeather(pxSound.Data, BUFFER_SIZE, 0.2, 0.8, 0.0);

    PXSFXParams pxSFXParams;


    PXSFXParams jump;
    jump.WaveType = PXAudioWaveTypeTRIANGLE;
    jump.base_freq = 155.0f;
    jump.freq_slide = 8.0f;
    jump.vibrato_speed = 1.8f;
    jump.vibrato_depth = 5.8f;
    jump.attack = 0.025f;
    jump.sustain = 0.5f;
    jump.decay = 0.02f;
    jump.volume = 1.0f;
    jump.PI = PXMathConstantPI;
    jump.SampleRate = SAMPLE_RATE;

    generate_sfx(pxSound.Data, BUFFER_SIZE, &jump);


    short* soundData = (short*)pxSound.Data;
    
    PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
    pxAudioWaveGenerateInfo.WaveType = PXAudioWaveTypeSQUARE;
    pxAudioWaveGenerateInfo.Hz = 20.0f;
    pxAudioWaveGenerateInfo.PWM = 0.2f;
    pxAudioWaveGenerateInfo.Amplitude = 1.0f;

    for(size_t i = 0; i < BUFFER_SIZE; i++)
    {
        pxAudioWaveGenerateInfo.X = (float)i / SAMPLE_RATE;

        //float sample = 0.3f * sinf(2.0f * PXMathConstantPI * pxAudioWaveGenerateInfo.Phase * pxAudioWaveGenerateInfo.T);
        
        //buffer[i] = (int16_t)(sample * 32767);
        //soundData[i] = (short)(sample * (0xFFFF / 2));

       // soundData[i] = (short)(PXAudioWaveGenerate(&pxAudioWaveGenerateInfo) * (0xFFFF / 2));
    }

    for(size_t i = 0; i < BUFFER_SIZE; i++)
    {
        //soundData[i] = PXAudioWaveGenerate(&pxAudioWaveGenerateInfo) * (0xFFFF / 2);
    }


    FILE* fileHandle = fopen("C:\\Users\\BitPaw\\Downloads\\TEST.csv", "wb");

 

    for(size_t i = 0; i < BUFFER_SIZE/4; i++)
    {
        fprintf(fileHandle, "%i\n", soundData[i]);
    }

    fclose(fileHandle);

    PXDirectSoundDeviceBufferCreate(&pxAudioDevice, &pxSound);



    while(1)
    {
        PXSoundDeviceProperty pxSoundDeviceProperty;
        pxSoundDeviceProperty.Type = PXSoundDevicePropertyStatePlay;

        PXDirectSoundDeviceProperty(&pxAudioDevice, &pxSoundDeviceProperty);

        Sleep(100);
    }

  
    

    /*
    DSBUFFERDESC desc = {
        sizeof(DSBUFFERDESC), DSBCAPS_GLOBALFOCUS, BUFFER_SIZE * 2,
        0, &format, 0
    };

    dsound->lpVtbl->CreateSoundBuffer(dsound, &desc, &buffer, NULL);

    void* audioPtr;
    DWORD audioBytes;
    buffer->lpVtbl->Lock(buffer, 0, BUFFER_SIZE * 2, &audioPtr, &audioBytes, NULL, NULL, 0);

    short* samples = (short*)audioPtr;
    //FillBufferWithElectricHum(SAMPLE_RATE, samples, BUFFER_SIZE, 50.0f);  // 60Hz hum
    FillBufferWithWeather(samples, BUFFER_SIZE, 0.2, 0.8,0.0);

    buffer->lpVtbl->Unlock(buffer, audioPtr, audioBytes, NULL, 0);
    buffer->lpVtbl->Play(buffer, 0, 0, DSBPLAY_LOOPING);
    */

    while(1)
    {
        Sleep(10);
    }






    // USP server test
    PXSocket server;

    PXNetworkModulState(PXNull, PXModuleStateDoInitialize);

    PXSocketCreateInfo pxSocketCreateInfo;
    pxSocketCreateInfo.SocketReference = &server;
    pxSocketCreateInfo.AdressFamily = IPAdressFamilyINET;
    pxSocketCreateInfo.Type = PXSocketTypeDatagram;
    pxSocketCreateInfo.ProtocolMode = PXProtocolModeUDP;

    PXNetworkSocketCreate(&pxSocketCreateInfo);


    PXSocketBindInfo pxSocketBindInfo;
    pxSocketBindInfo.SocketReference = &server;
    pxSocketBindInfo.Port = 8000;
    pxSocketBindInfo.IP = PXNull;

    PXNetworkSocketBind(&pxSocketBindInfo);

    while(1)
    {
        char message[128];
        PXSocket SocketPeer;

        PXSocketDataInfo pxSocketDataInfo;
        PXClear(PXSocketDataInfo, &pxSocketDataInfo);
        pxSocketDataInfo.SocketIDCurrent = server.ID;
        pxSocketDataInfo.Buffer = message;
        pxSocketDataInfo.BufferSize = 128; 
        pxSocketDataInfo.BufferUsed = 0;
        pxSocketDataInfo.SocketPeer = &SocketPeer;

        PXNetworkSocketReceive(&pxSocketDataInfo);
        Sleep(10);
    }













    PXMAC pxMAC;

    char nameWhoIs[1024];

    PXNetworkAdapterFetch();
    PXNetworkNameFromIPv4A(nameWhoIs, "192.168.0.246");

    PXNetworkMACFromIPv4A(&pxMAC, "192.168.0.246");

    PXNetworkMACFromIPv6A(&pxMAC, "2a02:908:f42:fa00:b628:6934:dd08:6933");
 


    PXSystemPrelude();

    PXDebug* pxDebug = PXDebugInstanceGet();

    PXConsoleWrite(0, "[i] Starting testing...\n");



    {
        PXModel pxModel;
        PXClear(PXBinaryWindows, &pxModel);

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceTarget = &pxModel;

    
       // PXResourceLoadA(&pxResourceTransphereInfo, "H:\\[Cache]\\FINAL\\Champions\\Yuumi.wad.client");

       // PXResourceLoadA(&pxResourceTransphereInfo, "H:\\[Cache]\\FINAL\\Champions\\MasterYi.wad.client");
       // PXResourceLoadA(&pxResourceTransphereInfo, "H:\\[Cache]\\FINAL\\Champions\\MasterYi.en_US.wad.client");
       //PXResourceLoadA(&pxResourceTransphereInfo, "H:/[Cache]/FINAL/Maps/Shipping/Map11.wad.client");
       PXResourceLoadA(&pxResourceTransphereInfo, "C:/Data/Map11.wad.client");

        //PXResourceLoadA(&pxResourceTransphereInfo, "H:\\[Cache]\\FINAL\\Global.wad.client");


        PXConsoleWrite(0, "EE");
    }

    return 0;
    



    PXI8U bufferA[64] = {'A', 'A' , 'A' };
    PXI8U bufferB[64];

    for(size_t i = 0; i < 64; i+=3)
    {
        bufferA[i + 0] = 'B';
        bufferA[i + 1] = 'G';
        bufferA[i + 2] = 'R';
    }

    PXI8U order[] = { 2,1,0 };


    PXMathShuffleI8(bufferA, bufferB, 64, order, 3);

    PXConsoleWrite(0, "[i] Starting testing...\n");


#if 0
    {
        PXBinaryWindows bin;
        PXClear(PXBinaryWindows, &bin);

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceTarget = &bin;

        PXResourceLoadA(&pxResourceTransphereInfo, "C:\\Windows\\System32\\calc.exe");
    }
#endif

 

    {
        PXModel pxModel;
        PXClear(PXBinaryWindows, &pxModel);

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceTarget = &pxModel;

        PXResourceLoadA(&pxResourceTransphereInfo, "P:\\_Cache\\Kitchen_set\\Kitchen_set.usd");
    }





    {
        PXModel pxModel;
        PXClear(PXBinaryWindows, &pxModel);

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceTarget = &pxModel;

        PXResourceLoadA(&pxResourceTransphereInfo, "P:\\_Cache\\test.fbx");
    }



    PXConsoleWrite(0, "eeeeeee\n");












    int amount = 100000;

    for(size_t i = 0; i < VXSize; i++)
    {
        calcNumbersIN[i] = i*00.25f;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "EEE",
        "AA",
        "Amount: %i",
        amount
    );


    //--------------
    {



    PXI64U timeStart = PXTimeCounterStampGet();

    for(size_t i = 0; i < amount; i++)
    {
       PXMathCosinusDEGF32VX16(calcNumbersOUT, calcNumbersIN, VXSize);
    }

    PXI64U timeStop = PXTimeCounterStampGet();

    PXI64U timeDelta = timeStop - timeStart;

    PXF32 timeTaken =  PXTimeCounterStampToSecoundsF(timeDelta);
   

    PXLogPrint
    (
        PXLoggingInfo,
        "EEE",
        "AA",
        "AVX: %fs",
        timeTaken
    );
    }
    //--------------



       //--------------
    {

        for(size_t i = 0; i < VXSize; i++)
        {
            calcNumbersIN[i] = i * 00.25f;
        }


        PXI64U timeStart = PXTimeCounterStampGet();

        for(size_t i = 0; i < amount; i++)
        {
            PXMathCosinusDEGF32V(calcNumbersOUT, calcNumbersIN, VXSize);
        }

        PXI64U timeStop = PXTimeCounterStampGet();

        PXI64U timeDelta = timeStop - timeStart;

        PXF32 timeTaken = PXTimeCounterStampToSecoundsF(timeDelta);


        PXLogPrint
        (
            PXLoggingInfo,
            "EEE",
            "AA",
            "COS: %fs",
            timeTaken
        );
    }
    //--------------








    PXLibraryCurrentlyLoaded(PXNull, 0, 0);

    while(1)
    {
        Sleep(1000);
    }



    /*

    PXDotNet pxDotNet;

    PXDotNetCompile(&pxDotNet);

    PXActionResult pxActionResult = PXDotNetInitialize(&pxDotNet, 0);

    PXDelegate pxDelegate;
    pxDelegate.NameLibrary = "PXTestDLLRelay.dll";
    pxDelegate.NameNamespace = "PX";
    pxDelegate.NameClass = "TESTClass";
    pxDelegate.NameFunction = "WONKMain";
    pxDelegate.FunctionAdress = 0;



    pxDotNet.DelegateFetch(pxDotNet.API, &pxDelegate);
    */








#if 0 
    PXI16U data[200];

    PXSize fgdgfdgfd = PXCollatzConjectureGenerate16(data, 200, 12);

    for(size_t i = 0; i < fgdgfdgfd; i++)
    {
        PXConsoleWriteF(0, "%3i\n", data[i]);
    }
#endif


    {
        PXBinaryWindows pxBinaryWindows;
        PXClear(PXBinaryWindows, &pxBinaryWindows);


        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceTarget = &pxBinaryWindows;

        PXResourceLoadA(&pxResourceTransphereInfo, "C:\\Data\\WorkSpace\\[GIT]\\BitFireEngine\\[Export]\\BitFireEngineIDE\\64B-Windows-Debug\\BitFireEngineIDE.exe");


    }
















    PXProcessorTemperatureInfo pxProcessorTemperatureInfo;
    PXClear(PXProcessorTemperatureInfo, &pxProcessorTemperatureInfo);

    for(;;)
    {
        PXProcessorTemperature(&pxProcessorTemperatureInfo);
    }


    PXSize listSize = 0;
    PXDriver* pxDriver = PXNull;
    PXDriverListFetch(&pxDriver, &listSize);




    PXNativDraw pxNativDraw;
    PXClear(PXNativDraw, &pxNativDraw);

    PXNativDrawDisplayListFetch(&pxNativDraw);
    


    //PXGUIIconGetViaExtension("vob");



    PXText pxTextSubDir;
    PXTextMakeFixedA(&pxTextSubDir, "N:\\NAS\\");

    PXDirectorySearchCache pxDirectorySearchCache;

    PXDirectorySearch(&pxDirectorySearchCache, &pxTextSubDir);

    Trace_FolderFiles(&pxTextSubDir);


#if 000000000000000000000000















    /*

    HRESULT hres;

    // Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if(FAILED(hres)) {
        printf("Failed to initialize COM library. Error code = 0x%x\n", hres);
        return 1; // Program has failed.
    }

    // Set general COM security levels
    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities
        NULL                         // Reserved
    );

    if(FAILED(hres)) {
        printf("Failed to initialize security. Error code = 0x%x\n", hres);
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemAdministrativeLocator, //CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    if(FAILED(hres)) {
        printf("Failed to create IWbemLocator object. Error code = 0x%x\n", hres);
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Connect to WMI through the IWbemLocator::ConnectServer method
    IWbemServices* pSvc = NULL;

    hres = pLoc->ConnectServer(
        L"ROOT\\CIMV2", // WMI namespace
        NULL,                    // User name
        NULL,                    // User password
        0,                       // Locale
        WBEM_FLAG_CONNECT_USE_MAX_WAIT,                    // Security flags
        0,                       // Authority
        0,                       // Context object
        &pSvc                    // IWbemServices proxy
    );

    if(FAILED(hres)) {
        printf("Could not connect. Error code = 0x%x\n", hres);
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    printf("Connected to ROOT\\CIMV2 WMI namespace\n");

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // Client identity
        EOAC_NONE                    // Proxy capabilities
    );

    if(FAILED(hres)) {
        printf("Could not set proxy blanket. Error code = 0x%x\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    IEnumWbemClassObject* enumWbemClassObject = PXNull;

    HRESULT SS = pSvc->CreateClassEnum
    (
        PXNull,
        WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
        PXNull,
        &enumWbemClassObject
    );


    {

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        HRESULT eeg = 0;

        do
        {
            eeg = enumWbemClassObject->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            VARIANT vtProp;
            PXClear(VARIANT, &vtProp);

            BSTR name = 0;

            // Get the value of the CurrentReading property
            HRESULT hr = pclsObj->Next(PXNull, &name, &vtProp, PXNull, PXNull);

            PXActionResult xxasew = PXWindowsErrorCurrent(0);

                printf("");

        } while(eeg);

       


    }




    // Use the IWbemServices pointer to make requests of WMI
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        TEXT("WQL"),
        TEXT("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY,
        NULL,
        &pEnumerator);

    if(FAILED(hres)) {
        printf("Query for temperature probe failed. Error code = 0x%x\n", hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1; // Program has failed.
    }

    // Get the data from the query
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while(pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        PXActionResult xxasew = PXWindowsErrorCurrent(0);

        if(0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        PXClear(VARIANT, &vtProp);

        // Get the value of the CurrentReading property
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if(SUCCEEDED(hr)) {

            switch(vtProp.vt)
            {
                case VT_BSTR:
                {
                    printf("- %ls\n", vtProp.bstrVal);
                    break;
                }

                case VT_INT:
                {
                    double tempCelsius = ((double)vtProp.intVal / 10.0) - 273.15;
                    printf("CPU Temperature: %.2f°C\n", tempCelsius);
                    break;
                }


                default:
                    break;
            }

       
        }
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();












    */









    CONST ULONG SAMPLE_INTERVAL_MS = 1000;
    CONST PWSTR BROWSE_DIALOG_CAPTION = L"Select a counter to monitor.";




    PDH_STATUS Status;
    HQUERY Query = NULL;
    HCOUNTER Counter;
    PDH_FMT_COUNTERVALUE DisplayValue;
    DWORD CounterType;
    SYSTEMTIME SampleTime;
    PDH_BROWSE_DLG_CONFIG BrowseDlgData;
    WCHAR CounterPathBuffer[512];

    //
    // Create a query.
    //

    Status = PdhOpenQuery(NULL, NULL, &Query);

    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhOpenQuery failed with status 0x%x.", Status);
    }

#if 0



    //
    // Initialize the browser dialog window settings.
    //

    ZeroMemory(&CounterPathBuffer, sizeof(CounterPathBuffer));
    ZeroMemory(&BrowseDlgData, sizeof(PDH_BROWSE_DLG_CONFIG));

    BrowseDlgData.bIncludedInstanceIndex = 0;
    BrowseDlgData.bSingleCounterPerAdd = 0;
    BrowseDlgData.bSingleCounterPerDialog = 0;
    BrowseDlgData.bLocalCountersOnly = 0;
    BrowseDlgData.bWildCardInstances = 0;
    BrowseDlgData.bHideDetailBox = 0;
    BrowseDlgData.bInitializePath = 0;
    BrowseDlgData.bDisableMachineSelection = 0;
    BrowseDlgData.bIncludedCostlyObjects = 0;
    BrowseDlgData.bShowObjectBrowser = 0;
    BrowseDlgData.hWndOwner = NULL;
    BrowseDlgData.szReturnPathBuffer = CounterPathBuffer;
    BrowseDlgData.cchReturnPathLength = 512;
    BrowseDlgData.pCallBack = NULL;
    BrowseDlgData.dwCallBackArg = 0;
    BrowseDlgData.CallBackStatus = ERROR_SUCCESS;
    BrowseDlgData.dwDefaultDetailLevel = PERF_DETAIL_WIZARD;
    BrowseDlgData.szDialogBoxCaption = BROWSE_DIALOG_CAPTION;

    //
    // Display the counter browser window. The dialog is configured
    // to return a single selection from the counter list.
    //

    Status = PdhBrowseCounters(&BrowseDlgData);


    if(Status != ERROR_SUCCESS)
    {
        if(Status == PDH_DIALOG_CANCELLED)
        {
            wprintf(L"\nDialog canceled by user. %ls", CounterPathBuffer);
        }
        else
        {
            wprintf(L"\nPdhBrowseCounters failed with status 0x%x.", Status);
        }
    }
    else if(wcslen(CounterPathBuffer) == 0)
    {
        wprintf(L"\nUser did not select any counter.");

        //wsprintf(CounterPathBuffer, "\\ProcessorInfo");
    }
    else
    {
        wprintf(L"\nCounter selected: %s\n", CounterPathBuffer);
    }

    //
    // Add the selected counter to the query.
    //

    Status = PdhAddCounter(Query, CounterPathBuffer, 0, &Counter);
    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhAddCounter failed with status 0x%x.", Status);
    }

    //
    // Most counters require two sample values to display a formatted value.
    // PDH stores the current sample value and the previously collected
    // sample value. This call retrieves the first value that will be used
    // by PdhGetFormattedCounterValue in the first iteration of the loop
    // Note that this value is lost if the counter does not require two
    // values to compute a displayable value.
    //

    Status = PdhCollectQueryData(Query);
    if(Status != ERROR_SUCCESS)
    {
        wprintf(L"\nPdhCollectQueryData failed with 0x%x.\n", Status);
    }

    //
    // Print counter values until a key is pressed.
    //

    while(!_kbhit())
    {
        Sleep(SAMPLE_INTERVAL_MS);

        GetLocalTime(&SampleTime);

        Status = PdhCollectQueryData(Query);
        if(Status != ERROR_SUCCESS)
        {
            wprintf(L"\nPdhCollectQueryData failed with status 0x%x.", Status);
        }

        wprintf(L"\n\"%2.2d/%2.2d/%4.4d %2.2d:%2.2d:%2.2d.%3.3d\"",
                SampleTime.wMonth,
                SampleTime.wDay,
                SampleTime.wYear,
                SampleTime.wHour,
                SampleTime.wMinute,
                SampleTime.wSecond,
                SampleTime.wMilliseconds);

        //
        // Compute a displayable value for the counter.
        //

        Status = PdhGetFormattedCounterValue(Counter,
                                             PDH_FMT_LONG,
                                             &CounterType,
                                             &DisplayValue);
        if(Status != ERROR_SUCCESS)
        {
            wprintf(L"\nPdhGetFormattedCounterValue failed with status 0x%x.", Status);
 
        }

        wprintf(L",\"%5d\"", DisplayValue.longValue);
    }


#elif 0




char* buffer = (char*)malloc(256);
DWORD bufferSIze = 256;

auto asas = PdhEnumObjectsA
(
    PXNull,
    PXNull,
    buffer,
    &bufferSIze,
    PERF_DETAIL_WIZARD,
    TRUE
);

buffer = (char*)malloc(bufferSIze);
PXClearList(char, buffer, bufferSIze);

asas = PdhEnumObjectsA
(
    PXNull,
    PXNull,
    buffer,
    &bufferSIze,
    PERF_DETAIL_WIZARD,
    TRUE
);


char* stringCurrent = buffer;

for(size_t i = 0; ; ++i)
{
    if(!*stringCurrent)
    {
        break;
    }

    PXSize size = PXTextLengthA(stringCurrent, 512);

    printf("%3i - %s\n", i, stringCurrent);

 


    char* mszCounterList = 0;
    DWORD mszCounterListSize = 0;
    char* mszInstanceList = 0;
    DWORD mszInstanceListSize = 0;

    auto xas = PdhEnumObjectItemsA
    (
        PXNull,
        PXNull,
        stringCurrent,
        mszCounterList,
        &mszCounterListSize,
        mszInstanceList,
        &mszInstanceListSize,
        PERF_DETAIL_WIZARD,
        TRUE
    );

    mszCounterList = (char*)malloc(mszCounterListSize);
    PXClearList(char, mszCounterList, mszCounterListSize);

    mszInstanceList = (char*)malloc(mszInstanceListSize);
    PXClearList(char, mszInstanceList, mszInstanceListSize);

    xas = PdhEnumObjectItemsA
    (
        PXNull,
        PXNull,
        stringCurrent,
        mszCounterList,
        &mszCounterListSize,
        mszInstanceList,
        &mszInstanceListSize,
        PERF_DETAIL_WIZARD,
        TRUE
    );


    char* stringCuursorAA = mszCounterList;

    for(size_t w = 0; ; w++)
    {
        if(!*stringCuursorAA)
        {
            break;
        }

        PXSize sizeww = PXTextLengthA(stringCuursorAA, 512);



        printf("         %s\n", stringCuursorAA);


        stringCuursorAA += sizeww + 1;
    }



    stringCurrent += size + 1;
}





// Add the CPU usage counter to the query
wchar_t text[] = L"\\System\\System Calls/sec";
//wchar_t text[] = L"\\Processor(_Total)\\% Processor Time";

if(PdhAddCounter(Query, text, 0, &Counter) != ERROR_SUCCESS) {
    fprintf(stderr, "PdhAddCounter failed\n");
    PdhCloseQuery(Query);
    return 1;
}

while(1)
{
    // Collect the initial data sample
    if(PdhCollectQueryData(Query) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhCollectQueryData failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    // Wait for a second to collect the next sample
    Sleep(1000);

    // Collect the next data sample and get the formatted counter value
    if(PdhCollectQueryData(Query) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhCollectQueryData failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    if(PdhGetFormattedCounterValue(Counter, PDH_FMT_LARGE, NULL, &DisplayValue) != ERROR_SUCCESS) {
        fprintf(stderr, "PdhGetFormattedCounterValue failed\n");
        PdhCloseQuery(Query);
        return 1;
    }

    // Print the CPU usage percentage
    printf("CPU Usage: %li\n", DisplayValue.largeValue);
}





#endif
























    PXTestNetworkAll();




    PXFileOpenInfo pxFileOpenInfo;
    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
    pxFileOpenInfo.FilePathAdress = "TEST.txt";
    pxFileOpenInfo.FlagList = PXFileIOInfoFilePhysical | PXFileIOInfoAllowMapping;
    pxFileOpenInfo.FileSizeRequest = 420;
    pxFileOpenInfo.AccessMode = PXAccessModeWriteOnly;

    PXFile pxFile;    
    PXFileOpen(&pxFile, &pxFileOpenInfo);

    for(size_t i = 0; i < pxFileOpenInfo.FileSizeRequest; ++i)
    {
        PXByte x = i % 0xFF;

        PXFileWriteB(&pxFile, &x, 1);
    }

    PXFileClose(&pxFile);










    PXBinaryWindows pxBinaryWindows;

    PXResourceTransphereInfo pxResourceTransphereInfo;
    PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
    pxResourceTransphereInfo.ResourceTarget = &pxBinaryWindows;

    PXResourceLoadA(&pxResourceTransphereInfo, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\[Export]\\PXUltimaCTest\\64B-Windows-Debug\\D.zip");












    int* dsfdsf = 0;// PXMemoryHeapMallocT(int, 420);

    PXThread* threads = 0;// PXMemoryHeapMallocT(PXThread, 1234);
    const PXSize xx = 1234;
    PXSize oeoeo = 0;


    PXThread* refA = threads;
    PXThread** refB = &refA;

   // HeapAlloc();


    PXProcessThreadsListAll(PXNull, refB, xx, &oeoeo);


    PXF32 percent = 0;

    for(size_t i = 0; i < 60000; i++)
    {
        const DWORD max = 0x7FFF;
        const DWORD min = 0x25;
    
        percent += 0.0001;

        DWORD hz = (percent * max) + min;  // 0x25 through 0x7FFF)



        Beep(hz, 100);
        PXConsoleWriteF(0, "[Beep] %i\n", hz);
    }

    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONERROR);
    MessageBeep(MB_ICONHAND);
    MessageBeep(MB_ICONINFORMATION);
    MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_ICONSTOP);
    MessageBeep(MB_ICONWARNING);
    MessageBeep(MB_OK);


    const PXSize amount = 100000000;
    const PXF32 res = 1/25.0f;
    
    PXConsoleWriteF(0, "[%3i] --- Start ---\n", amount);
    
    PXI64U startTime = PXTimeCounterStampGet();

    for(PXSize i = 0; i < amount; i++)
    {
        PXF32 input = i * res;
        PXF32 output = PXMathSinusRADF32(input);

        //PXConsoleWriteF(0, "[%3i] %6.4f -> %6.4f\n", i, input, output);
    }

    PXI64U endTime = PXTimeCounterStampGet();
    PXI64U delta = endTime - startTime;
    PXF32 endTimeInS = PXTimeCounterStampToSecoundsF(delta);

    PXF32 avg = delta / (PXF32)amount;

    PXConsoleWriteF(0, "[%3i] Took: %6.3fs. AVG: %6.3fus\n", amount, endTimeInS, avg);




    PXProcessor pxProcessor;

    PXProcessorFetchInfo(&pxProcessor);

    PXVideoCaptureDeviceList();





    
    PXResourceTransphereInfo pxResourceLoadInfo;
    pxResourceLoadInfo.Manager = 0;
   // pxResourceLoadInfo.Target = 0;
    pxResourceLoadInfo.FileReference = PXNull;
   // pxResourceLoadInfo.Type = 0;


    PXResourceLoadA(&pxResourceLoadInfo, "H:\\S.n64");


#if 0
    PXHardwareInfo pxHardwareInfo;
    PXHardwareInfoScan(&pxHardwareInfo, PXHardwareInfoAll);

    while(1)
    {
        PXI32S temperature = 0;



        //PXProcessorTemperature(&temperature);


    }
#endif

    PXTestCompilerAll();

    PXTestWindowAll();
    PXMediaTestAll();
    
    PXTestDebugAll();






#if 0

    PXResourceLoadInfo pxResourceLoadInfo;
    pxResourceLoadInfo.Target = 0;
    pxResourceLoadInfo.FileReference = 0;
    pxResourceLoadInfo.Type = PXGraphicResourceTypeInvalid;


    PXResourceLoadA(&pxResourceLoadInfo, "N:\\NAS\\Games\\Steam\\steamapps\\common\\Call of Duty Black Ops II\\zone\\all\\afghanistan.ff");

#endif // 1



#if 0
    PXTestWindowAll();
#endif // 0

#if 0
    PXTestNetworkAll();
#endif // 0






#if 0
    {
        PXBinaryWindows binaryWindows;

        const PXActionResult result = PXResourceLoadA(&binaryWindows, "C:\\Data\\WorkSpace\\[GIT]\\BitFireEngine\\[Export]\\GameCleaved\\32B-Windows-Release\\GameCleaved2K.exe");

        printf("\n");
    }
#endif // 1




#if 0
    {
        PXTexture PXTexture;
        PXClear(PXTexture, &PXTexture);

        const PXActionResult pxLoadResult = PXResourceLoadA(&PXTexture, "_TEST_DATA_INPUT_\\ImageInput.bmp");
        const PXActionResult pxSaveResult = PXResourceSaveA(&PXTexture, "_TEST_DATA_INPUT_\\ImageInput_COPY.bmp", PXFileFormatBitMap);

        printf("\n");
    }
#endif // 1


#if 0 // XML -> Document -> Image
    {
        PXKnowlegeGraph pxKnowlegeGraph;
        PXDocument pxDocument;
        PXTexture PXTexture;

        const PXActionResult pxLoadResult = PXResourceLoadA(&pxDocument, "_TEST_DATA_INPUT_\\books.xml");

        const PXActionResult pxGraphResult = PXKnowlegeGraphLoadAndBuild(&pxKnowlegeGraph, &pxDocument, &PXTexture);

        const PXActionResult pxSaveResult = PXResourceSaveA(&PXTexture, "_TEST_DATA_INPUT_\\books.bmp", PXFileFormatBitMap);

        printf("\n");

        return 0;
    }
#endif // 1 // XML -> Document -> Image





#if 0
    PXFile pxFile;

    PXResourceLoadA(&pxFile, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaC\\Media\\PXTexture.h");
#endif // 0




#if 0
    PXBinaryWindows pxBinaryWindows;

    PXResourceLoadA(&pxBinaryWindows, "C:\\Windows\\System32\\user32.dll");
#endif // 0


    
#if 0
    while (1)
    {
        PXI32U temp;

        PXProcessorTemperature(&temp);

        PXonsoleGoToXY(0,0);
        printf("CPU temp : %i\n", temp);

    }
#endif


#if 0
    TestSoundAll();
#endif // 0

    


#if 0
    
    PXTexture PXTexture;

    PXResourceLoadA(&PXTexture, "N:\\IMG_0147.CR3");
#endif


















    //PXBinaryLinux pxELF;
    //PXResourceLoadA(&pxELF, "N:\\NAS\\Games\\PC\\Re-Volt_Linux\\rvgl.64.elf");

#if 0
    
    PXBinaryWindows pxEXE;
    //PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/32B-Windows-Debug/PXUltimaCTest.exe");
    PXResourceLoadA(&pxEXE, "C:/Data/WorkSpace/[GIT]/PXUltima/Code/[Export]/PXUltimaCTest/64B-Windows-Debug/PXUltima.dll");
#endif


#if 0
    // X86
    {
        PXI32U coolNumber = 0xAABBCCDD;

        //PXEndianSwapI32U(&coolNumber);

        PXBool result = coolNumber == 0xDDCCBBAA;

        printf("Hello, \n");

    }
#endif




    



#if 0
    // Direct X - Test
    PXWindow pxWindow;
    PXWindowConstruct(&pxWindow);

    PXWindowCreateA(&pxWindow, -1, -1, "Direct X - Test", PXTrue);

    PXAwaitChangeCU(&pxWindow.IsRunning);


    const PXF32 vertices[] =
    {
#if 1
        -0.5f, -0.5f, 1, 0xffff0000, // x, y, z, rhw, color
        0.0f,  0.5f, 1, 0xff00ffff,
         0.5f, -0.5f, 1, 0xff00ff00
    

#else 
        50.0f, 50.0f, 0.5f, 1.0f, 0xffff0000, // x, y, z, rhw, color
        250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00,
        50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff,
#endif
};

    PXGraphicSelect(&pxWindow.GraphicInstance);


    PXModel pxModel;
    PXObjectClear(PXModel, &pxModel);
    pxModel.VertexBuffer.VertexData = vertices;
    pxModel.VertexBuffer.VertexDataSize = sizeof(vertices);// / sizeof(PXF32);
    pxModel.VertexBuffer.VertexDataRowSize = sizeof(vertices) / 3;
    pxModel.VertexBuffer.Format = PXVertexBufferFormatXYZC; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

    PXGraphicModelRegister(&pxWindow.GraphicInstance, &pxModel);

    while (1)
    {
        const PXColorRGBAF pxColorRGBAF = { 0.3f,0.3f,0.3f,1.0f };

        PXGraphicClear(&pxWindow.GraphicInstance, &pxColorRGBAF);

        PXGraphicSceneBegin(&pxWindow.GraphicInstance);

        PXGraphicModelDraw(&pxWindow.GraphicInstance, &pxModel);

        PXGraphicSceneEnd(&pxWindow.GraphicInstance);
        PXGraphicSceneDeploy(&pxWindow.GraphicInstance);
    }

    PXWindowDestruct(&pxWindow);


    printf("\n");



#endif




#if 0

    vswprintf_s;

    PXText filePath;
    PXTextMakeFixedA(&filePath, "_TEST_DATA_INPUT_/A.fnt");

    PXSpriteFont pxSpriteFont;
    PXActionResult result = PXFontLoad(&pxSpriteFont, &filePath);

    printf("OK\n");

#endif



#if PXLDAPTest

    printf("Begin");

    PXLDAPClient pxLDAP;
    PXLDAPClientConstruct(&pxLDAP);

    PXLDAPConnectionInfo pxLDAPConnectionInfo;
    pxLDAPConnectionInfo.ConnectionOriented = PXTrue;
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.Host, Host, "ipa.demo1.freeipa.org");
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.ConnectionDomain, ConnectionDomain, "uid=admin,cn=users,cn=accounts,dc=demo1,dc=freeipa,dc=org");
    PXTextMakeFixedNamedA(&pxLDAPConnectionInfo.AuthenticationCredentials, AuthenticationCredentials, "Secret123");
    pxLDAPConnectionInfo.Port = PXLDAPPortDefault;
    pxLDAPConnectionInfo.SSLUse = 0;
    pxLDAPConnectionInfo.ConnectTimeout = 2000;
    pxLDAPConnectionInfo.AuthenticationMethod = PXLDAPAuthenticationMethodInvalid;

    PXActionResult openResult = PXLDAPClientOpen(&pxLDAP, &pxLDAPConnectionInfo);
    

    PXLDAPSearchInfo pxLDAPSearchInfo;
    PXTextMakeFixedNamedA(&pxLDAPSearchInfo.EntryName, EntryName, "dc=demo1,dc=freeipa,dc=org");
    PXTextMakeFixedNamedA(&pxLDAPSearchInfo.Filter, Filter, "(objectclass=person)");
    pxLDAPSearchInfo.Async = PXFalse;
    pxLDAPSearchInfo.OnlyTypesRequired = PXFalse;
    pxLDAPSearchInfo.AttributeList = PXNull;
    pxLDAPSearchInfo.Scope = PXLDAPScopeSubTree;

    const PXActionResult searchResult = PXLDAPClientSearch(&pxLDAP, &pxLDAPSearchInfo);
    const PXBool successful = PXActionSuccessful == searchResult;

    if (successful)
    {
        

        for (LDAPMessage* msg = ldap_first_entry(pxLDAP.ID, pxLDAP.SearchResult); msg; msg = ldap_next_entry(pxLDAP.ID, msg))
        {
            ULONG ReturnCode;                            // returned by server
            PSTR MatchedDNs;         // free with ldap_memfree
            PSTR ErrorMessage;       // free with ldap_memfree
            PZPSTR Referrals;        // free with ldap_value_freeA
            PLDAPControlA* ServerControls;               // free with ldap_free_controlsA
            const ULONG result = ldap_parse_resultA(pxLDAP.ID, msg, &ReturnCode, &MatchedDNs,&ErrorMessage,&Referrals, &ServerControls, PXFalse);
            const PXBool success = LDAP_SUCCESS == result;

            if (success)
            {
                BerElement* berElement = 0;
                
                printf("[%p] ID:%i Type:%i\n", msg, msg->lm_msgid, msg->lm_msgtype);

                for (char* attribute = ldap_first_attributeA(pxLDAP.ID, msg, &berElement); attribute; attribute = ldap_next_attributeA(pxLDAP.ID, msg, berElement))
                {

                    struct berval** calxx = ldap_get_values_lenA(pxLDAP.ID, msg, attribute);
                    const PXBool valueFetchSuccess = PXNull != calxx;

                    if (valueFetchSuccess)
                    {
                        printf("\t- %-25s : %-25s\n", attribute, (*calxx)->bv_val);
                    
                    }
                    else
                    {
                        printf("\t- %-25s : %-25s\n", attribute, 0);
                    }                
                }

                printf("\n\n");
            }
            else
            {
                printf("[%p] Error\n", msg);
            }

            
        }
    }


    PXLDAPClientDestruct(&pxLDAP);

    printf("END");

#endif 


#if PXKeyBoardVirtualTest
    PXKeyBoardVirtualInput inputList[4];

    PXKeyBoardVirtualInputSet(&inputList[0], KeyWorld1, PXKeyPressStateDown);
    PXKeyBoardVirtualInputSet(&inputList[1], KeyD, PXKeyPressStateDown);

    PXKeyBoardVirtualInputSet(&inputList[2], KeyD, PXKeyPressStateUp);
    PXKeyBoardVirtualInputSet(&inputList[3], KeyWorld1, PXKeyPressStateUp);
    
    PXKeyBoardVirtualInsertAction(&inputList, 4);
#endif // 0










#if 0
    PXTextMatchTest();

    //PXActionResult res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*", OnFileElementDetected, 0x02, 0);
    //res = PXDirectoryFilesInFolderW(L"B:/Daten/Bilder/*png", OnFileElementDetected, 0x01, 0);


    printf("");



    char buffer[64];

    for (size_t i = 0; i <= 0xFF; i++)
    {
        TextFromIntToBinary64U(buffer, 64, i);

        printf("[%4i] %s\n", i, buffer);
    }
#endif
    

#if 0 // TEST FILE DIALOG

    PXText pxText;
    PXTextMakeFixedA(&pxText, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code");

    PXDialogFileOpen(&pxText);
    PXDialogFileSave(&pxText);

    PXColorRGBI8 pxColorRGBI8;

    PXDialogColorSelect(&pxColorRGBI8);
    PXDialogFontSelect();
    PXDialogPrint();

#endif // 0 // TEST FILE DIALOG


#define KeyBoardTest 0
#if KeyBoardTest

    PXWindow pxWindow;
    PXText pxText;
    PXTextMakeFixedA(&pxText, "Window input Test");

    PXWindowConstruct(&pxWindow);
    PXWindowCreate(&pxWindow, 600, 400, &pxText, PXTrue);

    while (1)
    {
        printf("\033[H\033[J");
        printf("\033[%d;%dH", 0, 0);
        PXKeyBoardInputPrint(&pxWindow.KeyBoardCurrentInput);
        PXThreadSleep(0, 100);
    }

#endif // 0



#if 0
    while (1)
    {
        const PXI32U temperature = PXProcessorTemperature();

        printf("CPU Temp : %i\n", temperature);

        PXThreadSleep(0, 100);
    }
#endif // 1



#if 0
    PXTexture image;

    //ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.bmp");
    ImageLoadTest(&image, "C:/Users/BitPaw/Videos/SquareBlue.png");

    printf("EE\n");
    //TestImageAll();
#endif // 0


#if 0
    PXTestDebugAll();
#endif // 1


#if 0
    TestSoundAll();
#endif

#if 0
    TestFontAll();
#endif // 

#if 0 // PXOpenGL Test
    TestPXOpenGLAll();
#endif


#if 0
    TestSocket();
#endif // 1


#if 0
    TestSBPAll();
#endif // 1


#if 0
    TestFTPAll();
#endif // 0



    //Image image;

    //ImageLoadA(&image, "D:/_Data/Git/PXUltima/PXUltimaTest/_TEST_DATA_INPUT_/ImageInput.bmp");

    //ImageLoadA(&image, "A:/_WorkSpace/Download/at3_1m4_03.tif");

    //TestYAMLAll();
    //TestSystemInfoAll();
    //TestPXCompilerAll();
    //
    //TestWindowAll();
    //TestImageAll();
    // 
    // 
    // 
    //TestSBPAll();


#endif


    PXConsoleWrite(0, "[i] Finished testing...\n");

    return 0;
}
