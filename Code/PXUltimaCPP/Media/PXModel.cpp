#include "PXModel.hpp"

PX::Model::Model()
{
	PXModelConstruct(this);
}

PX::Model::~Model()
{
	PXModelDestruct(this);
}

PXSize PX::Model::MaterialAmount()
{
	return 	PXModelMaterialAmount(this);
}

PXBool PX::Model::MaterialGet(const PXSize materialID, PXMaterial* const pxMaterial)
{
	return PXModelMaterialGet(this, materialID, pxMaterial);
}

unsigned char PX::Model::SegmentsAmount()
{
	return PXModelSegmentsAmount(this);
}

void PX::Model::SegmentsGet(const PXSize index, MeshSegment* const meshSegment)
{
	PXModelSegmentsGet(this, index, meshSegment);
}

void PX::Model::SegmentsAdd(const unsigned int renderMode, const unsigned int renderSize, const unsigned int renderMaterial)
{
	PXModelSegmentsAdd(this, renderMode, renderSize, renderMaterial);
}

PXSize PX::Model::VertexDataStride()
{
	return PXModelVertexDataStride(this);
}

PX::ActionResult PX::Model::Load(const PX::Text& const filePath)
{
	return (PX::ActionResult)PXModelLoad(this, &filePath);
}

PX::ActionResult PX::Model::LoadD(PX::File& const fileStream, const FileFormatExtension modelType)
{
	return (PX::ActionResult)PXModelLoadD(this, &fileStream, modelType);
}
