#include "Serialization.h"

void PXSerializationTypeInfoSet
(
	PXSerializationTypeInfo* serializationTypeInfo, 
	const char* name, 
	void* adress, 
	const PXSize size, 
	PXSerializationType type, 
	PXSerializationTypeInfo* parent, 
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
