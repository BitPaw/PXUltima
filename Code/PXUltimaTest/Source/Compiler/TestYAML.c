#include "TestYAML.h"

#include <Media/YAML/YAML.h>

void TestYAMLAll()
{
	TestYAMLParse();
}

void TestYAMLParse()
{
	PXDataStream inputStream;
	PXDataStream compiledStream;

	struct MyStroage
	{
		unsigned int ID;
		char Name[16];
	}MyStroage;

	SerializationTypeInfo serializationTypeInfo[2];

	SerializationTypeInfoSet(&serializationTypeInfo[0], "ID", &MyStroage.ID, sizeof(MyStroage.ID), SerializationTypeIntegerUnsigned32, 0, 0);
	SerializationTypeInfoSet(&serializationTypeInfo[1], "Name", &MyStroage.Name, sizeof(MyStroage.Name), SerializationTypeStringASCII, 0, 0);


	// Store
	//------------------------------

	//----<Load>--------------------------
	PXDataStreamMapToMemoryA(&inputStream, "A:/config.yml", 0, MemoryReadOnly);
	PXDataStreamMapToMemory(&compiledStream, inputStream.DataSize*7, MemoryReadAndWrite);

	YAMLFileCompile(&inputStream, &compiledStream);

	YAMLFileParse(&compiledStream, &serializationTypeInfo, 2u);
	//------------------------------


	printf("finish\n");
}