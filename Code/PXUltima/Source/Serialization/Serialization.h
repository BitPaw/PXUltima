#ifndef PXSerializationINCLUDE
#define PXSerializationINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXSerializationType_
	{
		SerializationTypeInvalid,
		
		SerializationTypeBool,

		SerializationTypeIntegerSigned8,
		SerializationTypeIntegerUnsigned8,
		SerializationTypeIntegerSigned16,
		SerializationTypeIntegerUnsigned16,
		SerializationTypeIntegerSigned32,
		SerializationTypeIntegerUnsigned32,
		SerializationTypeIntegerSigned64,
		SerializationTypeIntegerUnsigned64,

		SerializationTypeFloat,
		SerializationTypeDouble,

		SerializationTypeStringASCII,
		SerializationTypeFloatUNICODE,
		SerializationTypeFloatUTF
	}
	PXSerializationType;

	// Example i want, xxxx("ID", &ID, sizeof(ID), TYpeUnyi64, )
	
	typedef struct PXSerializationTypeInfo_ PXSerializationTypeInfo;

	typedef struct PXSerializationTypeInfo_
	{
		// Field Info
		char* Name; // Name a field
		void* Adress; // Adress of field		
		PXSize Size; // Size of field
		PXSerializationType Type; // Type of field

		PXSerializationTypeInfo* Parent;
		unsigned char Depth;

	}
	PXSerializationTypeInfo;

	PXPublic void PXSerializationTypeInfoSet
	(
		PXSerializationTypeInfo* serializationTypeInfo,
		const char* name, 
		void* adress, 
		const PXSize size, 
		PXSerializationType type,
		PXSerializationTypeInfo* parent,
		unsigned char depth
	);


#ifdef __cplusplus
}
#endif

#endif