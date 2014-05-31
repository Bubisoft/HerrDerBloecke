#pragma once

using namespace System::Drawing;
using namespace SlimDX::Direct3D9;

namespace HdB {
    ref class Unit;

    ref class HealthBar
    {
    public:
        HealthBar(Device^ device);
        virtual ~HealthBar();
        void DrawForUnit(Unit^ unit);

    protected:
        Device^ mDevice;
        Texture^ mBarTexture;
        Texture^ mAttackTexture;
        Sprite^ mSprite;
    };
}
