#pragma once

ref class myTimer
{
private:
    long count;
    long frequency;
    bool isRunning;
public:
    myTimer(void);
    void Start();
    void Stop();
    float Update();
};

