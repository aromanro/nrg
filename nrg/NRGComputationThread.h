#pragma once
#include "ComputationThread.h"

#include "NRGAlgorithm.h"

class NRGComputationThread :
	public ComputationThread
{
protected:
	NRG::NRGAlgorithm* nrg;

	void Calculate() override;
public:
	NRGComputationThread(NRG::NRGAlgorithm *algo = nullptr);
	~NRGComputationThread() override;
};

