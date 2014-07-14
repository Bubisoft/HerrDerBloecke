#include "Sound.h"
#include "Globals.h"

using namespace System::IO;
using namespace SlimDX::Multimedia;

HdB::Sound::Sound(DirectSound^ device, String^ file)
    : mFileName(file)
{
    Type = SoundType::SFX;
    SoundBufferDescription desc;
    WaveStream^ stream = gcnew WaveStream(SOUND_PATH + file + ".wav");
    desc.SizeInBytes = stream->Length;
    desc.Format = stream->Format;
    desc.Flags = BufferFlags::GlobalFocus | BufferFlags::ControlVolume;
    mBuffer = gcnew SecondarySoundBuffer(device, desc);
    array<Byte>^ data = gcnew array<Byte>(stream->Length);
    stream->Read(data, 0, stream->Length);
    mBuffer->Write(data, 0, LockFlags::None);
}

HdB::Sound::~Sound()
{
    delete mBuffer;
}

void HdB::Sound::Play(bool loop)
{
    mBuffer->CurrentPlayPosition = 0;
    mBuffer->Play(0, loop ? PlayFlags::Looping : PlayFlags::None);
}

void HdB::Sound::Stop()
{
    mBuffer->Stop();
}

void HdB::Sound::Volume::set(int volume) {
    // Convert so that 0 is Minimum and 100 Maximum
    if(volume == 0)
        mBuffer->Volume = (int)SlimDX::DirectSound::Volume::Minimum;
    else
        mBuffer->Volume = int(Math::Log(volume / 100.f, Math::Exp(1)) * 1000.f);
}

int HdB::Sound::Volume::get()
{
    // Invert conversion (see setter)
    int r;
    if(mBuffer->Volume == (int)SlimDX::DirectSound::Volume::Minimum)
        r = 0;
    else
        r = 100.f * Math::Exp(mBuffer->Volume / 1000.f);

    return r;
}
