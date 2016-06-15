#pragma once
#include "ComputationThread.h"

#include "NRGAlgorithm.h"

class NRGComputationThread :
	public ComputationThread
{
protected:
	NRG::NRGAlgorithm* nrg;

	virtual void Calculate();
public:
	NRGComputationThread(NRG::NRGAlgorithm *algo = NULL);
	~NRGComputationThread();
};

