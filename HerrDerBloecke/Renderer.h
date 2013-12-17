#pragma once

using namespace System::Windows::Forms;
using namespace SlimDX;
using namespace SlimDX::Direct3D9;
using namespace SlimDX::Windows;

ref class Renderer {
public:
    Renderer(Control^ target);
    void Resize(const int& w, const int& h);
    void RenderFrame();
    MainLoop^ loop;

private:
    Direct3D^ m3D;
    Device^ mDevice;
    Mesh ^meme;
};
