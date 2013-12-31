#pragma once

#include "Unit.h"

using namespace System::Collections::Generic;
//using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Model
    {
    public:
        Model();
        void Draw();
    private:
        List<Unit^>^ mInstances;
        /*VertexBuffer mVertices;
        IndexBuffer mIndices;*/
    };
}
