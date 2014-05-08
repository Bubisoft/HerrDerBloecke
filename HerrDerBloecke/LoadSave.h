#pragma once
#include "Map.h";
#include "Renderer.h";
#include "Player.h";

using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
namespace HdB
{
    ref class LoadSave
    {
    public:
        LoadSave(void);
        void SaveGame(Map^ map,Player^ player);
        void LoadGame(Map^ map,Player^ player,Renderer^ renderer);
    };
}

