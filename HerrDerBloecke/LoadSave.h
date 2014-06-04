#pragma once
#include "Map.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerAI.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
namespace HdB
{
    ref class LoadSave
    {
    public:
        LoadSave(void);
        void SaveGame(Map^ map,Player^ player, PlayerAI^ enemy);
        void LoadGame(Map^ map,Player^ player,PlayerAI^ enemy,Renderer^ renderer);
    };
}

