#include "stdafx.h"
#include "QDAnderson.h"
#include "DUpOperator.h"

#include "nrg.h"
#include "Options.h"



namespace NRG {

	QDAnderson::QDAnderson(int nrsteps, int maxSize, double Lambda)
		: NRGAlgorithm(nrsteps, maxSize, Lambda, 4)
	{
		startIteration = -1;
	}


	void QDAnderson::Init()
	{		
		const double B = theApp.options.B;
		const double U = theApp.options.U;
		const double eps = theApp.options.eps;
		double delta = theApp.options.delta;


		delta *= 0.5 * log(Lambda) * (Lambda + 1.) / (Lambda - 1.);

		t = sqrt(2. * delta / M_PI);
		

		hamiltonian.matrix = Eigen::MatrixXd::Zero(curMatrixSize, curMatrixSize);
		
		static const unsigned int ImpUp = 1;
		static const unsigned int ImpDown = 2;

		hamiltonian.matrix(ImpUp, ImpUp) = eps - 0.5 * B;
		hamiltonian.matrix(ImpDown, ImpDown) = eps + 0.5 * B;
		hamiltonian.matrix(ImpUp + ImpDown, ImpUp + ImpDown) = 2. * eps + U;
				
		
		// need this operator for the spectral function
		DUpOperator *up = new DUpOperator(curMatrixSize);
		up->matrix.adjointInPlace();

		/*
		FDownOperator down(curMatrixSize);
		down.matrix.adjointInPlace();
		up->matrix += down.matrix;
		*/

		spectralOperators.emplace_back(up);
	}
}
