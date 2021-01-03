/*
  ==============================================================================

    SOSfilter.h
    Created: 26 Apr 2018 8:29:28am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include "BQfilter.h"
#include <vector>
#include <memory>

/// <summary>
/// Cascade of second order sections.
/// 
/// This class implements a cascade of biquadratic filters (aka second order sections). It
/// can be used as an equalizer.
/// </summary>
class SOSfilter
{
public:
	SOSfilter();
	~SOSfilter() {}

	/// <summary>
	/// Initialize the SOS cascade
	/// </summary>
	/// <param name="numsects">number of SOS in cascade</param>
	/// <param name="fs">sampling frequency</param>
	void initsos(int numsects, double fs);

	/// <summary>
	/// Update settings of SOS filter section
	/// </summary>
	/// <param name="sect">index of filter in cascade</param>
	/// <param name="ftype">filter type (BASS, TREBLE, PEAK)</param>
	/// <param name="Gain">gain (dB)</param>
	/// <param name="f0">center or cut-off frequency (Hz)</param>
	/// <param name="Q">Q factor (no units)</param>
	void updateSection(int sect, FilterType ftype, double Gain, double f0, double Q);

	/// <summary>
	/// Step SOS filter through one sample period
	/// </summary>
	/// <param name="sample">input sample</param>
	/// <returns>output sample</returns>
	double step(double sample);

	/// <summary>
	/// Calculate magnitude of frequency response
	/// </summary>
	/// <param name="freq">frequency (Hz)</param>
	/// <returns>magnitude of frequency response (dB)</returns>
	double freqResponse(double freq);

private:
	double sampRate;
	std::vector<std::unique_ptr<BQfilter>> SOScascade;
};