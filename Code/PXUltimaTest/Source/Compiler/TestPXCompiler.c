#include "TestPXCompiler.h"

#include <OS/File/PXFile.h>

#include <Compiler/PXCompiler.h>
#include <Media/YAML/PXYAML.h>
#include <Media/XML/XML.h>
#include <Media/Wavefront/PXWavefront.h>


void TestPXCompilerAll()
{
	TestPXCompilerOBJ();
	//TestPXCompilerYAML();
	//TestPXCompilerXML();
}

void TestPXCompilerOBJ()
{
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

		PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "B:/Daten/Objects/Moze/Moze.obj");

		PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
	}	

	PXFileMapToMemory(&outputStream, inputStream.DataSize*4, PXMemoryAccessModeReadAndWrite);

	PXWavefrontFileCompile(&inputStream, &outputStream);

	PXModel model;	

	PXWavefrontParseToModel(&outputStream, &model);

	printf("____\n");
}

void TestPXCompilerYAML()
{
	PXSize size = 1024 * 2;
	char buffer[1024*2];

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
}

void TestPXCompilerXML()
{
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
	PXFileBufferExternal(&inputStream, xmlData, 299-1);	
#else
	PXFileMapToMemoryA(&inputStream, "A:/index.php", PXMemoryAccessModeReadOnly, FileCachingSequential);
#endif
	PXFileMapToMemory(&outputStream, inputStream.DataSize * 8, PXMemoryAccessModeReadAndWrite);

	XMLFileCompile(&inputStream, &outputStream);

	printf("____\n");
}