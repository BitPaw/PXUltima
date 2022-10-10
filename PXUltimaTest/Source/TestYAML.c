#include "TestYAML.h"

#include <Format/YAML/YAML.h>

void TestYAMLAll()
{
	TestYAMLParse();
}

void TestYAMLParse()
{
	DataStream inputStream;
	DataStream compiledStream;



	DataStreamMapToMemoryA(&inputStream, "A:/config.yml", 0, MemoryReadOnly);
	DataStreamMapToMemory(&compiledStream, inputStream.DataSize, MemoryReadAndWrite);

	YAMLFileCompile(&inputStream, &compiledStream);

	printf("finish\n");
}