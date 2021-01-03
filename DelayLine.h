/*
  ==============================================================================

    DelayLine.h
    Created: 17 Nov 2019 10:28:01am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include <vector>

typedef std::vector<float>::size_type vector_size;

/// <summary>
/// This class models a propagation delay with frequency-dependent attenuation
/// </summary>
class DelayLine
{
public:
	DelayLine();
	~DelayLine() {}

	/// <summary>
	/// Set sample delay
	/// </summary>
	/// <param name="sampledelay">delay in samples</param>
	void setsampledelay(int sampledelay);

	/// <summary>
	/// Set damping parameter
	/// </summary>
	/// <param name="damp">damping parameter (no units)</param>
	void setdamping(double damp) { damping = damp; }

	/// <summary>
	/// Get sample dealy
	/// </summary>
	/// <returns>delay in samples</returns>
	unsigned int getdelay() { return buffer.size(); }

	/// <summary>
	/// Step delay line through one sample period
	/// </summary>
	/// <param name="sample">input sample</param>
	/// <returns>output sample</returns>
	double step(double sample);

	/// <summary>
	/// Reset the delay line
	/// 
	/// Clears the buffer and the output, but maintains the sample delay and damping.
	/// </summary>
	void reset();

private:
	std::vector<double> buffer;
	unsigned int head;
	double outsample;
	double damping;
};