#include "TestPXCompiler.h"

#include <File/DataStream.h>

#include <Compiler/Compiler.h>
#include <Format/YAML/YAML.h>
#include <Format/OBJ/OBJ.h>

void TestPXCompilerAll()
{
	TestPXCompilerOBJ();
	TestPXCompilerYAML();
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

	OBJFileParse(&inputStream, &outputStream);

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

	compilerSettings.WhiteSpaceKeep = 1;

	PXCompilerLexicalAnalysis(&inputStream, &outputStream, &compilerSettings);

	printf("____\n");
}