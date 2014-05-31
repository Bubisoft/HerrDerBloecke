#include "HealthBar.h"
#include "Globals.h"
#include "Unit.h"

#define HB_TEXTURE "HealthBar.png"
#define HB_TEX_WIDTH 512
#define HB_TEX_HEIGHT 64
#define HB_SCALE 0.02f
#define ATTACKING_TEXTURE "attacking.png"
#define ATK_TEX_WIDTH 512
#define ATK_TEX_HEIGHT 512
#define ATK_SCALE 0.005f

using namespace System::IO;
using namespace System::Diagnostics;

HdB::HealthBar::HealthBar(Device^ device)
    : mDevice(device), mBarTexture(nullptr), mAttackTexture(nullptr)
{
    try {
        mBarTexture = Texture::FromFile(mDevice, TEXTURE_PATH + HB_TEXTURE);
    } catch (Exception^ e) {
        Debug::WriteLine("ERROR: Could not load texture " + HB_TEXTURE);
    }
    try {
        mAttackTexture = Texture::FromFile(mDevice, TEXTURE_PATH + ATTACKING_TEXTURE);
    } catch (Exception^ e) {
        Debug::WriteLine("ERROR: Could not load texture " + ATTACKING_TEXTURE);
    }
}

HdB::HealthBar::~HealthBar()
{
    delete mBarTexture;
    delete mAttackTexture;
}

void HdB::HealthBar::DrawForUnit(Unit^ unit)
{
    // Create and draw the Healthbar sprite
    mSprite = gcnew Sprite(mDevice);
    float barZ = unit->GetType()->IsSubclassOf(Soldier::typeid) ? 5.f : 20.f;
    Vector3 pos3D = Vector3(unit->Position.X, unit->Position.Y, barZ);
    mSprite->SetWorldViewRH(Matrix::Translation(pos3D), mDevice->GetTransform(TransformState::View));
    mSprite->Begin(SpriteFlags::Billboard);

    float healthy = HB_TEX_WIDTH * unit->PercentHP();
    mSprite->Transform = Matrix::Scaling(HB_SCALE, HB_SCALE, 0.f); // Scale down (otherwise: texture size in meters)
    mSprite->Draw(mBarTexture, Rectangle(0, 0, HB_TEX_WIDTH, HB_TEX_HEIGHT),
        Vector3(HB_TEX_WIDTH / 2.f, HB_TEX_HEIGHT / 2.f, 0.f), Vector3::Zero, Color::Red);
    mSprite->Draw(mBarTexture, Rectangle(0, 0, healthy, HB_TEX_HEIGHT),
        Vector3(HB_TEX_WIDTH / 2.f, HB_TEX_HEIGHT / 2.f, 0.f), Vector3::Zero, Color::Green);

    mSprite->End();
    delete mSprite;

    // Draw an extra sprite to mark attacking units
    Soldier^ s = dynamic_cast<Soldier^>(unit);
    if (s && s->IsInRange()) {
        mSprite = gcnew Sprite(mDevice);
        mSprite->SetWorldViewRH(Matrix::Translation(pos3D), mDevice->GetTransform(TransformState::View));
        mSprite->Begin(SpriteFlags::Billboard | SpriteFlags::AlphaBlend);
        mSprite->Transform = Matrix::Scaling(ATK_SCALE, ATK_SCALE, 0.f);
        mSprite->Draw(mAttackTexture, Vector3(ATK_TEX_WIDTH / 2.f, -ATK_TEX_HEIGHT / 2.f, 0.f), Vector3::Zero, Color::White);
        mSprite->End();
        delete mSprite;
    }
}
