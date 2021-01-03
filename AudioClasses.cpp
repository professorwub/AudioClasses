// AudioClasses.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <cmath>

#include "BQfilter.h"
#include "DelayLine.h"
#include "LPAPfilter.h"
#include "LPcombfilter.h"
#include "SOSfilter.h"
#include "SSfilter.h"

int main()
{
	std::ofstream impresp;
	double roomLength = 10.0;
	double samplingRate = 44100.0;
	double absCoef = 0.2;
	double speedSound = 343.0;
	double timeIR = 1.0;

	impresp.open("impresp.txt", std::ofstream::out);
	double damping = (1.0 - pow(10, -absCoef * roomLength / 20.0))/ (1.0 + pow(10, -absCoef * roomLength / 20.0));
	unsigned int sampDelay = round(roomLength * samplingRate / speedSound);
	unsigned int numSamp = round(samplingRate * timeIR);

	SOSfilter filt;
	filt.initsos(2, samplingRate);
	filt.updateSection(0, FilterType::PEAK, 1.0, 100.0, 2.0);
	filt.updateSection(1, FilterType::PEAK, -1.0, 1000.0, 2.0);
	impresp << filt.step(1.0) << std::endl;
	for (auto n = 1; n < numSamp; n++)
		impresp << filt.step(0.0) << std::endl;

}