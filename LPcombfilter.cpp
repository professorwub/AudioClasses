/*
  ==============================================================================

    LPcombfilter.cpp
    Created: 28 Jul 2020 10:36:40am
    Author:  profw

  ==============================================================================
*/

#include "LPcombfilter.h"

LPcombfilter::LPcombfilter()
{
    damping = 0.0;
    reflection = 0.0;
    oldest = 0;
    state = 0.0;
}

void LPcombfilter::setdelay(int delay)
{
    buffer.resize(delay, 0.0);
    if (oldest >= buffer.size())
        oldest = 0;
}

double LPcombfilter::step(double sample)
{
    state = damping * state + reflection * (1.0 - damping) * buffer[oldest];
    float outsample = state + sample;
    buffer[oldest] = outsample;
    oldest = ++oldest % buffer.size();
    return outsample;
}

void LPcombfilter::reset()
{
    for (auto& sample : buffer)
        sample = 0.0f;
    oldest = 0;
    state = 0.0;
}
