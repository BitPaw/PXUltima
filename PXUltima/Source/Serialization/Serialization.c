#include "Serialization.h"

void SerializationTypeInfoSet
(
	SerializationTypeInfo* serializationTypeInfo, 
	const char* name, 
	void* adress, 
	const size_t size, 
	SerializationType type, 
	SerializationTypeInfo* parent, 
	unsigned char depth
)
{
	serializationTypeInfo->Name= name;
	serializationTypeInfo->Adress = adress;
	serializationTypeInfo->Size = size;
	serializationTypeInfo->Type = type;
	serializationTypeInfo->Parent = parent;
	serializationTypeInfo->Depth = depth;
}
