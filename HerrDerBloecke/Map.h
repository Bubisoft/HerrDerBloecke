#pragma once

#include "IDrawable.h"

using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Map : IDrawable
    {
    public:
        /** Construct our renderable map. */
        Map(Device^ device);

        /** Render the map. */
        virtual void Draw();

    private:
        Device^ mDevice;
    };
}
