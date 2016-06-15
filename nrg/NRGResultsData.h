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
protected:
	std::mutex section;

	std::atomic_bool finished;

	std::list<std::pair<double, std::vector<double>>> data;

public:
	Chart* spectralChart;

	NRGResultsData();
	virtual ~NRGResultsData();

	virtual void PassEigenvalues(int iter, const Eigen::VectorXd& evals, double scale);	
	virtual void Finished(NRG::NRGAlgorithm *nrg);

	bool TransferToChart(Chart& chart);

	bool isFinished();
	void resetFinished();
};

