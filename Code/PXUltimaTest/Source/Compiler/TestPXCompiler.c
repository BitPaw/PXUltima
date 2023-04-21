#include "TestPXCompiler.h"

#include <File/PXDataStream.h>

#include <Compiler/PXCompiler.h>
#include <Media/YAML/YAML.h>
#include <Media/Wavefront/PXWavefront.h>
#include <Media/XML/XML.h>

void TestPXCompilerAll()
{
	TestPXCompilerOBJ();
	//TestPXCompilerYAML();
	//TestPXCompilerXML();
}

void TestPXCompilerOBJ()
{
	PXDataStream inputStream;
	PXDataStream outputStream;

	PXDataStreamConstruct(&inputStream);
	PXDataStreamConstruct(&outputStream);

	PXDataStreamMapToMemoryA
	(
		&inputStream,
		"B:/Daten/Objects/Moze/Moze.obj",
		0,
		MemoryReadOnly
	);

	PXDataStreamMapToMemory(&outputStream, inputStream.DataSize*4, MemoryReadAndWrite);

	PXWavefrontFileCompile(&inputStream, &outputStream);

	PXModel model;	

	PXWavefrontParseToModel(&outputStream, &model);

	printf("____\n");
}

void TestPXCompilerYAML()
{
	PXSize size = 1024 * 2;
	char buffer[1024*2];

	MemoryClear(buffer, size);

	PXDataStream inputStream;
	PXDataStream outputStream;

	PXDataStreamConstruct(&inputStream);
	PXDataStreamConstruct(&outputStream);

	PXDataStreamMapToMemoryA
	(
		&inputStream,
		"A:/config.yml",
		0,
		MemoryReadOnly		
	);

	PXDataStreamFromExternal(&outputStream, buffer, size);

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

	PXDataStream inputStream;
	PXDataStream outputStream;

	PXDataStreamConstruct(&inputStream);
	PXDataStreamConstruct(&outputStream);

#if 1
	PXDataStreamFromExternal(&inputStream, xmlData, 299-1);	
#else
	PXDataStreamMapToMemoryA(&inputStream, "A:/index.php", MemoryReadOnly, FileCachingSequential);
#endif
	PXDataStreamMapToMemory(&outputStream, inputStream.DataSize * 8, MemoryReadAndWrite);

	XMLFileCompile(&inputStream, &outputStream);

	printf("____\n");
}