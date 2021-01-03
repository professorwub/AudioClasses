/*
  ==============================================================================

    LPAPfilter.h
    Created: 5 Aug 2020 9:23:43am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include <vector>
/// <summary>
/// This class implements a low pass all pass filter
/// 
/// Filter transfer function: h(z) = (-R + Rdz^{-1} + (1 - d)z^{-N})/(1 - dz^{-1} - R(1 - d)z^{-N})
/// </summary>
class LPAPfilter
{
public:
    LPAPfilter();
    ~LPAPfilter() {}

    /// <summary>
    /// Set damping parameter
    /// </summary>
    /// <param name="damp">damping parameter (no units)</param>
    void setdamping(double damp) { damping = damp; }

    /// <summary>
    /// Set reflection parameter
    /// </summary>
    /// <param name="R">reflection parameter (no units)</param>
    void setreflection(double R) { reflection = R; }

    /// <summary>
    /// Set sample delay
    /// </summary>
    /// <param name="delay">delay in samples</param>
    void setdelay(int delay);

    /// <summary>
    /// Clear buffer and reset state variables
    /// </summary>
    void reset();

    /// <summary>
    /// Step filter through one sample period
    /// </summary>
    /// <param name="sample">input sample</param>
    /// <returns>output sample</returns>
    double step(double sample);

private:
    double damping;
    double reflection;
    std::vector<float> xbuffer;
    std::vector<float> ybuffer;
    unsigned int oldest;
    float xprev;
    float yprev;
};