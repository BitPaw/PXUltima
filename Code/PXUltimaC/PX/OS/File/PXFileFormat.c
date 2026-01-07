#include "PXFileFormat.h"

#include <PX/OS/File/PXFileFormat.h>
#include <PX/OS/Console/PXConsole.h>


#include <PX/Media/Autodesk3DS/PXAutodesk3DS.h>
#include <PX/Media/AAC/PXAAC.h>
#include <PX/Media/AVI/PXAVI.h>
#include <PX/Media/Bitmap/PXBitmap.h>
#include <PX/Media/FilmBox/PXFilmBox.h>
#include <PX/Media/FLAC/PXFLAC.h>
#include <PX/Media/GIF/PXGIF.h>
#include <PX/Media/JPEG/PXJPEG.h>
#include <PX/Media/MIDI/PXMIDI.h>
#include <PX/Media/MP3/PXMP3.h>
#include <PX/Media/MP4/PXMP4.h>
#include <PX/Media/OGG/PXOGG.h>
#include <PX/Media/PLY/PXPLY.h>
#include <PX/Media/PNG/PXPNG.h>
#include <PX/Media/SpriteFont/PXSpriteFont.h>
#include <PX/Media/STEP/PXSTEP.h>
#include <PX/Media/STL/PXSTL.h>
#include <PX/Media/SVG/PXSVG.h>
#include <PX/Media/HEIF/PXHEIF.h>
#include <PX/Media/HTML/PXHTML.h>
#include <PX/Media/JSON/PXJSON.h>
#include <PX/Media/INI/PXINI.h>
#include <PX/Media/WEBM/PXWEBM.h>
#include <PX/Media/WEBP/PXWEBP.h>
#include <PX/Media/Wavefront/PXWavefront.h>
#include <PX/Media/Matroska/PXMatroska.h>
#include <PX/Media/PDF/PXPDF.h>
#include <PX/Media/TTF/PXTTF.h>
#include <PX/Media/VRML/PXVRML.h>
#include <PX/Media/N64/PXN64.h>
#include <PX/Media/PHP/PXPHP.h>
#include <PX/Media/MSI/PXMSI.h>
#include <PX/Media/Wave/PXWave.h>
#include <PX/Media/WMA/PXWMA.h>
#include <PX/Media/XML/PXXML.h>
#include <PX/Media/TIFF/PXTIFF.h>
#include <PX/Media/TGA/PXTGA.h>
#include <PX/Media/YAML/PXYAML.h>
#include <PX/Media/BinaryWindows/PXBinaryWindows.h>
#include <PX/Media/BinaryLinux/PXBinaryLinux.h>
#include <PX/Media/CanonRaw3/PXCanonRaw3.h>
#include <PX/Media/RAR/PXRAR.h>
#include <PX/Media/C/PXC.h>
#include <PX/Media/CSharp/PXCSharp.h>
#include <PX/Media/CPP/PXCPP.h>
#include <PX/Media/CSS/PXCSS.h>
#include <PX/Media/DDS/PXDDS.h>
#include <PX/Media/FastFile/PXFastFile.h>
#include <PX/Media/ADLER/PXAdler32.h>
#include <PX/Media/Java/PXJava.h>
#include <PX/Media/ZIP/PXZIP.h>
#include <PX/Media/USD/PXUSD.h>
#include <PX/Media/VOB/PXVOB.h>
#include <PX/Media/JavaScript/PXJavaScript.h>
#include <PX/Media/TAR/PXTAR.h>
#include <PX/Media/GLSL/PXGLSL.h>
#include <PX/Media/HLSL/PXHLSL.h>
#include <PX/Media/WAD/PXWAD.h>
#include <PX/Media/RTexture/PXRTexture.h>
#include <PX/Media/RGeometricMap/PXRGeometricMap.h>
#include <PX/Media/RGeometricWorld/PXRGeometricWorld.h>
#include <PX/Media/RSkin/PXRSkin.h>
#include <PX/Media/RedshiftMesh/PXRedshiftMesh.h>
#include <PX/Media/RedshiftSkeleton/PXRedshiftSkeleton.h>
#include <PX/Media/RedshiftAnimation/PXRedshiftAnimation.h>





const char PXFileFormatText[] = "FileFormat";

const PXByte PXFileFormatRSkinedMeshSignature[] = { 0xC3, 0x4F, 0xFD, 0x22 };
const PXByte PXFileFormatRSkinedMeshSignatureLength = sizeof(PXFileFormatRSkinedMeshSignature) / sizeof(PXByte);

const PXByte PXSignature6D[] = { 0x6D, 0x8D, 0xBF, 0x30 };
const PXByte PXSignature6DLength = sizeof(PXSignature6D) / sizeof(PXByte);

const PXByte PXFileFormatRSkinSimpleSignature[4] = { 0x33, 0x22, 0x11, 0x00 };
const PXByte PXFileFormatRSkinSimpleSignatureLength = sizeof(PXFileFormatRSkinSimpleSignature) / sizeof(PXByte);

const PXByte PXFileFormatRLightGridSignature[] = { 0x03, 0x00, 0x00, 0x00 };;
const PXByte PXFileFormatRLightGridSignatureLength = sizeof(PXFileFormatRLightGridSignature) / sizeof(PXByte);

const char PXExtensionUnkown[] = "ooo";

PXResult PXAPI PXFileFormatInfoViaPath(PXFileFormatInfo PXREF pxFileFormatInfo, const PXText PXREF filePath)
{
    PXText pxTextExtension;
    PXTextConstructNamedBufferA(&pxTextExtension, extensionBuffer, ExtensionMaxSize);

    const PXSize writtenBytes = PXFilePathExtensionGet(filePath, &pxTextExtension);

    PXTextToUpperCase(&pxTextExtension, &pxTextExtension);

#if PXLogEnable
    char path[PXPathSizeMax];
    char extension[PXPathSizeMax];

    PXTextCopyA(filePath->A, filePath->SizeUsed, path, PXPathSizeMax);
    PXTextCopyA(pxTextExtension.A, pxTextExtension.SizeUsed, extension, PXPathSizeMax);

    PXLogPrint
    (
        PXLoggingInfo,
        PXFileFormatText,
        "Extension",
        "Detect:\n"
        "%20s : %s\n"
        "%20s : %s",
        "Path", path,
        "Extension", extension
    );
#endif

    pxFileFormatInfo->Flags = PXFileFormatUnkown;

    switch(writtenBytes)
    {
        case 0:
            pxFileFormatInfo->Flags = PXFileFormatInvalid;
            break;

        case 1u:
        {
            switch(*pxTextExtension.A)
            {
                case 'H':
                    pxFileFormatInfo->Flags = PXFileFormatC;
                    break;
                case 'C':
                    pxFileFormatInfo->Flags = PXFileFormatC;
                    break;
                case 'O':
                    pxFileFormatInfo->Flags = PXFileFormatBinaryLinux;
                    break;
            }

            break;
        }
        case 2u:
        {
            const PXI32U list = PXI16FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI16Make('K', 'O'):
                case PXI16Make('S', 'O'):
                    pxFileFormatInfo->Flags = PXFileFormatBinaryLinux;
                    break;
                case PXI16Make('F', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatFastFile;
                    break;
                case PXI16Make('C', 'S'):
                    pxFileFormatInfo->Flags = PXFileFormatCSharp;
                    break;
                case PXI16Make('J', 'S'):
                    pxFileFormatInfo->Flags = PXFileFormatJavaScript;
                    break;
            }

            break;
        }
        case 3u:
        {
            const PXI32U list = PXI24FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI24Make('V', 'O', 'B'):
                case PXI24Make('I', 'F', 'O'):
                case PXI24Make('B', 'U', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatVideoObject;
                    break;

                case PXI24Make('V', '6', '4'):
                case PXI24Make('Z', '6', '4'):
                case PXI24Make('N', '6', '4'):
                    pxFileFormatInfo->Flags = PXFileFormatN64;
                    break;

                case PXI24Make('C', 'P', 'P'):
                case PXI24Make('H', 'P', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatCPP;
                    break;

                case PXI24Make('C', 'R', '3'):
                    pxFileFormatInfo->Flags = PXFileFormatCanonRaw3;
                    break;

                case PXI24Make('B', 'K', '2'):
                case PXI24Make('B', 'I', 'K'):
                    pxFileFormatInfo->Flags = PXFileFormatBinkVideo;
                    break;

                case PXI24Make('D', 'D', 'S'):
                    pxFileFormatInfo->Flags = PXFileFormatDirectDrawSurfaceTexture;
                    break;

                case PXI24Make('B', 'I', 'N'):
                case PXI24Make('P', 'R', 'X'):
                case PXI24Make('M', 'O', 'D'):
                case PXI24Make('E', 'L', 'F'):
                case PXI24Make('O', 'U', 'T'):
                    pxFileFormatInfo->Flags = PXFileFormatBinaryLinux;
                    break;

                case PXI24Make('R', 'A', 'R'):
                    pxFileFormatInfo->Flags = PXFileFormatEugeneRoshalArchive;
                    break;

                case PXI24Make('F', 'N', 'T'):
                    pxFileFormatInfo->Flags = PXFileFormatSpriteFont;
                    break;

                case PXI24Make('G', 'I', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatGIF;
                    break;
                case PXI24Make('H', 'T', 'M'):
                    pxFileFormatInfo->Flags = PXFileFormatHTML;
                    break;
                case PXI24Make('I', 'N', 'I'):
                    pxFileFormatInfo->Flags = PXFileFormatINI;
                    break;
                case PXI24Make('M', '4', 'A'):
                    pxFileFormatInfo->Flags = PXFileFormatM4A;
                    break;
                case PXI24Make('3', 'D', 'S'):
                    pxFileFormatInfo->Flags = PXFileFormatA3DS;
                    break;
                case PXI24Make('A', 'C', 'C'):
                    pxFileFormatInfo->Flags = PXFileFormatAAC;
                    break;
                case PXI24Make('A', 'V', 'I'):
                    pxFileFormatInfo->Flags = PXFileFormatAVI;
                    break;
                case PXI24Make('B', 'M', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatBitMap;
                    break;
                case PXI24Make('C', 'S', 'S'):
                    pxFileFormatInfo->Flags = PXFileFormatCSS;
                    break;
                case PXI24Make('E', 'M', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatEML;
                    break;
                case PXI24Make('S', 'Y', 'S'):
                case PXI24Make('C', 'O', 'M'):
                case PXI24Make('D', 'L', 'L'):
                case PXI24Make('E', 'X', 'E'):
                    pxFileFormatInfo->Flags = PXFileFormatBinaryWindows;
                    break;
                case PXI24Make('F', 'B', 'X'):
                    pxFileFormatInfo->Flags = PXFileFormatFilmBox;
                    break;
                case PXI24Make('M', 'P', '3'):
                    pxFileFormatInfo->Flags = PXFileFormatMP3;
                    break;
                case PXI24Make('M', 'P', '4'):
                    pxFileFormatInfo->Flags = PXFileFormatMP4;
                    break;
                case PXI24Make('M', 'S', 'I'):
                    pxFileFormatInfo->Flags = PXFileFormatMSI;
                    break;
                case PXI24Make('M', 'T', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatMTL;
                    break;
                case PXI24Make('O', 'B', 'J'):
                    pxFileFormatInfo->Flags = PXFileFormatWavefront;
                    break;
                case PXI24Make('M', 'K', 'V'):
                    pxFileFormatInfo->Flags = PXFileFormatMatroska;
                    break;
                case PXI24Make('O', 'G', 'G'):
                    pxFileFormatInfo->Flags = PXFileFormatOGG;
                    break;
                case PXI24Make('P', 'D', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatPDF;
                    break;
                case PXI24Make('P', 'H', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatPHP;
                    break;
                case PXI24Make('P', 'L', 'Y'):
                    pxFileFormatInfo->Flags = PXFileFormatPLY;
                    break;
                case PXI24Make('P', 'N', 'G'):
                    pxFileFormatInfo->Flags = PXFileFormatPNG;
                    break;
                case PXI24Make('Q', 'U', 'I'):
                    pxFileFormatInfo->Flags = PXFileFormatQOI;
                    break;
                case PXI24Make('S', 'T', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatSTL;
                    break;
                case PXI24Make('S', 'V', 'G'):
                    pxFileFormatInfo->Flags = PXFileFormatSVG;
                    break;
                case PXI24Make('T', 'I', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatTagImage;
                    break;
                case PXI24Make('U', 'S', 'D'):
                    pxFileFormatInfo->Flags = PXFileFormatUniversalSceneDescription;
                    break;
                case PXI24Make('T', 'G', 'A'):
                    pxFileFormatInfo->Flags = PXFileFormatTGA;
                    break;
                case PXI24Make('T', 'T', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatTrueTypeFont;
                    break;
                case PXI24Make('W', 'A', 'D'):
                    pxFileFormatInfo->Flags = PXFileFormatWAD;
                    break;
                case PXI24Make('W', 'A', 'V'):
                    pxFileFormatInfo->Flags = PXFileFormatWave;
                    break;
                case PXI24Make('W', 'M', 'A'):
                    pxFileFormatInfo->Flags = PXFileFormatWMA;
                    break;
                case PXI24Make('X', 'M', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatXML;
                    break;
                case PXI24Make('Y', 'M', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatYAML;
                    break;
                case PXI24Make('Z', 'I', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatZIP;
                    break;
            }

            break;
        }
        case 4u:
        {
            const PXI32U list = PXI32FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI32Make('B', 'I', 'K', '2'):
                    pxFileFormatInfo->Flags = PXFileFormatBinkVideo;
                    break;
                case PXI32Make('J', 'A', 'V', 'A'):
                    pxFileFormatInfo->Flags = PXFileFormatJava;
                    break;
                case PXI32Make('H', 'E', 'I', 'C'): // Fall though
                case PXI32Make('H', 'E', 'I', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatHighEfficiencyImageFile;
                    break;
                case PXI32Make('P', 'U', 'F', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatBinaryLinux;
                    break;
                case PXI32Make('F', 'L', 'A', 'C'):
                    pxFileFormatInfo->Flags = PXFileFormatFLAC;
                    break;
                case PXI32Make('M', 'I', 'D', 'I'):
                    pxFileFormatInfo->Flags = PXFileFormatMIDI;
                    break;
                case PXI32Make('S', 'T', 'E', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatSTEP;
                    break;
                case PXI32Make('T', 'I', 'F', 'F'):
                    pxFileFormatInfo->Flags = PXFileFormatTagImage;
                    break;
                case PXI32Make('J', 'P', 'E', 'G'):
                    pxFileFormatInfo->Flags = PXFileFormatJPEG;
                    break;
                case PXI32Make('J', 'S', 'O', 'N'):
                    pxFileFormatInfo->Flags = PXFileFormatJSON;
                    break;
                case PXI32Make('V', 'R', 'M', 'L'):
                    pxFileFormatInfo->Flags = PXFileFormatVRML;
                    break;
                case PXI32Make('W', 'E', 'B', 'M'):
                    pxFileFormatInfo->Flags = PXFileFormatWEBM;
                    break;
                case PXI32Make('W', 'E', 'B', 'P'):
                    pxFileFormatInfo->Flags = PXFileFormatWEBP;
                    break;
            }

            break;
        }
    }

    if(pxFileFormatInfo->Flags != PXFileFormatUnkown)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFileFormatText,
            "Extension",
            "Sucessfully detected!"
        );
#endif

        return PXActionSuccessful;
    }


    // When we did not detect any format, it could be, that some nice guy added
    // an additional "fake" extension. 
    // Example: Instead of "myfile.ext" we get "myfile.ext"

    // Lets find the secound dot.
    PXSize firstDot = PXTextFindLastCharacter(filePath, '.');

    if(firstDot != -1)
    {
        PXText pxText;
        PXTextFromAdressA(&pxText, filePath->A, firstDot, firstDot);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXFileFormatText,
            "Extension",
            "Unkown extension! Two dots are detected, we might have a fake extension"
        );
#endif

        return PXFileFormatInfoViaPath(pxFileFormatInfo, &pxText);
    }

    pxFileFormatInfo->Flags = PXFileFormatUnkown;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingError,
        PXFileFormatText,
        "Extension",
        "Could not resolve extension"
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXFileFormatInfoViaContent(PXFileFormatInfo PXREF pxFileFormatInfo, PXFile PXREF pxFile)
{
    PXClear(PXFileFormatInfo, pxFileFormatInfo);

    const PXFileFormatInfo pxFileFormatInfoList[] =
    {
        {"scb", 6, "r3d2Mesh", 8, 0, PXFileFormatRedshiftMesh | PXFileFormatVarriantBinary},
        {"anm", 6, "r3d2anmd", 8, 0, PXFileFormatRedshiftAnimation | PXFileFormatVarriantBinary},
        {"anm", 6, "r3d2canm", 8, 0, PXFileFormatRedshiftAnimation | PXFileFormatVarriantBinary},
        {"skl", 6, "r3d2sklt", 8, 0, PXFileFormatRedshiftSkeleton | PXFileFormatVarriantBinary},
        {"dds", 3, "DDS", 3, 0, PXFileFormatDirectDrawSurfaceTexture | PXFileFormatVarriantBinary},
        {"skn", 4, PXFileFormatRSkinSimpleSignature, PXFileFormatRSkinSimpleSignatureLength, 0, PXFileFormatRSkinSimple},
        {"bin", 4, "PROP", 4, 0, PXFileFormatRProperty },
        {"bnk", 3, "BKHD", 4, 0, PXFileFormatWAudioBank},
        {"wgeo", 3, "WGEO", 4, 0, PXFileFormatRGeometryWorld},
        {"mapgeo", 3, "OEGM", 4, 0, PXFileFormatRGeometryMap},
        {"sco", 3, "[ObjectBegin]", 13, 0, PXFileFormatRedshiftMesh | PXFileFormatVarriantText},
        {"luaobj", 3, "LuaQ", 4, 0, PXFileFormatLua},
        {PXExtensionUnkown, 3, "PreLoad", 7, 0, PXFileFormatRPreLoad },
        {PXExtensionUnkown, 3, PXFileFormatRLightGridSignature, PXFileFormatRLightGridSignatureLength, 0, PXFileFormatRLightGrid },
        {PXExtensionUnkown, 3, "RST", 3, 0, PXFileFormatRStringTable },
        {PXExtensionUnkown, 3, "PTCH", 4, 0, PXFileFormatRPropertyOverride},
        {PXExtensionUnkown, 3, PXFileFormatRSkinedMeshSignature, PXFileFormatRSkinedMeshSignatureLength, 0, PXFileFormatRSkinedMesh },
        {"tex", 3, "TEX", 3, 0, PXFileFormatRTexture},
        {PXExtensionUnkown, 8, PXSignature6D, PXSignature6DLength, 4, PXFileFormat6D},
        {"oegm", 8, "OEGM", 4, 0, PXFileFormatOEGM},
        {"opam", 8, "OPAM", 4, 0, PXFileFormatOEGM},

    };
    const PXI8U amount = sizeof(pxFileFormatInfoList) / sizeof(PXFileFormatInfo);


    for(size_t i = 0; i < amount; ++i)
    {
        const PXFileFormatInfo PXREF pxFileFormatInfoComp = &pxFileFormatInfoList[i];

        const PXBool isMatch = PXTextCompareA
        (
            pxFileFormatInfoComp->Siganture.A,
            pxFileFormatInfoComp->Siganture.SizeUsed,
            PXFileDataAtCursorWithOffset(pxFile, pxFileFormatInfoComp->SigantureOffset),
            PXFileAllocatedSize(pxFile),
            0
        );

        if(isMatch)
        {
            PXCopy(PXFileFormatInfo, pxFileFormatInfoComp, pxFileFormatInfo);
            return PXActionSuccessful;
        }
    }

    PXClear(PXFileFormatInfo, pxFileFormatInfo);
    pxFileFormatInfo->Flags = PXFileFormatUnkown;

    return PXActionSuccessful;
}

PXResult PXAPI PXFileTypeInfoProbe(PXFileFormatInfo PXREF pxFileFormatInfo, const PXText PXREF pxText)
{
    // Probe for file extension
    if(pxText)
    {
        PXFileFormatInfoViaPath(pxFileFormatInfo, pxText);
    }

    switch(PXFileFormatMask & pxFileFormatInfo->Flags)
    {
        case PXFileFormatA3DS:
            pxFileFormatInfo->Flags = PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXAutodesk3DSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAutodesk3DSSafeFromFile;
            pxFileFormatInfo->Extension.A = "3DS";
            break;

        case PXFileFormatAAC:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXAACLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAACSaveToFile;
            pxFileFormatInfo->Extension.A = "ACC";
            break;

        case PXFileFormatAVI:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXAVILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAVISaveToFile;
            pxFileFormatInfo->Extension.A = "AVI";
            break;

        case PXFileFormatBitMap:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourcePeek = PXBitmapPeekFromFile;
            pxFileFormatInfo->ResourceLoad = PXBitmapLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBitmapSaveToFile;
            pxFileFormatInfo->Extension.A = "BMP";
            break;

        case PXFileFormatCanonRaw3:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXCanonRaw3LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCanonRaw3SaveToFile;
            pxFileFormatInfo->Extension.A = "CR3";
            break;

        case PXFileFormatC:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSaveToFile;
            pxFileFormatInfo->Extension.A = "C";
            break;

        case PXFileFormatCSharp:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCSharpLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSharpSaveToFile;
            pxFileFormatInfo->Extension.A = "CS";
            break;

        case PXFileFormatCSS:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXCSSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSSSaveToFile;
            pxFileFormatInfo->Extension.A = "CSS";
            break;

        case PXFileFormatCPP:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCPPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCPPSaveToFile;
            pxFileFormatInfo->Extension.A = "CPP";
            break;

        case PXFileFormatBinaryWindows:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXBinaryWindowsLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBinaryWindowsSaveToFile;
            pxFileFormatInfo->Extension.A = "EXE";
            break;

        case PXFileFormatBinaryLinux:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXBinaryLinuxLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBinaryLinuxSaveToFile;
            pxFileFormatInfo->Extension.A = "ELF";
            break;

        case PXFileFormatDirectDrawSurfaceTexture:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXDDSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXDDSSaveToFile;
            pxFileFormatInfo->Extension.A = "DDS";
            break;

        case PXFileFormatEML:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->Extension.A = "EML";
            break;

        case PXFileFormatFastFile:
            pxFileFormatInfo->Flags |= PXFileFormatTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXFastFileLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFastFileSaveToFile;
            pxFileFormatInfo->Extension.A = "FF";
            break;

        case PXFileFormatFilmBox:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXFilmBoxLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFilmBoxSaveToFile;
            pxFileFormatInfo->Extension.A = "FBX";
            break;

        case PXFileFormatFLAC:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXFLACLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFLACSaveToFile;
            pxFileFormatInfo->Extension.A = "FLAC";
            break;

        case PXFileFormatSpriteFont:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXSpriteFontLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSpriteFontSaveToFile;
            pxFileFormatInfo->Extension.A = "FNT";
            break;

        case PXFileFormatGIF:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXGIFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXGIFSaveToFile;
            pxFileFormatInfo->Extension.A = "GIF";
            break;

        case PXFileFormatOpenGLShader:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXGLSLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXGLSLSaveToFile;
            pxFileFormatInfo->Extension.A = "GLSL";
            break;

        case PXFileFormatDirectXShader:
            pxFileFormatInfo->Flags = PXResourceTypeShaderProgram;
            pxFileFormatInfo->ResourceLoad = PXHLSLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHLSLSaveToFile;
            pxFileFormatInfo->Extension.A = "HLSL";
            break;

        case PXFileFormatHighEfficiencyImageFile:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXHEIFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHEIFSaveToFile;
            break;

        case PXFileFormatHTML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXHTMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHTMLSaveToFile;
            pxFileFormatInfo->Extension.A = "HTML";
            break;

        case PXFileFormatINI:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXINILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXINISaveToFile;
            pxFileFormatInfo->Extension.A = "INI";
            break;

        case PXFileFormatJava:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXJavaLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJavaSaveToFile;
            pxFileFormatInfo->Extension.A = "JAR";
            break;

        case PXFileFormatJavaScript:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXJavaScriptLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJavaScriptSaveToFile;
            break;

        case PXFileFormatJPEG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXJPEGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJPEGSaveToFile;
            pxFileFormatInfo->Extension.A = "JPG";
            break;

        case PXFileFormatJSON:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXJSONLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJSONSaveToFile;
            pxFileFormatInfo->Extension.A = "JSON";
            break;

        case PXFileFormatM4A:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->Extension.A = "M4A";
            break;

        case PXFileFormatMIDI:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXMIDILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMIDISaveToFile;
            pxFileFormatInfo->Extension.A = "MIDI";
            break;

        case PXFileFormatMP3:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXMP3LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMP3SaveToFile;
            pxFileFormatInfo->Extension.A = "MP3";
            break;

        case PXFileFormatMP4:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXMP4LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMP4SaveToFile;
            pxFileFormatInfo->Extension.A = "MP4";
            break;

        case PXFileFormatMSI:
            pxFileFormatInfo->Flags = PXResourceTypeInstaller;
            pxFileFormatInfo->ResourceLoad = PXMSILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMSISaveToFile;
            pxFileFormatInfo->Extension.A = "MSI";
            break;

        case PXFileFormatMTL:
            pxFileFormatInfo->Flags = PXResourceTypeMaterialList;
            pxFileFormatInfo->ResourceLoad = PXMTLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMTLSaveToFile;
            pxFileFormatInfo->Extension.A = "MTL";
            break;

        case PXFileFormatN64:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXN64LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXN64SaveToFile;
            pxFileFormatInfo->Extension.A = "N64";
            break;

        case PXFileFormatWavefront:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXWavefrontLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWavefrontSaveFromFile;
            pxFileFormatInfo->Extension.A = "WAV";
            break;

        case PXFileFormatMatroska:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXMatroskaLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMatroskaSaveToFile;
            pxFileFormatInfo->Extension.A = "";
            break;

        case PXFileFormatOGG:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXOGGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXOGGSaveToFile;
            pxFileFormatInfo->Extension.A = "OGG";
            break;

        case PXFileFormatEugeneRoshalArchive:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXRARLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRARSaveToFile;
            pxFileFormatInfo->Extension.A = "RAR";
            break;

        case PXFileFormatPDF:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXPDFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPDFSaveToFile;
            pxFileFormatInfo->Extension.A = "PDF";
            break;

        case PXFileFormatPHP:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXPHPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPHPSaveToFile;
            pxFileFormatInfo->Extension.A = "PHP";
            break;

        case PXFileFormatPLY:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXPLYLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPLYSaveToFile;
            pxFileFormatInfo->Extension.A = "PLY";
            break;

        case PXFileFormatPNG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            //pxFileFormatInfo->FileSizePredict = PXPNGFilePredictSize;
            pxFileFormatInfo->ResourcePeek = PXPNGPeekFromFile;
            pxFileFormatInfo->ResourceLoad = PXPNGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPNGSaveToFile;
            pxFileFormatInfo->Extension.A = "PNG";
            break;

        case PXFileFormatQOI:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->Extension.A = "QOI";
            break;

        case PXFileFormatSTEP:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXSTEPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSTEPSaveToFile;
            pxFileFormatInfo->Extension.A = "STEP";
            break;

        case PXFileFormatSTL:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXSTLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSTLSaveToFile;
            pxFileFormatInfo->Extension.A = "STL";
            break;

        case PXFileFormatSVG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXSVGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSVGSaveToFile;
            pxFileFormatInfo->Extension.A = "SVG";
            break;

        case PXFileFormatTAR:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXTARLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTARSaveToFile;
            pxFileFormatInfo->Extension.A = "TAR";
            break;

        case PXFileFormatTGA:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXTGALoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTGASaveToFile;
            pxFileFormatInfo->Extension.A = "TGA";
            break;

        case PXFileFormatTagImage:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXTIFFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTIFFSaveToFile;
            pxFileFormatInfo->Extension.A = "TIFF";
            break;

        case PXFileFormatTrueTypeFont:
            pxFileFormatInfo->Flags = PXResourceTypeFont;
            pxFileFormatInfo->ResourceLoad = PXTTFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTTFSaveToFile;
            pxFileFormatInfo->Extension.A = "TTF";
            break;

        case PXFileFormatUniversalSceneDescription:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXUSDLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXUSDSaveToFile;
            pxFileFormatInfo->Extension.A = "UCD";
            break;

        case PXFileFormatVideoObject:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXVOBLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXVOBLSaveToFile;
            pxFileFormatInfo->Extension.A = "";
            break;

        case PXFileFormatVRML:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXVRMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXVRMLSaveToFile;
            pxFileFormatInfo->Extension.A = "VRML";
            break;

        case PXFileFormatWAD:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXWADLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWADSaveToFile;
            pxFileFormatInfo->Extension.A = "WAD";
            break;

        case PXFileFormatWave:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXWaveLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWaveSaveToFile;
            pxFileFormatInfo->Extension.A = "WAV";
            break;

        case PXFileFormatWEBM:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXWEBMLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWEBMSaveToFile;
            pxFileFormatInfo->Extension.A = "WEBM";
            break;

        case PXFileFormatWEBP:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXWEBPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWEBPSaveToFile;
            pxFileFormatInfo->Extension.A = "WEBP";
            break;

        case PXFileFormatWMA:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXWMALoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWMASaveToFile;
            pxFileFormatInfo->Extension.A = "WMA";
            break;

        case PXFileFormatXML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXXMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXXMLSaveToFile;
            pxFileFormatInfo->Extension.A = "XML";
            break;

        case PXFileFormatYAML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXYAMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXYAMLSaveToFile;
            pxFileFormatInfo->Extension.A = "YML";
            break;

        case PXFileFormatZIP:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXZIPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXZIPSaveToFile;
            pxFileFormatInfo->Extension.A = "ZIP";
            break;


            // NEW

        case PXFileFormatRedshiftMesh:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXR3D2LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXR3D2SaveToFile;
            pxFileFormatInfo->Extension.A = "R3D2";
            break;

        case PXFileFormatRedshiftSkeleton:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRedshiftSkeletonLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRedshiftSkeletonSaveToFile;
            pxFileFormatInfo->Extension.A = "SKL";
            break;

        case PXFileFormatRedshiftAnimation:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRedshiftAnimationLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRedshiftAnimationSaveToFile;
            pxFileFormatInfo->Extension.A = "ANM";
            break;

        case PXFileFormatRGeometryMap:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRGeometricMapLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRGeometricMapSaveToFile;
            pxFileFormatInfo->Extension.A = "MAPGEO";
            break;

        case PXFileFormatRGeometryWorld:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRGeometricWorldLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRGeometricWorldSaveToFile;
            pxFileFormatInfo->Extension.A = "WGEO";
            break;

        case PXFileFormatRSkinSimple:
            pxFileFormatInfo->Flags |= PXFileFormatTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXRSkinLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRSkinSaveToFile;
            pxFileFormatInfo->Extension.A = "SKN";
            break;

        case PXFileFormatRTexture:
            pxFileFormatInfo->Flags |= PXFileFormatTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXRTextureLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRTextureSaveToFile;
            pxFileFormatInfo->Extension.A = "TEX";
            break;

        default:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            break;
    }

    return PXActionSuccessful;
}
