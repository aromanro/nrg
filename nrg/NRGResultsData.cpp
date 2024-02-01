#include "stdafx.h"
#include "NRGResultsData.h"

#include "SpectralOperator.h"


void NRGResultsData::PassEigenvalues(int iter, const Eigen::VectorXd& evals, double scale)
{
	if (iter % 2 == 0) return;

	std::lock_guard<std::mutex> lock(section);

	data.emplace_back(std::make_pair(iter, std::vector<double>()));
	data.back().second.reserve(evals.size());

	const double base = evals(0);

	for (int i = 0; i < evals.size(); ++i)
	{
		const double level = evals(i);
		data.back().second.emplace_back(scale * (level - base));
	}
}

bool NRGResultsData::TransferToChart(Chart& chart)
{
	std::lock_guard<std::mutex> lock(section);

	if (data.size() == 0) return false;

	for (const auto &obj : data)
		chart.AddDataSlice(obj.first, obj.second.data(), static_cast<unsigned int>(obj.second.size()));

	data.clear();

	return true;
}

bool NRGResultsData::isFinished()
{
	return finished;
}

void NRGResultsData::resetFinished()
{
	data.clear();

	finished = false;
}

void NRGResultsData::Finished(NRG::NRGAlgorithm *nrg)
{
	if (nrg)
	{
		std::vector<std::pair<double, double>> spectrum;

		if (nrg->spectralOperators.size())
			// from the first one
			spectrum = nrg->spectralOperators.front()->GetSpectrum();

		std::lock_guard<std::mutex> lock(section);

		if (spectralChart) spectralChart->AddDataSet(&spectrum);
	}

	finished = true;
}


