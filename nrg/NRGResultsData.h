#pragma once

#include "NRGAlgorithm.h"
#include "Chart.h"

#include <list>
#include <vector>
#include <utility>
#include <mutex>
#include <atomic>

class NRGResultsData :
	public NRG::ResultsRetrieverInterface
{
private:
	std::mutex section;

	std::atomic_bool finished{ true };

	std::list<std::pair<double, std::vector<double>>> data;

public:
	Chart* spectralChart = nullptr;

	void PassEigenvalues(int iter, const Eigen::VectorXd& evals, double scale) override;	
	void Finished(NRG::NRGAlgorithm *nrg) override;

	bool TransferToChart(Chart& chart);

	bool isFinished();
	void resetFinished();
};

