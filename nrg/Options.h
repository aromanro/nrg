#pragma once
class Options
{
public:
	// NRG options
	unsigned int iterationsNumber;
	unsigned int keptStates;
	double Lambda;
	int model;

	// Parameters
	// for models
	double U;
	double eps;
	double J;
	double B;
	double delta;
	// for spectral function
	double b;
	double step;

	// for charts

	// group flow chart
	double YMaxFlow;
	unsigned int YBigTicksFlow;
	unsigned int YSmallTicksFlow;

	// for spectral function chart
	double XMinSpectral;
	double XMaxSpectral;
	unsigned int XBigTicksSpectral;
	unsigned int XSmallTicksSpectral;

	double YMaxSpectral;
	unsigned int YBigTicksSpectral;
	unsigned int YSmallTicksSpectral;

	Options();
	~Options();
	void Load();
	void Save();
protected:
	static double GetDouble(LPCTSTR param, double defval);
};

