/*
  ==============================================================================

    BQfilter.h
    Created: 10 Jan 2018 9:40:18am
    Author:  profw

  ==============================================================================
*/

#pragma once

/// <summary>
/// This class enumerates the filter types
/// </summary>
enum class FilterType { 
	BASS, ///< bass shelf filter
	TREBLE, ///< treble shelf filter
	PEAK /// <peak filter
};




/// <summary>
/// Biquadratic filter (aka SOS)
/// 
/// The class implements a biquadratic digital filter, sometimes known as a second order section (SOS).
/// </summary>
class BQfilter
{
public:
	BQfilter();
	~BQfilter() {}

	/// <summary>
	/// Update filter coefficients
	/// 
	/// This method sets the filter coefficients based on parameters provided by the caller
	/// </summary>
	/// <param name="ftype">filter type (BASS, TREBLE, PEAK)</param>
	/// <param name="Gain">gain (dB)</param>
	/// <param name="f0">center or cutoff frequency (Hz)</param>
	/// <param name="Q">Q factor (no units)</param>
	/// <param name="fs">sampling frequency (Hz)</param>
	void update(FilterType ftype, double Gain, double f0, double Q, double fs);

	/// <summary>
	/// Step the filter through one sample period
	/// </summary>
	/// <param name="sample">input sample</param>
	/// <returns>output sample</returns>
	double step(double sample);

	/// <summary>
	/// Reset the filter state variables
	/// </summary>
	/// <param name=""></param>
	void resetstate();

	/// <summary>
	/// Magnitude of frequency response at a given frequency.
	/// </summary>
	/// <param name="freq">frequency (Hz)</param>
	/// <param name="fs">sampling frequency (Hz)</param>
	/// <returns>magnitude of frequency response (dB)</returns>
	double freqresp(double freq, float fs);

private:
	// state variables
	double s1;
	double s2;
	// coefficients
	double b[3];
	double a[3];
};