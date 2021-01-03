#include "SSfilter.h"

SSfilter::SSfilter()
{
    hpout = 0.0;
    bpout = 0.0;
    lpout = 0.0;
    damping = 0.0;
    F1 = 0.0;
}

void SSfilter::step(double insample)
{
    hpout = -lpout - damping * bpout + insample;
    bpout += F1 * hpout;
    lpout += F1 * bpout;
}

void SSfilter::step(double insample, double Omegac)
{
    double f1 = 2.0 * sin(Omegac / 2.0);
    hpout = -lpout - damping * bpout + insample;
    bpout += f1 * hpout;
    lpout += f1 * bpout;
}

void SSfilter::reset()
{
    hpout = 0.0;
    bpout = 0.0;
    lpout = 0.0;
}
