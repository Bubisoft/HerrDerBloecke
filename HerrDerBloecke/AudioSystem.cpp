#include "AudioSystem.h"
#include "Sound.h"

HdB::AudioSystem::AudioSystem()
    : mVolumeSFX(100), mVolumeMusic(50)
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
    mPrimaryBuffer = gcnew PrimarySoundBuffer(mSoundDev, desc);
    mPrimaryBuffer->Play(0, PlayFlags::Looping);
    return true;
}

void HdB::AudioSystem::PlaySFX(String^ name)
{
    PlaySFX(name, false);
}

void HdB::AudioSystem::PlaySFX(String^ name, bool loop)
{
    // Load on demand
    if (Play(name))
        return;
    Sound^ sound = gcnew Sound(mSoundDev, name);
    sound->Type = SoundType::SFX;
    sound->Volume = VolumeSFX;
    mLoadedSounds->Add(sound);
    sound->Play(loop);
}

void HdB::AudioSystem::PlayMusic(String^ name)
{
    PlayMusic(name, false);
}

void HdB::AudioSystem::PlayMusic(String^ name, bool loop)
{
    // Load on demand
    if (Play(name))
        return;
    Sound^ sound = gcnew Sound(mSoundDev, name);
    sound->Type = SoundType::Music;
    sound->Volume = VolumeMusic;
    mLoadedSounds->Add(sound);
    sound->Play(loop);
}

bool HdB::AudioSystem::Play(String^ name)
{
    for each (Sound^ s in mLoadedSounds) {
        if (s->Name == name) {
            s->Play(false);
            return true;
        }
    }
    return false;
}

void HdB::AudioSystem::Stop(String^ name)
{
    for each (Sound^ s in mLoadedSounds) {
        if (s->Name == name) {
            s->Stop();
            return;
        }
    }
}

void HdB::AudioSystem::VolumeSFX::set(int volume)
{
    mVolumeSFX = volume;
    for each (Sound^ s in mLoadedSounds)
        if (s->Type == SoundType::SFX)
            s->Volume = volume;
}

void HdB::AudioSystem::VolumeMusic::set(int volume)
{
    mVolumeMusic = volume;
    for each (Sound^ s in mLoadedSounds)
        if (s->Type == SoundType::Music)
            s->Volume = volume;
}
