#pragma once

#include <vector>

#include "Operator.h"
#include "SpectralOperator.h"
#include "Hamiltonian.h"

namespace NRG {

	class NRGAlgorithm;

	class ResultsRetrieverInterface {
	public:
		virtual ~ResultsRetrieverInterface() {}
		virtual void PassEigenvalues(int iter, const Eigen::VectorXd& evals, double scale) = 0;
		virtual void Finished(NRGAlgorithm* algo) = 0;
	};

	class ControllerInterface {
	public:
		virtual ~ControllerInterface() {}
		virtual bool ShouldCancel() = 0;
	};

	class NRGAlgorithm
	{
	protected:
		double Lambda;
		double SqrtLambda;
		double Rescale;
		
		int NrSteps;
		int startIteration;
	
		double t;

		int curMatrixSize;
		int maxSize;

		Hamiltonian hamiltonian;

		FUpOperator fUpOperator;
		FDownOperator fDownOperator;

		ResultsRetrieverInterface *passData;
		ControllerInterface *controller;

		double GetCouplingForIteration(int iter) const;
		void AdjustForEnergyScale();

		void AddSite();
		void Step(int iter);
	public:
		NRGAlgorithm(int nrsteps = 50, int maxSize = 200, double Lambda = 2., int startHamiltonianSize = 4);
		virtual ~NRGAlgorithm();

		void SetResultsRetriever(ResultsRetrieverInterface *setd) { passData = setd; }
		void SetController(ControllerInterface *ctrl) { controller = ctrl; }

		void Calculate();

		virtual void Init() = 0;

		std::vector<Operator*> staticOperators;
		std::vector<SpectralOperator*> spectralOperators;
	};

}
