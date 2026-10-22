
#pragma once

#if PXLanguageCPP

#include <PX/Type/PXMaterial.h>

namespace PX
{
    class Material : public PXMaterial
    {
        private:
        PXMaterial* MaterialRef;

        public:
        Material()
        {
            PXMaterialCreate(&MaterialRef, );
        }
        ~Material()
        {

        }
    };
}

#endif