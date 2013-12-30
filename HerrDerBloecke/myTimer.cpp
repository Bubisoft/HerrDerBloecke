#include "myTimer.h"
using namespace System::Diagnostics;

myTimer::myTimer(void)
{
    frequency=Stopwatch::Frequency;
}

void myTimer::Start()
{
    count=Stopwatch::GetTimestamp();
    isRunning=true;
}

void myTimer::Stop()
{
    isRunning=false;
}

float myTimer::Update()
{
    float elapsed=0.0f;
    if(isRunning)
    {
        long last=count;
        count=Stopwatch::GetTimestamp();
        elapsed=(float)(count - last)/frequency;
    }
    return elapsed;
}
