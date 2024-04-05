#ifndef PXMTLINCLUDE
#define PXMTLINCLUDE

#include "../PXResource.h"

#define MTLNameSize 32u
#define MTLFilePath 60u

typedef enum PXMTLLineType_
{
	MTLLineInvalid,
	MTLLineName,
	MTLLineTexture,
	MTLLineWeight,
	MTLLineColorAmbient,
	MTLLineColorDiffuse,
	MTLLineColorSpecular,
	MTLLineColorEmission,
	MTLLineDissolved,
	MTLLineDensity,
	MTLLineIllumination
}
PXMTLLineType;

typedef struct MTLMaterial_
{
	unsigned char NameSize;
	char* Name;

	unsigned char DiffuseTexturePathSize;
	char* DiffuseTexturePath;

	// Ranges between 0 and 1000
	float Weight;

	float Ambient[3];
	float Diffuse[3];
	float Specular[3];
	float Emission[3];

	float Dissolved;

	// range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
	float Density;

	PXMaterialIlluminationMode Illumination;
}
PXMTLMaterial;

PXPrivate PXMaterialIlluminationMode PXAPI PXMTLIlluminationModeFromID(const unsigned int illuminationModeID);

PXPrivate PXMTLLineType PXAPI PXMTLPeekLine(const char* const line, const PXSize lineSize);

PXPublic PXSize PXAPI PXMTLFetchAmount(const void* const data, const PXSize dataSize);
PXPublic PXBool PXAPI PXMTLFetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, PXMTLMaterial* const mtlMaterial);

PXPublic PXActionResult PXAPI PXMTLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMTLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif