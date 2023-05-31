#ifndef PXCPPModelINCLUDE
#define PXCPPModelINCLUDE

#include <Media/PXModel.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class Model : public PXModel
	{
		public:
		Model();
		~Model();

		PXSize MaterialAmount();
		PXBool MaterialGet( const PXSize materialID, PXMaterial* const pxMaterial);


		unsigned char SegmentsAmount();
		void SegmentsGet(const PXSize index, MeshSegment* const meshSegment);
		void SegmentsAdd(const unsigned int renderMode, const unsigned int renderSize, const unsigned int renderMaterial);

		PXSize VertexDataStride();

		PX::ActionResult Load(const PX::Text& const filePath);
		PX::ActionResult LoadD(PX::File& const fileStream, const FileFormatExtension modelType);
	};
}

#endif