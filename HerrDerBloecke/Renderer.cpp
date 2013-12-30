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

    mCamera = gcnew Camera(Vector3(0.f, 10.f, 9.f), Vector3::Zero);
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
    mDevice->Clear(ClearFlags::Target | ClearFlags::ZBuffer, Color4(0.f, 0.f, 0.f), 1.f, 0);
    mDevice->BeginScene();
    mDevice->SetTransform(TransformState::World, Matrix::RotationY(0));
    mDevice->SetTransform(TransformState::View, mCamera->ViewMatrix());
    mDevice->SetTransform(TransformState::Projection,
            Matrix::PerspectiveFovRH(45.f, mDevice->Viewport.Width * 1.f / mDevice->Viewport.Height, 1.0f, 100.0f));
    meme->DrawSubset(0);
    mDevice->EndScene();
    mDevice->Present();
}

void Renderer::ResetDevice() {
    mDevice->Reset(mParams);
    mDevice->SetRenderState(RenderState::ZEnable, ZBufferType::UseZBuffer);
    mDevice->SetRenderState(RenderState::CullMode, Cull::Clockwise);
    mDevice->SetRenderState(RenderState::Lighting, true);

    Light l;
    l.Position = Vector3(0.f, 100.f, 0.f);
    l.Type = LightType::Point;
    l.Direction = Vector3::Zero;
    l.Diffuse = Color4(1.f, 1.f, 1.f);
    l.Range = 200.f;
    mDevice->SetLight(0, l);
    mDevice->EnableLight(0, true);

    Material mat;
    mat.Diffuse = Color4(.75f, .75f, .75f);
    mDevice->Material = mat;

}

void Renderer::MoveCamera(Vector3& change)
{
		mCamera->MoveCamera(change);
}

void Renderer::SetCameraSpeed(float _speed)
{
	mCamera->mCameraSpeed=_speed;
}

