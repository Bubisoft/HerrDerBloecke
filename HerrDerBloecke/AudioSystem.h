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
        void PlaySFX(String^ name);
        void PlayMusic(String^ name);

        property int VolumeSFX {
            void set(int volume);
            int get() { return mVolumeSFX; }
        }
        property int VolumeMusic {
            void set(int volume);
            int get() { return mVolumeMusic; }
        }

    private:
        bool Play(String^ name);

        SlimDX::DirectSound::DirectSound^ mSoundDev;
        PrimarySoundBuffer^ mPrimaryBuffer;
        List<Sound^>^ mLoadedSounds;
        int mVolumeSFX;
        int mVolumeMusic;
    };
}
