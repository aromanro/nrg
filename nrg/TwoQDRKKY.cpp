#include "stdafx.h"
#include "TwoQDRKKY.h"
#include "DUpOperator.h"

#include "nrg.h"
#include "Options.h"


namespace NRG {

	TwoQDRKKY::TwoQDRKKY(int nrsteps, int maxSize, double Lambda)
		: NRGAlgorithm(nrsteps, maxSize, Lambda, 16)
	{
		startIteration = -1;
	}


	TwoQDRKKY::~TwoQDRKKY()
	{
	}

	void TwoQDRKKY::Init()
	{
		double B = theApp.options.B;
		double J = theApp.options.J;
		double U = theApp.options.U;
		double eps = theApp.options.eps;
		double delta = theApp.options.delta;

		delta *= 1 / 2. * log(Lambda) * (Lambda + 1.) / (Lambda - 1.);

		t = sqrt(2. * delta / M_PI);
				

		hamiltonian.matrix = Eigen::MatrixXd::Zero(curMatrixSize, curMatrixSize);

		Hamiltonian H;		
		H.matrix = Eigen::MatrixXd::Zero(4, 4);

		// first quantum dot

		static const unsigned int ImpUp = 1;
		static const unsigned int ImpDown = 2;
		static const unsigned int ImpUpDown = ImpUp + ImpDown;

		H.matrix(ImpUp, ImpUp) = eps - 1. / 2. * B;
		H.matrix(ImpDown, ImpDown) = eps + 1. / 2. * B;
		H.matrix(ImpUpDown, ImpUpDown) = (2 * eps + U);

		H.Extend();

		// add the second quantum dot
		
		static const unsigned int ImpUp2 = (1 << 2);
		static const unsigned int ImpDown2 = (2 << 2);
		static const unsigned int ImpUpDown2 = ImpUp2 + ImpDown2;

		Eigen::Matrix4d I = Eigen::Matrix4d::Identity();


		H.matrix.block(ImpUp2, ImpUp2, 4, 4) += (eps - 1. / 2. * B) * I;
		H.matrix.block(ImpDown2, ImpDown2, 4, 4) += (eps + 1. / 2. * B) * I;
		H.matrix.block(ImpUpDown2, ImpUpDown2, 4, 4) += (2 * eps + U) * I;


		// there are two quantum dots here, but they are not coupled yet
		
		// an easy check is to run the program without the coupling that follows and get the same results
		// as for a single quantum dot, since the other one is decoupled
		
		// the coupling terms:

		// on diagonal values
		H.matrix(ImpUp2 + ImpUp, ImpUp2 + ImpUp) += 1. / 4. * J;
		H.matrix(ImpDown2 + ImpDown, ImpDown2 + ImpDown) += 1. / 4. * J;

		H.matrix(ImpUp2 + ImpDown, ImpUp2 + ImpDown) -= 1. / 4. * J;
		H.matrix(ImpDown2 + ImpUp, ImpDown2 + ImpUp) -= 1. / 4. * J;

		// off diagonal values, S^+ * s^- and S^- * s^+ with the 1/2 factor
		
		H.matrix(ImpUp2 + ImpDown, ImpDown2 + ImpUp) = 1. / 2. * J;
		H.matrix(ImpDown2 + ImpUp, ImpUp2 + ImpDown) = 1. / 2. * J;
		

		hamiltonian.matrix = H.matrix;

		// need this operator for the spectral function
		DUpOperator *up = new DUpOperator(curMatrixSize);				
		up->matrix.adjointInPlace();		
		spectralOperators.push_back(up);		
	}

}

