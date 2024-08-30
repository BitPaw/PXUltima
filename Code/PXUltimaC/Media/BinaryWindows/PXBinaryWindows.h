#ifndef PXEXEINCLUDE
#define PXEXEINCLUDE

#include "../PXResource.h"
#include <Media/COFF/PXCOFF.h>

typedef struct PXDOSHeader_
{
    PXInt16UCluster Magic;          // "MZ" 
    PXInt16U LastPageSize;          // Size of final block in the file, or 0 if the final block is a full 512 bytes. 
    PXInt16U TotalPagesInFile;      // Number of 512-byte blocks in the file, including the final block which may not be the full 512 bytes. 
    PXInt16U ReallocationItems;
    PXInt16U ParagraphsInHeader;
    PXInt16U ParagraphsExtraMinimum;
    PXInt16U ParagraphsExtraMaximum;
    PXInt16U StackInitialSegment;
    PXInt16U StackInitialPointer;
    PXInt16U ChecksumCompliment;
    PXInt16U InstructionPointerInternal;
    PXInt16U CodeSegmentInitial;
    PXInt16U e_lfarlc;
    PXInt16U e_ovno;
    PXInt16U e_oemid;
    PXInt16U e_oeminfo;
    PXInt32U FileOffsetToHeader;

    PXInt32U ActualSize;
}
PXDOSHeader;


typedef struct PXNEHeader_
{
    char Magic[2];                 // {'N', 'E'}
    PXInt8U LinkerVersionMajor;    //The major linker version
    PXInt8U LinkerVersionMinor;    //The minor linker version
    PXInt16U EntryTableOffset;   //Offset of entry table, see below
    PXInt16U EntryTableLength;   //Length of entry table in bytes
    PXInt32U FileLoadCRC;        //32-bit CRC of entire contents of file
    PXInt16U FlagList;            // Uses the FlagWord enum
    PXInt16U SegmentDataAutoIndex;   //The automatic data segment index
    PXInt16U HeapSizeInit;       //The initial local heap size
    PXInt16U StackSizeInit;      //The initial stack size
    PXInt32U EntryPoint;         //CS:IP entry point, CS is index into segment table
    PXInt32U InitStack;          //SS:SP initial stack pointer, SS is index into segment table
    PXInt16U SegmentTableEntryAmount;           //Number of segments in segment table
    PXInt16U ModuleReferenceTableEntryAmount;            //Number of module references (DLLs)
    PXInt16U NonresidentNameTableEntryAmount;   //Size of non-resident names table, in bytes (Please clarify non-resident names table)
    PXInt16U SegmentTableOffset;
    PXInt16U ResourceTableOffset;
    PXInt16U ResidentNamesTableOffset;
    PXInt16U ModRefTable;        //Offset of module reference table
    PXInt16U ImportNameTable;    //Offset of imported names table (array of counted strings, terminated with string of length 00h)
    PXInt32U OffStartNonResTab;  //Offset from start of file to non-resident names table
    PXInt16U MovEntryCount;      //Count of moveable entry point listed in entry table
    PXInt16U FileAlnSzShftCnt;   //File alignment size shift count (0=9(default 512 byte pages))
    PXInt16U nResTabEntries;     //Number of resource table entries
    PXInt8U targOS;              //Target OS

    //
    // The rest of these are not defined in the Windows 3.0 standard and
    // appear to be specific to OS/2.

    PXInt8U OS2EXEFlags;         //Other OS/2 flags
    PXInt16U retThunkOffset;     //Offset to return thunks or start of gangload area - what is gangload?
    PXInt16U segrefthunksoff;    //Offset to segment reference thunks or size of gangload area
    PXInt16U mincodeswap;        //Minimum code swap area size
    PXInt8U expctwinver[2];      //Expected windows version (minor first)
}
PXNEHeader;


typedef struct PXNEHeaderSegmentEntry_
{
    PXInt16U LogicalSectorOffset;
    PXInt16U LengthoftheSegment;
    PXInt16U Flag;
    PXInt16U MinimumAllocationSize;
}
PXNEHeaderSegmentEntry;


typedef struct PXPEHeader_
{
    PXInt32UCluster Signature; // PE--
}
PXPEHeader;

typedef struct PXBinaryWindows_
{
    PXDOSHeader Header;

    PXCOFF COFFHeader;
}
PXBinaryWindows;

PXPublic PXActionResult PXAPI PXBinaryWindowsLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXBinaryWindowsSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif