#include "PXFile.h"
#include <PX/OS/Error/PXResult.hpp>
#include <PX/Type/PXText.hpp>
namespace PX
{
    class File
    {
        private:
        PXFile* _pxFileHandle;

        public:
        bool DoesExist(const PX::Text& pxText) { return PXFileDoesExist(&pxText); }
        bool DoesExist(const char PXREF filePath) { return PXFileDoesExistA(filePath); }
        PX::Result Remove(const PX::Text& filePath) { return (PX::Result)PXFileRemove(&filePath); }
        PX::Result Rename(const PX::Text& oldName, const PX::Text& newName) { return (PX::Result)PXFileRename(&oldName, &newName); }
    };
}