#pragma once

#ifndef PXEXEIncluded
#define PXEXEIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Media/COFF/PXCOFF.h>

typedef struct PXDOSHeader_
{
    char Magic[2];          // "MZ"
    PXI16U LastPageSize;          // Size of final block in the file, or 0 if the final block is a full 512 bytes.
    PXI16U TotalPagesInFile;      // Number of 512-byte blocks in the file, including the final block which may not be the full 512 bytes.
    PXI16U ReallocationItems;
    PXI16U ParagraphsInHeader;
    PXI16U ParagraphsExtraMinimum;
    PXI16U ParagraphsExtraMaximum;
    PXI16U StackInitialSegment;
    PXI16U StackInitialPointer;
    PXI16U ChecksumCompliment;
    PXI16U InstructionPointerInternal;
    PXI16U CodeSegmentInitial;
    PXI16U e_lfarlc;
    PXI16U e_ovno;
    PXI16U e_oemid;
    PXI16U e_oeminfo;
    PXI32U FileOffsetToHeader;
    PXI32U ActualSize;
}
PXDOSHeader;


typedef struct PXNEHeader_
{
    char Magic[2];                 // {'N', 'E'}
    PXI8U LinkerVersionMajor;    //The major linker version
    PXI8U LinkerVersionMinor;    //The minor linker version
    PXI16U EntryTableOffset;   //Offset of entry table, see below
    PXI16U EntryTableLength;   //Length of entry table in bytes
    PXI32U FileLoadCRC;        //32-bit CRC of entire contents of file
    PXI16U FlagList;            // Uses the FlagWord enum
    PXI16U SegmentDataAutoIndex;   //The automatic data segment index
    PXI16U HeapSizeInit;       //The initial local heap size
    PXI16U StackSizeInit;      //The initial stack size
    PXI32U EntryPoint;         //CS:IP entry point, CS is index into segment table
    PXI32U InitStack;          //SS:SP initial stack pointer, SS is index into segment table
    PXI16U SegmentTableEntryAmount;           //Number of segments in segment table
    PXI16U ModuleReferenceTableEntryAmount;            //Number of module references (DLLs)
    PXI16U NonresidentNameTableEntryAmount;   //Size of non-resident names table, in bytes (Please clarify non-resident names table)
    PXI16U SegmentTableOffset;
    PXI16U ResourceTableOffset;
    PXI16U ResidentNamesTableOffset;
    PXI16U ModRefTable;        //Offset of module reference table
    PXI16U ImportNameTable;    //Offset of imported names table (array of counted strings, terminated with string of length 00h)
    PXI32U OffStartNonResTab;  //Offset from start of file to non-resident names table
    PXI16U MovEntryCount;      //Count of moveable entry point listed in entry table
    PXI16U FileAlnSzShftCnt;   //File alignment size shift count (0=9(default 512 byte pages))
    PXI16U nResTabEntries;     //Number of resource table entries
    PXI8U targOS;              //Target OS

    //
    // The rest of these are not defined in the Windows 3.0 standard and
    // appear to be specific to OS/2.

    PXI8U OS2EXEFlags;         //Other OS/2 flags
    PXI16U retThunkOffset;     //Offset to return thunks or start of gangload area - what is gangload?
    PXI16U segrefthunksoff;    //Offset to segment reference thunks or size of gangload area
    PXI16U mincodeswap;        //Minimum code swap area size
    PXI8U expctwinver[2];      //Expected windows version (minor first)
}
PXNEHeader;


typedef struct PXNEHeaderSegmentEntry_
{
    PXI16U LogicalSectorOffset;
    PXI16U LengthoftheSegment;
    PXI16U Flag;
    PXI16U MinimumAllocationSize;
}
PXNEHeaderSegmentEntry;


typedef struct PXPEHeader_
{
    PXI32UCluster Signature; // PE--
}
PXPEHeader;

typedef struct PXBinaryWindows_
{
    PXDOSHeader Header;

    PXCOFF COFFHeader;
}
PXBinaryWindows;

PXPublic PXResult PXAPI PXBinaryWindowsLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXBinaryWindowsSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
