#include "stdafx.h"
#include "NRGComputationThread.h"


NRGComputationThread::NRGComputationThread(NRG::NRGAlgorithm *algo)
	: nrg(algo)
{
}


NRGComputationThread::~NRGComputationThread()
{
	delete nrg;
}

void NRGComputationThread::Calculate()
{
	if (nrg) nrg->Calculate();
}
