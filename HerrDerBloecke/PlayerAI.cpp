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
#define START_SOLDIER(atTime, UnitType, Model, pos) \
    if (mSeconds == atTime) { \
        Unit^ u = gcnew UnitType(mRenderer->GetRedModel(Model), mPositionHQ + Vector3(pos, 0.f)); \
        BuildUnit(u, u->BuildTime(), nullptr); \
    }

HdB::PlayerAI::PlayerAI(Renderer^ renderer, const Vector3% posHQ) : mRenderer(renderer), mPositionHQ(posHQ)
{
    // Initialize AI schedule timer
    mTimer = gcnew Timer();
    mTimer->Interval = 1000;
    mTimer->Tick += gcnew EventHandler(this, &PlayerAI::CheckSchedule);
    mTimer->Enabled = true;

    // Register callback for soldiers
    UnitBuilt += gcnew UnitEvent(this, &PlayerAI::OnNewUnit);

    Unit^ u = gcnew Hauptgebaeude(mRenderer->GetRedModel("Hauptgebaeude"), mPositionHQ);
    BuildUnit(u, 0, nullptr);
    mRenderer->Map->AddUnit(u);
}

void HdB::PlayerAI::CheckSchedule(Object^ source, EventArgs^ e)
{
    mSeconds++;
    static const Vector2 blockstattPos = Vector2(25.f, 25.f);

    // Define build schedule here, positions relative to the headquarters
    START_BUILDING(5, Blockhuette, "Blockhaus", Vector2(-25.f, -25.f));
    START_BUILDING(10, Blockfarm, "Kastenfarm", Vector2(25.f, -25.f));
    START_BUILDING(15, Blockwerk, "Blockwerk", Vector2(50.f, 50.f));
    START_BUILDING(20, Blockstatt, "Blockstatt", blockstattPos);
    START_SOLDIER(30, ZuseZ3, "ZuseZ3", blockstattPos + Vector2(25.f, -25.f));
}

void HdB::PlayerAI::OnNewUnit(Unit^ unit)
{
    if (unit->GetType()->IsSubclassOf(Soldier::typeid))
        mRenderer->Map->AddUnit(unit);
}
