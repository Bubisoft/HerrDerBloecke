#include "Timer.h"

using namespace System::Diagnostics;

HdB::Timer::Timer()
    : mFrequency(Stopwatch::Frequency)
{
}

void HdB::Timer::Start()
{
    mCount = Stopwatch::GetTimestamp();
    mIsRunning = true;
}

void HdB::Timer::Stop()
{
    mIsRunning = false;
}

float HdB::Timer::Update()
{
    float elapsed = 0.f;
    if (mIsRunning) {
        long long last = mCount;
        mCount = Stopwatch::GetTimestamp();
        elapsed = safe_cast<float>(mCount - last) / mFrequency;
    }
    return elapsed;
}
