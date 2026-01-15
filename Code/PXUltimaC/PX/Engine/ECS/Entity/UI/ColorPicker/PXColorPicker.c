#include "PXColorPicker.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Math/PXMath.h>

#include <gl/GL.h>
#include <PX/Math/PXTriangle.h>

#define PXColorPickerDebug 0

int colorTemp;

const char PXColorPickerName[] = "ColorPicker";
const PXI8U PXColorPickerNameLength = sizeof(PXColorPickerName);
const PXECSRegisterInfoStatic PXColorPickerInfoStatic =
{
    {sizeof(PXColorPickerName), sizeof(PXColorPickerName), PXColorPickerName, TextFormatASCII},
    sizeof(PXColorPicker),
    __alignof(PXColorPicker),
    PXECSTypeEntity,
    PXColorPickerCreate,
    PXNull,
    PXColorPickerDraw
};
PXECSRegisterInfoDynamic PXColorPickerInfoDynamic;

PXResult PXAPI PXColorPickerRegisterToECS()
{
    PXECSRegister(&PXColorPickerInfoStatic, &PXColorPickerInfoDynamic);

    return PXResultOK;
}


PXColorHSV currentHSV;
PXColorRGBAF currentRGBA;
float currentAlpha;

static void DrawFilledRect(float x0, float y0, float x1, float y1,
                           PXColorRGBF PXREF pxColorRGBF, float alpha)
{
    glColor4f(pxColorRGBF->Red, pxColorRGBF->Green, pxColorRGBF->Blue, alpha);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
}

static void RotatePoint(float cx, float cy, float angleRad, float* x, float* y)
{
    float s = PXMathSinusRADF32(angleRad);
    float c = PXMathCosinusRADF32(angleRad);

    float dx = *x - cx;
    float dy = *y - cy;

    float rx = dx * c - dy * s;
    float ry = dx * s + dy * c;

    *x = cx + rx;
    *y = cy + ry;
}

static void HSVToRGBA_Alpha(const PXColorHSV* hsv, float alpha, PXColorRGBAF* out)
{
    PXColorRGBAF tmp;
    PXColorHSVToRGBAF(hsv, &tmp);
    out->Red = tmp.Red;
    out->Green = tmp.Green;
    out->Blue = tmp.Blue;
    out->Alpha = alpha;
}


static void DrawGradientRectH(float x0, float y0, float x1, float y1,
                              PXColorRGBAF* left, PXColorRGBAF* right)
{
    glBegin(GL_QUADS);

    glColor4f(left->Red, left->Green, left->Blue, left->Alpha);
    glVertex2f(x0, y0);
    glVertex2f(x0, y1);

    glColor4f(right->Red, right->Green, right->Blue, right->Alpha);
    glVertex2f(x1, y1);
    glVertex2f(x1, y0);

    glEnd();
}

static void DrawGradientRectV(float x0, float y0, float x1, float y1,
                              PXColorRGBAF* bottom, PXColorRGBAF* top)
{
    glBegin(GL_QUADS);

    glColor4f(bottom->Red, bottom->Green, bottom->Blue, bottom->Alpha);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);

    glColor4f(top->Red, top->Green, top->Blue, top->Alpha);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);

    glEnd();
}

#define segmentAmount 30
#define angleStep 360.0f / segmentAmount

float vertices[(segmentAmount + 1) * 2 * 2]; // 2 rings, 2 coords
float colors[(segmentAmount + 1) * 2 * 4];   // RGBA per vertex



float cx, cy;        // center of the color wheel
float circleR = 0;       // radius of the color wheel
float rectW, rectH;  // width/height of the rectangle
float border = 2.0f; // border thickness
float cornerR = 0;       // corner radius (usually circleR - small offset)


void DrawRoundedRect(float x, float y, float w, float h, float r)
{
    const int segments = 16;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + w / 2, y + h / 2); // center

    // Top-right corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f);
        glVertex2f(x + w - r + cosf(a) * r,
                   y + h - r + sinf(a) * r);
    }

    // Top-left corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + PXMathConstantPI / 2.0f;
        glVertex2f(x + r + cosf(a) * r,
                   y + h - r + sinf(a) * r);
    }

    // Bottom-left corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + PXMathConstantPI;
        glVertex2f(x + r + cosf(a) * r,
                   y + r + sinf(a) * r);
    }

    // Bottom-right corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + 3.0f * PXMathConstantPI / 2.0f;
        glVertex2f(x + w - r + cosf(a) * r,
                   y + r + sinf(a) * r);
    }

    glEnd();
}

void DrawRoundedRectBorder(float x, float y, float w, float h, float r, float thickness)
{
    const int segments = 32;

    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);

    // Top-right corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f);
        glVertex2f(x + w - r + cosf(a) * r,
                   y + h - r + sinf(a) * r);
    }

    // Top-left corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + PXMathConstantPI / 2.0f;
        glVertex2f(x + r + cosf(a) * r,
                   y + h - r + sinf(a) * r);
    }

    // Bottom-left corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + PXMathConstantPI;
        glVertex2f(x + r + cosf(a) * r,
                   y + r + sinf(a) * r);
    }

    // Bottom-right corner
    for(int i = 0; i <= segments; i++)
    {
        float a = (float)i / segments * (PXMathConstantPI / 2.0f) + 3.0f * PXMathConstantPI / 2.0f;
        glVertex2f(x + w - r + cosf(a) * r,
                   y + r + sinf(a) * r);
    }

    glEnd();
}




PXBool wireFrame = 0;




// hsv:  H in [0..360], S in [0..1], V in [0..1]
// Ax,Ay, Bx,By, Cx,Cy: triangle vertex positions
// Px,Py: output knob position
void PXHSVTriangleToPosition(const PXColorHSV PXREF pxColorHSV,
                         const PXTriangleF32 PXREF pxTriangleF32,
                         PXVector2F32 PXREF point)
{
    float st = pxColorHSV->Saturation;
    float vt = pxColorHSV->Value;

    // Clamp to valid HSV triangle region
    if(vt < 0.0f) vt = 0.0f;
    if(vt > 1.0f) vt = 1.0f;

    if(st < 0.0f) st = 0.0f;
    if(st > vt)    st = vt;   // <-- the important part



    // Barycentric coordinates for HSV triangle
    float u = st;        // along A?B
    float v = vt - st;    // along B?C
    float w = 1.0f - vt; // toward C

    // Convert barycentric ? 2D position
    point->X = u * pxTriangleF32->A.X + v * pxTriangleF32->B.X + w * pxTriangleF32->C.X;
    point->Y = u * pxTriangleF32->A.Y + v * pxTriangleF32->B.Y + w * pxTriangleF32->C.Y;
}


void drawFilledCircle(float cx, float cy, float r, int segments)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#if 0
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // center

    for(int i = 0; i <= segments; i++)
    {
        float theta = (2.0f * 3.1415926f * i) / segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
#else

    glBegin(GL_LINE_LOOP);

    for(int i = 0; i < segments; i++)
    {
        float theta = (2.0f * 3.1415926f * i) / segments;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }

    glEnd();
#endif

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



PXResult PXAPI PXColorPickerDrawGL(PXColorPicker PXREF pxColorPicker, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXWindow PXREF pxWindow = pxColorPicker->WindowBase;

    float x = pxWindowDrawInfo->RectangleXYWH.X;
    float y = pxWindowDrawInfo->RectangleXYWH.Y;
    float width = pxWindowDrawInfo->RectangleXYWH.Width;
    float height = pxWindowDrawInfo->RectangleXYWH.Height;

 
  
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);


    glColor3f(0.8f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(+1, -1);
    glVertex2f(+1, +1);
    glVertex2f(-1, +1);
    glEnd();

    //return;

    // Layout: triangle on left, sliders on right, previews at bottom
    float padding = 8.0f;

    // Center for wheel + triangle
    float pickerSize = height * 0.6f;
    float centerX = x + pickerSize * 0.6f;
    float centerY = y + height * 0.65f;

    float outerRadius = pickerSize * 0.5f;
    float innerRadius = outerRadius * 0.75f; // inner edge of hue wheel
    float triRadius = innerRadius * 0.85f; // triangle fits inside



    cx = centerX;
    cy = centerY;
    circleR = innerRadius;
    rectW = width;
    rectH = height;
    cornerR = circleR * 0.12f;


    int idxV = 0;
    int idxC = 0;

    for(int i = 0; i <= segmentAmount; ++i)
    {
        float angle = i * angleStep;
        float rad = angle * (3.14159265f / 180.0f);

        float c = cosf(rad);
        float s = sinf(rad);

        // HSV ? RGB
        PXColorHSV hsv = { angle, 1.0f, 1.0f };
        PXColorRGBAF rgb;
        PXColorHSVToRGBAF(&hsv, &rgb);

        // Inner vertex
        vertices[idxV++] = centerX + innerRadius * c;
        vertices[idxV++] = centerY + innerRadius * s;

        colors[idxC++] = rgb.Red;
        colors[idxC++] = rgb.Green;
        colors[idxC++] = rgb.Blue;
        colors[idxC++] = 1.0f;

        // Outer vertex
        vertices[idxV++] = centerX + outerRadius * c;
        vertices[idxV++] = centerY + outerRadius * s;

        colors[idxC++] = rgb.Red;
        colors[idxC++] = rgb.Green;
        colors[idxC++] = rgb.Blue;
        colors[idxC++] = 1.0f;
    }








    // -----------------------
    // 1) HUE WHEEL
    // -----------------------
#if 1
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glColorPointer(4, GL_FLOAT, 0, colors);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, (segmentAmount + 1) * 2);

        // Wrieframe
        if(wireFrame)
        {
            glLineWidth(2.0f);
            glDisableClientState(GL_COLOR_ARRAY);
            glColor3f(0.2f, 0.2f, 0.2f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (segmentAmount + 1) * 2);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }    

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
#endif

    float s = innerRadius;// (innerRadius * 2) / 1.7320508f;// 1.414213560f ~sqrt(3)
    float a0 = 0;// 1.570796327f; // 90° 
    float a1 = 120;//3.665191429f; // 210° 
    float a2 = 240;//5.759586532f; // 330°

    if(1)
    {
        a0 += currentHSV.Hue;
        a1 += currentHSV.Hue;
        a2 += currentHSV.Hue;
    }

    if(wireFrame)
    {
        glColor3f(0.2f, 0.8f, 0.2f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glVertex2f(centerX - outerRadius, centerY - outerRadius);
        glVertex2f(centerX + outerRadius, centerY - outerRadius);
        glVertex2f(centerX + outerRadius, centerY + outerRadius);
        glVertex2f(centerX - outerRadius, centerY + outerRadius);
        glEnd();


        glBegin(GL_QUADS);
        glVertex2f(centerX - innerRadius, centerY - innerRadius);
        glVertex2f(centerX + innerRadius, centerY - innerRadius);
        glVertex2f(centerX + innerRadius, centerY + innerRadius);
        glVertex2f(centerX - innerRadius, centerY + innerRadius);
        glEnd();

        glColor3f(0.8f, 0.2f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(centerX - s, centerY - s);
        glVertex2f(centerX + s, centerY - s);
        glVertex2f(centerX + s, centerY + s);
        glVertex2f(centerX - s, centerY + s);
        glEnd();
    }    
    

    PXTriangleF32 pxTriangle;
    pxTriangle.A.X = centerX + s * PXMathCosinusDEGF32(a0);
    pxTriangle.A.Y = centerY + s * PXMathSinusDEGF32(a0);
    pxTriangle.B.X = centerX + s * PXMathCosinusDEGF32(a1);
    pxTriangle.B.Y = centerY + s * PXMathSinusDEGF32(a1);
    pxTriangle.C.X = centerX + s * PXMathCosinusDEGF32(a2);
    pxTriangle.C.Y = centerY + s * PXMathSinusDEGF32(a2);


  


#if 0
    glBegin(GL_TRIANGLES);
    glVertex2f(centerX - s, centerY - s);
    glVertex2f(centerX + s, centerY - s);
    glVertex2f(centerX, centerY + s);
    glEnd();
#endif

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // -----------------------
    // 2) ROTATING TRIANGLE (S/V)
    // -----------------------
    // Base triangle before rotation (equilateral, pointing up)

    //currentHSV.Hue += 0.15;




    // Rotate triangle by current hue so hue corner points outward
    float angleRad = currentHSV.Hue * (3.14159265f / 180.0f);
    //RotatePoint(pxTriangle.A.X, pxTriangle.A.Y, angleRad, &pxTriangle.A.X, &pxTriangle.A.Y);
    //otatePoint(pxTriangle.B.X, pxTriangle.B.Y, angleRad, &pxTriangle.B.X, &pxTriangle.B.Y);
    //RotatePoint(pxTriangle.C.X, pxTriangle.C.Y, angleRad, &pxTriangle.C.X, &pxTriangle.C.Y);

    // Colors at triangle corners:
    // Let's define:
    // - one corner = full hue color (S=1,V=1)
    // - one = white (S=0,V=1)
    // - one = black (V=0)
    PXColorHSV hsvHue = { currentHSV.Hue, 1.0f, 1.0f };
    PXColorHSV hsvWhite = { currentHSV.Hue, 0.0f, 1.0f };
    PXColorHSV hsvBlack = { currentHSV.Hue, 0.0f, 0.0f };

    PXColorRGBAF colHue, colWhite, colBlack;
    PXColorHSVToRGBAF(&hsvHue, &colHue);
    PXColorHSVToRGBAF(&hsvWhite, &colWhite);
    PXColorHSVToRGBAF(&hsvBlack, &colBlack);
    colHue.Alpha = 1.0f;
    colWhite.Alpha = 1.0f;
    colBlack.Alpha = 1.0f;

    // Assign: top = white, left = black, right = hue (common Photoshop-like layout)

    glBegin(GL_TRIANGLES);
    glColor4f(colHue.Red, colHue.Green, colHue.Blue, 1.0f);
    glVertex2f(pxTriangle.A.X, pxTriangle.A.Y);
    glColor4f(colWhite.Red, colWhite.Green, colWhite.Blue, 1.0f);
    glVertex2f(pxTriangle.B.X, pxTriangle.B.Y );
    glColor4f(colBlack.Red, colBlack.Green, colBlack.Blue, 1.0f);
    glVertex2f(pxTriangle.C.X, pxTriangle.C.Y);
    glEnd();


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2f(pxTriangle.A.X, pxTriangle.A.Y);
    glVertex2f(pxTriangle.B.X, pxTriangle.B.Y);
    glVertex2f(pxTriangle.C.X, pxTriangle.C.Y);
    glEnd();



    float widthAgng = 0.08;

    PXVector2F32 selectRectangle[4];
    selectRectangle[0].X = centerX + innerRadius * cos(angleRad+ widthAgng);
    selectRectangle[0].Y = centerY + innerRadius * sin(angleRad+ widthAgng);

    selectRectangle[1].X = centerX + outerRadius * cos(angleRad+ widthAgng);
    selectRectangle[1].Y = centerY + outerRadius * sin(angleRad+ widthAgng);

    selectRectangle[2].X = centerX + outerRadius * cos(angleRad- widthAgng);
    selectRectangle[2].Y = centerY + outerRadius * sin(angleRad- widthAgng);

    selectRectangle[3].X = centerX + innerRadius * cos(angleRad- widthAgng);
    selectRectangle[3].Y = centerY + innerRadius * sin(angleRad- widthAgng);

    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glColor4f(0.2f, 0.2f, 0.2f, 0.2);
    glVertex2f(selectRectangle[0].X, selectRectangle[0].Y);
    glVertex2f(selectRectangle[1].X, selectRectangle[1].Y);
    glVertex2f(selectRectangle[2].X, selectRectangle[2].Y);
    glVertex2f(selectRectangle[3].X, selectRectangle[3].Y);
    glEnd();





    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Knob

    
    PXVector2F32 knobPosition;

    PXHSVTriangleToPosition(&currentHSV, &pxTriangle, &knobPosition);

   // RotatePoint(knobPosition.X, knobPosition.Y, angleRad, &knobPosition.X, &knobPosition.Y);

    float knobSize = innerRadius * 0.05;

    glColor3f(0.8f, 0.8f, 0.8f);
    drawFilledCircle(knobPosition.X, knobPosition.Y, knobSize, 20);

    /*
    glLineWidth(6);
    glPointSize(6);
    glBegin(GL_QUADS);
    glVertex2f(knobPosition.X - knobSize, knobPosition.Y - knobSize);
    glVertex2f(knobPosition.X + knobSize, knobPosition.Y - knobSize);
    glVertex2f(knobPosition.X + knobSize, knobPosition.Y + knobSize);
    glVertex2f(knobPosition.X - knobSize, knobPosition.Y + knobSize);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(knobPosition.X - knobSize, knobPosition.Y - knobSize);
    glVertex2f(knobPosition.X + knobSize, knobPosition.Y - knobSize);
    glVertex2f(knobPosition.X + knobSize, knobPosition.Y + knobSize);
    glVertex2f(knobPosition.X - knobSize, knobPosition.Y + knobSize);
    glEnd();*/










    currentHSV.Hue += 2;
    //
    //

    currentHSV.Saturation += 0.008;
    currentHSV.Value += 0.002;



    if(currentHSV.Hue >= 360)
    {
        currentHSV.Hue -= 360;
        wireFrame = !wireFrame;
    }

    if(currentHSV.Saturation > 1)
    {
        currentHSV.Saturation = 0;
    }


    if(currentHSV.Value > 1)
    {
        currentHSV.Value = 0;
    }


#if 0
    float rectW = 80;
    float rectH = 40;
    float cornerR = 12;

    float xa = pxTriangle.A.X - rectW / 2;
    float ya = pxTriangle.A.Y - rectH / 2;

    glColor3f(1, 1, 1); // fill
    DrawRoundedRect(xa, ya, rectW, rectH, cornerR);

    glColor3f(0, 0, 0); // border
    DrawRoundedRectBorder(xa, ya, rectW, rectH, cornerR, 2.0f);
#endif









    // -----------------------
    // 3) Selection marker + tip color box
    // -----------------------
    // Here we assume currentHSV.Saturation/Value correspond to a point inside the triangle.
    // For now, just show the color box at the hue-corner direction (for visibility).
    PXColorRGBAF selectedFromHSV;
    HSVToRGBA_Alpha(&currentHSV, currentAlpha, &selectedFromHSV);

    float tipBoxSize = 18.0f;
    /*
    // “Pointer” position – we’ll place it along the line from center towards hue corner
    float pointerX = (triRightX + triTopX + triLeftX) / 3.0f; // placeholder: barycenter
    // You will likely replace this with real S/V ? barycentric mapping.
    float pointerY = (triRightY + triTopY + triLeftY) / 3.0f;

    // Box at the tip of triangle (you can move this to pointerX/Y if you like)
    DrawFilledRect(
        triTopX - tipBoxSize * 0.5f,
        triTopY + padding,
        triTopX + tipBoxSize * 0.5f,
        triTopY + padding + tipBoxSize,
        (PXColorRGBF*)&selectedFromHSV, selectedFromHSV.Alpha
    );

    // Optionally: small circle/box at pointer to visualize exact spot
    {
        float sz = 6.0f;
        PXColorRGBF white = { 1.0f, 1.0f, 1.0f };
        DrawFilledRect(
            pointerX - sz, pointerY - sz,
            pointerX + sz, pointerY + sz,
            &white, 1.0f
        );
    }*/

    // -----------------------
    // 4) Sliders (HSV / RGB / Alpha)
    // -----------------------
    float slidersX0 = x + pickerSize + padding * 2.0f;
    float sliderWidth = width - (slidersX0 - x) - padding;
    float sliderHeight = 16.0f;
    float sliderGap = 6.0f;
    float sliderY = y + height - sliderHeight - padding;

    // H slider
    {
        int   steps = 24;
        float stepW = sliderWidth / (float)steps;
        float sx = slidersX0;

        for(int i = 0; i < steps; ++i)
        {
            float h0 = (360.0f * i) / (float)steps;
            float h1 = (360.0f * (i + 1)) / (float)steps;

            PXColorHSV hsv0 = { h0, 1.0f, 1.0f };
            PXColorHSV hsv1 = { h1, 1.0f, 1.0f };
            PXColorRGBAF c0, c1;
            PXColorHSVToRGBAF(&hsv0, &c0);
            PXColorHSVToRGBAF(&hsv1, &c1);
            c0.Alpha = 1.0f;
            c1.Alpha = 1.0f;

            DrawGradientRectH(
                sx, sliderY,
                sx + stepW, sliderY + sliderHeight,
                (PXColorRGBF*)&c0, (PXColorRGBF*)&c1
            );

            sx += stepW;
        }

        sliderY -= (sliderHeight + sliderGap);
    }

    // S slider
    {
        PXColorHSV hsv0 = { currentHSV.Hue, 0.0f, currentHSV.Value };
        PXColorHSV hsv1 = { currentHSV.Hue, 1.0f, currentHSV.Value };
        PXColorRGBAF c0, c1;
        PXColorHSVToRGBAF(&hsv0, &c0);
        PXColorHSVToRGBAF(&hsv1, &c1);
        c0.Alpha = 1.0f;
        c1.Alpha = 1.0f;

        DrawGradientRectH(
            slidersX0, sliderY,
            slidersX0 + sliderWidth, sliderY + sliderHeight,
            (PXColorRGBF*)&c0, (PXColorRGBF*)&c1
        );
        sliderY -= (sliderHeight + sliderGap);
    }

    // V slider
    {
        PXColorHSV hsv0 = { currentHSV.Hue, currentHSV.Saturation, 0.0f };
        PXColorHSV hsv1 = { currentHSV.Hue, currentHSV.Saturation, 1.0f };
        PXColorRGBAF c0, c1;
        PXColorHSVToRGBAF(&hsv0, &c0);
        PXColorHSVToRGBAF(&hsv1, &c1);
        c0.Alpha = 1.0f;
        c1.Alpha = 1.0f;

        DrawGradientRectH(
            slidersX0, sliderY,
            slidersX0 + sliderWidth, sliderY + sliderHeight,
            (PXColorRGBF*)&c0, (PXColorRGBF*)&c1
        );
        sliderY -= (sliderHeight + sliderGap);
    }

    // R, G, B sliders (using currentRGBA)
    {
        PXColorRGBF c0, c1;

        // R
        c0.Red = 0.0f; c0.Green = currentRGBA.Green; c0.Blue = currentRGBA.Blue;
        c1.Red = 1.0f; c1.Green = currentRGBA.Green; c1.Blue = currentRGBA.Blue;
        DrawGradientRectH(slidersX0, sliderY,
                          slidersX0 + sliderWidth, sliderY + sliderHeight,
                          &c0, &c1);
        sliderY -= (sliderHeight + sliderGap);

        // G
        c0.Red = currentRGBA.Red; c0.Green = 0.0f; c0.Blue = currentRGBA.Blue;
        c1.Red = currentRGBA.Red; c1.Green = 1.0f; c1.Blue = currentRGBA.Blue;
        DrawGradientRectH(slidersX0, sliderY,
                          slidersX0 + sliderWidth, sliderY + sliderHeight,
                          &c0, &c1);
        sliderY -= (sliderHeight + sliderGap);

        // B
        c0.Red = currentRGBA.Red; c0.Green = currentRGBA.Green; c0.Blue = 0.0f;
        c1.Red = currentRGBA.Red; c1.Green = currentRGBA.Green; c1.Blue = 1.0f;
        DrawGradientRectH(slidersX0, sliderY,
                          slidersX0 + sliderWidth, sliderY + sliderHeight,
                          &c0, &c1);
        sliderY -= (sliderHeight + sliderGap);
    }

    // Alpha slider with checker background
    {
        float checkerSize = 6.0f;
        float ax0 = slidersX0;
        float ax1 = slidersX0 + sliderWidth;
        float ay0 = sliderY;
        float ay1 = sliderY + sliderHeight;

        for(float yy = ay0; yy < ay1; yy += checkerSize)
        {
            for(float xx = ax0; xx < ax1; xx += checkerSize)
            {
                int ix = (int)((xx - ax0) / checkerSize);
                int iy = (int)((yy - ay0) / checkerSize);
                float c = ((ix + iy) & 1) ? 0.7f : 0.9f;

                PXColorRGBF col = { c, c, c };
                DrawFilledRect(xx, yy,
                               xx + checkerSize, yy + checkerSize,
                               &col, 1.0f);
            }
        }

        PXColorRGBF opaque = { currentRGBA.Red, currentRGBA.Green, currentRGBA.Blue };
        PXColorRGBF transparent = { currentRGBA.Red * 0.0f,
                                    currentRGBA.Green * 0.0f,
                                    currentRGBA.Blue * 0.0f };

        DrawGradientRectH(
            ax0, ay0,
            ax1, ay1,
            &transparent, &opaque
        );

        sliderY -= (sliderHeight + sliderGap);
    }

    // Preview boxes
    {
        float previewSize = 32.0f;
        float previewY = y + padding;

        // Current RGBA
        PXColorRGBF previewRGB = { currentRGBA.Red, currentRGBA.Green, currentRGBA.Blue };
        DrawFilledRect(
            slidersX0,
            previewY,
            slidersX0 + previewSize,
            previewY + previewSize,
            &previewRGB, currentAlpha
        );

        // HSV-based selected color (same here)
        PXColorRGBAF sel = selectedFromHSV;
        PXColorRGBF selRGB = { sel.Red, sel.Green, sel.Blue };
        DrawFilledRect(
            slidersX0 + previewSize + padding,
            previewY,
            slidersX0 + 2.0f * previewSize + padding,
            previewY + previewSize,
            &selRGB, 1.0f
        );
    }

    return PXResultOK;
}

PXResult PXAPI PXColorPickerDraw(PXColorPicker PXREF pxColorPicker, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXWindow PXREF pxWindow = pxColorPicker->WindowBase;

#if PXLogEnable && PXColorPickerDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXColorPickerName,
        "Draw",
        "--- ColorPicker --- START"
    );
#endif
    PXWindowDrawRectangle2D
    (
        pxWindow,
        pxWindowDrawInfo
    );

    PXColorPickerDrawGL(pxColorPicker, pxWindowDrawInfo);

    return;

    PXF32 scaling = 0.80f;
    int width = (pxWindowDrawInfo->RectangleXYWH.Width - pxWindowDrawInfo->RectangleXYWH.X) / 2;
    int height = (pxWindowDrawInfo->RectangleXYWH.Height - pxWindowDrawInfo->RectangleXYWH.Y) / 2;
    int widthS = (pxWindowDrawInfo->RectangleXYWH.Width * scaling - pxWindowDrawInfo->RectangleXYWH.X * scaling) / 2;
    int heightS = (pxWindowDrawInfo->RectangleXYWH.Height * scaling - pxWindowDrawInfo->RectangleXYWH.Y * scaling) / 2;

    int precision = 8;

    PXF32 r = 250;

#if OSUnix
#elif OSWindows
    HDC hdc = PXWindowDCGet(pxColorPicker);

    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, brush);

    {
        TRIVERTEX vertexList[120];
        GRADIENT_TRIANGLE gRect[120];

        for(size_t i = 0; i < precision; ++i)
        {
            TRIVERTEX PXREF vertex = &vertexList[i];
            GRADIENT_TRIANGLE PXREF index = &gRect[i];

            const PXF32 steps = 360.f / (PXF32)precision;
            PXF32 degree = (steps * (i + 1)) * (3.14f / 180.f);

            PXF32 x = PXMathCosinusRADF32(degree);
            PXF32 y = PXMathSinusRADF32(degree);

            vertex->x = r * x + width;
            vertex->y = r * y + height;
            vertex->Red = 0xFF;
            vertex->Green = ((i) / (PXF32)precision) * 0xFF;
            vertex->Blue = 0x0000;
            vertex->Alpha = 0;

            index->Vertex1 = 0;
            index->Vertex2 = i;
            index->Vertex3 = i + 1;
        }




        // const int precision = 360;
        PXColorCircleVertex pxColorCircleVertexList[360];

        PXColorCircle pxColorCircle;
        pxColorCircle.Precision = 64;
        pxColorCircle.Size = 225;
        pxColorCircle.VertexList = pxColorCircleVertexList;
        pxColorCircle.StartX = width;
        pxColorCircle.StartY = height;
        pxColorCircle.ColorSelected.Hue = colorTemp;
        pxColorCircle.ColorSelected.Saturation = 1;
        pxColorCircle.ColorSelected.Value = 1;

        //PXMathCircle(&pxColorCircle);

        colorTemp += 15;
        colorTemp = (int)colorTemp % 360;

        for(PXSize i = 0; i < pxColorCircle.Precision; ++i)
        {
            int indexA = i;
            int indexB = (i + 1) % pxColorCircle.Precision;

            PXColorCircleVertex* dataA = &pxColorCircleVertexList[indexA];
            PXColorCircleVertex* dataB = &pxColorCircleVertexList[indexB];

            // TRIVERTEX* vertexA = &vertexList[i];
            // TRIVERTEX* vertexB = &vertexList[indexB];
            const COLORREF color = RGB(dataA->Red, dataA->Green, dataA->Blue);
            const HPEN hPen = CreatePen(PS_SOLID, 20, color);
            SelectObject(hdc, hPen);

            MoveToEx(hdc, dataA->X, dataA->Y, NULL);
            LineTo(hdc, dataB->X, dataB->Y);

        }

        GradientFill
        (
            hdc,
            vertexList,
            pxColorCircle.Precision * 3,
            &gRect,
            1,
            GRADIENT_FILL_TRIANGLE
        );


        PXColorRGBAI8 pxColorRGBAI8;

        PXColorHSVToRGBAI8(&pxColorCircle.ColorSelected, &pxColorRGBAI8);

        TRIVERTEX vertices[3];

        // Define the vertices of the triangle
        vertices[0].x = pxColorCircle.VertexListTriangle[1].X;
        vertices[0].y = pxColorCircle.VertexListTriangle[1].Y;
        vertices[0].Red = 0x0000;
        vertices[0].Green = 0x0000;
        vertices[0].Blue = 0x0000;
        vertices[0].Alpha = 0xFFFF;

        vertices[1].x = pxColorCircle.VertexListTriangle[0].X;
        vertices[1].y = pxColorCircle.VertexListTriangle[0].Y;
        vertices[1].Red = (pxColorRGBAI8.Red / (PXF32)0xFF) * 0xFFFF;
        vertices[1].Green = (pxColorRGBAI8.Green / (PXF32)0xFF) * 0xFFFF;
        vertices[1].Blue = (pxColorRGBAI8.Blue / (PXF32)0xFF) * 0xFFFF;
        vertices[1].Alpha = 0xFFFF;

        vertices[2].x = pxColorCircle.VertexListTriangle[2].X;
        vertices[2].y = pxColorCircle.VertexListTriangle[2].Y;
        vertices[2].Red = 0xFFFF;
        vertices[2].Green = 0xFFFF;
        vertices[2].Blue = 0xFFFF;
        vertices[2].Alpha = 0xFFFF;

        // Draw the triangle
        // HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
        GRADIENT_TRIANGLE  gRectaaa;
        gRectaaa.Vertex1 = 0;
        gRectaaa.Vertex2 = 1;
        gRectaaa.Vertex3 = 2;

        // Draw the gradient rectangle
        GradientFill(hdc, vertices, 3, &gRectaaa, 1, GRADIENT_FILL_TRIANGLE);
    }


#endif


    /*

    int width = 128;
    int height = 128;
    PXF32 scale = 80.5;

    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            HBRUSH brush = CreateSolidBrush(RGB(i * width * scale, j * height * scale, 128));
            RECT rect =
            {
                i * 15,
                j * 15,
                (i + 1) * 15,
                (j + 1) * 15
            };

            rect.left = pxWindow->Position.Left - rect.left;
            rect.top = pxWindow->Position.Top - rect.top;
            rect.right = pxWindow->Position.Right - rect.right;
            rect.bottom = pxWindow->Position.Bottom - rect.bottom;

            FillRect(pxWindow->DeviceContextHandle, &rect, brush);
            DeleteObject(brush);
        }
    }
    */

    //   PXWindowDrawA(pxGUIManager, pxWindow, pxWindow->NameContent, pxWindow->NameContentSize);


#if PXLogEnable && PXColorPickerDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXColorPickerName,
        "Draw",
        "--- ColorPicker --- END"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXColorPickerCreate(PXColorPicker** pxColorPickerREF, PXColorPickerCreateInfo PXREF pxColorPickerCreateInfo)
{
    PXColorPicker* pxColorPicker = PXNull;

    pxColorPickerCreateInfo->Info.Static = &PXColorPickerInfoStatic;
    pxColorPickerCreateInfo->Info.Dynamic = &PXColorPickerInfoDynamic;
    PXResult pxResult = PXECSCreate(pxColorPickerREF, pxColorPickerCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxColorPicker = *pxColorPickerREF;

    pxColorPickerCreateInfo->Window.EventList.CallBackOwner = pxColorPicker;
    pxColorPickerCreateInfo->Window.EventList.CallBackDraw = PXColorPickerDraw;

    PXWindowCreate(&pxColorPicker->WindowBase, &pxColorPickerCreateInfo->Window);

    return PXResultOK;
}