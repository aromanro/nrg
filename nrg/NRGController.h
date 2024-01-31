#pragma once
#include "NRGAlgorithm.h"

#include <atomic>

class NRGController :
	public NRG::ControllerInterface
{
public:
	NRGController();
	~NRGController() override;

	std::atomic_bool endIt;

	bool ShouldCancel() override;
};

