#include "HealthBar.h"
#include "Globals.h"
#include "Unit.h"

using namespace System::IO;
using namespace System::Diagnostics;

HdB::HealthBar::HealthBar(Device^ device)
    : mDevice(device), mTexture(nullptr)
{
    try {
        mTexture = Texture::FromFile(mDevice, TEXTURE_PATH + "HealthBar.png");
    } catch (Exception^ e) {
        Debug::WriteLine("ERROR: Could not load texture HealthBar.png");
    }
}

HdB::HealthBar::~HealthBar()
{
    delete mTexture;
}

void HdB::HealthBar::DrawForUnit(Unit^ unit)
{
    mSprite = gcnew Sprite(mDevice);
    Vector3 pos3D = Vector3(unit->Position.X, unit->Position.Y, 20.f); // Draw 20m above unit
    mSprite->SetWorldViewRH(Matrix::Translation(pos3D), mDevice->GetTransform(TransformState::View));
    mSprite->Begin(SpriteFlags::Billboard);

    int width = 512, height = 64; // Texture size... Needed for rectangle clipping below
    float healthy = width * unit->PercentHP();
    mSprite->Transform = Matrix::Scaling(0.02f, 0.02f, 0.f); // Scale down (otherwise: texture size in meters)
    mSprite->Draw(mTexture, Rectangle(0, 0, width, height), Vector3(width / 2.f, height / 2.f, 0.f), Vector3::Zero, Color::Red);
    mSprite->Draw(mTexture, Rectangle(0, 0, healthy, height), Vector3(width / 2.f, height / 2.f, 0.f), Vector3::Zero, Color::Green);

    mSprite->End();
    delete mSprite;
}
