/*
  ==============================================================================

    BQfilter.cpp
    Created: 10 Jan 2018 9:40:18am
    Author:  profw

  ==============================================================================
*/

#include <cmath>
#include "BQfilter.h"

BQfilter::BQfilter()
{
	// initialize state and coefficients
	s1 = 0.0;
	s2 = 0.0;
	for (int n = 0; n < 3; n++)
	{
		a[n] = 0.0;
		b[n] = 0.0;
	}
}

void BQfilter::update(FilterType ftype, double Gain, double f0, double Q, double fs)
{
	const double PI = 3.141592653589793238463;
	double gain = pow(10.0, abs(Gain) / 20.0);
	double w0 = 2.0 * PI * f0;

	// compute analog filter coefficients
	double B[3]{};
	double A[3]{};
	A[0] = 1.0;
	switch (ftype)
	{
	case FilterType::BASS:
		B[0] = 1.0;
		B[1] = 2.0 * sqrt(gain) * w0 / Q;
		B[2] = gain * pow(w0, 2.0);
		A[1] = 2.0 * w0 / Q;
		A[2] = pow(w0, 2.0);
		break;
	case FilterType::TREBLE:
		B[0] = gain;
		B[1] = 2.0 * gain * w0 / Q;
		B[2] = gain * pow(w0, 2.0);
		A[1] = 2.0 * sqrt(gain) * w0 / Q;
		A[2] = gain * pow(w0, 2.0);
		break;
	case FilterType::PEAK:
		B[0] = 1.0f;
		B[1] = gain * w0 / Q;
		B[2] = pow(w0, 2);
		A[1] = w0 / Q;
		A[2] = pow(w0, 2.0);
		break;
	default:
		break;
	}
	if (Gain < 0.0) // invert analog transfer function
	{
		double x;
		for (int n = 0; n < 3; n++)
		{
			x = B[n];
			B[n] = A[n];
			A[n] = x;
		}
	}

	// Use bilinear transform to compute digital filter coefficients
	double K = w0 / tan(PI * f0 / fs);
	double D = A[0] * pow(K, 2.0) + A[1] * K + A[2];

	a[0] = 1.0;
	a[1] = 2.0 * (A[2] - A[0] * pow(K, 2.0)) / D;
	a[2] = (A[0] * pow(K, 2.0) - A[1] * K + A[2]) / D;
	b[0] = (B[0] * pow(K, 2.0) + B[1] * K + B[2]) / D;
	b[1] = 2.0f * (B[2] - B[0] * pow(K, 2.0)) / D;
	b[2] = (B[0] * pow(K, 2.0) - B[1] * K + B[2]) / D;
}

double BQfilter::step(double sample)
{
	double y = b[0] * sample + s1;
	s1 = s2 + b[1] * sample - a[1] * y;
	s2 = b[2] * sample - a[2] * y;
	return y;
}

void BQfilter::resetstate(void)
{
	s1 = 0.0;
	s2 = 0.0;
}

double BQfilter::freqresp(double freq, float fs)
{
	// Compute magnitude of frequency response at frequency theta (radians/sample)
	// Double precision apparently necessary
	const double PI = 3.141592653589793238463;
	double B0 = pow(b[0], 2.0) + pow(b[1], 2.0) + pow(b[2], 2.0);
	double B1 = 2.0 * (b[0] * b[1] + b[1] * b[2]);
	double B2 = 2.0 * b[0] * b[2];
	double A0 = 1.0 + pow(a[1], 2.0) + pow(a[2], 2.0);
	double A1 = 2.0 * (a[1] + a[1] * a[2]);
	double A2 = 2.0 * a[2];
	double theta = 2.0 * PI * freq / fs;

	double h = abs((B0 + B1 * cos(theta) + B2 * cos(2.0 * theta)) / (A0 + A1 * cos(theta) + A2 * cos(2.0 * theta)));
	return (isnan(h) ? 0.0 : 10.0 * log10(h)); // in case of 0.0 / 0.0
}