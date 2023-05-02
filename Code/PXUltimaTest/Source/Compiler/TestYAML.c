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
	PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
	pxFileOpenFromPathInfo.FileSize;
	pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
	pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
	pxFileOpenFromPathInfo.AllowMapping = PXTrue;
	pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
	pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

	PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text,"A:/config.yml");

	PXFileOpenFromPath(&inputStream, &pxFileOpenFromPathInfo);
	PXFileMapToMemory(&compiledStream, inputStream.DataSize*7, PXMemoryAccessModeReadAndWrite);

	PXYAMLFileCompile(&inputStream, &compiledStream);

	PXYAMLFileParse(&compiledStream, &serializationTypeInfo, 2u);
	//------------------------------


	printf("finish\n");
}