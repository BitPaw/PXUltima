#ifndef PXWavefrontINCLUDE
#define PXWavefrontINCLUDE

#include "../PXResource.h"

#include <Media/MTL/PXMTL.h>

#define PXWavefrontNameSize 64u
#define PXWavefrontElementNameLength 50u
#define PXWavefrontDebug 1

typedef enum PXWavefrontLineType_
{
	PXWavefrontLineInvalid,
	PXWavefrontLineNone,
	PXWavefrontLineComment,
	PXWavefrontLineMaterialLibraryInclude,
	PXWavefrontLineMaterialLibraryUse,
	PXWavefrontLineObjectName,
	PXWavefrontLineObjectGroup,
	PXWavefrontLineVertexGeometric,
	PXWavefrontLineVertexTexture,
	PXWavefrontLineVertexNormal,
	PXWavefrontLineVertexParameter,
	PXWavefrontLineSmoothShading,
	PXWavefrontLineFaceElement,

	PXWavefrontEmbeddedMTL
}
PXWavefrontLineType;

typedef struct PXWavefrontElementMaterialInfo_
{
	PXSize MaterialIndex;
	PXSize Size; // Size of buffer to use given material
}
PXWavefrontElementMaterialInfo;

typedef struct PXWavefrontElement_
{
	char Name[PXWavefrontElementNameLength];
	float Smoothing;

	PXWavefrontElementMaterialInfo* MaterialInfo;
	PXSize MaterialInfoSize;

	PXSize VertexPositionListSize;
	float* VertexPositionList; 	// List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.

	PXSize TextureCoordinateListSize;
	float* TextureCoordinateList; 	// List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.

	PXSize VertexNormalPositionListSize;
	float* VertexNormalPositionList; 	// List of vertex normals in (x,y,z) form; normals might not be unit vectors.

	PXSize VertexParameterListSize;
	float* VertexParameterList; // Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement.

	PXSize FaceElementListSize;
	unsigned int* FaceElementList; // Polygonal face element.
}
PXWavefrontElement;

PXPublic void PXAPI PXWavefrontElementConstruct(PXWavefrontElement* objElement);
PXPublic void PXAPI PXWavefrontElementDestruct(PXWavefrontElement* objElement);

PXPrivate PXWavefrontLineType PXAPI PXWavefrontPeekLine(const void* line, const PXSize size);


// allowed syntax is
// A: "f 1 2 3"
// B: "f 1/2/3"
// C: "f 1//3"
// D: "f 1/3"
PXPublic void PXAPI PXWavefrontFaceLineParse(PXCompiler* const pxCompiler, PXInt32U* const pxInt32U);

PXPublic PXActionResult PXAPI PXWavefrontLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWavefrontSaveFromFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif