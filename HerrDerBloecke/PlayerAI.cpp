#include "PlayerAI.h"
#include "Renderer.h"
#include "Map.h"
#include "Unit.h"

// Schedule macros
#define START_BUILDING(atTime, UnitType, Model, pos) \
    if (mSeconds == atTime) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        Unit^ ph = gcnew UnitType(mRenderer->GetAlphaModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        BuildUnit(u, u->BuildTime(), ph); \
        mRenderer->Map->AddUnit(u); \
    }

HdB::PlayerAI::PlayerAI(Renderer^ renderer, const Vector3% posHQ) : mRenderer(renderer), mPositionHQ(posHQ)
{
    // Initialize AI schedule timer
    mTimer = gcnew Timer();
    mTimer->Interval = 1000;
    mTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckSchedule);
    mTimer->Enabled = true;

    Unit^ u = gcnew Hauptgebaeude(mRenderer->GetRedModel("Hauptgebaeude"), mPositionHQ);
    Units->Add(u);
    u->Spawn();
    mRenderer->Map->AddUnit(u);
}

void HdB::PlayerAI::CheckSchedule(Object^ source, EventArgs^ e)
{
    mSeconds++;

    // Define build schedule here, positions relative to the headquarters
    START_BUILDING(5, Blockstatt, "Blockstatt", Vector2(500.f, 50.f));
    START_BUILDING(10, Blockhuette, "Blockhaus", Vector2(-50.f, -50.f));
    START_BUILDING(15, Blockfarm, "Kastenfarm", Vector2(50.f, -50.f));
}
