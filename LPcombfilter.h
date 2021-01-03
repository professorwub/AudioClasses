/*
  ==============================================================================

    LPcombfilter.h
    Created: 28 Jul 2020 10:36:40am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include <vector>

/// <summary>
/// This class implements a low pass feedback comb filter
/// 
/// Filter transfer function: h(z)=\frac{1-dz^{-1}}{1-dz^{-1}-R(1 - d)z^{-N}}
/// </summary>
class LPcombfilter
{
public:
    LPcombfilter();
    ~LPcombfilter() {}

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
    /// Step filter through one sample period
    /// </summary>
    /// <param name="sample">audio sample</param>
    /// <returns>output sample</returns>
    double step(double sample);

    /// <summary>
    /// Clear buffer and reset state variable
    /// </summary>
    void reset();

private:
    double damping;
    double reflection;
    std::vector<float> buffer;
    unsigned int oldest;
    double state;
};