#include "TestYAML.h"

#include <Media/YAML/PXYAML.h>

void TestYAMLAll()
{
	TestYAMLParse();
}

void TestYAMLParse()
{
	PXFile inputStream;
	PXFile compiledStream;

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
	PXFileMapToMemoryA(&inputStream, "A:/config.yml", 0, PXMemoryAccessModeReadOnly);
	PXFileMapToMemory(&compiledStream, inputStream.DataSize*7, PXMemoryAccessModeReadAndWrite);

	PXYAMLFileCompile(&inputStream, &compiledStream);

	PXYAMLFileParse(&compiledStream, &serializationTypeInfo, 2u);
	//------------------------------


	printf("finish\n");
}