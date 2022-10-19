#include "TestPXCompiler.h"

#include <File/DataStream.h>

#include <Compiler/Compiler.h>
#include <Format/YAML/YAML.h>
#include <Format/OBJ/OBJ.h>
#include <Format/XML/XML.h>

void TestPXCompilerAll()
{
	TestPXCompilerOBJ();
	//TestPXCompilerYAML();
	//TestPXCompilerXML();
}

void TestPXCompilerOBJ()
{
	DataStream inputStream;
	DataStream outputStream;

	DataStreamConstruct(&inputStream);
	DataStreamConstruct(&outputStream);

	DataStreamMapToMemoryA
	(
		&inputStream,
		"B:/Daten/Objects/Moze/Moze.obj",
		0,
		MemoryReadOnly
	);

	DataStreamMapToMemory(&outputStream, inputStream.DataSize*4, MemoryReadAndWrite);

	OBJFileCompile(&inputStream, &outputStream);

	Model model;



	OBJParseToModel(&outputStream, &model);

	printf("____\n");
}

void TestPXCompilerYAML()
{
	size_t size = 1024 * 2;
	char buffer[1024*2];

	MemorySet(buffer, size, 0);

	DataStream inputStream;
	DataStream outputStream;

	DataStreamConstruct(&inputStream);
	DataStreamConstruct(&outputStream);

	DataStreamMapToMemoryA
	(
		&inputStream,
		"A:/config.yml",
		0,
		MemoryReadOnly		
	);

	DataStreamFromExternal(&outputStream, buffer, size);

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

	DataStream inputStream;
	DataStream outputStream;

	DataStreamConstruct(&inputStream);
	DataStreamConstruct(&outputStream);

#if 1
	DataStreamFromExternal(&inputStream, xmlData, 299-1);	
#else
	DataStreamMapToMemoryA(&inputStream, "A:/index.php", MemoryReadOnly, FileCachingSequential);
#endif
	DataStreamMapToMemory(&outputStream, inputStream.DataSize * 8, MemoryReadAndWrite);

	XMLFileCompile(&inputStream, &outputStream);

	printf("____\n");
}