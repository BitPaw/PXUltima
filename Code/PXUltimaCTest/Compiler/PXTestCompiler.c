#include "PXTestCompiler.h"

#include <PX/OS/File/PXFile.h>

#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/YAML/PXYAML.h>
#include <PX/Media/XML/PXXML.h>
#include <PX/Media/Wavefront/PXWavefront.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/PXDocument.h>
#include <PX/OS/Console/PXConsole.h>

void PXAPI PXTestCompilerAll()
{
    PXTestCompilerCToLanguages();
    PXTestCompilerOBJ("");
    PXTestCompilerYAML();
    PXTestCompilerXML();
}

void PXAPI PXTestCompilerCToLanguages()
{
    /*
    PXCodeDocument pxCodeDocument;

    {
        PXResourceTransphereInfo pxResourceLoadInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
        pxResourceLoadInfo.Type = PXResourceTypeCodeDocument;
        pxResourceLoadInfo.Target = &pxCodeDocument;

        PXActionResult resA = PXResourceLoadA(&pxResourceLoadInfo, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaC\\Engine\\PXEngine.h");
    }

    {
        PXResourceTransphereInfo pxResourceSaveInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceSaveInfo);
        pxResourceSaveInfo.Type = PXResourceTypeCodeDocument;
        pxResourceSaveInfo.Target = &pxCodeDocument;
        pxResourceSaveInfo.Format = PXFileFormatJava;

        PXActionResult resA = PXResourceSaveA(&pxResourceSaveInfo, "N:\\Cache\\PXTest.java");

        pxResourceSaveInfo.Format = PXFileFormatCSharp;

        PXActionResult resB = PXResourceSaveA(&pxResourceSaveInfo, "N:\\Cache\\PXTest.cs");

        PXConsoleWrite(0, 0);
    }
    */
    
}

void PXAPI PXTestCompilerOBJ(char* filePath)
{
    /*
    PXFile inputStream;
    PXFile outputStream;

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.FileSize = 0;
        pxFileOpenFromPathInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, filePath);

        PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
    }

    PXFileMapToMemory(&outputStream, inputStream.DataSize * 4, PXAccessModeReadAndWrite);

    PXWavefrontFileCompile(&inputStream, &outputStream);

    PXModel model;

    PXWavefrontParseToModel(&outputStream, &model);

    printf("____\n");
    */
}

void PXAPI PXTestCompilerYAML()
{
    /*
    PXSize size = 1024 * 2;
    char buffer[1024 * 2];

    PXMemoryClear(buffer, size);

    PXFile inputStream;
    PXFile outputStream;


    PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
    pxFileOpenFromPathInfo.FileSize;
    pxFileOpenFromPathInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenFromPathInfo.AllowMapping = PXTrue;
    pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
    pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

    PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "A:/config.yml");

    PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);


    PXFileConstruct(&outputStream);
    PXFileBufferExternal(&outputStream, buffer, size);

    PXCompilerSettings compilerSettings;

    //compilerSettings. = 1;

    PXCompilerLexicalAnalysis(&inputStream, &outputStream, &compilerSettings);

    printf("____\n");
    */
}

void PXAPI PXTestCompilerYAML_2()
{
    /*
    PXFile inputStream;
    PXFile compiledStream;

    struct MyStroage
    {
        unsigned int ID;
        char Name[16];
    }MyStroage;

    PXSerializationTypeInfo serializationTypeInfo[2];

    PXSerializationTypeInfoSet(&serializationTypeInfo[0], "ID", &MyStroage.ID, sizeof(MyStroage.ID), SerializationTypeIntegerUnsigned32, 0, 0);
    PXSerializationTypeInfoSet(&serializationTypeInfo[1], "Name", &MyStroage.Name, sizeof(MyStroage.Name), SerializationTypeStringASCII, 0, 0);


    // Store
    //------------------------------

    //----<Load>--------------------------
    PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
    pxFileOpenFromPathInfo.FileSize;
    pxFileOpenFromPathInfo.AccessMode = PXAccessModeReadOnly;
    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenFromPathInfo.AllowMapping = PXTrue;
    pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
    pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

    PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "A:/config.yml");

    PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
    PXFileMapToMemory(&compiledStream, inputStream.DataSize * 7, PXAccessModeReadAndWrite);

    PXYAMLFileCompile(&inputStream, &compiledStream);

    PXYAMLFileParse(&compiledStream, &serializationTypeInfo, 2u);
    //------------------------------


    printf("finish\n");
    */
}

void PXAPI PXTestCompilerXML()
{
    /*
    const char xmlData[] =
        "<part number=\"1976\">\n"
        "<name>Windscreen Wiper</name>\n"
        "<description>The Windscreen wiper\n"
        "automatically removes rain\n"
        "from your windscreen, if it\n"
        "should happen to splash there.\n"
        "It has a rubber <ref part=\"1977\">blade</ref>\n"
        "which can be ordered separately\n"
        "if you need to replace it.\n"
        "</description>\n"
        "</part>\n";

    PXFile inputStream;
    PXFile outputStream;

    PXFileConstruct(&inputStream);
    PXFileConstruct(&outputStream);

#if 1
    PXFileBufferExternal(&inputStream, xmlData, 299 - 1);
#else
    PXFileMapToMemoryA(&inputStream, "A:/index.php", PXAccessModeReadOnly, FileCachingSequential);
#endif
    PXFileMapToMemory(&outputStream, inputStream.DataSize * 8, PXAccessModeReadAndWrite);

    PXXMLFileCompile(&inputStream, &outputStream);

    printf("____\n");
    */
}