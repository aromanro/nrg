#pragma once
#include "Operator.h"

#include <thread>
#include <future>

#include <vector>


namespace NRG {

	class SpectralOperator :
		public Operator
	{
	protected:
		std::vector<std::pair<double, double>> negative_spectrum;
		std::vector<std::pair<double, double>> positive_spectrum;

		std::future<void> future1, future2;

		double b;
		double step;

		double b2;

		void AdjustSpectrum(double min_energy_before, double max_energy_current, double EnergyScale, std::vector<std::pair<double, double>>& spectrum) const;
		double LogGauss(double omega, double omegaN) const;
		double Broaden(double omega, const std::vector<std::pair<double, double>>& spectrum) const;
	public:
		SpectralOperator(int size = 4, bool extendChangeSign = true);
		virtual ~SpectralOperator();

		void PassSpectral(int iter, double EnergyScale, const Eigen::VectorXd& evals);

		std::vector<std::pair<double, double>> SpectralOperator::GetSpectrum() const;
	};

}
