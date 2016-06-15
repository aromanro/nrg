#include "stdafx.h"
#include "Options.h"

#include "nrg.h"

Options::Options()
	: 
	// NRG options
	iterationsNumber(50),
	keptStates(100),
	Lambda(2.),
	model(0),

	// Parameters
	// for models
	U(1),
	eps(-0.5),
	J(0.25),
	B(0),
	delta(0.0625),
	// for spectral function
	b(0.6),
	step(0.005),
	// for charts
	// group flow chart
	YMaxFlow(4.),
	YBigTicksFlow(4),
	YSmallTicksFlow(4),
	// for spectral function chart
	XMinSpectral(-3.),
	XMaxSpectral(3.),
	XBigTicksSpectral(6),
	XSmallTicksSpectral(2),
	YMaxSpectral(1.),
	YBigTicksSpectral(2),
	YSmallTicksSpectral(2)
{
}


Options::~Options()
{
}


void Options::Load()
{
	iterationsNumber = theApp.GetProfileInt(L"options", L"iterationsNumber", 50);
	keptStates = theApp.GetProfileInt(L"options", L"keptStates", 100);
	model = theApp.GetProfileInt(L"options", L"model", 0);
	Lambda = GetDouble(L"Lambda", 2.);
	// Parameters
	// for models
	U = GetDouble(L"U", 1.);
	eps = GetDouble(L"eps", -0.5);
	J = GetDouble(L"J", 0.25);
	B = GetDouble(L"B", 0.);
	delta = GetDouble(L"delta", 0.0625);
	// for spectral function
	b = GetDouble(L"bspectral", 0.6);
	step = GetDouble(L"step", 0.005);

	// for charts
	// group flow chart
	YMaxFlow = GetDouble(L"YMaxFlow", 4.);
	YBigTicksFlow = theApp.GetProfileInt(L"options", L"YBigTicksFlow", 4);
	YSmallTicksFlow = theApp.GetProfileInt(L"options", L"YSmallTicksFlow", 4);

	// for spectral function chart
	XMinSpectral = GetDouble(L"XMinSpectral", -3.);
	XMaxSpectral = GetDouble(L"XMaxSpectral", 3.);

	YMaxSpectral = GetDouble(L"YMaxSpectral", 1.);

	XBigTicksSpectral = theApp.GetProfileInt(L"options", L"XBigTicksSpectral", 6);
	XSmallTicksSpectral = theApp.GetProfileInt(L"options", L"XSmallTicksSpectral", 2);
	YBigTicksSpectral = theApp.GetProfileInt(L"options", L"YBigTicksSpectral", 2);
	YSmallTicksSpectral = theApp.GetProfileInt(L"options", L"YSmallTicksSpectral", 2);
}


void Options::Save()
{
	theApp.WriteProfileInt(L"options", L"iterationsNumber", iterationsNumber);
	theApp.WriteProfileInt(L"options", L"keptStates", keptStates);
	theApp.WriteProfileInt(L"options", L"model", model);
	theApp.WriteProfileBinary(L"options", L"Lambda", (LPBYTE)&Lambda, sizeof(double));
	// Parameters
	// for models	
	theApp.WriteProfileBinary(L"options", L"U", (LPBYTE)&U, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"eps", (LPBYTE)&eps, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"J", (LPBYTE)&J, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"B", (LPBYTE)&B, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"delta", (LPBYTE)&delta, sizeof(double));
	// for spectral function
	theApp.WriteProfileBinary(L"options", L"bspectral", (LPBYTE)&b, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"step", (LPBYTE)&step, sizeof(double));

	// for charts
	// group flow chart
	theApp.WriteProfileBinary(L"options", L"YMaxFlow", (LPBYTE)&YMaxFlow, sizeof(double));
	theApp.WriteProfileInt(L"options", L"YBigTicksFlow", YBigTicksFlow);
	theApp.WriteProfileInt(L"options", L"YSmallTicksFlow", YSmallTicksFlow);

	// for spectral function chart
	theApp.WriteProfileBinary(L"options", L"XMinSpectral", (LPBYTE)&XMinSpectral, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"XMaxSpectral", (LPBYTE)&XMaxSpectral, sizeof(double));

	theApp.WriteProfileBinary(L"options", L"YMaxSpectral", (LPBYTE)&YMaxSpectral, sizeof(double));

	theApp.WriteProfileInt(L"options", L"XBigTicksSpectral", XBigTicksSpectral);
	theApp.WriteProfileInt(L"options", L"XSmallTicksSpectral", XSmallTicksSpectral);
	theApp.WriteProfileInt(L"options", L"YBigTicksSpectral", YBigTicksSpectral);
	theApp.WriteProfileInt(L"options", L"YSmallTicksSpectral", YSmallTicksSpectral);
}


double Options::GetDouble(LPCTSTR param, double defval)
{
	double val = defval;

	UINT sz = 0;
	LPBYTE buf = NULL;

	if (theApp.GetProfileBinary(L"options", param, &buf, &sz))
	{
		if (sizeof(double) == sz) val = *((double*)buf);
		delete[] buf;
	}

	return val;
}
