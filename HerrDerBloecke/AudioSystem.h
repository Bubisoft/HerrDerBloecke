#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace SlimDX::DirectSound;

namespace HdB {
    ref class Sound;

    ref class AudioSystem
    {
    public:
        AudioSystem();
        virtual ~AudioSystem();
        bool Init(Control^ target);
        void PlaySound(String^ name);

    private:
        SlimDX::DirectSound::DirectSound^ mSoundDev;
        PrimarySoundBuffer^ mPrimaryBuffer;
        List<Sound^>^ mLoadedSounds;
    };
}
