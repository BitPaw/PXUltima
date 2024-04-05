#ifndef PXEXEINCLUDE
#define PXEXEINCLUDE

#include "../PXResource.h"
#include <Media/COFF/PXCOFF.h>

typedef struct PXDOSHeader_
{
	PXInt16UCluster Magic;
	PXInt16U LastPageSize;
	PXInt16U TotalPagesInFile;
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
}
PXDOSHeader;

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

PXPublic PXActionResult PXAPI PXBinaryWindowsLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXBinaryWindowsSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif