#ifndef PXSoftwareRenderINCLUDE
#define PXSoftwareRenderINCLUDE

#include <Media/PXType.h>
#include <Media/PXImage.h>
#include <Media/PXText.h>
#include <Media/PXResource.h>

typedef struct PXSoftwareRender_
{
	unsigned int __dummy__;
}
PXSoftwareRender;

PXPublic void PXAPI PXSoftwareRenderDrawRectangle
(
	PXImage* const image,
	const PXSize x,
	const PXSize y,
	const PXSize width,
	const PXSize height,
	const unsigned char red,
	const unsigned char green,
	const unsigned char blue,
	const unsigned char alpha
);
PXPublic void PXAPI PXImageDrawTextA
(
	PXImage* const image,
	const PXSize x,
	const PXSize y,
	const PXSize width,
	const PXSize height,
	const PXFont* const font,
	const char* text
);
PXPublic void PXAPI PXImageDrawTextW
(
	PXImage* const image,
	const PXSize x,
	const PXSize y,
	const PXSize width,
	const PXSize height,
	const PXFont* const font,
	const wchar_t* text
);

#pragma optimize( "", off )
PXPublic void PXAPI PXImageMerge
(
	PXImage* const image,
	const PXSize x,
	const PXSize y,
	const PXSize insertX,
	const PXSize insertY,
	const PXSize insertWidth,
	const PXSize insertHeight,
	const PXImage* const imageInsert
);

#endif