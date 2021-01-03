/*
  ==============================================================================

    LPAPfilter.cpp
    Created: 5 Aug 2020 9:23:43am
    Author:  profw

  ==============================================================================
*/

#include "LPAPfilter.h"

LPAPfilter::LPAPfilter()
{
    damping = 0.0;
    reflection = 0.0;
    oldest = 0;
    xprev = 0.0f;
    yprev = 0.0f;
}

void LPAPfilter::setdelay(int delay)
{
    xbuffer.resize(delay, 0.0f);
    ybuffer.resize(delay, 0.0f);
    if (oldest >= delay)
        oldest = 0;
}

void LPAPfilter::reset()
{
    for (auto n = 0; n < xbuffer.size(); n++)
    {
        xbuffer[n] = 0.0f;
        ybuffer[n] = 0.0f;
    }
    oldest = 0;
    xprev = 0.0f;
    yprev = 0.0f;
}

double LPAPfilter::step(double sample)
{
    float outsample = damping * yprev + reflection * (1.0 - damping) * ybuffer[oldest] - reflection * sample 
        + reflection * damping * xprev + (1 - damping) * xbuffer[oldest];
    yprev = outsample;
    xprev = sample;
    xbuffer[oldest] = sample;
    ybuffer[oldest] = outsample;
    oldest = ++oldest % xbuffer.size();
    return outsample;
}
