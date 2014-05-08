#pragma once
using namespace System::IO;
namespace HdB {
    ref class Renderer;
    interface class ISaveable
    {
        void Save(BinaryWriter^ bw);
        void Load(BinaryReader^ br,Renderer^ renderer);
    };
}