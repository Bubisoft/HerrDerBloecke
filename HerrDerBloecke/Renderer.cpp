#include "Renderer.h"

Renderer::Renderer(Control^ target)
{
    PresentParameters^ presentParams = gcnew PresentParameters();
    presentParams->BackBufferFormat = Format::X8R8G8B8;
    presentParams->BackBufferHeight = target->ClientRectangle.Height;
    presentParams->BackBufferWidth = target->ClientRectangle.Width;
    presentParams->DeviceWindowHandle = target->Handle;
	presentParams->BackBufferCount = 1;
	presentParams->Multisample = MultisampleType::EightSamples;
	presentParams->SwapEffect = SwapEffect::Discard;
	presentParams->EnableAutoDepthStencil = true;
	presentParams->AutoDepthStencilFormat = Format::D16;
	presentParams->PresentFlags = PresentFlags::DiscardDepthStencil;
	presentParams->PresentationInterval = PresentInterval::Default;
	presentParams->Windowed = true;

    m3D = gcnew Direct3D();
    mDevice = gcnew Device(m3D, 0, DeviceType::Hardware, target->Handle, CreateFlags::HardwareVertexProcessing, presentParams);
    mDevice->SetRenderState(SlimDX::Direct3D9::RenderState::ZEnable, SlimDX::Direct3D9::ZBufferType::UseZBuffer);
	mDevice->SetRenderState(SlimDX::Direct3D9::RenderState::CullMode, SlimDX::Direct3D9::Cull::None);

	mDevice->SetRenderState(SlimDX::Direct3D9::RenderState::Lighting, false);
	meme = SlimDX::Direct3D9::Mesh::CreateTeapot(mDevice);
}

void Renderer::Resize(const int& w, const int& h)
{
    mDevice->Viewport.Width = w;
    mDevice->Viewport.Height = h;
}

void Renderer::RenderFrame()
{
    mDevice->Clear(ClearFlags::Target | ClearFlags::ZBuffer, Color4(0.f, 0.f, 0.f), 1.0f, 0);
    mDevice->BeginScene();
    mDevice->SetTransform(SlimDX::Direct3D9::TransformState::World, SlimDX::Matrix::RotationY(1));
	mDevice->SetTransform(SlimDX::Direct3D9::TransformState::View,
							SlimDX::Matrix::LookAtRH(SlimDX::Vector3(-10.0f, 0.0f, 0.f),
							SlimDX::Vector3(0.0f, 0.0f, 0.0f), SlimDX::Vector3(0.0f, 1.f, 0.0f)));
	mDevice->SetTransform(SlimDX::Direct3D9::TransformState::Projection,
							SlimDX::Matrix::PerspectiveFovRH(45.f, mDevice->Viewport.Width / mDevice->Viewport.Height, 1.0f, 100.0f));
    meme->DrawSubset(0);
    mDevice->EndScene();
    mDevice->Present();
}
