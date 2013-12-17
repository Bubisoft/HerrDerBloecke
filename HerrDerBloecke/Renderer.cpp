#include "Renderer.h"

Renderer::Renderer(Control^ target)
{
    mParams = gcnew PresentParameters();
    mParams->BackBufferFormat = Format::X8R8G8B8;
    mParams->BackBufferHeight = target->ClientRectangle.Height;
    mParams->BackBufferWidth = target->ClientRectangle.Width;
    mParams->DeviceWindowHandle = target->Handle;
    mParams->BackBufferCount = 1;
    mParams->Multisample = MultisampleType::EightSamples;
    mParams->SwapEffect = SwapEffect::Discard;
    mParams->EnableAutoDepthStencil = true;
    mParams->AutoDepthStencilFormat = Format::D16;
    mParams->PresentFlags = PresentFlags::DiscardDepthStencil;
    mParams->PresentationInterval = PresentInterval::Default;
    mParams->Windowed = true;

    m3D = gcnew Direct3D();
    mDevice = gcnew Device(m3D, 0, DeviceType::Hardware, target->Handle, CreateFlags::HardwareVertexProcessing, mParams);
    ResetDevice();

    meme = Mesh::CreateTeapot(mDevice);
}

void Renderer::Resize(const int& w, const int& h)
{
    if (w < 1 || h < 1)
        return;
    mParams->BackBufferWidth = w;
    mParams->BackBufferHeight = h;
    ResetDevice();
}

void Renderer::Draw()
{
    mDevice->Clear(ClearFlags::Target | ClearFlags::ZBuffer, Color4(0.f, 0.f, 0.f), 1.0f, 0);
    mDevice->BeginScene();
    mDevice->SetTransform(TransformState::World, Matrix::RotationY(1));
    mDevice->SetTransform(TransformState::View, Matrix::LookAtRH(Vector3(-10.0f, 0.0f, 0.f),
            Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.f, 0.0f)));
    mDevice->SetTransform(TransformState::Projection,
            Matrix::PerspectiveFovRH(45.f, mDevice->Viewport.Width * 1.f / mDevice->Viewport.Height, 1.0f, 100.0f));
    meme->DrawSubset(0);
    mDevice->EndScene();
    mDevice->Present();
}

void Renderer::ResetDevice() {
    mDevice->Reset(mParams);
    mDevice->SetRenderState(RenderState::ZEnable, ZBufferType::UseZBuffer);
    mDevice->SetRenderState(RenderState::CullMode, Cull::None);
    mDevice->SetRenderState(RenderState::Lighting, false);
}
