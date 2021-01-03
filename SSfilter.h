#pragma once

#include <cmath>

const double PI = 3.141592653589793238463;

/// <summary>
/// A simple state space filter
/// 
/// This class implements a simple state space filter that produces
/// a highpass, lowpass, and bandpass output
/// </summary>
class SSfilter
{
public:
	SSfilter();
	~SSfilter() {}

	/// <summary>
	/// Process one sample through filter
	/// </summary>
	/// <param name="insample">input sample</param>
	void step(double insample);

	/// <summary>
	/// Process one sample through filter with variable center frequency
	/// </summary>
	/// <param name="insample">input sample</param>
	/// <param name="Omegac">center frequency (radians/sample)</param>
	void step(double insample, double Omegac);

	/// <summary>
	/// Get high-pass output sample
	/// </summary>
	/// <returns>high-pass output sample</returns>
	double gethp() { return hpout; }

	/// <summary>
	/// Get band-pass output sample
	/// </summary>
	/// <returns>band-pass output sample</returns>
	double getbp() { return bpout; }

	/// <summary>
	/// Get low-pass output sample
	/// </summary>
	/// <returns>low-pass output sample</returns>
	double getlp() { return lpout; }

	/// <summary>
	/// Set damping parameter
	/// </summary>
	/// <param name="dmp">damping parameter (no units)</param>
	void setdamping(double dmp) { damping = dmp; }

	/// <summary>
	/// Set F1 parameter
	/// </summary>
	/// <param name="fc">center or cutoff frequency (Hz)</param>
	/// <param name="fs">sampling frequency (Hz)</param>
	void setF1(double fc, double fs) { F1 = 2.0 * sin(PI * fc / fs); }

	/// <summary>
	/// Set F1 parameter
	/// </summary>
	/// <param name="Omegac">center frequency (radians/sample)</param>
	void setF1(double Omegac) { F1 = 2.0 * sin(Omegac / 2.0); }

	/// <summary>
	/// Reset state variables
	/// </summary>
	void reset();

private:
	double hpout;
	double bpout;
	double lpout;
	double damping;
	double F1;
};

