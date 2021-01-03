/*
  ==============================================================================

	Multiport.cpp
	Created: 10 Dec 2020 5:38:16am
	Author:  profw

  ==============================================================================
*/
#include "MultiPort.h"

Reflector::Reflector()
{
	insamples.resize(2);
	outsamples.resize(2);
	for (auto& sample : outsamples)
		sample = std::make_shared<double>(0.0);
	Gamma = 0.0;
}

void Reflector::step()
{
	*outsamples[0] = Gamma * *insamples[0] + (1.0 - Gamma) * *insamples[1];
	*outsamples[1] = (1.0 + Gamma) * *insamples[0] - Gamma * *insamples[1];
}

Waveguide::Waveguide()
{
	insamples.resize(2);
	outsamples.resize(2);
	for (auto& sample : outsamples)
		sample = std::make_shared<double>(0.0);
	damping = 0.0;
	oldest = 0;
}

void Waveguide::step()
{
	*outsamples[0] = damping * *outsamples[0] + (1.0 - damping) * westbuffer[oldest];
	*outsamples[1] = damping * *outsamples[1] + (1.0 - damping) * eastbuffer[oldest];
	eastbuffer[oldest] = *insamples[0];
	westbuffer[oldest] = *insamples[1];
	oldest = ++oldest % eastbuffer.size();
}

void Waveguide::setDelay(unsigned int D)
{
	eastbuffer.resize(D, 0.0);
	westbuffer.resize(D, 0.0);
	if (oldest >= eastbuffer.size())
		oldest = 0;
}

void Junction::step()
{
	auto sum = 0.0;
	for (auto port = 0; port < insamples.size(); port++)
	{
		sum += *insamples[port];
		*outsamples[port] = -*insamples[port];
	}
	for (auto port = 0; port < insamples.size(); port++)
		*outsamples[port] += 2.0 * sum / double(insamples.size());
}

void Junction::setNumPorts(unsigned int numports)
{
	insamples.clear();
	outsamples.clear();
	insamples.resize(numports);
	outsamples.resize(numports);
	for (auto& sample : outsamples)
		sample = std::make_shared<double>(0.0);
}

MPnetwork::MPnetwork()
{
	numjunct = 0;
	numwg = 0;
}

void MPnetwork::addJunctions(unsigned int numjunctions)
{
	numjunct += numjunctions;
	junction.resize(numjunct);
}

void MPnetwork::addWaveguides(unsigned int numwaveguides)
{
	numwg += numwaveguides;
	waveguide.resize(numwg);
}

void MPnetwork::setWGparams(unsigned int wgno, unsigned int delay, double damping)
{
	waveguide[wgno].setDelay(delay);
	waveguide[wgno].setDamping(damping);
}

void MPnetwork::connect(unsigned int wgno, unsigned int junct1, unsigned int port1, unsigned int junct2, unsigned int port2)
{
	junction[junct1].setInputPtr(port1, waveguide[wgno].getOutputPtr(0));
	junction[junct2].setInputPtr(port2, waveguide[wgno].getOutputPtr(1));
	waveguide[wgno].setInputPtr(0, junction[junct1].getOutputPtr(port1));
	waveguide[wgno].setInputPtr(1, junction[junct2].getOutputPtr(port2));
}

void MPnetwork::netstep()
{
	for (auto& junct : junction)
		junct.step();
	for (auto& wg : waveguide)
		wg.step();
}