#include "PXFilmBox.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>


const char PXFBXName[] = "FBX";
const char PXFBXSignature[] = "Kaydara FBX Binary  \x00\x1A"; // Hidden \0




const PXI32U PXFilmBoxNodeRecordArrayBinding[] =
{
    PXTypeInt32ULE,
    PXTypeInt32ULE,
    PXTypeInt32ULE,
};
const PXI8U PXFilmBoxNodeRecordArrayBindingSize = sizeof(PXFilmBoxNodeRecordArrayBinding) / sizeof(PXI32U);

typedef struct PXFilmBoxNodeRecordArray_
{
    PXI32U ArrayLength;
    PXI32U Encoding;
    PXI32U CompressedLength;

    void* Data;
}
PXFilmBoxNodeRecordArray;


typedef struct PXFilmBoxNodeRecordData_
{
    PXI32U Length;
    char* Data;
}
PXFilmBoxNodeRecordData;


typedef struct PXFilmBoxProperty_
{
    union
    {
        PXFilmBoxNodeRecordArray Array;
        PXFilmBoxNodeRecordData Data;
    };

    PXI8U Typecode;
    PXI32U PXType;


}
PXFilmBoxProperty;



const PXI32U PXFilmBoxNodeRecordList[] =
{
    PXTypeInt32ULE,
    PXTypeInt32ULE,
    PXTypeInt32ULE,
    PXTypeInt08U
};
const PXI8U PXFilmBoxNodeRecordListSize = sizeof(PXFilmBoxNodeRecordList) / sizeof(PXI32U);

typedef struct PXFilmBoxNodeRecord_
{
    PXI32U EndOffset;
    PXI32U NumProperties;
    PXI32U PropertyListLen;
    PXI8U NameLen;
    char* Name;
}
PXFilmBoxNodeRecord;



PXI32U PXAPI PXFilmBoxTypeTranslate(const PXI8U type)
{
    switch(type)
    {
        case 'Y': return PXTypeInt16SLE; // 2 byte signed Integer
        case 'C': return PXTypeBool; // 1 bit boolean(1: true, 0 : false) encoded as the LSB of a 1 Byte value.
        case 'I': return PXTypeInt32SLE; // 4 byte signed Integer
        case 'F': return PXTypeF32; // 4 byte single - precision IEEE 754 number
        case 'D': return PXTypeF64; // 8 byte double - precision IEEE 754 number
        case 'L': return PXTypeInt64ULE; // 8 byte signed Integer

        case 'f': return PXTypeIsArray | PXTypeF32; // Array of 4 byte single - precision IEEE 754 number
        case 'd': return PXTypeIsArray | PXTypeF64; // Array of 8 byte double - precision IEEE 754 number
        case 'l': return PXTypeIsArray | PXTypeInt64SLE; // Array of 8 byte signed Integer
        case 'i': return PXTypeIsArray | PXTypeInt32SLE; // Array of 4 byte signed Integer
        case 'b': return PXTypeIsArray | PXTypeBool; // Array of 1 byte Booleans(always 0 or 1)

        case 'S': return PXTypeIsArray; // String
        case 'R': return PXTypeIsArray; // raw binary data

        default:
            return 0;
    }
}



void PXAPI PXFilmBoxReadProperty(PXFile PXREF pxFile, PXFilmBoxProperty PXREF pxFilmBoxProperty)
{
    pxFilmBoxProperty->Typecode = 0;

    PXFileReadI8U(pxFile, &pxFilmBoxProperty->Typecode);

    pxFilmBoxProperty->PXType = PXFilmBoxTypeTranslate(pxFilmBoxProperty->Typecode);

    const PXSize dataTypeSize = PXTypeSizeGet(pxFilmBoxProperty->PXType);

    // char buffer[64];

   //  PXTypeToString(type, buffer);






    if(PXTypeIsArray & pxFilmBoxProperty->PXType)
    {
        // An array
        if(pxFilmBoxProperty->Typecode == 'S' || pxFilmBoxProperty->Typecode == 'R')
        {
            // string ro raw data
            PXFileReadI32U(pxFile, &pxFilmBoxProperty->Data.Length);
            pxFilmBoxProperty->Data.Data = (char*)PXFileDataAtCursor(pxFile);
            PXFileCursorAdvance(pxFile, pxFilmBoxProperty->Data.Length);



#if PXLogEnable
            char cacheB[64];

            if(pxFilmBoxProperty->Typecode == 'S')
            {
                PXTextCopyA(pxFilmBoxProperty->Data.Data, pxFilmBoxProperty->Data.Length, cacheB, 64);
            }
            else
            {
                PXTextPrintA(cacheB, 64, "**Data**");
            }


            PXLogPrint
            (
                PXLoggingInfo,
                PXFBXName,
                "Parsing",
                "Property-Data (%c): %s",
                pxFilmBoxProperty->Typecode,
                cacheB
            );
#endif
        }
        else
        {
            PXFileBinding
            (
                pxFile,
                &pxFilmBoxProperty->Array,
                PXFilmBoxNodeRecordArrayBinding,
                PXFilmBoxNodeRecordArrayBindingSize,
                PXFileBindingRead
            );

            pxFilmBoxProperty->Array.Data = PXFileDataAtCursor(pxFile);
            PXFileCursorAdvance(pxFile, pxFilmBoxProperty->Array.CompressedLength);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXFBXName,
                "Parsing",
                "Property-Array (%c):  Length:%i, Encoding:%i, CompressedLength:%i",
                pxFilmBoxProperty->Typecode,
                pxFilmBoxProperty->Array.ArrayLength,
                pxFilmBoxProperty->Array.Encoding,
                pxFilmBoxProperty->Array.CompressedLength
            );
#endif


        }
    }
    else
    {
        pxFilmBoxProperty->Data.Length = dataTypeSize;
        pxFilmBoxProperty->Data.Data = (char*)PXFileDataAtCursor(pxFile);

        // Not an array
#if PXLogEnable


        PXText dataType;
        PXTextConstructNamedBufferA(&dataType, dataTypeBuffer, 64);

        PXText dataContent;
        PXTextConstructNamedBufferA(&dataContent, dataContentBuffer, 64);

        PXTypeEntry pxFileDataElementType;
        pxFileDataElementType.Adress = pxFilmBoxProperty->Data.Data;
        pxFileDataElementType.Type = pxFilmBoxProperty->PXType;

        PXTypeEntryInfo(&pxFileDataElementType, &dataType, &dataContent);

        PXLogPrint
        (
            PXLoggingInfo,
            PXFBXName,
            "Parsing",
            "Property-Value (%c): (%i B) %20s : %s",
            pxFilmBoxProperty->Typecode,
            dataTypeSize,
            dataType.A,
            dataContent.A
        );
#endif

        PXFileCursorAdvance(pxFile, dataTypeSize);

    }
}

void PXAPI PXFilmBoxReadNode(PXFile PXREF pxFile, PXFilmBoxNodeRecord PXREF pxFilmBoxNodeRecord)
{
    // read node beginning
    PXFileBinding
    (
        pxFile,
        pxFilmBoxNodeRecord,
        PXFilmBoxNodeRecordList,
        PXFilmBoxNodeRecordListSize,
        PXFileBindingRead
    );

    if(pxFilmBoxNodeRecord->NameLen)
    {
        pxFilmBoxNodeRecord->Name = (char*)PXFileDataAtCursor(pxFile);
        PXFileCursorAdvance(pxFile, pxFilmBoxNodeRecord->NameLen);
    }
    else
    {
        pxFilmBoxNodeRecord->Name = PXNull;
    }

#if PXLogEnable
    char cacheA[64];

    if(pxFilmBoxNodeRecord->NameLen)
    {
        PXTextCopyA(pxFilmBoxNodeRecord->Name, pxFilmBoxNodeRecord->NameLen, cacheA, 64);
    }
    else
    {
        PXTextPrintA(cacheA, 64, "---");
    }

    PXLogPrint
    (
        PXLoggingInfo,
        PXFBXName,
        "Parsing",
        "NodeRecord:\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %s (%i)",
        "EndOffset", pxFilmBoxNodeRecord->EndOffset,
        "NumProperties", pxFilmBoxNodeRecord->NumProperties,
        "PropertyListLen", pxFilmBoxNodeRecord->PropertyListLen,
        "Name", cacheA, pxFilmBoxNodeRecord->NameLen
    );
#endif

    for(PXSize i = 0; i < pxFilmBoxNodeRecord->NumProperties; ++i)
    {
        PXFilmBoxProperty pxFilmBoxProperty;

        PXFilmBoxReadProperty(pxFile, &pxFilmBoxProperty);
    }



    if(pxFilmBoxNodeRecord->EndOffset == 0) // What does it mean?? DOne?
    {
        return;
    }


    // Check if it is nested, if we have more to read.
    while(1)
    {
        const PXSize restSize = pxFilmBoxNodeRecord->EndOffset - PXFileDataPosition(pxFile);

        if(restSize <= 4) // 4 byte, ?end
        {
            break;       
        }

        PXFilmBoxNodeRecord pxFilmBoxNodeRecordSub;

        PXFilmBoxReadNode(pxFile, &pxFilmBoxNodeRecordSub);
    }

 


    PXFileCursorMoveTo(pxFile, pxFilmBoxNodeRecord->EndOffset);
}



PXResult PXAPI PXFilmBoxLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;
    PXFilmBox pxFilmBox;

    // Header 27 Bytes
    {
        const PXBool isValidHeader = PXFileReadAndCompare(pxFile, PXFBXSignature, sizeof(PXFBXSignature));

        if(!isValidHeader)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXFileReadI32U(pxFile, &pxFilmBox.Version);
    }

    while(!PXFileIsAtEnd(pxFile))
    {
        PXFilmBoxNodeRecord pxFilmBoxNodeRecord;

        PXFilmBoxReadNode(pxFile, &pxFilmBoxNodeRecord);
    }

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXFilmBoxSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
