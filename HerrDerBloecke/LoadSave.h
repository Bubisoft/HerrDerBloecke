#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerAI.h"
#include "Score.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
namespace HdB
{
    ref class LoadSave
    {
    public:
        LoadSave(void);
        void SaveGame(Map^ map,Player^ player, PlayerAI^ enemy,Score^ PlayerScore,Score^ EnemyScore);
        void LoadGame(Map^% map,Player^% player,PlayerAI^% enemy,Score^% PlayerScore,Score^% EnemyScore,Renderer^% renderer);
    };
}

