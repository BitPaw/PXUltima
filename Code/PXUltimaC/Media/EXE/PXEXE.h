#ifndef PXEXEINCLUDE
#define PXEXEINCLUDE

#include <Media/PXResource.h>
#include <Media/COFF/PXCOFF.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
		PXInt16U ReservedA[4];
		PXInt16U e_oemid;
		PXInt16U e_oeminfo;
		PXInt16U ReservedB[10];
		PXInt32U FileOffsetToHeader;
	}
	PXDOSHeader;

	typedef struct PXPEHeader_
	{
		PXInt32UCluster Signature; // PE--
	}
	PXPEHeader;

	typedef struct PXEXEHeaderNT
	{
		PXInt32U Signature;
		//_IMAGE_FILE_HEADER FileHeader;
		//_IMAGE_OPTIONAL_HEADER OptionalHeader;
	};

	typedef struct PXEXE_
	{
		PXDOSHeader Header;

		PXCOFF COFFHeader;
	}
	PXEXE;

	PXPublic PXActionResult PXEXELoadFromFile(PXEXE* const pxEXE, PXFile* const pxFile);
	PXPublic PXActionResult PXEXESaveToFile(PXEXE* const pxEXE, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif