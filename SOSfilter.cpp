/*
  ==============================================================================

    SOSfilter.cpp
    Created: 26 Apr 2018 8:29:28am
    Author:  profw

  ==============================================================================
*/

#include "SOSfilter.h"

SOSfilter::SOSfilter()
{
	sampRate = 44100.0f;
}

void SOSfilter::initsos(int numsects, double fs)
{
	sampRate = fs;
	SOScascade.resize(numsects);
	for (auto& sos : SOScascade)
		sos = std::unique_ptr<BQfilter>(new BQfilter);
}

void SOSfilter::updateSection(int sect, FilterType ftype, double Gain, double f0, double Q)
{
	SOScascade[sect]->update(ftype, Gain, f0, Q, sampRate);
}

double SOSfilter::step(double sample)
{
	double y = sample;
	for (auto& bq : SOScascade)
		y = bq->step(y);
	return y;
}

double SOSfilter::freqResponse(double freq)
{
	// Compute magnitude of frequency response (dB) at given frequency
	double frmag = 0.0;

	for (auto& sos : SOScascade)
		frmag += sos->freqresp(freq, sampRate);
	return frmag;
}