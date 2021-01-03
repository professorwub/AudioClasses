/*
  ==============================================================================

	Multiport.h
	Created: 10 Dec 2020 5:38:16am
	Author:  profw

  ==============================================================================
*/

#pragma once

#include<vector>
#include<memory>

/// <summary>
/// Base class for multiport network elements
/// 
/// Multiport elements may be interconnected to create audio signal processing networks.
/// </summary>
class MultiPort
{
public:
	MultiPort() {}
	~MultiPort() {}

	/// <summary>
	/// Get a link to a given output port.
	/// 
	/// This method provides a shared pointer to an output of the element. The pointer can 
	/// be used as an input to another element or can be de-referenced to provide the output value.
	/// </summary>
	/// <param name="port">port number</param>
	/// <returns>pointer to port output</returns>
	std::shared_ptr<double> getOutputPtr(unsigned int port) { return(outsamples[port]); }

	/// <summary>
	/// Link input port to a source
	/// 
	/// This method links an input to the element to an input source. The arguments include a
	/// shared pointer to the source.
	/// </summary>
	/// <param name="port">port number</param>
	/// <param name="source">pointer to source</param>
	void setInputPtr(unsigned int port, std::shared_ptr<double> source) { insamples[port] = source; }

	/// <summary>
	/// Connect port input to ground (zero)
	/// 
	/// This method sets an input to the element to zero, using a shared pointer to a fixed value of 0.0.
	/// </summary>
	/// <param name="port">port number</param>
	void setGround(unsigned int port) { insamples[port] = std::make_shared<double>(0.0); }

	/// <summary>
	/// Compute output from current input
	/// 
	/// This is a virtual method that must be implemented for each derived class.
	/// </summary>
	virtual void step() {}

protected:
	std::vector<std::shared_ptr<double>> insamples, outsamples;
};

/// <summary>
/// Two port element representing waveguide termination or discontinuity
/// 
/// The inputs to this element are partially transmitted and partially reflected.
/// This element is characterized by a single parameter: the reflection coefficient.
/// </summary>
class Reflector : public MultiPort
{
public:
	Reflector();
	~Reflector() {}

	/// <summary>
	/// Compute output from current input
	/// 
	/// This method steps the reflector through one sample cycle and updates the port outputs
	/// </summary>
	void step() override;

	/// <summary>
	/// Set reflection coefficient
	/// </summary>
	/// <param name="G">reflection coefficient</param>
	void setReflection(double G) { Gamma = G; }

private:
	double Gamma;
};


/// <summary>
/// Two port network element representing propagation path
/// 
/// The element consists of two delay lines, denoted east and west, with frequency-dependent loss.
/// </summary>
class Waveguide : public MultiPort
{
public:
	Waveguide();
	~Waveguide() {}

	/// <summary>
	/// Compute output from current input
	/// 
	/// This method steps the waveguide through one sample cycle and updates the port outputs
	/// </summary>
	void step() override;

	/// <summary>
	/// Set damping factor
	/// </summary>
	/// <param name="damp">damping factor</param>
	void setDamping(double damp) { damping = damp; }

	/// <summary>
	/// Set sample delay
	/// </summary>
	/// <param name="D">sample delay</param>
	void setDelay(unsigned int D);

private:
	double damping;
	std::vector<double> eastbuffer;
	std::vector<double> westbuffer;
	unsigned int oldest;
};

/// <summary>
/// Multi-port network element representing simple waveguide junction
/// 
/// This element is normally used as a junction among three or more waveguide elements. It
/// can also be used to connect two waveguide elements, but this is simply equivalent to
/// connecting the waveguides directly.
/// </summary>
class Junction : public MultiPort
{
public:
	Junction() {};
	~Junction() {}

	/// <summary>
	/// Compute output from current input
	/// 
	/// This method steps the junction through one sample cycle and updates the port outputs
	/// </summary>
	void step() override;

	/// <summary>
	/// Set number of ports
	/// </summary>
	/// <param name="numports">number of ports</param>
	void setNumPorts(unsigned int numports);
};


/// <summary>
/// Multiport element network consisting of interconnected waveguides and junctions
/// 
/// This class facilitates creation of networks of multiport elements. It currently supports
/// networks of waveguides and junctions.
/// </summary>
class MPnetwork
{
public:
	MPnetwork();
	~MPnetwork() {}

	/// <summary>
	/// Add junctions to the network
	/// </summary>
	/// <param name="numjunctions">number of junctions to add</param>
	void addJunctions(unsigned int numjunctions);

	/// <summary>
	/// Add waveguides to the network
	/// </summary>
	/// <param name="numwaveguides">number of waveguides to add</param>
	void addWaveguides(unsigned int numwaveguides);

	/// <summary>
	/// Set port count for a junction
	/// </summary>
	/// <param name="junctno">junction number</param>
	/// <param name="numports">number of ports</param>
	void setNumPorts(unsigned int junctno, unsigned int numports) { junction[junctno].setNumPorts(numports); }

	/// <summary>
	/// Set waveguide sample delay and damping factor
	/// </summary>
	/// <param name="wgno">waveguide number</param>
	/// <param name="delay">sample delay</param>
	/// <param name="damping">damping</param>
	void setWGparams(unsigned int wgno, unsigned int delay, double damping);

	/// <summary>
	/// Connect two junctions with a waveguide
	/// 
	/// Once junctions and waveguides have been added to the network, use this method
	/// to establish the connections. Make sure all port inputs are either connected or 
	/// grounded.
	/// </summary>
	/// <param name="wgno">waveguide number</param>
	/// <param name="junct1">east junction number</param>
	/// <param name="port1">east junction port number</param>
	/// <param name="junct2">west junction number</param>
	/// <param name="port2">west junction port number</param>
	void connect(unsigned int wgno, unsigned int junct1, unsigned int port1, unsigned int junct2, unsigned int port2);

	/// <summary>
	/// Set junction input to ground (0.0)
	/// 
	/// Use this method if there is no input for a given port.
	/// </summary>
	/// <param name="junct">junction number</param>
	/// <param name="port">port number</param>
	void addground(unsigned int junct, unsigned int port) { junction[junct].setGround(port); }

	/// <summary>
	/// Set junction input to source
	/// 
	/// Use this method for external inputs to the network.
	/// </summary>
	/// <param name="junct">junction number</param>
	/// <param name="port">port number</param>
	/// <param name="src">source</param>
	void addsource(unsigned int junct, unsigned int port, std::shared_ptr<double> src) { junction[junct].setInputPtr(port, src); }

	/// <summary>
	/// Get output from given junction and port
	/// </summary>
	/// <param name="junct">junction number</param>
	/// <param name="port">port number</param>
	/// <returns>output value</returns>
	double getoutput(unsigned int junct, unsigned int port) { return(*junction[junct].getOutputPtr(port)); }

	/// <summary>
	/// Step every element in the network
	/// 
	/// This method steps the network through one sample cycle and updates all port outputs
	/// </summary>
	void netstep();

private:
	std::vector<Junction> junction;
	std::vector<Waveguide> waveguide;
	unsigned int numjunct;
	unsigned int numwg;
};