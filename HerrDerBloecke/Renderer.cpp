#include "Renderer.h"
#include "Camera.h"
#include "Model.h"
#include "Unit.h"

HdB::Renderer::Renderer(Control^ target)
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

    Camera = gcnew HdB::Camera(Vector3(0.f, -10.f, 10.f), Vector3::Zero);
    mDrawables = gcnew List<IDrawable^>();

    // TEMP Test: Load Model
    AddDrawable(gcnew Model("exampleUnit", mDevice));
    SpawnUnit(gcnew TestUnit("exampleUnit", Vector3::Zero));
    SpawnUnit(gcnew TestUnit("exampleUnit", Vector3(-5.f, -5.f, 0.f)));
    SpawnUnit(gcnew TestUnit("exampleUnit", Vector3(5.f, 5.f, 0.f)));
}

HdB::Renderer::~Renderer()
{
    for each (IDrawable^ d in mDrawables)
        delete d;
    mDrawables->Clear();

    delete mDevice;
    delete m3D;
}

void HdB::Renderer::Resize(const int& w, const int& h)
{
    if (w < 1 || h < 1)
        return;
    mParams->BackBufferWidth = w;
    mParams->BackBufferHeight = h;
    ResetDevice();
}

void HdB::Renderer::Draw()
{
    if (Paused)
        return;

    mDevice->Clear(ClearFlags::Target | ClearFlags::ZBuffer, Color4(0.f, 0.f, 0.f), 1.f, 0);
    mDevice->BeginScene();
    mDevice->SetTransform(TransformState::View, Camera->ViewMatrix());
    mDevice->SetTransform(TransformState::Projection,
            Matrix::PerspectiveFovRH(System::Math::PI / 4.f, mDevice->Viewport.Width * 1.f / mDevice->Viewport.Height, 1.0f, 100.0f));

    for each (IDrawable^ d in mDrawables) {
        d->Draw();
    }

    mDevice->EndScene();
    mDevice->Present();
}

void HdB::Renderer::ResetDevice() {
    mDevice->Reset(mParams);
    mDevice->SetRenderState(RenderState::ZEnable, ZBufferType::UseZBuffer);
    mDevice->SetRenderState(RenderState::CullMode, Cull::None);
    mDevice->SetRenderState(RenderState::AlphaBlendEnable, true);
    mDevice->SetRenderState(RenderState::SourceBlend, Blend::SourceAlpha);
    mDevice->SetRenderState(RenderState::DestinationBlend, Blend::InverseSourceAlpha);
    mDevice->SetRenderState(RenderState::Lighting, true);
    mDevice->SetRenderState(RenderState::Ambient, Color4(1.f, 1.f, 1.f).ToArgb());

    Light l;
    l.Position = Vector3(100.f, 100.f, 100.f);
    l.Type = LightType::Point;
    l.Direction = Vector3::Zero;
    l.Diffuse = Color4(1.f, 1.f, 1.f);
    l.Range = 500.f;
    mDevice->SetLight(0, l);
    mDevice->EnableLight(0, true);
}

void HdB::Renderer::AddDrawable(IDrawable^ drawable)
{
    mDrawables->Add(drawable);
}

void HdB::Renderer::SpawnUnit(Unit^ unit)
{
    for each (IDrawable^ d in mDrawables) {
        if (Model^ m = dynamic_cast<Model^>(d))
            if (m->Name == unit->ModelName)
                m->AddInstance(unit);
    }
}
