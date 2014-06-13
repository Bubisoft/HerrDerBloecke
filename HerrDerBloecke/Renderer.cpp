#include "Renderer.h"
#include "Globals.h"
#include "Camera.h"
#include "Model.h"
#include "Unit.h"
#include "Map.h"
#include "HealthBar.h"

HdB::Renderer::Renderer()
{
    mBlueDrawables = gcnew List<IDrawable^>();
    mRedDrawables = gcnew List<IDrawable^>();
    mAlphaDrawables = gcnew List<IDrawable^>();
    mSelectedUnits = gcnew List<Unit^>();
    mFrameTime = Stopwatch::GetTimestamp();
}

HdB::Renderer::~Renderer()
{
    delete mHealthBar;
    mSelectedUnits->Clear();

    for each (IDrawable^ d in mBlueDrawables)
        delete d;
    for each (IDrawable^ d in mAlphaDrawables)
        delete d;
    for each (IDrawable^ d in mRedDrawables)
        delete d;

    mBlueDrawables->Clear();
    mRedDrawables->Clear();
    mAlphaDrawables->Clear();
    delete mMap;

    delete mDevice;
    delete m3D;
}

bool HdB::Renderer::Init(Control^ target)
{
    // Basic configuration
    mParams = gcnew PresentParameters();
    mParams->BackBufferCount = 1;
    mParams->BackBufferHeight = target->ClientRectangle.Height;
    mParams->BackBufferWidth = target->ClientRectangle.Width;
    mParams->DeviceWindowHandle = target->Handle;
    mParams->SwapEffect = SwapEffect::Discard;
    mParams->EnableAutoDepthStencil = true;
    mParams->PresentFlags = PresentFlags::DiscardDepthStencil;
    mParams->PresentationInterval = PresentInterval::Default;
    mParams->Windowed = true;

    // Check whether we have supported hardware
    m3D = gcnew Direct3D();
    int adapter = m3D->Adapters->DefaultAdapter->Adapter;
    mParams->BackBufferFormat = m3D->Adapters->DefaultAdapter->CurrentDisplayMode.Format;
    if (!m3D->CheckDeviceType(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        mParams->BackBufferFormat, true))
        return false;

    // Which depth buffer format can our system use? 32 > 24X8 > 16
    if (m3D->CheckDeviceFormat(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        Usage::DepthStencil, ResourceType::Surface, Format::D32))
        mParams->AutoDepthStencilFormat = Format::D32;
    else if (m3D->CheckDeviceFormat(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        Usage::DepthStencil, ResourceType::Surface, Format::D24X8))
        mParams->AutoDepthStencilFormat = Format::D24X8;
    else if (m3D->CheckDeviceFormat(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        Usage::DepthStencil, ResourceType::Surface, Format::D16))
        mParams->AutoDepthStencilFormat = Format::D16;
    else
        return false;

    // Check for Antialiasing
    if (m3D->CheckDeviceMultisampleType(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        true, MultisampleType::EightSamples))
        mParams->Multisample = MultisampleType::EightSamples;
    else if (m3D->CheckDeviceMultisampleType(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        true, MultisampleType::FourSamples))
        mParams->Multisample = MultisampleType::FourSamples;
    else if (m3D->CheckDeviceMultisampleType(adapter, DeviceType::Hardware, mParams->BackBufferFormat,
        true, MultisampleType::FourSamples))
        mParams->Multisample = MultisampleType::TwoSamples;
    else
        mParams->Multisample = MultisampleType::None;

    // Finally, create our render device
    mDevice = gcnew Device(m3D, adapter, DeviceType::Hardware, target->Handle,
        CreateFlags::HardwareVertexProcessing, mParams);
    ResetDevice();

    mCamera = gcnew HdB::Camera(mDevice, Vector3(0.f, -30.f, 30.f), Vector3::Zero);
    mMap = gcnew HdB::Map(this);
    mHealthBar = gcnew HealthBar(mDevice);

    return true;
}

void HdB::Renderer::Resize(const int% w, const int% h)
{
    if (w < 1 || h < 1)
        return;
    mParams->BackBufferWidth = w;
    mParams->BackBufferHeight = h;
    ResetDevice();
}

void HdB::Renderer::Draw()
{
    long long time = Stopwatch::GetTimestamp();
    long long timeSinceLastFrame = time - mFrameTime;
    mFrameTime = time;

    mDevice->Clear(ClearFlags::Target | ClearFlags::ZBuffer, Color4(0.f, 0.f, 0.f), 1.f, 0);
    mDevice->BeginScene();

    Camera->MoveByKeys(timeSinceLastFrame / 1000000.f);
    mDevice->SetTransform(TransformState::View, Camera->ViewMatrix());
    mDevice->SetTransform(TransformState::Projection, Camera->ProjectionMatrix());

    mMap->Draw(timeSinceLastFrame);

    for each (IDrawable^ d in mBlueDrawables)
        d->Draw(timeSinceLastFrame);

    for each (IDrawable^ d in mRedDrawables)
        d->Draw(timeSinceLastFrame);

    for each (IDrawable^ d in mAlphaDrawables)
        d->Draw(timeSinceLastFrame);

    // Also show targets of selected targets
    for each (Unit^ u in mSelectedUnits->ToArray()) {
        if (Soldier^ s = dynamic_cast<Soldier^>(u))
            if (s->IsInRange() && !mSelectedUnits->Contains(s->AttackTarget))
                mSelectedUnits->Add(s->AttackTarget);
    }

    for each (Unit^ u in mSelectedUnits)
        mHealthBar->DrawForUnit(u);

    DrawSelectionFrame();

    mDevice->EndScene();
    mDevice->Present();
}

void HdB::Renderer::ResetDevice() {
    mDevice->Reset(mParams);

    mDevice->SetRenderState(RenderState::ZEnable, ZBufferType::UseZBuffer);
    mDevice->SetRenderState(RenderState::CullMode, Cull::None);
    mDevice->SetRenderState(RenderState::MultisampleAntialias, true);
    mDevice->SetRenderState(RenderState::AlphaBlendEnable, true);
    mDevice->SetRenderState(RenderState::SourceBlend, Blend::SourceAlpha);
    mDevice->SetRenderState(RenderState::DestinationBlend, Blend::InverseSourceAlpha);
    mDevice->SetRenderState(RenderState::DiffuseMaterialSource, ColorSource::Material);
    mDevice->SetRenderState(RenderState::Lighting, true);
    mDevice->SetRenderState(RenderState::Ambient, Color4(1.f, 1.f, 1.f).ToArgb());

    // Static fog environment, can be enabled later
    mDevice->SetRenderState(RenderState::FogEnable, false);
    mDevice->SetRenderState(RenderState::FogTableMode, FogMode::Exponential);
    mDevice->SetRenderState(RenderState::FogVertexMode, FogMode::None);
    mDevice->SetRenderState(RenderState::FogDensity, .02f);
    mDevice->SetRenderState(RenderState::FogColor, Color4(.9f, .9f, .8f).ToArgb());

    mDevice->SetSamplerState(0, SamplerState::MinFilter, TextureFilter::Linear);
    mDevice->SetSamplerState(0, SamplerState::MagFilter, TextureFilter::Linear);
    mDevice->SetSamplerState(0, SamplerState::MipFilter, TextureFilter::Linear);

    Light l;
    l.Position = Vector3(1000.f, 1000.f, -1000.f);
    l.Type = LightType::Point;
    l.Direction = Vector3::Zero;
    l.Diffuse = Color4(1.f, 1.f, 1.f);
    l.Range = 5000.f;
    mDevice->SetLight(0, l);
    mDevice->EnableLight(0, true);
}

void HdB::Renderer::DrawSelectionFrame()
{
    if (SelectionFrame) {
        // Create 2D Box as Rectangle
        float width = 2.f * SelectionFrame->Width / mDevice->Viewport.Width;
        float height = 2.f * SelectionFrame->Height / mDevice->Viewport.Height;
        float x = 2.f * SelectionFrame->X / mDevice->Viewport.Width + width / 2.f - 1.f;
        float y = -(2.f * SelectionFrame->Y / mDevice->Viewport.Height + height / 2.f - 1.f);
        Mesh^ box = Mesh::CreateBox(mDevice, Math::Abs(width), Math::Abs(height), 0.f);
        mDevice->SetTransform(TransformState::World, Matrix::Translation(Vector3(x, y, 0.f)));
        mDevice->SetTransform(TransformState::View, Matrix::Identity);
        mDevice->SetTransform(TransformState::Projection, Matrix::Identity);

        // Draw Rectangle
        Material m;
        m.Diffuse = Color4(0.1f, 0.f, 1.f, 0.f);
        mDevice->Material = m;
        box->DrawSubset(0);
        delete box;
    }
}

HdB::Model^ HdB::Renderer::GetBlueModel(String^ name)
{
    Model^ m;
    for each (IDrawable^ d in mBlueDrawables)
        if ((m = dynamic_cast<Model^>(d)) && m->Name == name)
            return m;

    m = gcnew Model(name, this);
    m->SetTeamColor(Color4(0.f, 0.f, 1.f));
    mBlueDrawables->Add(m);
    return m;
}

HdB::Model^ HdB::Renderer::GetRedModel(String^ name)
{
    Model^ m;
    for each (IDrawable^ d in mRedDrawables)
        if ((m = dynamic_cast<Model^>(d)) && m->Name == name)
            return m;

    m = gcnew Model(name, this);
    m->SetTeamColor(Color4(1.f, 0.f, 0.f));
    mRedDrawables->Add(m);
    return m;
}

HdB::Model^ HdB::Renderer::GetAlphaModel(String^ name)
{
    Model^ m;
    for each (IDrawable^ d in mAlphaDrawables) {
        if (m = dynamic_cast<Model^>(d))
            if (m->Name == name)
                return m;
    }
    m = gcnew Model(name, this);
    m->SetAlpha(0.5f); //Hardcoded alpha
    m->IsPlaceholder = true;
    mAlphaDrawables->Add(m);
    return m;
}
