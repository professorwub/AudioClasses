/*
  ==============================================================================

    DelayLine.cpp
    Created: 17 Nov 2019 10:28:01am
    Author:  profw

  ==============================================================================
*/

#include "DelayLine.h"

DelayLine::DelayLine()
{
	head = 0;
	outsample = 0.0;
	damping = 0.0;
}

void DelayLine::setsampledelay(int sampledelay)
{
	// This resizes the delay line
	buffer.resize(sampledelay, 0.0);
	if (head >= buffer.size())
		head = 0;
}

double DelayLine::step(double sample)
{
	// Update output, then insert sample
	outsample = damping * outsample + (1.0 - damping) * buffer[head];
	buffer[head] = sample;
	head = ++head % buffer.size();
	return outsample;
}

void DelayLine::reset()
{
	for (auto& x : buffer)
		x = 0.0;
	head = 0;
	outsample = 0.0;
}
