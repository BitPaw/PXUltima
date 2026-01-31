#pragma once

#ifndef PXFileFormatIncluded
#define PXFileFormatIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>

//---------------------------------------------------------
// File Management
//---------------------------------------------------------
#define PXFileFormatMask                        0x000000FF

#define PXFileFormatInvalid                     0
#define PXFileFormatUnkown                      1
#define PXFileFormatA3DS                        2
#define PXFileFormatAAC                         3 // .acc
#define PXFileFormatAVI                         4 // .avi
#define PXFileFormatBitMap                      5 // .bmp
#define PXFileFormatBinkVideo                   6 // .bnk
#define PXFileFormatC                           7
#define PXFileFormatCSharp                      8
#define PXFileFormatCSS                         9
#define PXFileFormatCPP                         10
#define PXFileFormatCanonRaw3                   11
#define PXFileFormatDirectDrawSurfaceTexture    12 // .dds
#define PXFileFormatBinaryWindows               13
#define PXFileFormatBinaryLinux                 14
#define PXFileFormatEML                         15
#define PXFileFormatFastFile                    16
#define PXFileFormatFilmBox                     17
#define PXFileFormatFLAC                        18
#define PXFileFormatSpriteFont                  19
#define PXFileFormatGIF                         20
#define PXFileFormatOpenGLShader                21
#define PXFileFormatDirectXShader               22
#define PXFileFormatHighEfficiencyImageFile     23
#define PXFileFormatHTML                        24
#define PXFileFormatINI                         25
#define PXFileFormatEugeneRoshalArchive         26 // .rar
#define PXFileFormatJava                        27 // .jar
#define PXFileFormatJavaScript                  28 // .js
#define PXFileFormatJPEG                        29
#define PXFileFormatJSON                        30
#define PXFileFormatM4A                         31
#define PXFileFormatMIDI                        32
#define PXFileFormatMP3                         33
#define PXFileFormatMP4                         34
#define PXFileFormatMSI                         35
#define PXFileFormatMTL                         36
#define PXFileFormatN64                         37
#define PXFileFormatWavefront                   38
#define PXFileFormatMatroska                    39 // .mkv
#define PXFileFormatOGG                         40
#define PXFileFormatPDF                         41
#define PXFileFormatPHP                         42
#define PXFileFormatPLY                         43
#define PXFileFormatPNG                         44
#define PXFileFormatQOI                         45
#define PXFileFormatSTEP                        46
#define PXFileFormatSTL                         47
#define PXFileFormatSVG                         48
#define PXFileFormatTAR                         49
#define PXFileFormatTGA                         50
#define PXFileFormatTagImage                    51
#define PXFileFormatTrueTypeFont                52
#define PXFileFormatUniversalSceneDescription   53
#define PXFileFormatVideoObject                 54
#define PXFileFormatVRML                        55
#define PXFileFormatWAD                         56
#define PXFileFormatWave                        57
#define PXFileFormatWEBM                        58
#define PXFileFormatWEBP                        59
#define PXFileFormatWMA                         60
#define PXFileFormatXML                         61
#define PXFileFormatYAML                        62
#define PXFileFormatZIP                         63

#define PXFileFormatLua                         64 // .luaobj

#define PXFileFormatRedshiftWwisePackage        65 // r3d2\0x01\0x00\0x00\0x00, .wpk
#define PXFileFormatRedshiftMesh                66 // r3d2Mesh, .scb, sco
#define PXFileFormatRedshiftSkeleton            67 // r3d2sklt, .skl
#define PXFileFormatRedshiftAnimation           68 // r3d2anmd, r3d2canm, .anm
#define PXFileFormatWAudioBank                  69 // BKHD, .bnk 
#define PXFileFormatRGeometryWorld              70 // WGEO, .wgeo
#define PXFileFormatRGeometryMap                71 // OEGM, .mapgeo
#define PXFileFormatRProperty                   72 // 
#define PXFileFormatRTexture                    73 // TEX, .tex
#define PXFileFormatRPreLoad                    74 // PreLoad
#define PXFileFormatRLightGrid                  75 // 3
#define PXFileFormatRStringTable                76 // RST
#define PXFileFormatRPropertyOverride           78 // PTCH
#define PXFileFormatRSkinSimple                 79 // 0x33221100
#define PXFileFormatRSkinedMesh                 80 // 0x22FD4FC3
#define PXFileFormat6D                          81
#define PXFileFormatOEGM                        82


//---------------------------------------------------------
// Flags
#define PXFileFormatVarriantMask            0b00000000000000110000000000000000
#define PXFileFormatVarriantUndefined       0b00000000000000000000000000000000
#define PXFileFormatVarriantBinary          0b00000000000000010000000000000000
#define PXFileFormatVarriantText            0b00000000000000100000000000000000

#define PXFileFormatSignatureOffsetMask     0b00000000000001000000000000000000
#define PXFileFormatSignatureOffsetAtBegin  0b00000000000000000000000000000000
#define PXFileFormatSignatureOffsetAtEnd    0b00000000000001000000000000000000





//---------------------------------------------------------
// Resource types and structs
//---------------------------------------------------------
#define PXFileFormatTypeMask                        0x0000FF00
//#define PXFileFormatTypeSet(flag)   (flag << 8)

#define PXFileFormatTypeInvalid     0<<8 // Resource invalid, don't use.
#define PXFileFormatTypeCustom      1<<8 // Undetected but valid format. Needs to be handled by the caller
#define PXFileFormatTypeImage       2<<8 // Image for pixeldata
#define PXFileFormatTypeTexture     3<<8 // texture is like an image but with additional context
#define PXFileFormatTypeSound       4<<8      
#define PXFileFormatTypeVideo       5<<8      
#define PXFileFormatTypeModel       6<<8 // 3D model, collection of vertex data
#define PXFileFormatTypeFont        7<<8 // Collection of spites or points to render text
#define PXFileFormatTypeMaterial    8<<8      
#define PXFileFormatTypeCode        9<<8
#define PXFileFormatTypeShader      10<<8  
#define PXFileFormatTypeIcon        11<<8
#define PXFileFormatTypeDocument    12<<8
#define PXFileFormatTypeBinary      13<<8
#define PXFileFormatTypeArchiv      14<<8  // Compressed object
//-----------------------------------------------------


typedef PXResult(PXAPI* PXResourceFileSizePredict)(void PXREF resource, PXSize PXREF fileSize);
typedef PXResult(PXAPI* PXResourceTransphereFunction)(struct PXECSInfo_ PXREF pxECSInfo, struct PXECSCreateInfo_ PXREF PXECSCreateInfo);

//---------------------------------------------------------
// Contains all info about a file
typedef struct PXFileFormatInfo_
{
    PXText Extension;
    PXText Siganture;

    PXI8U SigantureOffset;

    PXI32U Flags; // Behaviour

    //PXFileFormat FormatID;

    //PXI32U ResourceType;

    PXResourceTransphereFunction ResourcePeek;
    PXResourceTransphereFunction ResourceLoad;
    PXResourceTransphereFunction ResourceSave;
}
PXFileFormatInfo;
//---------------------------------------------------------

PXPublic PXResult PXAPI PXFileFormatInfoViaPath(PXFileFormatInfo PXREF pxFileFormatInfo, const PXText PXREF filePath);
PXPublic PXResult PXAPI PXFileFormatInfoViaContent(PXFileFormatInfo PXREF pxFileFormatInfo, PXFile PXREF pxFile);

PXPublic PXResult PXAPI PXFileTypeInfoProbe(PXFileFormatInfo PXREF pxFileFormatInfo, const PXText PXREF pxText);


#endif