#pragma once
#include "NRGAlgorithm.h"

#include <atomic>

class NRGController :
	public NRG::ControllerInterface
{
public:
	NRGController();
	virtual ~NRGController();

	std::atomic_bool endIt;

	virtual bool ShouldCancel();
};

