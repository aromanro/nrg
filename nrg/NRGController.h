#pragma once
#include "NRGAlgorithm.h"

#include <atomic>

class NRGController :
	public NRG::ControllerInterface
{
public:
	std::atomic_bool endIt{ false };

	bool ShouldCancel() override;
};

