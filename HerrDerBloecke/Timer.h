#pragma once

namespace HdB {
    ref class Timer
    {
    public:
        Timer();
        void Start();
        void Stop();
        float Update();

    private:
        long long mCount;
        long long mFrequency;
        bool mIsRunning;
    };
}
