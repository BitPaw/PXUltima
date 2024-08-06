#include "PXTestCompiler.h"

#include <OS/File/PXFile.h>

#include <Compiler/PXCompiler.h>
#include <Media/YAML/PXYAML.h>
#include <Media/XML/PXXML.h>
#include <Media/Wavefront/PXWavefront.h>
#include <Compiler/PXCompiler.h>
#include <Media/PXDocument.h>
#include <OS/Console/PXConsole.h>

void PXAPI PXTestCompilerAll()
{
    PXTestCompilerCToLanguages();
    PXTestCompilerOBJ("");
    PXTestCompilerYAML();
    PXTestCompilerXML();
}

void PXAPI PXTestCompilerCToLanguages()
{
    
    PXCodeDocument pxCodeDocument;

    {
        PXResourceLoadInfo pxResourceLoadInfo;
        PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
        pxResourceLoadInfo.Type = PXResourceTypeCodeDocument;
        pxResourceLoadInfo.Target = &pxCodeDocument;

        PXActionResult resA = PXResourceLoadA(&pxResourceLoadInfo, "C:\\Data\\WorkSpace\\[GIT]\\PXUltima\\Code\\PXUltimaC\\Engine\\PXEngine.h");
    }

    {
        PXResourceSaveInfo pxResourceSaveInfo;
        PXClear(PXResourceLoadInfo, &pxResourceSaveInfo);
        pxResourceSaveInfo.Type = PXResourceTypeCodeDocument;
        pxResourceSaveInfo.Target = &pxCodeDocument;
        pxResourceSaveInfo.Format = PXFileFormatJava;

        PXActionResult resA = PXResourceSaveA(&pxResourceSaveInfo, "N:\\Cache\\PXTest.java");

        pxResourceSaveInfo.Format = PXFileFormatCSharp;

        PXActionResult resB = PXResourceSaveA(&pxResourceSaveInfo, "N:\\Cache\\PXTest.cs");

        PXConsoleWrite(0, 0);
    }
    
}

void PXAPI PXTestCompilerOBJ(char* filePath)
{
    /*
    PXFile inputStream;
    PXFile outputStream;

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.FileSize = 0;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, filePath);

        PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
    }

    PXFileMapToMemory(&outputStream, inputStream.DataSize * 4, PXMemoryAccessModeReadAndWrite);

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
    pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
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
    pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
    pxFileOpenFromPathInfo.AllowMapping = PXTrue;
    pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
    pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

    PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "A:/config.yml");

    PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
    PXFileMapToMemory(&compiledStream, inputStream.DataSize * 7, PXMemoryAccessModeReadAndWrite);

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
    PXFileMapToMemoryA(&inputStream, "A:/index.php", PXMemoryAccessModeReadOnly, FileCachingSequential);
#endif
    PXFileMapToMemory(&outputStream, inputStream.DataSize * 8, PXMemoryAccessModeReadAndWrite);

    PXXMLFileCompile(&inputStream, &outputStream);

    printf("____\n");
    */
}