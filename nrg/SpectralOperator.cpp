#include "stdafx.h"
#include "SpectralOperator.h"

#include <thread>
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


	SpectralOperator::~SpectralOperator()
	{
	}

	double SpectralOperator::LogGauss(double omega, double omegaN) const
	{
		ASSERT((omega < 0 && omegaN < 0) || (omega > 0 && omegaN > 0));
		
		double lndif = log(abs(omega)) - log(abs(omegaN));

		return exp(-b2 / 4.) / (b * abs(omega) * sqrt(M_PI)) * exp(-lndif * lndif / b2);
	}


	double SpectralOperator::Broaden(double omega, const std::vector<std::pair<double, double>>& spectrum) const
	{
		double result = 0;

		for (auto &&val : spectrum)
			result += val.second * LogGauss(omega, val.first);

		return result;
	}

	void SpectralOperator::AdjustSpectrum(double min_energy_before, double max_energy_current, double EnergyScale, std::vector<std::pair<double, double>>& spectrum) const
	{
		double interval = max_energy_current - min_energy_before;

		ASSERT(interval >= 0);

		//walk over old spectrums and adjust
		for (auto it = spectrum.rbegin(); it != spectrum.rend(); ++it)
		{
			double omega = abs(it->first);
			if (omega >= max_energy_current) break;

			double weight = ((abs(interval) < 1E-5 * EnergyScale) ? 0.5 : (omega - min_energy_before) / interval);

			it->second *= weight;
		}
	}

	void SpectralOperator::PassSpectral(int iter, double EnergyScale, const Eigen::VectorXd& evals)
	{
		if (evals.size() <= 1) return;
		else if (iter % 2 == 1) return;

		unsigned int nrvals = (unsigned int)matrix.cols();

		double ground = EnergyScale * evals(0);

		double max_energy_current = EnergyScale * evals(nrvals - 1) - ground;

		ASSERT(max_energy_current >= 0);

		double min_energy_before = DBL_MAX;
		if (positive_spectrum.size()) min_energy_before = positive_spectrum.back().first;

		double interval = max_energy_current - min_energy_before;

		std::thread thread1, thread2;

		if (min_energy_before <= max_energy_current)
		{
			//walk over old spectrums and adjust
			
			thread1 = std::thread([&] { AdjustSpectrum(min_energy_before, max_energy_current, EnergyScale, positive_spectrum); });
			thread2 = std::thread([&] { AdjustSpectrum(min_energy_before, max_energy_current, EnergyScale, negative_spectrum); });
			thread1.join();
			thread2.join();			
		}

		double weight;

		for (unsigned int i = 0; i < nrvals; ++i)
		{
			double omega = EnergyScale * evals(i) - ground;

			//if (omega < 2. * EnergyScale) continue;

			if (omega <= min_energy_before) weight = 1.;
			else if (abs(interval) < 1E-5 * EnergyScale) weight = 0.5;
			else weight = (max_energy_current - omega) / interval;

			double val = matrix(i, 0);
			positive_spectrum.push_back(std::make_pair(omega, weight * val * val));

			val = matrix(0, i);
			negative_spectrum.push_back(std::make_pair(-omega, weight * val * val));
		}

		thread1 = std::thread([this] { std::sort(negative_spectrum.begin(), negative_spectrum.end(), [](auto val1, auto val2) -> bool { return val1.first < val2.first; }); });
		thread2 = std::thread([this] { std::sort(positive_spectrum.begin(), positive_spectrum.end(), [](auto val1, auto val2) -> bool { return val1.first > val2.first; }); });
		thread1.join();
		thread2.join();
	}

	std::vector<std::pair<double, double>> SpectralOperator::GetSpectrum() const
	{
		std::vector<std::pair<double, double>> spectrum;

		double negative_limit = -10;
		if (negative_spectrum.size()) negative_limit = round(negative_spectrum.front().first) - 1 - step / 2;

		double positive_limit = 10;
		if (positive_spectrum.size()) positive_limit = round(positive_spectrum.front().first) + 1 + step / 2;

		unsigned int nrnegative = (unsigned int)ceill(abs(negative_limit) / step);
		unsigned int nrpositive = (unsigned int)ceill(positive_limit / step);

		spectrum.reserve(nrnegative + nrpositive);

		//double integral = 0;
		double value, omega;

		for (unsigned int pos = nrnegative; pos > 0; --pos)
		{
			omega = -step * pos;
			value = Broaden(omega, negative_spectrum);
			spectrum.push_back(std::make_pair(omega, value));

			//integral += step * value;			
		}

		for (unsigned int pos = 1; pos <= nrpositive; ++pos)
		{
			omega = step * pos;
			value = Broaden(omega, positive_spectrum);
			spectrum.push_back(std::make_pair(omega, value));

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