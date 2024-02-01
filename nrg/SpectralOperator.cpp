#include "stdafx.h"
#include "SpectralOperator.h"



#include <list>

#include "nrg.h"
#include "Options.h"



namespace NRG {


	SpectralOperator::SpectralOperator(int size, bool extendChangeSign)
		: Operator(size, extendChangeSign)
	{
		b = theApp.options.b;
		step = theApp.options.step;

		b2 = b*b;
	}

	double SpectralOperator::LogGauss(double omega, double omegaN) const
	{
		ASSERT((omega < 0 && omegaN < 0) || (omega > 0 && omegaN > 0));
		
		const double lndif = log(abs(omega)) - log(abs(omegaN));

		return exp(-b2 * 0.25) / (b * abs(omega) * sqrt(M_PI)) * exp(-lndif * lndif / b2);
	}


	double SpectralOperator::Broaden(double omega, const std::vector<std::pair<double, double>>& spectrum) const
	{
		double result = 0;

		for (const auto &val : spectrum)
			result += val.second * LogGauss(omega, val.first);

		return result;
	}

	void SpectralOperator::AdjustSpectrum(double min_energy_before, double max_energy_current, double EnergyScale, std::vector<std::pair<double, double>>& spectrum) const
	{
		const double interval = max_energy_current - min_energy_before;

		ASSERT(interval >= 0);

		//walk over old spectrums and adjust
		for (auto it = spectrum.rbegin(); it != spectrum.rend(); ++it)
		{
			const double omega = abs(it->first);
			if (omega >= max_energy_current) break;

			const double weight = ((abs(interval) < 1E-5 * EnergyScale) ? 0.5 : (omega - min_energy_before) / interval);

			it->second *= weight;
		}
	}

	void SpectralOperator::PassSpectral(int iter, double EnergyScale, const Eigen::VectorXd& evals)
	{
		if (evals.size() <= 1) return;
		else if (iter % 2 == 1) return;

		const unsigned int nrvals = static_cast<unsigned int>(matrix.cols());

		const double ground = EnergyScale * evals(0);

		double max_energy_current = EnergyScale * evals(nrvals - 1) - ground;

		ASSERT(max_energy_current >= 0);

		double min_energy_before = DBL_MAX;
		if (positive_spectrum.size()) min_energy_before = positive_spectrum.back().first;

		const double interval = max_energy_current - min_energy_before;

		// if passed spectrum overlaps with the existing one
		if (min_energy_before <= max_energy_current)
		{
			//walk over old spectrums and adjust
			future1 = std::async(std::launch::async, [&] { AdjustSpectrum(min_energy_before, max_energy_current, EnergyScale, positive_spectrum); });
			AdjustSpectrum(min_energy_before, max_energy_current, EnergyScale, negative_spectrum);;
			future1.get();
		}

		double weight = 1;

		// also adjust the new passed spectrum
		for (unsigned int i = 0; i < nrvals; ++i)
		{
			const double omega = EnergyScale * evals(i) - ground;

			//if (omega < 2. * EnergyScale) continue;

			if (omega <= min_energy_before) weight = 1.;
			else if (abs(interval) < 1E-5 * EnergyScale) weight = 0.5;
			else weight = (max_energy_current - omega) / interval;

			double val = matrix(i, 0);
			positive_spectrum.emplace_back(std::make_pair(omega, weight * val * val));

			val = matrix(0, i);
			negative_spectrum.emplace_back(std::make_pair(-omega, weight * val * val));
		}

		future1 = std::async(std::launch::async, [this] { std::sort(negative_spectrum.begin(), negative_spectrum.end(), [](const auto& val1, const auto& val2) -> bool { return val1.first < val2.first; }); });
		std::sort(positive_spectrum.begin(), positive_spectrum.end(), [](const auto& val1, const auto& val2) -> bool { return val1.first > val2.first; });
		future1.get();
	}

	std::vector<std::pair<double, double>> SpectralOperator::GetSpectrum() const
	{
		std::vector<std::pair<double, double>> spectrum;

		double negative_limit = -10;
		if (negative_spectrum.size()) negative_limit = round(negative_spectrum.front().first) - 1 - step / 2;

		double positive_limit = 10;
		if (positive_spectrum.size()) positive_limit = round(positive_spectrum.front().first) + 1 + step / 2;

		const unsigned int nrnegative = static_cast<unsigned int>(ceill(abs(negative_limit) / step));
		const unsigned int nrpositive = static_cast<unsigned int>(ceill(positive_limit / step));

		spectrum.reserve(static_cast<size_t>(nrnegative) + nrpositive);

		//double integral = 0;

		// spectral lines broadening

		for (unsigned int pos = nrnegative; pos > 0; --pos)
		{
			const double omega = -step * pos;
			const double value = Broaden(omega, negative_spectrum);
			spectrum.emplace_back(std::make_pair(omega, value));

			//integral += step * value;			
		}

		for (unsigned int pos = 1; pos <= nrpositive; ++pos)
		{
			const double omega = step * pos;
			const double value = Broaden(omega, positive_spectrum);
			spectrum.emplace_back(std::make_pair(omega, value));

			//integral += step * value;			
		}

		/*
		CString str;
		str.Format(L"Spectral sum: %f", integral);
		AfxMessageBox(str);
		*/

		return spectrum;
	}

}