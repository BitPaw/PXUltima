#ifndef PXCPPLibraryINCLUDE
#define PXCPPLibraryINCLUDE

#include <OS/Library/PXLibrary.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
    class Library : public PXLibrary
    {
        public:
        Library();
        Library(const Text& const filePath);
        ~Library();

        bool Open(const Text& const filePath); //  gain access to an executable object file. RTLD_LAZY
        bool Close(); // close a dlopen object
        bool GetSymbol(LibraryFunction* libraryFunction, const Text& symbolName); // obtain the address of a symbol from a dlopen object

        PX::ActionResult Name(Text& const libraryName);

        bool ParseSymbols();
    };
}
#endif