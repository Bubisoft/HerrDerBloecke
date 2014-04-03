#pragma once

using namespace System;
using namespace SlimDX::DirectSound;

namespace HdB {
    ref class Sound
    {
    public:
        Sound(DirectSound^ device, String^ filename);
        virtual ~Sound();
        void Play(bool loop);
        void Stop();
        
        property int Volume {
            void set(int volume) { mBuffer->Volume = volume; }
            int get() { return mBuffer->Volume; }
        }
        property String^ Name {
            String^ get() { return mFileName; }
        }
    protected:
        String^ mFileName;
        SecondarySoundBuffer^ mBuffer;
    };
}
