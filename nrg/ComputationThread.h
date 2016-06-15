#pragma once

#include <thread>

class ComputationThread
{
public:
	ComputationThread();

	std::thread Start();
protected:
	virtual ~ComputationThread();

	virtual void Calculate() = 0;
};

