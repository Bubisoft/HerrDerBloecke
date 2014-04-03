#include "AudioSystem.h"
#include "Sound.h"

HdB::AudioSystem::AudioSystem()
{
    mLoadedSounds = gcnew List<Sound^>();
}

HdB::AudioSystem::~AudioSystem()
{
    for each (Sound^ s in mLoadedSounds)
        delete s;
    mLoadedSounds->Clear();
    delete mPrimaryBuffer;
    delete mSoundDev;
}

bool HdB::AudioSystem::Init(Control^ target)
{
    // Initialize system
    mSoundDev = gcnew DirectSound();
    mSoundDev->SetCooperativeLevel(target->Handle, CooperativeLevel::Priority);
    SoundBufferDescription desc;
    desc.Flags = BufferFlags::PrimaryBuffer;
    //primaryBufferDesc.AlgorithmFor3D = System::Guid::Empty;
    mPrimaryBuffer = gcnew PrimarySoundBuffer(mSoundDev, desc);
    mPrimaryBuffer->Play(0, PlayFlags::Looping);
    return true;
}

void HdB::AudioSystem::PlaySound(String^ name)
{
    // Load on demand
    for each (Sound^ s in mLoadedSounds) {
        if (s->Name == name) {
            s->Play(false);
            return;
        }
    }
    Sound^ sound = gcnew Sound(mSoundDev, name);
    mLoadedSounds->Add(sound);
    sound->Play(false);
}
